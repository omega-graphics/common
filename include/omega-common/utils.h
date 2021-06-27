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

        const_reference operator[](unsigned idx){
            return begin()[idx];
        };

    private:
        bool compare(StrRefBase & str){
            if(str._len != str._len)
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

        StrRefBase(std::basic_string<CharTY> & strty):_data(strty.data()),_len(strty.size()){

        }

        StrRefBase(CharTY *buffer,size_type length):_data(buffer),_len(length){

        }

        StrRefBase(const CharTY *c_str):_data(const_cast<char *>(c_str)),_len(strlen(c_str)){

        }

        bool operator==(std::basic_string<CharTY> & str){
            StrRefBase ref(str);
            return compare(ref);
        };

        bool operator==(StrRefBase &str){
            return compare(str);
        };

        bool operator==(const CharTY *str){
            StrRefBase ref(str);
            return compare(ref);
        };
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

    template<class K,class V>
    using MapVec = std::unordered_map<K,V>;

    /** 
      An immutable reference to an Map or an MapVector
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

    // template<class T>
    // class ArcPtr {
    //     struct _ptr_data {
    //         T *data;
    //         unsigned ref_count;
    //     };
    //     _ptr_data * _data;
    // private:
    //     ArcPtr(_ptr_data *data):_data(data){
    //         _data->ref_count += 1;
    //     };
    // public:
    //     _ptr_data * _get_data() const{
    //         return _data;
    //     };
    // public:
    //     T & operator*() throw(){
    //         return *_data->data;
    //     };
    //     auto operator->(){
    //         return _data->data;
    //     };

    //     operator bool() const{
    //         return _data != nullptr;
    //     };

    //     // template<class _nTy>
    //     // ArcPtr<_nTy> castAs(){
    //     //     if(typeid(_nTy) == _data->t)
    //     //         return *this;
    //     //     else  
    //     //         return nullptr;
    //     // };

    //     template<class _nTy,std::enable_if_t<std::is_base_of_v<T,_nTy>,int> = 0>
    //     ArcPtr<_nTy> downcastAs(){
    //         if(!_data){
    //             return nullptr;
    //         };
    //         return *this;
    //     };

    //     template<class _nTy,std::enable_if_t<std::is_convertible_v<T,_nTy>,int> = 0>
    //     ArcPtr<_nTy> castTo(){
    //         if(!_data){
    //             return nullptr;
    //         };
    //         return *this;
    //     };
        
    //     ArcPtr():_data(nullptr){

    //     };

    //     ArcPtr(decltype(nullptr)):_data(nullptr){

    //     };

    //     ArcPtr(T * _ptr):_data(new _ptr_data()){
    //         _data->data = _ptr;
    //         _data->ref_count = 1;
    //     };

        
    //     ArcPtr(const ArcPtr & other):_data((_ptr_data *)other._get_data()){
    //         _data->ref_count += 1;
    //     };

      
    //     ArcPtr(ArcPtr && other):_data((_ptr_data *)other._get_data()){
    //         _data->ref_count += 1;
    //     };

    //     template<class _Ty,std::enable_if_t<std::is_base_of_v<_Ty,T>,int> = 0>
    //     ArcPtr(const ArcPtr<_Ty> & other):_data((_ptr_data *)other._get_data()){
    //         _data->ref_count += 1;
    //     };

    //     template<class _Ty,std::enable_if_t<std::is_base_of_v<_Ty,T>,int> = 0>
    //     ArcPtr(ArcPtr<_Ty> && other):_data((_ptr_data *)other._get_data()){
    //         _data->ref_count += 1;
    //     };

    //     template<class _Ty,std::enable_if_t<std::is_convertible_v<_Ty,T>,int> = 0>
    //     ArcPtr(const ArcPtr<_Ty> & other):_data((_ptr_data *)other._get_data()){
    //         _data->ref_count += 1;
    //     };

    //     template<class _Ty,std::enable_if_t<std::is_convertible_v<_Ty,T>,int> = 0>
    //     ArcPtr(ArcPtr<_Ty> && other):_data((_ptr_data *)other._get_data()){
    //         _data->ref_count += 1;
    //     };



    //     inline ArcPtr & operator=(ArcPtr && other){
    //         _data = (_ptr_data *)other._get_data();
    //         _data->ref_count += 1;
    //         return *this;
    //     };
        
    //     inline ArcPtr & operator=(const ArcPtr & other){
    //         _data = (_ptr_data *)other._get_data();
    //         _data->ref_count += 1;
    //         return *this;
    //     };

    //     template<class _Ty,std::enable_if_t<std::is_convertible_v<T,_Ty>,int> = 0>
    //     inline ArcPtr<T> & operator=(const ArcPtr<_Ty> & other){
    //         _data = (_ptr_data *)other._get_data();
    //         _data->ref_count += 1;
    //         return *this;
    //     };

    //     template<class _Ty,std::enable_if_t<std::is_convertible_v<T,_Ty>,int> = 0>
    //     inline ArcPtr<T> & operator=(ArcPtr<_Ty> && other){
    //         _data = (_ptr_data *)other._get_data();
    //         _data->ref_count += 1;
    //         return *this;
    //     };

    //     ~ArcPtr(){
    //         if(_data != nullptr){
    //             _data->ref_count -= 1;
    //             if(_data->ref_count == 0){
    //                 delete _data->data;
    //                 delete _data;
    //             };
    //         };
    //     };
    // };

    // template<class T,class ..._Args>
    // ArcPtr<T> make_arc(_Args && ...args){
    //     return ArcPtr<T>(new T(args...));
    // };

    // ArcPtr<int> bitch = make_arc<int>(0);
    
    // void boo (){
    //     auto test = bitch.castTo<float>();

    // };

    typedef enum : int {
        Ok,
        Failed
    } StatusCode;
    
};

#endif
