 #include "omega-common/fs.h"
 #include <cctype>
 
 namespace OmegaCommon::FS {
 
    void Path::parse(const String & str){

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
                case '.' : {
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    clear_buffer(Token::Dot);
                    break;
                }
                case '\\' : {
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    clear_buffer(Token::Slash);
                    break;
                };
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
        
        
    };

    String & Path::ext(){
        // if(tokens.back().type == Token::ID && tokens[tokens.size() - 2].type == Token::Dot){
        //     return tokens.back().str;
        // }
        return tokens.back().str;
    };

    String Path::filename(){
        String res = "";
        auto it = tokens.begin();
        while(it != tokens.end()){
            auto &type  = it->type;
            if(type == Token::ID){
                ++it;
                auto & type = it->type;
                if(type == Token::Dot){
                    res.append((it - 1)->str);
                    if(it == (tokens.end() - 2)){
                        return res;
                        break;
                    };
                    
                    while(it != (tokens.end() - 2)){
                        res.append(it->str);
                        ++it;
                    };
                    return res;
                    break;
                };
            };
            ++it;
        };
        return "";
    };

    String & Path::str(){
        return _str;
    };


    Path & Path::append(const char *str){
        // std::string_view _view(str);
        #ifdef _WIN32
        _str += "\\" + str;
        #else
        _str + "/" + str;
        #endif
        return *this;
    };

    Path & Path::append(String & str){
        _str + "/" + str;
        return *this;
    };

    Path Path::operator+(const char *str){
        return _str + "/" + str;
    };

    Path Path::operator+(String & str){
        return _str + "/" + str;
    };

    Path::~Path(){
        
    };

 };