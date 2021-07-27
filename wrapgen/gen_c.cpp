#include "wrapper_gen.h"

namespace OmegaWrapGen {


    class CGen final : public Gen {
        GenContext *ctxt;

        CGenSettings & settings;
    public:

        CGen(CGenSettings & settings):settings(settings){

        };

        void setContext(GenContext &ctxt) override {
            this->ctxt = &ctxt;
        };

        GenContext & getContext() override {
            return *ctxt;
        };


        void consumeDecl(DeclNode *node) override {

        };


        void finish() override {

        };

    };

    Gen *Gen::CreateCGen(CGenSettings & settings){
        return new CGen(settings);
    };
};