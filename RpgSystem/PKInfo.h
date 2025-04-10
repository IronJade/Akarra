/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    General definitions for player-killing.
 **/

#ifndef _PKINFO_H_
#define _PKINFO_H_

#include <list>

namespace RpgSystem
{

	struct SVictimName
	{
		char	name[16];	// name of victim
		int		seconds;	// expire time
	};
	typedef std::list<SVictimName*>		TDVictimNameList;

	const int	PK_EXPIRETIME		= 10 * 60 * 60;	// default expire time (10 hours)
	const int	PK_PROTECTIONTIME	= 5 * 60;		// protection time for newly respawned players (5 minutes)
}


#endif
