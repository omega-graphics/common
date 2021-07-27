#include "../parser.h"
#include <iostream>

using namespace OmegaWrapGen;

int main(int argc,char *argv[]){

    TreeDumper tree {std::cout};

    Parser parser {&tree};

    OmegaCommon::TStrRef filename {argv[1]};

    std::ifstream in {filename};

    parser.setInputStream(&in);
    parser.beginParse();
    parser.finish();

    return 0;
};