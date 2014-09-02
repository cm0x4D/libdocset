#include "docsetobject.h"
#include "docsetobject_p.hpp"
#include <assert.h>
#include <iostream>
#include "docset.h"
using std::string;
using std::cout;
using std::endl;
using std::pair;

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

bool DocsetObject::operator <(const DocsetObject &other) const {
    assert(p);
    assert(other.p);
    return p->name < other.p->name;
}

bool DocsetObject::isValid() const {
    assert(p);
    return !p->name.empty() && !p->url.empty() && !p->docset.expired();
}

int DocsetObject::id() const {
    assert(p);
    return p->id;
}

string DocsetObject::name() const {
    assert(p);
    return p->name;
}

DocsetObject::Type DocsetObject::type() const {
    assert(p);
    return p->type;
}

string DocsetObject::url() const {
    assert(p);
    return p->url;
}

Docset DocsetObject::docset() const {
    assert(p);
    if (p->docset.expired())
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
