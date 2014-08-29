#pragma once
#include <string>
#include "docsetobject.h"
using std::string;

struct DocsetPrivate {
    virtual ~DocsetPrivate() {}

    virtual DocsetObjectList find(const string &what, DocsetObject::Type type) const = 0;
    virtual bool isValid() const = 0;

    inline DocsetObject createDocsetObject(DocsetObjectPrivate *p) const {
        return DocsetObject(p);
    }

    string name;
    string path;
};
