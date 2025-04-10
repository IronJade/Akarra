/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player character levels.
 **/

#ifndef _CEXPERIENCE_H_
#define _CEXPERIENCE_H_

namespace RpgSystem
{
	// some constants:
	const int	LEVEL_START					= 0;
	const int	LEVEL_MAX					= 250;
	const int	LEVEL_INCREASEDDELTA		= 50;	// at this level you can gain exp from a 20-level difference
	const int	ATTRIBUTES_PER_LEVEL		= 5;
	const int	HIGHLEVEL_BORDER			= 100;
	const int	ATTRIBUTES_PER_HIGHLEVEL	= 3;	// levels 100 to 125 only gives 3 attributes per level
	const int	NOATTR_BORDER				= 125;	// after level 125 you never gain any more attributes
	const int	UNLIMITED_EXP				= 0x7FFFFFFF;	// this value is returned if it's impossible to gain more

	// static class for experience calculations
	class CExperienceMath
	{
		public:

			static int getCurrentLevel(int experience);
			static int getNextLevelExpByExp(int experience);
			static int getNextLevelExpByLevel(int level);

			static int getProgressType(int level);
			static int getProgressPosition(int level, int experience);

			static int getNextAttribute(int level, int experience);			

			static int addExperience(int srcamount, int srclevel, int destlevel, int deltashift = 0);
	};
}

#endif
