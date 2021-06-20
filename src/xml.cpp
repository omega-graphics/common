#include "omega-common/xml.h"
#include <iostream>

namespace OmegaCommon {

    struct XMLTok {
        typedef enum : int  {
            LCarrot,
            RCarrot,
            LCarrotSl,
            RCarrotSl,
            Identifier,
            StrLiteral,
            EndOfFile,
            NullTok
        } Type;
        Type type;
        String str;
    };

    class XMLLexer {
        std::istream * is;
    public:
        void setInputStream(std::istream * is){
            this->is = is;
        };
        bool tokenizeToBuffer(char *bufferSt,char *bufferEnd,size_t * len,XMLTok::Type * type){
            auto getChar = [&](){
                char c = is->get();
                return c;
            };

            auto aheadChar = [&](){
                char c = is->get();
                is->seekg(-1,std::ios::cur);
                return c;
            };

            #define INC_TO_TOK() is->seekg(1,std::ios::cur);

            #define PUSH_CHAR(c) *bufferEnd = c;++bufferEnd;
            #define PUSH_TOK(t) *type = t;*len = bufferEnd - bufferSt;return true;

            char c;
            while((c = getChar()) != -1){
                switch (c) {
                    case '<':
                    {
                        PUSH_CHAR(c)
                        if(c == '/'){
                            INC_TO_TOK()
                            PUSH_CHAR(c)
                            PUSH_TOK(XMLTok::LCarrotSl)
                        };
                        PUSH_TOK(XMLTok::LCarrot)
                        break;
                    }
                    
                    case '>':
                    {
                        PUSH_CHAR(c)
                        PUSH_TOK(XMLTok::RCarrot)
                        break;
                    }

                    case '/':
                    {
                        PUSH_CHAR(c)
                        c = aheadChar();
                        if(c == '>'){
                            INC_TO_TOK()
                            PUSH_CHAR(c)
                            PUSH_TOK(XMLTok::RCarrotSl)
                        };
                        break;
                    }
                    default: {
                        if(isalpha(c)){
                            PUSH_CHAR(c)
                            c = aheadChar();
                            if(!isalpha(c)){
                                INC_TO_TOK()
                                PUSH_CHAR(c)
                                PUSH_TOK(XMLTok::Identifier)
                            };
                        }
                        else {
                            std::cerr << "Unexpected token: " << c << std::endl;
                            return false;
                        }
                        break;
                    }
                }
            };

            PUSH_CHAR(c)
            PUSH_TOK(XMLTok::EndOfFile)

            
        };
        XMLTok nextToken(){
            char buffer[200];
            XMLTok::Type t;
            size_t len;
            auto rc = tokenizeToBuffer(buffer,buffer,&len,&t);
            if(!rc){
                t = XMLTok::NullTok;
                return {t};
            };
            XMLTok tok;
            tok.type = t;
            tok.str.resize(len);
            std::move(buffer,buffer + len,tok.str.begin());
            return tok;
        };
    };

    class XMLParser {
        static std::unique_ptr<XMLLexer> lexer;
    public:
        void setInputStream(std::istream * is);
        XML parseToJSON(){

        };
        XML parse(){
            return parseToJSON();
        };
    };

    std::unique_ptr<XMLLexer> XMLParser::lexer = std::make_unique<XMLLexer>();

    std::unique_ptr<XMLParser> XML::parser = std::make_unique<XMLParser>();
};