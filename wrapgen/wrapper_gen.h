#include "ast.h"
#include <memory>

#ifndef OMEGA_WRAPGEN_WRAPPER_GEN_H
#define OMEGA_WRAPGEN_WRAPPER_GEN_H

namespace OmegaWrapGen {

    struct GenContext {
        OmegaCommon::String header;
        OmegaCommon::TStrRef output_dir;
    };

    struct CGenSettings {
        OmegaCommon::TStrRef headerName;
    };


    class Gen : public TreeConsumer {
        friend class Parser;
    public:
        using TreeConsumer::consumeDecl;
        virtual void setContext(GenContext & ctxt) = 0;
        virtual GenContext & getContext() = 0;
        virtual void finish() = 0;

        static Gen *CreateCGen(CGenSettings & settings);
    };

};

#endif