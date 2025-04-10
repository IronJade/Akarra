
#ifndef _CUP_WORLD_H_
#define _CUP_WORLD_H_

namespace Comm
{

	// CUP_W_GAMESTATE --------------------------------------------------------------------------
	enum EWorldGameStates
	{
		WGS_PVP = 0,	// player-vs-player killing
	};
	struct SCUPWGameState
	{
		unsigned char	type;
		unsigned char	reserved;

		unsigned char	state;		// what state that changed
		unsigned char	setting;	// 0 = off, 1 = on
	};

	// CUP_W_DAYTIME ----------------------------------------------------------------------------
	struct SCUPWDayTime
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	time;		// time of day (0-23)
	};

	// CUP_W_AREANAME -----------------------------------------------------------------------------
	const int AREA_NAMELENGTH = 32; // this constant may be located elsewhere too
	struct SCUPWAreaName
	{
		unsigned char	type;
		unsigned char	reserved[3];

		char			name[AREA_NAMELENGTH];
	};

	// CUP_W_VEGETATIONSPRITE -----------------------------------------------------------------------------
	struct SCUPWVegetationSprite
	{
		unsigned char	type;
		unsigned char	padding;

		unsigned char	spriteset;		// from which spriteset
		unsigned char	sprite;			// what sprite in the set

		int				x;				// world-relative coordinate
		int				y;

	};

}

#endif
