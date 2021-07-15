#include "../lexer.h"
#include <iostream>

#include <omega-common/common.h>

using namespace OmegaWrapGen;

int main(int argc,char *argv[]){

    OmegaCommon::TStrRef flag {argv[1]};

    std::ifstream in(flag.data());
    DiagnosticBuffer buffer;
    Lexer lex(buffer);
    lex.setInputStream(&in);
    Tok t;
    for(;t.type != TOK_EOF && !buffer.hasErrored();t = lex.nextTok()){
        std::cout << "TOK: { '" << t.content << "' }\n" << std::endl;
    };
    if(buffer.hasErrored())
        buffer.logAll();
    
    lex.finish();

    return 0;
};