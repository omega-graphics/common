#include "utils.h"
#include <initializer_list>
#include <istream>
#include <sstream>

#ifndef OMEGA_COMMON_JSON_H
#define OMEGA_COMMON_JSON_H

namespace OmegaCommon {

    class JSONParser;
    class JSONSerializer;

    /**
     @brief A simple class for parsing and serializing JSON, and for defining all of the JSON structures and data types.
     (Represents a single JSON object node in a JSON tree.)
     @paragraph 
     An instance of this class represents a single JSON Object Node in the JSON object tree.
    */
    class OMEGACOMMON_EXPORT JSON {
        static std::unique_ptr<JSONParser> parser;

        static std::unique_ptr<JSONSerializer> serializer;

        unsigned ty;

        void *_data;
        friend class JSONParser;
        friend class JSONSerializer;
    public:
        JSON() = default;

        /// Construct JSON as String
        JSON(const char *c_str);

        /// Construct JSON as String
        JSON(String str);

        /// Construct JSON as Array
        JSON(std::initializer_list<JSON> array);
        
        /// Construct JSON as Map
        JSON(std::map<String,JSON> map);

        bool isString() const;

        bool isArray() const;

        bool isNumber() const;

        bool isMap() const;

        /// Get this JSON node as a String.
        String & asString();
        
        /// Get this JSON node as a Vector 
        /// (From a JSON Array).
        Vector<JSON> & asVector();

        /// Get this JSON node as a Map.
        Map<String,JSON> & asMap();

        float asFloat();

        bool asBool();



        static JSON parse(String str);

        static JSON parse(std::istream & in);

        static String serialize(JSON & json);

        static void serialize(JSON & json,std::ostream & out);

        ~JSON() = default;
    };

    OMEGACOMMON_EXPORT std::istream & operator>>(std::istream & in,JSON & json);
    OMEGACOMMON_EXPORT std::ostream & operator<<(std::ostream & out,JSON & json);

    typedef Map<String,JSON> JSONMap;
    typedef Vector<JSON> JSONArray;

    struct OMEGACOMMON_EXPORT JSONConvertible {
        virtual void toJSON(JSON & j) = 0;
        virtual void fromJSON(JSON & j) = 0;
    };

    #define IJSONConvertible public ::OmegaCommon::JSONConvertible

};




#endif