/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Requirement structure used by items etc.
 **/

#ifndef _SREQUIREMENT_H_
#define _SREQUIREMENT_H_

#include "CCharacterAttributes.h"
#include "../library/basic/ccharstring.h"

namespace RpgSystem
{

	enum ERequirementTypes
	{
		RT_DISABLED = -1,	// this requirement isn't used
		RT_LEVEL = 0,		// the character must have aquired a certain level
		RT_DEITY,			// gifted or shining
		RT_GENDER,			// male or female
		RT_COLOR,			// soul color

		RT_CLASS,			// must be of a specific character class
		RT_SKILL,			// must have a certain skill
		RT_LOCKED,			// must have locked a certain skill

		RT_STR,
		RT_CON,
		RT_AGI,
		RT_INT,
		RT_FTH,

		NUM_REQUIREMENT_TYPES
	};

	struct SRequirement
	{
		int	type;	// what kind of requirement (out of ERequirementTypes)
		int	value;	// how much the requirement is
	};

	bool testRequirement(CCharacterAttributes& attribs, SRequirement& req);
	Basic::CCharString getRequirementString(SRequirement& req);
	Basic::CCharString borlandRequirementString(SRequirement& req);

};

#endif
