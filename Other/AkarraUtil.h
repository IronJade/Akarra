
#ifndef _AKARRAUTIL_H_
#define _AKARRAUTIL_H_

// this files holds a bunch of functions that help with common tasks such as
// name-checking and password encryption

namespace Common
{

	// "corrects" a name (makes it lowercase) and returns true if no unsupported
	// characters (such as '.', '-' or 'ä') are detected
	// if new name is true, we need to check it against the "protected" names list
	bool correctAccountName(char* ptr, bool newname, bool force);

	// "corrects" a password (makes it lowercase) and returns true if it has a
	// reasonable length
	bool correctAccountPassword(char* ptr);

	// checks an email so it has a reasonable length and includes a @
	bool correctAccountEmail(char* ptr);

	// checks a character name so it doesn't include offensive or protected nicknames
	// character names are also limited to A-Z characters only.
	bool correctCharacterName(char* name);

	// converts seconds to days/hours/mins/secs
	void convertSecs(unsigned int in, unsigned int& days, unsigned int& hrs, unsigned int& mins, unsigned int& secs);

} // end of Common namespace


#endif
