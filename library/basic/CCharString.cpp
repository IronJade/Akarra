/** 
 *  File description (may not be accurate):
 *    Signed char string class used throughout the project. This class was based on
 *    somebody else's class, but I don't remember who. In other words, the EULA doesn't
 *    count for this file.
 **/

#if defined (_MSC_VER)  
#define  strlwr  _strlwr
#define  strupr  _strupr
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CCharString.h"
#include "Basic.h"

namespace Basic
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString()
{
	m_clear();

	// create an empty string (so that this constructor behaves exactly like the other
	// constructor, just that the created string will be empty)
	if (!m_createMem(1))
	{
		return;
	}

	ma_text[0] = 0; // equals ""
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const char *) (constructor)
//
// Description:	Constructor with a string parameter.
// Parameters:	const char *text:	A pointer to the string that should be stored.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString:: CCharString (const char* text)
{
    // clear all data
	m_clear();

	// get length of input string
	int length = strlen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const char) (constructor)
//
// Description:	Constructor with a single character as a parameter.
// Parameters:	const char chr:	A character which should be stored in the string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const char chr)
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
// CCharString(const int) (constructor)
//
// Description:	Constructor with an integer parameter.
// Parameters:	const int value:	An integer which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const int value)
{
    // clear all data
	m_clear();

	// the longest string that the integer will need is 12 since the longest value for an int is -2147483648
	char text[12];

	// convert the value to a string
	itoa(value, text, 10);

	// get the length of the string
	int length = strlen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const int, char) (constructor)
//
// Description:	Constructor that takes an integer and inserts a character between every
//				third number, such as "12 000" or "1,435,532"
// Parameters:	const int value:			An integer which should be stored as a string.
//				const char kilodelimiter:	The character that should be placed between
//											the numbers.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const int value, char kilodelimiter)
{
    // clear all data
	m_clear();

	// the longest string that the integer will need is 12 since the longest value for an int is -2147483648
	char text[12];

	// convert the value to a string
	itoa(value, text, 10);

	// get the length of the string
	int length = strlen(text);

	// now copy the string to a new buffer, placing the delimiter between every 1000
	char ntext[20];
	int pos(0), npos(0);
	if (text[pos] == '-')
	{
		ntext[npos] = '-';
		npos++;
		pos++;
	}
	do
	{
		// copy a number
		ntext[npos++] = text[pos++];
		// check how many numbers there are until we reach the end
		if ((pos < length) && (((length - pos) % 3) == 0))
		{
			ntext[npos++] = kilodelimiter;
		}
		
	} while(pos < length);
	ntext[npos] = 0;
	
	// get new length
	length = strlen(ntext);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, ntext, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const unsigned long value, char kilodelimiter)
{
    // clear all data
	m_clear();

	// the longest string that the unsigned long will need is 21 since the maximum value for an unsigned long is 18446744073709551616
	char text[21];

	// convert the value to a string
	ultoa(value, text, 10);

	// get the length of the string
	int length = strlen(text);

	// now copy the string to a new buffer, placing the delimiter between every 1000
	char ntext[30];
	int pos(0), npos(0);
	if (text[pos] == '-')
	{
		ntext[npos] = '-';
		npos++;
		pos++;
	}
	do
	{
		// copy a number
		ntext[npos++] = text[pos++];
		// check how many numbers there are until we reach the end
		if ((pos < length) && (((length - pos) % 3) == 0))
		{
			ntext[npos++] = kilodelimiter;
		}
		
	} while(pos < length);
	ntext[npos] = 0;
	
	// get new length
	length = strlen(ntext);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, ntext, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const unsigned long) (constructor)
//
// Description:	Constructor with an integer parameter.
// Parameters:	const unsigned long value:	An integer which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const unsigned long value)
{
    // clear all data
	m_clear();

	// the longest string that the unsigned long will need is 21 since the maximum value for an unsigned long is 18446744073709551616
	char text[21];

	// convert the value to a string
	ultoa(value, text, 10);

	// get the length of the string
	int length = strlen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const float) (constructor)
