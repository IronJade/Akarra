/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This is a wrapper class for all of a player character's attributes.
 **/

#include "CCharacterAttributes.h"
#include "CCharacterInventory.h"
#include "../PlayerSystem/AkarraCharacter.h"
#include <memory.h>
#include "CSkill.h"
#include "CChrEffect.h"
#include "CExperience.h"

namespace RpgSystem
{

CCharacterAttributes::CCharacterAttributes()
{
	m_clear();
}

CCharacterAttributes::~CCharacterAttributes()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterAttributes::m_clear()
{
	u_skills.clear();
	u_runeffects.clear();
	u_victims.clear();
}

void CCharacterAttributes::removeAll()
{
	//delete [] up_skills;
	//delete [] up_skilldata;

	{
		for (int i(0); i < u_skills.size(); i++)
		{
			delete u_skills[i];
		}
		u_skills.clear();
	}
	{
		std::list<CChrEffect*>::iterator effect;
		for (effect = u_runeffects.begin(); effect != u_runeffects.end(); ++effect)
		{
			delete (*effect);
		}
	}

	// delete victim names
	{
		TDVictimNameList::iterator victim;
		for (victim = u_victims.begin(); victim != u_victims.end(); ++victim)
		{
			delete (*victim);
		}
	}

	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	Reads the character attributes from a file.
// Parameters:	file:		The current file, must have been opened before call.
//				version:	The character file version that is being read.
// Returns:		Success or failure.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::read(FileSystem::CFileStream& file, int version)
{
	// clear old data
	removeAll();

	// read base stats and number of skills
	if (!u_stats.read(file, version))
	{
		return false;
	}

	if (version <= Player::CHRFV_020818)
	{
		int blah;
		file.read(&blah, sizeof(int), 1);
	}
	else
	{
		// new skill system
		int skills;
		file.read(&skills, sizeof(int), 1);

		for (int i(0); i < skills; i++)
		{
			CSkill* skill = new CSkill;
			skill->read(file, version);

			u_skills.push_back(skill);
		}
	}

	// runable effects
	if (version > Player::CHRFV_021022)
	{
		int numeffects = 0;
		file.read(&numeffects, sizeof(int), 1);
		{
			for (int i(0); i < numeffects; i++)
			{
				CChrEffect* effect = new CChrEffect;
				effect->read(file, version);

				u_runeffects.push_back(effect);
			}
		}
	}

	// read victim list
	if (version > Player::CHRFV_030301)
	{
		int count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			SVictimName* victim = new SVictimName;
			file.read(victim->name, sizeof(char), 16);
			file.read(&victim->seconds, sizeof(int), 1);

			u_victims.push_back(victim);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	Writes the character attributes to a file.
// Parameters:	file:		The current file, must have been opened before call.
//				version:	The character file version that is being written.
// Returns:		Success or failure.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::write(FileSystem::CFileStream& file, int version)
{
	// save base stats first
	//file.write(&u_stats, sizeof(SBaseStats), 1);
	if (!u_stats.write(file, version))
	{
		return false;
	}

	// save number of skills
	//file.write(&u_numskills, sizeof(int), 1);
	// save the skill data
	//file.write(up_skills, sizeof(unsigned short), u_numskills);
	//file.write(up_skilldata, sizeof(CSkill), u_numskills);

	// new skill system
	int numskills = u_skills.size();
	file.write(&numskills, sizeof(int), 1);
	{
		for (int i(0); i < numskills; i++)
		{
			u_skills[i]->write(file);
		}
	}

	// runable effects
	int numeffects = u_runeffects.size();
	file.write(&numeffects, sizeof(int), 1);
	{
		std::list<CChrEffect*>::iterator effect;
		for (effect = u_runeffects.begin(); effect != u_runeffects.end(); ++effect)
		{
			(*effect)->write(file);
		}
	}

	// write victim list
	{
		int count = (int)u_victims.size();
		file.write(&count, sizeof(int), 1);

		TDVictimNameList::iterator victim;
		for (victim = u_victims.begin(); victim != u_victims.end(); ++victim)
		{
			file.write((*victim)->name, sizeof(char), 16);
			file.write(&(*victim)->seconds, sizeof(int), 1);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	Used to get the status of a character skill.
// Parameters:	skillnumber:		The skill we're asking for.
//				saveindex:			Use this if you need to know the position in the
//									player's skill array that the skill is stored.
//				mustbeabletouse:	Set this to true if the skill must be use-able by the
//									character (some skills can be learnt and disabled
//									later because of an attribute change).
//				mustbelocked:		Set this to true if the skill must be locked.
// Returns:		True if the character has the skill and that it fullfills the requirements
//				(ie, if it has to be useable or locked).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::hasSkill(int skillnumber, int* saveindex, bool mustbelocked)
{
	for (int i = 0; i < u_skills.size(); i++)
	{
		if (u_skills[i]->getSkillId() == skillnumber)
		{
			if (saveindex)
			{
				*saveindex = i;
			}
			if (mustbelocked)
			{
				if (!u_skills[i]->isSpecialized())
				{
					return false;
				}
			}
			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	Adds a skill to the player. The skill isn't added if the character
//				already has it.
// Parameters:	skillnumber:	The skill's id number.
// Returns:		True if the skill actually where added.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::addSkill(CSkillType2* skilltype, int startrow)
{
	if (!skilltype)
	{
		return false;
	}

	if (hasSkill(skilltype->getId()))
	{
		return false;
	}

	// create the skill
	CSkill* skill = new CSkill;
	skill->setNewSkill(skilltype, startrow);

	// add skill
	u_skills.push_back(skill);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterAttributes::addVictim(char* charactername)
{
	// search for the name, because we don't want to add doubles
	TDVictimNameList::iterator victim;
	for (victim = u_victims.begin(); victim != u_victims.end(); ++victim)
	{
		if (stricmp((*victim)->name, charactername) == 0)
		{
			// we already have this victim, so reset the timer
			(*victim)->seconds = PK_EXPIRETIME;
			return;
		}
	}

	// new victim, add it
	SVictimName* vn = new SVictimName;
	strcpy(vn->name, charactername);
	vn->seconds = PK_EXPIRETIME;

	u_victims.push_back(vn);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::clearVictim(char* charactername)
{
	// search for the name
	TDVictimNameList::iterator victim;
	for (victim = u_victims.begin(); victim != u_victims.end(); ++victim)
	{
		if (stricmp((*victim)->name, charactername) == 0)
		{
			// found the victim, so remove him from this player
			delete (*victim);
			u_victims.erase(victim);
			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::hasVictim(char* charactername)
{
	// search for the name
	TDVictimNameList::iterator victim;
	for (victim = u_victims.begin(); victim != u_victims.end(); ++victim)
	{
		if (stricmp((*victim)->name, charactername) == 0)
		{
			return true;
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterAttributes::isPKer()
{
	return (!u_victims.empty());
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

SBaseStats::SBaseStats()
{
	clear();
}

void SBaseStats::clear()
{
	pclass = 0;
	level = 0;
	experience = 0;
	gender = 0;
	soulcolor = 0;
	god = 0;
	hunger = 100.0f;
	race = CR_HUMAN;

	// set default stats:
	int i;
	for (i = 0; i < NUM_ATTRIBUTES; i++)
	{
		attributes[i].setBase(10.0f);
	}
	for (i = 0; i < NUM_VARATTRIBUTES; i++)
	{
		variable[i].setBase(20.0f);
		variable[i].restore();
	}
	for (i = 0; i < NUM_ABILITIES; i++)
	{
		abilities[i].setBase(10.0f);
	}

	newpoints = 0;
	totalpoints = 0;
	freegems[0] = 0;
	freegems[1] = 0;
	freegems[2] = 0;
	freegems[3] = 0;
	freegems[4] = 0;
	totalgems = 0;

	attackspeed = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	Reads the stats from a file.
// Parameters:	file:		The current file, must have been opened before call.
//				version:	The character file version that is being read.
// Returns:		Success or failure.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool SBaseStats::read(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	// attributes and abilities
	int i;
	for (i = 0; i < NUM_ATTRIBUTES; i++)
	{
		attributes[i].read(file, version);
	}

	if (version > Player::CHRFV_020505)
	{
		for (i = 0; i < NUM_VARATTRIBUTES; i++)
		{
			variable[i].read(file, version);
		}
	}
	else
	{
		// this version didn't have the stamina attribute
		for (i = 0; i <= HA_MANA; i++)
		{
			variable[i].read(file, version);
		}
		variable[HA_STAMINA].setBase(attributes[BA_CONSTITUTION].getValue() * 3.0f);
	}
	
	if (version > Player::CHRFV_020530)
	{
		for (i = 0; i < NUM_ABILITIES; i++)
		{
			abilities[i].read(file, version);
		}
	}
	else
	{
		// this version didn't have the damage reduction variable
		for (i = 0; i <= ABILITY_RESIST_YELLOW; i++)
		{
			abilities[i].read(file, version);
		}
		abilities[ABILITY_REDUCTION].setBase(0);
	}

	// basic character information
	file.read(&pclass,			sizeof(unsigned short), 1);
	file.read(&level,			sizeof(unsigned short), 1);
	file.read(&experience,		sizeof(unsigned int), 1);
	file.read(&gender,			sizeof(unsigned char), 1);
	file.read(&soulcolor,		sizeof(unsigned char), 1);
	file.read(&god,				sizeof(unsigned char), 1);
	file.read(&race,			sizeof(unsigned char), 1);

	// attribute points
	file.read(&newpoints,		sizeof(unsigned short), 1);
	file.read(&totalpoints,		sizeof(unsigned short), 1);

	// proficiency gems
	file.read(freegems,			sizeof(unsigned char), 5);
	file.read(&totalgems,		sizeof(unsigned char), 1);

	if (version > Player::CHRFV_020505)
	{
		file.read(&hunger,		sizeof(float), 1);
	}
	else
	{
		hunger = 100.0f;
	}

	// annoying bug...
	if (version <= Player::CHRFV_030506)
	{
		race = CR_HUMAN;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	Writes the stats to a file.
// Parameters:	file:		The current file, must have been opened before call.
//				version:	The character file version that is being written.
// Returns:		Success or failure.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool SBaseStats::write(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	// attributes and abilities
	int i;
	for (i = 0; i < NUM_ATTRIBUTES; i++)
	{
		attributes[i].write(file, version);
	}
	for (i = 0; i < NUM_VARATTRIBUTES; i++)
	{
		variable[i].write(file, version);
	}
	for (i = 0; i < NUM_ABILITIES; i++)
	{
		abilities[i].write(file, version);
	}

	// basic character information
	file.write(&pclass,			sizeof(unsigned short), 1);
	file.write(&level,			sizeof(unsigned short), 1);
	file.write(&experience,		sizeof(unsigned int), 1);
	file.write(&gender,			sizeof(unsigned char), 1);
	file.write(&soulcolor,		sizeof(unsigned char), 1);
	file.write(&god,			sizeof(unsigned char), 1);
	file.write(&race,			sizeof(unsigned char), 1);

	// attribute points
	file.write(&newpoints,		sizeof(unsigned short), 1);
	file.write(&totalpoints,	sizeof(unsigned short), 1);

	// proficiency gems
	file.write(freegems,		sizeof(unsigned char), 5);
	file.write(&totalgems,		sizeof(unsigned char), 1);

	// other
	file.write(&hunger,			sizeof(float), 1);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

void CSmallStats::setStats(SBaseStats& stats, CCharacterInventory* inventory)
{
	{
		for (int i(0); i < NUM_ATTRIBUTES; i++)
		{
			ua_attributes[i] = stats.attributes[i].getValue();
		}
	}
	{
		for (int i(0); i < NUM_VARATTRIBUTES; i++)
		{
			ua_varcurr[i] = stats.variable[i].getCurrent();
			ua_varmax[i] = stats.variable[i].getValue();
		}
	}
	{
		for (int i(0); i < NUM_ABILITIES; i++)
		{
			ua_abilities[i] = stats.abilities[i].getValue();
		}
	}

	u_hunger = stats.hunger;

	u_pclass = stats.pclass;
	u_level = stats.level;
	//u_experience = stats.experience;
// I don't want to be forced to include CExperience.cpp in the client build because somebody might
// reverse engineer the .exe to get the experience table
#ifdef AKARRASERVER
	u_expprogresstype = CExperienceMath::getProgressType(u_level);
	u_expprogress = CExperienceMath::getProgressPosition(u_level, stats.experience);
#endif
	u_gender = stats.gender;
	u_soulcolor = stats.soulcolor;
	u_god = stats.god;
	u_race = stats.race;

	u_newpoints = stats.newpoints;
	u_totalpoints = stats.totalpoints;

	{
		for (int i(0); i < 5; i++)
		{
			ua_freegems[i] = stats.freegems[i];
		}
	}
	u_totalgems = stats.totalgems;

	u_itemdisabled = 0;
	if (inventory)
	{
		for (int i(0); i < (int)NUM_EQUIPMENT_SLOTS; i++)
		{
			if (!inventory->upa_equipment[i])
			{
				continue;
			}
			if (!inventory->upa_equipment[i]->isEnabled())
			{
				u_itemdisabled |= (1 << i);
			}
		}
	}

}

} // end of RpgSystem namespace
