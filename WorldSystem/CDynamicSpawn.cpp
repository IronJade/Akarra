/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class defines a dynamic (random) spawn point.
 **/

#include "CDynamicSpawn.h"
#include "../Communication/SGenericPacket.h"
#include "../Library/Basic/CCharString.h"
#include "../Library/FileSystem/FileSystem.h"

#ifndef AKARRASERVER
#ifndef AKARRACLIENT
#include <mem.h>
#endif
#endif

namespace WorldSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDynamicSpawnPoint::CDynamicSpawnPoint()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDynamicSpawnPoint::~CDynamicSpawnPoint()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDynamicSpawnPoint::m_clear()
{
	ma_idname[0] = 0;
	m_info.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDynamicSpawnPoint::removeAll()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnPoint::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// file version...
	int version;
	file.read(&version, sizeof(int), 1);

	file.read(ma_idname, sizeof(char), 32);
	if (!m_info.read(file))
	{
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnPoint::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// file version...
	int version = DSPFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(ma_idname, sizeof(char), 32);
	if (!m_info.write(file))
	{
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CDynamicSpawnPoint::getIdName()
{
	return ma_idname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDynamicSpawnPoint::setIdName(char* name)
{
	strncpy(ma_idname, name, 32);
	ma_idname[31] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSpawnFileInfo& CDynamicSpawnPoint::getSpawnInfo()
{
	return m_info;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function will create a data buffer and write all the spawn's information into it.
// The buffer will make place for a Comm::SGenericPacket header which the caller should
// fill in with the required information in order to send the spawn from the client to
// the server.
// IMPORTANT: The caller must delete the memory created.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnPoint::createData(unsigned char** dest, int& size, Basic::CCharString& failreason)
{
	*dest = NULL;
	size = 0;

	// the spawn must include at least one monster to work
	if (m_info.u_monsterlist.empty())
	{
		failreason = "No monsters in spawn";
		return false;
	}

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
	int version = DSPFV_CURRENT;
	memcpy(&data[size], &version, sizeof(int));
	size += sizeof(int);

	// dynamic spawn data
	memcpy(&data[size], ma_idname, sizeof(char) * 32);
	size += sizeof(char) * 32;

	// --- spawn file data ---

	// version
	version = SFV_CURRENT;
	memcpy(&data[size], &version, sizeof(int));
	size += sizeof(int);

	// info
	memcpy(&data[size], &m_info.u_x, sizeof(int));
	size += sizeof(int);
	memcpy(&data[size], &m_info.u_y, sizeof(int));
	size += sizeof(int);
	memcpy(&data[size], &m_info.u_spawninterval, sizeof(int));
	size += sizeof(int);
	memcpy(&data[size], &m_info.u_spawnradius, sizeof(int));
	size += sizeof(int);
	memcpy(&data[size], m_info.ua_spawnname, sizeof(char) * 32);
	size += sizeof(char) * 32;

	// list of monster groups
	int count = m_info.u_monsterlist.size();
	memcpy(&data[size], &count, sizeof(int));
	size += sizeof(int);

	{
		for (int i(0); i < count; i++)
		{
			memcpy(&data[size], m_info.u_monsterlist[i].name, sizeof(char) * 32);
			size += sizeof(char) * 32;
			memcpy(&data[size], &m_info.u_monsterlist[i].maxspawns, sizeof(int));
			size += sizeof(int);
		}
	}

	// move speed
	memcpy(&data[size], &m_info.u_movespeed, sizeof(int));
	size += sizeof(int);

	// number of waypoints
	count = (int)m_info.u_waypoints.size();
	memcpy(&data[size], &count, sizeof(int));
	size += sizeof(int);

	// write waypoints
	{
		for (int i(0); i < count; i++)
		{
			memcpy(&data[size], &m_info.u_waypoints[i].x, sizeof(int));
			size += sizeof(int);
			memcpy(&data[size], &m_info.u_waypoints[i].y, sizeof(int));
			size += sizeof(int);
		}
	}
	
	// place a checkpoint (to make sure that the packet is received correctly)
	int checkpoint = SPAWNPACKET_CHECK;
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
bool CDynamicSpawnPoint::loadData(unsigned char* source, int size, Basic::CCharString& failreason)
{
	if (!source || (size < 12))
	{
		failreason = "Incorrect parameters in packet";
		return false;
	}

	// reset this spawn
	m_clear();

	int pos = sizeof(Comm::SGenericPacket) + sizeof(int);	// skip the header

	// get spawn information version
	int dspversion;
	memcpy(&dspversion, &source[pos], sizeof(int));
	pos += sizeof(int);

	// dynamic spawn data
	memcpy(ma_idname, &source[pos], sizeof(char) * 32);
	pos += sizeof(char) * 32;

	// --- spawn file data ---

	// version
	int siversion;
	memcpy(&siversion, &source[pos], sizeof(int));
	pos += sizeof(int);

	// info
	memcpy(&m_info.u_x, &source[pos], sizeof(int));
	pos += sizeof(int);
	memcpy(&m_info.u_y, &source[pos], sizeof(int));
	pos += sizeof(int);
	memcpy(&m_info.u_spawninterval, &source[pos], sizeof(int));
	pos += sizeof(int);
	memcpy(&m_info.u_spawnradius, &source[pos], sizeof(int));
	pos += sizeof(int);
	memcpy(m_info.ua_spawnname, &source[pos], sizeof(char) * 32);
	pos += sizeof(char) * 32;

	// list of monster groups
	int count = 0;
	memcpy(&count, &source[pos], sizeof(int));
	pos += sizeof(int);

	{
		for (int i(0); i < count; i++)
		{
			SMonsterGroup newgroup;

			memcpy(newgroup.name, &source[pos], sizeof(char) * 32);
			pos += sizeof(char) * 32;
			memcpy(&newgroup.maxspawns, &source[pos], sizeof(int));
			pos += sizeof(int);

			m_info.u_monsterlist.push_back(newgroup);
		}
	}

	// move speed
	memcpy(&m_info.u_movespeed, &source[pos], sizeof(int));
	pos += sizeof(int);

	// number of waypoints
	count = 0;
	memcpy(&count, &source[pos], sizeof(int));
	pos += sizeof(int);

	// write waypoints
	{
		for (int i(0); i < count; i++)
		{
			SSpawnWaypoint newwp;

			memcpy(&newwp.x, &source[pos], sizeof(int));
			pos += sizeof(int);
			memcpy(&newwp.y, &source[pos], sizeof(int));
			pos += sizeof(int);

			m_info.u_waypoints.push_back(newwp);
		}
	}
	
	// check the checkpoint
	int checkpoint = 0;
	memcpy(&checkpoint, &source[pos], sizeof(int));
	pos += sizeof(int);

	if (checkpoint != SPAWNPACKET_CHECK)
	{
		failreason = "CRC check failed";
		return false;
	}

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDynamicSpawnBundle::CDynamicSpawnBundle()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDynamicSpawnBundle::~CDynamicSpawnBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDynamicSpawnBundle::m_clear()
{
	m_spawns.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDynamicSpawnBundle::removeAll()
{
	// remove all spawns
	TDDynamicSpawnVec::iterator spawn;
	for (spawn = m_spawns.begin(); spawn != m_spawns.end(); ++spawn)
	{
		delete (*spawn);
	}

	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnBundle::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	removeAll();

	// read version
	int version;
	file.read(&version, sizeof(int), 1);

	// get spawns
	int count = 0;
	file.read(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			CDynamicSpawnPoint* spawn = new CDynamicSpawnPoint;

			spawn->read(file);

			m_spawns.push_back(spawn);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnBundle::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = DSBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write spawns
	int count = getNumSpawns();
	file.write(&count, sizeof(int), 1);

	{
		for (int i(0); i < count; i++)
		{
			m_spawns[i]->write(file);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnBundle::open(char* filename)
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

	// read data
	bool success = read(file);

	FileSystem::directClose(file);

	return success;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnBundle::save(char* filename)
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

	// write data
	bool success = write(file);

	FileSystem::directClose(file);

	return success;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDDynamicSpawnVec& CDynamicSpawnBundle::getSpawns()
{
	return m_spawns;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDynamicSpawnBundle::getNumSpawns()
{
	return (int)m_spawns.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDynamicSpawnPoint* CDynamicSpawnBundle::getSpawn(char* dataname)
{
	TDDynamicSpawnVec::iterator spawn;
	for (spawn = m_spawns.begin(); spawn != m_spawns.end(); ++spawn)
	{
		if (stricmp((*spawn)->getIdName(), dataname) == 0)
		{
			return (*spawn);
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CDynamicSpawnBundle::addSpawn(CDynamicSpawnPoint* item)
{
	if (!item)
	{
		return false;
	}

	// check if we need to replace another one
	if (getSpawn(item->getIdName()) != NULL)
	{
		deleteSpawn(item->getIdName());
	}

	m_spawns.push_back(item);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDynamicSpawnBundle::deleteSpawn(char* name)
{
	TDDynamicSpawnVec::iterator spawn;
	for (spawn = m_spawns.begin(); spawn != m_spawns.end(); ++spawn)
	{
		if (stricmp((*spawn)->getIdName(), name) == 0)
		{
			delete (*spawn);
			m_spawns.erase(spawn);
			break;
		}
	}
}

}
