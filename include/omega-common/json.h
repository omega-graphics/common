#include "utils.h"
#include <sstream>

#ifndef OMEGA_COMMON_JSON_H
#define OMEGA_COMMON_JSON_H

namespace OmegaCommon {

    class JSONParser;

    class JSON {
        static std::unique_ptr<JSONParser> parser;

        static JSON __parse(std::istream & in);
        static void __serialize(JSON &json,std::ostream & out);

        unsigned ty;

        void *_data;
        friend class JSONParser;
    public:


        String asString();
        
        Vector<JSON> asVector();

        Map<String,JSON> asMap();

        float asFloat();

        bool asBool();



        static JSON parse(String str);

        static JSON parse(std::istringstream & in);

        static JSON parse(std::ifstream & in);


        static String serialize(JSON json);

        static void serialize(JSON json,std::ofstream & out);

        static void serialize(JSON json,std::ostringstream & out);
    
    };

    std::istream & operator>>(std::istream & in,JSON & json);
    std::ostream & operator<<(std::ostream & in,JSON & json);

};



#endif