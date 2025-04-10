/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The name of this class is a bit weird, yes, but it's purpose is to
 *    save/load areas from the file system.
 **/
#ifndef _CWORLDAREACACHE_H_
#define _CWORLDAREACACHE_H_

#include "../Library/FileSystem/IFileManager.h"
#include "../Library/FileSystem/CFileStream.h"
#include "../Library/Basic/CCharString.h"
#include "CArea.h"
#include "CArea2.h"

namespace WorldSystem
{

	// each world "cell" is an area
	// since the whole world is too large to be dynamically updated (it's 130 megs)
	// we need to split it up into reasonably smaller areas instead and put togheter
	// again during run-time
	//const int	SECTOR_WIDTH	= 32;
	//const int	SECTOR_HEIGHT	= 32;
	//const int	SECTOR_SIZE		= SECTOR_WIDTH * SECTOR_HEIGHT;

	const int	SECTOR2_WIDTH	= 64;
	const int	SECTOR2_HEIGHT	= 64;
	const int	SECTOR2_SIZE	= SECTOR2_WIDTH * SECTOR2_HEIGHT;

	// the following area caches are planned:
	// 0..15	Main world sectors
	// 16..31	Buildings in cities (16 is City of Hope)
	// 32..		Limbo sectors (such as caves etc)
	enum EDefinedAreaSectors
	{
		DAS_WORLD00 = 0,	// world sector (x, y) = (0, 0) (upper left corner)
		DAS_WORLD10,
		DAS_WORLD20,
		DAS_WORLD30,
		DAS_WORLD01,
		DAS_WORLD11,
		DAS_WORLD21,
		DAS_WORLD31,
		DAS_WORLD02,
		DAS_WORLD12,
		DAS_WORLD22,
		DAS_WORLD32,
		DAS_WORLD03,
		DAS_WORLD13,
		DAS_WORLD23,
		DAS_WORLD33,		// (lower right corner)

		DAS_HOPE = 16,
		// more cities to follow

		DAS_CAVES = 32,
		DAS_CAVES2,
		// more to follow...

		HIGHEST_DAS
	};

	//const char* const DAS_NAMES[HIGHEST_DAS] =
	//{
	//	"world0x0y.sec", "world1x0y.sec", "world2x0y.sec", "world3x0y.sec",
	//	"world0x1y.sec", "world1x1y.sec", "world2x1y.sec", "world3x1y.sec",
	//	"world0x2y.sec", "world1x2y.sec", "world2x2y.sec", "world3x2y.sec",
	//	"world0x3y.sec", "world1x3y.sec", "world2x3y.sec", "world3x3y.sec",
		
	//	"hope.sec", "", "", "",
	//	"", "", "", "",
	//	"", "", "", "",
	//	"", "", "", "",

	//	"caves.sec"
	//};

	// 030212: I grew tired of having the extension names in the area names, so
	// I renamed the list to make sure that I didn't do anything wrong.

	const char* const DAS_NAMES[HIGHEST_DAS] =
	{
		"\0world0x0y", "\0world1x0y", "\0world2x0y", "\0world3x0y",
		"\0world0x1y", "\0world1x1y", "\0world2x1y", "\0world3x1y",
		"world0x2y", "world1x2y", "\0world2x2y", "\0world3x2y",
		"world0x3y", "world1x3y", "\0world2x3y", "\0world3x3y",
		
		"\0hope", "", "", "",
		"", "", "", "",
		"", "", "", "",
		"", "", "", "",

		"caves", "caves2"
	};

	/*
	class CWorldAreaCache
	{
		public:

			CWorldAreaCache(FileSystem::IFileManager* manager, char* filename);
			~CWorldAreaCache();

			void removeAll();

			bool openFile();
			void closeFile();
			int fileIsOk();
			
			bool areaExists(int x, int y);
			int getAreaVersion(int x, int y);
			bool getArea(int x, int y, CAreaStatic* dest);

			bool updateArea(int x, int y, CAreaStatic* source);

		private:

			void m_clear();

		protected:

			// information about the file
			FileSystem::IFileManager*	mp_manager;		// position of the file
			Basic::CCharString			m_filename;		// filename
			FileSystem::CFileStream		m_filestream;	// the open file source
			int							m_isok;			// the file has been found

			// information about the areas
			unsigned int				ma_offsets[SECTOR_SIZE];
	};
	*/

	const int PASSWORD_LENGTH = 32;
	const int PASSWORD_KEY = 0xB25A4CC8;

	enum EWorldCacheFileVersions
	{
		WCFV_ORIGINAL = 0,

		WCFV_030125,		// added m_areatype so we know what file format to use ø Ø
		WCFV_050708,		// Edited the fileformat in order to release the world editor for public (without conflicting with DoD)
		WCFV_050709,		// Added a password field

		WCFV_CURRENT
	};

	class CWorldArea2Cache
	{
		public:

			CWorldArea2Cache(FileSystem::IFileManager* manager, char* filename);
			~CWorldArea2Cache();

			void removeAll();

			bool openFile(char* comparePassword);
			void closeFile();
			int fileIsOk();
			
			bool areaExists(int x, int y);
			int getAreaVersion(int x, int y);
			bool getArea(int x, int y, CArea2Static* dest);
			bool createNewArea(int x, int y);

			CArea2Static* getAreaPointer(int x, int y);

			bool createAreas();
			bool updateArea(int x, int y, CArea2Static* source);

			bool convertToWorldSector();
			bool isWorldSector();

		private:

			void m_clear();

		protected:

			// information about the file
			FileSystem::IFileManager*	mp_manager;		// position of the file
			Basic::CCharString			m_filename;		// filename
			FileSystem::CFileStream		m_filestream;	// the open file source
			int							m_isok;			// the file has been found

			int							m_areatype;

			// when areas are supposed to be updated (in the editors), we should keep them
			// all loaded in memory...
			CArea2Static**				mpa_areas;

			// information about the areas
			unsigned int				ma_offsets[SECTOR2_SIZE];

			bool						m_dodFix;

		public:
			char						ma_password[PASSWORD_LENGTH];
	};

}


#endif
