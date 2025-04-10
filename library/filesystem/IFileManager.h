/*
*	interface IFileManager - Tomas
*
*/


#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H

#include "CFileStream.h"

namespace FileSystem{
    
    class IFileManager{

    public:
        virtual bool directoryExists(const char* name) = 0;
        virtual bool fileExists(const char* name) = 0;

        virtual bool createDirectory(const char* name) = 0;
        virtual bool deleteDirectory(const char* name) = 0;

        virtual void deleteFile(const char* name) = 0;
		virtual void renameFile(char* oldname, char* newname) = 0;

        virtual CFileStream open(const char* filename) = 0;
		virtual	CFileStream openWrite(const char* filename) = 0;

        virtual void close(CFileStream& fo) = 0;

		virtual bool isOk() = 0;
    };

}

#endif