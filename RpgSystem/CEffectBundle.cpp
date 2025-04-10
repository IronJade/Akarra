/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    "Bundle" is my way of telling it's a list of several items, stored to a file.
 *    Since I didn't know how hash- or treemaps worked back when I wrote these bundles,
 *    I'm looping to find a requested data object.
 **/

#include "CEffectBundle.h"
#include "CEffect.h"
#include "../library/FileSystem/FileSystem.h"
#include "../library/FileSystem/CFileStream.h"

namespace RpgSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEffectTypeBundle::CEffectTypeBundle()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEffectTypeBundle::~CEffectTypeBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEffectTypeBundle::m_clear()
{
	m_types.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEffectTypeBundle::removeAll()
{
	{
		for (int i(0); i < m_types.size(); i++)
		{
			delete m_types[i];
		}
	}
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectTypeBundle::read(FileSystem::CFileStream& file)
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
			CEffectType* item = new CEffectType;
			if (!item->read(file))
			{
				delete item;
				return false;
			}

			// insert skill
			m_types.push_back(item);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectTypeBundle::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = EBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write item data
	{
		int count = getNumEffectTypes();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_types[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectTypeBundle::open(char* filename)
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
bool CEffectTypeBundle::save(char* filename)
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
TDEffectTypeVec& CEffectTypeBundle::getEffectTypes()
{
	return m_types;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CEffectTypeBundle::getNumEffectTypes()
{
	return (int)m_types.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEffectType* CEffectTypeBundle::getEffectType(char* dataname)
{
	// attempt to find the item
	for (int i(0); i < getNumEffectTypes(); i++)
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
CEffectType* CEffectTypeBundle::createNewEffectType()
{
	CEffectType* item = new CEffectType;
	m_types.push_back(item);

	return item;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectTypeBundle::addEffectType(CEffectType* item)
{
	if (!item)
	{
		return false;
	}

	// error-check name
	if (getEffectType(item->ua_dataname))
	{
		// already exists
		return false;
	}

	m_types.push_back(item);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Returns true if the item was successfully removed
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectTypeBundle::deleteEffectType(char* name)
{
	if (!name)
	{
		return false;
	}

	// find the item
	TDEffectTypeVec::iterator item = m_types.begin();
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

}
