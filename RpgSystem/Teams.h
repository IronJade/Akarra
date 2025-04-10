/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Teams definitions, not very well-developed.
 **/

#ifndef _TEAMS_H_
#define _TEAMS_H_

namespace RpgSystem
{

	// a team is an integer that consists of 3 blocks,
	// 000000FF = main team
	// 0000FF00 = religion team
	// FFFF0000 = group team

	const int MAINTEAM_SHIFT		= 0;
	const int MAINTEAM_MASK			= 0xFF;
	const int RELIGIONTEAM_SHIFT	= 8;
	const int RELIGIONTEAM_MASK		= 0xFF00;
	const int GROUPTEAM_SHIFT		= 16;
	const int GROUPTEAM_MASK		= 0xFFFF0000;

	enum EMainTeams
	{
		TEAM_M_PLAYERS = 1,
		TEAM_M_MONSTERS, 
		TEAM_M_NPCS,

		NUM_MAIN_TEAMS
	};

	enum EReligionTeams
	{
		TEAM_R_GIFTED = 1,
		TEAM_R_SHINING,
		TEAM_R_FAITHLESS,

		NUM_RELIGION_TEAMS
	};

	// group teams are depending on main team and alliance
	enum EGroupTeams
	{
		TEAM_G_NONE = 0,		// no special conditions (default)

		// monster default teams
		
		TEAM_G_PLAYERSUMMONED,	// summoned by a player, uses player rules

		// monster special groups

		TEAM_G_DAERAK = 100,	// daerak darklings are enemies towards arags
		TEAM_G_ARAG,			// arag darklings are enemies towards daerak
	};
}

#endif
