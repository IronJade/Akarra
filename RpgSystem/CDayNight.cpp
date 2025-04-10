/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Server daycycle system.
 **/

#include "CDayNight.h"

namespace RpgSystem
{

int CDayNight::getCurrentTimeGroup(int millitime)
{
	while (millitime > DAYCYCLE_MILLIS)
	{
		millitime -= DAYCYCLE_MILLIS;
	}
	while (millitime < 0)
	{
		millitime += DAYCYCLE_MILLIS;
	}

	return (millitime / DAYCYCLE_GROUP_TIME);
}

char* CDayNight::getTimeGroupName(int group)
{
	const char* const GROUP_NAMES[NUM_CYCLE_GROUPS] =
	{
		"Midnight",
		"Night",
		"Dawn",
		"Morning",
		"Noon",
		"Afternoon",
		"Evening",
		"Dusk",
		"Night"
	};

	return (char*)GROUP_NAMES[group];
}

}
