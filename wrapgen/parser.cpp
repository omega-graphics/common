#include "parser.h"
#include <memory>

namespace OmegaWrapGen {

    /// Tree Builder

    class TreeBuilder {
        Lexer *lexer;
        DiagnosticBuffer & errStream;
        Type *buildType(Tok & first_tok);
        DeclNode *buildDecl(Tok & first_tok,TreeScope *parentScope);
    public:
        Tok currentTok;

        TreeBuilder(Lexer *lexer,DiagnosticBuffer & errStream);
        DeclNode *nextDecl(bool *hasErrored);
    };

    struct ParserError : public Diagnostic {
        OmegaCommon::String message;
        void format(std::ostream &out) override {
            out << "\x1b[31mPARSE ERROR\x1b[0m:" << message << std::flush;
        };
        bool isError() override {
            return true;
        };
        ParserError(const char *message):message(message){

        };
    };


    TreeBuilder::TreeBuilder(Lexer *lexer,DiagnosticBuffer & errStream):lexer(lexer),errStream(errStream){

    };

    #define PARSER_ERROR_PUSH(m) errStream.push(new ParserError(m));
    #define ERROR_RETURN return nullptr
    #define EXPECTED_ID() PARSER_ERROR_PUSH("Expected Identifier") ERROR_RETURN
    #define EXPECTED_KW() PARSER_ERROR_PUSH("Expected Keyword") ERROR_RETURN
    #define EXPECTED_LBRACE() PARSER_ERROR_PUSH("Expected LBrace") ERROR_RETURN
    #define EXPECTED_RBRACE() PARSER_ERROR_PUSH("Expected RBrace") ERROR_RETURN
    #define EXPECTED_LPAREN() PARSER_ERROR_PUSH("Expected LParen") ERROR_RETURN
    #define EXPECTED_RPAREN() PARSER_ERROR_PUSH("Expected RParen") ERROR_RETURN
    #define EXPECTED_COLON() PARSER_ERROR_PUSH("Expected Colon") ERROR_RETURN



    Type *TreeBuilder::buildType(Tok & first_tok){
        if(first_tok.type != TOK_ID){
            EXPECTED_ID();
        };
        auto t = Type::Create(first_tok.content);
        return t;
    };

    DeclNode * TreeBuilder::buildDecl(Tok &first_tok,TreeScope *parentScope){
        if(first_tok.type == TOK_KW){
            DeclNode *node;
            if(first_tok.content == KW_CLASS){
                ClassDeclNode *class_node = new ClassDeclNode();
                class_node->type = CLASS_DECL;
                first_tok = lexer->nextTok();
                if(first_tok.type != TOK_ID){
                    EXPECTED_ID();
                };
                class_node->name = first_tok.content;
                first_tok = lexer->nextTok();

                if(first_tok.type != TOK_LBRACE){
                    EXPECTED_LBRACE();
                };

                first_tok = lexer->nextTok();
                while(first_tok.type != TOK_RBRACE){
                    auto class_scope = new TreeScope {TreeScope::Class,class_node->name,parentScope};
                    auto child_node = buildDecl(first_tok,class_scope);
                    if(child_node->type == FUNC_DECL){
                        FuncDeclNode *f = (FuncDeclNode *)child_node;
                        if(f->isStatic)
                            class_node->staticMethods.push_back(f);
                        else 
                            class_node->instMethods.push_back(f);
                    };
                    first_tok = lexer->nextTok();
                };
                node = class_node;
            }
            else if(first_tok.content == KW_FUNC){
                FuncDeclNode *func_decl = new FuncDeclNode();
                func_decl->type = FUNC_DECL;

                first_tok = lexer->nextTok();
                if(first_tok.type != TOK_ID){
                    EXPECTED_ID();
                };
                func_decl->name = first_tok.content;
                first_tok = lexer->nextTok();
                if(first_tok.type != TOK_LPAREN){
                    EXPECTED_LPAREN();
                };
                first_tok = lexer->nextTok();
                while(first_tok.type != TOK_RPAREN){

                    if(first_tok.type != TOK_ID){
                        EXPECTED_ID();
                    };
                    OmegaCommon::String name = first_tok.content;
                    first_tok = lexer->nextTok();

                    if(first_tok.type != TOK_COLON){
                        EXPECTED_COLON();
                    };

                    first_tok = lexer->nextTok();
                    auto _arg_ty = buildType(first_tok);

                    if(!_arg_ty)
                        ERROR_RETURN;

                    func_decl->params.insert(std::make_pair(name,_arg_ty));
                };

                first_tok = lexer->nextTok();
                auto _ty = buildType(first_tok);
                if(!_ty){
                    ERROR_RETURN;
                };
                func_decl->returnType = _ty;

                node = func_decl;
            };
            node->scope = parentScope;
            return node;
        }
        else {
            /// Throw Error
            EXPECTED_KW();
        };
    };

    DeclNode * TreeBuilder::nextDecl(bool *hasErrored){
        *hasErrored = false;

        currentTok = lexer->nextTok();
        if(currentTok.type == TOK_EOF){
            return nullptr;
        };
        auto res = buildDecl(currentTok,GLOBAL_SCOPE);
        if(!res)
            *hasErrored = true;
        return res;
    };




    /// Tree Semantics

    struct SemanticsSymTable {
        struct Symbol {
            typedef enum : int {
                Class,
                Interface,
                Func,
                Namespace
            } Ty;
            OmegaCommon::String name;
        };
        OmegaCommon::MapVec<Symbol,TreeScope *> symbol_map;
    }; 

    struct TreeSemanticsContext {
        std::unique_ptr<SemanticsSymTable> symbolTable;
    };


    class TreeSemantics {
        DiagnosticBuffer & errStream;
        TreeSemanticsContext *context;
    public:
        void setContext(TreeSemanticsContext *context);
        bool checkDeclInContext(DeclNode *decl);
        TreeSemantics(DiagnosticBuffer & errStream);
    };

    TreeSemantics::TreeSemantics(DiagnosticBuffer & errStream):
    errStream(errStream),
    context(nullptr){

    };

    void TreeSemantics::setContext(TreeSemanticsContext *context){
        this->context = context;
    };

    bool TreeSemantics::checkDeclInContext(DeclNode *decl){
        switch (decl->type) {
            case CLASS_DECL : {
                auto class_decl = (ClassDeclNode *)decl;
                
                break;
            }
            case FUNC_DECL : {
                break;
            }
        }
    };






    /// Core Parser

   static std::unique_ptr<TreeSemantics> semantics;
    static std::unique_ptr<TreeBuilder> builder;



    Parser::Parser(Gen *gen):
    gen(gen),
    lexer(std::make_unique<Lexer>()),
    errStream(std::make_unique<DiagnosticBuffer>()){

        builder = std::make_unique<TreeBuilder>(lexer.get(),*errStream);
        semantics = std::make_unique<TreeSemantics>(*errStream);
    };

    void Parser::setInputStream(std::istream *is){
        this->lexer->setInputStream(is);
    };

    void Parser::beginParse(){
        DeclNode *node;
        bool err = false;
        while(((node = builder->nextDecl(&err)) != nullptr) && (!err)){
            gen->genDecl(node);
        };
        gen->finish();
    };

    void Parser::finish(){

    };

};