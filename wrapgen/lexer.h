#include <string>
#include <vector>
#include <istream>

#include "toks.def"

#ifndef OMEGA_WRAPGEN_LEXER_H
#define  OMEGA_WRAPGEN_LEXER_H

namespace OmegaWrapGen {

    struct Tok {
        TokTy type;
        std::string content;
    };


    class Lexer final {
         std::istream * is;
    public:
        void setInputStream(std::istream * is);
        Tok nextTok();
        void finish();
    };
};

#endif