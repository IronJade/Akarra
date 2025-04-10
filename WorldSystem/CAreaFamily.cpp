/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class is only used by the map editor.
 **/

#include "CAreaFamily.h"
#include "CArea2.h"
#include "../library/FileSystem/FileSystem.h"
#include "../library/FileSystem/CFileStream.h"
#include "../../Akarra Server/ServerWorld/AreaEntities/CSpawnFile.h"

#ifndef AKARRASERVER
#include <stdlib.h>
#endif

namespace WorldSystem
{

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAreaFamily::CAreaFamily(int sectortype)
{
	clear();

	m_sectortype = sectortype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAreaFamily::~CAreaFamily()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamily::clear()
{
	m_sectortype = AT_WORLDCOMPRESSED;
	m_areas.clear();
	ma_name[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamily::removeAll()
{
	TDAreaVec::iterator area;
	for (area = m_areas.begin(); area != m_areas.end(); ++area)
	{
		delete (*area);
	}
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAreaFamily::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// clear old data
	removeAll();

	// read version...
	int version = 0;
	file.read(&version, sizeof(int), 1);

	// read name and type
	file.read(ma_name, sizeof(char), 32);
	file.read(&m_sectortype, sizeof(int), 1);
	
	// read number of areas in the file
	int count = 0;
	file.read(&count, sizeof(int), 1);

	// create areas
	{
		for (int i(0); i < count; i++)
		{
			CArea2Static* area = new CArea2Static;
			if (!area->read(file, false, version > AFFV_050708))
			{
				delete area;
				return false;
			}

			m_areas.push_back(area);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAreaFamily::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// version...
	int version = AFFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write family name and type
	file.write(ma_name, sizeof(char), 32);
	file.write(&m_sectortype, sizeof(int), 1);
	
	// write number of areas
	int count = getNumAreas();
	file.write(&count, sizeof(int), 1);

	// write areas
	{
		for (int i(0); i < count; i++)
		{
			m_areas[i]->createSaveData(m_sectortype);
			if (!m_areas[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDAreaVec& CAreaFamily::getAreas()
{
	return m_areas;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CArea2Static* CAreaFamily::getArea(int index)
{
	if (index < 0 || index >= m_areas.size())
	{
		return NULL;
	}
	return m_areas[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamily::deleteArea(int index)
{
	if (index < 0 || index >= m_areas.size())
	{
		return;
	}
	TDAreaVec::iterator area = m_areas.begin();
	std::advance(area, index);
	delete (*area);
	m_areas.erase(area);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamily::insertArea(CArea2Static* source)
{
	if (!source)
	{
		return;
	}
	// make a copy of the area because we don't want to modify pointers that don't belong
	// to us
	CArea2Static* area = new CArea2Static;
	area->copyArea(source, AR_NOTUSED);

	m_areas.push_back(area);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAreaFamily::getNumAreas()
{
	return (int)m_areas.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CAreaFamily::getFamilyName()
{
	return ma_name;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamily::setFamilyName(char* name)
{
	strncpy(ma_name, name, 31);
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSpawnFamily::CSpawnFamily()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSpawnFamily::~CSpawnFamily()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSpawnFamily::clear()
{
	ma_name[0] = 0;
	m_spacing = 10;
	m_spawnvec.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSpawnFamily::removeAll()
{
	for (int i(0); i < m_spawnvec.size(); i++)
	{
		delete m_spawnvec[i];
	}
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSpawnFamily::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	removeAll();

	// read version
	int version = 0;
	file.read(&version, sizeof(int), 1);

	file.read(ma_name, sizeof(char), 32);
	file.read(&m_spacing, sizeof(int), 1);

	// read spawns
	int count = 0;
	file.read(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			CSpawnFileInfo* spawn = new CSpawnFileInfo;
			if (!spawn->read(file))
			{
				delete spawn;
				return false;
			}
			m_spawnvec.push_back(spawn);				
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSpawnFamily::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = SFFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(ma_name, sizeof(char), 32);
	file.write(&m_spacing, sizeof(int), 1);

	// write spawns
	int count = getNumSpawns();
	file.write(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			if (!m_spawnvec[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDSpawnVec& CSpawnFamily::getSpawns()
{
	return m_spawnvec;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSpawnFamily::deleteSpawn(int index)
{
	if (index < 0 || index >= m_spawnvec.size())
	{
		return;
	}
	TDSpawnVec::iterator spawn = m_spawnvec.begin();
	std::advance(spawn, index);
	delete (*spawn);
	m_spawnvec.erase(spawn);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSpawnFamily::insertSpawn(CSpawnFileInfo* source)
{
	if (!source)
	{
		return;
	}

	CSpawnFileInfo* spawn = new CSpawnFileInfo;
	spawn->copySpawn(source);
	m_spawnvec.push_back(spawn);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSpawnFamily::getNumSpawns()
{
	return (int)m_spawnvec.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSpawnFileInfo* CSpawnFamily::getRandomSpawn()
{
	if (m_spawnvec.empty())
	{
		return NULL;
	}
	int count = m_spawnvec.size();

	return m_spawnvec[rand() % count];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSpawnFamily::getFamilyName()
{
	return ma_name;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSpawnFamily::setFamilyName(char* name)
{
	strncpy(ma_name, name, 31);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSpawnFamily::getSpacing()
{
	return m_spacing;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSpawnFamily::setSpacing(int value)
{
	m_spacing = value;
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAreaFamilyBundle::CAreaFamilyBundle()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAreaFamilyBundle::~CAreaFamilyBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamilyBundle::clear()
{
	m_families.clear();
	m_spawns.clear();
	m_sectortype = AT_WORLDCOMPRESSED;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamilyBundle::removeAll()
{
	TDAreaFamilyVec::iterator family;
	for (family = m_families.begin(); family != m_families.end(); ++family)
	{
		delete (*family);
	}

	TDSpawnFamilyVec::iterator spawn;
	for (spawn = m_spawns.begin(); spawn != m_spawns.end(); ++spawn)
	{
		delete (*spawn);
	}

	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAreaFamilyBundle::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// remove old data
	removeAll();

	// read version
	int version = 0;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&m_sectortype, sizeof(int), 1);

	// read families
	int count = 0;
	file.read(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			// create the family
			CAreaFamily* family = new CAreaFamily(m_sectortype);
			if (!family->read(file))
			{
				delete family;
				return false;
			}

			m_families.push_back(family);
		}
	}

	if (version > AFBFV_030331)
	{
		count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			// create the family
			CSpawnFamily* family = new CSpawnFamily;
			if (!family->read(file))
			{
				delete family;
				return false;
			}

			m_spawns.push_back(family);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAreaFamilyBundle::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = AFBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(&m_sectortype, sizeof(int), 1);

	// write families
	int count = getNumFamilies();
	file.write(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			if (!m_families[i]->write(file))
			{
				return false;
			}
		}
	}

	count = getNumSpawns();
	file.write(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			if (!m_spawns[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAreaFamilyBundle::open(char* filename)
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

	bool success = read(file);
	FileSystem::directClose(file);

	return success;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CAreaFamilyBundle::save(char* filename)
{
	if (!filename)
	{
		return false;
	}

	// attempt to open the file
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");
	if (!file.isOk())
	{
		return false;
	}

	bool success = write(file);
	FileSystem::directClose(file);

	return success;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDAreaFamilyVec& CAreaFamilyBundle::getFamilies()
{
	return m_families;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAreaFamily* CAreaFamilyBundle::getFamily(int index)
{
	if (index < 0 || index >= m_families.size())
	{
		return NULL;
	}
	return m_families[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamilyBundle::deleteFamily(int index)
{
	if (index < 0 || index >= m_families.size())
	{
		return;
	}
	TDAreaFamilyVec::iterator family = m_families.begin();
	std::advance(family, index);
	delete (*family);
	m_families.erase(family);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAreaFamily* CAreaFamilyBundle::addNewFamily()
{
	CAreaFamily* family = new CAreaFamily(m_sectortype);
	m_families.push_back(family);

	return family;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAreaFamilyBundle::getNumFamilies()
{
	return (int)m_families.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAreaFamilyBundle::getSectorType()
{
	return m_sectortype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamilyBundle::setSectorType(int type)
{
	m_sectortype = type;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDSpawnFamilyVec& CAreaFamilyBundle::getSpawns()
{
	return m_spawns;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSpawnFamily* CAreaFamilyBundle::getSpawn(int index)
{
	if (index < 0 || index >= m_spawns.size())
	{
		return NULL;
	}
	return m_spawns[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAreaFamilyBundle::deleteSpawn(int index)
{
	if (index < 0 || index >= m_spawns.size())
	{
		return;
	}
	TDSpawnFamilyVec::iterator spawn = m_spawns.begin();
	std::advance(spawn, index);
	delete (*spawn);
	m_spawns.erase(spawn);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSpawnFamily* CAreaFamilyBundle::addNewSpawn()
{
	CSpawnFamily* spawn = new CSpawnFamily;
	m_spawns.push_back(spawn);

	return spawn;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAreaFamilyBundle::getNumSpawns()
{
	return (int)m_spawns.size();
}

}
