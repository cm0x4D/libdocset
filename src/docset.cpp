#include "docset.h"
#include "docsetobject.h"
#include "docsetobject_p.hpp"
#include <sqlite3.h>
#include <functional>
#include <algorithm>
#include "docset_p.hpp"
#include <fstream>
#include "docsetsort.hpp"
using std::string;
using std::list;
using std::function;
using std::pair;
using std::find;
using std::ifstream;
using std::copy;
using std::tolower;
using std::size_t;

class AppleFormat: public DocsetPrivate {
public:
    void find(const string &what, DocsetObjectList &objects) const override {
        string query = "SELECT id, name, type, path FROM searchIndex where name Like '%";
        query += what + "%'";

        sqlite3_exec(index, query.c_str(),
            [](void *data, int argc, char **argv, char **) -> int {
                DocsetObjectPrivate *object = new DocsetObjectPrivate();
                object->id = atoi(argv[0]);
                object->name = argv[1];
                object->type = DocsetObject::typeFromString(argv[2]);
                object->url = argv[3];
                static_cast<DocsetObjectList *>(data)->push_back(DocsetPrivate::createDocsetObject(object));
                return 0;
            }, &objects, nullptr);
    }

    bool isValid() const {
        return index;
    }

    struct sqlite3 *index = nullptr;
 };

struct ZFormat: public DocsetPrivate {
    void find(const string &what, DocsetObjectList &objects) const override {
        string query = "SELECT ztoken.z_pk, ztokenname, ztypename, zpath, zanchor FROM ztoken "
                       "JOIN ztokentype ON ztoken.ztokentype = ztokentype.z_pk "
                       "JOIN ztokenmetainformation ON ztoken.zmetainformation = ztokenmetainformation.z_pk "
                       "JOIN zfilepath ON ztokenmetainformation.zfile = zfilepath.z_pk "
                       "WHERE ztokenname LIKE '%";
        query += what + "%'";

        sqlite3_exec(index, query.c_str(),
            [](void *data, int argc, char **argv, char **) -> int {
                DocsetObjectPrivate *object = new DocsetObjectPrivate();
                object->id = atoi(argv[0]);
                object->name = argv[1];
                object->type = DocsetObject::typeFromString(argv[2]);
                object->url = argv[3];
                if (argc >= 5 && argv[4]) {
                    object->url += '#';
                    object->url += argv[4];
                }
                static_cast<DocsetObjectList *>(data)->push_back(DocsetPrivate::createDocsetObject(object));
                return 0;
            }, &objects, nullptr);
    }

    bool isValid() const {
        return index;
    }

    struct sqlite3 *index = nullptr;
};

bool Docset::operator <(const Docset &other) const {
    if (p && other.p)
        return p->name < other.p->name;
    else
        return p < other.p;
}

bool Docset::isValid() const {
    return p && p->isValid();
}

string Docset::name() const {
    if (p)
        return p->name;
    else
        return "INVALID DOCSET";
}

string Docset::documentPath() const {
    if (p)
        return p->path + "/Contents/Resources/Documents/";
    else
        return "INVALID DOCSET";
}

string Docset::xmlDescription() {
    if (p) {
        if (p->xmlDescription.empty()) {
            ifstream info(p->path + "/Contents/Info.plist");
            copy(std::istreambuf_iterator<char>(info), std::istreambuf_iterator<char>(),
                 std::back_inserter(p->xmlDescription));
        }
        return p->xmlDescription;
    } else
        return "INVALID DOCSET";
}

DocsetObjectList Docset::find(const string &what, bool sorted) const {
    DocsetObjectList objects;

    if (p && p->isValid()) {
        if (p->inMemory) {
            for (auto &entry: p->objects)
                for (auto &object: entry.second) {
                    if (object.containsInName(what)) {
                        objects.push_back(object);
                        }
                    }
        } else {
            p->find(what, objects);
            for (auto object: objects) {
                object.p->docset = p;
            }
        }
    }

    if (sorted)
        objects.sort(DocsetIntelligentSort(what));

    return objects;
}

DocsetObjectList Docset::objects(DocsetObject::Type type) const {
    if (p && p->isValid()) {
        if (!p->inMemory)
            loadToMemory();
        return p->objects[type];
    } else
        return DocsetObjectList();
}

size_t Docset::loadToMemory() const {
     if (p && p->isValid() && !p->inMemory) {
         DocsetObjectList objects = find("");
         for (auto &object: objects)
             p->objects[object.type()].push_back(object);
         p->inMemory = true;
         return objects.size();
     }
     return 0;
}

void Docset::unloadFromMemory() const {
    if (p && p->isValid() && p->inMemory) {
        p->objects.clear();
        p->inMemory = false;
    }
}

Docset Docset::open(const string &path) {
    string indexPath = path + "/Contents/Resources/docSet.dsidx";
    sqlite3 *index = nullptr;

    // Open the SQLITE3 index file of the docset.
    if (sqlite3_open_v2(indexPath.c_str(), &index, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK) {
        return Docset();
    }

    // Determine the version by getting the list of all tables and instanciate the correct implementation.
    list<string> tables;
    DocsetPrivate *p;
    sqlite3_exec(index, "SELECT name FROM sqlite_master WHERE type='table'",
                    [](void *data, int argc, char **argv, char **) -> int {
                        if (argc == 1) {
                            list<string> *tables = static_cast<list<string> *>(data);
                            tables->push_back(argv[0]);
                        }
                            return 0;
                    }, &tables, nullptr);
    if (std::find(tables.begin(), tables.end(), "searchIndex") != tables.end()) {
        AppleFormat *a = new AppleFormat();
        a->index = index;
        p = a;
    } else {
        ZFormat *z = new ZFormat();
        z->index = index;
        p = z;
    }

    int from = path.rfind("/"); int to = path.rfind(".docset");
    p->name = path.substr(from + 1, to - from - 1);
    p->path = path;

    return Docset(p);
}

Docset::Docset(DocsetPrivate *p):p(p) {
}
