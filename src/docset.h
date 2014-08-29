#pragma once
#include "docsetobject.h"
#include <string>
#include <memory>

struct DocsetPrivate;

class Docset {
public:
    inline bool operator ==(const Docset &other) const {
        return p == other.p;
    }
    bool operator <(const Docset &other) const;

    bool isValid() const;
    std::string name() const;
    std::string documentPath() const;

    DocsetObjectList find(const std::string &what, DocsetObject::Type type = DocsetObject::Type::All) const;

    static Docset open(const std::string &path);

private:
    Docset(DocsetPrivate *p = nullptr);
    std::shared_ptr<DocsetPrivate> p;
};
