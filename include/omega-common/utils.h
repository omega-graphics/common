#include <string>
#include <fstream>
#include <sstream>
#include <sys/_types/_int32_t.h>
#include <type_traits>
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

    /** 
      Immutable Reference to a String.
    */
    template<class CharTY>
    class StrRefBase {
        CharTY *_data;
    public:
        typedef unsigned size_type;
    private:
        size_type _len;
        using SELF = StrRefBase<CharTY>;
    public:
        using const_iterator = const CharTY *;
        using const_reference = const CharTY &;

        size_type & size(){
            return _len;
        }
        CharTY *data(){
            return _data;
        }
        const_iterator begin(){
            return const_iterator(_data);
        }
        const_iterator end(){
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


    String operator+(const String & lhs,TStrRef & rhs);
    WString operator+(const WString & lhs,WStrRef & rhs);
    UString operator+(const UString & lhs,UStrRef & rhs);

    void operator+=(String & lhs,TStrRef & rhs);
    void operator+=(WString & lhs,WStrRef & rhs);
    void operator+=(UString & lhs,UStrRef & rhs);

    TStrRef operator&(String & str);
    WStrRef operator&(WString & str);
    UStrRef operator&(UString & str);

    template<class T>
    using Vector = std::vector<T>;

    template<class T,unsigned len>
    using Array = std::array<T,len>;

    /** 
      The base class for all container reference classes.
    */
    template<class T>
    class ContainerRefBase {
    protected:
        T *_data;
    public:
        typedef unsigned size_type;
    protected:
        size_type _size;
    public:
        using const_iterator = const T *;
        using const_reference = const T &;

        bool empty() noexcept{
            return _size == 0;
        };

        size_type & size(){
            return _size;
        };
        
        const_iterator begin(){
            return const_iterator(_data);
        };
        const_iterator end(){
            return const_iterator(_data + _size);
        };

        const_reference front(){
            return begin()[0];
        };

        const_reference back(){
            return end()[-1];
        };

        template<class _iterator>
        ContainerRefBase(_iterator _st,_iterator _end):_data(_st),_size(_end - _st){

        };

        ContainerRefBase(T * data,size_type len):_data(data),_size(len){

        };
    };

    /** 
      An immutable reference to an Array or Vector
    */
    template<class T>
    class ArrayRef : public ContainerRefBase<T>{
        using super = ContainerRefBase<T>;
    public:
        typedef typename super::const_reference const_reference;
        typedef typename super::size_type size_type;

        const_reference operator[](size_type idx){
            assert(idx < this->size() && "Index must be smaller than the size of the ArrayRef");
            return this->begin()[idx];
        };
        

        ArrayRef(Vector<T> & vec):ContainerRefBase<T>(vec.begin(),vec.end()){
            
        };

        template<unsigned len>
        ArrayRef(Array<T,len> & array):ContainerRefBase<T>(array.data(),len){
            
        };

        operator Vector<T>(){
            return {this->begin(),this->end()};
        }
    };

    template<class T>
    ArrayRef<T> operator&(Vector<T> & other){
        return other;
    };

    template<class K,class V>
    using Map = std::map<K,V>;

    /** 
      An immutable reference to an Map
    */
    template<class K,class V>
    class MapRef : public ContainerRefBase<std::pair<K,V>>{
        using super = ContainerRefBase<std::pair<K,V>>;
    public:
        typedef typename super::size_type size_type;
        typedef typename super::const_iterator const_iterator;
        typedef typename super::const_reference const_reference;

        typedef std::add_lvalue_reference<std::add_const<typename const_reference::first_type>> const_key_ref;
        typedef std::add_lvalue_reference<std::add_const<typename const_reference::second_type>> const_val_ref;
        
        const_iterator find(const_key_ref key){
            auto it = this->begin();
            for(;it != this->end();it++){
                if(it->first == key)
                    break;
            };
            return it;
        };

        const_val_ref operator[](const_key_ref key){
            auto res = find(key);
            return res->second;
        };

        MapRef(Map<K,V> & map):ContainerRefBase<std::pair<K,V>>(map.begin(),map.end()){

        };

        operator Map<K,V>(){
            return {this->begin(),this->end()};
        };
    };

    template<class K,class V>
    MapRef<K,V> operator&(Map<K,V> & other){
        return other;
    };

    typedef enum : int {
        Ok,
        Failed
    } StatusCode;
    
};

#endif
