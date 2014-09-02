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
    Docset docset(const std::string docsetName) const;
    inline Docset operator [](std::string index) const {
        return docset(index);
    }
    Docset docset(int index) const;
    inline Docset operator[](int index) const {
        return docset(index);
    }

    DocsetObjectList find(const std::string &what) const;

    void loadToMemory() const;
    void unloadFromMemory() const;

    static DocsetGroup open(const std::string folder, bool recursive = false);

private:
    DocsetGroupPrivate *p;
};
