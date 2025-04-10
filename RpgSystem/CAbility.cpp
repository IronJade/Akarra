/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Abilities are derived from the entity's attributes. They are used in
 *    most calculations.
 **/

#include "CAbility.h"

namespace RpgSystem
{

char* getAbilityName(int ability)
{
	static const char* const ABILITY_NAMES[NUM_ABILITIES] =
	{
		"Attack Rating",
		"Cast Rating",
		"Minimum Damage",
		"Maximum Damage",
		"Armor Class",
		"Resist White",
		"Resist Red",
		"Resist Green",
		"Resist Blue",
		"Resist Yellow",
		"Damage Reduction",
	};

	return (char*)ABILITY_NAMES[ability];
}

}
