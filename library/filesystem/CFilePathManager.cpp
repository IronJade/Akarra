#include "CFilePathManager.h"

using namespace Basic;

namespace FileSystem
{

	CFilePathManager::CFilePathManager(char* path) : m_path(path)
	{
		if(m_path[m_path.getStringLength() - 1] != '/')
		{
			m_path += '/';
		}
	}

	CFileStream CFilePathManager::open(const char* filename)
	{
		long start, end;
		bool uni;
		FILE* temp;

		CCharString path = m_path + filename;

		temp = fopen(path.getString(), "rb");  

		if(temp)
		{
			start = ftell(temp);
			fseek(temp, 0, SEEK_END);
			end = ftell(temp);
			fseek(temp, 0, SEEK_SET);
			
			if(fixUnicode(temp))
			{
				uni = true;
				start += 2;                     // Start is after unicode bytes
			}
			return CFileStream(temp, start, end, uni);
		}

		return CFileStream(0,0,0,false);

	}

	CFileStream CFilePathManager::openWrite(const char* filename)
	{
		CCharString path = m_path + filename;

		FILE* temp = fopen(path.getString(), "wb");

		return CFileStream(temp, 0, 0, false);
	}

	void CFilePathManager::close(CFileStream& f)
	{
		FILE* tp = f.getFilePointer();

		if(tp)
		{
			fclose(tp);
		}
	}

	bool CFilePathManager::directoryExists(const char* name)
	{
		WIN32_FIND_DATA fd;
		HANDLE handle;
		
		if((handle = ::FindFirstFile(name, &fd)) != INVALID_HANDLE_VALUE)
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				return true;
			}
		}
		return false;
	}

	bool CFilePathManager::fileExists(const char* name)
	{
		WIN32_FIND_DATA fd;
		HANDLE handle;
		
		if((handle = ::FindFirstFile(name, &fd)) != INVALID_HANDLE_VALUE)
		{
			return true;
		}
		return false;
	}

	
	bool CFilePathManager::createDirectory(const char* name)
	{
		return ::CreateDirectory(name, NULL) ? true : false;
	}
	
	bool CFilePathManager::deleteDirectory(const char* name)
	{
		return ::RemoveDirectory(name) ? true : false;
	}

	void CFilePathManager::deleteFile(const char* name)
	{
		CCharString filename(m_path);
		filename += (char*)name;

		DeleteFile((char*)filename);
	}

	void CFilePathManager::renameFile(char* oldname, char* newname)
	{
		CCharString fullold(m_path), fullnew(m_path);
		fullold += (char*)oldname;
		fullnew += (char*)newname;

		rename(fullold, fullnew);
	}

	/*
	*    PRIVATE FUNCTIONS
	*/
	bool CFilePathManager::fixUnicode(FILE* f)
	{
		unsigned char buf[2];
		
		fread(&buf, 2, 1, f);  // Read first two bytes
		
		if (buf[0] == 0xFF && buf[1] == 0xFE)  // Compare with unicode bytes
		{
			return true;
		}

		fseek(f, 0, SEEK_SET);  // Not unicade, so step back to start

		return false;
	}
	
	bool CFilePathManager::find(FILE* f, const char* string, bool start)
	{
		int counter(0);
		int length = strlen(string);

		fseek(f, 0, SEEK_SET);
		
		char c;
		while((c = (char)getc(f)) != EOF)  // get character
		{
			if(c == string[counter] || (c - 32) == string[counter] || (c + 32) == string[counter])  // compare it with passed string's chars (Ignore upper-lowercase)
			{
				counter++;
				if(counter == length)  // true if the strings match
				{
					if(start)           // Sets the filepointer to beginning of string
					{
						fseek(f, -counter, SEEK_CUR);
					}
					return true;
				}
			}
			else
			{
				counter = 0;  // Reset counter if chars didn't match
			}
		}
		return false;
	}


}

