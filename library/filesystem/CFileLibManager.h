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

    class CFileLibManager : public IFileManager{
        public:
			CFileLibManager::CFileLibManager(char* libfile);
			CFileLibManager::~CFileLibManager(){ if(mp_file) fclose(mp_file); }
			
			bool directoryExists(const char* name);
			bool fileExists(const char* name);
			
			bool createDirectory(const char* name);
			bool deleteDirectory(const char* name);

			void deleteFile(const char* name);
			void renameFile(char* oldname, char* newname) {}

			CFileStream open(const char* filename);       // open just for reading.

			// We're not able to write in libfiles yet...
			CFileStream openWrite(const char* filename){ return CFileStream(0,0,0,false); }

			void close(CFileStream& fo);
      
			bool isOk(){ return m_ok; }

        private:
			bool fixUnicode(FILE* f);
			bool find(FILE* f, const char* string, bool start);

			bool m_ok;

			int m_headerSize;

			FILE* mp_file;
						
    };
}

#endif