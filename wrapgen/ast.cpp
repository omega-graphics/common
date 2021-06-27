#include "ast.h"

namespace OmegaWrapGen {

    TreeScope *GLOBAL_SCOPE = new TreeScope {TreeScope::Namespace,"__GLOBAL__"};

      namespace stdtypes {

        Type * VOID = Type::Create("void");
        Type * INT = Type::Create("int");
        Type * FLOAT = Type::Create("float");
        Type * LONG = Type::Create("long");
        Type * DOUBLE = Type::Create("double");
        Type * CHAR = Type::Create("char");
        /// ADT

        Type *STRING = Type::Create("string");
        Type *VECTOR = Type::Create("vector");
        Type *MAP = Type::Create("map");
    };


    

    Type * Type::Create(OmegaCommon::TStrRef name){
        auto t = new Type();
        t->name = name.data();
        return t;
    };

    OmegaCommon::TStrRef Type::getName(){
        return name;
    };
};