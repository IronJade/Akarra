
#ifndef _SCHARACTERUPDATEPACKETS_H_
#define _SCHARACTERUPDATEPACKETS_H_

#include "../RpgSystem/CFellowship.h"
#include "../RpgSystem/CCharacterInventory.h"

namespace Comm
{
	// Each character can receive up to 1 update packet each frame. The packet itself
	// can then include several different pieces of information, chosen from the list
	// below.
	// There are never allowed to be more than 255 different packet types, since their
	// headers only has a singe-char identifier.
	
	enum ECharacterUpdatePackets
	{
		CUP_NONE = 0,

		// Entity handling

			CUP_E_ADDITEM,		// Adds entities to the character's perimiter.
							// The entity can be of any type (another character or a
							// monster, item, effect etc) and will have an "unique
							// identifier" (int).
			//CUP_E_INACTIVATE,	// "Inactivates" an entity (cease to update it but do not remove it)
			CUP_E_REMOVE,	// Removes an entity completely.
			CUP_E_MOVE,		// An entity has changed position within the perimiter.
			CUP_E_HEALTH,	// updates the entity's current health, stamina and mana
			CUP_E_STATE,	// reports a state change (such as death)
			CUP_E_APPEARANCE,	// the entity has changed appearance
			CUP_E_ADDPLAYER,
			CUP_E_PLAYERNAME,	// updates a player's name (both on the entity and in the people box)
			CUP_E_PLAYERLEVEL,	// updates the player's level (for the people box)
			CUP_E_MONSTERHEALTH,	// sends information about a monster's health (only a percentage of total)
			CUP_E_ADDMONSTER,	// changed the monster packet to make it a lot smaller
			CUP_E_MONSTERTYPE,	// here's the monstertype info now :)
			CUP_E_ADDSPELLENTITY,	// a "living" spell entity, such as a flare marker

		// Character handling and prediction

			CUP_A_ACTIONCANCELLED = 20,	// The character's current action has been
							// cancelled (includes action type).
			CUP_A_ACTIONSTARTED,	// an action has started (such an attack), used to update the player
			CUP_A_MOVE,		// the entity has started walking somewhere (doesn't actually move anything,
							// as CUP_E_MOVE does)

		// System messages

			CUP_SYS_REQUESTCRC = 25,	// sends a file CRC request to the client (hack detection)

		// Character messages

			CUP_M_CHATMESSAGE_ = 30, // <--- OBSOLETE! (see 130 and below)
			CUP_M_GAINEXP,
			CUP_M_GAINATTRIBUTE,
			CUP_M_GAINLEVEL,
			CUP_M_GAINGEM,

			CUP_M_GAINCOINS,
			CUP_M_DROPCOINS,

			CUP_M_STATSCHANGE,		// an attribute or ability has changed
			CUP_M_VARSTATSCHANGE,	// a variable attribute has changed (hp, mp, sp)
			CUP_M_NEWINVLIMITS,		// maximum backpack slots and/or money limits has changed

		// Combat messages

			CUP_C_ATTACK = 40,	// this message is sent when an entity attacks another entity

		// Fellowship and "people" messages

			CUP_F_UPDATEFELLOWSHIP = 50,
			CUP_F_DISABLEFELLOWSHIP,		// this message is sent when the player leaves the fellowship
			CUP_F_UPDATEHEALTH,				// updates the health for a fellowship member

			CUP_F_ADDONLINEPLAYER,
			CUP_F_REMOVEONLINEPLAYER,

		// Item and inventory messages

			CUP_I_FULLITEMINFO = 60,		// is sent when the player is "looking" at an item... sends
											// a complete item description (stats and everything)
			CUP_I_NEWSLOTINFO,				// is sent for each slot in the inventory that may have changed
			CUP_I_OPENTRADE,				// opens a trade dialog box with another character
			CUP_I_CLOSETRADE,				// closes the trade dialog box
			CUP_I_TRADESTATE,				// updates the trade state
			CUP_I_TRADEMONEY,				// updates the amounts of money on each side
			CUP_I_UPDATEMONEY,				// refreshes the money for the character

