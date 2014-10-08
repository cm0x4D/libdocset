#include "docsetobject.h"
#include "docsetobject_p.hpp"
#include <assert.h>
#include <iostream>
#include "docset.h"
#include <algorithm>
using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::search;
using std::shared_ptr;

const char *const DocsetObject_Type_Strings[] = {
    "Unknown",
    "All",
    "Annotation",
    "Attribute",
    "Binding",
    "Builtin",
    "Callback",
    "Category",
    "Class",
    "Command",
    "Component",
    "Constant",
    "Constructor",
    "Define",
    "Delegate",
    "Diagram",
    "Directive",
    "Element",
    "Entry",
    "Enum",
    "Environment",
    "Error",
    "Event",
    "Exception",
    "Extension",
    "Field",
    "File",
    "Filter",
    "Framework",
    "Function",
    "Global",
    "Guide",
    "Hook",
    "Instance",
    "Instruction",
    "Interface",
    "Keyword",
    "Library",
    "Literal",
    "Macro",
    "Method",
    "Mixin",
    "Modifier",
    "Module",
    "Namespace",
    "Notation",
    "Object",
    "Operator",
    "Option",
    "Package",
    "Parameter",
    "Plugin",
    "Procedure",
    "Property",
    "Protocol",
    "Provider",
    "Provisioner",
    "Query",
    "Record",
    "Resource",
    "Sample",
    "Section",
    "Service",
    "Setting",
    "Shortcut",
    "Specialization",
    "Statement",
    "Struct",
    "Style",
    "Subroutine",
    "Tag",
    "Test",
    "Trait",
    "Type",
    "Typedef",
    "Union",
    "Value",
    "Variable",
    "Word",
    nullptr
};

const pair<const char *const, const DocsetObject::Type> DocsetObject_Type_String_Aliases[] = {
    {"cl", DocsetObject::Type::Class},
    {"macro", DocsetObject::Type::Macro},
    {"clm", DocsetObject::Type::Method},
    {"func", DocsetObject::Type::Function},
    {"tdef", DocsetObject::Type::Typedef},
    {"clconst", DocsetObject::Type::Constant},
    {"specialization", DocsetObject::Type::Specialization},
    {"instp", DocsetObject::Type::Property},
    {"intf", DocsetObject::Type::Property},
    {nullptr, DocsetObject::Type::Unknown}
};

DocsetObject::DocsetObject(DocsetObjectPrivate *p): p(p) {
}

bool DocsetObject::isValid() const {
    return p && !p->name.empty() && !p->url.empty() && !p->docset.expired();
}

int DocsetObject::id() const {
    if (p) {
        return p->id;
    } else {
        return -1;
    }
}

string DocsetObject::name() const {
    if (p) {
        return p->name;
    } else {
        return "INVALID OBJECT";
    }
}

DocsetObject::Type DocsetObject::type() const {
    if (p) {
        return p->type;
    } else {
        return Type::Unknown;
    }
}

string DocsetObject::url() const {
    if (p) {
        if (!p->docset.expired()) {
            Docset d(p->docset.lock());
            return string("file://") + d.documentPath() + p->url;
        } else {
            return "INVALID DOCSET";
        }
    } else {
        return "INVALID OBJECT";
    }
}

bool DocsetObject::operator <(const DocsetObject &other) const {
    if (p && other.p) {
        return p->name < other.p->name;
    } else {
        return p != nullptr;
    }
}

int DocsetObject::positionInName(const string &what) const {
    if (!p) return -1;
    auto it = search(p->name.cbegin(), p->name.cend(), what.cbegin(), what.cend(),
                       [](const char &n, const char &w) -> bool {
                           return tolower(n) == tolower(w);
                       });
    if (it != p->name.cend())
        return it - p->name.cbegin();
    else
        return -1;
}

bool DocsetObject::wordEndsWith(const std::string &what, int indexHint) const
{
    if (indexHint == -1) {
        indexHint = positionInName(what);
    }
    return p && p->name.length() <= indexHint + what.length() || p->name.at(indexHint + what.length()) == ' ';
}

Docset DocsetObject::docset() const {
    if (!p || p->docset.expired())
        return Docset();
    else
        return Docset(p->docset.lock());
}

DocsetObject::Type DocsetObject::typeFromString(const std::string type) {
    for (int i = 0; DocsetObject_Type_Strings[i]; ++i)
        if (type == DocsetObject_Type_Strings[i])
            return static_cast<Type>(i);

    for (int i = 0; DocsetObject_Type_String_Aliases[i].first; ++i) {
        if (type == DocsetObject_Type_String_Aliases[i].first)
            return DocsetObject_Type_String_Aliases[i].second;
    }

    cout << "Hmm, I do not know the type " << type.c_str() << endl;
    return Type::Unknown;
}

string DocsetObject::stringFromType(Type type) {
    return DocsetObject_Type_Strings[static_cast<int>(type)];
}
