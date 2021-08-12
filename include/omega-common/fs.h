#include "utils.h"

#ifndef OMEGA_COMMON_FS_H
#define OMEGA_COMMON_FS_H

#if defined(_WIN32)
#define PATH_SLASH '\\'
#else 
#define PATH_SLASH '/'
#endif

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
            
            String _dir;
            String _fname;
            String _ext;

            bool isRelative;

            void parse(const String & str);
            typedef Path SELF;
        public:
            // const unsigned getTokenCount(){ return tokens.size();};
            // String debugString(){
            //     std::ostringstream out;
            //     auto it = tokens.begin();
            //     while(it != tokens.end()){
            //         out << "{Type:" << int(it->type) << ",Content:" << it->str << "}, " << std::flush;
            //         ++it;
            //     };
            //     return out.str().c_str();
            // };
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

            /**
             Appends a TStrRef to the end of the path.
             @param str 
             @returns Path
            */
            SELF & append(TStrRef & str);

            /// @name Concat Operators
            /// @{
            SELF operator+(const char *str);

            SELF operator+(String & str);

             SELF operator+(TStrRef & str);
            /// @}
            /**
             Retrieve the path as a string. (Relative path) 
             @returns String &
            */
            String &str();

            /**
             Retrieves the top directory part of the path (if it has one).
             @returns String
            */
            String & dir();

            /**
             Retrieves the filename part of the path (if it has one).
             @returns String
            */
            String & filename();

            /**
             Retrieves the file extension of the path (if it has one).
             @returns String
            */
            String & ext();

            /**
             Gets the absolute path of this path
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
            Path result_path;
            bool _end;
            using SELF = DirectoryIterator;
            void *data;
            #ifdef _WIN32
            void *dirp;
            #endif
            long loc;
        public:
            bool & end();
            explicit DirectoryIterator(Path path);
            Path operator*();
            SELF & operator++();
            ~DirectoryIterator();
        };
    };
};

#endif