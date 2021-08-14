#include "omega-common/common.h"
#include "../parser.h"
#include <iostream>
#include <filesystem>

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
    OmegaCommon::TStrRef output_dir;
    --argc;
    unsigned i = 1;
    while(argc > 1){
        OmegaCommon::TStrRef arg(argv[i]);
        if(arg == "--help" || arg == "-h"){
            printHelp();
        }
        else if(arg == "-o"){
            output_dir = argv[++i];
        }
        ++i;
        --argc;

        if(argc == 1){
            src_file = argv[i-1];
        };
    };



    std::ifstream in("./example.owrap");

    if(!std::filesystem::exists(output_dir.data())){
        std::filesystem::create_directory(output_dir.data());
    }
//    std::string output_dir = "./dist";
    OmegaWrapGen::GenContext gen_ctxt;
    gen_ctxt.output_dir = output_dir;
    gen_ctxt.name = OmegaCommon::FS::Path(src_file).filename();

    OmegaWrapGen::CGenSettings settings {OmegaWrapGen::CGenSettings::Retain};
    
    auto generator = OmegaWrapGen::Gen::CreateCGen(settings);
    generator->setContext(gen_ctxt);

    OmegaWrapGen::Parser parser(generator);
    parser.setInputStream(&in);
    parser.beginParse();
    parser.finish();

    generator->finish();

    return 0;
};