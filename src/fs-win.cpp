#include "omega-common/fs.h"

#include <windows.h>
#include <ShlObj.h>

#pragma comment(lib,"user32.lib")

namespace OmegaCommon::FS {

    bool Path::isDirectory(){
        DWORD attrs = GetFileAttributes(str().c_str());
        return attrs & FILE_ATTRIBUTE_DIRECTORY;
    };

    bool Path::isFile(){
        DWORD attrs = GetFileAttributes(str().c_str());
        return attrs & FILE_ATTRIBUTE_NORMAL;
    };

    bool Path::isSymLink(){
        DWORD attrs = GetFileAttributes(str().c_str());
        return attrs & FILE_ATTRIBUTE_REPARSE_POINT;
    };


    StatusCode createDirectory(Path path){
        BOOL success = CreateDirectoryA(path.absPath().c_str(),NULL);
        if(success){
            return Ok;
        }
        else {
            return Failed;
        };
    };

    StatusCode deleteDirectory(Path path){
        BOOL success = RemoveDirectoryA(path.absPath().c_str());
        if(success){
            return Ok;
        }
        else {
            return Failed;
        };
    };

    StatusCode createSymLink(Path file, Path symlinkDest){
        BOOL success = CreateSymbolicLinkA(symlinkDest.absPath().c_str(),file.absPath().c_str(),file.isDirectory()? SYMBOLIC_LINK_FLAG_DIRECTORY : NULL);
        if(success){
            return Ok;
        }
        else {
            return Failed;
        };
    };

    StatusCode changeCWD(Path newPath){
        BOOL success = SetCurrentDirectoryA(newPath.absPath().c_str());
        if(success){
            return Ok;
        }
        else {
            return Failed;
        };
    };


    DirectoryIterator::DirectoryIterator(Path path):path(path),result_path(""),_end(false){
        data = new WIN32_FIND_DATAA;
        HANDLE hFind;
        hFind = FindFirstFileA(path.str().c_str(),(LPWIN32_FIND_DATAA)data);
        if(hFind == INVALID_HANDLE_VALUE){
            /// Fuck!
        }
        dirp = hFind;
    };

    Path DirectoryIterator::operator*(){
        LPWIN32_FIND_DATAA d = (LPWIN32_FIND_DATAA)data;
        return path + d->cFileName;
    };

    DirectoryIterator & DirectoryIterator::operator++(){
        BOOL success = FindNextFileA(dirp,(LPWIN32_FIND_DATAA)data);
        if(!success)
            _end = true;
        return *this;
    };
};