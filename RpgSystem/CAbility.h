/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Abilities are derived from the entity's attributes. They are used in
 *    most calculations.
 **/

#ifndef _CABILITY_H_
#define _CABILITY_H_

#include "CAttribute.h"

namespace RpgSystem
{

	// Great, o holy list of skills
	// (020517): The monsters are using this list too, so make sure you update monster class
	//           if this list is modified.
	// (020505): Important note. ALWAYS use setBase() for attribute points (ie don't use
	//           addBonus() for damage and such. This is because it's easier to calculate.
	// (011028): Major rework... removed weapon abilities and only kept "attack rating",
	//           and added a "cast rating" for spells.
	// (011028): Added ABILITY_ARMORCLASS.
	// (011027): Removed ABILITY_WEAPON_LARGE because it was redundant.
	enum EAbilities
	{
		// Combat Skills
		ABILITY_ATTACK_RATING = 0,
		ABILITY_CAST_RATING,

		// damage
		ABILITY_DAMAGE_MIN,
		ABILITY_DAMAGE_MAX,

		// defense
		ABILITY_ARMORCLASS,

		// resistances
		ABILITY_RESIST_WHITE,
		ABILITY_RESIST_RED,
		ABILITY_RESIST_GREEN,
		ABILITY_RESIST_BLUE,
		ABILITY_RESIST_YELLOW,

		// defense 2 (note: added this here because it's easier to update savefiles then)
		// damage reduction reduces damage inflicted by normal weapons (not spells)
		ABILITY_REDUCTION,

		NUM_ABILITIES
	};

	char* getAbilityName(int ability);

	/* **********************************************************************************
	* CLASS CAbility
	* Derived:	(base class)
	*
	* Purpose:	To keep information about a single character base ability.
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011026): Removed the class and made it a typdef of CAttribute instead.
	* (011026): Renamed the class from CSkill to CAbility.
	* (011022): First version.
	* **********************************************************************************/
	typedef CAttribute CAbility;

} // end of RpgSystem namespace

#endif
