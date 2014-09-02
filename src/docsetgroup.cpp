#include "docsetgroup.h"
#include <list>
#include <assert.h>
#include <future>
#include <functional>
#ifdef WINDOWS
#   include <windows.h>
#else
#   include <dirent.h>
#endif
#include <algorithm>
#include <iterator>
#include <cstring>
using std::list;
using std::future;
using std::async;
using std::bind;
using std::size_t;
using std::string;
using std::move;
using std::back_inserter;
using std::size_t;

struct DocsetGroupPrivate {
    list<Docset> docsets;

#   ifdef WINDOWS
    // TODO...
#   else
    static void open(DocsetGroup &docsetGroup, const std::string folder, bool recursive) {
        DIR *directory = opendir(folder.c_str());
        struct dirent *entry;
        while ((entry = readdir(directory))) {
            if (entry->d_type == DT_DIR) {
                if (string(entry->d_name).find(".docset") != string::npos) {
                    Docset docset = Docset::open(folder + "/" + entry->d_name);
                    if (docset.isValid()) {
                        docsetGroup << docset;
                    }
                } else if (recursive && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                    open(docsetGroup, folder + "/" + entry->d_name, true);
                }
            }
        }
        closedir(directory);
    }
#   endif
};

DocsetGroup::DocsetGroup(): p(new DocsetGroupPrivate()) {
    assert(p);
}

DocsetGroup::DocsetGroup(const DocsetGroup &other): p(new DocsetGroupPrivate()) {
    assert(p && other.p);
    *p = *other.p;
}

const DocsetGroup &DocsetGroup::operator =(const DocsetGroup &other) {
    assert(p && other.p);
    if (this != &other) {
        *p = *other.p;
    }

    return *this;
}

DocsetGroup::~DocsetGroup() {
    assert(p);
    delete p;
}

size_t DocsetGroup::count() const {
    assert(p);
    return p->docsets.size();
}

void DocsetGroup::add(const Docset &docset) {
    assert(p);
    p->docsets.push_back(docset);
}

void DocsetGroup::remove(const Docset &docset) {
    assert(p);
    p->docsets.remove(docset);
}

Docset DocsetGroup::docset(const std::string docsetName) const {
    assert(p);
    auto docset = std::find_if(p->docsets.cbegin(), p->docsets.cend(),
                               [&](const Docset &docset){ return docset.name() == docsetName; });
    if (docset != p->docsets.cend())
        return *docset;
    else
        return Docset();

}

Docset DocsetGroup::docset(int index) const {
    if (index < p->docsets.size()) {
        auto docset = p->docsets.cbegin();
        for (int i = 0; i < index; ++i)
            ++docset;
        return *docset;
    } else
        return Docset();
}

DocsetObjectList DocsetGroup::find(const std::string &what) const {
    assert(p);

    list<future<list<DocsetObject>>> futures;
    for (auto &docset: p->docsets) {
        futures.push_back(async(std::launch::async, bind(&Docset::find, &docset, what)));
    }

    list<DocsetObject> objects;
    for (auto &f: futures) {
        auto result = f.get();
        move(result.begin(), result.end(), back_inserter(objects));
    }

    objects.sort();

    return objects;
}

std::size_t DocsetGroup::loadToMemory() const {
    assert(p);

    std::size_t count = 0;
    list<future<size_t>> futures;
    for (auto &docset: p->docsets) {
        futures.push_back(async(std::launch::async, bind(&Docset::loadToMemory, &docset)));
    }
    for (auto &f: futures)
        count += f.get();

    return count;
}

void DocsetGroup::unloadFromMemory() const {
    assert(p);

    list<future<void>> futures;
    for (auto &docset: p->docsets) {
        futures.push_back(async(std::launch::async, bind(&Docset::unloadFromMemory, &docset)));
    }

    for (auto &f: futures) f.get();
}

DocsetGroup DocsetGroup::open(const std::string folder, bool recursive) {
    DocsetGroup docsetGroup;
    DocsetGroupPrivate::open(docsetGroup, folder, recursive);
    return docsetGroup;
}
