/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Monster stats and info, as edited by the monster editor.
 **/

#ifndef _CMONSTERSTATS_H_
#define _CMONSTERSTATS_H_

#include "CAbility.h"
#include "CMoney.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/Basic/CCharString.h"

#include <vector>

namespace RpgSystem
{

	const int			MONSTER_NAMELENGTH	= 32;	// 31 usable characters
	const char* const	MONSTER_EXTENSION  	= ".mon";
	const char* const	MONSTER_DIRECTORY  	= "Creatures";

	enum EMonsterTypeFileVersion
	{
		MTFV_ORIGINAL = 0,

		MTFV_020530,	// added damage reduction in character abilities
		MTFV_020530b,	// had forgotten to store monstery type (lol?)
		MTFV_020611,	// added money drops
		MTFV_020626,	// added item drops
		MTFV_020716,	// added attack names and attack chance (ranged attacks)
		MTFV_020726,	// added attack effect and spell list
		MTFV_020730,	// added u_magictime
		MTFV_020803,	// added u_aggression
		MTFV_020909,	// changed cointype system
		MTFV_020921,	// added chat manuscript id
		MTFV_021018,	// u_type is now the sprite id instead of monster id (multiply by 2)
		MTFV_021020,	// added u_lightradius
		MTFV_021110,	// added u_groupteam
		MTFV_021121,	// added ua_dataname and ua_displayname
		MTFV_021124,	// added u_designnotes
		MTFV_021205,	// added u_effectchance
		MTFV_021212,	// added chatlists
		MTFV_030120,	// i made all monsters slightly harder (AR, AC, MinDmg, MaxDmg)
		MTFV_030401,	// rebalanced resistances to equal level of monster
		MTFV_030405,	// another rebalance
		MTFV_030504,	// major item rebalance (decreasing HP for monsters that are weaker than level 50)
		MTFV_030506,	// added multiple item drops
		MTFV_030510,	// another major rebalance (boosted monsters because of improved skill system)
		MTFV_030513,	// added u_moralehealth
		MTFV_030516,	// changed the sprite info so each monster has three sprites: idle, move and attack
		MTFV_030519,	// test attack speed

		CURRENT_MONSTERTYPE_VERSION
	};

	// max 32
	enum EMonsterGroups
	{
		MG_UNKNOWN = 0,

		MG_HUMAN,
		MG_MENIT,
		MG_IWID,
		MG_HUMANOID,

		MG_DARKLING,
		MG_OUTSIDER,	// demons, angels, or other abstract monster things on the "Gifted" or "Shining"

		MG_ANIMAL,		// mammals, birds, fish, reptiles/snakes
		MG_INSECT,		// insects (normal and giant) and similar (such as spiders)

		MG_UNDEAD,		// skeletons, zombies, wraiths, vampires etc
		MG_DRAGON,		// all dragons
		MG_GIANT,		// giants, ogres, minotaurs, titans (ie, large humanoids)
		MG_PLANT,		// living trees, vines or other plants or plant-like creatures
		MG_FABLE,		// griffon, hydra, manticore, chimera, gorgon, medusae etc

		MG_ELEMENTAL,	// elementars, salamanders, tempests etc
		MG_CONSTRUCT,	// golems and other mechanical creatures (note: some golems are elemtal though)

		NUM_MONSTERGROUPS

	};
	
	class CMonsterType
	{
		public:

			CMonsterType();
			virtual ~CMonsterType();

			void clear();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			void copyOther(CMonsterType* other);

			// network functions

			bool createData(unsigned char** dest, int& size, Basic::CCharString& failreason);
			bool loadData(unsigned char* source, int size, Basic::CCharString& failreason);

		public:

			char			ua_dataname[MONSTER_NAMELENGTH];
			char			ua_displayname[MONSTER_NAMELENGTH];

			unsigned short	u_idlesprite;	// sprite used when the monster is standing still
			unsigned short	u_movesprite;
			unsigned short	u_attacksprite;

			unsigned char	u_level;		// challenge rating
			unsigned char	u_grouptype;	// out of EMonsterGroups
			unsigned char	u_gender;		// 0 is male, 1 is female, 2 is random, 3 is n/a
			unsigned char	u_deity;		// 0 is gifted, 1 is shining, 2 is none

			unsigned char	u_color;		// soul color (5 is random)
			unsigned char	u_red;			// monster color (for diffuse layer)
			unsigned char	u_green;
			unsigned char	u_blue;

			int				u_groupteam;	// if these monsters belong to a special group

			unsigned int	u_experience;	// amount of experience this monster is worth
			
			float			ua_abilities[NUM_ABILITIES];
			float			u_maxhealth;	// starting health for all monsters of this type
			float			u_moralehealth;	// a value from 0 to 1 that says how low the monster's health can go before it tries to flee
			unsigned int	u_movespeed;

