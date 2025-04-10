/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This file contains the combat rules.
 **/

#ifndef _ATTACKCALCULATIONS_H_
#define _ATTACKCALCULATIONS_H_

#include "IAttributeObject.h"

namespace RpgSystem
{

	// these are the possible hit results for melee and ballistic attacks
	enum EHitResultTypes
	{
		HIT_NORMAL,
		HIT_CRITICAL,
		HIT_GLANCING,

		HIT_MISS,
		HIT_FUMBLE,

		// ranged attack results

		HIT_FARMISS,	// never even reached is target

	};

	// these are the possible damage types (depending on weapon or spell used)
	enum EDamageTypes
	{
		DMG_UNKNOWN = 0,	// special type

		// the four basic physical types:
			
		DMG_SLASHING,
		DMG_PIERCING,
		DMG_CRUSHING,
		DMG_CHOPPING,

		// the five spell types:

		DMG_WHITE,
		DMG_RED,
		DMG_GREEN,
		DMG_BLUE,
		DMG_YELLOW,

		NUM_DAMAGETYPES		
	};

	// these are different "attack types" used for basic and ballistic weapons
	// the monsters can have all of these attack types, while the players are limited
	// the the first few
	enum EAttackTypes
	{
		// attacks available to players and monsters

		ATT_NORMALMELEE = 0,// normal melee attack

		ATT_BULLET,			// ranged bullet attack (slings)
		ATT_ARROW,			// ranged arrow attack (bows and crossbows)

		// attacks available to monsters

		ATT_DOUBLEMELEE,	// will attack twice at the same time
		ATT_TRIPLEMELEE,	// will attack trice at the same time
	};

	// attack "names"
	enum EAttackNames
	{
		AN_HITS = 0,		// "Khaile hits ..."

		AN_SLASHES,			// "Khaile slashes ..."
		AN_CRUSHES,
		AN_CHOPS,
		AN_PIERCES,
		AN_PUNCHES,

		AN_BITES,
		AN_CLAWS,
		AN_SQUEEZES,

		AN_BURNS,
		AN_FREEZES,
		AN_SHOCKS,
		AN_BLASTS,
		AN_DRAINS,

		AN_STINGS,
		AN_WHIPS,

		NUM_ATTACKNAMES
	};

	// attack flags
	enum EAttackFlags
	{
		AF_IGNORESHIELD = 0x1,	// ignores the defender's shield AC
	};

	const int	FUMBLE_RECOVERY_MULTIPLIER	= 2;	// extra time upon fumbles

	class CAttackMath
	{
		public:

			static int getAttackResult(IAttributeObject* attacker, IAttributeObject* target);
			static float getDamageAmount(float min, float max, int attackresult);

			static char* getAttackName(int name, bool s);

			static bool testNegateEffect(float castrating, float resistance);
			static float testDecreaseEffect(float castrating, float resistance);

	};

	// The class "IAttackable" was used to create a uniform style for both player characters
	// and monsters, but only for combat calculations. This wasn't (obviously) enough, so I
	// removed the class and created a new one in "IAttributeObject.h".

	/*
	class IAttackable
	{
		public:
		
			// Should return the objects current attack rating
			virtual float getAttackRating() = 0;

			// Should return the objects current armor class
			virtual float getArmorClass() = 0;

			// Should evaluate the damagetype and modify the amount of damage given (if any)
			// and return the real amount of damage given
			virtual float dealDamage(float damage, EDamageTypes damagetype, int attacker) = 0;

			// Should decrease the attacker's fatigue based on the weapon he's weilding
			virtual void addAttackFatigue() = 0;

			// Should decrease the target's fatigue based on the armor he's carrying
			virtual void addDefenseFatigue() = 0;

			// Should return true if the object still can fight and be attacked
			virtual bool isAlive() = 0;
	};
	*/
}


#endif

