#include "omega-common/common.h"
#include "../parser.h"
#include <iostream>

void printHelp(){
    std::cout << 
    R"(
    Omega Wrapper Generator
    =======================
    Usage : omega-wrapgen [options] <input_file>
    
    Options:
    
       Args:
           --help, -h        -> Display this info
           --output-dir, -o  -> Output dir of generated wrapper files
       Modes:
           --cc     -> Generate C Code
           --python -> Generate Python Code
           --go     -> Generate Go Code
           --java   -> Generate Java Code
           --swift  -> Generate Swift Code
           --rust   -> Generate Rust Code)" << std::endl;
    exit(0);
};

int main(int argc,char *argv[]){
    /// Omit First Arg in `argv`
    OmegaCommon::TStrRef src_file;
    --argc;
    unsigned i = 1;
    while(argc > 0){
        OmegaCommon::TStrRef arg(argv[i]);
        if(arg == "--help" || arg == "-h"){
            printHelp();
        }
        ++i;
        --argc;
        if(argc == 0){
            src_file = argv[i-1];
        };
    };



    std::ifstream in("./example.owrap");

    std::string output_dir = "./dist";
    OmegaWrapGen::GenContext gen_ctxt;
    gen_ctxt.output_dir = output_dir;

    OmegaWrapGen::CGenSettings settings {""};
    
    auto generator = OmegaWrapGen::Gen::CreateCGen(settings);
    generator->setContext(gen_ctxt);

    OmegaWrapGen::Parser parser(generator);
    parser.setInputStream(&in);
    parser.beginParse();
    parser.finish();

    generator->finish();

    return 0;
};