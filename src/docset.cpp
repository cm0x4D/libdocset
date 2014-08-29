#include "docset.h"
#include "docsetobject.h"
#include "docsetobject_p.hpp"
#include <sqlite3.h>
#include <functional>
#include <algorithm>
#include "docset_p.hpp"
using std::string;
using std::list;
using std::function;
using std::pair;
using std::find;

class AppleFormat: public DocsetPrivate {
public:
    DocsetObjectList find(const string &what, DocsetObject::Type type) const override {
        DocsetObjectList objects;

        auto handler = [](void *data, int argc, char **argv, char **) -> int {
            if (argc == 4) {
                auto param = static_cast<pair<DocsetPrivate *, DocsetObjectList *> *>(data);

                DocsetObjectPrivate *objectPriv = new DocsetObjectPrivate();
                objectPriv->id = atoi(argv[0]);
                objectPriv->name = argv[1];
                objectPriv->type = DocsetObject::typeFromString(argv[2]);
                objectPriv->url = param->first->path + "/Contents/Resources/Documents/" + argv[3];
                param->second->push_back(param->first->createDocsetObject(objectPriv));
            }
            return 0;
        };

        string query = "SELECT id, name, type, path FROM searchIndex where name Like '%";
        query += what + "%'";

        if (type != DocsetObject::Type::All)
            query += " AND type = '" + DocsetObject::stringFromType(type) + "'";

        auto param = make_pair(this, &objects);
        sqlite3_exec(index, query.c_str(), handler, &param, nullptr);

        return objects;
    }

    bool isValid() const {
        return index;
    }

    struct sqlite3 *index = nullptr;
 };

struct ZFormat: public DocsetPrivate {
    DocsetObjectList find(const string &what, DocsetObject::Type type) const override {
        DocsetObjectList objects;

        auto handler = [](void *data, int argc, char **argv, char **) -> int {
            if (argc == 4) {
                auto param = static_cast<pair<DocsetPrivate *, DocsetObjectList *> *>(data);

                DocsetObjectPrivate *objectPriv = new DocsetObjectPrivate();
                objectPriv->id = atoi(argv[0]);
                objectPriv->name = argv[1];
                objectPriv->type = DocsetObject::typeFromString(argv[2]);
                objectPriv->url = param->first->path + "/Contents/Resources/Documents/" + argv[3];
                param->second->push_back(param->first->createDocsetObject(objectPriv));
            }
            return 0;
        };

        string query = "SELECT ztoken.z_pk, ztokenname, ztypename, zpath FROM ztoken "
                       "JOIN ztokentype ON ztoken.ztokentype = ztokentype.z_pk "
                       "JOIN ztokenmetainformation ON ztoken.zmetainformation = ztokenmetainformation.z_pk "
                       "JOIN zfilepath ON ztokenmetainformation.zfile = zfilepath.z_pk "
                       "WHERE ztokenname LIKE '%";
        query += what + "%'";

        if (type != DocsetObject::Type::All)
            query += " AND ztypename = '" + DocsetObject::stringFromType(type) + "'";

        auto param = make_pair(this, &objects);
        sqlite3_exec(index, query.c_str(), handler, &param, nullptr);

        return objects;
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

DocsetObjectList Docset::find(const string &what, DocsetObject::Type type) const {
    DocsetObjectList objects;

    if (p && p->isValid()) {
        objects = p->find(what, type);
    }

    return objects;
}

Docset Docset::open(const string &path) {
    string indexPath = path + "/Contents/Resources/docSet.dsidx";
    sqlite3 *index = nullptr;

    // Open the SQLITE3 index file of the docset.
    if (sqlite3_open_v2(indexPath.c_str(), &index, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK) {
        return Docset();
    }

    // Determine the version by getting the list of all tables and instanciate the right implementation.
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
