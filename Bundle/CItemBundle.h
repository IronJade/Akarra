/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Don't know why I placed this one here.
 **/

// This class was created because it became too complicated to keep a hundred
// different item files. Instead I needed an item library, with the ability
// to merge with other libraries to allow several people to work on the items
// at the same time. Obviously this must be easy and automatic, with as little
// fuss as possible (just as when the items were in separate files).

#ifndef _ITEMBUNDLE_H_
#define _ITEMBUNDLE_H_

namespace RpgSystem
{
	class CItemType;
}

namespace FileSystem
{
	class CFileStream;
}

#include <list>

namespace Bundle
{
	enum EItemBundleFileVersions
	{
		IBFV_ORIGINAL = 0,

		IBFV_CURRENT
	};


	class CItemBundle
	{
		public:

			CItemBundle();
			~CItemBundle();

			bool open(char* filename);
			bool save(char* filename);
			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			RpgSystem::CItemType* getItemType(char* name);
			std::list<RpgSystem::CItemType*>& accessItemList();

			// these functions _fills_ the list with items

			void getAllItems(std::list<RpgSystem::CItemType*>& list);
			void getAllOfType(std::list<RpgSystem::CItemType*>& list, int maintype, int subflags = 0xFFFFFFFF);

			// these functions sorts the items that _already are in the list_

			void sortByName(std::list<RpgSystem::CItemType*>& list);
			void sortByLevel(std::list<RpgSystem::CItemType*>& list);

			// edit functions

			bool addItem(RpgSystem::CItemType* item);

		protected:

			std::list<RpgSystem::CItemType*>	m_itemlist;

	};

}
