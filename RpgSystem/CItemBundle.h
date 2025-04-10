/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    "Bundle" is my way of telling it's a list of several items, stored to a file.
 *    Since I didn't know how hash- or treemaps worked back when I wrote these bundles,
 *    I'm looping to find a requested data object.
 **/

#ifndef _CITEMBUNDLE_H_
#define _CITEMBUNDLE_H_

#include <vector>
#include "../library/Basic/CCharString.h"

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{

	class CItemType;
	class CItemShop;

	typedef std::vector<CItemShop*>		TDShopVec;

	// ITEM TYPE FAMILY ---------------------------------------------------------------------------

	enum EItemTypeFamilyFileVersions
	{
		ITFFV_ORIGINAL = 0,

		ITFFV_021123,		// added designer's notes and changed the requirement data (didn't change file format, though)

		ITFFV_CURRENT
	};

	// The families are used to rebalance items according to a certain formula. The designer can
	// add and remove families depending on item styles, and then balance them using the item's
	// "level".
	class CItemTypeFamily
	{
		public:

			CItemTypeFamily();
			~CItemTypeFamily();

			void clear();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

		public:

			char				ua_name[32];	// family name
			Basic::CCharString	u_notes;		// designer's notes

			// multipliers

			float	ua_baseparams[3];	// base values for the parameters
			float	ua_mulparams[3];	// parameters multiplied with the item level
			float	u_value;			// percentage added to the standard value calculation

			// static values (same regardless of item level)

			int		ua_ranges[3];		// static values for ballistic ranges and crit/fumble values
			int		u_actiondelay;		// static value for action delay (weapons)
			int		u_recoverydelay;	// static value for recovery delay (weapons)

			// requirements

			int		ua_reqtypes[3];		// doesn't change... -1 disables
			int		ua_reqbase[3];		// starting value
			float	ua_reqmulti[3];		// multiplied with item level and added ontop of the base
		
	};

	// ITEM TYPE BUNDLE ---------------------------------------------------------------------------

	enum EItemTypeBundleFileVersions
	{
		ITBFV_ORIGINAL = 0,

		ITBFV_021119,		// added item families
		ITBFV_030403,		// added item shops

		ITBFV_CURRENT
	};

	typedef std::vector<CItemType*>			TDItemTypeVec;
	typedef std::vector<CItemTypeFamily*>	TDItemFamilyVec;

	class CItemTypeBundle
	{
		public:

			CItemTypeBundle();
			~CItemTypeBundle();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
			bool open(char* filename);
			bool save(char* filename);

			// items

			TDItemTypeVec& getItems();
			int getNumItems();

			CItemType* getItem(char* dataname);

			CItemType* createNewItem();
			bool addItem(CItemType* item);
			bool deleteItem(char* name);

			// item families

			TDItemFamilyVec& getFamilies();
			int getNumFamilies();

			CItemTypeFamily* getFamily(char* dataname);

			bool addFamily(CItemTypeFamily* item);
			bool deleteFamily(char* name);

			// item shops

			TDShopVec& getShops();
			int getNumShops();

			CItemShop* getShop(char* shopname);
			CItemShop* getShop(int index);

			CItemShop* addNewShop(char* shopname);
			bool deleteShop(char* name);
			bool deleteShop(int index);

		protected:

			void m_clear();

		protected:

			TDItemTypeVec	m_items;
			TDItemFamilyVec	m_families;
			TDShopVec		m_shops;

	};

};

#endif
