#include "utils.h"

#ifndef OMEGA_COMMON_FORMAT_H
#define OMEGA_COMMON_FORMAT_H

namespace OmegaCommon {

     template<class T>
    struct ObjectFormatter {
        static void format(std::ostream & os,T && object);
    };

    template<class ..._Args>
    OmegaCommon::String formatString(const char *fmt,_Args && ...args){
        std::ostringstream out;
        OmegaCommon::ObjectFormatter<_Args>::format(out,args)...;
        return out.str();
    };
};

#endif