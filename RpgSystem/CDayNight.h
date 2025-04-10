/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Server daycycle system.
 **/

#ifndef _CDAYNIGHT_H_
#define _CDAYNIGHT_H_

namespace RpgSystem
{

	const int	DAYCYCLE_HOURS		= 3;
	const int	DAYCYCLE_MINUTES	= DAYCYCLE_HOURS * 60;
	const int	DAYCYCLE_SECONDS	= DAYCYCLE_MINUTES * 60;
	const int	DAYCYCLE_MILLIS		= DAYCYCLE_SECONDS * 1000;

	enum EDayCycleTimes
	{
		DCT_MIDNIGHT = 0,
		DCT_MORNINGNIGHT,

		DCT_DAWN,
		DCT_MORNING,

		DCT_NOON,

		DCT_AFTERNOON,
		DCT_EVENING,
		DCT_DUSK,

		DCT_EVENINGNIGTH,

		NUM_CYCLE_GROUPS
	};
	const int DAYCYCLE_GROUP_TIME = (DAYCYCLE_MILLIS / NUM_CYCLE_GROUPS);

	class CDayNight
	{
		public:

			static int getCurrentTimeGroup(int millitime);
			static char* getTimeGroupName(int group);

	};
}


#endif