			CUP_I_STARTNPCTRADE,			// creates a npc trade dialog, items are added later (item location number 6)
			CUP_I_REMOVENPCITEM,			// removes an item from the NPCs trade dialog

			CUP_I_DISCARDITEM,				// asks the player if he really wants to discard his non-dropable item

			CUP_I_OPENCRAFT,				// initiates crafting
			CUP_I_NEWCRAFTITEM,				// updates the craft recepy result

		// Events

			CUP_V_PLAYSOUND = 75,			// plays a sound effect
			CUP_V_VFXPROJECTILE,			// visual effect: projectile
			CUP_V_VFXEXPLOSION,				// visual effect: explosion (used both as starting and ending effects)

		// World info

			CUP_W_GAMESTATE	= 80,			// a game state has changed for this player (such as PK'ing)
			CUP_W_AREANAME,					// sends an areaname to the character
			CUP_W_DAYTIME,					// the time of the day has changed (night/day cycle)

			CUP_W_VEGETATIONSPRITE,			// a vegetation sprite needs to be added on the client-side

		// Skills

			CUP_S_SKILLINFO = 90,			// sends information about a skill that the character knows
			CUP_S_AVAILGEMS,				// sends the number of free gems the character has
			CUP_S_NEWSKILLDATA,				// sends a block with all skill information

		// NPC Chatting

			CUP_NPC_CHATBOX = 100,			// a chat box with some text and up to 6 choices
			CUP_NPC_CLOSE,					// closes the current chat box
			CUP_NPC_TRADE,					// starts trading with the npc

		// Quest packets

			CUP_Q_QUESTINFO = 110,			// full quest info (description and objectives)
			CUP_Q_QUESTINFODONE,			// quest info for finished quests (doesn't include objectives)
			CUP_Q_OBJECTIVECHANGE,			// sends info when a quest objective state has changed
			CUP_Q_OBJECTIVEUPDATE,			// a full objective change (can change description too)

		// Effect packets

			CUP_X_EFFECTVARCHANGE = 120,	// sent when an entity is harmed or healed by an effect
			CUP_X_EFFECTVARCHANGE2,			// same as above, but without specific details
			CUP_X_EFFECTACTION,				// an effect has been added/nullified/resisted etc
			CUP_X_SPELLUSE,					// this packet is sent whenever an entity uses a spell (it replaces a text message, nothing else)

		// Chat messages

			CUP_M_CHATMESSAGE_TINY = 130,
			CUP_M_CHATMESSAGE_SMALL,
			CUP_M_CHATMESSAGE_NORMAL,
			CUP_M_CHATMESSAGE_BIG,
			CUP_M_CHATMESSAGE_HUGE,

	};


	// Entity handling

	// CUP_E_ADDITEM -----------------------------------------------------
	const int	ENTITY_NAME_LENGTH	= 32;
	const int	PLAYER_NAME_LENGTH	= 16;
	struct SCUPEAddItem
	{
		unsigned char	type;		// header

		unsigned char	entitytype;
		unsigned char	entityflags;	// see flags below
		unsigned char	lightradius;	// lightradius for monsters
		unsigned int	appearance1;	// main sprite

		char			name[ENTITY_NAME_LENGTH];

		int				x;				// global position
		int				y;

		int				id;
	};
	// CUP_E_ADDMONSTER -----------------------------------------------------------
	struct SCUPEAddMonster
	{
		unsigned char	type;		// header

		unsigned char	entitytype;
		unsigned char	entityflags;	// see flags below
		unsigned char	lightradius;	// lightradius for monsters

		unsigned int	monsterid;		// id number of the monster type (so the client knows what to request)

		int				x;				// global position
		int				y;

		int				id;
	};

	// CUP_E_ADDSPELLENTITY ---------------------------------------------------------
	enum ESpellEntityTypes
	{
		SET_LIGHT	= 0,	// invisible light marker

		NUM_SPELLENTITY_TYPES
	};
	struct SCUPEAddSpellEntity
	{
		unsigned char	type;		// header

		unsigned char	spelltype;
		unsigned char	spellflags;
		unsigned char	radius;

