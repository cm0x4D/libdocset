#pragma once
#include "docsetobject.h"
#include <string>

inline bool DocsetAlphabeticalSort(const DocsetObject &l, const DocsetObject &r) {
    return l < r;
}

class DocsetIntelligentSort {
public:
    DocsetIntelligentSort(const std::string &what): _what(what) {}

    bool operator() (const DocsetObject &l, const DocsetObject &r) {
        if (_what.empty()) return DocsetAlphabeticalSort(l, r);

        int li = l.positionInName(_what);
        int ri = r.positionInName(_what);
        if (ri == li) {
            if (ri == -1) {
                return l < r;
            } else {
                bool lws = l.wordEndsWith(_what, li);
                bool rws = r.wordEndsWith(_what, ri);
                if (lws && !rws) return true;
                if (rws && !lws) return false;
                return l.name() < r.name();
            }
        } else {
            return ri == -1 || li < ri;
        }
    }

private:
    std::string _what;
};
