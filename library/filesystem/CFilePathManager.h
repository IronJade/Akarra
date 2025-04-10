/*
*  class FileManager - Tomas
*
*  Purpose: To take care of file locations and lib-files
*
*  Note:  - This has to be rewritten for different OS's
* 
*         - When a call to open() has been done you must send
*           the copied object to the close() function!
*
*  Example:
*
*    CFileStream fs = fileManagerObject->open("file.txt");
*    
*    fs.read(buffer, fs.getSize());
*
*    fileManagerObject->close(fs);
*/


#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include <windows.h>
#include "CFileStream.h"
#include "IFileManager.h"
#include "../basic/CCharString.h"

namespace FileSystem{

    class CFilePathManager : public IFileManager{
        public:
			CFilePathManager::CFilePathManager(char* path);
			
			bool directoryExists(const char* name);
			bool fileExists(const char* name);
			
			bool createDirectory(const char* name);
			bool deleteDirectory(const char* name);

			void deleteFile(const char* name);
			void renameFile(char* oldname, char* newname);

			CFileStream open(const char* filename);       // open just for reading.
			CFileStream openWrite(const char* filename);  // If the file exists, it's overwritten
			void close(CFileStream& fo);

			bool isOk(){ return true; }
      
        private:
			bool fixUnicode(FILE* f);
			bool find(FILE* f, const char* string, bool start);

			Basic::CCharString m_path;  // absolut path
    };
}

#endif