		// TODO: Add sprite information and similar stuff here

		int				x;				// global position
		int				y;

		int				id;			// identification number
	};
	
	// CUP_E_ADDPLAYER ---------------------------------------------------------
	struct SCUPEAddPlayer
	{
		unsigned char	type;		// header

		unsigned char	entitytype;
		unsigned char	entityflags;	// see flags below

		unsigned char	guildcolor;
		char			guildtag[4];	// note, the tag is actually 5 chars, but we don't send the null terminator

		// appearance
		unsigned char	gfxlibrary;		// 0 = male, 1 = female, other races to follow
		unsigned char	skincolor;		// see below
		unsigned char	hairstyle;		// low four bits sets style, high bits set hair color
		unsigned char	mount;			// if he rides anything

		char			publiclight;	// light radius that affects everybody
		char			personallight;	// light radius that only the local player can see (nightsight)
		unsigned char	padding[2];

		unsigned int	bodyappearance;		// armor / body
		unsigned int	weaponappearance;	// right hand
		unsigned int	shieldappearance;	// left hand
		unsigned int	helmappearance;		// ontop of hair
		unsigned int	handsappearance;	// hands color
		unsigned int	feetappearance;		// feet color

		char			name[PLAYER_NAME_LENGTH];

		int				x;				// global position
		int				y;

		int				id;
	};
	const unsigned char	ADDFLAG_DEAD	= 0x1;	// the entity is already dead
	const unsigned char ADDFLAG_RESTING	= 0x2;	// the entity is currently resting
	const unsigned char ADDFLAG_GIFTED	= 0x4;	// the entity is gifted
	const unsigned char ADDFLAG_SHINING	= 0x8;	// the entity is shining
	const unsigned char	ADDFLAG_NPC		= 0x10;	// this is a NPC, which cannot be killed (but talked to)
	const unsigned char ADDFLAG_AFK		= 0x20;	// used for players when they are AFK
	const unsigned char ADDFLAG_DNESS	= 0x20;	// used for monsters if they cause darkness instead of brightness
	const unsigned char ADDFLAG_PK		= 0x40;	// entity is a player-killer
	const unsigned char ADDFLAG_WAR		= 0x80;	// entity is a member of a hostile clan

	// CUP_E_MONSTERTYPE ---------------------------------------------------
	struct SCUPEMonsterTypeInfo
	{
		unsigned char	type;
		unsigned char	padding[3];

		int				id;			// monstertype id number
		unsigned int	appearance;
		unsigned short	movesprite;
		unsigned short	attacksprite;
		char			name[32];
	};

	// CUP_E_INACTIVATE -----------------------------------------------------
	/*
	struct SCUPEInactivate
	{
		unsigned char	type;
		unsigned char	reserved[3];

		int				id;
	};
	*/

	// CUP_E_REMOVE -----------------------------------------------------
	struct SCUPERemove
	{
		unsigned char	type;
		unsigned char	reserved[3];

		int				id;
	};

	// CUP_E_MOVE -----------------------------------------------------
	struct SCUPEMove
	{
		unsigned char	type;			// header
		unsigned char	reserved[3];

		int				id;				// entity that is moving
		int				x;				// new position
		int				y;
	};

	// CUP_E_HEALTH -----------------------------------------------------
	struct SCUPEHealth
	{
		unsigned char	type;
		unsigned char	reserved[3];

		int				id;
		float			currents[3];	// current values (health, mana, stamina)
	};

	// CUP_E_STATE -----------------------------------------------------
	enum EEntityStateChanges
	{
		ESC_NORMAL = 0,		// return the entity to normal state

