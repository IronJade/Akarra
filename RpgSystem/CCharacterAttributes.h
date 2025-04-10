/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This is a wrapper class for all of a player character's attributes.
 **/

#ifndef _CCHARACTERATTRIBUTES_H_
#define _CCHARACTERATTRIBUTES_H_

#include "CAttribute.h"
#include "CAbility.h"
#include "PKInfo.h"
#include "../library/FileSystem/CFileStream.h"
#include <vector>
#include <list>

namespace RpgSystem
{
	class CSkill;
	class CSkillType2;
	class CChrEffect;
	class CCharacterInventory;

	enum ECharacterRaces
	{
		CR_HUMAN = 0,
		CR_MENIT,
		CR_IWID,

		NUM_RACES
	};

	/* **********************************************************************************
	* STRUCTURE SBaseStats
	*
	* Purpose:	I made this structure to group basic information that should be easily
	*			sent to other players. In short, this is a struct that makes it easier
	*			to do a sizeof().
	*
	* ***********************************************************************************
	* (011029): First version.
	* **********************************************************************************/
	struct SBaseStats
	{
		CAttribute		attributes[NUM_ATTRIBUTES];		// the base attributes
		CVarAttribute	variable[NUM_VARATTRIBUTES];	// health and mana
		CAbility		abilities[NUM_ABILITIES];		// the base abilities

		float			hunger;		// the current food value (100 is max)

		unsigned short	pclass;		// player class
		unsigned short	level;		// character level
		unsigned int	experience;	// experience points
		unsigned char	gender;		// 0 for male, 1 for female
		unsigned char	soulcolor;	// the player's soul color (white, red, green, blue or yellow)
		unsigned char	god;		// 0 for the gifted, 1 for the shining
		unsigned char	race;		// this is currently always 0 (human), but can be changed to menit/iwid at some time

		unsigned short	newpoints;		// new attribute points the character has to spend
		unsigned short	totalpoints;	// total number of attribute points the character has received

		unsigned char	freegems[5];	// unassigned stones
		unsigned char	totalgems;		// total number of character proficiensy stones

		// stats that aren't stored to file:

		int				attackspeed;	// this value is added to the speed of the weapon (which means it is 0 for the most of the time)
		float			movespeedmultiplier;
		char			publiclight;
		char			personallight;

		SBaseStats();
		void clear();

		bool read(FileSystem::CFileStream& file, int version);
		bool write(FileSystem::CFileStream& file, int version);

	};

	// this class is used for network communication and ignores stuff that the
	// client doesn't need to know about
	class CSmallStats
	{
		public:

			void setStats(SBaseStats& stats, CCharacterInventory* inventory);

			float	ua_attributes[NUM_ATTRIBUTES];
			float	ua_varcurr[NUM_VARATTRIBUTES];
			float	ua_varmax[NUM_VARATTRIBUTES];
			float	ua_abilities[NUM_ABILITIES];

			// this is an exact copy of SBaseStats:

			float			u_hunger;		// the current food value (100 is max)

			unsigned short	u_pclass;		// player class
			unsigned short	u_level;		// character level
			//unsigned int	u_experience;	// experience points
			unsigned char	u_gender;		// 0 for male, 1 for female
			unsigned char	u_soulcolor;	// the player's soul color (white, red, green, blue or yellow)
			unsigned char	u_god;		// 0 for the gifted, 1 for the shining
			unsigned char	u_race;		// this is currently always 0 (human), but can be changed to menit/iwid at some time

			unsigned short	u_newpoints;		// new attribute points the character has to spend
			unsigned short	u_totalpoints;	// total number of attribute points the character has received

			unsigned char	ua_freegems[5];	// unassigned stones
			unsigned char	u_totalgems;		// total number of character proficiensy stones

			//unsigned char	buffer[2];	// not used
			unsigned char	u_expprogresstype;
			unsigned char	u_expprogress;

			unsigned short	u_itemdisabled;	// bitfield (NUM_EQUIPMENT_SLOTS) that says if an item is disabled
	};

	/* **********************************************************************************
	* CLASS CCharacterAttributes
	* Derived:	(base class)
	*
	* Purpose:	To keep information about a character's attributes and basic abilities.
	*			All characters in the world should have one of these structures. It is
	*			intended to keep track of all the character's stats.
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011022): First version.
	* **********************************************************************************/
	class CCharacterAttributes
	{
		public:

			CCharacterAttributes();
			~CCharacterAttributes();

			void removeAll();

			bool read(FileSystem::CFileStream& file, int version = 0);
			bool write(FileSystem::CFileStream& file, int version = 0);

			bool hasSkill(int skillnumber, int* saveindex = 0, bool mustbelocked = false);
			bool addSkill(CSkillType2* skill, int startrow);

			// player-killing...

			void addVictim(char* charactername);
			bool clearVictim(char* charactername);
			bool hasVictim(char* charactername);
			bool isPKer();

		public:

			// basic attributes and general information:

			SBaseStats		u_stats;

			// skills:

			//int				u_numskills;	// the number of skills this character has
			//unsigned short*	up_skills;		// a list of skills the character has
			//CSkill*			up_skilldata;	// a list of information about each skill

			std::vector<CSkill*>	u_skills;
			std::list<CChrEffect*>	u_runeffects;	// these are effects that needs to "run"
			//std::list<???*>		u_timeeffects;	// these are timed effects that only have on/off settings

			// player-killer list
			TDVictimNameList		u_victims;

		private:

			void m_clear();

		protected:

	};

} // end of RpgSystem namespace


#endif