//
// Description:	Constructor with a float parameter.
// Parameters:	const float value:	A float which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const float value)
{
    // clear all data
	m_clear();

	// create a string big enough to hold the float
	char text[25];

	// convert the float into a string
	gcvt((double)value, 15, text);

	// get the length of the converted string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// re-calculate the length of the string
		length = strlen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

CCharString::CCharString(const float value, const int maxdigits)
{
    // clear all data
	m_clear();

	// create a string big enough to hold the float
	char text[20];
	char conversion[20];

	// convert the float into a string, using the ugly method
	// but first we need to create the conversion string
	sprintf(conversion, "%%.%df", maxdigits);
	sprintf(text, (const char*)conversion, value);

	// get the length of the converted string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// re-calculate the length of the string
		length = strlen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const double) (constructor)
//
// Description:	Constructor with a double parameter.
// Parameters:	const double value:	A double which should be stored as a string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const double value)
{
    // clear all data
	m_clear();

	// create a string big enough to hold the double
	char text[25];

	// convert the double into a string
	gcvt(value, 15, text);

	// get the length of the converted string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// re-calculate the length of the string
		length = strlen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const CCharString *) (constructor)
//
// Description:	Constructor that creates a copy of another CCharString's text.
// Parameters:	const CCharString *other:	Pointer to a CCharString object from which the
//											string data will be taken.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const CCharString *other)
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
	memcpy(ma_text, other->ma_text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const CCharString &) (constructor)
//
// Description:	Constructor that creates a copy of another CCharString's text.
// Parameters:	const CCharString &other:	Reference to a CCharString object from which
//											the string data will be taken.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const CCharString &other)
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
	memcpy(ma_text, other.ma_text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const CCharString *, const int) (constructor)
//
// Description:	Constructor that creates a copy of another CCharString's text, with a
//				limited number of characters.
// Parameters:	const CCharString *other:	Pointer to a CCharString object from which
//											the string data will be taken.
//				const int num:				The maximum number of characters to be copied.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const CCharString *other, const int num)
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
	memcpy(ma_text, other->ma_text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const CCharString &, const int) (constructor)
//
// Description:	Constructor that creates a copy of another CCharString's text, with a
//				limited number of characters.
// Parameters:	const CCharString &other:	Reference to a CCharString object from which
//											the string data will be taken.
//				const int num:				The maximum number of characters to be copied.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const CCharString &other, const int num)
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
	memcpy(ma_text, other.ma_text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString(const char *, const int) (constructor)
//
// Description:	Constructor which copies a string with a limited length.
// Parameters:	const char *text:	The string to be copied.
//				const int num:		The maximum length of the string.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::CCharString(const char *text, const int num)
{
    // clear all data
	m_clear();

	// get length of input string
	int length = strlen(text);

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
	memcpy(ma_text, text, length);
	ma_text[length] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ~CCharString() (destructor)
//
// Description:	Destructor. Removes all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::~CCharString()
{
	// Delete all pointers
	//removeAll();
	delete [] ma_text;
	ma_text = NULL;

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::m_clear()
//
// Description:	Clears all member data. Doesn't remove data. NOTE: Pointers must be
//				deleted if they point to created memory before this function is called.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::m_clear()
{
	ma_text = NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::m_createMem()
//
// Description:	Destroys the old data and creates a new string of the requested length.
//				Note: You should always include the NULL character in the length.
// Parameters:	int length: The length of the new data to be created.
// Returns:		False if the memory couldn't be allocated.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::m_createMem(int length)
{
	// remove old data
	delete [] ma_text;

	// create new data
	ma_text = new char[length];

	// error-check
	if (!ma_text)
	{
		// TODO: HANDLE OUT OF MEMORY CONDITIONS!
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::removeAll()
//
// Description:	Deletes all pointers and sets all member variables to zero.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::removeAll()
{
	delete [] ma_text;
	ma_text = NULL;
}		

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getString()
//
// Description:	Returns a pointer to the class' string data.
// Parameters:	None.
// Returns:		Returns a pointer to the class' string data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char *CCharString::getString()
{
	return ma_text;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getText()
//
// Description:	Returns a pointer to the class' string data.
// Parameters:	None.
// Returns:		Returns a pointer to the class' string data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char *CCharString::getText()
{
	return ma_text;
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getStringLength() const
//
// Description:	Used to get the length of the string.
// Parameters:	None.
// Returns:		The length of the string, not counting the ending eol marker.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CCharString::getStringLength() const
{
	if (ma_text)
	{
		return strlen(ma_text);
	}

	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::isValid() const
//
// Description:	Used to check if the string us usable.
// Parameters:	None.
// Returns:		True if the string has any content, false if it's NULL or has an empty
//				string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::isValid() const
{
	if (!ma_text)
	{
		return false;
	}

	return (ma_text[0] != 0);
}


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::pointToLastCharacter(char *, char)
//
// Description:	Returns a pointer to the last character of the specific type in a string.
// Parameters:	char *text:		The string to be checked.
//				char search:	The character to be looked for.
// Returns:		A pointer to the location of the last character of the type 'search'. If
//				no match exists, it returns a pointer to the start of the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char *CCharString::pointToLastCharacter(char *text, char search)
{
	char *last = text;

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
// CCharString::pointToNextCharacter(char *, char)
//
// Description:	Returns a pointer to the next character of the specific type in a string.
//				Ignores the first character in the string.
// Parameters:	char *text:		The string to be checked.
//				char search:	The character to be looked for.
// Returns:		A pointer to the location of the next character of the type 'search'. If
//				no match exists, it returns a pointer to the start of the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char *CCharString::pointToNextCharacter(char* text, char search)
{
	// save the pointer to the start of the string
	char *start = text;

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
// CCharString::getPositionFromEnd(char *, int)
//
// Description:	Used to find a position backwards from the end of the text. Useful for
//				renaming filename extensions, for example.
// Parameters:	char *text:	The string that we want to find the end of.
//				int pos:	The number of characters to backtrack from eol.
// Returns:		A pointer to the position of 'pos' number of characters backwards in the
//				string. If 'pos' is zero, it returns a pointer to the eol character (same
//				result as if GetEnd() had been called).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char *CCharString::getPositionFromEnd(char *text, int pos)
{
	// make sure we have a string to work with
	if (!text)
	{
		return NULL;
	}

	// get the length of the text
	int length = strlen(text);

	// if the length is less than the wanted position, we return the first character in the string
	if (pos >= length)
	{
		return text;
	}

	// return a pointer to the wanted location
	return &text[length - pos];
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getEnd(char *)
//
// Description:	Used to find the position of the eol character.
// Parameters:	char *text:	The string that we want to find the end of.
// Returns:		A pointer to the position of the eol characters.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char *CCharString::getEnd(char *text)
{
	// make sure we have a string to work with
	if (!text)
	{
		return NULL;
	}

	// get the length of the text
	int length = strlen(text);

	// return a pointer to the wanted location
	return &text[length];
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::convertAllType1ToType2(char *, char, char)
//
// Description:	Used to replace all characters of a specific type with another character.
// Parameters:	char *text:	The string that we want to change.
//				char type1:	The characters we want to find.
//				char type2: The character we want to use instead.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::convertAllType1ToType2(char *text, char type1, char type2)
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
// CCharString::extractFileName()
//
// Description:	Used to get the filename out of a path.
// Parameters:	None.
// Returns:		Returns a CCharString holding the filename (WITHOUT its extension).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::extractFileName()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return CCharString("(null)");
	}

	// Find the last slash (the last directory)
	char *file = pointToLastCharacter(ma_text, '/');

	// if we didn't find a slash, we assume the filename is located at the start of the
	// string, otherwise skip the first slash
	if ((file != ma_text) || (file[0] == '/'))
	{
		file++;
	}

	// create a string to hold the filename
	char filename[MAX_FILENAME_LENGTH];
	
	// copy the filename
	strcpy (filename, file);

	// "remove" the extension by replacing the dot with NULL
	convertAllType1ToType2(filename, '.', 0);
	
	return CCharString(filename);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::extractExtension()
//
// Description:	Used to get the extension of a file out of a path.
// Parameters:	None.
// Returns:		Returns a CCharString holding the extension, without the dot ('.').
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::extractExtension()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return CCharString();
	}

	// find the last dot (most likely the start of the extension)
	char *ext = pointToLastCharacter(ma_text, '.');

	// if we didn't find a dot, the pointer will point at the start of the string, so check
	// that it doesn't point to a dot itself (perhaps the string ONLY included an extension)
	if ((ext == ma_text) && (ext[0] != '.'))
	{
		// this string didn't include a legal extension, so return an empty string
		return CCharString();
	}

	// skip the dot
	ext++;

	// create a string to hold the extension
	char extension[MAX_EXTENSION_LENGTH];

	// copy the extension
	strcpy (extension, ext);

	return CCharString(extension);
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::extractPath()
//
// Description:	Used to get the path of a file.
// Parameters:	None.
// Returns:		Returns a CCharString holding the path, without the last slash.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::extractPath() 
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return CCharString();
	}

	// create an copy of the string
	char path[MAX_PATH_LENGTH];
	strcpy(path, ma_text);

	// find the last slash in the path
	char *file = pointToLastCharacter (path, '/');

	// if the last slash was at the start of the path, or no slash existed, we return an empty string
	if (file == path)
	{
		return CCharString();
	}

	// replace the last occurence of a slash with zero, effectivily ending the string there
	*file = 0;
	
	return CCharString(path);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::eliminateExtension()
//
// Description:	Used to remove the extension from a filepath.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::eliminateExtension()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return;
	}

	// find the last dot that marks the place were we want to cut
	char *extension = pointToLastCharacter(ma_text, '.');

	// if we didn't find the extension, the pointer will point at the start of the string,
	// but also check that the first character wasn't a dot itself
	if (extension == ma_text)
	{
		if (extension[0] == '.')
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
	int startlength = strlen(ma_text);
	int endlength = startlength - strlen(extension);

	char *filename = new char[endlength + 1];

	if (filename)
	{
		// copy old string, without the extension
		memcpy(filename, ma_text, endlength);
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
// CCharString::eliminateFilename()
//
// Description:	Used to remove the filename from a filepath. Removes the last slash as
//				well.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::eliminateFilename() // replaces last '/' with 0
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return;
	}

	// find the last dot that marks the place were we want to cut
	char *filename = pointToLastCharacter(ma_text, '/');

	// if we didn't find the slash, the pointer will point at the start of the string,
	// but also check that the first character wasn't a slash itself
	if (filename == ma_text)
	{
		if (filename[0] == '/')
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
	int startlength = strlen(ma_text);
	int endlength = startlength - strlen(filename);

	char *path = new char[endlength + 1];

	if (path)
	{
		// copy old string, without the filename
		memcpy(path, ma_text, endlength);
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
// CCharString::hasExtension()
//
// Description:	Used check if the string includes an extension.
// Parameters:	None.
// Returns:		True if a dot is found, false otherwise.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::hasExtension()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return false;
	}

	char *extension = pointToLastCharacter(ma_text, '.');
	if ((extension == ma_text) && (extension[0] != '.'))
	{
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::toLowerCase()
//
// Description:	Converts the string to lowercase.
// Parameters:	None.
// Returns:		A reference to itsef.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString& CCharString::toLowerCase()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return *this;
	}

	// convert the string to lowercase
	strlwr(ma_text);

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::toUpperCase()
//
// Description:	Converts the string to uppercase.
// Parameters:	None.
// Returns:		A reference to itsef.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString& CCharString::toUpperCase()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return *this;
	}

	// convert the string to uppercase
	strupr(ma_text);

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const char)
//
// Description:	Assigns the string to equal the character.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const char rhs)
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
// CCharString::operator=(const int)
//
// Description:	Assigns an integer value to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const int rhs)
{
	// the longest string that the integer will need is 12 since the longest value for an int is -2147483648
	char text[12];

	// convert the value to a string
	itoa(rhs, text, 10);

	// get the length of the string
	int length = strlen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const unsigned long)
//
// Description:	Assigns an integer value to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const unsigned long rhs)
{
	// the longest string that the unsigned long will need is 21 since the maximum value for an unsigned long is 18446744073709551616
	char text[21];

	// convert the value to a string
	ultoa(rhs, text, 10);

	// get the length of the string
	int length = strlen(text);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const char *)
//
// Description:	Assigns a string to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const char *rhs)
{
	// get length of input string
	int length = strlen(rhs);

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, rhs, length);
	ma_text[length] = 0;

	return *this;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const float)
//
// Description:	Assigns a float to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const float rhs)
{
	// create a string big enough to hold the float
	char text[25];

	// convert the float into a string
	gcvt((double)rhs, 15, text);

	// get the length of the converted string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// re-calculate the length of the string
		length = strlen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const double)
//
// Description:	Assigns a double to the class.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const double rhs)
{
	// create a string big enough to hold the float
	char text[25];

	// convert the float into a string
	gcvt(rhs, 15, text);

	// get the length of the converted string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// re-calculate the length of the string
		length = strlen(text);
	}

	// create string (with an extra byte for the eol character)
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, text, length);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const CCharString *)
//
// Description:	Makes this class' text data equal to another's.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const CCharString *rhs)
{
	// check assignment to self!
	if (rhs == this)
	{
		return *this;
	}

	// get length of the string
	int length = strlen(rhs->ma_text);

	// create new string
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, rhs->ma_text, length);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator=(const CCharString &)
//
// Description:	Makes this class' text data equal to another's.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator = (const CCharString &rhs)
{
	// check assignment to self!
	if (&rhs == this)
	{
		return *this;
	}

	// get length of the string
	int length = strlen(rhs.ma_text);

	// create new string
	if (!m_createMem(length + 1))
	{
		return *this;
	}

	// copy the text and set the final eol byte
	memcpy(ma_text, rhs.ma_text, length);
	ma_text[length] = 0;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator char *()
//
// Description:	Used to assign character pointers to ma_text.
// Parameters:	None.
// Returns:		A pointer to ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::operator char*()
{
	return ma_text;
}
		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator int()
//
// Description:	Used to assign integers to the value of ma_text.
// Parameters:	None.
// Returns:		The integer value of ma_text. If ma_text isn't convertable to an integer,
//				it will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::operator int()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	int value = atoi(ma_text);
	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator float()
//
// Description:	Used to assign floats to the value of ma_text.
// Parameters:	None.
// Returns:		The float value of ma_text. If ma_text isn't convertable to a float, it
//				will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::operator float()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	float value = (float)atof(ma_text);
	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator double()
//
// Description:	Used to assign a double to the value of ma_text.
// Parameters:	None.
// Returns:		The double value of ma_text. If ma_text isn't convertable to a double, it
//				will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::operator double()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	double value = atof(ma_text);
	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator unsigned long()
//
// Description:	Used to assign an unsigned long to the value of ma_text.
// Parameters:	None.
// Returns:		The unsigned long value of ma_text. If ma_text isn't convertable to an
//				unsigned long it will return 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString::operator unsigned long()
{
	// make sure we have a string to work with
	if (!ma_text)
	{
		return 0;
	}

	// get a pointer to the place where strtoul() should stop scanning
	char *end = &ma_text[strlen(ma_text)];

	unsigned long value = strtoul(ma_text, &end, 10);
	return value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator ==(const char *)
//
// Description:	Used check if ma_text equals another string, ignoring case.
// Parameters:	None.
// Returns:		True if the string is equal to the class data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator ==(const char *rhs)//comparison
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
	return (stricmp(ma_text, rhs) == 0);
}
			//*********************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator !=(const char *)
//
// Description:	Used check if ma_text is not equal another string, ignoring case.
// Parameters:	None.
// Returns:		True if the string is not equal to the class data.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator !=(const char* rhs)//comparison
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
	return (stricmp(ma_text, rhs) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator <(const char *)
//
// Description:	Used check if ma_text is "less than" another string, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "less than" the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator <(const char* rhs)
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
	return (stricmp(ma_text, rhs) < 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator >(const char *)
//
// Description:	Used check if ma_text is "more than" another string, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "more than" the string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator >(const char* rhs)
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
	return (stricmp(ma_text, rhs) > 0);
}


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator ==(const CCharString *)
//
// Description:	Used check if ma_text is equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings are equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator ==(const CCharString *rhs)
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
	return (stricmp(ma_text, rhs->ma_text) == 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator !=(const CCharString *)
//
// Description:	Used check if ma_text is not equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings aren't equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator !=(const CCharString *rhs)
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
	return (stricmp(ma_text, rhs->ma_text) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator <(const CCharString *)
//
// Description:	Used check if ma_text is "less than" another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "less than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator <(const CCharString *rhs)
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
	return (stricmp(ma_text, rhs->ma_text) < 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator >(const CCharString *)
//
// Description:	Used check if ma_text is "more than" another another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "more than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator >(const CCharString *rhs)
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
	return (stricmp(ma_text, rhs->ma_text) > 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator ==(const CCharString &)
//
// Description:	Used check if ma_text is equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings are equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator ==(const CCharString &rhs)
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
	return (stricmp(ma_text, rhs.ma_text) == 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator !=(const CCharString &)
//
// Description:	Used check if ma_text is not equal to another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the strings aren't equal.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator !=(const CCharString &rhs)
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
	return (stricmp(ma_text, rhs.ma_text) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator <(const CCharString &)
//
// Description:	Used check if ma_text is "less than" another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "less than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator <(const CCharString &rhs)
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
	return (stricmp(ma_text, rhs.ma_text) < 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator >(const CCharString &)
//
// Description:	Used check if ma_text is "more than" another another's ma_text, ignoring case.
// Parameters:	None.
// Returns:		True if the class data is "more than" the another's ma_text.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::operator >(const CCharString &rhs)
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
	return (stricmp(ma_text, rhs.ma_text) > 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::HexToDec()
//
// Description:	Converts the string from hexadecimal form to decimal form.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::hexToDec()
{
	// check if we have any data to work with
	if (!ma_text)
	{
		return *this;
	}

	// get a pointer to the end of the string
	char *end = &ma_text[strlen(ma_text)];

	// convert hexadecimal string to long
	long value = strtol(ma_text, &end, 16);

	// if the result is zero, things get a lot easier
	if (!value)
	{
		// create new string
		if (!m_createMem(2))
		{
			return *this;
		}

		ma_text[0] = '0';
		ma_text[1] = 0;

		return *this;
	}

	// create a string big enough to hold the value
	char text[21];
	// insert value into string
	ltoa(value, text, 10);

	this->operator =(text);	
	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::DecToHex()
//
// Description:	Converts the string from decimal to hexadecimal form.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString& CCharString::decToHex ()
{
	// check if we have any data to work with
	if (!ma_text)
	{
		return *this;
	}

	// get the value of the string
	long value = atol(ma_text);

	// create a string big enough to hold the value
	char text[30];
	// insert value into string
	ltoa(value, text, 16);

	this->operator =(text);
	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator +=(const char)
//
// Description:	Adds an character to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator += (char rhs)
{
	// check that we have data to work with
	// since if we don't, we can just assign the new value
	if (!ma_text)
	{
		this->operator =(rhs);
		return *this;
	}

	// create a new string, big enough to hold both ma_text and text
	int oldlength = strlen(ma_text);
	char *result = new char[oldlength + 2];  // For a terminating zero

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength);
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
// CCharString::operator +=(const int)
//
// Description:	Adds an integer to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const int rhs)
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
	char text[12];
	// convert the value to a string
	itoa(rhs, text, 10);
	// get the length of the string
	int length = strlen(text);

	// create a new string, big enough to hold both ma_text and text
	int oldlength = strlen(ma_text);
	char *result = new char[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength);
		// copy new data
		memcpy(result + oldlength, text, length);
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
// CCharString::operator +=(const unsigned long)
//
// Description:	Adds an unsigned long to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const unsigned long rhs)
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
	char text[21];
	// convert the value to a string
	ultoa(rhs, text, 10);
	// get the length of the string
	int length = strlen(text);

	// create a new string, big enough to hold both ma_text and text
	int oldlength = strlen(ma_text);
	char *result = new char[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength);
		// copy new data
		memcpy(result + oldlength, text, length);
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
// CCharString::operator +=(const char *)
//
// Description:	Adds another string to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const char *rhs)
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
	int length = strlen(rhs);

	// create a new string, big enough to hold both ma_text and text
	int oldlength = strlen(ma_text);
	char *result = new char[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength);
		// copy new data
		memcpy(result + oldlength, rhs, length);
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
// CCharString::operator +=(const float)
//
// Description:	Adds a float to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const float rhs)
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
	char text[25];
	// convert the float into a string
	gcvt((double)rhs, 15, text);
	// get the length of the string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// recalculate the length of the string
		length = strlen(text);
	}

	// create a new string, big enough to hold both ma_text and text
	int oldlength = strlen(ma_text);
	char *result = new char[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength);
		// copy new data
		memcpy(result + oldlength, text, length);
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
// CCharString::operator +=(const double)
//
// Description:	Adds a float to the current string.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const double rhs)
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
	char text[25];
	// convert the double into a string
	gcvt(rhs, 15, text);
	// get the length of the string
	int length = strlen(text);

	// check if the last character is a dot ('.'), which would mean that the value's
	// decimal part is zero (1.0 for example)
	if (text[length - 1] == '.')
	{
		// add a '0' to the string, to make it look better
		strcat(text, "0");
		// recalculate the length of the string
		length = strlen(text);
	}

	// create a new string, big enough to hold both ma_text and text
	int oldlength = strlen(ma_text);
	char *result = new char[length + oldlength + 1];

	// check that the new data was created
	if (result)
	{
		// copy old data
		memcpy(result, ma_text, oldlength);
		// copy new data
		memcpy(result + oldlength, text, length);
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
// CCharString::operator +=(const CCharString *)
//
// Description:	Adds a string to the current string from another CCharString.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const CCharString *rhs)
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
	int length = strlen(ma_text);
	char *text;

	// check if we're adding to ourselves from ourselves
	if (this == rhs)
	{
		// create a new string, twice as big
		text = new char[(length * 2) + 1];

		// check that we received a legal pointer
		if (text)
		{
			// copy string twice
			memcpy(text, ma_text, length);
			memcpy(text + length, ma_text, length);
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
	int length2 = strlen(rhs->ma_text);
	text = new char[length + length2 + 1];

	// check that we received a legal pointer
	if (text)
	{
		// copy the strings
		memcpy(text, ma_text, length);
		memcpy(text + length, rhs->ma_text, length2);
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
// CCharString::operator +=(const CCharString &)
//
// Description:	Adds a string to the current string from another CCharString.
// Parameters:	None.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator +=(const CCharString &rhs)
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
	int length = strlen(ma_text);
	char *text;

	// check if we're adding to ourselves from ourselves
	if (this == &rhs)
	{
		// create a new string, twice as big
		text = new char[(length * 2) + 1];

		// check that we received a legal pointer
		if (text)
		{
			// copy string twice
			memcpy(text, ma_text, length);
			memcpy(text + length, ma_text, length);
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
	int length2 = strlen(rhs.ma_text);
	text = new char[length + length2 + 1];

	// check that we received a legal pointer
	if (text)
	{
		// copy the strings
		memcpy(text, ma_text, length);
		memcpy(text + length, rhs.ma_text, length2);
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
// CCharString::operator + (const char)
//
// Description:	Creates a reference to an object containing this string plus a character.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus a character.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (char rhs) const
{
	return (CCharString (*this)) += rhs;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const int)
//
// Description:	Creates a reference to an object containing this string plus an integer.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus an integer.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const int rhs) const
{
	return (CCharString (*this)) += rhs;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const unsigned long)
//
// Description:	Creates a reference to an object containing this string plus an unsigned
//				long.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus an unsigned long.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const unsigned long rhs) const
{
	return (CCharString (*this)) += rhs;
}	

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const char *)
//
// Description:	Creates a reference to an object containing this string plus another
//				string.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus another string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const char *rhs) const
{
	return (CCharString (*this)) += rhs;
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const float)
//
// Description:	Creates a reference to an object containing this string plus a float.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus a float.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const float rhs) const
{
	return (CCharString (*this)+= rhs);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const float)
//
// Description:	Creates a reference to an object containing this string plus a float.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus a float.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const double rhs) const
{
	return (CCharString (*this)+= rhs);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const CCharString *)
//
// Description:	Creates a reference to an object containing this string plus the string
//				of the right-hand object.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus the string
//				of the right-hand object.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const CCharString *rhs) const
{
	return (CCharString (*this)+= rhs);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator + (const CCharString &)
//
// Description:	Creates a reference to an object containing this string plus the string
//				of the right-hand object.
// Parameters:	None.
// Returns:		Reference to an object containing this string plus the string
//				of the right-hand object.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator + (const CCharString &rhs) const
{
	return (CCharString (*this)+= rhs);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator <<(const int)
//
// Description:	Shifts the string left and removes the first 'amount' of characters.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		A CCharString containing the shifted string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator << (const int amount)
{
	// check the data
	if (!ma_text)
	{
		// return an empty string
		return CCharString();
	}

	// get the length of the current string
	int length = strlen(ma_text);

	// if the string is shorter than the amount of shifts, return an empty string
	if (length <= amount)
	{
		return CCharString();
	}

	// otherwise return a string with the correct pointer position
	return CCharString((char *)(ma_text + amount));
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator >>(const int)
//
// Description:	Shifts the string right and inserts spaces in the new bytes.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		A CCharString containing the shifted string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::operator >> (const int amount)
{
	// create a new CCharString so we can be sure that its data is removed upon exit
	CCharString tempstring;
	int length;
	
	// check the current string
	if (!ma_text)
	{
		length = 0;
	}
	else
	{
		length = strlen(ma_text);
	}

	// create a new string, big enough to hold both the spaces and the original string
	if (!tempstring.m_createMem(amount + length + 1))
	{
		return CCharString();
	}

	// insert spaces
	int i;
	for (i = 0; i < amount; i++)
	{
		tempstring.ma_text[i] = ' ';
	}

	// copy old string
	if (length > 0)
	{
		memcpy(tempstring.ma_text + amount, ma_text, length);
	}

	// insert eol
	tempstring.ma_text[amount + length] = 0;

	return CCharString(tempstring.ma_text);

	// tempstring's character array will be deleted here
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::operator <<=(const int)
//
// Description:	Shifts the string left and removes the first 'amount' of characters.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator <<=(const int amount)
{
	// check the data
	if (!ma_text)
	{		
		return *this;
	}

	// get the length of the current string
	int length = strlen(ma_text);

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
	char *text = new char[length - amount + 1];

	// check pointer
	if (text)
	{
		// copy the string from the correct location
		memcpy(text, ma_text + amount, length - amount);
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
// CCharString::operator >>=(const int)
//
// Description:	Shifts the string right and inserts spaces in the new bytes.
// Parameters:	const int amount:	The number of characters to be shifted.
// Returns:		Reference to this.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString &CCharString::operator >>=(const int amount)
{
	int length;
	
	// check the current string
	if (!ma_text)
	{
		length = 0;
	}
	else
	{
		length = strlen(ma_text);
	}

	// create a new string, big enough to hold both the spaces and the original string
	char *text = new char[amount + length + 1];

	// check that the new memory was created
	if (text)
	{
		// insert spaces
		int i;
		for (i = 0; i < amount; i++)
		{
			text[i] = ' ';
		}

		// copy old string
		if (length > 0)
		{
			memcpy(text + amount, ma_text, length);
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
// CCharString::operator [](const int)
//
// Description:	Returns the character at the assigned position in the array.
// Parameters:	const int pos:	The position of the character.
// Returns:		The character at the position.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char CCharString::operator [](const int pos) const
{
	// check text
	if (!ma_text)
	{
		return 0;
	}

	// check length
	if (pos >= (int)strlen(ma_text))
	{
		return 0;
	}

	return ma_text[pos];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::set(const char, const int)
//
// Description:	Sets the character at the specified position in the array.
// Parameters:	const char chr:	The new character.
//				const int pos:	The position in the array.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::set(const char chr, const int pos)
{
	// check the pointer
	if (!ma_text)
	{
		return;
	}

	// check the position
	if (pos >= 0 && pos <= (int)strlen(ma_text))
	{
		ma_text[pos] = chr;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getLeft(const int)
//
// Description:	Creates a new string object with the 'num' characters from the left side
//				of the current string.
// Parameters:	int num:	The number of characters to keep from the original string.
// Returns:		A CCharString object containing the new string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString	CCharString::getLeft(const int num)
{
	// check pointer
	if (!ma_text)
	{
		return CCharString();
	}

	// get length of the string
	int length = strlen(ma_text);

	// if the requested number of characters is equal to or more than the current string
	// length we return a copy of this object
	if (num >= length)
	{
		return CCharString(this);
	}

	return CCharString(ma_text, num);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getRight(const int)
//
// Description:	Creates a new string object with the 'num' characters from the right side
//				of the current string.
// Parameters:	int num:	The number of characters to keep from the original string.
// Returns:		A CCharString object containing the new string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString	CCharString::getRight(const int num)
{
	// check pointer
	if (!ma_text)
	{
		return CCharString();
	}

	// get length of the string
	int length = strlen(ma_text);

	// if the requested number of characters is equal to or more than the current string
	// length we return a copy of this object
	if (num >= length)
	{
		return CCharString(this);
	}

	return CCharString(ma_text + (length - num), num);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getMiddle(const int, const int)
//
// Description:	Creates a new string object starting with character 'left' bytes from the
//				left of the string up to 'right'.
// Parameters:	int left:	The number of characters to count up to before copying.
//				int right:	The number of characters to keep from the right side.
// Returns:		A CCharString object containing the new string.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString	CCharString::getMiddle(const int left, const int right)
{
	// check pointer
	if (!ma_text)
	{
		return CCharString();
	}

	// get length of the string
	int length = strlen(ma_text);

	// if left is more than the length of the string, we're keeping nothing
	if (left >= length)
	{
		return CCharString();
	}

	// same goes for right
	if (right >= length)
	{
		return CCharString();
	}

	// now check if the interval between left and right is 0 or less
	if ((length - right) <= left)
	{
		return CCharString();
	}

	// return a new string
	return CCharString(ma_text + left, length - right - left);
}
	
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::replace(const char, const char)
//
// Description:	Replaces all occurences of 'item' with 'type' in the object's member string.
// Parameters:	const char item:	The character to look for.
//				const char type:	The value to replace all occurences with.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharString::replace(const char item, const char type)
{
	convertAllType1ToType2(ma_text, item, type);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::contains(char c)
//
// Description:	Checks if this string contains the passed character
// Parameters:	char c:	The character to look for.
// Returns:		true if found else false.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::contains(char c)
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
// CCharString::contains(char* str)
//
// Description:	Checks if this string contains the passed string.
// Parameters:	char str:	The string to look for within our own string.
// Returns:		true if found else false.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::contains(char* str)
{
	int l1 = getStringLength();
	int l2 = strlen(str);

	if ((l1 < l2) || (l2 <= 0))
	{
		// the string can't contain another string if it is longer
		return false;
	}

	int i(0), j(0);

	for( ; i < ((l1 - l2) + 1); i++)
	{
		for ( ; j < l2; j++)
		{
			if(ma_text[i + j] != str[j])
			{
				break;
			}
		}
		if (j >= l2)
		{
			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::isDigit(int index)
//
// Description:	Checks if the character at position 'index' is a digit
// Parameters:	int index:	The index to check.
// Returns:		true if it's a digit else false.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharString::isDigit(int index)
{
	if(index >= getStringLength())
    {
        return false;
    }

    char c = ma_text[index];

    if(c < '0' || c > '9')
    {
        return false;
    }

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::toInt()
//
// Description:	Converts the string to an int.
// Parameters:	None
// Returns:		If successful, an int. If Not successful the return value is 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CCharString::toInt()
{
    return atoi(ma_text);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::toLong()
//
// Description:	Converts the string to a long.
// Parameters:	None
// Returns:		If successful, a long. If Not successful the return value is 0L.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
long CCharString::toLong()
{
    return atol(ma_text);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::toDouble()
//
// Description:	Converts the string to a double.
// Parameters:	None
// Returns:		If successful, a double. If Not successful the return value is 0.0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
double CCharString::toDouble()
{
    return atof(ma_text);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CCharString::getSubString(int begin, int end)
//
// Description:	Returns a new CCharString which is a substring of this string.
// Parameters:	begin = index to begin copying from. end = index of one character PAST the end.
// Returns:		If successful, an int. If Not successful the return value is 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString CCharString::getSubString(int begin, int end)
{
    if(begin < 0 || begin >= end || end > getStringLength())
    {
        return CCharString();
    }

    CCharString string;

    for(int i(begin); i < end; i++)
    {
        string += ma_text[i];
    }

    return string;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****|
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****|
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****|

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::CTokenizedCharString() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTokenizedCharString::CTokenizedCharString()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::CTokenizedCharString(CCharString&, char, bool) (constructor)
//
// Description:	This constructor takes a CCharString and cuts it up in several smaller
//				tokens, using a delimiter to determine where to cut.
// Parameters:	CCharString& string:	The string that should be tokenized.
//				char delimiter:			The char that describes the cutting points.
//				bool quotes:			If this is true, the parser will ignore the
//										delimiters between two quotation marks ("") and
//										intepret the string as a single token.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTokenizedCharString::CTokenizedCharString(CCharString& string, char delimiter, bool quotes)
{
	tokenizeString(string, delimiter, quotes);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::~CTokenizedCharString() (destructor)
//
// Description:	Destructor.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTokenizedCharString::~CTokenizedCharString()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::tokenizeString()
//
// Description:	This function takes a string and cuts it up into smaller tokens. The
//				tokens are placed in a std::vector of CCharStrings, and can be retrieved
//				through the getToken() function.
// Parameters:	CCharString& string:	The string that should be tokenized.
//				char delimiter:			The delimiting character. The function will look
//										for this char in the string. If two delimiters
//										are placed next to each other, the second de-
//										limiter will be ignored (and so on).
//				bool quotes:			If this is true, the parser will ignore the
//										delimiters between two quotation marks ("") and
//										intepret the string as a single token.
// Returns:		The number of tokens added to the vector.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CTokenizedCharString::tokenizeString(CCharString& string, char delimiter, bool quotes)
{
	int start;
	int end;
	char* ptr;
	char* original;
	bool quoting = false;

	// clear old
	removeAll();
	
	original = string.getString();
	ptr = original;
	start = 0;
	end = 0;

	while (*ptr != 0)
	{
		quoting = false;

		// while the string isn't at eol, and we're not pointing at a delimiter (unless
		// we want quotes and a quote has started), we should continue
		while (*ptr != 0 && ((*ptr != delimiter) || (quotes && quoting)))
		{
			// if we want to keep quotes ("..."), we need to set the trigger so the parser
			// will ignore delimiters from now on, until a new quotation mark is found
			if (*ptr == '\"' && quotes == true)
			{
				if (quoting)
				{
					break;
				}
				else
				{
					quoting = true;
				}
			}

			end++;
			ptr++;
		}

		if (end != start)
		{
			if (quoting)
			{
				if (((end-1) - (start+1)) > 0)
				{
					// if we're quoting, we want to ignore the two quotation marks
					CCharString newstring(&original[start+1], (end-1) - (start+1));
					m_tokens.push_back(newstring);
				}
			}
			else
			{
				CCharString newstring(&original[start], end - start);
				m_tokens.push_back(newstring);
			}

			start = end;
		}

		// skip delimiter
		if (*ptr == delimiter)
		{
			ptr++;
			start++;
			end++;
		}
	}

	return getNumTokens();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::getNumTokens()
//
// Description:	Use this function to retrieve the number of tokens in the class.
// Parameters:	None.
// Returns:		The number of tokens added to the vector.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CTokenizedCharString::getNumTokens()
{
	return (int)m_tokens.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::getToken()
//
// Description:	Use this function to retrieve the a token from the class.
// Parameters:	int pos:	Token index, counting from 0.
// Returns:		A reference to the CCharString token.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharString& CTokenizedCharString::getToken(int pos)
{
	return m_tokens[pos];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CTokenizedCharString::removeAll()
//
// Description:	Clears the string vector.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CTokenizedCharString::removeAll()
{
	m_tokens.clear();
}

CCharString CTokenizedCharString::bundleTokens(int first, int last, char delimiter)
{

	int current = first;
	CCharString results;

	for (current; (current < getNumTokens()) && (current <= last); current++)
	{
		if (current != first)
		{
			results += delimiter;
		}

		results += getToken(current);
	}

	return results;
}

} // end of Basic namespace

