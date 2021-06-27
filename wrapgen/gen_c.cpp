#include "wrapper_gen.h"

namespace OmegaWrapGen {


    class CGen final : public Gen {

        void setContext(GenContext &ctxt) override {

        };



        void genDecl(DeclNode *node) override {

        };


        void finish() override {

        };

    };

    Gen *Gen::CreateCGen(){
        return new CGen();
    };
};