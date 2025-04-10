/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class is only used by the map editor.
 **/

#ifndef _CAREAFAMILY_H_
#define _CAREAFAMILY_H_

#include <vector>

namespace FileSystem
{
	class CFileStream;
}

namespace WorldSystem
{

	class CArea2Static;
	class CSpawnFileInfo;

	typedef std::vector<CArea2Static*>		TDAreaVec;

	enum EAreaFamilyFileVersions
	{
		AFFV_ORIGINAL = 0,

		AFFV_050708,		// new area version for DoD fix

		AFFV_CURRENT
	};

	// *************************************************************************************
	class CAreaFamily
	{
		public:

			CAreaFamily(int sectortype);
			~CAreaFamily();

			TDAreaVec& getAreas();			void clear();
			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);


			CArea2Static* getArea(int index);
			void deleteArea(int index);
			void insertArea(CArea2Static* source);
			int getNumAreas();

			char* getFamilyName();
			void setFamilyName(char* name);

		protected:

			int			m_sectortype;		// if it's a compressed world sector or a normal cave sector
			TDAreaVec	m_areas;
			char		ma_name[32];

	};

	typedef std::vector<CAreaFamily*>		TDAreaFamilyVec;

	// *************************************************************************************
	enum ESpawnFamilyFileVersions
	{
		SFFV_ORIGINAL = 0,

		SFFV_CURRENT
	};
	typedef std::vector<CSpawnFileInfo*>	TDSpawnVec;
	class CSpawnFamily
	{
		public:

			CSpawnFamily();
			~CSpawnFamily();

			void clear();
			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			TDSpawnVec& getSpawns();
			void deleteSpawn(int index);
			void insertSpawn(CSpawnFileInfo* source);
			int getNumSpawns();
			CSpawnFileInfo* getRandomSpawn();
			
			char* getFamilyName();
			void setFamilyName(char* name);
			int getSpacing();
			void setSpacing(int value);

		protected:

			char		ma_name[32];	// name of the spawn family
			int			m_spacing;		// how far apart the spawns must be

			TDSpawnVec	m_spawnvec;
	};
	typedef std::vector<CSpawnFamily*>		TDSpawnFamilyVec;

	enum EAreaFamilyBundleFileVersions
	{
		AFBFV_ORIGINAL = 0,

		AFBFV_030331,			// added spawn families

		AFBFV_CURRENT
	};

	// *************************************************************************************
	class CAreaFamilyBundle
	{
		public:

			CAreaFamilyBundle();
			~CAreaFamilyBundle();

			void clear();
			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
			bool open(char* filename);
			bool save(char* filename);

			TDAreaFamilyVec& getFamilies();
			CAreaFamily* getFamily(int index);
			void deleteFamily(int index);
			CAreaFamily* addNewFamily();
			int getNumFamilies();

			TDSpawnFamilyVec& getSpawns();
			CSpawnFamily* getSpawn(int index);
			void deleteSpawn(int index);
			CSpawnFamily* addNewSpawn();
			int getNumSpawns();

			int getSectorType();
			void setSectorType(int type);

		protected:

			TDAreaFamilyVec		m_families;
			TDSpawnFamilyVec	m_spawns;
			int					m_sectortype;

	};

}


#endif
