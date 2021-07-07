#include "wrapper_gen.h"

namespace OmegaWrapGen {


    class CGen final : public Gen {
        GenContext *ctxt;
    

        void setContext(GenContext &ctxt) override {
            this->ctxt = &ctxt;
        };

        GenContext & getContext() override {
            return *ctxt;
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