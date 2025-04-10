/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    wchar_t implementation of CCharString. Templates, anyone? =)
 **/

#if defined (_MSC_VER)  
#define  strlwr  _strlwr
#define  strupr  _strupr
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <winnls.h>
#include "CWideString.h"
#include "CCharString.h"
#include "Basic.h"

namespace Basic
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString()
{
	m_clear();

	// create an empty string (so that this constructor behaves exactly like the other
	// constructor, just that the created string will be empty)
	if (!m_createMem(1))
	{
		return;
	}

	ma_text[0] = 0; // equals L""
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const wchar_t *) (constructor)
//
// Description:	Constructor with a string parameter.
// Parameters:	const wchar_t *text:	A pointer to the string that should be stored.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString:: CWideString (const wchar_t* text)
{
    // clear all data
	m_clear();

	// get length of input string
	int length = wcslen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const wchar_t) (constructor)
//
// Description:	Constructor with a single character as a parameter.
// Parameters:	const wchar_t chr:	A character which should be stored in the string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const wchar_t chr)
{
    // clear all data
	m_clear();

	// create string with the size of 2 (the character + null)
	if (!m_createMem(2))
	{
		return;
	}

	// setup the string
	ma_text[0] = chr;
	ma_text[1] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const int) (constructor)
//
// Description:	Constructor with an integer parameter.
// Parameters:	const int value:	An integer which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const int value)
{
    // clear all data
	m_clear();

	// the longest string that the integer will need is 12 since the longest value for an int is -2147483648
	wchar_t text[12];

	// convert the value to a string
	_itow(value, text, 10);

	// get the length of the string
	int length = wcslen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const unsigned long) (constructor)
//
// Description:	Constructor with an integer parameter.
// Parameters:	const unsigned long value:	An integer which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const unsigned long value)
{
    // clear all data
	m_clear();

	// the longest string that the unsigned long will need is 21 since the maximum value for an unsigned long is 18446744073709551616
	wchar_t text[21];

	// convert the value to a string
	_ultow(value, text, 10);

	// get the length of the string
	int length = wcslen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const float) (constructor)
//
// Description:	Constructor with a float parameter.
// Parameters:	const float value:	A float which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const float value)
{
    // clear all data
	m_clear();

	// create a string big enough to hold the float
	wchar_t text[25];

	// convert the float into a string
	//gcvt((double)value, 15, text);
	swprintf(text, L"%f", (double)value);

	// get the length of the converted string
	int length = wcslen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == L'.')
	{
		// add a '0' to the string, to make it look better
		wcscat(text, L"0");
		// re-calculate the length of the string
		length = wcslen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const double) (constructor)
//
// Description:	Constructor with a double parameter.
// Parameters:	const double value:	A double which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const double value)
{
    // clear all data
	m_clear();

	// create a string big enough to hold the double
	wchar_t text[25];

	// convert the double into a string
	//gcvt(value, 15, text);
	swprintf(text, L"%f", value);

	// get the length of the converted string
	int length = wcslen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == L'.')
	{
		// add a '0' to the string, to make it look better
		wcscat(text, L"0");
		// re-calculate the length of the string
		length = wcslen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const CWideString *) (constructor)