			// AI / Behaviour settings

			unsigned int	u_flags;
			unsigned char	u_sightrange;	// how far away an enemy can be to annoy this monster
			unsigned char	u_behaviour;	// out of EMonsterBehaviours

			int				u_magictime;	// how often the monster will cast spells (seconds)
			int				u_aggression;	// how many seconds the monster will fight/hunt a player
			int				u_lightradius;	// how far the monster spreads light

			char			ua_npcchat[32];	// name of the chat manuscript for NPCs

			// primary attack setting

			unsigned char	u_attacktype;
			unsigned char	u_attackrange;
			unsigned char	u_attackchance;	// percentual chance to hit with ranged attacks
			unsigned char	u_attackname;	// "hits", "bites", "claws" etc
			unsigned int	u_attackspeed;
			float			u_criticalchance;
			float			u_fumblechance;

			// Add attack effect and spell effects:

			Basic::CCharString				u_attackeffect;
			std::vector<Basic::CCharString>	u_spelleffects;
			int				u_effectchance;	// percentual chance for the attack effect to take effect

			// money drops

			int				ua_coindropchance[NUM_MONEYTYPES];	// percentage chance that money is dropped
			int				ua_coindropamount[NUM_MONEYTYPES];	// max amount of money dropped

			int				u_itemdropchance;
			int				u_itemdropcount;
			int				u_secondaryitemdropchance;
			int				u_secondaryitemdropcount;
			std::vector<Basic::CCharString>	u_itemnamelist;
			std::vector<Basic::CCharString>	u_secondaryitemnamelist;

			// monster chatting

			std::vector<Basic::CCharString>	u_idlechat;		// randomly saying stuff
			std::vector<Basic::CCharString>	u_combatchat;	// saying stuff while fighting
			std::vector<Basic::CCharString>	u_deathchat;	// saying stuff when dying

			// other

			Basic::CCharString	u_designnotes;

			// The runtime id number is used by the server to identify monster types for players.
			// This is used to decrease the size of a monster packet when a new monster is added on
			// the client-side. The runtime id number is set by the monsterbundle as the bundle is loaded.

			int					u_runtimeid;
			
	};

	// ai flags uses 0x0000FFFF of u_flags
	const unsigned int AIFLAG_DETERMINED	= 0x1;	// this monster will not change target once it has found one
	const unsigned int AIFLAG_SOCIAL		= 0x2;	// will help other monsters if they are attacked
	const unsigned int AIFLAG_MAGICUSER		= 0x4;	// will prefer to use magic over normal attacks
	const unsigned int AIFLAG_FORCESOCIAL	= 0x8;	// will force monsters at the same spawn to help out
	const unsigned int AIFLAG_SLAYER		= 0x10;	// will always attack the weakest attacker
	const unsigned int AIFLAG_NOIDLEMOVE	= 0x20;	// this monster stands still while in idle mode

	// other flags uses 0xFFFF0000 of u_flags
	const unsigned int MOFLAG_FLYING		= 0x10000;	// can move over "block non-flyer" tiles
	const unsigned int MOFLAG_ETHEREAL		= 0x20000;	// can move over "block non-ethereal" tiles
	const unsigned int MOFLAG_NPC			= 0x40000;	// this monster is a NPC, and cannot be killed
	const unsigned int MOFLAG_SHIELDUSER	= 0x80000;	// this monster carries a shield (used for certain effects)
	const unsigned int MOFLAG_DARKNESS		= 0x100000;	// this monster causes darknes around itself

	const int MONSTERPACKET_CHECK			= 0x23344556;

	enum EMonsterBehaviours
	{
		MB_PEACEFUL,	// will ignore characters unless attacked
		MB_AGGRESSIVE,	// will attack characters on sight
		MG_SHY,			// will attempt to avoid characters unless attacked
	};

	// This class is used on the client to store information about monstertypes. I made it because
	// I wanted to minimize the amount of information that was required to send a monster from the server
	// to the client (we save about 32 bytes per monster, which means that the new budjet is max 50
	// instead of 23)
	class CMonsterTypeInstance
	{
		public:

			CMonsterTypeInstance(int runtimeid);
			CMonsterTypeInstance(int runtimeid, unsigned int appearance, unsigned short move, unsigned short attack, char* name);

		public:

			int				u_runtimeid;
			bool			u_dataisok;

			char			ua_name[32];
			unsigned int	u_appearance;	// holds sprite and colors
			unsigned short	u_movesprite;
			unsigned short	u_attacksprite;

			// this is used by the client to prevent numerous monstertype requests (since an
			// unknown monstertype can be added lots of times in a single area, but we should only
			// request updated info about it once)
			int				u_lastrequesttime;
	};
}


#endif
