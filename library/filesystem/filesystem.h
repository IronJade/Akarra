/*
*	Global specific functions
*
*/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H


namespace FileSystem
{
	class IFileManager;
	class CFileStream;
	class CFilePathManager;
	class CFileLibManager;
}

/*namespace std
{
	class vector<T>;
}*/

#include "../basic/basic.h"
#include <vector>
#include <list>

namespace FileSystem
{

	// read a string with a preset length from a file, returns a CCharString
	// with the string, must be matched with writeStringWithLength()
	Basic::CCharString readStringWithLength(CFileStream& file);

	// read a string with a terminating NULL character, returns a CCharString
	// with the string, must be matched with writeTerminatedString()
	Basic::CCharString readTerminatedString(CFileStream& file);

	// write a string to a file, saving the length for faster reads
	void writeStringWithLength(CFileStream& file, Basic::CCharString& string);

	// write a string to a file, including the terminating NULL character
	void writeTerminatedString(CFileStream& file, Basic::CCharString& string);

	// read an unicode string with a preset length from a file, returns a CWideString
	// with the string, must be matched with writeStringWithLength()
	Basic::CWideString readStringWithLengthW(CFileStream& file);

	// read a string with a terminating NULL character, returns a CWideString
	// with the string, must be matched with writeTerminatedString()
	Basic::CWideString readTerminatedStringW(CFileStream& file);

	// write an unicode string to a file, saving the length for faster reads
	void writeStringWithLengthW(CFileStream& file, Basic::CWideString& string);

	// write an unicode string to a file, including the terminating NULL character
	void writeTerminatedStringW(CFileStream& file, Basic::CWideString& string);

	// read a std::vector of any template from a file, must be matched with
	// writeStdVector(), and the templates must be equal
	template <class T> void readStdVector(CFileStream* file, std::vector<T>& vec);

	// writes a std::vector of any template to a file
	template <class T> void writeStdVector(CFileStream* file, std::vector<T>& vec);

	// read a std::list of any template from a file, must be matched with
	// writeStdVector(), and the templates must be equal
	template <class T> void readStdList(CFileStream* file, std::list<T>& l);

	// writes a std::list of any template to a file
	template <class T> void writeStdList(CFileStream* file, std::list<T>& l);

	// opens a file directly (without the involvement of a file manager),
	// using standard C/C++ functions, returns the filestream created
	CFileStream directOpen(const char* path, const char* flags);

	// close a file stream (without the involvement of a file manager)
	void directClose(CFileStream& file);

	// check if a file exists, returns true if it does
	bool fileExists(const char* path);

	// check if a directory exists (uses Win32 structures), returns true if it does
	bool directoryExists(const char* name);

	// counts all files that match 'filename' in the 'path', looks in sub-directories
	// if 'recursive' is true. The path must be submitted without the last slash,
	// the filename may include wildcards ("*.txt" for example)
	int countFiles(char* path, char* filename, bool recursive);

	// These two functions were removed due to suspicious usage
	//int countFiles(Basic::CCharString* filename_buffer, char* path, char* filename, bool recursive);
	//int getFilename(char* dest, char* path, char* filename, int index, bool recursive, int count = 0);

	// looks for all files that match the 'filename' search critera and places these
	// filenames into a std::vector of CCharStrings, including the paths
	void getFilenames(std::vector<Basic::CCharString>& vec, char* path, char* filename, bool recursive);

	// looks for all files that match the 'filename' search critera and places these
	// filenames into a std::vector of CCharStrings, without the paths
	void getFilenamesNoPath(std::vector<Basic::CCharString>& vec, char* path, char* filename, bool recursive);

}

#endif