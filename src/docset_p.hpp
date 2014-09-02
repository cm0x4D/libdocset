#pragma once
#include <string>
#include "docsetobject.h"
#include <map>
#include <list>
using std::string;
using std::map;

struct DocsetPrivate {
    virtual ~DocsetPrivate() {}

    virtual void find(const string &what, DocsetObjectList &objects) const = 0;
    virtual bool isValid() const = 0;

    static inline DocsetObject createDocsetObject(DocsetObjectPrivate *p) {
        return DocsetObject(p);
    }

    string name;
    string path;
    string xmlDescription;

    bool inMemory = false;
    map<DocsetObject::Type, DocsetObjectList> objects;
};
