/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Shared helper methods.
 **/

#include "AkarraUtil.h"
#include "../PlayerSystem/AkarraAccount.h"
#include "../library/Basic/CCharString.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/FileSystem.h"

#ifndef AKARRASERVER
#include <stdlib.h>
#endif

namespace Common
{
	const char* PROTECTED_ACCOUNT_NAMES[] = 
	{
		//"khaile",
		//"arqon",
		//"macc",
		//"soma",
		//"enji",

		NULL // end of list marker
	};

	// "corrects" a name (makes it lowercase) and returns true if no unsupported
	// characters (such as '.', '-' or 'ä') are detected
	bool correctAccountName(char* ptr)//, bool newname, bool force)
	{
		int count = 0;

		//if (!force && newname)
		//{
		//	int i = 0;
		//	while (PROTECTED_ACCOUNT_NAMES[i])
		//	{
		//		if (strcmp(ptr, PROTECTED_ACCOUNT_NAMES[i]) == 0)
		//		{
		//			return false;
		//		}
		//
		//		i++;
		//	}
		//}

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
				// check for certain characters
				if ((*ptr == '_') ||
					(*ptr == '-') ||
					(*ptr == '=') ||
					(*ptr == '(') ||
					(*ptr == ')') ||
					(*ptr == '!') ||
					(*ptr == '#')
					)
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
		}

		if (count < 1 || count >= Player::ACCOUNT_NAMELENGTH)
		{
			return false;
		}

		return true;
	}

	// this function checks if the account is an anon account
	bool isAnonAccountName(char* name)
	{
		if ((strcmp("anon", name) == 0) ||
			(strcmp("anon1", name) == 0) ||
			(strcmp("anon2", name) == 0) ||
			(strcmp("anon3", name) == 0) ||
			(strcmp("anon4", name) == 0) ||
			(strcmp("anon5", name) == 0))
		{
			return true;
		}
		return false;
	}

	// "corrects" a password (makes it lowercase) and returns true if it has a
	// reasonable length
	bool correctAccountPassword(char* ptr)
	{
		int count = 0;
		/*
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
		*/
		count = strlen(ptr);

		if (count < 1 || count >= Player::ACCOUNT_PASSWORDLENGTH)
		{
			return false;
		}

		return true;
	}

	// this function generates a random password from two name lists (easier to remember than
	// completely random garbage)
#ifdef AKARRASERVER
	Basic::CCharString getRandomPassword()
	{
		const char* PW_FIRST[20] =
		{
			"White", "Green", "Black", "Yellow",
			"Diamond", "Opaque", "Jade", "Golden",
			"Silvery", "Shiny", "Fire", "Ice",
			"Poison", "Liquid", "Master", "Swedish",
			"Strong", "Quick", "Fast", "Flying"
		};
		const char* PW_SECOND[20] =
		{
			"Dragon", "Troll", "Ogre", "Giant",
			"Human", "Man", "Woman", "Pig",
			"Pigeon", "Trout", "Zebra", "Snake",
			"Spider", "Turtle", "Wolf", "Rabbit",
			"Rat", "Bird", "Ghost", "Zombie"
		};

		Basic::CCharString pass;
		pass = (char*)PW_FIRST[rand()%20];
		pass += (char)('2' + rand()%8); // avoid 0 and 1 because of similarities with other letters
		pass += (char*)PW_SECOND[rand()%20];

		return Basic::CCharString(pass);
	}
