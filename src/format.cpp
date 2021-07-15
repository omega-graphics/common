#include "omega-common/format.h"

namespace OmegaCommon {

    template<>
    struct ObjectFormatter<int> {
        static void format(std::ostream & os,int && obj){
            
        };
    };

    template<>
    struct ObjectFormatter<bool> {
        static void format(std::ostream & os,bool && obj){
            
        };
    };

    template<>
    struct ObjectFormatter<OmegaCommon::String> {
        static void format(std::ostream & os,OmegaCommon::String && obj){
            
        };
    };

    template<>
    struct ObjectFormatter<OmegaCommon::TStrRef> {
        static void format(std::ostream & os,OmegaCommon::TStrRef && obj){

        };
    };

    template<>
    struct ObjectFormatter<char> {
        static void format(std::ostream & os,char && obj){
            
        };
    };

}