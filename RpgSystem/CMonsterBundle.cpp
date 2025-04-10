/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    "Bundle" is my way of telling it's a list of several items, stored to a file.
 *    Since I didn't know how hash- or treemaps worked back when I wrote these bundles,
 *    I'm looping to find a requested data object.
 **/

#include "CMonsterBundle.h"
#include "CMonsterStats.h"
#include "../library/FileSystem/FileSystem.h"
#include "../library/FileSystem/CFileStream.h"

namespace RpgSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterTypeBundle::CMonsterTypeBundle()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterTypeBundle::~CMonsterTypeBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CMonsterTypeBundle::m_clear()
{
	m_types.clear();
	m_families.clear();

	m_nextid = 1;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CMonsterTypeBundle::removeAll()
{
	{
		for (int i(0); i < m_types.size(); i++)
		{
			delete m_types[i];
		}
	}
	{
		for (int i(0); i < m_families.size(); i++)
		{
			delete m_families[i];
		}
	}
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::read(FileSystem::CFileStream& file)
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

	// read item data
	{
		int count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			CMonsterType* item = new CMonsterType;
			if (!item->read(file))
			{
				delete item;
				return false;
			}

			// set runtime id number
			item->u_runtimeid = m_nextid++;

			// insert monster
			m_types.push_back(item);
		}
	}

