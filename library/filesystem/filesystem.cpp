#include "filesystem.h"
#include "../basic/CCharString.h"
#include "../basic/CWideString.h"
#include "CFileStream.h"
#include <windows.h>


namespace FileSystem
{

	const int STRINGBUFFER_LENGTH	= 1024;
	const int STRINGBUFFER_LENGTHW	= STRINGBUFFER_LENGTH / 2;
	char stringbuffer[STRINGBUFFER_LENGTH];

	
	// read a string with a preset length from a file, returns a CharString
	// with the string
	Basic::CCharString readStringWithLength(CFileStream& file)
	{
		// read length
		int stringlength = file.readInt();
		// if the required length is longer than the stringbuffer we must
		// read one part at a time
		Basic::CCharString tempstring("");
		while (stringlength >= STRINGBUFFER_LENGTH)
		{
			file.read(stringbuffer, STRINGBUFFER_LENGTH - 1);
			stringbuffer[STRINGBUFFER_LENGTH - 1] = 0;

			tempstring += stringbuffer;
			stringlength -= STRINGBUFFER_LENGTH - 1;
		}

		// read the remaining string
		file.read(stringbuffer, stringlength);
		stringbuffer[stringlength] = 0;  // Terminating zero

		tempstring += stringbuffer;
		
		return Basic::CCharString(tempstring); 
	}

	// read a string with a terminating NULL character, returns a CCharString
	// with the string
	Basic::CCharString readTerminatedString(CFileStream& file)
	{
		int index(0);
		char chr = ' ';
		Basic::CCharString tempstring("");

		// read until end of string
		while(chr != 0)
		{
			chr = file.readChar();
			stringbuffer[index++] = chr;

			// check if the string buffer overflows
			if (index == (STRINGBUFFER_LENGTH - 1))
			{
				// copy the current stringbuffer
				stringbuffer[index] = 0;
				tempstring += stringbuffer;

				// reset counter
				index = 0;
				stringbuffer[0] = 0;
			}
		}

		// add remaining string to result
		tempstring += stringbuffer;

		return Basic::CCharString(tempstring);
	}

	// write a string to a file, saving the length for faster reads
	void writeStringWithLength(CFileStream& file, Basic::CCharString& string)
	{
		file.write(string.getStringLength());
		file.write(string.getText(), string.getStringLength());
	}

	// write a string to a file, including the terminating NULL character
	void writeTerminatedString(CFileStream& file, Basic::CCharString& string)
	{
		int stringlength = string.getStringLength();
		file.write(string.getString(), stringlength + 1); // + 1 -> terminating zero
	}

	// read an unicode string with a preset length from a file, returns a CWideString
	// with the string, must be matched with writeStringWithLength()
	Basic::CWideString readStringWithLengthW(CFileStream& file)
	{
		// read length
		int stringlength = file.readInt();
		wchar_t *wbuffer = (wchar_t *)stringbuffer;
		// if the required length is longer than the stringbuffer we must
		// read one part at a time
		Basic::CWideString tempstring(L"");
		while (stringlength >= STRINGBUFFER_LENGTHW)
		{
			file.read(wbuffer, STRINGBUFFER_LENGTH - 2);
			wbuffer[STRINGBUFFER_LENGTHW - 1] = 0;

			tempstring += wbuffer;
			stringlength -= STRINGBUFFER_LENGTHW - 1;
		}

		// read the remaining string
		file.read(wbuffer, stringlength * 2);
		wbuffer[stringlength] = 0;  // Terminating zero

		tempstring += wbuffer;
		
		return Basic::CWideString(tempstring); 
	}

	// read a string with a terminating NULL character, returns a CWideString
	// with the string, must be matched with writeTerminatedString()
	Basic::CWideString readTerminatedStringW(CFileStream& file)
	{
		int index(0);
		wchar_t chr = L' ';
		wchar_t *wbuffer = (wchar_t *)stringbuffer;
		Basic::CWideString tempstring(L"");

		// read until end of string
		while(chr != 0)
		{
			file.read(&chr, 2);
			wbuffer[index++] = chr;

			// check if the string buffer overflows
			if (index == (STRINGBUFFER_LENGTHW - 1))
			{
				// copy the current stringbuffer
				wbuffer[index] = 0;
				tempstring += wbuffer;

				// reset counter
				index = 0;
				wbuffer[0] = 0;
			}
		}

		// add remaining string to result
		tempstring += wbuffer;

		return Basic::CWideString(tempstring);
	}

	// write an unicode string to a file, saving the length for faster reads
	void writeStringWithLengthW(CFileStream& file, Basic::CWideString& string)
	{
		file.write(string.getStringLength());
		file.write(string.getText(), string.getStringLength());
	}

	// write an unicode string to a file, including the terminating NULL character
	void writeTerminatedStringW(CFileStream& file, Basic::CWideString& string)
	{
		int stringlength = string.getStringLength();
		file.write(string.getString(), (stringlength + 1) * 2); // + 1 -> terminating zero, * 2 -> wide char
	}