//
// Description:	Constructor that creates a copy of another CWideString's text.
// Parameters:	const CWideString *other:	Pointer to a CWideString object from which the
//											string data will be taken.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const CWideString *other)
{
    // clear all data
	m_clear();

	// get the length of the string to be copied
	int length = other->getStringLength();

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, other->ma_text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const CWideString &) (constructor)
//
// Description:	Constructor that creates a copy of another CWideString's text.
// Parameters:	const CWideString &other:	Reference to a CWideString object from which
//											the string data will be taken.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const CWideString &other)
{
    // clear all data
	m_clear();

	// get the length of the string to be copied
	int length = other.getStringLength();

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, other.ma_text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const CWideString *, const int) (constructor)
//
// Description:	Constructor that creates a copy of another CWideString's text, with a
//				limited number of characters.
// Parameters:	const CWideString *other:	Pointer to a CWideString object from which
//											the string data will be taken.
//				const int num:				The maximum number of characters to be copied.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const CWideString *other, const int num)
{
    // clear all data
	m_clear();

	// get the length of the string to be copied
	int length = other->getStringLength();

	// make sure the length is less than the maximum allowed
	if (length > num)
	{
		length = num;
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, other->ma_text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const CWideString &, const int) (constructor)
//
// Description:	Constructor that creates a copy of another CWideString's text, with a
//				limited number of characters.
// Parameters:	const CWideString &other:	Reference to a CWideString object from which
//											the string data will be taken.
//				const int num:				The maximum number of characters to be copied.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const CWideString &other, const int num)
{
    // clear all data
	m_clear();

	// get the length of the string to be copied
	int length = other.getStringLength();

	// make sure the length is less than the maximum allowed
	if (length > num)
	{
		length = num;
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, other.ma_text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString(const wchar_t *, const int) (constructor)
//
// Description:	Constructor which copies a string with a limited length.
// Parameters:	const wchar_t *text:	The string to be copied.
//				const int num:		The maximum length of the string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::CWideString(const wchar_t *text, const int num)
{
    // clear all data
	m_clear();

	// get length of input string
	int length = wcslen(text);

	// make sure the length is less than the maximum allowed
	if (length > num)
	{
		length = num;
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ~CWideString() (destructor)
//
// Description:	Destructor. Removes all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::~CWideString()
{
	// Delete all pointers
	//removeAll();
	delete [] ma_text;
	ma_text = NULL;

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::m_clear()
//
// Description:	Clears all member data. Doesn't remove data. NOTE: Pointers must be
//				deleted if they point to created memory before this function is called.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::m_clear()
{
	ma_text = NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::m_createMem()
//
// Description:	Destroys the old data and creates a new string of the requested length.
//				Note: You should always include the NULL character in the length.
// Parameters:	int length: The length of the new data to be created.
// Returns:		False if the memory couldn't be allocated.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::m_createMem(int length)
{
	// remove old data
	delete [] ma_text;

	// create new data
	ma_text = new wchar_t[length];

	// error-check
	if (!ma_text)
	{
		// TODO: HANDLE OUT OF MEMORY CONDITIONS!
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::removeAll()
//
// Description:	Deletes all pointers and sets all member variables to zero.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::removeAll()
{
	delete [] ma_text;
	ma_text = NULL;
}		

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getString()
//
// Description:	Returns a pointer to the class' string data.
// Parameters:	None.
// Returns:		Returns a pointer to the class' string data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t *CWideString::getString()
{
	return ma_text;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getText()
//
// Description:	Returns a pointer to the class' string data.
// Parameters:	None.
// Returns:		Returns a pointer to the class' string data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t *CWideString::getText()
{
	return ma_text;
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getStringLength() const
//
// Description:	Used to get the length of the string.
// Parameters:	None.
// Returns:		The length of the string, not counting the ending eol marker.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CWideString::getStringLength() const
{
	if (ma_text)
	{
		return wcslen(ma_text);
	}

	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::isValid() const
//
// Description:	Used to check if the string us usable.
// Parameters:	None.
// Returns:		True if the string has any content, false if it's NULL or has an empty
//				string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::isValid() const
{
	if (!ma_text)
	{
		return false;
	}

	return (ma_text[0] != 0);
}


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::pointToLastCharacter(wchar_t *, wchar_t)
//
// Description:	Returns a pointer to the last character of the specific type in a string.
// Parameters:	wchar_t *text:		The string to be checked.
//				wchar_t search:	The character to be looked for.
// Returns:		A pointer to the location of the last character of the type 'search'. If
//				no match exists, it returns a pointer to the start of the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t *CWideString::pointToLastCharacter(wchar_t *text, wchar_t search)
{
	wchar_t *last = text;

	// make sure 'text' points to anything
	if (!text)
	{
		return NULL;
	}

	// loop until we can't find any more characters of the type 'search'
	while (true)
	{
		// get a pointer to the next character of the type
		text = pointToNextCharacter(last, search);

		// if last and text match, we've found the last character of the type
		// this also occurs if no characters of the type exists at all
		if (last == text)
		{
			return last;
		}

		// we've found a new character of the type
		last = text;
	}
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::pointToNextCharacter(wchar_t *, wchar_t)
//
// Description:	Returns a pointer to the next character of the specific type in a string.
//				Ignores the first character in the string.
// Parameters:	wchar_t *text:		The string to be checked.
//				wchar_t search:	The character to be looked for.
// Returns:		A pointer to the location of the next character of the type 'search'. If
//				no match exists, it returns a pointer to the start of the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t *CWideString::pointToNextCharacter(wchar_t* text, wchar_t search)
{
	// save the pointer to the start of the string
	wchar_t *start = text;

	// make sure text points to anything at all
	if (!text)
	{
		return NULL;
	}

	
	// loop until we reach eol or the first character of the type is reached
	while (*text)
	{
		// go to the next character
		// this will skip the first character in the string since we wanted to find the NEXT
		// character, not the current one
		text++;

		if (*text == search)
		{
			// we've found another character of the type, so return the pointer to that
			// location
			return text;
		}
	}

	// we didn't find a character of the type, so return the pointer to the start instead
	return start;
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getPositionFromEnd(wchar_t *, int)
//
// Description:	Used to find a position backwards from the end of the text. Useful for
//				renaming filename extensions, for example.
// Parameters:	wchar_t *text:	The string that we want to find the end of.
//				int pos:	The number of characters to backtrack from eol.
// Returns:		A pointer to the position of 'pos' number of characters backwards in the
//				string. If 'pos' is zero, it returns a pointer to the eol character (same
//				result as if GetEnd() had been called).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t *CWideString::getPositionFromEnd(wchar_t *text, int pos)
{
	// make sure we have a string to work with
	if (!text)
	{
		return NULL;
	}

	// get the length of the text
	int length = wcslen(text);

	// if the length is less than the wanted position, we return the first character in the string
	if (pos >= length)
	{
		return text;
	}

	// return a pointer to the wanted location
	return &text[length - pos];
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getEnd(wchar_t *)
//
// Description:	Used to find the position of the eol character.
// Parameters:	wchar_t *text:	The string that we want to find the end of.
// Returns:		A pointer to the position of the eol characters.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t *CWideString::getEnd(wchar_t *text)
{
	// make sure we have a string to work with
	if (!text)
	{
		return NULL;
	}

	// get the length of the text
	int length = wcslen(text);

	// return a pointer to the wanted location
	return &text[length];
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::convertAllType1ToType2(wchar_t *, wchar_t, wchar_t)
//
// Description:	Used to replace all characters of a specific type with another character.
// Parameters:	wchar_t *text:	The string that we want to change.
//				wchar_t type1:	The characters we want to find.
//				wchar_t type2: The character we want to use instead.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::convertAllType1ToType2(wchar_t *text, wchar_t type1, wchar_t type2)
{
	// loop until eol is found
	while (*text)
	{
		// if the current character equals the type we want to find...
		if (*text == type1)
		{
			// ... replace it with the new type
			*text = type2;
		}

		text++;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::extractFileName()
//
// Description:	Used to get the filename out of a path.
// Parameters:	None.
// Returns:		Returns a CWideString holding the filename (WITHOUT its extension).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::extractFileName()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return CWideString(L"(null)");
	}

	// Find the last slash (the last directory)
	wchar_t *file = pointToLastCharacter(ma_text, L'/');

	// if we didn't find a slash, we assume the filename is located at the start of the
	// string, otherwise skip the first slash
	if ((file != ma_text) || (file[0] == L'/'))
	{
		file++;
	}

	// create a string to hold the filename
	wchar_t filename[MAX_FILENAME_LENGTH];
	
	// copy the filename
	wcscpy(filename, file);

	// "remove" the extension by replacing the dot with NULL
	convertAllType1ToType2(filename, L'.', 0);
	
	return CWideString(filename);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::extractExtension()
//
// Description:	Used to get the extension of a file out of a path.
// Parameters:	None.
// Returns:		Returns a CWideString holding the extension, without the dot ('.').
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::extractExtension()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return CWideString();
	}

	// find the last dot (most likely the start of the extension)
	wchar_t *ext = pointToLastCharacter(ma_text, L'.');

	// if we didn't find a dot, the pointer will point at the start of the string, so check
	// that it doesn't point to a dot itself (perhaps the string ONLY included an extension)
	if ((ext == ma_text) && (ext[0] != L'.'))
	{
		// this string didn't include a legal extension, so return an empty string
		return CWideString();
	}

	// skip the dot
	ext++;

	// create a string to hold the extension
	wchar_t extension[MAX_EXTENSION_LENGTH];

	// copy the extension
	wcscpy (extension, ext);

	return CWideString(extension);
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::extractPath()
//
// Description:	Used to get the path of a file.
// Parameters:	None.
// Returns:		Returns a CWideString holding the path, without the last slash.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::extractPath() 
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return CWideString();
	}

	// create an copy of the string
	wchar_t path[MAX_PATH_LENGTH];
	wcscpy(path, ma_text);

	// find the last slash in the path
	wchar_t *file = pointToLastCharacter (path, L'/');

	// if the last slash was at the start of the path, or no slash existed, we return an empty string
	if (file == path)
	{
		return CWideString();
	}

	// replace the last occurence of a slash with zero, effectivily ending the string there
	*file = 0;
	
	return CWideString(path);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::eliminateExtension()
//
// Description:	Used to remove the extension from a filepath.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::eliminateExtension()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return;
	}

	// find the last dot that marks the place were we want to cut
	wchar_t *extension = pointToLastCharacter(ma_text, L'.');

	// if we didn't find the extension, the pointer will point at the start of the string,
	// but also check that the first character wasn't a dot itself
	if (extension == ma_text)
	{
		if (extension[0] == L'.')
		{
			// the first character was a dot, so reset the data
			if (!m_createMem(1))
			{
				return;
			}

			ma_text[0] = 0;
		}
		return;
	}

	// create a temporary area to store the path
	int startlength = wcslen(ma_text);
	int endlength = startlength - wcslen(extension);

	wchar_t *filename = new wchar_t[endlength + 1];

	if (filename)
	{
		// copy old string, without the extension
		memcpy(filename, ma_text, endlength * 2);
		filename[endlength] = 0;

		// delete the old data
		delete [] ma_text;

		// save new data
		ma_text = filename;
	}
	else
	{
		// TODO: handle out of memory conditions
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::eliminateFilename()
//
// Description:	Used to remove the filename from a filepath. Removes the last slash as
//				well.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::eliminateFilename() // replaces last '/' with 0
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return;
	}

	// find the last dot that marks the place were we want to cut
	wchar_t *filename = pointToLastCharacter(ma_text, L'/');

	// if we didn't find the slash, the pointer will point at the start of the string,
	// but also check that the first character wasn't a slash itself
	if (filename == ma_text)
	{
		if (filename[0] == L'/')
		{
			// the first character was a slash, so reset the data
			if (!m_createMem(1))
			{
				return;
			}
			ma_text[0] = 0;
		}
		return;
	}

	// create a temporary area to store the path
	int startlength = wcslen(ma_text);
	int endlength = startlength - wcslen(filename);

	wchar_t *path = new wchar_t[endlength + 1];

	if (path)
	{
		// copy old string, without the filename
		memcpy(path, ma_text, endlength * 2);
		path[endlength] = 0;

		// delete the old data
		delete [] ma_text;

		// save new data
		ma_text = path;
	}
	else
	{
		// TODO: handle out of memory conditions
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::hasExtension()
//
// Description:	Used check if the string includes an extension.
// Parameters:	None.
// Returns:		True if a dot is found, false otherwise.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::hasExtension()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return false;
	}

	wchar_t *extension = pointToLastCharacter(ma_text, L'.');
	if ((extension == ma_text) && (extension[0] != L'.'))
	{
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::toLowerCase()
//
// Description:	Converts the string to lowercase.
// Parameters:	None.
// Returns:		A reference to itsef.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString& CWideString::toLowerCase()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return *this;
	}

	// convert the string to lowercase
	_wcslwr(ma_text);

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::toUpperCase()
//
// Description:	Converts the string to uppercase.
// Parameters:	None.
// Returns:		A reference to itsef.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString& CWideString::toUpperCase()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return *this;
	}

	// convert the string to uppercase
	_wcsupr(ma_text);

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const wchar_t)
//
// Description:	Assigns the string to equal the character.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const wchar_t rhs)
{
	// the string will always be of length 2 (the char + null)
	if (!m_createMem(2))
	{
		return *this;
	}

	// create the string
	ma_text[0] = rhs;
	ma_text[1] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const int)
//
// Description:	Assigns an integer value to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const int rhs)
{
	// the longest string that the integer will need is 12 since the longest value for an int is -2147483648
	wchar_t text[12];

	// convert the value to a string
	_itow(rhs, text, 10);

	// get the length of the string
	int length = wcslen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const unsigned long)
//
// Description:	Assigns an integer value to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const unsigned long rhs)
{
	// the longest string that the unsigned long will need is 21 since the maximum value for an unsigned long is 18446744073709551616
	wchar_t text[21];

	// convert the value to a string
	_ultow(rhs, text, 10);

	// get the length of the string
	int length = wcslen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const wchar_t *)
//
// Description:	Assigns a string to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const wchar_t *rhs)
{
	// get length of input string
	int length = wcslen(rhs);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, rhs, length * 2);
	ma_text[length] = 0;

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const float)
//
// Description:	Assigns a float to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const float rhs)
{
	// create a string big enough to hold the float
	wchar_t text[25];

	// convert the float into a string
	//gcvt((double)rhs, 15, text);
	swprintf(text, L"%f", (double)rhs);

	// get the length of the converted string
	int length = wcslen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == L'.')
	{
		// add a '0' to the string, to make it look better
		wcscat(text, L"0");
		// re-calculate the length of the string
		length = wcslen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const double)
//
// Description:	Assigns a double to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const double rhs)
{
	// create a string big enough to hold the float
	wchar_t text[25];

	// convert the float into a string
	//gcvt(rhs, 15, text);
	swprintf(text, L"%f", (double)rhs);

	// get the length of the converted string
	int length = wcslen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == L'.')
	{
		// add a '0' to the string, to make it look better
		wcscat(text, L"0");
		// re-calculate the length of the string
		length = wcslen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length * 2);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const CWideString *)
//
// Description:	Makes this class' text data equal to another's.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const CWideString *rhs)
{
	// check assignment to self!
	if (rhs == this)
	{
		return *this;
	}

	// get length of the string
	int length = wcslen(rhs->ma_text);

	// create new string
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, rhs->ma_text, length * 2);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator=(const CWideString &)
//
// Description:	Makes this class' text data equal to another's.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator = (const CWideString &rhs)
{
	// check assignment to self!
	if (&rhs == this)
	{
		return *this;
	}

	// get length of the string
	int length = wcslen(rhs.ma_text);

	// create new string
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, rhs.ma_text, length * 2);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator wchar_t *()
//
// Description:	Used to assign character pointers to ma_text.
// Parameters:	None.
// Returns:		A pointer to ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::operator wchar_t*()
{
	return ma_text;
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator int()
//
// Description:	Used to assign integers to the value of ma_text.
// Parameters:	None.
// Returns:		The integer value of ma_text. If ma_text isn't convertable to an integer,
//				it will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::operator int()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	// get a pointer to the end of the string
	wchar_t *end = &ma_text[wcslen(ma_text)];

	// convert hexadecimal string to long
	int value = (int)wcstol(ma_text, &end, 10);

	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator float()
//
// Description:	Used to assign floats to the value of ma_text.
// Parameters:	None.
// Returns:		The float value of ma_text. If ma_text isn't convertable to a float, it
//				will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::operator float()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	// get a pointer to the end of the string
	wchar_t *end = &ma_text[wcslen(ma_text)];

	// convert hexadecimal string to long
	float value = (float)wcstod(ma_text, &end);

	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator double()
//
// Description:	Used to assign a double to the value of ma_text.
// Parameters:	None.
// Returns:		The double value of ma_text. If ma_text isn't convertable to a double, it
//				will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::operator double()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	// get a pointer to the end of the string
	wchar_t *end = &ma_text[wcslen(ma_text)];

	// convert hexadecimal string to long
	double value = wcstod(ma_text, &end);

	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator unsigned long()
//
// Description:	Used to assign an unsigned long to the value of ma_text.
// Parameters:	None.
// Returns:		The unsigned long value of ma_text. If ma_text isn't convertable to an
//				unsigned long it will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::operator unsigned long()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	// get a pointer to the place where strtoul() should stop scanning
	wchar_t *end = &ma_text[wcslen(ma_text)];

	unsigned long value = wcstol(ma_text, &end, 10);
	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator CCharString()
//
// Description:	Used to convert the unicode string to an ansi counterpart.
// Parameters:	None.
// Returns:		A CCharString.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString::operator CCharString()
{
	int length = getStringLength();

	if (length <= 0)
	{
		return CCharString();
	}

	char *buffer = new char[length * 2];

	sprintf(buffer, "%S", ma_text);
	CCharString temporary(buffer);

	delete buffer;

	return CCharString(temporary);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator ==(const wchar_t *)
//
// Description:	Used check if ma_text equals another string, ignoring case.
// Parameters:	None.
// Returns:		True if the string is equal to the class data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator ==(const wchar_t *rhs)//comparison
{
	// check if we have any data
	if (!ma_text)
	{
		// since rhs actually can be NULL too, we need to check for comparison
		if (!rhs)
		{
			return true;
		}
		return false;
	}

	// check if the value we're comparing with is NULL
	if (!rhs)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs) == 0);
}
			//*********************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator !=(const wchar_t *)
//
// Description:	Used check if ma_text is not equal another string, ignoring case.
// Parameters:	None.
// Returns:		True if the string is not equal to the class data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator !=(const wchar_t* rhs)//comparison
{
	// check if we have any data
	if (!ma_text)
	{
		// since rhs actually can be NULL too, we need to check for comparison
		if (!rhs)
		{
			return false;
		}
		return true;
	}

	// check if the value we're comparing with is NULL
	if (!rhs)
	{
		return true;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator <(const wchar_t *)
//
// Description:	Used check if ma_text is "less than" another string, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "less than" the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator <(const wchar_t* rhs)
{
	// check if we have any data
	if (!ma_text)
	{
		return false;
	}

	// check if the value we're comparing with is NULL
	if (!rhs)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs) < 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator >(const wchar_t *)
//
// Description:	Used check if ma_text is "more than" another string, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "more than" the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator >(const wchar_t* rhs)
{
	// check if we have any data
	if (!ma_text)
	{
		return false;
	}

	// check if the value we're comparing with is NULL
	if (!rhs)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs) > 0);
}


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator ==(const CWideString *)
//
// Description:	Used check if ma_text is equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings are equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator ==(const CWideString *rhs)
{
	// check if we're comparing with ourselves
	if (this == rhs)
	{
		return true;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs->ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs->ma_text) == 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator !=(const CWideString *)
//
// Description:	Used check if ma_text is not equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings aren't equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator !=(const CWideString *rhs)
{
	// check if we're comparing with ourselves
	if (this == rhs)
	{
		return false;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs->ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs->ma_text) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator <(const CWideString *)
//
// Description:	Used check if ma_text is "less than" another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "less than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator <(const CWideString *rhs)
{
	// check if we're comparing with ourselves
	if (this == rhs)
	{
		return false;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs->ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs->ma_text) < 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator >(const CWideString *)
//
// Description:	Used check if ma_text is "more than" another another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "more than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator >(const CWideString *rhs)
{
	// check if we're comparing with ourselves
	if (this == rhs)
	{
		return false;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs->ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs->ma_text) > 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator ==(const CWideString &)
//
// Description:	Used check if ma_text is equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings are equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator ==(const CWideString &rhs)
{
	// check if we're comparing with ourselves
	if (this == &rhs)
	{
		return true;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs.ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs.ma_text) == 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator !=(const CWideString &)
//
// Description:	Used check if ma_text is not equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings aren't equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator !=(const CWideString &rhs)
{
	// check if we're comparing with ourselves
	if (this == &rhs)
	{
		return false;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs.ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs.ma_text) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator <(const CWideString &)
//
// Description:	Used check if ma_text is "less than" another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "less than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator <(const CWideString &rhs)
{
	// check if we're comparing with ourselves
	if (this == &rhs)
	{
		return false;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs.ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs.ma_text) < 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator >(const CWideString &)
//
// Description:	Used check if ma_text is "more than" another another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "more than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::operator >(const CWideString &rhs)
{
	// check if we're comparing with ourselves
	if (this == &rhs)
	{
		return false;
	}

	// check if we have any data to work with
	if (!ma_text || !rhs.ma_text)
	{
		return false;
	}

	// compare the strings
	return (_wcsicmp(ma_text, rhs.ma_text) > 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::HexToDec()
//
// Description:	Converts the string from hexadecimal form to decimal form.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::hexToDec()
{
	// check if we have any data to work with
	if (!ma_text)
	{
		return *this;
	}

	// get a pointer to the end of the string
	wchar_t *end = &ma_text[wcslen(ma_text)];

	// convert hexadecimal string to long
	long value = wcstol(ma_text, &end, 16);

	// if the result is zero, things get a lot easier
	if (!value)
	{
		// create new string
		if (!m_createMem(2))
		{
			return *this;
		}

		ma_text[0] = L'0';
		ma_text[1] = 0;

		return *this;
	}

	// create a string big enough to hold the value
	wchar_t text[21];
	// insert value into string
	_ltow(value, text, 10);

	this->operator =(text);	
	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::DecToHex()
//
// Description:	Converts the string from decimal to hexadecimal form.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString& CWideString::decToHex ()
{
	// check if we have any data to work with
	if (!ma_text)
	{
		return *this;
	}

	// get the value of the string
	//long value = atol(ma_text);
	
	// get a pointer to the place where strtoul() should stop scanning
	wchar_t *end = &ma_text[wcslen(ma_text)];

	unsigned long value = wcstol(ma_text, &end, 10);

	// create a string big enough to hold the value
	wchar_t text[30];
	// insert value into string
	_ltow(value, text, 16);

	this->operator =(text);
	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const wchar_t)
//
// Description:	Adds an character to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator += (wchar_t rhs)
{
	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}

	// create a new string, big enough to hold both ma_text and text
	int oldlength = wcslen(ma_text);
	wchar_t *result = new wchar_t[oldlength + 2];  // For a terminating zero

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength * 2);
		// copy new data
		result[oldlength] = rhs;
		// set eol
		result[oldlength + 1] = 0;

		// remove old data
		delete [] ma_text;
		ma_text = result;
	}
	else
	{
		// TODO: handle out of memory conditions
		// just leave ma_text as it is for now
	}

	return *this;
}


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const int)
//
// Description:	Adds an integer to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const int rhs)
{
	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// the longest string that the integer will need is 12 since the longest value for an int is -2147483648
	wchar_t text[12];
	// convert the value to a string
	_itow(rhs, text, 10);
	// get the length of the string
	int length = wcslen(text);

	// create a new string, big enough to hold both ma_text and text
	int oldlength = wcslen(ma_text);
	wchar_t *result = new wchar_t[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength * 2);
		// copy new data
		memcpy(result + oldlength, text, length * 2);
		// set eol
		result[oldlength + length] = 0;

		// remove old data
		delete [] ma_text;
		ma_text = result;
	}
	else
	{
		// TODO: handle out of memory conditions
		// just leave ma_text as it is for now
	}

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const unsigned long)
//
// Description:	Adds an unsigned long to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const unsigned long rhs)
{
	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// the longest string that the unsigned long will need is 21 since the maximum value for an unsigned long is 18446744073709551616
	wchar_t text[21];
	// convert the value to a string
	_ultow(rhs, text, 10);
	// get the length of the string
	int length = wcslen(text);

	// create a new string, big enough to hold both ma_text and text
	int oldlength = wcslen(ma_text);
	wchar_t *result = new wchar_t[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength * 2);
		// copy new data
		memcpy(result + oldlength, text, length * 2);
		// set eol
		result[oldlength + length] = 0;

		// remove old data
		delete [] ma_text;
		ma_text = result;
	}
	else
	{
		// TODO: handle out of memory conditions
		// just leave ma_text as it is for now
	}

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const wchar_t *)
//
// Description:	Adds another string to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const wchar_t *rhs)
{
	// check that we received a legal pointer
	if (!rhs)
	{
		return *this;
	}

	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// get length of input string
	int length = wcslen(rhs);

	// create a new string, big enough to hold both ma_text and text
	int oldlength = wcslen(ma_text);
	wchar_t *result = new wchar_t[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength * 2);
		// copy new data
		memcpy(result + oldlength, rhs, length * 2);
		// set eol
		result[oldlength + length] = 0;

		// remove old data
		delete [] ma_text;
		ma_text = result;
	}
	else
	{
		// TODO: handle out of memory conditions
		// just leave ma_text as it is for now
	}

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const float)
//
// Description:	Adds a float to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const float rhs)
{
	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// create a string big enough to hold the float
	wchar_t text[25];
	// convert the float into a string
	//gcvt((double)rhs, 15, text);
	swprintf(text, L"%f", (double)rhs);
	// get the length of the string
	int length = wcslen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == L'.')
	{
		// add a '0' to the string, to make it look better
		wcscat(text, L"0");
		// recalculate the length of the string
		length = wcslen(text);
	}

	// create a new string, big enough to hold both ma_text and text
	int oldlength = wcslen(ma_text);
	wchar_t *result = new wchar_t[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength * 2);
		// copy new data
		memcpy(result + oldlength, text, length * 2);
		// set eol
		result[oldlength + length] = 0;

		// remove old data
		delete [] ma_text;
		ma_text = result;
	}
	else
	{
		// TODO: handle out of memory conditions
		// just leave ma_text as it is for now
	}

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const double)
//
// Description:	Adds a float to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const double rhs)
{
	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// create a string big enough to hold the double
	wchar_t text[25];
	// convert the double into a string
	//gcvt(rhs, 15, text);
	swprintf(text, L"%f", rhs);
	// get the length of the string
	int length = wcslen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == L'.')
	{
		// add a '0' to the string, to make it look better
		wcscat(text, L"0");
		// recalculate the length of the string
		length = wcslen(text);
	}

	// create a new string, big enough to hold both ma_text and text
	int oldlength = wcslen(ma_text);
	wchar_t *result = new wchar_t[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength * 2);
		// copy new data
		memcpy(result + oldlength, text, length * 2);
		// set eol
		result[oldlength + length] = 0;

		// remove old data
		delete [] ma_text;
		ma_text = result;
	}
	else
	{
		// TODO: handle out of memory conditions
		// just leave ma_text as it is for now
	}

	return *this;
}
            
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const CWideString *)
//
// Description:	Adds a string to the current string from another CWideString.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const CWideString *rhs)
{
	// check that we received a legal pointer
	if (!rhs)
	{
		return *this;
	}

	// check if we have any legal data
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// get string length
	int length = wcslen(ma_text);
	wchar_t *text;

	// check if we're adding to ourselves from ourselves
	if (this == rhs)
	{
		// create a new string, twice as big
		text = new wchar_t[(length * 2) + 1];

		// check that we received a legal pointer
		if (text)
		{
			// copy string twice
			memcpy(text, ma_text, length * 2);
			memcpy(text + length, ma_text, length * 2);
			text[length * 2] = 0;

			// replace old data
			delete [] ma_text;
			ma_text = text;
		}
		else
		{
			// TODO: handle out of memory conditions
		}

		return *this;
	}

	// check the other's data
	if (!rhs->ma_text)
	{
		return *this;
	}

	// create a new string, big enough to hold both strings
	int length2 = wcslen(rhs->ma_text);
	text = new wchar_t[length + length2 + 1];

	// check that we received a legal pointer
	if (text)
	{
		// copy the strings
		memcpy(text, ma_text, length * 2);
		memcpy(text + length, rhs->ma_text, length2 * 2);
		text[length + length2] = 0;

		// replace old data
		delete [] ma_text;
		ma_text = text;
	}
	else
	{
		// TODO: handle out of memory conditions
	}

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator +=(const CWideString &)
//
// Description:	Adds a string to the current string from another CWideString.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator +=(const CWideString &rhs)
{
	// check if we have any legal data
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}
	if (!ma_text[0])
	{
		this->operator =(rhs);
		return *this;
	}

	// get string length
	int length = wcslen(ma_text);
	wchar_t *text;

	// check if we're adding to ourselves from ourselves
	if (this == &rhs)
	{
		// create a new string, twice as big
		text = new wchar_t[(length * 2) + 1];

		// check that we received a legal pointer
		if (text)
		{
			// copy string twice
			memcpy(text, ma_text, length * 2);
			memcpy(text + length, ma_text, length * 2);
			text[length * 2] = 0;

			// replace old data
			delete [] ma_text;
			ma_text = text;
		}
		else
		{
			// TODO: handle out of memory conditions
		}

		return *this;
	}

	// check the other's data
	if (!rhs.ma_text)
	{
		return *this;
	}

	// create a new string, big enough to hold both strings
	int length2 = wcslen(rhs.ma_text);
	text = new wchar_t[length + length2 + 1];

	// check that we received a legal pointer
	if (text)
	{
		// copy the strings
		memcpy(text, ma_text, length * 2);
		memcpy(text + length, rhs.ma_text, length2 * 2);
		text[length + length2] = 0;

		// replace old data
		delete [] ma_text;
		ma_text = text;
	}
	else
	{
		// TODO: handle out of memory conditions
	}

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const wchar_t)
//
// Description:	Creates a reference to an object containing this string plus a character.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus a character.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (wchar_t rhs) const
{
	return (CWideString (*this)) += rhs;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const int)
//
// Description:	Creates a reference to an object containing this string plus an integer.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus an integer.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const int rhs) const
{
	return (CWideString (*this)) += rhs;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const unsigned long)
//
// Description:	Creates a reference to an object containing this string plus an unsigned
//				long.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus an unsigned long.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const unsigned long rhs) const
{
	return (CWideString (*this)) += rhs;
}	

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const wchar_t *)
//
// Description:	Creates a reference to an object containing this string plus another
//				string.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus another string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const wchar_t *rhs) const
{
	return (CWideString (*this)) += rhs;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const float)
//
// Description:	Creates a reference to an object containing this string plus a float.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus a float.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const float rhs) const
{
	return (CWideString (*this)+= rhs);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const float)
//
// Description:	Creates a reference to an object containing this string plus a float.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus a float.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const double rhs) const
{
	return (CWideString (*this)+= rhs);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const CWideString *)
//
// Description:	Creates a reference to an object containing this string plus the string
//				of the right-hand object.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus the string
//				of the right-hand object.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const CWideString *rhs) const
{
	return (CWideString (*this)+= rhs);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator + (const CWideString &)
//
// Description:	Creates a reference to an object containing this string plus the string
//				of the right-hand object.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus the string
//				of the right-hand object.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator + (const CWideString &rhs) const
{
	return (CWideString (*this)+= rhs);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator <<(const int)
//
// Description:	Shifts the string left and removes the first 'amount' of characters.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		A CWideString containing the shifted string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator << (const int amount)
{
	// check the data
	if (!ma_text)
	{
		// return an empty string
		return CWideString();
	}

	// get the length of the current string
	int length = wcslen(ma_text);

	// if the string is shorter than the amount of shifts, return an empty string
	if (length <= amount)
	{
		return CWideString();
	}

	// otherwise return a string with the correct pointer position
	return CWideString((wchar_t *)(ma_text + amount));
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator >>(const int)
//
// Description:	Shifts the string right and inserts spaces in the new bytes.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		A CWideString containing the shifted string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString CWideString::operator >> (const int amount)
{
	// create a new CWideString so we can be sure that its data is removed upon exit
	CWideString tempstring;
	int length;
	
	// check the current string
	if (!ma_text)
	{
		length = 0;
	}
	else
	{
		length = wcslen(ma_text);
	}

	// create a new string, big enough to hold both the spaces and the original string
	if (!tempstring.m_createMem(amount + length + 1))
	{
		return CWideString();
	}

	// insert spaces
	int i;
	for (i = 0; i < amount; i++)
	{
		tempstring.ma_text[i] = L' ';
	}

	// copy old string
	if (length > 0)
	{
		memcpy(tempstring.ma_text + amount, ma_text, length * 2);
	}

	// insert eol
	tempstring.ma_text[amount + length] = 0;

	return CWideString(tempstring.ma_text);

	// tempstring's character array will be deleted here
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator <<=(const int)
//
// Description:	Shifts the string left and removes the first 'amount' of characters.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator <<=(const int amount)
{
	// check the data
	if (!ma_text)
	{		
		return *this;
	}

	// get the length of the current string
	int length = wcslen(ma_text);

	// check that amount is less than length
	if (length <= amount)
	{
		// replace the text with an empty string
		if (!m_createMem(1))
		{
			return *this;
		}

		ma_text[0] = 0;

		return *this;
	}

	// create a new, shorter string
	wchar_t *text = new wchar_t[length - amount + 1];

	// check pointer
	if (text)
	{
		// copy the string from the correct location
		memcpy(text, ma_text + amount, (length - amount) * 2);
		text[length - amount] = 0;

		// replace texts
		delete [] ma_text;
		ma_text = text;
	}
	else
	{
		// TODO: handle out of memory conditions here
	}

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator >>=(const int)
//
// Description:	Shifts the string right and inserts spaces in the new bytes.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString &CWideString::operator >>=(const int amount)
{
	int length;
	
	// check the current string
	if (!ma_text)
	{
		length = 0;
	}
	else
	{
		length = wcslen(ma_text);
	}

	// create a new string, big enough to hold both the spaces and the original string
	wchar_t *text = new wchar_t[amount + length + 1];

	// check that the new memory was created
	if (text)
	{
		// insert spaces
		int i;
		for (i = 0; i < amount; i++)
		{
			text[i] = L' ';
		}

		// copy old string
		if (length > 0)
		{
			memcpy(text + amount, ma_text, length * 2);
		}

		// insert eol
		text[amount + length] = 0;

		// replace pointers
		delete [] ma_text;
		ma_text = text;
	}
	else
	{
		// TODO: handle out of memory conditions here
	}

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::operator [](const int)
//
// Description:	Returns the character at the assigned position in the array.
// Parameters:	const int pos:	The position of the character.
// Returns:		The character at the position.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
wchar_t CWideString::operator [](const int pos) const
{
	// check text
	if (!ma_text)
	{
		return 0;
	}

	// check length
	if (pos >= (int)wcslen(ma_text))
	{
		return 0;
	}

	return ma_text[pos];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::set(const wchar_t, const int)
//
// Description:	Sets the character at the specified position in the array.
// Parameters:	const wchar_t chr:	The new character.
//				const int pos:	The position in the array.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::set(const wchar_t chr, const int pos)
{
	// check the pointer
	if (!ma_text)
	{
		return;
	}

	// check the position
	if (pos >= 0 && pos <= (int)wcslen(ma_text))
	{
		ma_text[pos] = chr;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getLeft(const int)
//
// Description:	Creates a new string object with the 'num' characters from the left side
//				of the current string.
// Parameters:	int num:	The number of characters to keep from the original string.
// Returns:		A CWideString object containing the new string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString	CWideString::getLeft(const int num)
{
	// check pointer
	if (!ma_text)
	{
		return CWideString();
	}

	// get length of the string
	int length = wcslen(ma_text);

	// if the requested number of characters is equal to or more than the current string
	// length we return a copy of this object
	if (num >= length)
	{
		return CWideString(this);
	}

	return CWideString(ma_text, num);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getRight(const int)
//
// Description:	Creates a new string object with the 'num' characters from the right side
//				of the current string.
// Parameters:	int num:	The number of characters to keep from the original string.
// Returns:		A CWideString object containing the new string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString	CWideString::getRight(const int num)
{
	// check pointer
	if (!ma_text)
	{
		return CWideString();
	}

	// get length of the string
	int length = wcslen(ma_text);

	// if the requested number of characters is equal to or more than the current string
	// length we return a copy of this object
	if (num >= length)
	{
		return CWideString(this);
	}

	return CWideString(ma_text + (length - num), num);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::getMiddle(const int, const int)
//
// Description:	Creates a new string object starting with character 'left' bytes from the
//				left of the string up to 'right'.
// Parameters:	int left:	The number of characters to count up to before copying.
//				int right:	The number of characters to keep from the right side.
// Returns:		A CWideString object containing the new string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWideString	CWideString::getMiddle(const int left, const int right)
{
	// check pointer
	if (!ma_text)
	{
		return CWideString();
	}

	// get length of the string
	int length = wcslen(ma_text);

	// if left is more than the length of the string, we're keeping nothing
	if (left >= length)
	{
		return CWideString();
	}

	// same goes for right
	if (right >= length)
	{
		return CWideString();
	}

	// now check if the interval between left and right is 0 or less
	if ((length - right) <= left)
	{
		return CWideString();
	}

	// return a new string
	return CWideString(ma_text + left, length - right - left);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::replace(const wchar_t, const wchar_t)
//
// Description:	Replaces all occurences of 'item' with 'type' in the object's member string.
// Parameters:	const wchar_t item:	The character to look for.
//				const wchar_t type:	The value to replace all occurences with.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::replace(const wchar_t item, const wchar_t type)
{
	convertAllType1ToType2(ma_text, item, type);
}


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::contains(wchar_t c)
//
// Description:	Checks if this string contains the passed character
// Parameters:	wchar_t c:	The character to look for.
// Returns:		true if found else false.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWideString::contains(wchar_t c)
{
	int len = getStringLength();
	for(int i = 0; i < len; i++)
	{
		if(ma_text[i] == c)
		{
			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CWideString::convertMBCS(char *string, int size)
//
// Description:	Converts a Multibyte Character string to an Unicode (Wide) string.
// Parameters:	char *string:	The character string.
//				int size:		Size of the string, used to create a temporary buffer
//								for the converted string.
// Returns:		void. Results are stored in the WideString class.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWideString::convertMBCS(char *string, int size)
{
	// error-check
	if (!string || size < 1)
	{
		return;
	}

	// create buffer, big enough to hold the largest possible string
	wchar_t *buffer = new wchar_t[size * 2];

	if (!buffer)
	{
		return;
	}

	// make the conversion
	MultiByteToWideChar(CP_ACP, 0, string, -1, buffer, size * 2);

	// insert into class
	this->operator=(buffer);

	// remove buffer
	delete buffer;
}

} // end of Basic namespace

