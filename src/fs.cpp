 #include "omega-common/fs.h"
 #include <cctype>
 #include "omega-common/utils.h"


 namespace OmegaCommon::FS {

    struct Token {
        typedef enum : int {
            ID,
            Dot,
            Slash,
        } Type;
        Type type;
        String str;
    };
 
    void Path::parse(const String & str){

        _str = str;

        Vector<Token> tokens;

        // Core::Regex regex(R"(([\w|_|\/|\.]*)\/(\w+)(?:\.(\w+))?$)");

        unsigned idx = 0;
        
        char buffer[200];
        char *buffer_ptr = buffer;
        char *buf_start = buffer_ptr;
        
        auto get_char = [&](){
            return str[idx];
        };
        
        auto ahead_char = [&](){
            return str[idx + 1];
        };
        
        auto clear_buffer = [&](Token::Type ty){
            auto len = buffer_ptr - buf_start;
            tokens.push_back({ty,String(buffer,len)});
            buffer_ptr = buffer;
        };
        
        auto isAlnumAndOther = [&](char c){
            return isalnum(c) ||  (c == '-') || (c == '_');
        };
        
        char c;
        /// A Boolean to decide whether to continue!
        bool cont = true;
        while(idx != str.size()){
            c = get_char();
            switch (c) {
                // case '\0' : {
                //     cont = false;
                //     break;
                // }
                case '/' : {
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    clear_buffer(Token::Slash);
                    break;
                };
                case '\\' : {
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    clear_buffer(Token::Slash);
                    break;
                };
                case '.' : {
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    clear_buffer(Token::Dot);
                    break;
                }
                case ' ' : {
                    break;
                }
                default : {
                    if(isAlnumAndOther(c)){
                        *buffer_ptr = c;
                        ++buffer_ptr;
                        if(!isAlnumAndOther(ahead_char())){
                            clear_buffer(Token::ID);
                        };
                    };
                    break;
                }
            };
            ++idx;
        };
        

        auto it = tokens.rbegin();
        _dir = "";
        while(it != tokens.rend()){
            auto & tok = *it;
            if(tok.type == Token::ID && it == tokens.rbegin()){
                ++it;
                auto & tok2 = *it;
                if(tok2.type == Token::Dot){
                    _ext = tok.str;
                };
                ++it;
                tok2 = *it;
                if(tok2.type == Token::ID){
                    _fname = tok2.str;
                }
                ++it;
                
            }
            else {
                _dir = tok.str + _dir;
            };
            ++it;
        };

        isRelative = tokens.front().type == Token::Dot || tokens.front().type == Token::ID;
           
        
    };

    String & Path::dir(){
        return _dir;
    };

    String & Path::ext(){
        return _ext;
    };

    String & Path::filename(){
        return _fname;
    };

    String & Path::str(){
        return _str;
    };


    Path & Path::append(const char *str){
        _str = _str + PATH_SLASH + str;
        return *this;
    };

    Path & Path::append(String & str){
        _str += PATH_SLASH + str;
        return *this;
    };

    Path & Path::append(TStrRef & str){
        _str = _str + PATH_SLASH + str;
        return *this;
    };

    Path::Path(const char *str){
        parse(str);
    };

    Path::Path(const String & str){
        parse(str);
    };

    

    Path Path::operator+(const char *str){
        return _str + PATH_SLASH + str;
    };

    Path Path::operator+(String & str){
        return _str + PATH_SLASH + str;
    };

    Path Path::operator+(TStrRef & str){
        return _str + PATH_SLASH + str;
    };

    Path::~Path(){
        
    };

    bool & DirectoryIterator::end(){
        return _end;
    };

 };