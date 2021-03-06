#pragma once
#include <string>
#include <list>
#include <memory>
class Docset;

struct DocsetObjectPrivate;

class DocsetObject {
    friend class Docset;
    friend struct DocsetPrivate;

public:
    enum class Type {
        Unknown = 0,            All,                    Annotation,                 Attribute,
        Binding,                Builtin,                Callback,                   Category,
        Class,                  Command,                Component,                  Constant,
        Constructor,            Define,                 Delegate,                   Diagram,
        Directive,              Element,                Entry,                      Enum,
        Environment,            Error,                  Event,                      Exception,
        Extension,              Field,                  File,                       Filter,
        Framework,              Function,               Global,                     Guide,
        Hook,                   Instance,               Instruction,                Interface,
        Keyword,                Library,                Literal,                    Macro,
        Method,                 Mixin,                  Modifier,                   Module,
        Namespace,              Notation,               Object,                     Operator,
        Option,                 Package,                Parameter,                  Plugin,
        Procedure,              Property,               Protocol,                   Provider,
        Provisioner,            Query,                  Record,                     Resource,
        Sample,                 Section,                Service,                    Setting,
        Shortcut,               Specialization,         Statement,                  Struct,
        Style,                  Subroutine,             Tag,                        Test,
        Trait,                  Type,                   Typedef,                    Union,
        Value,                  Variable,               Word,                       TypeEnd
    };

    DocsetObject(): p(nullptr) {}

    bool isValid() const;
    int id() const;
    std::string name() const;
    Type type() const;
    inline std::string typeString() const {
        return stringFromType(type());
    }
    std::string url() const;

    Docset docset() const;

    bool operator <(const DocsetObject &other) const;
    int positionInName(const std::string &what) const;
    inline bool containsInName(const std::string &what) const {
        return positionInName(what) != -1;
    }
    bool wordEndsWith(const std::string &what, int indexHint = -1) const;

    static Type typeFromString(const std::string type);
    static std::string stringFromType(Type type);

private:
    DocsetObject(DocsetObjectPrivate *p);
    std::shared_ptr<DocsetObjectPrivate> p;
};

typedef std::list<DocsetObject> DocsetObjectList;
