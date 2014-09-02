#pragma once
#include "docsetobject.h"
#include <string>
#include <assert.h>
#include "docset_p.hpp"
#include <memory>

struct DocsetObjectPrivate {
    int id;
    std::string name;
    DocsetObject::Type type;
    std::string url;
    std::weak_ptr<DocsetPrivate> docset;
};
