/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The player's character file.
 **/

#ifndef _AKARRACHARACTER_H_
#define _AKARRACHARACTER_H_

#include "../RpgSystem/CCharacterAttributes.h"
#include "../RpgSystem/CCharacterInventory.h"
#include "../RpgSystem/CCharacterQuests.h"
#include "../RpgSystem/CGuild.h"
#include "../WorldSystem/CArea.h"
#include "../library/Basic/CPoint.h"

namespace RpgSystem
{
	class IAttributeObject;
}

namespace Player
{

	const int CHARACTER_NAMELENGTH			= 16;	// length of a character name

	const char * const CHARACTER_FOLDER		= "characters/";
	const char * const CHARACTER_EXT		= ".achr";
	const char * const CHARACTER_FOLDER2	= "characters";	// for FileSystem::countFiles() and similar
	const char * const CHARACTER_SEARCH		= "*.achr";		// for FileSystem::countFiles() and similar
	const char * const CHARACTER_DELEXT		= ".~chr";		// file extension for deleted characters

	enum ECharacterFileVersions
	{
		CHRFV_ORIGINAL = 0,

		CHRFV_020505,	// added stamina and hunger to SBaseStats and a flags integer to the character file
		CHRFV_020530,	// added damina reduction and level 50-time
		CHRFV_020611,	// added first version of CCharacterInventory
		CHRFV_020622,	// added items to the CCharacterInventory
		CHRFV_020629,	// added respawn point position
		CHRFV_020810,	// added stackable items
		CHRFV_020818,	// added new skill system
		CHRFV_020908,	// converted coin system and decreased max inventory capacity to 30 items
		CHRFV_020913,	// added CharacterQuests
		CHRFV_021002,	// added new character graphics with poses and hair styles
		CHRFV_021003,	// added eye colors too
		CHRFV_021009,	// added account name to character file
		CHRFV_021022,	// added "runeffects" list (CChrEffect)
		CHRFV_021102,	// added a slot for mounts in the character's inventory
		CHRFV_021208,	// added guild info
		CHRFV_030102,	// added mount coordinates in the item instances
		CHRFV_030118,	// added u_muteseconds
		CHRFV_030204,	// changed the skillsystem
		CHRFV_030219,	// added custom names to item instances
		CHRFV_030301,	// added player-killer victim list and id numbers for effects
		CHRFV_030405,	// added item storage to inventory
		CHRFV_030506,	// added last guild message time and all characters prior to this version will have their APs reset
		CHRFV_030508,	// skills now have 10 gems per row instead of 5
			
		// add new versions here

		CHARACTER_FILEVERSION
	};

	const int CHARACTER_START_X			= 1056;
	const int CHARACTER_START_Y			= 7332;

	// Character Flags
	const int CHRFLAG_RETROTIMEBONUS	= 0x1;	// is set if the retro-active time-bonus has been added
	const int CHRFLAG_STARTITEMS		= 0x2;	// is set when the character has received his starting items
	const int CHRFLAG_APPEARANCESET		= 0x4;	// is set when the character has legally set his appearance
	const int CHRFLAG_REBIRTHUSED		= 0x8;	// set when the player has used the "rebirth" feature
	const int CHRFLAG_TORCHESADDED		= 0x10;	// added two free torches (non-dropable) for all old pA13 characters

	// Player Information flags (uses 0x00FF0000 of the bits)
	const int CHRFLAG_UNLOCKLEVEL		= 0x10000;	// makes it possible for people to see the char's level
	const int CHRFLAG_UNLOCKEMAIL		= 0x20000;	// -"- to see the char's email
	const int CHRFLAG_UNLOCKPOSITION	= 0x40000;	// -"- to see the char's world position
	const int CHRFLAG_NOSTATISTICS		= 0x80000;	// this flag removes the character from the statistics page

	// this class holds most information about a character
	// (at least the information that needs to be stored to file)
	class CCharacterFile
	{
		public:

			CCharacterFile();
			~CCharacterFile();

			void removeAll();

			bool open(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			void setNewCharacter();
			void resetCharacterAttributes();
			void recalculateCharacterStats(RpgSystem::IAttributeObject* source);
			void applyEffect(RpgSystem::CEffectType* effect, RpgSystem::IAttributeObject* source, float castrating = 0);

		private:

			void m_clear();

		public:

			char			u_name[CHARACTER_NAMELENGTH];	// name of the character
			char			ua_accountname[16];				// 021009: In order to track characters back to their owners I've added the accountname here too
			RpgSystem::CCharacterAttributes	u_attribs;		// the character stats and skills
			RpgSystem::CCharacterInventory	u_inventory;	// items and money
			RpgSystem::CCharacterQuests		u_quests;		// quests

			int		u_globalx;	// character world position
			int		u_globaly;
			int		u_steps;	// number of steps the character has taken (reward every 100th)
			int		u_onlinetime;	// number of seconds the character has been used online
			int		u_level50time;	// online time when the character reached level 15 (seconds)

			int		u_flags;		// various character flags
			int		u_muteseconds;	// if the character has been globally muted by an admin

			// respawn point:
			int		u_respawnx;
			int		u_respawny;

			// appearance info:
			int		u_bodypose;		// three different poses for each gender
			int		u_skincolor;	// four different skin colors (ESkinColors)
			int		u_eyecolor;		// four different skin colors (EEyeColors)
			int		u_haircolor;	// eight different hair colors (EHairColors)
			int		u_hairstyle;	// ??? different hair styles for each gender

			// guild info:
			RpgSystem::SCharacterGuildState	u_guild;
			long	u_lastguildmessageread;

			// other info
			int		u_nexteffectid;	// next "runable" effect id number
	};

