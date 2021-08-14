#include "utils.h"
#include <tuple>
#include <array>

#ifndef OMEGA_COMMON_FORMAT_H
#define OMEGA_COMMON_FORMAT_H

namespace OmegaCommon {

    template<typename T>
    struct FormatProvider;

    template<typename T>
    using _has_format_provider = std::is_function<decltype(FormatProvider<T>::format)>;

    struct ObjectFormatProviderBase {
        virtual void insertFormattedObject(std::ostream & os) = 0;
        virtual ~ObjectFormatProviderBase() = default;
    };

    template<class T,class Pr = FormatProvider<T>>
            struct ObjectFormatProvider : public ObjectFormatProviderBase {
        T & object;
        void insertFormattedObject(std::ostream &os) override {
            Pr::format(os,object);
        };

        template<std::enable_if_t<_has_format_provider<T>::value,int> = 0>
                explicit ObjectFormatProvider(T & object):object(object){

        };
        ~ObjectFormatProvider() = default;
    };


    template<>
    struct FormatProvider<int> {
        static void format(std::ostream & os,int & object){
            os << object;
        }
    };

    template<>
    struct FormatProvider<char> {
        static void format(std::ostream & os,char & object){
            os << object;
        }
    };

    template<>
    struct FormatProvider<std::string> {
        static void format(std::ostream & os,std::string & object){
            os << object;
        }
    };

    template<>
    struct FormatProvider<TStrRef> {
        static void format(std::ostream & os,TStrRef & object){
            os << object.data();
        }
    };

    class Formatter;

    Formatter *createFormatter(TStrRef fmt,std::ostream & out);
    void format(Formatter * formatter,ArrayRef<ObjectFormatProviderBase *> objectFormatProviders);
    void freeFormatter(Formatter *formatter);

    template<typename T>
    ObjectFormatProvider<T> * buildFormatProvider(T object){
        return new ObjectFormatProvider<T>(object);
    };

    template<class ..._Args>
    OmegaCommon::String fmtString(const char *fmt,_Args && ...args){
        std::ostringstream out;
//        auto t_args = std::make_tuple(std::forward<_Args>(args)...);
        std::array<ObjectFormatProviderBase *,sizeof...(args)> arrayArgs = {buildFormatProvider(std::forward<_Args>(args))...};
        auto formatter = createFormatter(fmt,out);
        format(formatter,arrayArgs);
        freeFormatter(formatter);
        for(auto a : arrayArgs){
            delete a;
        }
        
        return out.str();
    };
};

#endif