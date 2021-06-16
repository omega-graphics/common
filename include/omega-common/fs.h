#include "utils.h"

#ifndef OMEGA_COMMON_FS_H
#define OMEGA_COMMON_FS_H

namespace OmegaCommon {
    namespace FS {

        /**
        @brief A platform specific filesystem path class.
        @paragraph 
        In addition to performing basic path parsing on construction and runtime, 
        it supports rapid string concatnation as well absolute path resolution.
        */
        class Path {
            String _str;
            struct Token {
                typedef enum : int {
                    ID,
                    Dot,
                    Slash,
                } Type;
                Type type;
                String str;
            };
            Vector<Token> tokens;
            void parse(const String & str);
            typedef Path SELF;
        public:
            const unsigned getTokenCount(){ return tokens.size();};
            String debugString(){
                std::ostringstream out;
                auto it = tokens.begin();
                while(it != tokens.end()){
                    out << "{Type:" << int(it->type) << ",Content:" << it->str << "}, " << std::flush;
                    ++it;
                };
                return out.str().c_str();
            };
            /**
             Appends a CString to the end of the path.
             @param str 
             @returns Path
            */
            SELF & append(const char *str);

            /**
             Appends a String to the end of the path.
             @param str 
             @returns Path
            */
            SELF & append(String & str);

            /// @name Concat Operators
            /// @{
            SELF operator+(const char *str);

            SELF operator+(String & str);
            /// @}
            /**
             Retrieve the path as a string. (Relative path) 
             @returns String &
            */
            String &str();

            /**
             Retrieves the filename part of the path (if it has one).
             @returns String
            */
            String filename();

            /**
             Retrieves the file extension of the path (if it has one).
             @returns String
            */
            String & ext();

            /**
             Retrieves the file extension of the path (if it has one).
             @returns String
            */
            String absPath();
            bool exists();
            Path(const char * str);
            Path(const String & str);
            ~Path();
            bool isFile();
            bool isDirectory();
            bool isSymLink();
        };

        StatusCode changeCWD(Path newPath);

        StatusCode createSymLink(Path  file,Path symlinkDest);

        StatusCode createDirectory(Path path);

        StatusCode deleteDirectory(Path path);

        class DirectoryIterator {
            Path path;
            bool _end;
            using SELF = DirectoryIterator;
            void *data;
            long loc;
        public:
            bool & end();
            explicit DirectoryIterator(Path path);
            Path operator*();
            SELF & operator++();
        };
    };
};

#endif