#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#ifndef OMEGA_COMMON_COMMON_UTILS_H
#define OMEGA_COMMON_COMMON_UTILS_H

namespace OmegaCommon {

    typedef std::string String;
    typedef std::u32string UString;
    typedef std::wstring WString;

    template<class T>
    using Vector = std::vector<T>;

    typedef enum : int {
        Ok,
        Failed
    } StatusCode;
    
};

#endif
