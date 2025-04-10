/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Shared helper methods.
 **/

#ifndef _AKARRAUTIL_H_
#define _AKARRAUTIL_H_

// this files holds a bunch of functions that help with common tasks such as
// name-checking and password encryption

#include "../library/basic/CCharString.h"

namespace Common
{

	// "corrects" a name (makes it lowercase) and returns true if no unsupported
	// characters (such as '.', '-' or 'ä') are detected
	// if new name is true, we need to check it against the "protected" names list
	bool correctAccountName(char* ptr);//, bool newname, bool force);
	bool isAnonAccountName(char* name);

	// "corrects" a password (makes it lowercase) and returns true if it has a
	// reasonable length
	bool correctAccountPassword(char* ptr);
#ifdef AKARRASERVER
	Basic::CCharString getRandomPassword();
#endif

	// checks an email so it has a reasonable length and includes a @
	bool correctAccountEmail(char* ptr);

	// checks a character name so it doesn't include offensive or protected nicknames
	// character names are also limited to A-Z characters only.
	bool correctCharacterName(char* name);

	// converts seconds to days/hours/mins/secs
	void convertSecs(unsigned int in, unsigned int& days, unsigned int& hrs, unsigned int& mins, unsigned int& secs);

	// a "world position" can be in any sector and area, so these functions are intended
	// to make it easier to convert positions
	//int getSectorForPosition(unsigned int x, unsigned int y);
	//int getAreaForPosition(unsigned int x, unsigned int y);
	//void getPositionForArea(int sector, int areax, int areay, unsigned int& x, unsigned int& y);

	const int MASK_MOD_SECTOR	= 0x7FF;	// ((32*64)-1), used to get the upper left corner of an area (in global cells)
	const int MASK_MOD_AREA		= 0x3F;		// 63, used to get a cell within a sector
	const int SHIFT_AREA_WIDTH	= 6;		// 64

	const int MASK_MOD_SECTOR2	= 0x7FF;	// ((64*32)-1), used to get the upper left corner of an area (in global cells)
	const int MASK_MOD_AREA2	= 0x1F;		// 31, used to get a cell within a sector
	const int SHIFT_AREA2_WIDTH	= 5;		// 32

	int getSector2ForPosition(unsigned int x, unsigned int y);
	int getArea2ForPosition(unsigned int x, unsigned int y);
	void getPositionForArea2(int sector, int areax, int areay, unsigned int& x, unsigned int& y);

	// used to calculate the distance between two objects in cellspace without using sqrt.
	// the result will rarely be exact, but rather and approximation
	float getCellDistance(int dx, int dy);

	void scrambleString(char* str, int length);
	void xorData(unsigned int* data, int ilength, unsigned int key);

	// anti-hacking:

	const int NUM_CRC_FILES		= 10;
	char* getCrcFilename(int fileid);
	int crcCheckFile(int fileid);

} // end of Common namespace


#endif
