#include <string>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstring>
#include <typeinfo>
#include <memory>
#include <cassert>
#include <optional>
#include <iostream>

#ifdef _WIN32
#ifdef OMEGACOMMON__BUILD__
#define OMEGACOMMON_EXPORT __declspec(dllexport)
#else 
#define OMEGACOMMON_EXPORT __declspec(dllimport)
#endif
#else 

#define OMEGACOMMON_EXPORT

#endif


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
        const CharTY *_data;
    public:
        typedef unsigned size_type;
    private:
        const size_type _len;
        using SELF = StrRefBase<CharTY>;
    public:
        using const_iterator = const CharTY *;
        using const_reference = const CharTY &;

        const size_type & size() const{
            return _len;
        }
        const CharTY *data() const{
            return _data;
        }
        const_iterator begin() const{
            return const_iterator(_data);
        }
        const_iterator end() const{
            return const_iterator(_data + _len);
        }

        const_reference operator[](unsigned idx) const{
            return begin()[idx];
        };

    private:
        bool compare(StrRefBase & str) const{
            if(_len != str._len)
                return false;
            
            for(unsigned i = 0;i < _len;i++){
                auto & c = *(begin() + i);
                if(c != str[i]){
                    return false;
                };
            };
            return true;
        };
    public:

        StrRefBase():_data(nullptr),_len(0){

        };

        StrRefBase(const std::basic_string<CharTY> & str):_data(str.data()),_len(str.size()){

        }

        StrRefBase(CharTY *buffer,size_type length):_data(buffer),_len(length){

        }

        StrRefBase(const CharTY *c_str):_data(const_cast<char *>(c_str)),_len(strlen(c_str)){

        }

        bool operator==(const std::basic_string<CharTY> & str) const{
            StrRefBase ref(str);
            return compare(ref);
        };

        bool operator==(StrRefBase &str) const{
            return compare(str);
        };

        bool operator==(const CharTY *str) const{
            StrRefBase ref(str);
            return compare(ref);
        };

        bool operator!=(const std::basic_string<CharTY> & str) const{
            StrRefBase ref(str);
            return !compare(ref);
        };

        bool operator!=(StrRefBase &str) const{
            return !compare(str);
        };

        bool operator!=(const CharTY *str) const{
            StrRefBase ref(str);
            return !compare(ref);
        };

        operator std::basic_string<CharTY>(){
            return {this->begin(),this->end()};
        };

    };

    template<class CharTy>
    inline bool operator==(const std::basic_string<CharTy> & lhs,const StrRefBase<CharTy> & rhs){
        return rhs == lhs;
    }

    template<class CharTy>
    inline bool operator==(const char * lhs,const StrRefBase<CharTy> & rhs){
        return rhs == lhs;
    }

    typedef StrRefBase<char> StrRef;
    typedef StrRefBase<wchar_t> WStrRef;
    typedef StrRefBase<char32_t> UStrRef;


    OMEGACOMMON_EXPORT String operator+(const String & lhs,const StrRef & rhs);
    OMEGACOMMON_EXPORT WString operator+(const WString & lhs,const WStrRef & rhs);
    OMEGACOMMON_EXPORT UString operator+(const UString & lhs,const UStrRef & rhs);

    OMEGACOMMON_EXPORT void operator+=(String & lhs, StrRef & rhs);
    OMEGACOMMON_EXPORT void operator+=(WString & lhs,WStrRef & rhs);
    OMEGACOMMON_EXPORT void operator+=(UString & lhs,UStrRef & rhs);

    OMEGACOMMON_EXPORT StrRef operator&(String & str);
    OMEGACOMMON_EXPORT WStrRef operator&(WString & str);
    OMEGACOMMON_EXPORT UStrRef operator&(UString & str);



    template<class T>
    using Vector = std::vector<T>;

    template<class T,class Comp = std::equal_to<T>>
    class SetVector : Vector<T>{
        Comp compare;
        typedef Vector<T> super;
    public:
        typedef typename super::size_type size_type;
        typedef typename super::iterator iterator;
        typedef typename super::reference reference;

        size_type & size() const{
            return super::size();
        }

        iterator begin(){
            return super::begin();
        }

        iterator end(){
            return super::end();
        }

        iterator find(const T & el){
            auto it = begin();
            for(;it != end();it++){
                if(compare(*it,el)){
                    break;
                }
            }
            return it;
        }

        void push(const T & el){
            if(find(el) == end())
                super::push_back(el);
        }

        void push(T && el){
            if(find(el) == end())
                super::push_back(el);
        }

        void pop(){
            super::pop_back();
        }
    };



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
        typedef const T * const_iterator;
        typedef const T & const_reference;

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
        template<class __It>
        ArrayRef(__It beg,__It end):ContainerRefBase<T>(beg,end){
            
        };
        

        ArrayRef(Vector<T> & vec):ContainerRefBase<T>(vec.begin(),vec.end()){
            
        };

        template<size_t len>
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

    template<class K,class V>
    using MapVec = std::unordered_map<K,V>;



    /** 
      An immutable reference to an Map or an MapVector
    */
    template<class K,class V>
    class MapRef : public ContainerRefBase<std::pair<K,V>>{
        typedef ContainerRefBase<std::pair<K,V>> super;
    public:
        using typename super::size_type;
        using typename super::const_reference;
        using typename super::const_iterator;

        typedef const K & const_key_ref;
        typedef const V & const_val_ref;
        
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

        MapRef(MapVec<K,V> & map):ContainerRefBase<std::pair<K,V>>(map.begin(),map.end()){

        };

        operator Map<K,V>(){
            return {this->begin(),this->end()};
        };
    };

    template<class K,class V>
    MapRef<K,V> operator&(Map<K,V> & other){
        return other;
    };

    template<class K,class V>
    MapRef<K,V> operator&(MapVec<K,V> & other){
        return other;
    };

    struct RuntimeObject {
        unsigned refCount;
        void inc() { refCount += 1;};
        void dec() {refCount -= 1;}
        RuntimeObject() : refCount(1){

        }
    };

    template<class T,std::enable_if_t<std::is_base_of_v<RuntimeObject,T>,int> = 0>
    class ARC {
    protected:
        T *data;
    public:
        operator bool(){
            return data != nullptr;
        }
        T & operator *() const{
            return *data;
        }
        T * operator->() const{
            return data;
        }
        explicit ARC(T * ptr){
            data = ptr;
        };
        template<class OTy,std::enable_if_t<std::is_convertible_v<OTy,T>,int> = 0>
        ARC(OTy * ptr){
            data = ptr;
        };
        template<class OTy,std::enable_if_t<std::is_convertible_v<OTy,T>,int> = 0>
        ARC(const ARC<OTy> &other){
            data = other.data;
            data->inc();
        }
        template<class OTy,std::enable_if_t<std::is_convertible_v<OTy,T>,int> = 0>
        ARC(ARC<OTy> && other){
            data = other.data;
            data->inc();
        }
        ~ARC(){
            data->dec();
            if(data->refCount == 0){
                delete data;
            }
        };
    };

    template<class T,class ...Args>
    ARC<T> makeARC(Args && ...args){
        return ARC<T>(new T(args...));
    };

    template<class T>
    struct RuntimeTypeWrapper : public RuntimeObject {
        T data;
    };

    template<class T>
    class ARCAny : protected ARC<RuntimeTypeWrapper<T>> {
    public:
        explicit ARCAny(RuntimeTypeWrapper<T> * ptr): ARC<RuntimeTypeWrapper<T>>(ptr){

        }
        T & operator *() const {
            return this->data->data;
        }
        T * operator->() const {
            return &(this->data->data);
        }
    };

    template<class T,class ...Args>
    ARCAny<T> makeARCAny(Args && ...args){
        return ARCAny<T>(new RuntimeTypeWrapper<T>{{args...}});
    };

    typedef enum : int {
        Ok,
        Failed
    } StatusCode;

