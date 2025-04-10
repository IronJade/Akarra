#include "CFileLibManager.h"

using namespace Basic;

namespace FileSystem{

	CFileLibManager::CFileLibManager(char* libfile) : m_ok(true)
	{		
		if(!(mp_file = fopen(libfile, "rb")))
		{
			m_ok = false;
		}
		if(m_ok)
		{
			if((fscanf(mp_file, "%i", &m_headerSize)) != 1)
			{
				m_ok = false;
			}
		}
	}

	CFileStream CFileLibManager::open(const char* filename)
	{
		long start, end;
		bool uni = false;

		if(find(mp_file, filename, false))
		{
			if((fscanf(mp_file, "%i", &start)) != 1)
			{
				::MessageBox(0, "Error in gfx libfile", "Error", MB_ICONERROR);	
				return CFileStream(0,0,0,false);
			}
			if((fscanf(mp_file, "%i", &end)) != 1)
			{
				::MessageBox(0, "Error in gfx libfile", "Error", MB_ICONERROR);	
				return CFileStream(0,0,0,false);
			}
			
			fseek(mp_file, start + m_headerSize, SEEK_SET);
			
			if(fixUnicode(mp_file))
			{
				uni = true;
				start += 2;                  // Start is after unicode bytes
			}

			return CFileStream(mp_file, start, end, uni);
		}

		return CFileStream(0,0,0,false);

	}

	void CFileLibManager::close(CFileStream& f)
	{
	}

	bool CFileLibManager::directoryExists(const char* name)
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

	bool CFileLibManager::fileExists(const char* name)
	{
		if(find(mp_file, name, false))
		{
			return true;
		}
		return false;
	}

	
	bool CFileLibManager::createDirectory(const char* name)
	{
		return false;
	}
	
	bool CFileLibManager::deleteDirectory(const char* name)
	{
		return false;
	}

	void CFileLibManager::deleteFile(const char* name)
	{
	}

	/*
	*    PRIVATE FUNCTIONS
	*/
	bool CFileLibManager::fixUnicode(FILE* f)
	{
		unsigned char buf[2];
		
		fread(&buf, 2, 1, f);  // Read first two bytes
		
		if (buf[0] == 0xFF && buf[1] == 0xFE)  // Compare with unicode bytes
		{
			return true;
		}

		fseek(f, -2, SEEK_CUR);  // Not unicade, so step back to start

		return false;
	}
	
	bool CFileLibManager::find(FILE* f, const char* string, bool start)
	{
		int counter(0);
		int length = strlen(string);

		fseek(f, 0, SEEK_SET);
		
		char c;
		while((c = getc(f)) != EOF)  // get character
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

