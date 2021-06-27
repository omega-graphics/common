#include "omega-common/common.h"
#include "parser.h"
#include <iostream>

void printHelp(){
    std::cout << 
    "Omega Wrapper Generator\n"
    "=======================\n"
    "Usage : omega-wrapgen [options] <input_file>\n"
    "\n"
    "\n"
    "Options:\n"
    "\n"
    "   Args:\n"
    "       --help, -h        -> Display this info\n"
    "       --output-dir, -o  -> Output dir of generated wrapper files\n"
    "   Modes:\n"
    "       --cc     -> Generate C Code\n"
    "       --python -> Generate Python Code\n"
    "       --go     -> Generate Go Code\n"
    "       --java   -> Generate Java Code\n"
    "       --swift  -> Generate Swift Code\n"
    "       --rust   -> Generate Rust Code" << std::endl;
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
    
    auto generator = OmegaWrapGen::Gen::CreateCGen();
    generator->setContext(gen_ctxt);

    OmegaWrapGen::Parser parser(generator);
    parser.setInputStream(&in);
    parser.beginParse();
    parser.finish();

    generator->finish();

    return 0;
};