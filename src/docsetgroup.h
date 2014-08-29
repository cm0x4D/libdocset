#pragma once
#include "docset.h"

struct DocsetGroupPrivate;

class DocsetGroup
{
public:
    DocsetGroup();
    DocsetGroup(const DocsetGroup &other);
    const DocsetGroup &operator =(const DocsetGroup &other);
    ~DocsetGroup();

    std::size_t count() const;
    inline std::size_t size() const {
        return count();
    }

    void add(const Docset &docset);
    void remove(const Docset &docset);
    inline DocsetGroup &operator <<(const Docset &docset) {
        add(docset);
        return *this;
    }

    DocsetObjectList find(const std::string &what, DocsetObject::Type type = DocsetObject::Type::All) const;

    static DocsetGroup open(const std::string folder, bool recursive = false);

private:
    DocsetGroupPrivate *p;
};