	// read a std::vector of any template from a file, must be matched with
	// writeStdVector(), and the templates must be equal
	template <class T> void readStdVector(CFileStream* file, std::vector<T>& vec)
	{
		int nbr_elements = file->readInt();

		for(int i(0); i < nbr_elements; i++)
		{
			T temp;
            file->read(&temp, sizeof(T));
			vec.push_back(temp);
		}
	}

	// writes a std::vector of any template to a file
	template <class T> void writeStdVector(CFileStream* file, std::vector<T>& vec)
	{
		int nbr_elements = vec.size();

		file->write(nbr_elements);

		for(int i(nbr_elements); i >= 0; i--)
		{
			T temp = vec[i];
			file->write(&temp, sizeof(T));
		}
	}

    // read a std::list of any template from a file, must be matched with
    // writeStdVector(), and the templates must be equal
	template <class T> void readStdList(CFileStream* file, std::list<T>& l)
    {
        int nbr_elements = file->readInt();

        std::list<T>::iterator it     = l.begin();
        std::list<T>::iterator it_end = l.end();

        while(it != it_end)
        {
            T temp; 
            file->read(&temp, sizeof(T));
            l.push_back(temp);
            it++;
        }
    }

	// writes a std::list of any template to a file
	template <class T> void writeStdList(CFileStream* file, std::list<T>& l)
    {
        int nbr_elements = l.size();

        file->write(nbr_elements);

        std::list<T>::reverse_iterator it     = l.rbegin();
        std::list<T>::reverse_iterator it_end = l.rend();

        while(it != it_end)
        {
            T temp = *it;
            file->write(&temp, sizeof(T));
            it++;
        }
    }


	// opens a file directly (without the involvement of a file manager),
	// using standard C/C++ functions, returns the filestream created
	CFileStream directOpen(const char* path, const char* flags)
	{
		long start, end;
		bool uni;
		FILE* f;

		f = fopen(path, flags);  

		if(f)
		{
			start = ftell(f);
			fseek(f, 0, SEEK_END);
			end = ftell(f);
			fseek(f, 0, SEEK_SET);
			
			unsigned char buf[2];
			
			fread(&buf, 2, 1, f);  // Read first two bytes
			
			if (buf[0] == 0xFF && buf[1] == 0xFE)  // Compare with unicode bytes
			{
				uni = true;
			}
			else
			{
				fseek(f, 0, SEEK_SET);  // Not unicade, so step back to start
			}

			return CFileStream(f, start, end, uni);

		}

		return CFileStream(0,0,0,false);
	}

	// close a file stream (without the involvement of a file manager)
	void directClose(CFileStream& file)
	{
		if (file.isOk())
		{
			fclose(file.getFilePointer());
		}
	}

	// check if a file exists, returns true if it does
	bool fileExists(const char* path)
	{
		FILE *f = fopen(path, "rb");

		if (!f)
		{
			return false;
		}

		fclose(f);
		
		return true;
	}