	struct CMinimumCharacterInfo
	{
			char			name[CHARACTER_NAMELENGTH];	// name of the character
			//char			guildtag[4];
			//char			guildcolor;
			//char			padding[3];

			RpgSystem::CSmallStats		stats;			// the character stats
			RpgSystem::CSmallInventory	inventory;		// character inventory (only id numbers and sprites)

			int				globalx;					// character world position
			int				globaly;

			// some experience settings
			//int				nextlevel;
			//int				nextattribute;
			unsigned char	expprogresstype;
			unsigned char	expprogress;

			// appearance
			unsigned char	gfxlibrary;		// 0 = male, 1 = female, other races to follow
			unsigned char	skincolor;		// see below
			unsigned char	hairstyle;		// low four bits sets style, high bits set hair color
			unsigned char	mount;			// if he rides anything

			char			publiclight;
			char			personallight;
			//unsigned char	padding[2];

			unsigned int	bodyappearance;		// armor / body
			unsigned int	weaponappearance;	// right hand
			unsigned int	shieldappearance;	// left hand
			unsigned int	helmappearance;		// ontop of hair
			unsigned int	handsappearance;	// hands color
			unsigned int	feetappearance;		// feet color
	};

	// Character Actions (max 255):
	enum ECharacterActions
	{
		CA_IDLE = 0,		// no action

		// world interaction actions
		CA_STEPPING,		// taking a single step
		CA_MOVING,			// continously moving (making several steps in a row)
		CA_LOOKING,			// examining an object
		CA_TALKING,			// talking or trading with another character/npc

		CA_FOLLOW,
		CA_MOVETO,

		// combat actions:
		CA_ATTACKING,		// the character is in combat with an enemy
		CA_STRIKING,		// ... is currently making a melee attack attempt (part of CA_ATTACKING)
		CA_SHOOTING,		// ... is currently firing a arrow/bolt/bullet (part of CA_ATTACKING)
		CA_CASTING,			// ... is currently casting a spell (part of CA_IDLE)

		CA_REST,

		// item actions
		CA_DROPITEM,
		CA_MOVEITEM,
		CA_USEITEM,
		CA_LOOKITEM,

		// skill actions
		CA_ADDSKILLGEM,
		CA_REMOVESKILLGEM,
		CA_SPECIALIZESKILL,

		CA_LOOKNPCITEM,

		CA_CASTTARGET,	// cast spell on target
		CA_CASTGROUND,	// cast spell on ground

		NUM_CHARACTERACTIONS
	};

	// Character Movement Directions
	// I chose the order so it would match a sine-circle
	enum ECharacterDirections
	{
		CDIR_STOP = 0,	// dont move

		CDIR_RIGHT,
		CDIR_UPRIGTH,
		CDIR_UP,
		CDIR_UPLEFT,
		CDIR_LEFT,
		CDIR_DOWNLEFT,
		CDIR_DOWN,
		CDIR_DOWNRIGHT
	};
	const int DIRECTION_X[9] = { 0,  1,  1,  0, -1, -1, -1,  0,  1 };
	const int DIRECTION_Y[9] = { 0,  0, -1, -1, -1,  0,  1,  1,  1 };

	// character action constants!
	// moved these to Character/Travel.cpp
	//const int	CHR_MOVETIME_ORTH	= 800;
	//const int	CHR_MOVETIME_DIAG	= 1131;	// diagonal movements takes (sqrt(2)) 1.4 times longer

	// commands...
	const char	CHR_COMMAND1		= '/';
	const char	CHR_COMMAND2		= '.';

	// appearance stuff:

	enum ESkinColors
	{
		SC_PALE = 0,
		SC_SUNBURNED,
		SC_CHESTNUT,
		SC_DARK,

		NUM_SKIN_COLORS
	};

	enum EEyeColors
	{
		EC_BLUE = 0,
		EC_GREEN,
		EC_CYAN,
		EC_BROWN,
		EC_BLACK,
		EC_WHITE,

		NUM_EYE_COLORS
	};

	enum EHairColors
	{
		HC_WHITE = 0,
		HC_GREY,
		HC_BROWN,
		HC_YELLOW,
		HC_LIGHTBROWN,
		HC_BLACK,
		HC_RED,
		HC_ORANGE,

		NUM_HAIR_COLORS
	};

	// there are 3 bits for hair styles (8 different styles)

}

#endif