		ESC_INACTIVATE,		// "Inactivates" an entity (cease to update it but do not remove it)
		ESC_DEAD,			// the entity has died!
		ESC_ALIVE,
		ESC_RESTING,		// has started resting
		ESC_NOTRESTING,		// has stopped resting
		ESC_AFK,			// player is afk
		ESC_NOTAFK,			// player is no longer afk
		ESC_PK,				// player-killer
		ESC_NOTPK,
		ESC_WAR,			// this player is in a guild which is at war with us
		ESC_NOTWAR,

	};
	struct SCUPEStateChange
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	state;
		int				id;
	};

	// CUP_E_APPEARANCE -----------------------------------------------------
	struct SCUPEAppearance
	{
		unsigned char	type;
		unsigned char	reserved[3];

		int				id;

		// appearance
		unsigned char	gfxlibrary;		// 0 = male, 1 = female, other races to follow
		unsigned char	skincolor;		// see below
		unsigned char	hairstyle;		// low four bits sets style, high bits set hair color
		unsigned char	mount;			// if he rides anything

		unsigned int	bodyappearance;		// armor / body
		unsigned int	weaponappearance;	// right hand
		unsigned int	shieldappearance;	// left hand
		unsigned int	helmappearance;		// ontop of hair
		unsigned int	handsappearance;	// hands color
		unsigned int	feetappearance;		// feet color

		char			publiclight;	// light radius that affects everybody
		char			personallight;	// light radius that only the local player can see (nightsight)
		unsigned char	padding[2];
	};

	// CUP_A_ACTIONCANCELLED -----------------------------------------------------
	struct SCUPAActionCancelled
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	action;		// the action type that was cancelled
	};

	// CUP_A_MOVE -----------------------------------------------------
	const unsigned char MOVEDIRECTION_MASK		= 0x0F;
	const unsigned char MOVEDIRECTION_DOUBLE	= 0x10;	// if this flag is set the entity is taking double-steps
	struct SCUPAMove
	{
		unsigned char	type;

		unsigned char	direction;	// out of ECharacterDirections
		short			traveltime;	// how many milliseconds to travel the distance of one square

		int				id;			// the entity that is moving around
	};

	// CUP_A_ACTIONSTARTED -----------------------------------------------------
	struct SCUPAActionStarted
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	actiontype; // out of Player::ECharacterActions

		short			actiondelay;
		short			recoverydelay;
	};

	// CUP_SYS_REQUESTCRC ---------------------------------------------------
	struct SCUPSYSRequestCRC
	{
		unsigned char	type;
		unsigned char	crcfile;	// file id to crc-check
		unsigned char	padding[2];
	};

	// CUP_M_GAINEXP -----------------------------------------------------
	/*
	struct SCUPMGainExp
	{
		unsigned char	type;
		unsigned char	reserved[3];

		int				amount;
		int				dropoff;	// amount lost due to drop-off
		int				current;	// current experience
	};
	*/
	struct SCUPMGainExp
	{
		unsigned char	type;
		//unsigned char	reserved[3];
		unsigned char	padding;

		unsigned char	progresstype;	// 0 for levels 0-100, 1 for 100-125 and 2 for 125+
		unsigned char	progress;		// current progress

		int				amount;			// amount gained
		//int				dropoff;	// amount lost due to drop-off
		//int				current;	// current experience
	};

	// CUP_M_GAINATTRIBUTE -----------------------------------------------------
	struct SCUPMGainAttribute
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	count;		// number of attribute points to spend (caps at 255)
		int				next;		// experience for next attribute
	};

	// CUP_M_GAINLEVEL -----------------------------------------------------
	struct SCUPMGainLevel
	{
		unsigned char	type;
		unsigned char	reserved[2];
		
		unsigned char	current;	// current character level
		int				next;		// experience for next level		
	};

	// CUP_M_GAINGEM -----------------------------------------------------
	struct SCUPMGainGem
	{
		unsigned char	type;
		
		unsigned char	totalgems;	// total number of gems
		unsigned char	gems[5];	// current free gems
		unsigned char	gem;		// what kind of gem that was added
	};

	// CUP_M_GAINCOINS -------------------------------------------------------
	struct SCUPMGainCoins
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	cointype;	// out of RpgSystem::EMoneyTypes
		int				amount;		// amount gained
		int				current;	// total accumulated

	};

	// CUP_M_DROPCOINS -------------------------------------------------------
	struct SCUPMDropCoins
	{
		unsigned char	type;
		unsigned char	padding;

		unsigned char	dropped;	// 1 if the coins were dropped, 0 if they were lost
		unsigned char	cointype;	// out of RpgSystem::EMoneyTypes
		int				amount;		// amount gained
		int				current;	// total accumulated

	};

	// CUP_M_STATSCHANGE -----------------------------------------------------------
	struct SCUPMStatsChange
	{
		unsigned char	type;
		unsigned char	reserved;

		unsigned char	stattype;	// 0 = ability, 1 = attribute
		unsigned char	stat;		// either out of RpgSystem::EAbilities or RpgSystem::EBaseAttributes

		float			value;		// new value
	};

	// CUP_M_VARSTATSCHANGE -----------------------------------------------------------
	struct SCUPMVarStatsChange
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	stat;		// out of RpgSystem::EVarAttributes
		float			current;	// current value
		float			max;		// max value
	};

	// CUP_M_NEWINVLIMITS -------------------------------------------------------------
	struct SCUPMNewInvLimits
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	backpack;		// number of active backpack slots
		int				maxmoney[3];	// maximum amount of money
	};

	// CUP_C_ATTACK -----------------------------------------------------
	struct SCUPCAttack
	{
		unsigned char	type;

		unsigned char	attacktype;		// out of RpgSystem::EAttackTypes
		unsigned char	attackname;		// out of RpgSystem::EAttackNames
		unsigned char	attackresult;	// out of RpgSystem::EHitTypes

		int				attacker;	// entity gid
		int				target;		// another gid
		float			damage;
	};

	// CUP_F_UPDATEFELLOWSHIP -------------------------------------------------
	enum EFellowshipUpdateActions
	{
		FUA_UNKNOWN = 0,
		FUA_ADDPLAYER,
		FUA_REMOVEPLAYER,

		NUM_FU_ACTIONS
	};
	struct SCUPFUpdateFellowship
	{
		unsigned char	type;

		unsigned char	fua;		// out of EFellowshipUpdateActions
		unsigned char	colors;		// five bits set the color information
		unsigned char	reserved;

		int				actionparameter;	// a parameter for the fua (usually a player gid)

		// player gids for all players in the fellowship
		int				players[RpgSystem::FELLOWSHIP_COUNT];

		// name of the fellowship
		char			name[RpgSystem::FELLOWSHIP_NAMELENGTH];
		// name of the circle effect
		char			circle[RpgSystem::FELLOWSHIP_CIRCLELENGTH];
	};

	// CUP_F_DISABLEFELLOWSHIP --------------------------------------------------
	struct SCUPFDisableFellowship
	{
		unsigned char	type;
		unsigned char	reserved[3];
	};

	// CUP_F_UPDATEHEALTH ---------------------------------------------------------
	struct SCUPFUpdateHealth
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	slot;		// the player slot that is updated
		int				gid;		// player id
		float			current;	// current health
		float			max;		// max health
	};

	// CUP_F_ADDONLINEPLAYER --------------------------------------------------------------
	const unsigned char	AOPFLAG_ADMIN			= 0x01;	// this player is an admin
	const unsigned char	AOPFLAG_TAGCOLORMASK	= 0xF0;	// the upper half of the flags are reserved for the guild color
	struct SCUPFAddOnlinePlayer
	{
		unsigned char	type;

		unsigned char	initial;
		unsigned char	flags;
		unsigned char	playerlevel;

		char			guildtag[4];	// note: the tag is actually 5 chars, but we don't send the null terminator

		char			name[16];
		int				gid;
	};

	// CUP_E_PLAYERNAME --------------------------------------------------------------
	struct SCUPEPlayerName
	{
		unsigned char	type;
		unsigned char	padding[2];

		unsigned char	tagcolor;

		char			guildtag[4];	// note: the tag is actually 5 chars, but we don't send the null terminator

		char			name[16];
		int				gid;
	};

	// CUP_E_PLAYERLEVEL ------------------------------------------------------------------
	struct SCUPEPlayerLevel
	{
		unsigned char	type;
		unsigned char	padding[2];

		unsigned char	level;			// 0xFF for hidden
		int				gid;
	};

	// CUP_E_MONSTERHEALTH -------------------------------------------------------------
	struct SCUPEMonsterHealth
	{
		unsigned char	type;
		unsigned char	padding[2];

		unsigned char	percent;		// 0 to 100, percent of total health
		int				gid;			// monster id number
	};

	// CUP_F_REMOVEONLINEPLAYER --------------------------------------------------------------
	struct SCUPFRemoveOnlinePlayer
	{
		unsigned char	type;
		unsigned char	reserved[3];

		int				gid;
	};

	// CUP_V_PLAYSOUND --------------------------------------------------------------------------
	struct SCUPVPlaySound
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	soundeffect;	// sound effect number
		int				source;			// sound effect source gid (-1 for broadcast)
	};

	// CUP_V_VFXPROJECTILE --------------------------------------------------------------------------
	struct SCUPVVFXProjectile
	{
		unsigned char	type;

		unsigned char	soundeffect;	// sound effect number
		unsigned char	style;			// projectile styles (hardcoded on the client)
		unsigned char	spritestyle;	// what sprite that is used for the style

		// explosion data

		unsigned char	expsoundeffect;
		unsigned char	expstyle;
		unsigned char	expspritestyle;
		unsigned char	expradius;

		// positions

		int				sx;				// start location (cell coordinates)
		int				sy;
		int				ex;				// end location
		int				ey;
	};

	// CUP_V_VFXEXPLOSION --------------------------------------------------------------------------
	struct SCUPVVFXExplosion
	{
		unsigned char	type;

		unsigned char	soundeffect;	// sound effect number
		unsigned char	style;			// explosion styles (hardcoded on the client)
		unsigned char	spritestyle;	// what sprite that is used for the style

		unsigned char	radius;
		unsigned char	padding[3];

		int				x;				// world location (cell coordinates)
		int				y;
	};

	// CUP_X_EFFECTVARCHANGE ---------------------------------------------------------------------
	// This packet is sent to the attacker and target of the spell effect. It includes specific
	// details about what happened.
	struct SCUPXEffectVarChange
	{
		unsigned char	type;

		unsigned char	changetype;		// 0 = damage, 1 = regain
		unsigned char	variable;		// 0 = health, 1 = mana, 2 = stamina

		unsigned char	effectcolor;	// white, red, green, blue, yellow (0-4)
		char			effectname[32];

		float			amount;
		int				target;			// id number of target
	};

	// CUP_X_EFFECTVARCHANGE2 -----------------------------------------------------------------
	// This packet is sent to all on-looking players who are neither the attacker or target.
	struct SCUPXEffectVarChange2
	{
		unsigned char	type;

		unsigned char	changetype;		// 0 = damage, 1 = regain
		unsigned char	variable;		// 0 = health, 1 = mana, 2 = stamina

		unsigned char	effectcolor;	// white, red, green, blue, yellow (0-4)
		int				target;			// id number of target
	};

	// CUP_X_EFFECTACTION ---------------------------------------------------------------------
	enum EEffectActions
	{
		EA_ADDED = 0,		// the effect has been added to the player
		EA_NULLIFIED,		// the effect's time has run out, or it has been dispelled
		EA_PREVENTED,		// the effect has been prevented already before it took effect
	};
	struct SCUPXEffectAction
	{
		unsigned char	type;

		unsigned char	action;			// out of EEffectActions
		unsigned char	effectcolor;	// white, red, green, blue, yellow (0-4)
		unsigned char	effecticon;		// for the interface
		int				duration;		// duration (in seconds)
		char			effectname[32];

		int				target;			// id number of target
		int				effectid;		// id number of the effect that is added (only useful to the victim)
	};

	// CUP_X_SPELLUSE --------------------------------------------------------------------------
	enum ESpellUseResults
	{
		SUR_NORMAL = 0,
		SUR_FAILED,		// the caster failed to cast the specified spell

		NUM_SPELLUSERESULTS
	};
	struct SCUPXSpellUse
	{
		unsigned char	type;


		unsigned char	useresult;		// out of ESpellUseResults
		unsigned char	soundeffect;	// (if any)

		unsigned char	effectcolor;
		char			spellname[32];

		int				caster;
	};

}

#endif
