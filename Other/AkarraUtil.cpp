
#include "AkarraUtil.h"
#include "../PlayerSystem/AkarraAccount.h"
#include "../library/Basic/CCharString.h"

namespace Common
{
	const char* PROTECTED_ACCOUNT_NAMES[] = 
	{
		"khaile",
		"arqon",
		"macc",
		"soma",
		"enji",

		NULL // end of list marker
	};

	// "corrects" a name (makes it lowercase) and returns true if no unsupported
	// characters (such as '.', '-' or 'ä') are detected
	bool correctAccountName(char* ptr, bool newname, bool force)
	{
		int count = 0;

		if (!force && newname)
		{
			int i = 0;
			while (PROTECTED_ACCOUNT_NAMES[i])
			{
				if (strcmp(ptr, PROTECTED_ACCOUNT_NAMES[i]) == 0)
				{
					return false;
				}

				i++;
			}
		}

		while (*ptr)
		{
			if (*ptr >= 'A' && *ptr <= 'Z')
			{
				*ptr += ('a' - 'A'); // make lowercase
			}

			if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= '0' && *ptr <= '9'))
			{
				ptr++;
				count++;
			}
			else
			{
				// this name included illegal characters, so deny it
				return false;
			}
		}

		if (count < 1 || count >= Player::ACCOUNT_NAMELENGTH)
		{
			return false;
		}

		return true;
	}

	// "corrects" a password (makes it lowercase) and returns true if it has a
	// reasonable length
	bool correctAccountPassword(char* ptr)
	{
		int count = 0;
		while (*ptr)
		{
			if (*ptr >= 'A' && *ptr <= 'Z')
			{
				*ptr += ('a' - 'A'); // make lowercase
			}

			if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= '0' && *ptr <= '9'))
			{
				ptr++;
				count++;
			}
			else
			{
				// this name included illegal characters, so deny it
				return false;
			}
		}

		if (count < 1 || count >= Player::ACCOUNT_PASSWORDLENGTH)
		{
			return false;
		}

		return true;
	}

	// checks an email so it has a reasonable length and includes a @
	bool correctAccountEmail(char* ptr)
	{
		Basic::CCharString string(ptr);

		if (!string.contains('@'))
		{
			return false;
		}

		if (string.getStringLength() < 3)
		{
			return false;
		}

		return true;
	}

	const char* PROTECTED_CHARACTER_NAMES[] = 
	{
		"khail",
		"kahil",

		// protect against offensive names
		"fuck",
		"fack",
		"shit",
		"gay",
		"gey",
		"hitler",
		"taliban",
		"laden",

		NULL // end of list marker
	};

	// checks a character name so it doesn't include offensive or protected nicknames
	// character names are also limited to A-Z characters only.
	bool correctCharacterName(char* name)
	{
		// make a copy of the name, because we do not want to ruin the cAsE settings
		Basic::CCharString str(name);

		if (str.getStringLength() < 1 || str.getStringLength() >= Player::ACCOUNT_PASSWORDLENGTH)
		{
			return false;
		}

		char* ptr = str.getString();
		while (*ptr)
		{
			if (*ptr >= 'A' && *ptr <= 'Z')
			{
				*ptr += ('a' - 'A'); // make lowercase
			}

			if (*ptr >= 'a' && *ptr <= 'z')
			{
				ptr++;
			}
			else
			{
				// this name included illegal characters, so deny it
				return false;
			}
		}

		int i = 0;
		while (PROTECTED_CHARACTER_NAMES[i])
		{
			if (str.contains((char*)PROTECTED_CHARACTER_NAMES[i]))
			{
				return false;
			}

			i++;
		}

		return true;
	}

	// converts seconds to days/hours/mins/secs
	void convertSecs(unsigned int in, unsigned int& days, unsigned int& hrs, unsigned int& mins, unsigned int& secs)
	{
		secs = in % 60;
		
		if (in >= 60)
		{
			in /= 60;
			mins = in % 60;

			if (in >= 60)
			{
				in /= 60;
				hrs = in % 24;

				if (in >= 24)
				{
					in /= 24;
					days = in;
				}
				else
				{
					days = 0;
				}
			}
			else
			{
				hrs = 0;
				days = 0;
			}
		}
		else
		{
			mins = 0;
			hrs = 0;
			days = 0;
		}
	}

}

