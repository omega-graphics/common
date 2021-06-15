#include "omega-common/json.h"
#include <istream>
#include <cctype>
#include <sstream>
#include <iostream>

namespace OmegaCommon {
    
    struct JSONTok {
        typedef enum : int {
            StrLiteral,
            LBrace,
            RBrace,
            LBracket,
            RBracket,
            Comma,
            Colon,
            BoolLiteral,
            NumLiteral,
            Eof
        } TokType;
        TokType type;
        String str;
    };

    class JSONLexer {
        std::istream * in = nullptr;
    public:
        void setInputStream(std::istream * in){
            this->in = in;
        };
        
        void lexToBuffer(char *bufferStart,char *bufferEnd,size_t * len,JSONTok::TokType * type){
            
            auto getChar = [&]() -> char{
                return in->get();
            };

            auto aheadChar = [&]() -> char {
                auto ch = in->get();
                in->seekg(-1,std::ios::cur);
                return ch;
            };

            #define PUSH_CHAR(c) *bufferEnd = c;++bufferEnd;
            #define PUSH_TOK(t) *type = t;*len = bufferEnd - bufferStart;return;
            char c;
            while((c = getChar()) != -1){
                switch(c){
                    case '\0':{
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::Eof)
                        break;
                    }
                    case '"':{
                        PUSH_CHAR(c)
                        while((c = getChar()) != '"'){
                            PUSH_CHAR(c)
                        };
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::StrLiteral)
                        break;
                    }
                    case ':' : {
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::Colon)
                        break;
                    }
                    case ',' : {
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::Comma)
                        break;
                    }
                    case '[' : {
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::LBracket)
                        break;
                    }
                    case ']': {
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::RBracket)
                        break;
                    }
                    case '{': {
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::RBrace)
                        break;
                    }
                    case '}': {
                        PUSH_CHAR(c)
                        PUSH_TOK(JSONTok::LBrace)
                        break;
                    }
                    default:{
                        break;
                    }
                };
            };

            PUSH_TOK(JSONTok::Eof)


        };


        JSONTok nextTok(){
            char buffer[200];
            JSONTok tok;
            size_t len;
            lexToBuffer(buffer,buffer,&len,&tok.type);
            tok.str.resize(len);
            std::move(buffer,buffer + len,tok.str.begin());
            return tok;
        };
        void finish(){
            in = nullptr;
        };
    };

    #define JSON_STR 0x00
    #define JSON_ARRAY 0x01
    #define JSON_MAP 0x02
    #define JSON_BOOL 0x03
    #define JSON_NUM 0x04

    #define JSON_ERROR(message) std::cerr << "JSON Parse Error:" << message << std::endl;exit(1);

    class JSONParser {
        std::unique_ptr<JSONLexer> lexer;
    public:
        void setInputStream(std::istream * in){
            lexer->setInputStream(in);
        };

        JSON parseToJSON(JSONTok & firstTok){
            JSON j;
            /// Map
            if(firstTok.type == JSONTok::LBrace){
                firstTok = lexer->nextTok();
                j.ty = JSON_MAP;
                auto data = new Map<String,JSON>();
                while(firstTok.type != JSONTok::RBrace){
                    if(firstTok.type != JSONTok::StrLiteral)
                        JSON_ERROR("Expected a StrLiteral")
                    auto key = firstTok.str;
                    if(lexer->nextTok().type != JSONTok::Colon)
                        JSON_ERROR("Expected a Colon!")
                    
                    firstTok = lexer->nextTok();
                    auto val = parseToJSON(firstTok);
                    data->insert(std::make_pair(key,val));

                    firstTok = lexer->nextTok();
                    if(firstTok.type != JSONTok::Comma && firstTok.type != JSONTok::RBrace)
                        JSON_ERROR("Expected Comma!")
                    if(firstTok.type == JSONTok::Comma)
                        firstTok = lexer->nextTok();
                };
                j._data = data;
            }
            /// Vector
            else if(firstTok.type == JSONTok::LBracket){
                firstTok = lexer->nextTok();
                auto data = new Vector<JSON>();
                j.ty = JSON_ARRAY;
                while(firstTok.type != JSONTok::RBracket){
                    data->push_back(parseToJSON(firstTok));
                    firstTok = lexer->nextTok();
                    if(firstTok.type != JSONTok::Comma && firstTok.type != JSONTok::RBracket)
                        JSON_ERROR("Expected Comma!")
                    if(firstTok.type == JSONTok::Comma)
                        firstTok = lexer->nextTok();
                };
                j._data = data;
            }
            /// String
            else if(firstTok.type == JSONTok::StrLiteral){
                j.ty = JSON_STR;
                j._data = new String(firstTok.str);
            }

            /// Number

            /// Boolean
            return j;
        };

        JSON parse(){
            JSONTok firstTok = lexer->nextTok();
            return parseToJSON(firstTok);
        };

        void finish(){

        };
    };



    class JSONSerializer {

    };

    std::unique_ptr<JSONParser> JSON::parser = std::make_unique<JSONParser>();

    JSON JSON::parse(String str){
        std::istringstream in(str);
        parser->setInputStream(&in);
        return parser->parse();
    };


};