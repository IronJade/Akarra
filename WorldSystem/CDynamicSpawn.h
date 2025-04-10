/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class defines a dynamic (random) spawn point.
 **/

#ifndef _CDYNAMICSPAWN_H_
#define _CDYNAMICSPAWN_H_

#include "../../Akarra Server/ServerWorld/AreaEntities/CSpawnFile.h"

namespace Basic
{
	class CCharString;
}

namespace WorldSystem
{

	// The dynamic spawn data file is used to create a spawnpoint in the world during runtime.
	// The spawn can be designed by an admin and uploaded from the client through a console
	// command. The spawns will be stored in a list on the server, and can be activated or
	// removed by any admins. This class is used to define the spawn, and is used by the
	// borland editor.

	enum EDynamicSpawnPointFileVersions
	{
		DSPFV_ORIGINAL = 0,

		DSPFV_CURRENT
	};

	class CDynamicSpawnPoint
	{
		public:

			CDynamicSpawnPoint();
			~CDynamicSpawnPoint();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			char* getIdName();
			void setIdName(char* name);
			CSpawnFileInfo& getSpawnInfo();

			// network functions

			bool createData(unsigned char** dest, int& size, Basic::CCharString& failreason);
			bool loadData(unsigned char* source, int size, Basic::CCharString& failreason);

		protected:

			void m_clear();

		protected:

			CSpawnFileInfo		m_info;			// the spawnpoint data
			char				ma_idname[32];	// identifier name used by the admins to control the spawn

	};
	typedef std::vector<CDynamicSpawnPoint*>	TDDynamicSpawnVec;

	const int SPAWNPACKET_CHECK		= 0x12345678;

	// this is a bundle handler

	enum EDynamicSpawnBundleFileVersion
	{
		DSBFV_ORIGINAL = 0,

		DSBFV_CURRENT
	};

	class CDynamicSpawnBundle
	{
		public:

			CDynamicSpawnBundle();
			~CDynamicSpawnBundle();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
			bool open(char* filename);
			bool save(char* filename);

			// items

			TDDynamicSpawnVec& getSpawns();
			int getNumSpawns();

			CDynamicSpawnPoint* getSpawn(char* dataname);

			bool addSpawn(CDynamicSpawnPoint* item);
			void deleteSpawn(char* name);

		protected:

			void m_clear();

		protected:

			TDDynamicSpawnVec	m_spawns;
	};
}

#endif
