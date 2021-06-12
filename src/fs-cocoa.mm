#include "fs.h"

#include <unistd.h>
#include <dirent.h>

#import <Foundation/Foundation.h>

namespace OmegaCommon::FS { 

    bool Path::isDirectory(){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSString *str = [[NSString alloc] initWithUTF8String:_str.c_str()];
        NSError *error;
        NSDictionary<NSFileAttributeKey,id> * dict = [fileManager attributesOfItemAtPath:str error:&error];
        NSFileAttributeType fileType = dict[NSFileType];
        return fileType == NSFileTypeDirectory;
    };

    bool Path::isFile(){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSString *str = [[NSString alloc] initWithUTF8String:_str.c_str()];
        NSError *error;
        NSDictionary<NSFileAttributeKey,id> * dict = [fileManager attributesOfItemAtPath:str error:&error];
        NSFileAttributeType fileType = dict[NSFileType];
        return fileType == NSFileTypeRegular;
    };

    bool Path::isSymLink(){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSString *str = [[NSString alloc] initWithUTF8String:_str.c_str()];
        NSError *error;
        NSDictionary<NSFileAttributeKey,id> * dict = [fileManager attributesOfItemAtPath:str error:&error];
        NSFileAttributeType fileType = dict[NSFileType];
        return fileType == NSFileTypeSymbolicLink;
    };

    StatusCode changeCWD(Path newPath){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSURL *fileUrl = [NSURL fileURLWithFileSystemRepresentation:newPath.str().c_str() isDirectory:YES relativeToURL:nil];
        [fileManager changeCurrentDirectoryPath:fileUrl.absoluteString];
        return Ok;
    };

    StatusCode createSymLink(Path file, Path symlinkDest){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSURL *src = [NSURL fileURLWithFileSystemRepresentation:file.str().c_str() isDirectory:YES relativeToURL:nil];
        NSURL *dest = [NSURL fileURLWithFileSystemRepresentation:symlinkDest.str().c_str() isDirectory:YES relativeToURL:nil];
        NSError *error;
        [fileManager createSymbolicLinkAtURL:src withDestinationURL:dest error:&error];
        if(error.code >= 0){
            return Ok;
        }
        else {
            return Failed;
        };
    };

    StatusCode createDirectory(Path path){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSURL *fileUrl = [NSURL fileURLWithFileSystemRepresentation:path.str().c_str() isDirectory:YES relativeToURL:nil];
        NSError *error;
        [fileManager createDirectoryAtURL:fileUrl withIntermediateDirectories:YES attributes:nil error:&error];
        if(error.code >= 0){
            return Ok;
        }
        else {
            return Failed;
        };
    };

    StatusCode deleteDirectory(Path path){
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSURL *fileUrl = [NSURL fileURLWithFileSystemRepresentation:path.str().c_str() isDirectory:YES relativeToURL:nil];
        NSError *error;
        [fileManager removeItemAtURL:fileUrl error:&error];
        if(error.code >= 0){
            return Ok;
        }
        else {
            return Failed;
        };
    };

    DirectoryIterator::DirectoryIterator(Path path):path(path){
        DIR * dir = opendir(path.str().c_str());
    };
    
};