//    namespace Argv {
//
//        template<class T>
//        struct ArgVal{
//            std::shared_ptr<T> value;
//            inline operator bool(){
//                return value;
//            };
//            inline operator T & (){
//                return *value;
//            }
//        };
//
//        enum class ArgumentType : int {
//            Flag,
//            Positional,
//        };
//
//        struct Flag {
//            StrRef val;
//            Flag(StrRef val):val(val){
//
//            }
//        };
//
//        struct Desc {
//            StrRef val;
//            Desc(StrRef val):val(val){
//
//            }
//        };
//
//        template<class T>
//        struct ArgumentParser;
//
//        struct ArgumentParserBase {
//            virtual void printHelp() = 0;
//            virtual bool parseArg(OmegaCommon::StrRef arg) = 0;
//        };
//
//        template<class T,
//                std::enable_if_t<std::is_function_v<decltype(ArgumentParser<T>::help)>,int> = 0,
//                std::enable_if_t<std::is_function_v<decltype(ArgumentParser<T>::format)>,int> = 0>
//        struct ArgumentParserImpl : public ArgumentParserBase {
//
//            std::vector<std::string> flagMatches;
//            Desc desc;
//            ArgumentType type;
//            std::shared_ptr<T> & val;
//
//            ArgumentParserImpl(ArgumentType type,std::vector<std::string> flagMatches, Desc & desc,std::shared_ptr<T> & val):
//            flagMatches(flagMatches),
//            desc(desc),
//            type(type),val(val){
//
//            };
//
//            void printHelp() override{
//                ArgumentParser<T>::help(std::cout);
//            }
//
//            bool parseArg(OmegaCommon::StrRef arg) override{
//                return ArgumentParser<T>::format(arg,flagMatches,val);
//            }
//        };
//
//        template<>
//        struct  ArgumentParser<bool> {
//
//            static void help(std::ostream & out,Desc & desc,ArrayRef<std::string> & m){
//
//            }
//            static bool parse(OmegaCommon::StrRef arg,const ArrayRef<std::string> & m,std::shared_ptr<bool> & value){
//
//            }
//        };
//
//
//        template<class T,class ..._Args>
//        ArgumentParserBase * buildArgumentParser(std::shared_ptr<T> & val,ArgumentType type,std::vector<std::string> flagMatches, Desc & desc){
//            return new ArgumentParserImpl<T>(type,flagMatches,desc,val);
//        };
//
//
//        class Parser {
//
//        public:
//
//            Parser();
//
//            template<class T>
//            ArgVal<T> argument();
//
//            template<>
//            ArgVal<bool> argument(){
//
//            };
//            template<>
//            ArgVal<String> argument(){
//
//            };
//            template<>
//            ArgVal<Vector<String>> argument(){
//
//            };
//
//            void parseArgv(int & argc,char **argv);
//        };
//
//    }
    
};

bool findProgramInPath(const OmegaCommon::StrRef & prog,OmegaCommon::String & out);

inline std::ostream & operator<<(std::ostream &os,OmegaCommon::StrRef &str){
    return os << str.data();
};

inline std::wostream & operator<<(std::wostream &os,OmegaCommon::WStrRef &str){
    return os << str.data();
};

//inline std::basic_ostream<char32_t> & operator<<(std::basic_ostream<char32_t> &os,OmegaCommon::UStrRef &str){
//    return os << str.data();
//};

#endif
