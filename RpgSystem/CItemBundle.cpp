/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    "Bundle" is my way of telling it's a list of several items, stored to a file.
 *    Since I didn't know how hash- or treemaps worked back when I wrote these bundles,
 *    I'm looping to find a requested data object.
 **/

#include "CItemBundle.h"
#include "CItem.h"
#include "CItemShop.h"
#include "../library/FileSystem/FileSystem.h"
#include "../library/FileSystem/CFileStream.h"

namespace RpgSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemTypeBundle::CItemTypeBundle()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemTypeBundle::~CItemTypeBundle()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemTypeBundle::m_clear()
{
	m_items.clear();
	m_families.clear();
	m_shops.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemTypeBundle::removeAll()
{
	{
		for (int i(0); i < m_items.size(); i++)
		{
			delete m_items[i];
		}
	}
	{
		for (int i(0); i < m_families.size(); i++)
		{
			delete m_families[i];
		}
	}
	{
		for (int i(0); i < m_shops.size(); i++)
		{
			delete m_shops[i];
		}
	}
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::read(FileSystem::CFileStream& file)
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
			CItemType* item = new CItemType;
			if (!item->read(file))
			{
				delete item;
				return false;
			}

			// insert skill
			m_items.push_back(item);
		}
	}

	// family data
	if (version > ITBFV_021119)
	{
		int count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			CItemTypeFamily* family = new CItemTypeFamily;
			if (!family->read(file))
			{
				delete family;
				return false;
			}

			// insert skill
			m_families.push_back(family);
		}
	}

	// shop data
	if (version > ITBFV_030403)
	{
		int count = 0;
		file.read(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			CItemShop* shop = new CItemShop;
			if (!shop->read(file))
			{
				delete shop;
				return false;
			}

			// insert skill
			m_shops.push_back(shop);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = ITBFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write item data
	{
		int count = getNumItems();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_items[i]->write(file))
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

	// shop data
	{
		int count = getNumShops();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			if (!m_shops[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::open(char* filename)
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
bool CItemTypeBundle::save(char* filename)
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
TDItemTypeVec& CItemTypeBundle::getItems()
{
	return m_items;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemTypeBundle::getNumItems()
{
	return (int)m_items.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemType* CItemTypeBundle::getItem(char* dataname)
{
	// attempt to find the item
	for (int i(0); i < getNumItems(); i++)
	{
		if (stricmp(m_items[i]->ua_dataname, dataname) == 0)
		{
			return m_items[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemType* CItemTypeBundle::createNewItem()
{
	CItemType* item = new CItemType;
	m_items.push_back(item);

	return item;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::addItem(CItemType* item)
{
	if (!item)
	{
		return false;
	}

	// error-check name
	if (getItem(item->ua_dataname))
	{
		// already exists
		return false;
	}

	m_items.push_back(item);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Returns true if the item was successfully removed
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::deleteItem(char* name)
{
	if (!name)
	{
		return false;
	}

	// find the item
	TDItemTypeVec::iterator item = m_items.begin();
	for (item; item != m_items.end(); ++item)
	{
		if (stricmp((*item)->ua_dataname, name) == 0)
		{
			// item found, remove it
			delete (*item);
			m_items.erase(item);

			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDItemFamilyVec& CItemTypeBundle::getFamilies()
{
	return m_families;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemTypeBundle::getNumFamilies()
{
	return (int)m_families.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemTypeFamily* CItemTypeBundle::getFamily(char* dataname)
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
bool CItemTypeBundle::addFamily(CItemTypeFamily* item)
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
bool CItemTypeBundle::deleteFamily(char* name)
{
	if (!name)
	{
		return false;
	}

	// find the item
	TDItemFamilyVec::iterator item = m_families.begin();
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

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDShopVec& CItemTypeBundle::getShops()
{
	return m_shops;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemTypeBundle::getNumShops()
{
	return (int)m_shops.size();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemShop* CItemTypeBundle::getShop(char* shopname)
{
	// attempt to find the shop
	for (int i(0); i < getNumShops(); i++)
	{
		if (stricmp(m_shops[i]->getShopName(), shopname) == 0)
		{
			return m_shops[i];
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemShop* CItemTypeBundle::getShop(int index)
{
	if (index < 0 || index >= getNumShops())
	{
		return NULL;
	}
	return m_shops[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemShop* CItemTypeBundle::addNewShop(char* shopname)
{
	if (!shopname)
	{
		return NULL;
	}

	// error-check name
	if (getShop(shopname))
	{
		// already exists
		return NULL;
	}

	// create shop
	CItemShop* shop = new CItemShop;
	shop->setShopName(shopname);
	m_shops.push_back(shop);

	return shop;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::deleteShop(char* name)
{
	if (!name)
	{
		return false;
	}

	// find the item
	TDShopVec::iterator item = m_shops.begin();
	for (item; item != m_shops.end(); ++item)
	{
		if (stricmp((*item)->getShopName(), name) == 0)
		{
			// item found, remove it
			delete (*item);
			m_shops.erase(item);

			return true;
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeBundle::deleteShop(int index)
{
	if (index < 0 || index >= getNumShops())
	{
		return false;
	}

	TDShopVec::iterator item = m_shops.begin();
	std::advance(item, index);
	delete (*item);
	m_shops.erase(item);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemTypeFamily::CItemTypeFamily()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemTypeFamily::~CItemTypeFamily()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemTypeFamily::clear()
{
	ua_name[0] = 0;

	for (int i(0); i < 3; i++)
	{
		ua_baseparams[i] = 0;
		ua_mulparams[i] = 0;
		ua_ranges[i] = 0;
		ua_reqtypes[i] = -1;
		ua_reqmulti[i] = 0;
		ua_reqbase[i] = 0;
	}
	u_value = 1.0f;
	u_actiondelay = 1000;
	u_recoverydelay = 1000;
	u_notes = "";
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeFamily::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// version...
	int version = 0;
	file.read(&version, sizeof(int), 1);

	// data...
	file.read(ua_name, sizeof(char), 32);
	file.read(ua_baseparams, sizeof(float), 3);
	file.read(ua_mulparams, sizeof(float), 3);
	file.read(&u_value, sizeof(float), 1);
	file.read(ua_ranges, sizeof(int), 3);
	file.read(&u_actiondelay, sizeof(int), 1);
	file.read(&u_recoverydelay, sizeof(int), 1);
	file.read(ua_reqtypes, sizeof(int), 3);
	file.read(ua_reqmulti, sizeof(float), 3);
	file.read(ua_reqbase, sizeof(int), 3);

	// notes
	if (version > ITFFV_021123)
	{
		u_notes = FileSystem::readStringWithLength(file);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemTypeFamily::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// version...
	int version = ITFFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// data...
	file.write(ua_name, sizeof(char), 32);
	file.write(ua_baseparams, sizeof(float), 3);
	file.write(ua_mulparams, sizeof(float), 3);
	file.write(&u_value, sizeof(float), 1);
	file.write(ua_ranges, sizeof(int), 3);
	file.write(&u_actiondelay, sizeof(int), 1);
	file.write(&u_recoverydelay, sizeof(int), 1);
	file.write(ua_reqtypes, sizeof(int), 3);
	file.write(ua_reqmulti, sizeof(float), 3);
	file.write(ua_reqbase, sizeof(int), 3);

	// notes
	FileSystem::writeStringWithLength(file, u_notes);

	return true;
}

}
