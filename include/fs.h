#include "utils.h"

#ifndef OMEGA_COMMON_FS_H
#define OMEGA_COMMON_FS_H

namespace OmegaCommon {
    namespace FS {
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
        public:
            const unsigned getTokenCount(){ return tokens.size();};
            String debugString(){
                std::ostringstream out;
                auto it = tokens.begin();
                while(it != tokens.end()){
                    out << "{Type:" << int(it->type) << ",Content:" << it->str << "}, " << std::flush;
                    ++it;
                };
                return out.str();
            };
            String &str();
            String filename();
            String & ext();
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
        public:
            bool & end();
            explicit DirectoryIterator(Path path);
            SELF & operator++();
        };
    };
};

#endif