	// family data
	{
		int count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			CMonsterFamily* family = new CMonsterFamily;
			if (!family->read(file))
			{
				delete family;
				return false;
			}

			// insert skill
			m_families.push_back(family);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = MBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write item data
	{
		int count = getNumMonsterTypes();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_types[i]->write(file))
			{
				return false;
			}
		}
	}

	// family data
	{
		int count = getNumFamilies();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_families[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::open(char* filename)
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
bool CMonsterTypeBundle::save(char* filename)
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
TDMonsterTypeVec& CMonsterTypeBundle::getMonsterTypes()
{
	return m_types;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CMonsterTypeBundle::getNumMonsterTypes()
{
	return (int)m_types.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterType* CMonsterTypeBundle::getMonsterType(char* dataname)
{
	// attempt to find the item
	for (int i(0); i < getNumMonsterTypes(); i++)
	{
		if (stricmp(m_types[i]->ua_dataname, dataname) == 0)
		{
			return m_types[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterType* CMonsterTypeBundle::createNewMonsterType()
{
	CMonsterType* item = new CMonsterType;

	// set runtime id
	item->u_runtimeid = m_nextid++;

	m_types.push_back(item);

	return item;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::addMonsterType(CMonsterType* item)
{
	if (!item)
	{
		return false;
	}

	// error-check name
	if (getMonsterType(item->ua_dataname))
	{
		// already exists
		return false;
	}

	// set runtime id
	item->u_runtimeid = m_nextid++;

	m_types.push_back(item);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Returns true if the item was successfully removed
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::deleteMonsterType(char* name)
{
	if (!name)
	{
		return false;
	}

	// find the item
	TDMonsterTypeVec::iterator item = m_types.begin();
	for (item; item != m_types.end(); ++item)
	{
		if (stricmp((*item)->ua_dataname, name) == 0)
		{
			// item found, remove it
			delete (*item);
			m_types.erase(item);

			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDMonsterFamilyVec& CMonsterTypeBundle::getFamilies()
{
	return m_families;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CMonsterTypeBundle::getNumFamilies()
{
	return (int)m_families.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterFamily* CMonsterTypeBundle::getFamily(char* dataname)
{
	// attempt to find the family
	for (int i(0); i < getNumFamilies(); i++)
	{
		if (stricmp(m_families[i]->ua_name, dataname) == 0)
		{
			return m_families[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterFamily* CMonsterTypeBundle::getFamily(int index)
{
	if (index < 0 || index >= getNumFamilies())
	{
		return NULL;
	}

	return m_families[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::addFamily(CMonsterFamily* item)
{
	if (!item)
	{
		return false;
	}

	// error-check name
	if (getFamily(item->ua_name))
	{
		// already exists
		return false;
	}

	m_families.push_back(item);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterTypeBundle::deleteFamily(char* name)
{
	if (!name)
	{
		return false;
	}

	// find the item
	TDMonsterFamilyVec::iterator item = m_families.begin();
	for (item; item != m_families.end(); ++item)
	{
		if (stricmp((*item)->ua_name, name) == 0)
		{
			// item found, remove it
			delete (*item);
			m_families.erase(item);

			return true;
		}
	}

	return false;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterFamily::CMonsterFamily()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CMonsterFamily::~CMonsterFamily()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CMonsterFamily::clear()
{
	ua_name[0] = 0;

	// setup default balance:
	ua_baseabs[ABILITY_ATTACK_RATING] = 10.0f;
	ua_multiabs[ABILITY_ATTACK_RATING] = 2.0f;
	ua_baseabs[ABILITY_CAST_RATING] = 5.0f;
	ua_multiabs[ABILITY_CAST_RATING] = 2.0f;
	ua_baseabs[ABILITY_DAMAGE_MIN] = 1.0f * 1.25f;
	ua_multiabs[ABILITY_DAMAGE_MIN] = 0.22f * 1.25f;
	ua_baseabs[ABILITY_DAMAGE_MAX] = 2.0f * 1.25f;
	ua_multiabs[ABILITY_DAMAGE_MAX] = 0.55f * 1.25f;
	ua_baseabs[ABILITY_ARMORCLASS] = 5.0f;
	ua_multiabs[ABILITY_ARMORCLASS] = 1.5f;
	ua_baseabs[ABILITY_RESIST_WHITE] = 2.0f;
	ua_multiabs[ABILITY_RESIST_WHITE] = 1.0f;
	ua_baseabs[ABILITY_RESIST_RED] = 2.0f;
	ua_multiabs[ABILITY_RESIST_RED] = 1.0f;
	ua_baseabs[ABILITY_RESIST_GREEN] = 2.0f;
	ua_multiabs[ABILITY_RESIST_GREEN] = 1.0f;
	ua_baseabs[ABILITY_RESIST_BLUE] = 2.0f;
	ua_multiabs[ABILITY_RESIST_BLUE] = 1.0f;
	ua_baseabs[ABILITY_RESIST_YELLOW] = 2.0f;
	ua_multiabs[ABILITY_RESIST_YELLOW] = 1.0f;
	ua_baseabs[ABILITY_REDUCTION] = 0.5f;
	ua_multiabs[ABILITY_REDUCTION] = 0.1f;

	u_basehp = -10.0f;
	u_multihp = 8.0f;

	u_expmultiplier = 1.0f;

	u_designnotes = "";
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterFamily::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	clear();

	// version...
	int version = 0;
	file.read(&version, sizeof(int), 1);

	// data...
	file.read(ua_name, sizeof(char), 32);
	if (version > MFFV_030401)
	{
		{
			for (int i(0); i < NUM_ABILITIES; i++)
			{
				file.read(&ua_baseabs[i], sizeof(float), 1);
				file.read(&ua_multiabs[i], sizeof(float), 1);
			}
		}
		file.read(&u_basehp, sizeof(float), 1);
		file.read(&u_multihp, sizeof(float), 1);
		file.read(&u_expmultiplier, sizeof(float), 1);

		u_designnotes = FileSystem::readStringWithLength(file);
	}

	if (version <= MFFV_030513)
	{
		ua_baseabs[ABILITY_DAMAGE_MIN] *= 1.25f;
		ua_multiabs[ABILITY_DAMAGE_MIN] *= 1.25f;
		ua_baseabs[ABILITY_DAMAGE_MAX] *= 1.25f;
		ua_multiabs[ABILITY_DAMAGE_MAX] *= 1.25f;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CMonsterFamily::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// version...
	int version = MFFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// data...
	file.write(ua_name, sizeof(char), 32);
	{
		for (int i(0); i < NUM_ABILITIES; i++)
		{
			file.write(&ua_baseabs[i], sizeof(float), 1);
			file.write(&ua_multiabs[i], sizeof(float), 1);
		}
	}
	file.write(&u_basehp, sizeof(float), 1);
	file.write(&u_multihp, sizeof(float), 1);
	file.write(&u_expmultiplier, sizeof(float), 1);

	FileSystem::writeStringWithLength(file, u_designnotes);

	return true;
}

}
