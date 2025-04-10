
#include "CClientSkill.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/FileSystem.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientSkillType::CClientSkillType()
{
	u_skillid = 0;
	u_skillversion = 0;
	u_skillicon = 0;

	ua_name[0] = 0;
	ua_desc[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientSkillType::~CClientSkillType()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CClientSkillType::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&u_skillid, sizeof(int), 1);
	file.read(&u_skillversion, sizeof(int), 1);
	file.read(&u_skillicon, sizeof(int), 1);
	file.read(&u_skillvariant, sizeof(int), 1);
	file.read(ua_name, sizeof(char), 32);
	if (version > CSTFV_030211)
	{
		file.read(ua_desc, sizeof(char), 512);
	}
	else
	{
		file.read(ua_desc, sizeof(char), 256);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CClientSkillType::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = CSTFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(&u_skillid, sizeof(int), 1);
	file.write(&u_skillversion, sizeof(int), 1);
	file.write(&u_skillicon, sizeof(int), 1);
	file.write(&u_skillvariant, sizeof(int), 1);
	file.write(ua_name, sizeof(char), 32);
	file.write(ua_desc, sizeof(char), 512);

	return true;
}

// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientSkillBundle::CClientSkillBundle()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientSkillBundle::~CClientSkillBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientSkillBundle::m_clear()
{
	m_skills.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientSkillBundle::removeAll()
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
bool CClientSkillBundle::open(char* filename)
{
	// attempt to open the file
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "rb");

	if (!file.isOk())
	{
		return false;
	}

	// clear old data
	removeAll();

	// read version
	int version;
	file.read(&version, sizeof(int), 1);

	// get skills
	{
		int count;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			CClientSkillType* skill = new CClientSkillType;
			if (!skill->read(file))
			{
				delete skill;

				FileSystem::directClose(file);
				return false;
			}
			m_skills.push_back(skill);
		}
	}

	FileSystem::directClose(file);
	return true;

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CClientSkillBundle::save(char* filename)
{
	// attempt to open the file
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");

	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = CSBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write skills
	{
		int count = m_skills.size();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_skills[i]->write(file))
			{
				FileSystem::directClose(file);
				return false;
			}
		}
	}

	FileSystem::directClose(file);
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientSkillType* CClientSkillBundle::getSkill(int id)
{
	// find a skill with a matching id
	for (int i(0); i < m_skills.size(); i++)
	{
		if (m_skills[i]->u_skillid == id)
		{
			return m_skills[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientSkillBundle::updateSkill(int id, int version, int icon, int variant, char* name, char* desc)
{
	// check if we have the skill
	CClientSkillType* skill = getSkill(id);

	if (skill)
	{
		skill->u_skillversion = version;
		skill->u_skillicon = icon;
		skill->u_skillvariant = variant;
		strcpy(skill->ua_name, name);
		strcpy(skill->ua_desc, desc);
	}
	else
	{
		// add the skill
		skill = new CClientSkillType;

		skill->u_skillid = id;
		skill->u_skillversion = version;
		skill->u_skillicon = icon;
		skill->u_skillvariant = variant;
		strcpy(skill->ua_name, name);
		strcpy(skill->ua_desc, desc);

		m_skills.push_back(skill);
	}
}

}
