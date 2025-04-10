/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player character levels.
 **/

#include "CExperience.h"

namespace RpgSystem
{

const int EXPERIENCE_LEVELS[LEVEL_MAX + 1] =
{
	0,

	// level 1-10
	250, 750, 1500, 2500, 3750, 5250, 7000, 9000, 11250, 13750,
	// 11-20
	19250, 25250, 31750, 38750, 46250, 54250, 62750, 71750, 81250, 91250,
	// 21-30
	101750, 112750, 124250, 136250, 148750, 161750, 175250, 189250, 203750, 218750,
	// 31-40
	251300, 284900, 319550, 355250, 392000, 429800, 468650, 508550, 549500, 591500,
	// 41-50
	634550, 678650, 723800, 770000, 817250, 865550, 914900, 965300, 1016750, 1069250,
	// 51-60
	1235000, 1404000, 1576250, 1751750, 1930500, 2112500, 2297750, 2486250, 2678000, 2873000,
	// 61-70
	3071250, 3272750, 3477500, 3685500, 3896750, 4111250, 4329000, 4550000, 4774250, 5001750,
	// 71-80
	5232500, 5466500, 5703750, 5944250, 6188000, 6454000, 6730815, 7018635, 7317650, 7628050,
	// 81-90
	7950025, 8283765, 8629460, 8987300, 9357475, 9740175, 10135590, 10543910, 10965325, 11400025,
	// 91-100
	11848200, 12310040, 12785735, 13275475, 13779450, 14297850, 14830865, 15378685, 15941500, 16519500,
	// 101-110
	17112875, 17721815, 18346510, 18987150, 19643925, 20317025, 21006640, 21712960, 22436175, 23176475,
	// 111-120
	23934050, 24709090, 25501785, 26312325, 27140900, 27987700, 28852915, 29736735, 30639350, 31560950,
	// 121-130
	32501725, 33461865, 34441560, 35441000, 36460375, 38255875, 40065625, 41889625, 43727875, 45580375,
	// 131-140
	47447125, 49328125, 51223375, 53132875, 55056625, 56994625, 58946875, 60913375, 62894125, 64889125,
	// 141-150
	66898375, 68921875, 70959625, 73011625, 75077875, 77158375, 79253125, 81362125, 83485375, 85622875,
	// 151-160
	87880325, 90152725, 92440075, 94742375, 97059625, 99391825, 101738975, 104101075, 106478125, 108870125,
	// 161-170
	111277075, 113698975, 116135825, 118587625, 121054375, 123536075, 126032725, 128544325, 131070875, 133612375,
	// 171-180
	136168825, 138740225, 141326575, 143927875, 146544125, 149175325, 151821475, 154482575, 157158625, 159849625,
	// 181-190
	162555575, 165276475, 168012325, 170763125, 173528875, 176309575, 179105225, 181915825, 184741375, 187581875,
	// 191-200
	192347325, 197329725, 202531075, 207953375, 213598625, 219468825, 225565975, 231892075, 238449125, 245449125,
	// 201-250
	260000000, 275000000, 300000000, 325000000, 350000000, 375000000, 400000000, 425000000, 450000000, 475000000,
	500000000, 525000000, 550000000, 575000000, 600000000, 625000000, 650000000, 675000000, 700000000, 725000000,
	750000000, 775000000, 800000000, 825000000, 850000000, 875000000, 900000000, 925000000, 950000000, 975000000,
	1000000000, 1025000000, 1050000000, 1075000000, 1100000000, 1125000000, 1150000000, 1175000000, 1200000000, 1225000000,
	1250000000, 1275000000, 1300000000, 1325000000, 1350000000, 1375000000, 1400000000, 1425000000, 1450000000, 1475000000
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// returns the current level based on an experience count
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::getCurrentLevel(int experience)
{
	for (int i(0); i <= LEVEL_MAX; i++)
	{
		if (experience < EXPERIENCE_LEVELS[i])
		{
			return (i - 1);
		}
	}

	return LEVEL_MAX;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// returns the amount of experience that is required to reach the next level,
// calculated from a current experience value
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::getNextLevelExpByExp(int experience)
{
	for (int i(0); i <= LEVEL_MAX; i++)
	{
		if (experience < EXPERIENCE_LEVELS[i])
		{
			return EXPERIENCE_LEVELS[i];
		}
	}

	// return the maximum possible unsigned int value
	return 0x7FFFFFFF;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// returns the amount of experience that is required to reach the next level,
// calculated from the current character level
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::getNextLevelExpByLevel(int level)
{
	if (level < LEVEL_MAX)
	{
		return EXPERIENCE_LEVELS[level + 1];
	}

	// return the maximum possible unsigned int value
	return 0x7FFFFFFF;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::getProgressType(int level)
{
	if (level < HIGHLEVEL_BORDER)
	{
		return 0;
	}
	if (level < NOATTR_BORDER)
	{
		return 1;
	}
	return 2;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::getProgressPosition(int level, int experience)
{
	// get the difference between the current level and next level
	int leveldiff = EXPERIENCE_LEVELS[level+1] - EXPERIENCE_LEVELS[level];
	int expdiff = experience - EXPERIENCE_LEVELS[level];

	if (leveldiff <= 0)
	{
		return 0;
	}

	float ratio = (float)expdiff / (float)leveldiff;
	if (ratio < 0)
	{
		return 0;
	}
	if (ratio >= 1.0f)
	{
		ratio = 0.999f;
	}

	if (level < HIGHLEVEL_BORDER)
	{
		// these levels have 50 steps (2% each)
		return (int)(ratio * 50.0f);
	}
	if (level < NOATTR_BORDER)
	{
		// these levels have 16 steps (6.25% each)
		return (int)(ratio * 16.0f);
	}
	return (int)(ratio * 100.0f);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// returns the amount of experience that is required to gain another attribute
// point
// the amount of experience required is calculated by comparing the experience
// differences between the current and the next level, dividing that value by
// ATTRIBUTES_PER_LEVEL, shifting it half a step (so the attribute points never
// are awarded at an exact level up) and returning the new value plus the base
// level value
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::getNextAttribute(int level, int experience)
{
	// check if we've passed the border for more attributes
	if (level >= NOATTR_BORDER)
	{
		// return the maximum possible unsigned int value
		return UNLIMITED_EXP;
	}

	// get experience points for this level
	int low = EXPERIENCE_LEVELS[level];
	// and for the next level
	int high = EXPERIENCE_LEVELS[level + 1];

	// calculate the difference and divide it by the number of attributes you
	// get each level (to get a "step length")
	int step = (high - low);
	if (level >= HIGHLEVEL_BORDER)
	{
		step /= ATTRIBUTES_PER_HIGHLEVEL;
	}
	else
	{
		 step /= ATTRIBUTES_PER_LEVEL;
	}

	// calculate the first attribute point at this character level
	int attribute((step / 2) + low);
	
	while (experience >= attribute)
	{
		// increase attribute by one step
		attribute += step;

		// check if we've passed a level line
		if (attribute >= high)
		{
			// the character will not receive an attribute until next level,
			// so calculate from another level height
			return getNextAttribute(level + 1, experience);
		}
	}

	return attribute;
}
			
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// calculates how much experience that should be added by comparing the source
// level to the destination level (source = the monster that was killed,
// destination = the player who killed the monster). The amount of experience
// decreases when the difference is increases, but is never less than 1.
//
// Set doubledelta to true if you want to use the increased delta rules (currently occurs
// when a party kills a monster beyond their own level).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CExperienceMath::addExperience(int srcamount, int srclevel, int destlevel, int deltashift)
{
	const int MAX_DELTA = 20;
	const float EXPDROP_UPWARDS[MAX_DELTA + 1] =
		{
			// no difference
			0.0f,
			// 1 to 10 levels difference
			0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f,
			// 11 to 20 levels difference
			1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.0f
		};
	const float EXPDROP_DOWNWARDS[MAX_DELTA + 1] =
		{
			// no difference
			0.0f,
			// 1 to 10 levels difference
			0.02f, 0.04f, 0.06f, 0.08f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f,
			// 11 to 20 levels difference
			0.41f, 0.52f, 0.63f, 0.74f, 0.85f, 1.0f, 1.20f, 1.45f, 1.70f, 2.0f
		};

	if (destlevel >= LEVEL_MAX)
	{
		return 0;
	}

	// get a level difference
	int deltalevel = destlevel - srclevel;
	if (deltalevel < 0)
	{
		deltalevel = -deltalevel;
	}

	// If the monster is level 15 or below, and the monster is equal to or higher than the
	// the character's level, don't drop
	if ((srclevel <= 15) && (destlevel <= srclevel))
	{
		return srcamount;
	}

	// if the character has reached level 100, none of the monsters above 100 yields any dropoff
	if (srclevel >= 100 && destlevel >= 100)
	{
		return srcamount;
	}

	// if the monster is below the destination level, use the normal delta rules
	float ratio = 0.0f;
	if (srclevel <= destlevel)
	{
		//if (deltalevel > MAX_DELTA)
		//{
		//	return srclevel;
		//}
		//ratio = EXPDROP_DOWNWARDS[deltalevel];

		// 030512: each level drops 2% now
		ratio = (float)deltalevel * 0.02f;
	}
	else
	{
		// if the character is in a team with double-delta rules, decrease the delta level with up
		// to 10 if the source level is higher than the destination level
		if (deltashift)
		{
			deltalevel -= deltashift;
			if (deltalevel < 0)
			{
				deltalevel = 0;
			}
		}
		if (deltalevel > MAX_DELTA)
		{
			return srclevel;
		}
		ratio = EXPDROP_UPWARDS[deltalevel];
	}

	// if the character has reached the upper boundary we decrease the drop
	if ((destlevel >= LEVEL_INCREASEDDELTA) && (srclevel > destlevel))
	{
		ratio *= 0.5f;
	}

	/*

	// if the character is in a team with double-delta rules, decrease the delta level with up
	// to 10 if the source level is higher than the destination level
	if (deltashift && (destlevel < srclevel))
	{
		deltalevel -= deltashift;
		if (deltalevel < 0)
		{
			deltalevel = 0;
		}
	}
	// if the character is above the monster's level we shift the drop off by 10 levels (to
	// allow soloing versus smaller monsters)
	else if (destlevel > srclevel)
	{
		deltalevel -= 10;
		if (deltalevel < 0)
		{
			deltalevel = 0;
		}
	}
	
	if (deltalevel == 0)
	{
		// there were no level difference, so return the exact amount
		return srcamount;
	}

	float newamount((float)srcamount);
	int maxdelta(10);
	float deltaratio(0.10f);

	if (destlevel >= LEVEL_INCREASEDDELTA)
	{
		maxdelta = 20;
		deltaratio = 0.05f;
	}
	if (deltalevel >= maxdelta)
	{
		// the level difference was too great so return the minimum amount
		return srclevel;
	}
	*/

	float newamount = (float)srcamount - ((float)srcamount * ratio);

	if (newamount < 1)
	{
		// return the minimum amount
		return srclevel;
	}

	// return a rounded amount
	return (int)(newamount + 0.5f);
}

}
