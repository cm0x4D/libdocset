#pragma once
#include "docsetobject.h"
#include <string>
#include <memory>

struct DocsetPrivate;

class Docset {
    friend class DocsetGroup;
    friend class DocsetObject;

public:
    Docset(): p(nullptr) {}

    inline bool operator ==(const Docset &other) const {
        return p == other.p;
    }
    bool operator <(const Docset &other) const;

    bool isValid() const;
    std::string name() const;
    std::string documentPath() const;
    std::string xmlDescription();

    DocsetObjectList find(const std::string &what, bool sorted = true) const;

    DocsetObjectList objects(DocsetObject::Type type) const;
    inline DocsetObjectList operator [](DocsetObject::Type index) const {
        return objects(index);
    }

    std::size_t loadToMemory() const;
    void unloadFromMemory() const;

    static Docset open(const std::string &path);

private:
    Docset(DocsetPrivate *p);
    inline Docset(std::shared_ptr<DocsetPrivate> p): p(p) {}
    std::shared_ptr<DocsetPrivate> p;
};
