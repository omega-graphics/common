#include <memory>
#include "ast.h"
#include "wrapper_gen.h"
#include "lexer.h"
#include "diagnostics.h"

#ifndef OMEGA_WRAPGEN_PARSER_H
#define OMEGA_WRAPGEN_PARSER_H


namespace OmegaWrapGen {


    class Parser final {
        std::unique_ptr<Lexer> lexer;
        Gen * gen;
        std::unique_ptr<DiagnosticBuffer> errStream;
        DeclNode *nextDecl();
    public:
        Parser(Gen * gen);
         void setInputStream(std::istream * is);
         void beginParse();
         void finish();
    };


};

#endif