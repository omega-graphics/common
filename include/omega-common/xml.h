#include "utils.h"

#ifndef OMEGA_COMMON_XML_H
#define OMEGA_COMMON_XML_H

namespace OmegaCommon {
    class XMLParser;
    class XMLSerializer;

    class OMEGACOMMON_EXPORT XML {
        String tag;
        Map<String,String> tagProps;
        Vector<XML> children;

        static std::unique_ptr<XMLParser> parser;

        static std::unique_ptr<XMLSerializer> serializer;

        static XML __parse(std::istream & in);
        static void __serialize(XML &xml,std::ostream & out);
    public:
        String & getName();
        String & getProp(const String & propName);
        Vector<XML> & getChildren();

        static XML parse(String str);
        static XML parse(std::istream & is);
    };

    // class XMLDocument {

    // public:
    //     static XMLDocument parse(String str);
    //     static XMLDocument parse(std::istream & is);
    // };
}

#endif