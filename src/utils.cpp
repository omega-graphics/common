#include "omega-common/utils.h"

namespace OmegaCommon {

    String operator+(const String & lhs,const StrRef & rhs){
        String ret;
        ret = lhs;
        ret.resize(ret.size() + rhs.size());
        std::copy(rhs.begin(),rhs.end(),ret.begin());
        return ret;
    }
    WString operator+(const WString & lhs,const WStrRef & rhs){
        WString ret;
        ret = lhs;
        ret.resize(ret.size() + rhs.size());
        std::copy(rhs.begin(),rhs.end(),ret.begin());
        return ret;
    }
    UString operator+(const UString & lhs,const UStrRef & rhs){
        UString ret;
        ret = lhs;
        ret.resize(ret.size() + rhs.size());
        std::copy(rhs.begin(),rhs.end(),ret.begin());
        return ret;
    }

    void operator+=(String & lhs, StrRef & rhs){
        lhs.resize(lhs.size() + rhs.size());
        std::copy(rhs.begin(),rhs.end(),lhs.begin());
    }
    void operator+=(WString & lhs,WStrRef & rhs){
        lhs.resize(lhs.size() + rhs.size());
        std::copy(rhs.begin(),rhs.end(),lhs.begin());
    }
    void operator+=(UString & lhs,UStrRef & rhs){
        lhs.resize(lhs.size() + rhs.size());
        std::copy(rhs.begin(),rhs.end(),lhs.begin());
    }

    StrRef operator&(String & str){
        return str;
    }
    WStrRef operator&(WString & str){
        return str;
    }
    UStrRef operator&(UString & str){
        return str;
    }
    
    namespace Argv {
        
    }

}
