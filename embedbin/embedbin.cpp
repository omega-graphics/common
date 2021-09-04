#include <omega-common/common.h>

#include <iostream>

using namespace OmegaCommon;

static Map<String,String> srcs;

inline void extractSources(StrRef lineBuffer){
    std::string name,src;
    auto line_it = lineBuffer.begin();
    while(line_it != lineBuffer.end()){
        auto & c =*line_it;
        if(c == '=') {
            break;
        }
        name.push_back(c);
        ++line_it;
    };
    ++line_it;
    while(line_it != lineBuffer.end()){
        auto & c =*line_it;
        src.push_back(c);
        ++line_it;
    };

    srcs.insert(std::make_pair(name,src));
};

auto header = 
R"(// WARNING!! This file was generated by omega-ebin.
// )";

#define STATIC_BUFFER_DECLARE "const char "
#define CHAR_LINE_MAX 20

int main(int argc,char *argv[]){
    
    
    StrRef input;
    StrRef output;

    for(unsigned i = 1;i < argc;i++){
        StrRef in(argv[i]);
        if(in == "-o"){
            output = argv[++i];
        }
        else if(in == "-i"){
            input = argv[++i];
        };
    };

    std::ifstream infile {input,std::ios::in};
    if (infile.is_open()) {
        while(!infile.eof()){
            std::string line;
            std::getline(infile,line);
            // std::cout << line << std::endl;
            extractSources(line);
        }
    }
    else {
        std::cerr << "ERROR: " << input.data() << " cannot be opened. Exiting..." << std::endl;
        exit(1);
    }
    infile.close();


    std::ofstream out {output.data(),std::ios::out};

    auto p = FS::Path(input);

    std::cout << p.dir() << std::endl;

    FS::changeCWD(p.dir());

    out << header << std::endl;

    for(auto & s : srcs){
        out << STATIC_BUFFER_DECLARE << s.first << "[] =";
        out << "{";
        std::ifstream binIn(s.second,std::ios::binary | std::ios::in);
        if(!binIn.is_open()){
            std::cerr << "ERROR: " << s.second << " cannot be opened. Exiting..." << std::endl;
            exit(1);
        };
        unsigned c_count = 0;
        out << std::hex;
        while(!binIn.eof()) {
            int c = binIn.get();
            ++c_count;
            if(!binIn.eof()){ 
                out << "0x" << c;
            }
            else {
                break;
            }
            if(!binIn.eof()){
                out << ",";
            };
            if(c_count >= CHAR_LINE_MAX){
                out << std::endl;
                c_count = 0;
            };
        }
        out << std::dec;
        out << "};" << std::endl;
    };

    out.close();


    return 0;
};