	// check if a directory exists (uses Win32 structures), returns true if it does
	bool directoryExists(const char* name)
	{
		WIN32_FIND_DATA fd;
		HANDLE handle = FindFirstFile(name, &fd);

		if(handle != INVALID_HANDLE_VALUE)
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				return true;
			}
		}
		return false;
	}

	// counts all files that match 'filename' in the 'path', looks in sub-directories
	// if 'recursive' is true. The path must be submitted without the last slash,
	// the filename may include wildcards ("*.txt" for example)
	int countFiles(char* path, char* filename, bool recursive)
	{
		WIN32_FIND_DATA data;
		HANDLE handle;

		int count = 0;

		char temp[512];

		if (recursive)
		{
			sprintf(temp, "%s/*", path);

			// find all sub directories
			if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
			{
				// check if it's a directory, and that it's not the parent directory
				// or the root directory
				do
				{
					if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
						strcmp(data.cFileName, ".") && 
						strcmp(data.cFileName, ".."))
					{
						char new_path[512];
						sprintf(new_path, "%s/%s", path, data.cFileName);
						count += countFiles(new_path, filename, true);
					}
				} while(::FindNextFile(handle, &data));
			}
			::FindClose(handle);
		}

		sprintf(temp, "%s/%s", path, filename);

		if( (handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
		{
			do
			{
				count++;
			} while(::FindNextFile(handle, &data));
			::FindClose(handle);
		}

		return count;
	}

	/*
	int countFiles(Basic::CCharString* filename_buffer, char* path, char* filename, bool recursive)
	{
		WIN32_FIND_DATA data;
		HANDLE handle;

		int count = 0;

		char temp[512];

		char buffer_path[512];

		if (recursive)
		{
			sprintf(temp, "%s/*", path);
			if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
			{
				if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
					!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
					!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
					strcmp(data.cFileName, ".") && 
					strcmp(data.cFileName, ".."))
				{
					char new_path[512];
					sprintf(new_path, "%s/%s", path, data.cFileName);
					count += countFiles(&filename_buffer[count], new_path, filename, true);
				}

				while( ::FindNextFile(handle, &data))
				{
					if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
						strcmp(data.cFileName, ".") && 
						strcmp(data.cFileName, ".."))
					{
						char new_path[512];
						sprintf(new_path, "%s/%s", path, data.cFileName);
						count += countFiles(&filename_buffer[count], new_path, filename, true);
					}
				}
			}
			::FindClose(handle);
		}

		sprintf(temp, "%s/%s", path, filename);

		if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
		{
			sprintf(buffer_path, "%s/%s", path, data.cFileName);
			filename_buffer[count] = buffer_path;
			count++;
			while(::FindNextFile(handle, &data))
			{
				sprintf(buffer_path, "%s/%s", path, data.cFileName);
				filename_buffer[count] = buffer_path;
				count++;
			}
			::FindClose(handle);
		}

		return count;
	}

	int getFilename(char* dest, char* path, char* filename, int index, bool recursive, int count)
	{
		WIN32_FIND_DATA data;
		HANDLE handle;

		char temp[512];

		if (recursive)
		{
			sprintf(temp, "%s/*", path);
			if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
			{
				if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
					!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
					!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
					strcmp(data.cFileName, ".") && 
					strcmp(data.cFileName, ".."))
				{
					char new_path[512];
					sprintf(new_path, "%s/%s", path, data.cFileName);
					count = getFilename(dest, new_path, filename, index, true, count);
				}

				while(::FindNextFile(handle, &data))
				{
					if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
						strcmp(data.cFileName, ".") && 
						strcmp(data.cFileName, ".."))
					{
						char new_path[512];
						sprintf(new_path, "%s/%s", path, data.cFileName);
						count = getFilename(dest, new_path, filename, index, true, count);
					}
				}
				::FindClose(handle);
			}
		}

		sprintf(temp, "%s/%s", path, filename);

		if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
		{
			if (index == count)
			{
				sprintf(dest, "%s/%s", path, data.cFileName);
				::FindClose(handle);

				return count + 1;
			}
			else
			{
				count++;
				while(::FindNextFile(handle, &data))
				{
					if (index == count)
					{
						sprintf(dest, "%s/%s", path, data.cFileName);
						::FindClose(handle);
						
						return count + 1;
					}
					count++;
				}
			}
			::FindClose(handle);
		}
		return count;
	}
	*/

	// looks for all files that match the 'filename' search critera and places these
	// filenames into a std::vector of CCharStrings, including the paths
	void getFilenames(std::vector<Basic::CCharString>& vec, char* path, char* filename, bool recursive)
	{
		WIN32_FIND_DATA data;
		HANDLE handle;

		char temp[512];

		char buffer_path[512];

		if (recursive)
		{
			sprintf(temp, "%s/*", path);
			if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
			{
				do
				{
					if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
						strcmp(data.cFileName, ".") && 
						strcmp(data.cFileName, ".."))
					{
						char new_path[512];
						sprintf(new_path, "%s/%s", path, data.cFileName);
						getFilenames(vec, new_path, filename, true);
					}
				} while( ::FindNextFile(handle, &data));
			}
			::FindClose(handle);
		}

		sprintf(temp, "%s/%s", path, filename);

		if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
		{
			do
			{
				sprintf(buffer_path, "%s/%s", path, data.cFileName);
				vec.push_back(Basic::CCharString(buffer_path));
			} while(::FindNextFile(handle, &data));
			::FindClose(handle);
		}
	}

	// looks for all files that match the 'filename' search critera and places these
	// filenames into a std::vector of CCharStrings, without the paths
	void getFilenamesNoPath(std::vector<Basic::CCharString>& vec, char* path, char* filename, bool recursive)
	{
		WIN32_FIND_DATA data;
		HANDLE handle;

		char temp[512];

		if (recursive)
		{
			sprintf(temp, "%s/*", path);
			if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
			{
				do
				{
					if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
						!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
						strcmp(data.cFileName, ".") &&
						strcmp(data.cFileName, ".."))
					{
						char new_path[512];
						sprintf(new_path, "%s/%s", path, data.cFileName);
						getFilenamesNoPath(vec, new_path, filename, true);
					}
				} while( ::FindNextFile(handle, &data));
			}
			::FindClose(handle);
		}

		char buffer_path[512];
		sprintf(temp, "%s/%s", path, filename);

		if((handle = ::FindFirstFile(temp, &data)) != INVALID_HANDLE_VALUE )
		{
			do
			{
				sprintf(buffer_path, "%s", data.cFileName);
				vec.push_back(Basic::CCharString(buffer_path));
			} while(::FindNextFile(handle, &data));
			::FindClose(handle);
		}
	}

}  // End namespace