#endif

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
		"akarr",

		// protect against offensive names
		"fuck",
		"fack",
		"shit",
		"gay",
		"hitler",
		"taliban",
		"laden",
		"cunt",
		"puss",
		"nazi",
		"nigg",
		"cock",
		"suck",

		NULL // end of list marker
	};

	// checks a character name so it doesn't include offensive or protected nicknames
	// character names are also limited to A-Z characters only.
	bool correctCharacterName(char* name)
	{
		// make a copy of the name, because we do not want to ruin the cAsE settings
		Basic::CCharString str(name);

		if (str.getStringLength() < 1 || str.getStringLength() >= Player::ACCOUNT_NAMELENGTH)
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

	// a "world position" can be in any sector and area, so these functions are intended
	// to make it easier to convert positions

	/*
	int getSectorForPosition(unsigned int x, unsigned int y)
	{
		// there are 4 sectors per "sector row", and 32 areas of 64 cells in each sector...
		// so a sector is 32*64 = 2048 cells, and a sector row is 8192 cells
		int sectorx = (x >> 11);
		int sectory = (y >> 11);

		return (sectorx + (sectory << 2));
	}

	int getAreaForPosition(unsigned int x, unsigned int y)
	{
		// to get the area position we need to remove the sector overhead and divide
		// by 64 (each area has 64 cells)
		// ~0x800 = ~(1000 0000 0000) = 111 1111 1111 = 0x7FF
		int areax = (x & 0x7FF) >> 6;
		int areay = (y & 0x7FF) >> 6;

		// each sector has 32 areas in each row
		return (areax + (areay << 5));
	}

	void getPositionForArea(int sector, int areax, int areay, unsigned int& x, unsigned int& y)
	{
	}
	*/

	int getSector2ForPosition(unsigned int x, unsigned int y)
	{
		// there are 4 sectors per "sector row", and 64 areas of 32 cells in each sector...
		// so a sector is 64*32 = 2048 cells, and a sector row is 8192 cells
		int sectorx = (x >> 11);
		int sectory = (y >> 11);

		return (sectorx + (sectory << 2));
	}

	int getArea2ForPosition(unsigned int x, unsigned int y)
	{
		// to get the area position we need to remove the sector overhead and divide
		// by 32 (each area2 has 32 cells)
		// ~0x800 = ~(1000 0000 0000) = 111 1111 1111 = 0x7FF
		int areax = (x & 0x7FF) >> 5;
		int areay = (y & 0x7FF) >> 5;

		// each sector has 64 areas in each row
		return (areax + (areay << 6));
	}

	void getPositionForArea2(int sector, int areax, int areay, unsigned int& x, unsigned int& y)
	{
	}

	// used to calculate the distance between two objects in cellspace without using sqrt.
	// the result will rarely be exact, but rather and approximation
	float getCellDistance(int dx, int dy)
	{
		const float DIAGONAL_DISTANCE = 1.414214f;

		if (dx < 0)
		{
			dx = -dx;
		}
		if (dy < 0)
		{
			dy = -dy;
		}

		if (dy < dx)
		{
			int temp = dy;
			dy = dx;
			dx = temp;
		}

		// delta y is now the largest of the two, so use dx for diagonal steps and dy for the rest
		dy -= dx;

		float results((float)dx * DIAGONAL_DISTANCE);
		results += (float)dy;

		return results;
	}

	void scrambleString(char* str, int length)
	{
		bool start = false;

		for (int i(0); i < length; i++)
		{
			if (start)
			{
				str[i] = rand()%0xFF;
			}
			else if (str[i] == 0)
			{
				start = true;
			}
		}
	}

	void xorData(unsigned int* data, int ilength, unsigned int key)
	{
		for (int i(0); i < ilength; i++)
		{
			data[i] ^= key;
		}
	}

	#define MAXFILENAME (128)
	#define FILENAMEXOR (0xF5781E5B)
	#define INTNAME(s) \
			(int)s[0] ^ FILENAMEXOR, \
			(int)s[1] ^ FILENAMEXOR, \
			(int)s[2] ^ FILENAMEXOR, \
			(int)s[3] ^ FILENAMEXOR, \
			(int)s[4] ^ FILENAMEXOR, \
			(int)s[5] ^ FILENAMEXOR, \
			(int)s[6] ^ FILENAMEXOR, \
			(int)s[7] ^ FILENAMEXOR, \
			(int)s[8] ^ FILENAMEXOR, \
			(int)s[9] ^ FILENAMEXOR, \
			(int)s[10] ^ FILENAMEXOR, \
			(int)s[11] ^ FILENAMEXOR, \
			(int)s[12] ^ FILENAMEXOR, \
			(int)s[13] ^ FILENAMEXOR, \
			(int)s[14] ^ FILENAMEXOR, \
			(int)s[15] ^ FILENAMEXOR, \
			(int)s[16] ^ FILENAMEXOR, \
			(int)s[17] ^ FILENAMEXOR, \
			(int)s[18] ^ FILENAMEXOR, \
			(int)s[19] ^ FILENAMEXOR, \
			(int)s[20] ^ FILENAMEXOR, \
			(int)s[21] ^ FILENAMEXOR, \
			(int)s[22] ^ FILENAMEXOR, \
			(int)s[23] ^ FILENAMEXOR, \
			(int)s[24] ^ FILENAMEXOR, \
			(int)s[25] ^ FILENAMEXOR, \
			(int)s[26] ^ FILENAMEXOR, \
			(int)s[27] ^ FILENAMEXOR, \
			(int)s[28] ^ FILENAMEXOR, \
			(int)s[29] ^ FILENAMEXOR, \
			(int)s[30] ^ FILENAMEXOR, \
			(int)s[31] ^ FILENAMEXOR, \
			(int)s[32] ^ FILENAMEXOR, \
			(int)s[33] ^ FILENAMEXOR, \
			(int)s[34] ^ FILENAMEXOR, \
			(int)s[35] ^ FILENAMEXOR, \
			(int)s[36] ^ FILENAMEXOR, \
			(int)s[37] ^ FILENAMEXOR, \
			(int)s[38] ^ FILENAMEXOR, \
			(int)s[39] ^ FILENAMEXOR, \
			(int)s[40] ^ FILENAMEXOR, \
			(int)s[41] ^ FILENAMEXOR, \
			(int)s[42] ^ FILENAMEXOR, \
			(int)s[43] ^ FILENAMEXOR, \
			(int)s[44] ^ FILENAMEXOR, \
			(int)s[45] ^ FILENAMEXOR, \
			(int)s[46] ^ FILENAMEXOR, \
			(int)s[47] ^ FILENAMEXOR, \
			(int)s[48] ^ FILENAMEXOR, \
			(int)s[49] ^ FILENAMEXOR, \
			(int)s[50] ^ FILENAMEXOR, \
			(int)s[51] ^ FILENAMEXOR, \
			(int)s[52] ^ FILENAMEXOR, \
			(int)s[53] ^ FILENAMEXOR, \
			(int)s[54] ^ FILENAMEXOR, \
			(int)s[55] ^ FILENAMEXOR, \
			(int)s[56] ^ FILENAMEXOR, \
			(int)s[57] ^ FILENAMEXOR, \
			(int)s[58] ^ FILENAMEXOR, \
			(int)s[59] ^ FILENAMEXOR, \
			(int)s[60] ^ FILENAMEXOR, \
			(int)s[61] ^ FILENAMEXOR, \
			(int)s[62] ^ FILENAMEXOR, \
			(int)s[63] ^ FILENAMEXOR, \
			(int)s[64] ^ FILENAMEXOR, \
			(int)s[65] ^ FILENAMEXOR, \
			(int)s[66] ^ FILENAMEXOR, \
			(int)s[67] ^ FILENAMEXOR, \
			(int)s[68] ^ FILENAMEXOR, \
			(int)s[69] ^ FILENAMEXOR, \
			(int)s[70] ^ FILENAMEXOR, \
			(int)s[71] ^ FILENAMEXOR, \
			(int)s[72] ^ FILENAMEXOR, \
			(int)s[73] ^ FILENAMEXOR, \
			(int)s[74] ^ FILENAMEXOR, \
			(int)s[75] ^ FILENAMEXOR, \
			(int)s[76] ^ FILENAMEXOR, \
			(int)s[77] ^ FILENAMEXOR, \
			(int)s[78] ^ FILENAMEXOR, \
			(int)s[79] ^ FILENAMEXOR, \
			(int)s[80] ^ FILENAMEXOR, \
			(int)s[81] ^ FILENAMEXOR, \
			(int)s[82] ^ FILENAMEXOR, \
			(int)s[83] ^ FILENAMEXOR, \
			(int)s[84] ^ FILENAMEXOR, \
			(int)s[85] ^ FILENAMEXOR, \
			(int)s[86] ^ FILENAMEXOR, \
			(int)s[87] ^ FILENAMEXOR, \
			(int)s[88] ^ FILENAMEXOR, \
			(int)s[89] ^ FILENAMEXOR, \
			(int)s[90] ^ FILENAMEXOR, \
			(int)s[91] ^ FILENAMEXOR, \
			(int)s[92] ^ FILENAMEXOR, \
			(int)s[93] ^ FILENAMEXOR, \
			(int)s[94] ^ FILENAMEXOR, \
			(int)s[95] ^ FILENAMEXOR, \
			(int)s[96] ^ FILENAMEXOR, \
			(int)s[97] ^ FILENAMEXOR, \
			(int)s[98] ^ FILENAMEXOR, \
			(int)s[99] ^ FILENAMEXOR, \
			(int)s[100] ^ FILENAMEXOR, \
			(int)s[101] ^ FILENAMEXOR, \
			(int)s[102] ^ FILENAMEXOR, \
			(int)s[103] ^ FILENAMEXOR, \
			(int)s[104] ^ FILENAMEXOR, \
			(int)s[105] ^ FILENAMEXOR, \
			(int)s[106] ^ FILENAMEXOR, \
			(int)s[107] ^ FILENAMEXOR, \
			(int)s[108] ^ FILENAMEXOR, \
			(int)s[109] ^ FILENAMEXOR, \
			(int)s[110] ^ FILENAMEXOR, \
			(int)s[111] ^ FILENAMEXOR, \
			(int)s[112] ^ FILENAMEXOR, \
			(int)s[113] ^ FILENAMEXOR, \
			(int)s[114] ^ FILENAMEXOR, \
			(int)s[115] ^ FILENAMEXOR, \
			(int)s[116] ^ FILENAMEXOR, \
			(int)s[117] ^ FILENAMEXOR, \
			(int)s[118] ^ FILENAMEXOR, \
			(int)s[119] ^ FILENAMEXOR, \
			(int)s[120] ^ FILENAMEXOR, \
			(int)s[121] ^ FILENAMEXOR, \
			(int)s[122] ^ FILENAMEXOR, \
			(int)s[123] ^ FILENAMEXOR, \
			(int)s[124] ^ FILENAMEXOR, \
			(int)s[125] ^ FILENAMEXOR, \
			(int)s[126] ^ FILENAMEXOR, \
			(int)s[127] ^ FILENAMEXOR
	
	// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
	// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
	char* getCrcFilename(int fileid)
	{
		const int CRCFILENAMES[NUM_CRC_FILES][MAXFILENAME] =
			{
				{ INTNAME("Akarra.exe\0----|||||-----|||||-----|||||-----|||||-----|||||-----|||||--lisgfljsdghfljsagfljhbavmcbiew---|||||---rh|||||-----|||") },
				{ INTNAME("sectors/caves.sc2\0----|||||-----|||||--öadubvökcbgöiurheapoth24ö3uh5ökjhfgökahaöuihh||||-----|||||-----|||||-----|||||----|||||-") },
				{ INTNAME("sectors/caves2.sc2\0----|||||-----|||||-----|||||-----|||||-----alsjdhfblöehwbrökwhbeökjbnsökvbödjhvökahkögjerahök|||||-----|||||") },
				{ INTNAME("gfx/cave.ats\0---ert||||-----|||||-----|||||---b9vycb9h234ouhkshefsödhpiah HÖKGHÖGshkgbasökhfbköwherb p72yr9p8y239yfduhk sdäjflds") },
				{ INTNAME("gfx/indoors.ats\0----|||||----öahtöoiuh4wöuihröku43hök5h3jkh5köjh34n,tbf.dbgödfghökufdha --|||||-----|||||----rh||||-----|||||---") },
				{ INTNAME("gfx/temple.ats\0----|sldkflsdjflkj4eliwjrlkwejrljsdlkncsd---|||||-----|||||-----|||||-----|||sdfsdfsd sdfsdf---|||||-----||||hg--") },
				{ INTNAME("gfx/woodland.ss2\0rthtr|||||-----|||||-----|||||-----|||||--sfgdfgsdgf---|||||-----|||||-----||||fghgfhgf||-----|||||-----|||||--") },
				{ INTNAME("gfx/darkwood.ss2\0----|||||-----lkvj h f5 776 84575675676-----|||||-----|||||-----|||||-----|||||-----|||||-----dfgdfgdfgaadfh|--") },
				{ INTNAME("gfx/interior.ss2\0----|||||-----|||||-----|||||-----|||||--sfghsgfhfsghgsfhsgfhsfhfsghgfgh--|||||-----fjkhfjkfjhj||||-----|||||--") },
				{ INTNAME("gfx/wetcave.ss2\0----|||||----sfdgaefgadfgfadLUYGLJGSFDkuh----|||||-----|||||-----||||asdkjagslffsdöfgöiuefhku3hb|||-----|||||---") }
			};

		// convert the filename back from its old value
		static char name[MAXFILENAME];
		for (int i(0); i < (int)MAXFILENAME; i++)
		{
			int token = CRCFILENAMES[fileid][i];
			token ^= FILENAMEXOR;
			name[i] = (char)(token & 0xFF);
			if (!name[i])
			{
				break;
			}
		}
		return name;
	}

	// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
	// This function is used to CRC-check a file in the Akarra directory, to make sure the
	// user uses the same version of a file as the server does. Unfortunately hackers aren't
	// that stupid, so they will notice when this happens and look for this function in the
	// .exe by searching for filenames. To make this harder, I've made a macro that converts
	// the filenames into arrays of integers and then back again.
	// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
	int crcCheckFile(int fileid)
	{
		// get filename
		char* filename = getCrcFilename(fileid);

		// attempt to open
		FileSystem::CFileStream file = FileSystem::directOpen(filename, "rb");
		if (!file.isOk())
		{
			return 0;
		}

		// create a buffer of ints
		const long MAX_READ_SIZE = 32 * 1024; // 32 kB
		int* buffer = new int[MAX_READ_SIZE / 4];

		// get file size
		long size = file.getSize();

		// the crc starts by storing the size of the file (so people can't fill it with 'nop' commands to hide the hack)
		int crc = (int)(size & 0x00000000FFFFFFFF);

		while (size > 0)
		{
			long readsize = size;
			if (readsize > MAX_READ_SIZE)
			{
				readsize = MAX_READ_SIZE;
			}
			file.read(buffer, sizeof(int), (int)readsize / sizeof(int));

			for (int i(0); i < ((int)readsize / sizeof(int)); i++)
			{
				// note: this will most likely overflow, but that doesn't matter
				crc += buffer[i];
			}

			size -= readsize;
		}

		FileSystem::directClose(file);
		delete [] buffer;

		return crc;
	}

}

