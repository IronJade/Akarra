/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Skills, as edited in the skills editor.
 **/

#include "CSkill.h"
#include "CClass.h"
#include "CCharacterAttributes.h"
#include "SRequirement.h"
#include "../PlayerSystem/AkarraCharacter.h"
#include "CEffect.h"
#include "../library/filesystem/FileSystem.h"

// hack solution
#ifdef AKARRASERVER

// server-side
#include "../../Akarra Server/AkarraServerManager.h"
#include "../../Akarra Server/AkarraServerMain.h"

#else

#ifdef AKARRACLIENT
// client-side
#include "CClientSkill.h"
#include "../../Akarra/ClientPlayer/CPlayer.h"

using namespace Akarra;

#endif

#endif
// !hack

namespace RpgSystem
{

char* getGemColorName(int gem)
{
	static const char* const GEM_NAMES[NUM_GEM_SLOTS] =
	{
		"White",
		"Red",
		"Green",
		"Blue",
		"Yellow"
	};

	return (char*)GEM_NAMES[gem];
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillType2::CSkillType2()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillType2::~CSkillType2()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::m_clear()
{
	m_version = 0;
	m_skillid = 0;
	ma_skillname[0] = 0;
	ma_skilldesc[0] = 0;
	m_varianttype = SV_TALENT;
	m_icon = 0;
	m_modified = false;
	m_flags = 0;
	m_reqattribute = 0;
	ma_effectnamebase[0] = 0;

	{
		for (int i(0); i < NUM_SKILL_SLOTS; i++)
		{
			//ma_effectnames[i][0] = 0;
			mpa_runtime[i] = NULL;
			ma_requirements[i] = 0;
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillType2::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&m_version, sizeof(int), 1);
	file.read(&m_skillid, sizeof(int), 1);
	file.read(&m_varianttype, sizeof(int), 1);
	file.read(&m_icon, sizeof(int), 1);
	if (version > STFV_030326)
	{
		file.read(&m_flags, sizeof(int), 1);
	}

	file.read(ma_skillname, sizeof(char), 32);

	if (version > STFV_030204)
	{
		file.read(ma_skilldesc, sizeof(char), 512);

		if (version <= STFV_030508)
		{
			char placeholders[25][32];
			for (int i(0); i < 25; i++)
			{
				file.read(placeholders[i], sizeof(char), 32);
			}
			strcpy(ma_effectnamebase, placeholders[0]);
		}
		else if (version <= STFV_030509)
		{
			char placeholders[50][32];
			for (int i(0); i < 50; i++)
			{
				file.read(placeholders[i], sizeof(char), 32);
			}
			strcpy(ma_effectnamebase, placeholders[0]);
		}
		else
		{
			file.read(ma_effectnamebase, sizeof(char), 32);
		}
	}
	else
	{
		char placeholders[5][32];
		file.read(ma_skilldesc, sizeof(char), 256);
		// old version did only have 5 slots
		for (int i(0); i < NUM_GEM_SLOTS; i++)
		{
			file.read(placeholders[i], sizeof(char), 32);
		}
		strcpy(ma_effectnamebase, placeholders[0]);
	}

	if (version > STFV_030509)
	{
		file.read(&m_reqattribute, sizeof(int), 1);
		for (int i(0); i < NUM_SKILL_SLOTS; i++)
		{
			file.read(&ma_requirements[i], sizeof(int), 1);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillType2::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = STFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	version = (m_modified) ? m_version + 1 : m_version;

	file.write(&version, sizeof(int), 1);
	file.write(&m_skillid, sizeof(int), 1);
	file.write(&m_varianttype, sizeof(int), 1);
	file.write(&m_icon, sizeof(int), 1);
	file.write(&m_flags, sizeof(int), 1);

	file.write(ma_skillname, sizeof(char), 32);
	file.write(ma_skilldesc, sizeof(char), 512);

	//{
	//	for (int i(0); i < NUM_SKILL_SLOTS; i++)
	//	{
	//		file.write(ma_effectnames[i], sizeof(char), 32);
	//	}
	//}
	file.write(ma_effectnamebase, sizeof(char), 32);
	{
		file.write(&m_reqattribute, sizeof(int), 1);
		for (int i(0); i < NUM_SKILL_SLOTS; i++)
		{
			file.write(&ma_requirements[i], sizeof(int), 1);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType2::getVersion()
{
	return m_version;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType2::getId()
{
	return m_skillid;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setId(int id)
{
	m_skillid = id;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSkillType2::getName()
{
	return ma_skillname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setName(char* name)
{
	strncpy(ma_skillname, name, 32 - 1);
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSkillType2::getDesc()
{
	return ma_skilldesc;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setDesc(char* desc)
{
	strncpy(ma_skilldesc, desc, 512 - 1);
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType2::getIcon()
{
	return m_icon;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setIcon(int icon)
{
	m_icon = icon;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillType2::getFlag(int flag)
{
	return ((m_flags & flag) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setFlag(int flag, bool setting)
{
	if (setting)
	{
		m_flags |= flag;
	}
	else
	{
		m_flags &= ~flag;
	}
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType2::getVariantType()
{
	return m_varianttype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setVariantType(int type)
{
	m_varianttype = type;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
Basic::CCharString CSkillType2::getEffectName(int slot)
{
	Basic::CCharString name(ma_effectnamebase);
	name += " ";
	name += (slot / NUM_GEMS_ROW) + 1;
	name += "x";
	name += (slot % NUM_GEMS_ROW);

	return Basic::CCharString(name);
//	if (slot < 0 || slot >= NUM_SKILL_SLOTS)
//	{
//		return "";
//	}
//
//	return ma_effectnames[slot];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSkillType2::getEffectBaseName()
{
	return ma_effectnamebase;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//void CSkillType2::setEffectName(int slot, char* effect)
void CSkillType2::setEffectName(char* name)
{
	//if (slot < 0 || slot >= NUM_SKILL_SLOTS)
	//{
	//	return;
	//}

	//strncpy(ma_effectnames[slot], effect, 32 - 1);
	strncpy(ma_effectnamebase, name, 32 - 1);
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setRunTimeEffect(int slot, CEffectType* effect)
{
	if (slot < 0 || slot >= NUM_SKILL_SLOTS)
	{
		return;
	}

	mpa_runtime[slot] = effect;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEffectType* CSkillType2::getRunTimeEffect(int slot)
{
	if (slot < 0 || slot >= NUM_SKILL_SLOTS)
	{
		return NULL;
	}

	return mpa_runtime[slot];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType2::getRequirementAttribute()
{
	return m_reqattribute;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setRequirementAttribute(int attr)
{
	if (attr < 0 || attr > 4)
	{
		return;
	}
	m_reqattribute = attr;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType2::getRequirement(int slots)
{
	if (slots < 0)
	{
		ma_requirements[0];
	}
	if (slots >= NUM_SKILL_SLOTS)
	{
		ma_requirements[NUM_SKILL_SLOTS-1];
	}
	return ma_requirements[slots];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillType2::setRequirement(int slots, int requirement)
{
	if (slots < 0 || slots >= NUM_SKILL_SLOTS)
	{
		return;
	}
	ma_requirements[slots] = requirement;
	m_modified = true;
}


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillTypeBundle::CSkillTypeBundle()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillTypeBundle::~CSkillTypeBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillTypeBundle::m_clear()
{
	m_skills.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkillTypeBundle::removeAll()
{
	{
		for (int i(0); i < m_skills.size(); i++)
		{
			delete m_skills[i];
		}
	}
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillTypeBundle::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// clear old data
	removeAll();

	// read version
	int version;
	file.read(&version, sizeof(int), 1);

	// read skill data
	{
		int count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			CSkillType2* skill = new CSkillType2;
			if (!skill->read(file))
			{
				delete skill;
				return false;
			}

			// insert skill
			m_skills.push_back(skill);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillTypeBundle::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = STBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write skill data
	{
		int count = getNumSkills();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_skills[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillTypeBundle::open(char* filename)
{
	if (!filename)
	{
		return false;
	}

	// attempt to open the file
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "rb");

	if (!file.isOk())
	{
		return false;
	}

	// read skill data
	bool success = read(file);

	FileSystem::directClose(file);

	return success;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillTypeBundle::save(char* filename)
{
	if (!filename)
	{
		return false;
	}

	// attempt to save the file
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");

	if (!file.isOk())
	{
		return false;
	}

	// write skill data
	bool success = write(file);

	FileSystem::directClose(file);

	return success;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDSkillTypeVec& CSkillTypeBundle::getSkills()
{
	return m_skills;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillTypeBundle::getNumSkills()
{
	return (int)m_skills.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillType2* CSkillTypeBundle::getSkill(char* skillname)
{
	// attempt to find the skill
	for (int i(0); i < getNumSkills(); i++)
	{
		if (stricmp(m_skills[i]->getName(), skillname) == 0)
		{
			return m_skills[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillType2* CSkillTypeBundle::getSkill(int id)
{
	// attempt to find the skill
	for (int i(0); i < getNumSkills(); i++)
	{
		if (m_skills[i]->getId() == id)
		{
			return m_skills[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillType2* CSkillTypeBundle::createNewSkill()
{
	CSkillType2* skill = new CSkillType2;
	m_skills.push_back(skill);

	return skill;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillTypeBundle::addSkill(CSkillType2* skill)
{
	if (!skill)
	{
		return false;
	}

	// error-check name and id number
	if (getSkill(skill->getName()))
	{
		// already exists
		return false;
	}
	if (getSkill(skill->getId()))
	{
		// already exists
		return false;
	}

	m_skills.push_back(skill);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkill::CSkill()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkill::~CSkill()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkill::clear()
{
	{
		for (int i(0); i < NUM_SKILL_SLOTS; i++)
		{
			ma_gems[i] = 0;
		}
	}
	{
		for (int i(0); i < NUM_SKILL_ROWS; i++)
		{
			ma_rows[i] = 0;
		}
	}

	m_skill = 0;
	m_flags = 0;
	m_changedelay = 0;
	mp_skill = NULL;

	m_numrows = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::read(FileSystem::CFileStream& file, int version)
{
	clear();

	if (!file.isOk())
	{
		return false;
	}

	if (version <= Player::CHRFV_020818)
	{
		// there were no skills in this version
		return true;
	}

	file.read(&m_skill, sizeof(int), 1);
	file.read(&m_flags, sizeof(unsigned int), 1);
	file.read(&m_changedelay, sizeof(int), 1);

	if (version > Player::CHRFV_030204)
	{
		int i;
		if (version <= Player::CHRFV_030508)
		{
			for (i = 0; i < (NUM_SKILL_SLOTS / 2); i++)
			{
				file.read(&ma_gems[i], sizeof(unsigned char), 1);
			}
		}
		else
		{
			for (i = 0; i < NUM_SKILL_SLOTS; i++)
			{
				file.read(&ma_gems[i], sizeof(unsigned char), 1);
			}
		}

		for (i = 0; i < NUM_SKILL_ROWS; i++)
		{
			file.read(&ma_rows[i], sizeof(unsigned char), 1);
		}
	}
	else
	{
		for (int i(0); i < 5; i++)
		{
			file.read(&ma_gems[i], sizeof(unsigned char), 1);
		}

		// this skill has the first row set
		ma_rows[0] = 1;
	}

	m_numrows = getNumRows();

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	file.write(&m_skill, sizeof(int), 1);
	file.write(&m_flags, sizeof(unsigned int), 1);
	file.write(&m_changedelay, sizeof(int), 1);

	{
		for (int i(0); i < NUM_SKILL_SLOTS; i++)
		{
			file.write(&ma_gems[i], sizeof(unsigned char), 1);
		}
	}
	{
		for (int i(0); i < NUM_SKILL_ROWS; i++)
		{
			file.write(&ma_rows[i], sizeof(unsigned char), 1);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkill::updateTimer()
{
	if (m_changedelay > 0)
	{
		m_changedelay--;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkill::getDelay()
{
	return m_changedelay;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkill::setDelay(int seconds)
{
	m_changedelay = seconds;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSkillType2* CSkill::getSkillType()
{
	return mp_skill;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkill::getSkillId()
{
	return m_skill;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//void CSkill::setSkillType(int skill)
//{
//	m_skill = skill;
//}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkill::setNewSkill(CSkillType2* skill, int startrow)
{
	if (!skill)
	{
		return;
	}

	clear();

	mp_skill = skill;
	m_skill = skill->getId();
	m_changedelay = 0;
	m_flags = 0;

	ma_rows[startrow] = 1;
	m_numrows = getNumRows();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkill::setSkill(CSkillType2* skill)
{
	if (!skill)
	{
		return;
	}

	mp_skill = skill;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSkill::getSkillName()
{
	//return CSkillType::getSkillName(m_skill);
	if (!mp_skill)
	{
		return "NULL";
	}

	return mp_skill->getName();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//char* CSkill::getEffectName()
//{
//	return CSkillType::getSkillEffectName(m_skill, getNumStones());
//}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEffectType* CSkill::getEffectType()
{
	//return mp_effecttype;

	if (!mp_skill)
	{
		return NULL;
	}

	return mp_skill->getRunTimeEffect(getNumStones() - 1);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//void CSkill::setEffectType(CEffectType* effect)
//{
//	mp_effecttype = effect;
//}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSkill::setSpecialized(bool set)
{
	if (set)
	{
		m_flags |= SKILLFLAG_SPECIALIZED;
	}
	else
	{
		m_flags &= ~SKILLFLAG_SPECIALIZED;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::isSpecialized()
{
	return ((m_flags & SKILLFLAG_SPECIALIZED) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::addStone(int slot)
{
	if (slot < 0 || slot >= (NUM_GEMS_ROW * m_numrows))
	{
		return false;
	}

	// if we're specialized we can't change the stones
	if (isSpecialized())
	{
		return false;
	}

	// if the change delay timer still is running we can't change either...
	//if (m_changedelay > 0)
	//{
	//	return false;
	//}

	// if the slot already is taken we can't change...
	if (ma_gems[slot])
	{
		return false;
	}

	// finally! =)
	ma_gems[slot] = 1;

	// since the effect from this skill now is undefined, we must reset it
	//mp_effecttype = NULL;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::removeStone(int slot)
{
	if (slot < 0 || slot >= (NUM_GEMS_ROW * m_numrows))
	{
		return false;
	}

	// if we're specialized we can't change the stones
	if (isSpecialized())
	{
		return false;
	}

	// if the change delay timer still is running we can't change either...
	if (mp_skill && !mp_skill->getFlag(STFLAG_NOLOCK) && m_changedelay > 0)
	{
		return false;
	}

	// if the there isn't a gem in this slot we can't remove it
	if (!ma_gems[slot])
	{
		return false;
	}

	// finally! =)
	ma_gems[slot] = 0;
	// since the effect from this skill now is undefined, we must reset it
	//mp_effecttype = NULL;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::hasStone(int slot)
{
	if (slot < 0 || slot >= (NUM_GEMS_ROW * m_numrows))
	{
		return false;
	}

	return (ma_gems[slot] != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkill::getNumStones()
{
	// count number of gems
	int count(0);
	for (int i(0); i < (NUM_GEMS_ROW * m_numrows); i++)
	{
		if (ma_gems[i])
		{
			count++;
		}
	}
	
	return count;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::addRow(int row)
{
	if (row < 0 || row >= NUM_SKILL_ROWS)
	{
		return false;
	}

	if (hasRow(row))
	{
		// row has already been enabled
		return false;
	}

	// enable row
	ma_rows[row] = 1;
	m_numrows = getNumRows();

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkill::hasRow(int row)
{
	if (row < 0 || row >= NUM_SKILL_ROWS)
	{
		return false;
	}

	return (ma_rows[row] != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkill::getNumRows()
{
	int count(0);
	for (int i(0); i < NUM_SKILL_ROWS; i++)
	{
		if (ma_rows[i] != 0)
		{
			count++;
		}
	}

	return count;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
/*
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const char* const SKILL_NAMES[NUM_SKILLS] =
{
	"Fighting I",
	"Weapon Mastery I",
	"Dodge I",
	"Axes I",
	"Swords I",
	"Shield Use I",
	"Venom Resistance",
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const char* const SKILL_DESCRIPTIONS[NUM_SKILLS] =
{
	"§n§3Fighting I§n§n"
	"§7This talent improves your ability to use weapons in combat.§n§n"
	"§41 Proficiency Gem§7 - Adds 3 to Attack Rating§n"
	"§42 Proficiency Gem§7 - Adds 6 to Attack Rating§n"
	"§43 Proficiency Gem§7 - Adds 9 to Attack Rating§n"
	"§44 Proficiency Gem§7 - Adds 12 to Attack Rating§n"
	"§45 Proficiency Gem§7 - Adds 15 to Attack Rating§n",

	"§n§3Weapon Mastery I§n§n"
	"§7This talent improves your ability to use weapons in combat.§n§n"
	"§41 Proficiency Gem§7 - Adds 1 to Max Damage§n"
	"§42 Proficiency Gem§7 - Adds 2 to Max Damage§n"
	"§43 Proficiency Gem§7 - Adds 3 to Max Damage§n"
	"§44 Proficiency Gem§7 - Adds 4 to Max Damage§n"
	"§45 Proficiency Gem§7 - Adds 5 to Max Damage§n",

	"§n§3Dodge I§n§n"
	"§7This talent makes your more nimble and dexterious, helping you to avoid "
	"blows in combat.§n§n"
	"§41 Proficiency Gem§7 - Adds 1 to Armor Class§n"
	"§42 Proficiency Gem§7 - Adds 2.5 to Armor Class§n"
	"§43 Proficiency Gem§7 - Adds 4 to Armor Class§n"
	"§44 Proficiency Gem§7 - Adds 5.5 to Armor Class§n"
	"§45 Proficiency Gem§7 - Adds 7 to Armor Class§n",

	"§n§3Axes I§n§n"
	"§7This talent improves your ability to use axes in combat.§n§n"
	"§41 Proficiency Gem§7 - Adds 2 to Attack Rating while wielding an axe§n"
	"§42 Proficiency Gem§7 - Adds 6 to Attack Rating while wielding an axe§n"
	"§43 Proficiency Gem§7 - Adds 10 to Attack Rating while wielding an axe§n"
	"§44 Proficiency Gem§7 - Adds 14 to Attack Rating while wielding an axe§n"
	"§45 Proficiency Gem§7 - Adds 18 to Attack Rating while wielding an axe§n",

	"§n§3Swords I§n§n"
	"§7This talent improves your ability to use swords in combat.§n§n"
	"§41 Proficiency Gem§7 - Adds 2 to Attack Rating while wielding a sword§n"
	"§42 Proficiency Gem§7 - Adds 6 to Attack Rating while wielding a sword§n"
	"§43 Proficiency Gem§7 - Adds 10 to Attack Rating while wielding a sword§n"
	"§44 Proficiency Gem§7 - Adds 14 to Attack Rating while wielding a sword§n"
	"§45 Proficiency Gem§7 - Adds 18 to Attack Rating while wielding a sword§n",

	"§n§3Shield Use I§n§n"
	"§7This talent improves your ability to use shields.§n§n"
	"§41 Proficiency Gem§7 - Adds 2 to Armor Class while using a shield§n"
	"§42 Proficiency Gem§7 - Adds 4 to Armor Class while using a shield§n"
	"§43 Proficiency Gem§7 - Adds 6 to Armor Class while using a shield§n"
	"§44 Proficiency Gem§7 - Adds 8 to Armor Class while using a shield§n"
	"§45 Proficiency Gem§7 - Adds 10 to Armor Class while using a shield§n",

	"§n§3Venom Resistance§n§n"
	"§7With this talent you are less prone to venomous attacks and decay spells.§n§n"
	"§41 Proficiency Gem§7 - Adds 2 to Yellow Resistance§n"
	"§42 Proficiency Gem§7 - Adds 4 to Yellow Resistance§n"
	"§43 Proficiency Gem§7 - Adds 6 to Yellow Resistance§n"
	"§44 Proficiency Gem§7 - Adds 8 to Yellow Resistance§n"
	"§45 Proficiency Gem§7 - Adds 10 to Yellow Resistance§n",
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const SRequirement SKILL_REQS[NUM_SKILLS][3] =
{
	// "Fighting I"
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Weapon Mastery I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Dodge I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Axe I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Sword I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Shield Use I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Venom Resistance",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const SRequirement SKILL_SPEC_REQS[NUM_SKILLS][3] =
{
	// "Fighting I"
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Weapon Mastery I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Dodge I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Axe I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Sword I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Shield Use I",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
	// "Venom Resistance",
	{
		{RT_DISABLED, 0}, {RT_DISABLED, 0}, {RT_DISABLED, 0}
	},
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const char* const SKILL_EFFECTS[NUM_SKILLS][NUM_SLOTS] =
{
	// "Fighting I"
	{
		"Fighting IxI", "Fighting IxII", "Fighting IxIII", "Fighting IxIV", "Fighting IxV"
	},
	// "Weapon Mastery I",
	{
		"Weapon Master IxI", "Weapon Master IxII", "Weapon Master IxIII", "Weapon Master IxIV", "Weapon Master IxV"
	},
	// "Dodge I",
	{
		"Dodge IxI", "Dodge IxII", "Dodge IxIII", "Dodge IxIV", "Dodge IxV"
	},
	// "Axe I",
	{
		"Axe IxI", "Axe IxII", "Axe IxIII", "Axe IxIV", "Axe IxV"
	},
	// "Sword I",
	{
		"Sword IxI", "Sword IxII", "Sword IxIII", "Sword IxIV", "Sword IxV"
	},
	// "Shield Use I",
	{
		"Shield Use IxI", "Shield Use IxII", "Shield Use IxIII", "Shield Use IxIV", "Shield Use IxV"
	},
	// "Venom Resistance",
	{
		"Venom Resistance xI", "Venom Resistance xII", "Venom Resistance xIII", "Venom Resistance xIV", "Venom Resistance xV"
	},
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const bool SKILL_ISSPELL[NUM_SKILLS] =
{
	// "Attack Skill I",
	false,
	// "Weapon Mastery I",
	false,
	// "Dodge I",
	false,
	// "Axe I",
	false,
	// "Sword I",
	false,
	// "Shield Use I",
	false,
	// "Venom Resistance",
	false,
};

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
*/


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This is a "hack solution"
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER

// server-side
char* CSkillType::getSkillName(int skillnumber)
{
	if (skillnumber < 0)
	{
		return "Unknown Skill";
	}

	CSkillType2* skill = g_app.manager.u_worldmanager.getSkillType(skillnumber);
	if (!skill)
	{
		return "Unknown Skill";
	}

	return skill->getName();
}

#else

#ifdef AKARRACLIENT

// client-side
char* CSkillType::getSkillName(int skillnumber)
{
	if (skillnumber < 0 || !gp_player || !gp_player->getPlayerWorld())
	{
		return "Unknown Skill";
	}

	// look for the skill
	CClientSkillType* skill = gp_player->getPlayerWorld()->getSkill(skillnumber);

	if (!skill)
	{
		return "Unknown Skill";
	}

	return skill->ua_name;
}

#else

// borland
char* CSkillType::getSkillName(int skillnumber)
{
	return "Unknown Skill";
}

#endif

#endif


/*
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSkillType::getSkillNumber(char* skillname)
{
	if (!skillname)
	{
		return -1;
	}

	for (int i(0); i < NUM_SKILLS; i++)
	{
		if (strcmp(skillname, SKILL_NAMES[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillType::canLearnSkill(int skill, CCharacterAttributes& attributes, SRequirement** failedreq)
{
	if (skill < 0 || skill >= NUM_SKILLS)
	{
		return false;
	}

	for (int i(0); i < 3; i++)
	{
		if (!testRequirement(attributes, (SRequirement)SKILL_REQS[skill][i]))
		{
			if (failedreq)
			{
				*failedreq = (SRequirement*)&SKILL_REQS[skill][i];
			}
			return false;
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillType::canSpecializeSkill(int skill, CCharacterAttributes& attributes, SRequirement** failedreq)
{
	if (skill < 0 || skill >= NUM_SKILLS)
	{
		return false;
	}

	for (int i(0); i < 3; i++)
	{
		if (!testRequirement(attributes, (SRequirement)SKILL_SPEC_REQS[skill][i]))
		{
			if (failedreq)
			{
				*failedreq = (SRequirement*)&SKILL_REQS[skill][i];
			}
			return false;
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSkillType::getSkillEffectName(int skill, int numgems)
{
	if (skill < 0 || skill >= NUM_SKILLS || numgems < 1 || numgems > NUM_SLOTS)
	{
		return "No Effect";
	}

	return (char*)SKILL_EFFECTS[skill][numgems - 1];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSkillType::isSpell(int skill)
{
	if (skill < 0 || skill >= NUM_SKILLS)
	{
		return false;
	}

	return SKILL_ISSPELL[skill];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSkillType::getSkillDescription(int skill)
{
	if (skill < 0 || skill >= NUM_SKILLS)
	{
		return "Unknown Skill";
	}

	return (char*)SKILL_DESCRIPTIONS[skill];
}
*/
} // end of RpgSystem namespace
