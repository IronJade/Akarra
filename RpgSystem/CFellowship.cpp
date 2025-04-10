/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player fellowship rules.
 **/

#include "CFellowship.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CFellowship::getCircle(bool white, bool red, bool green, bool blue, bool yellow)
{
	// Global powers

	if (white && red && green && blue && yellow)
	{
		return FC_PERFECTCIRCLE;
	}

	if (!white && !red && !green && !blue && !yellow)
	{
		return FC_THEVOID;
	}

	// Major powers
	
	if (white && !red && !green && blue && yellow)
	{
		return FC_DIVINEPOWERS;
	}

	if (!white && red && green && blue && !yellow)
	{
		return FC_BALANCEDELEMENTS;
	}

	if (white && !red && green && !blue && yellow)
	{
		return FC_GREATSTRUGGLE;
	}

	if (white && red && !green && blue && !yellow)
	{
		return FC_BURNINGSTORM;
	}

	if (!white && red && green && !blue && yellow)
	{
		return FC_TURNSOFFATE;
	}

	// Minor powers

	if (white && red && !green && !blue && !yellow)
	{
		return FC_PUREFLAME;
	}

	if (white && !red && green && !blue && !yellow)
	{
		return FC_GREATESTILLUSION;
	}

	if (!white && red && !green && !blue && yellow)
	{
		return FC_SOULCREMATION;
	}

	if (!white && !red && green && blue && !yellow)
	{
		return FC_STREAMOFLIFE;
	}

	if (!white && !red && !green && blue && yellow)
	{
		return FC_FROSTDEATH;
	}

	return FC_NONE;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CFellowship::getCircleName(int type)
{
	const char* const CIRCLENAMES[NUM_FELLOWSHIP_CIRCLES] =
	{
		"",
		"The Void",
		"Perfect Circle",

		"Divine Powers",
		"Balanced Elements",
		"Great Struggle",
		"Burning Storm",
		"Turns of Fate",

		"Pure Flame",
		"Greatest Illusion",
		"Soul Cremation",
		"Stream of Life",
		"Frostdeath"
	};

	return (char*)CIRCLENAMES[type];
}

}
