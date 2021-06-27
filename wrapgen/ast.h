#include "nodes.def"
#include <vector>
#include <string>

#include "omega-common/common.h"

#ifndef OMEGA_WRAPGEN_AST_H
#define OMEGA_WRAPGEN_AST_H

namespace OmegaWrapGen {

    struct TreeScope {
        typedef enum : int {
            Class,
            Func,
            Interface,
            Namespace
        } Ty;
        Ty type;
        OmegaCommon::String name;
        TreeScope *parentScope = nullptr;
    };

    extern TreeScope *GLOBAL_SCOPE;

    struct DeclNode {
        TreeNodeType type;
        TreeScope * scope;
    };

    class Type {
        OmegaCommon::String name;
    public:
        static Type * Create(OmegaCommon::TStrRef name);
        OmegaCommon::TStrRef getName();
    };

    #define _STANDARD_TYPE(name) extern Type *name

    namespace stdtypes {

        _STANDARD_TYPE(VOID);
        _STANDARD_TYPE(INT);
        _STANDARD_TYPE(FLOAT);
        _STANDARD_TYPE(LONG);
        _STANDARD_TYPE(DOUBLE);
        _STANDARD_TYPE(CHAR);
        /// ADT

        _STANDARD_TYPE(STRING);
        _STANDARD_TYPE(VECTOR);
        _STANDARD_TYPE(MAP);
    };

    struct FuncDeclNode : public DeclNode {
        OmegaCommon::String name;
        bool isStatic = false;
        OmegaCommon::MapVec<OmegaCommon::String,Type *> params;
        Type *returnType;
    };

    struct ClassDeclNode : public DeclNode {
        OmegaCommon::String name;
        OmegaCommon::Vector<FuncDeclNode *> instMethods;
        OmegaCommon::Vector<FuncDeclNode *> staticMethods;
    };

    struct InterfaceDeclNode : public DeclNode {
        OmegaCommon::String name;
        OmegaCommon::Vector<FuncDeclNode *> instMethods;
    };

};

#endif