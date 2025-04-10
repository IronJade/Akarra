/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player fellowship rules.
 **/

#ifndef _CFELLOWSHIP_H_
#define _CFELLOWSHIP_H_

namespace RpgSystem
{

	const int	FELLOWSHIP_COUNT		= 10;	// maximum 10 players in the same fellowship
	const float	FELLOWSHIP_STARTRATIO	= 0.75f;
	const float	FELLOWSHIP_PLAYERADD	= 0.25f;

	const int	FELLOWSHIP_NAMELENGTH	= 16;	// 15 available characters (fellowship names)
	const int	FELLOWSHIP_CIRCLELENGTH	= 32;	// 31 available characters (for circle effects)
	
	enum EFellowshipCircles
	{
		FC_NONE = 0,

		FC_THEVOID,				// "The Void", adds 10% experience
		FC_PERFECTCIRCLE,		// "Perfect Circle", makes characters always get the most wanted prof gem and
								// increases all resistances

		FC_DIVINEPOWERS,		// increases attack and cast rating
		FC_BALANCEDELEMENTS,	// increases green/red/blue resistances
		FC_GREATSTRUGGLE,		// increases damage reduction
		FC_BURNINGSTORM,		// increases white/yellow resistances and min damage
		FC_TURNSOFFATE,			// doubles critical hit chance

		// these are all lesser circles that causes the characters to cast spells when they die (not implemented yet)

		FC_PUREFLAME,			// red spell
		FC_GREATESTILLUSION,	// white spell
		FC_SOULCREMATION,		// yellow spell
		FC_STREAMOFLIFE,		// green (healing) spell
		FC_FROSTDEATH,			// blue spell

		NUM_FELLOWSHIP_CIRCLES
	};

	class CFellowship
	{
		public:

			static int getCircle(bool white, bool red, bool green, bool blue, bool yellow);
			static char* getCircleName(int type);
	};

}

#endif

