/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Monster stats and info, as edited by the monster editor.
 **/

#include "CMonsterStats.h"
#include "../library/FileSystem/FileSystem.h"
#include "../Communication/SGenericPacket.h"

#ifndef AKARRASERVER
#ifndef AKARRACLIENT
#include <mem.h>
#endif
#endif

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterType::CMonsterType()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterType::~CMonsterType()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CMonsterType::clear()
{
	ua_dataname[0] = 0;
	ua_displayname[0] = 0;

	u_idlesprite = 0;					// monster type (determines which sprite that is used)
	u_movesprite = 0;
	u_attacksprite = 0;

	u_level = 0;				// challenge rating
	u_grouptype = MG_UNKNOWN;	// out of EMonsterGroups
	u_gender = 2;				// 0 is male
	u_deity = 2;				// 0 is gifted, 1 is shining

	u_color = 5;				// soul color
	u_red = 0xFF;				// monster color (for diffuse layer)
	u_green = 0xFF;
	u_blue = 0xFF;

	u_groupteam = 0;

	u_experience = 1;			// amount of experience this monster is worth

	{
		for (int i(0); i < NUM_ABILITIES; i++)
		{
			ua_abilities[i] = 1;
		}
	}

	u_maxhealth = 10;			// starting health for all monsters of this type
	u_moralehealth = 0;
	u_movespeed = 1000;

	u_flags = AIFLAG_SOCIAL;
	u_sightrange = 10;			// how far away an enemy can be to annoy this monster
	u_behaviour = MB_AGGRESSIVE;	// out of EMonsterBehaviours

	u_magictime = 0;
	u_aggression = 30;
	u_lightradius = 0;

	ua_npcchat[0] = 0;

	u_attacktype = 0;
	u_attackrange = 1;
	u_attackspeed = 5000;
	u_criticalchance = 0.1f;
	u_fumblechance = 0.1f;
	u_attackname = 0;
	u_attackchance = 100;

	u_attackeffect = "";
	u_spelleffects.clear();
	u_effectchance = 0;

	{
		for (int i(0); i < NUM_MONEYTYPES; i++)
		{
			ua_coindropchance[i] = 0;
			ua_coindropamount[i] = 0;
		}
	}

	u_itemdropchance = 0;
	u_itemdropcount = 1;
	u_secondaryitemdropchance = 0;
	u_secondaryitemdropcount = 0;
	u_itemnamelist.clear();
	u_secondaryitemnamelist.clear();

	u_idlechat.clear();
	u_combatchat.clear();
	u_deathchat.clear();

	u_designnotes = "";

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterType::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	clear();

	// read file version
	int version;
	file.read(&version, sizeof(int), 1);
	
	// read stats
	if (version > MTFV_021121)
	{
		file.read(ua_dataname, sizeof(char), MONSTER_NAMELENGTH);
		file.read(ua_displayname, sizeof(char), MONSTER_NAMELENGTH);
	}
	else
	{
		file.read(ua_dataname, sizeof(char), MONSTER_NAMELENGTH);
		strcpy(ua_displayname, ua_dataname);
	}

	if (version > MTFV_020530b)
	{
		file.read(&u_idlesprite, sizeof(unsigned short), 1);
		if (version > MTFV_030516)
		{
			file.read(&u_movesprite, sizeof(unsigned short), 1);
			file.read(&u_attacksprite, sizeof(unsigned short), 1);
		}
		else
		{
			u_movesprite = u_idlesprite;
			u_attacksprite = u_idlesprite;
		}
	}
	else
	{
		u_idlesprite = 0;
	}
	if (version <= MTFV_021018)
	{
		// i changed the u_type to be the sprite number instead of monster type number,
		// which happens to be exactly the double...
		u_idlesprite *= 2;
	}

	file.read(&u_level, sizeof(unsigned char), 1);
	file.read(&u_grouptype, sizeof(unsigned char), 1);
	file.read(&u_gender, sizeof(unsigned char), 1);
	file.read(&u_deity, sizeof(unsigned char), 1);

	file.read(&u_color, sizeof(unsigned char), 1);
	file.read(&u_red, sizeof(unsigned char), 1);
	file.read(&u_green, sizeof(unsigned char), 1);
	file.read(&u_blue, sizeof(unsigned char), 1);

	if (version > MTFV_021110)
	{
		file.read(&u_groupteam, sizeof(int), 1);
	}

	file.read(&u_experience, sizeof(unsigned int), 1);

	if (version <= MTFV_020530)
	{
		// this version didn't have the damage reduction ability
		file.read(ua_abilities, sizeof(float), NUM_ABILITIES - 1);
		ua_abilities[ABILITY_REDUCTION] = 0;
	}
	else
	{
		file.read(ua_abilities, sizeof(float), NUM_ABILITIES);
	}
	file.read(&u_maxhealth, sizeof(float), 1);
	if (version > MTFV_030513)
	{
		file.read(&u_moralehealth, sizeof(float), 1);
	}
	else
	{
		switch (u_grouptype)
		{
			case MG_UNDEAD:
			case MG_PLANT:
			case MG_CONSTRUCT:
				u_moralehealth = 0.0f;
				break;
		
			case MG_OUTSIDER:
			case MG_ELEMENTAL:
				u_moralehealth = 0.10f;
				break;
		
			default:
				u_moralehealth = 0.25f;
				break;
		}
	}
	file.read(&u_movespeed, sizeof(unsigned int), 1);

	file.read(&u_flags, sizeof(unsigned int), 1);
	file.read(&u_sightrange, sizeof(unsigned char), 1);
	file.read(&u_behaviour, sizeof(unsigned char), 1);

	if (version > MTFV_020730)
	{
		file.read(&u_magictime, sizeof(int), 1);
	}
	if (version > MTFV_020803)
	{
		file.read(&u_aggression, sizeof(int), 1);
	}

	if (version > MTFV_021020)
	{
		file.read(&u_lightradius, sizeof(int), 1);
	}

	if (version > MTFV_020921)
	{
		file.read(ua_npcchat, sizeof(char), 32);
	}

	file.read(&u_attacktype, sizeof(unsigned char), 1);
	file.read(&u_attackrange, sizeof(unsigned char), 1);
	file.read(&u_attackspeed, sizeof(unsigned int), 1);
	file.read(&u_criticalchance, sizeof(float), 1);
	file.read(&u_fumblechance, sizeof(float), 1);

	if (version > MTFV_020716)
	{
		file.read(&u_attackname, sizeof(unsigned char), 1);
		file.read(&u_attackchance, sizeof(unsigned char), 1);
	}
	if (version > MTFV_021205)
	{
		file.read(&u_effectchance, sizeof(int), 1);
	}

	if (version > MTFV_020611)
	{
		file.read(ua_coindropchance, sizeof(int), NUM_MONEYTYPES);
		file.read(ua_coindropamount, sizeof(int), NUM_MONEYTYPES);

		if (version <= MTFV_020909)
		{
			ua_coindropamount[0] = (ua_coindropamount[0] * 100) + (ua_coindropamount[1] * 10) + ua_coindropamount[2];
			ua_coindropchance[0] = (ua_coindropchance[2] > ua_coindropchance[0]) ? 
				((ua_coindropchance[2] > ua_coindropchance[1]) ? ua_coindropchance[2] : ua_coindropchance[1]) :
				((ua_coindropchance[0] > ua_coindropchance[1]) ? ua_coindropchance[0] : ua_coindropchance[1]);
			ua_coindropamount[1] = 0;
			ua_coindropamount[2] = 0;
			ua_coindropchance[1] = 0;
			ua_coindropchance[2] = 0;
		}
	}

	if (version > MTFV_020626)
	{
		file.read(&u_itemdropchance, sizeof(int), 1);

		int count(0);
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			char name[32];
			file.read(name, sizeof(char), 32);
			u_itemnamelist.push_back(Basic::CCharString(name));
		}
	}
	if (version > MTFV_030506)
	{
		file.read(&u_itemdropcount, sizeof(int), 1);
		file.read(&u_secondaryitemdropchance, sizeof(int), 1);
		file.read(&u_secondaryitemdropcount, sizeof(int), 1);

		int count(0);
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			char name[32];
			file.read(name, sizeof(char), 32);
			u_secondaryitemnamelist.push_back(Basic::CCharString(name));
		}
	}

	if (version > MTFV_020726)
	{
		{
			char name[32];
			file.read(name, sizeof(char), 32);
			u_attackeffect = name;
		}
		
		int count(0);
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			char name[32];
			file.read(name, sizeof(char), 32);
			u_spelleffects.push_back(Basic::CCharString(name));
		}
	}

	if (version > MTFV_021124)
	{
		u_designnotes = FileSystem::readStringWithLength(file);
	}
	
	if (version > MTFV_021212)
	{
		{
			int count(0);
			file.read(&count, sizeof(int), 1);
			for (int i(0); i < count; i++)
			{
				Basic::CCharString string = FileSystem::readStringWithLength(file);
				u_idlechat.push_back(string);
			}
		}
		{
			int count(0);
			file.read(&count, sizeof(int), 1);
			for (int i(0); i < count; i++)
			{
				Basic::CCharString string = FileSystem::readStringWithLength(file);
				u_combatchat.push_back(string);
			}
		}
		{
			int count(0);
			file.read(&count, sizeof(int), 1);
			for (int i(0); i < count; i++)
			{
				Basic::CCharString string = FileSystem::readStringWithLength(file);
				u_deathchat.push_back(string);
			}
		}
	}

	// make monsters harder
	if (version <= MTFV_030120)
	{
		ua_abilities[ABILITY_ATTACK_RATING] *= 1.1f;
		ua_abilities[ABILITY_ARMORCLASS] *= 1.1f;
		ua_abilities[ABILITY_DAMAGE_MIN] *= 1.1f;
		ua_abilities[ABILITY_DAMAGE_MAX] *= 1.1f;
	}
	if (version <= MTFV_030401)
	{
		ua_abilities[ABILITY_RESIST_WHITE] = 2.0f + (float)u_level;
		ua_abilities[ABILITY_RESIST_RED] = 2.0f + (float)u_level;
		ua_abilities[ABILITY_RESIST_GREEN] = 2.0f + (float)u_level;
		ua_abilities[ABILITY_RESIST_BLUE] = 2.0f + (float)u_level;
		ua_abilities[ABILITY_RESIST_YELLOW] = 2.0f + (float)u_level;
	}
	if (version <= MTFV_030405)
	{
		ua_abilities[ABILITY_RESIST_WHITE] += 3.0f;
		ua_abilities[ABILITY_RESIST_RED] += 3.0f;
		ua_abilities[ABILITY_RESIST_GREEN] += 3.0f;
		ua_abilities[ABILITY_RESIST_BLUE] += 3.0f;
		ua_abilities[ABILITY_RESIST_YELLOW] += 3.0f;
	}
	if (version <= MTFV_030504)
	{
		// decrease HP for monsters that are below level 50
		if (u_level < 50)
		{
			float decreaseamount = 0.25f;
			decreaseamount -= (float)(u_level / 10) * 0.05f;
			decreaseamount = 1.0f - decreaseamount;
			u_maxhealth *= decreaseamount;
		}
	}
	if (version <= MTFV_030510)
	{
		float percentageboost = (100.0f + (float)u_level) * 0.01f;
		for (int i(0); i < (int)NUM_ABILITIES; i++)
		{
			ua_abilities[i] *= percentageboost;
		}
		u_maxhealth *= percentageboost;

		// add extra boost to damage because of CON boost (for characters)
		ua_abilities[ABILITY_DAMAGE_MAX] *= 1.25f;
		ua_abilities[ABILITY_DAMAGE_MIN] *= 1.25f;
	}
	if (version <= MTFV_030519)
	{
		u_attackspeed = (unsigned int)((float)u_attackspeed * 0.70f);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterType::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write file version
	int version = CURRENT_MONSTERTYPE_VERSION;
	file.write(&version, sizeof(int), 1);

	// write stats
	file.write(ua_dataname, sizeof(char), MONSTER_NAMELENGTH);
	file.write(ua_displayname, sizeof(char), MONSTER_NAMELENGTH);

	file.write(&u_idlesprite, sizeof(unsigned short), 1);
	file.write(&u_movesprite, sizeof(unsigned short), 1);
	file.write(&u_attacksprite, sizeof(unsigned short), 1);

	file.write(&u_level, sizeof(unsigned char), 1);
	file.write(&u_grouptype, sizeof(unsigned char), 1);
	file.write(&u_gender, sizeof(unsigned char), 1);
	file.write(&u_deity, sizeof(unsigned char), 1);

	file.write(&u_color, sizeof(unsigned char), 1);
	file.write(&u_red, sizeof(unsigned char), 1);
	file.write(&u_green, sizeof(unsigned char), 1);
	file.write(&u_blue, sizeof(unsigned char), 1);

	file.write(&u_groupteam, sizeof(int), 1);

	file.write(&u_experience, sizeof(unsigned int), 1);

	file.write(ua_abilities, sizeof(float), NUM_ABILITIES);
	file.write(&u_maxhealth, sizeof(float), 1);
	file.write(&u_moralehealth, sizeof(float), 1);
	file.write(&u_movespeed, sizeof(unsigned int), 1);

	file.write(&u_flags, sizeof(unsigned int), 1);
	file.write(&u_sightrange, sizeof(unsigned char), 1);
	file.write(&u_behaviour, sizeof(unsigned char), 1);

	file.write(&u_magictime, sizeof(int), 1);
	file.write(&u_aggression, sizeof(int), 1);
	file.write(&u_lightradius, sizeof(int), 1);

	file.write(ua_npcchat, sizeof(char), 32);

	file.write(&u_attacktype, sizeof(unsigned char), 1);
	file.write(&u_attackrange, sizeof(unsigned char), 1);
	file.write(&u_attackspeed, sizeof(unsigned int), 1);
	file.write(&u_criticalchance, sizeof(float), 1);
	file.write(&u_fumblechance, sizeof(float), 1);

	file.write(&u_attackname, sizeof(unsigned char), 1);
	file.write(&u_attackchance, sizeof(unsigned char), 1);
	file.write(&u_effectchance, sizeof(int), 1);

	// save item drop

	file.write(ua_coindropchance, sizeof(int), NUM_MONEYTYPES);
	file.write(ua_coindropamount, sizeof(int), NUM_MONEYTYPES);

	file.write(&u_itemdropchance, sizeof(int), 1);

	int count = (int)u_itemnamelist.size();
	file.write(&count, sizeof(int), 1);
	{
		for (int i(0); i < count; i++)
		{
			char name[32];
			strcpy(name, u_itemnamelist[i].getString());
			file.write(name, sizeof(char), 32);
		}
	}

	file.write(&u_itemdropcount, sizeof(int), 1);
	file.write(&u_secondaryitemdropchance, sizeof(int), 1);
	file.write(&u_secondaryitemdropcount, sizeof(int), 1);

	count = (int)u_secondaryitemnamelist.size();
	file.write(&count, sizeof(int), 1);
	{
		for (int i(0); i < count; i++)
		{
			char name[32];
			strcpy(name, u_secondaryitemnamelist[i].getString());
			file.write(name, sizeof(char), 32);
		}
	}

	// save attack effect name
	{
		char name[32];
		strcpy(name, u_attackeffect.getString());
		file.write(name, sizeof(char), 32);
	}

	// save spell list
	count = (int)u_spelleffects.size();
	file.write(&count, sizeof(int), 1);
	{
		for (int i = 0; i < count; i++)
		{
			char name[32];
			strcpy(name, u_spelleffects[i].getString());
			file.write(name, sizeof(char), 32);
		}
	}

	FileSystem::writeStringWithLength(file, u_designnotes);

	{
		int count = (int)u_idlechat.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			FileSystem::writeStringWithLength(file, u_idlechat[i]);
		}
	}
	{
		int count = (int)u_combatchat.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			FileSystem::writeStringWithLength(file, u_combatchat[i]);
		}
	}
	{
		int count = (int)u_deathchat.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			FileSystem::writeStringWithLength(file, u_deathchat[i]);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CMonsterType::copyOther(CMonsterType* other)
{
	// remove old info first
	clear();

	strcpy(ua_dataname, other->ua_dataname);
	strcpy(ua_displayname, other->ua_displayname);

	u_idlesprite = other->u_idlesprite;
	u_movesprite = other->u_movesprite;
	u_attacksprite = other->u_attacksprite;

	u_level = other->u_level;
	u_grouptype = other->u_grouptype;	
	u_gender = other->u_gender;				
	u_deity = other->u_deity;				

	u_color = other->u_color;				
	u_red = other->u_red;				
	u_green = other->u_green;
	u_blue = other->u_blue;

	u_groupteam = other->u_groupteam;

	u_experience = other->u_experience;

	{
		for (int i(0); i < NUM_ABILITIES; i++)
		{
			ua_abilities[i] = other->ua_abilities[i];
		}
	}

	u_maxhealth = other->u_maxhealth;			// starting health for all monsters of this type
	u_moralehealth = other->u_moralehealth;
	u_movespeed = other->u_movespeed;

	u_flags = other->u_flags;
	u_sightrange = other->u_sightrange;			// how far away an enemy can be to annoy this monster
	u_behaviour = other->u_behaviour;	// out of EMonsterBehaviours

	u_magictime = other->u_magictime;
	u_aggression = other->u_aggression;
	u_lightradius = other->u_lightradius;

	strcpy(ua_npcchat, other->ua_npcchat);

	u_attacktype = other->u_attacktype;
	u_attackrange = other->u_attackrange;
	u_attackspeed = other->u_attackspeed;
	u_criticalchance = other->u_criticalchance;
	u_fumblechance = other->u_fumblechance;
	u_attackname = other->u_attackname;
	u_attackchance = other->u_attackchance;

	u_attackeffect = other->u_attackeffect;
	u_effectchance = other->u_effectchance;
	{
		for (int i(0); i < (int)other->u_spelleffects.size(); i++)
		{
			u_spelleffects.push_back(other->u_spelleffects[i]);
		}
	}

	{
		for (int i(0); i < NUM_MONEYTYPES; i++)
		{
			ua_coindropchance[i] = other->ua_coindropchance[i];
			ua_coindropamount[i] = other->ua_coindropamount[i];
		}
	}

	u_itemdropchance = other->u_itemdropchance;
	u_itemdropcount = other->u_itemdropcount;
	u_secondaryitemdropchance = other->u_secondaryitemdropchance;
	u_secondaryitemdropcount = other->u_secondaryitemdropcount;
	{
		for (int i(0); i < (int)other->u_itemnamelist.size(); i++)
		{
			u_itemnamelist.push_back(other->u_itemnamelist[i]);
		}
	}
	{
		for (int i(0); i < (int)other->u_secondaryitemnamelist.size(); i++)
		{
			u_secondaryitemnamelist.push_back(other->u_secondaryitemnamelist[i]);
		}
	}

	{
		for (int i(0); i < (int)other->u_idlechat.size(); i++)
		{
			u_idlechat.push_back(other->u_idlechat[i]);
		}
	}
	{
		for (int i(0); i < (int)other->u_combatchat.size(); i++)
		{
			u_combatchat.push_back(other->u_combatchat[i]);
		}
	}
	{
		for (int i(0); i < (int)other->u_deathchat.size(); i++)
		{
			u_deathchat.push_back(other->u_deathchat[i]);
		}
	}

	u_designnotes = other->u_designnotes;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterType::createData(unsigned char** dest, int& size, Basic::CCharString& failreason)
{
	*dest = NULL;
	size = 0;

	// create a temporary buffer that is insanely big
	unsigned char* data = new unsigned char[1024 * 16];

	if (!data)
	{
		failreason = "Unable to create data buffer";
		return false;
	}

	// skip the first 8 bytes to allow the caller to insert a packet header since this
	// buffer will be used as a network message
	// 030106: Also make room for another integer that identifies this as a spawnpoint,
	// since new items and monsters will use the same packet type
	memset(data, 0, sizeof(Comm::SGenericPacket) + sizeof(int));
	size += sizeof(Comm::SGenericPacket) + sizeof(int);

	// insert version info so the client can send spawns to the server even if the server
	// has been updated
	int version = CURRENT_MONSTERTYPE_VERSION;
	memcpy(&data[size], &version, sizeof(int));
	size += sizeof(int);

	// write stats
	memcpy(&data[size], ua_dataname, sizeof(char) * MONSTER_NAMELENGTH);
	size += sizeof(char) * MONSTER_NAMELENGTH;
	memcpy(&data[size], ua_displayname, sizeof(char) * MONSTER_NAMELENGTH);
	size += sizeof(char) * MONSTER_NAMELENGTH;

	memcpy(&data[size], &u_idlesprite, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;
	memcpy(&data[size], &u_movesprite, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;
	memcpy(&data[size], &u_attacksprite, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;

	memcpy(&data[size], &u_level, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_grouptype, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_gender, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_deity, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;

	memcpy(&data[size], &u_color, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_red, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_green, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_blue, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;

	memcpy(&data[size], &u_groupteam, sizeof(int) * 1);
	size += sizeof(int) * 1;

	memcpy(&data[size], &u_experience, sizeof(unsigned int) * 1);
	size += sizeof(unsigned int) * 1;

	memcpy(&data[size], ua_abilities, sizeof(float) * NUM_ABILITIES);
	size += sizeof(float) * NUM_ABILITIES;
	memcpy(&data[size], &u_maxhealth, sizeof(float) * 1);
	size += sizeof(float) * 1;
	memcpy(&data[size], &u_moralehealth, sizeof(float) * 1);
	size += sizeof(float) * 1;
	memcpy(&data[size], &u_movespeed, sizeof(unsigned int) * 1);
	size += sizeof(unsigned int) * 1;

	memcpy(&data[size], &u_flags, sizeof(unsigned int) * 1);
	size += sizeof(unsigned int) * 1;
	memcpy(&data[size], &u_sightrange, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_behaviour, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;

	memcpy(&data[size], &u_magictime, sizeof(int) * 1);
	size += sizeof(int) * 1;
	memcpy(&data[size], &u_aggression, sizeof(int) * 1);
	size += sizeof(int) * 1;
	memcpy(&data[size], &u_lightradius, sizeof(int) * 1);
	size += sizeof(int) * 1;

	memcpy(&data[size], ua_npcchat, sizeof(char) * 32);
	size += sizeof(char) * 32;

	memcpy(&data[size], &u_attacktype, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_attackrange, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_attackspeed, sizeof(unsigned int) * 1);
	size += sizeof(unsigned int) * 1;
	memcpy(&data[size], &u_criticalchance, sizeof(float) * 1);
	size += sizeof(float) * 1;
	memcpy(&data[size], &u_fumblechance, sizeof(float) * 1);
	size += sizeof(float) * 1;

	memcpy(&data[size], &u_attackname, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_attackchance, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_effectchance, sizeof(int) * 1);
	size += sizeof(int) * 1;

	// save item drop

	memcpy(&data[size], ua_coindropchance, sizeof(int) * NUM_MONEYTYPES);
	size += sizeof(int) * NUM_MONEYTYPES;
	memcpy(&data[size], ua_coindropamount, sizeof(int) * NUM_MONEYTYPES);
	size += sizeof(int) * NUM_MONEYTYPES;

	memcpy(&data[size], &u_itemdropchance, sizeof(int) * 1);
	size += sizeof(int) * 1;

	int count = (int)u_itemnamelist.size();
	memcpy(&data[size], &count, sizeof(int) * 1);
	size += sizeof(int) * 1;
	{
		for (int i(0); i < count; i++)
		{
			char name[32];
			strcpy(name, u_itemnamelist[i].getString());

			memcpy(&data[size], name, sizeof(char) * 32);
			size += sizeof(char) * 32;
		}
	}
	
	memcpy(&data[size], &u_itemdropcount, sizeof(int) * 1);
	size += sizeof(int) * 1;
	memcpy(&data[size], &u_secondaryitemdropchance, sizeof(int) * 1);
	size += sizeof(int) * 1;
	memcpy(&data[size], &u_secondaryitemdropcount, sizeof(int) * 1);
	size += sizeof(int) * 1;

	count = (int)u_secondaryitemnamelist.size();
	memcpy(&data[size], &count, sizeof(int) * 1);
	size += sizeof(int) * 1;
	{
		for (int i(0); i < count; i++)
		{
			char name[32];
			strcpy(name, u_secondaryitemnamelist[i].getString());

			memcpy(&data[size], name, sizeof(char) * 32);
			size += sizeof(char) * 32;
		}
	}

	// save attack effect name
	{
		char name[32];
		strcpy(name, u_attackeffect.getString());
		memcpy(&data[size], name, sizeof(char) * 32);
		size += sizeof(char) * 32;
	}

	// save spell list
	count = (int)u_spelleffects.size();
	memcpy(&data[size], &count, sizeof(int) * 1);
	size += sizeof(int) * 1;
	{
		for (int i = 0; i < count; i++)
		{
			char name[32];
			strcpy(name, u_spelleffects[i].getString());

			memcpy(&data[size], name, sizeof(char) * 32);
			size += sizeof(char) * 32;
		}
	}

	//FileSystem::writeStringWithLength(file, u_designnotes);
	count = (int)u_designnotes.getStringLength();
	memcpy(&data[size], &count, sizeof(int) * 1);
	size += sizeof(int) * 1;
	memcpy(&data[size], u_designnotes.getString(), sizeof(char) * count);
	size += sizeof(char) * count;

	{
		count = (int)u_idlechat.size();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;
		for (int i(0); i < count; i++)
		{
			int length = (int)u_idlechat[i].getStringLength();
			memcpy(&data[size], &length, sizeof(int) * 1);
			size += sizeof(int) * 1;
			memcpy(&data[size], u_idlechat[i].getString(), sizeof(char) * length);
			size += sizeof(char) * length;
		}
	}
	{
		count = (int)u_combatchat.size();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;
		for (int i(0); i < count; i++)
		{
			int length = (int)u_combatchat[i].getStringLength();
			memcpy(&data[size], &length, sizeof(int) * 1);
			size += sizeof(int) * 1;
			memcpy(&data[size], u_combatchat[i].getString(), sizeof(char) * length);
			size += sizeof(char) * length;
		}
	}
	{
		count = (int)u_deathchat.size();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;
		for (int i(0); i < count; i++)
		{
			int length = (int)u_deathchat[i].getStringLength();
			memcpy(&data[size], &length, sizeof(int) * 1);
			size += sizeof(int) * 1;
			memcpy(&data[size], u_deathchat[i].getString(), sizeof(char) * length);
			size += sizeof(char) * length;
		}
	}

	// place a checkpoint (to make sure that the packet is received correctly)
	int checkpoint = MONSTERPACKET_CHECK;
	memcpy(&data[size], &checkpoint, sizeof(int));
	size += sizeof(int);

	// copy buffer
	*dest = new unsigned char[size];
	memcpy(*dest, data, size);
	delete [] data;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterType::loadData(unsigned char* source, int size, Basic::CCharString& failreason)
{
	if (!source || (size < 12))
	{
		failreason = "Incorrect parameters in packet";
		return false;
	}

	// reset this monster
	clear();

	int pos = sizeof(Comm::SGenericPacket) + sizeof(int);	// skip the header

	// get monster information version
	int monsterversion;
	memcpy(&monsterversion, &source[pos], sizeof(int));
	pos += sizeof(int);

	// read stats
	memcpy(ua_dataname, &source[pos], sizeof(char) * MONSTER_NAMELENGTH);
	pos += sizeof(char) * MONSTER_NAMELENGTH;
	memcpy(ua_displayname, &source[pos], sizeof(char) * MONSTER_NAMELENGTH);
	pos += sizeof(char) * MONSTER_NAMELENGTH;

	memcpy(&u_idlesprite, &source[pos], sizeof(unsigned short) * 1);
	pos += sizeof(unsigned short) * 1;
	if (monsterversion > MTFV_030516)
	{
		memcpy(&u_movesprite, &source[pos], sizeof(unsigned short) * 1);
		pos += sizeof(unsigned short) * 1;
		memcpy(&u_attacksprite, &source[pos], sizeof(unsigned short) * 1);
		pos += sizeof(unsigned short) * 1;
	}

	memcpy(&u_level, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_grouptype, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_gender, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_deity, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;

	memcpy(&u_color, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_red, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_green, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_blue, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;

	memcpy(&u_groupteam, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	memcpy(&u_experience, &source[pos], sizeof(unsigned int) * 1);
	pos += sizeof(unsigned int) * 1;

	memcpy(ua_abilities, &source[pos], sizeof(float) * NUM_ABILITIES);
	pos += sizeof(float) * NUM_ABILITIES;
	memcpy(&u_maxhealth, &source[pos], sizeof(float) * 1);
	pos += sizeof(float) * 1;
	if (monsterversion > MTFV_030513)
	{
		memcpy(&u_moralehealth, &source[pos], sizeof(float) * 1);
		pos += sizeof(float) * 1;
	}
	memcpy(&u_movespeed, &source[pos], sizeof(unsigned int) * 1);
	pos += sizeof(unsigned int) * 1;

	memcpy(&u_flags, &source[pos], sizeof(unsigned int) * 1);
	pos += sizeof(unsigned int) * 1;
	memcpy(&u_sightrange, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_behaviour, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;

	memcpy(&u_magictime, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;
	memcpy(&u_aggression, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;
	memcpy(&u_lightradius, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	memcpy(ua_npcchat, &source[pos], sizeof(char) * 32);
	pos += sizeof(char) * 32;

	memcpy(&u_attacktype, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_attackrange, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_attackspeed, &source[pos], sizeof(unsigned int) * 1);
	pos += sizeof(unsigned int) * 1;
	memcpy(&u_criticalchance, &source[pos], sizeof(float) * 1);
	pos += sizeof(float) * 1;
	memcpy(&u_fumblechance, &source[pos], sizeof(float) * 1);
	pos += sizeof(float) * 1;

	memcpy(&u_attackname, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_attackchance, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_effectchance, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	// save item drop

	memcpy(ua_coindropchance, &source[pos], sizeof(int) * NUM_MONEYTYPES);
	pos += sizeof(int) * NUM_MONEYTYPES;
	memcpy(ua_coindropamount, &source[pos], sizeof(int) * NUM_MONEYTYPES);
	pos += sizeof(int) * NUM_MONEYTYPES;

	memcpy(&u_itemdropchance, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	int count = 0;
	memcpy(&count, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;
	{
		for (int i(0); i < count; i++)
		{
			char name[32];
			memcpy(name, &source[pos], sizeof(char) * 32);
			pos += sizeof(char) * 32;

			Basic::CCharString newitem(name);
			u_itemnamelist.push_back(newitem);
		}
	}

	if (monsterversion > MTFV_030506)
	{
		memcpy(&u_itemdropcount, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		memcpy(&u_secondaryitemdropchance, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		memcpy(&u_secondaryitemdropcount, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;

		count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		{
			for (int i(0); i < count; i++)
			{
				char name[32];
				memcpy(name, &source[pos], sizeof(char) * 32);
				pos += sizeof(char) * 32;

				Basic::CCharString newitem(name);
				u_secondaryitemnamelist.push_back(newitem);
			}
		}
	}

	// save attack effect name
	{
		char name[32];
		memcpy(name, &source[pos], sizeof(char) * 32);
		pos += sizeof(char) * 32;

		u_attackeffect = name;
	}

	// save spell list
	count = 0;
	memcpy(&count, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;
	{
		for (int i = 0; i < count; i++)
		{
			char name[32];
			memcpy(name, &source[pos], sizeof(char) * 32);
			pos += sizeof(char) * 32;

			Basic::CCharString newspell(name);
			u_spelleffects.push_back(newspell);
		}
	}

	//FileSystem::writeStringWithLength(file, u_designnotes);
	count = 0;
	memcpy(&count, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;
	{
		char* string = new char[count + 1];
		memcpy(string, &source[pos], sizeof(char) * count);
		pos += sizeof(char) * count;
		string[count] = 0;

		u_designnotes = string;

		delete [] string;
	}

	{
		count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		for (int i(0); i < count; i++)
		{
			int length = 0;
			memcpy(&length, &source[pos], sizeof(int) * 1);
			pos += sizeof(int) * 1;

			char* string = new char[length + 1];
			memcpy(string, &source[pos], sizeof(char) * length);
			pos += sizeof(char) * length;
			string[length] = 0;

			Basic::CCharString newline(string);
			u_idlechat.push_back(newline);
		}
	}
	{
		count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		for (int i(0); i < count; i++)
		{
			int length = 0;
			memcpy(&length, &source[pos], sizeof(int) * 1);
			pos += sizeof(int) * 1;

			char* string = new char[length + 1];
			memcpy(string, &source[pos], sizeof(char) * length);
			pos += sizeof(char) * length;
			string[length] = 0;

			Basic::CCharString newline(string);
			u_combatchat.push_back(newline);
		}
	}
	{
		count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		for (int i(0); i < count; i++)
		{
			int length = 0;
			memcpy(&length, &source[pos], sizeof(int) * 1);
			pos += sizeof(int) * 1;

			char* string = new char[length + 1];
			memcpy(string, &source[pos], sizeof(char) * length);
			pos += sizeof(char) * length;
			string[length] = 0;

			Basic::CCharString newline(string);
			u_deathchat.push_back(newline);
		}
	}

	// check the checkpoint
	int checkpoint = 0;
	memcpy(&checkpoint, &source[pos], sizeof(int));
	pos += sizeof(int);

	if (checkpoint != MONSTERPACKET_CHECK)
	{
		failreason = "CRC check failed";
		return false;
	}

	return true;
}

// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This constructor is used when we don't have any useful info about the monster
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterTypeInstance::CMonsterTypeInstance(int runtimeid)
:
	u_runtimeid(runtimeid),
	u_dataisok(false),
	u_appearance(0xFFFF0000),	// this will look like a white rabbit
	u_lastrequesttime(0)
{
	strcpy(ua_name, "Lagmonster");
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterTypeInstance::CMonsterTypeInstance(int runtimeid, unsigned int appearance, unsigned short move, unsigned short attack, char* name)
:
	u_runtimeid(runtimeid),
	u_dataisok(true),
	u_appearance(appearance),
	u_movesprite(move),
	u_attacksprite(attack),
	u_lastrequesttime(0)
{
	strcpy(ua_name, name);
}

}
