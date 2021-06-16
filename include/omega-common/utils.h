#include <string>
#include <fstream>
#include <sstream>
#include <sys/_types/_int32_t.h>
#include <vector>
#include <map>
#include <cstring>

#include "crt.h"

#ifndef OMEGA_COMMON_COMMON_UTILS_H
#define OMEGA_COMMON_COMMON_UTILS_H


namespace OmegaCommon {

    template<class CharTY>
    class StringBaseImpl {
        std::allocator<CharTY> alloc;
        CharTY *_data;
    public:
        typedef unsigned size_type;
    private:
        size_type _len;
        using SELF = StringBaseImpl<CharTY>;
    public:
        using iterator = CharTY *;
        using reference = CharTY &;
        using const_iterator = const CharTY *;
        using const_reference = const CharTY &;

        size_type & size(){
            return _len;
        };
        CharTY *data(){
            return _data;
        };
        iterator begin(){
            return iterator(_data);
        };
        iterator end(){
            return iterator(_data + _len);
        };
        const_iterator cbegin(){
            return const_iterator(_data);
        };
        const_iterator cend(){
            return const_iterator(_data + _len);
        };
    private:
        #define STATIC_ALLOC(n) CharTY temp[n];
        inline void _grow_self(unsigned by){
            if(!_data){
                _data = alloc.allocate(by);
                _len += by;
            }
            else {
                STATIC_ALLOC(by)
                std::move(begin(),end(),temp);
                alloc.deallocate(_data,size());
                _data = alloc.allocate(size() + by);
                std::move(temp,temp + by,_data);
                _len += by;
            }
        };
        void _append_self(const char *str_data,size_type len){
            _grow_self(len);
            _data = strcat(_data,str_data);
        };
    public:
        SELF & append(StringBaseImpl & otherStr){
            _append_self(otherStr._data,otherStr._len);
            return *this;
        };
        SELF & append(const char *cstr){
            _append_self(cstr,strlen(cstr));
            return *this;
        };
        SELF & operator +=(StringBaseImpl & other){
            return append(other);
        };
        SELF & operator +=(const char * other){
            return append(other);
        };

        StringBaseImpl():_data(nullptr),_len(0){

        };
        StringBaseImpl(std::string & other){
            _data = alloc.allocate(other.size());
            _len = other.size();
            std::copy(other.begin(),other.end(),_data);
        };
        StringBaseImpl(const char *cstr){
            size_type l = strlen(cstr);
            _data = alloc.allocate(l);
            _len = l;
            std::move(cstr,cstr + l,_data);
        };
        StringBaseImpl(char * cbuf,size_type len){
            _data = alloc.allocate(len);
            _len = len;
            std::move(cbuf,cbuf + len,_data);
        };
        StringBaseImpl(StringBaseImpl & other){
            _data = alloc.allocate(other.size());
            _len = other.size();
            std::copy(other.begin(),other.end(),_data);
        };

    };

    typedef StringBaseImpl<char> OMTString;
    typedef StringBaseImpl<wchar_t> OMWString;
    typedef StringBaseImpl<char32_t> OMUString;

    typedef std::string String;
    typedef std::u32string UString;
    typedef std::wstring WString;

    template<class CharTY>
    class StrRefBase {
        CharTY *_data;
    public:
        typedef unsigned size_type;
    private:
        size_type _len;
        using SELF = StrRefBase<CharTY>;
    public:
        using iterator = CharTY *;
        using reference = CharTY &;
        using const_iterator = const CharTY *;
        using const_reference = const CharTY &;

        size_type & size(){
            return _len;
        }
        CharTY *data(){
            return _data;
        }
        iterator begin(){
            return iterator(_data);
        }
        iterator end(){
            return iterator(_data + _len);
        }
        const_iterator cbegin(){
            return const_iterator(_data);
        }
        const_iterator cend(){
            return const_iterator(_data + _len);
        }

    private:

    public:

        StrRefBase(std::basic_string<CharTY> & strty):_data(strty.data()),_len(strty.size()){

        }

        StrRefBase(CharTY *buffer,size_type length):_data(buffer),_len(length){

        }
    };

    typedef StrRefBase<char> TStrRef;
    typedef StrRefBase<wchar_t> WStrRef;
    typedef StrRefBase<char32_t> UStrRef;

    TStrRef operator&(String & str);
    WStrRef operator&(WString & str);
    UStrRef operator&(UString & str);

    template<class T>
    using Vector = std::vector<T>;

    template<class K,class V>
    using Map = std::map<K,V>;

    typedef enum : int {
        Ok,
        Failed
    } StatusCode;
    
};

#endif
