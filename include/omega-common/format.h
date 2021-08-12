#include "utils.h"
#include <tuple>

#ifndef OMEGA_COMMON_FORMAT_H
#define OMEGA_COMMON_FORMAT_H

namespace OmegaCommon {

    struct ObjectFormatterBase {

    };

     template<class T>
    struct ObjectFormatter : public ObjectFormatterBase {
        std::ostream & os;
        T && object;
        ObjectFormatter(std::ostream & os,T && object):os(os),object(object){

        };
        static void format(std::ostream & os,T && object);
    };

    template<class ..._Args>
    OmegaCommon::String formatString(const char *fmt,_Args && ...args){
        std::ostringstream out;
        auto _args = std::make_tuple(new ObjectFormatter<_Args>(out,args)...);
        auto _args_array = std::array<ObjectFormatterBase *,std::tuple_size<decltype(_args)>::value>();
        
        return out.str();
    };
};

#endif