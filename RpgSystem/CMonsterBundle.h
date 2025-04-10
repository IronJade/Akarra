/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    "Bundle" is my way of telling it's a list of several items, stored to a file.
 *    Since I didn't know how hash- or treemaps worked back when I wrote these bundles,
 *    I'm looping to find a requested data object.
 **/

#ifndef _CMONSTERBUNDLE_H_
#define _CMONSTERBUNDLE_H_

#include <vector>
#include "CAbility.h"
#include "../library/basic/CCharString.h"

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{

	class CMonsterType;

	// MONSTER TYPE FAMILY ---------------------------------------------------------------------------

	enum EMonsterFamilyFileVersions
	{
		MFFV_ORIGINAL = 0,

		MFFV_030401,		// added ability values, health, exp multiplier and design notes
		MFFV_030513,		// i included the monster boost changes (+25% to damage values)

		MFFV_CURRENT
	};

	// The families are used to rebalance items according to a certain formula. The designer can
	// add and remove families depending on item styles, and then balance them using the item's
	// "level".
	class CMonsterFamily
	{
		public:

			CMonsterFamily();
			~CMonsterFamily();

			void clear();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

		public:

			char	ua_name[32];	// family name

			float	ua_baseabs[NUM_ABILITIES];	// base ability value
			float	ua_multiabs[NUM_ABILITIES];	// ability multipliers (total is = base + multi * level)
			float	u_basehp, u_multihp;
			float	u_expmultiplier;			// experience multiplier

			Basic::CCharString	u_designnotes;

	};

	// MONSTER TYPE BUNDLE ---------------------------------------------------------------------------

	enum EMonsterBundleFileVersions
	{
		MBFV_ORIGINAL = 0,

		MBFV_CURRENT
	};

	typedef std::vector<CMonsterType*>		TDMonsterTypeVec;
	typedef std::vector<CMonsterFamily*>	TDMonsterFamilyVec;

	class CMonsterTypeBundle
	{
		public:

			CMonsterTypeBundle();
			~CMonsterTypeBundle();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
			bool open(char* filename);
			bool save(char* filename);

			// monstertypes

			TDMonsterTypeVec& getMonsterTypes();
			int getNumMonsterTypes();

			CMonsterType* getMonsterType(char* dataname);

			CMonsterType* createNewMonsterType();
			bool addMonsterType(CMonsterType* item);
			bool deleteMonsterType(char* name);

			// monstertype families

			TDMonsterFamilyVec& getFamilies();
			int getNumFamilies();

			CMonsterFamily* getFamily(char* dataname);
			CMonsterFamily* getFamily(int index);

			bool addFamily(CMonsterFamily* item);
			bool deleteFamily(char* name);

		protected:

			void m_clear();

		protected:

			TDMonsterTypeVec	m_types;
			TDMonsterFamilyVec	m_families;

			// The runtime id number is used by the server to identify monster types for players.
			// This is used to decrease the size of a monster packet when a new monster is added on
			// the client-side. The runtime id number is set by the monsterbundle as the bundle is loaded.

			int					m_nextid;
	};

};

#endif
