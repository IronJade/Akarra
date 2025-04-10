/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This file contains the combat rules.
 **/

#include "CAttackCalculations.h"
#include "CAbility.h"
#include "../Common/AkarraUtil.h"
#include <stdlib.h>

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAttackMath::getAttackResult(IAttributeObject* attacker, IAttributeObject* target)
{
	if (!attacker || !target)
	{
		return HIT_MISS;
	}

	// check distance to target
	// distance 0-1 is point blank (melee attacks)
	// 1 - 8 is near
	// 9 - 16 is medium
	// 16+ is far
	int x1, y1, x2, y2;
	attacker->getCoordinates(x1, y1);
	target->getCoordinates(x2, y2);

	x2 -= x1;
	y2 -= y1;
	//x2 = abs(x2);
	//y2 = abs(y2);

	float distancechance = attacker->getDistanceAccuracy(Common::getCellDistance(x2, y2));
	if (distancechance <= 0)
	{
		return HIT_FARMISS;
	}

	// make a dice roll about it
	float r = ((float)(rand()%1000) * 0.001f);
	if (r > distancechance)
	{
		// we've shot far away
		return HIT_FARMISS;
	}

	/*
	if (x2 > 1 || y2 > 1)
	{
		// this isn't point blank, so calculate a correct distance
		float distance = Common::getCellDistance(x2, y2);
		float hitchance = 0;

		if (distance < 8)
		{
			hitchance = attacker->getNearAccuracy();
		}
		else if (distance < 16)
		{
			hitchance = attacker->getMediumAccuracy();
		}
		else
		{
			hitchance = attacker->getFarAccuracy();
		}

		if (hitchance <= 0)
		{
			return HIT_FARMISS;
		}

		// make a dice roll about it
		float r = ((float)(rand()%1000) * 0.001f);
		if (r > hitchance)
		{
			// we've shot far away
			return HIT_FARMISS;
		}
	}
	*/

	float attackrating = attacker->getAbility(ABILITY_ATTACK_RATING);
	float armorrating = target->getAbility(ABILITY_ARMORCLASS);

	int flags = attacker->getAttackFlags(target);
	if (flags & AF_IGNORESHIELD)
	{
		armorrating -= target->getAbility(ABILITY_ARMORCLASS, GAS_SHIELDONLY);
	}

	// calculate the different values
	//float upper = (attackrating * attackrating);
	//float lower = upper + (1.75f * armorrating * armorrating);
	//float ratio = (upper / lower);
	// ($D27/($D27+E$26*$B$27))+(($D27-E$26)*$B$28)
	//float ratio = (attackrating / (attackrating + (armorrating * 1.5f))) + ((attackrating - armorrating) * 0.005f);
	float ratio = (attackrating / (attackrating + (armorrating * 2.0f))) + ((attackrating - (armorrating * 1.5f)) * 0.0030f);

	// the chance to hit is never below 5% or above 95%
	if (ratio < 0.05f)
	{
		ratio = 0.05f;
	}
	else if (ratio > 0.95f)
	{
		ratio = 0.95f;
	}

	// randomize a value between 0 and 1 and compare it to our ratio
	float roll = ((float)(rand()%1000) * 0.001f);

	if (roll <= ratio)
	{
		// we've hit!
		float criticalhit = attacker->getCriticalValue();

		// randomize against the critical value
		roll = ((float)(rand()%1000) * 0.001f);
		if (roll <= criticalhit)
		{
			// critical hit
			return HIT_CRITICAL;
		}
		// normal hit
		return HIT_NORMAL;
	}
	else
	{
		// we've missed :(

		// check for glancing hit
		//lower = upper + (1.25f * armorrating * armorrating);
		//ratio = (upper / lower);
		//ratio = (attackrating / (attackrating + (armorrating * 1.0f))) + ((attackrating - armorrating) * 0.005f);
		ratio = (attackrating / (attackrating + (armorrating * 1.5f)));

		// the chance to hit is never below 5% or above 75%
		if (ratio < 0.05f)
		{
			ratio = 0.05f;
		}
		else if (ratio > 0.75f)
		{
			ratio = 0.75f;
		}

		roll = ((float)(rand()%1000) * 0.001f);
		if (roll <= ratio)
		{
			// glancing hit!
			return HIT_GLANCING;
		}

		float fumble = attacker->getFumbleValue();

		// we've missed the glancing, so check for fumbles
		roll = ((float)(rand()%1000) * 0.001f);
		if (roll <= fumble)
		{
			return HIT_FUMBLE;
		}
	}

	return HIT_MISS;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CAttackMath::getDamageAmount(float min, float max, int attackresult)
{
	if (attackresult == HIT_CRITICAL)
	{
		return max;
	}
	else if (attackresult == HIT_MISS ||
		attackresult == HIT_FUMBLE ||
		attackresult == HIT_FARMISS)
	{
		return 0;
	}

	float delta = max - min;
	float amount(min);

	if (delta > 0)
	{
		int percentage = rand()%1001;
		amount += delta * ((float)percentage * 0.001f);
		//int intdelta = (int)(delta * 1000.0f);

		//amount += ((float)(rand()%intdelta) * 0.001f);
	}

	switch (attackresult)
	{
		case HIT_NORMAL:
			return (amount < 1) ? 1 : amount;
		//case HIT_CRITICAL:
		//	amount *= 2;
		//	return (amount < 1) ? 1 : amount;
		case HIT_GLANCING:
			amount *= 0.33f;
			return (amount < 0) ? 0 : amount;
	}

	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Returns true if the spell is negated.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAttackMath::testNegateEffect(float castrating, float resistance)
{
	// calculate the different values
	float upper = (castrating * castrating);
	float lower = upper + (1.75f * resistance * resistance);
	float ratio = (upper / lower);

	// the chance to hit is never below 5%
	if (ratio < 0.05f)
	{
		ratio = 0.05f;
	}

	// randomize a value between 0 and 1 and compare it to our ratio
	float roll = ((float)(rand()%1000) * 0.001f);

	if (roll <= ratio)
	{
		return false;
	}
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// The decrease test will test the wizard's castrating against the victim's resistance
// over and over again until the wizard "wins". For each time the victim "wins", the cast-
// rating is increased by 50% and the damage is decreased by 33%.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CAttackMath::testDecreaseEffect(float castrating, float resistance)
{
	if (resistance <= 0)
	{
		return 2.5f;
	}

	float damage = castrating / (resistance * 2);

	// normalize the damage slightly towards 1.0f
	// and put a max damage multiplier depending on the resistance of the target
	if (damage > 1.0f)
	{
		//float norm = damage - 1.0f;
		//norm *= 0.5f;
		//damage = 1.0f + norm;
		// NOTE: I removed the norm because PvP damage is decreased by 25% (only monsters deal full damage)

		float maxmultiplier = 2.5f + (resistance / 20.0f);
		if (damage > maxmultiplier)
		{
			damage = maxmultiplier;
		}
	}


	float roll = ((float)(rand()%1000) * 0.001f);
	if (roll < 0.05f)
	{
		damage *= 0.5f;
	}
	else if (roll >= 0.95f)
	{
		damage *= 1.5f;
	}

	return damage;


	/*
	// calculate the different values
	float upper = (castrating * castrating);
	float lower = (1.75f * resistance * resistance);
	float ratio = (upper / (upper + lower));
	float damage = 1.0f;

	do
	{
		// the chance to hit is never below 5%
		if (ratio < 0.05f)
		{
			ratio = 0.05f;
		}

		// randomize a value between 0 and 1 and compare it to our ratio
		float roll = ((float)(rand()%1000) * 0.001f);

		if (roll <= ratio)
		{
			// the wizard won this test
			return damage;	// return current damage value
		}

		// the victim won this test, so increase cast rating and recalculate values
		damage *= 0.67f; // 1.0 -> 0.67 -> 0.45 -> 0.30 -> 0.20 -> 0.135 ...
		castrating *= 1.5f;

		upper = (castrating * castrating);
		//lower = upper + (2.0f * resistance * resistance);
		ratio = (upper / (upper + lower));

	} while(true);

	return 0.01f;
	*/

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CAttackMath::getAttackName(int name, bool s)
{
	if (name < 0 || name >= NUM_ATTACKNAMES)
	{
		if (s)
		{
			return "hits";
		}
		return "hit";
	}

	const char* const ATTACK_NAMES[NUM_ATTACKNAMES] =
	{
		"hit",
		"slash",
		"crush",
		"chop",
		"pierce",
		"punch",
		"bite",
		"claw",
		"squeez",
		"burn",
		"freez",
		"shock",
		"blast",
		"drain",
		"sting",
		"whip"
	};
	const char* const ATTACK_NAMES_S[NUM_ATTACKNAMES] =
	{
		"hits",
		"slashes",
		"crushes",
		"chops",
		"pierces",
		"punches",
		"bites",
		"claws",
		"squeezes",
		"burns",
		"freezes",
		"shocks",
		"blasts",
		"drains",
		"stings",
		"whips"
	};

	if (s)
	{
		return (char*)ATTACK_NAMES_S[name];
	}

	return (char*)ATTACK_NAMES[name];
}

}
