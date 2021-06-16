#include "omega-common/utils.h"

namespace OmegaCommon {

    TStrRef operator&(String & str){
        return str;
    };
    WStrRef operator&(WString & str){
        return str;
    };
    UStrRef operator&(UString & str){
        return str;
    };

};