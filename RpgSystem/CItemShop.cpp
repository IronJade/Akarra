/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A definition for NPC shops.
 **/

#include "CItemShop.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/FileSystem.h"

#ifndef AKARRASERVER
#ifndef AKARRACLIENT
#include <stdlib.h>
#endif
#endif

namespace RpgSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopItem::SShopItem()
:
	weight(1)
{
	itemname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopItem::SShopItem(char* name, int w)
:
	weight(w)
{
	strncpy(itemname, name, 31);
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopCraftRecepy::SShopCraftRecepy()
{
	ingredients[0][0] = 0;
	ingredients[1][0] = 0;
	ingredients[2][0] = 0;
	ingredients[3][0] = 0;
	ingredients[4][0] = 0;
	result[0] = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemShop::CItemShop()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemShop::~CItemShop()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::clear()
{
	ma_shopname[0] = 0;

	m_shoplist.clear();
	m_totalweight = 0;
	m_sellcostmultiplier = 1.6f;
	m_buyvaluemultiplier = 0.3f;
	m_buylist.clear();
	m_buymaintype = 0;
	{
		for (int i(0); i < 32; i++)
		{
			ma_buysubtypes[i] = 0;
		}
	}

	m_restockcount = 6;
	m_maxstock = 20;
	m_minstock = 0;

	m_craftlist.clear();
	m_craftcosttype = SCCT_DIFFERENCE;
	m_craftcostmultiplier = 1.6f;
	
	m_flags = 0;
	u_designnotes = "";
	m_cointype = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::removeAll()
{
	{
		for (int i(0); i < (int)m_shoplist.size(); i++)
		{
			delete m_shoplist[i];
		}
	}
	{
		for (int i(0); i < (int)m_buylist.size(); i++)
		{
			delete m_buylist[i];
		}
	}
	{
		for (int i(0); i < (int)m_craftlist.size(); i++)
		{
			delete m_craftlist[i];
		}
	}
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemShop::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// remove old stuff
	removeAll();

	// get version
	int version = 0;
	file.read(&version, sizeof(int), 1);

	// normal data...

	file.read(ma_shopname, sizeof(char), 32);
	file.read(&m_sellcostmultiplier, sizeof(float), 1);
	file.read(&m_buyvaluemultiplier, sizeof(float), 1);
	file.read(&m_cointype, sizeof(int), 1);
	file.read(&m_buymaintype, sizeof(int), 1);
	file.read(ma_buysubtypes, sizeof(int), 32);
	file.read(&m_restockcount, sizeof(int), 1);
	file.read(&m_maxstock, sizeof(int), 1);
	file.read(&m_minstock, sizeof(int), 1);
	file.read(&m_flags, sizeof(int), 1);

	file.read(&m_craftcostmultiplier, sizeof(float), 1);
	file.read(&m_craftcosttype, sizeof(int), 1);

	// listed data...

	{
		int count(0);
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SShopItem* item = new SShopItem;
			file.read(item->itemname, sizeof(char), 32);
			file.read(&item->weight, sizeof(int), 1);
			m_shoplist.push_back(item);
		}
	}
	{
		int count(0);
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SShopItem* item = new SShopItem;
			file.read(item->itemname, sizeof(char), 32);
			file.read(&item->weight, sizeof(int), 1);
			m_buylist.push_back(item);
		}
	}
	{
		int count(0);
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SShopCraftRecepy* recepy = new SShopCraftRecepy;
			file.read(recepy->ingredients[0], sizeof(char), 32);
			file.read(recepy->ingredients[1], sizeof(char), 32);
			file.read(recepy->ingredients[2], sizeof(char), 32);
			file.read(recepy->ingredients[3], sizeof(char), 32);
			file.read(recepy->ingredients[4], sizeof(char), 32);
			file.read(recepy->result, sizeof(char), 32);
			m_craftlist.push_back(recepy);
		}
	}	

	u_designnotes = FileSystem::readStringWithLength(file);

	recalculateSellWeight();

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemShop::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = ISFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// normal data...

	file.write(ma_shopname, sizeof(char), 32);
	file.write(&m_sellcostmultiplier, sizeof(float), 1);
	file.write(&m_buyvaluemultiplier, sizeof(float), 1);
	file.write(&m_cointype, sizeof(int), 1);
	file.write(&m_buymaintype, sizeof(int), 1);
	file.write(ma_buysubtypes, sizeof(int), 32);
	file.write(&m_restockcount, sizeof(int), 1);
	file.write(&m_maxstock, sizeof(int), 1);
	file.write(&m_minstock, sizeof(int), 1);
	file.write(&m_flags, sizeof(int), 1);

	file.write(&m_craftcostmultiplier, sizeof(float), 1);
	file.write(&m_craftcosttype, sizeof(int), 1);

	// listed data...

	{
		int count = (int)m_shoplist.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_shoplist[i]->itemname, sizeof(char), 32);
			file.write(&m_shoplist[i]->weight, sizeof(int), 1);
		}
	}
	{
		int count = (int)m_buylist.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_buylist[i]->itemname, sizeof(char), 32);
			file.write(&m_buylist[i]->weight, sizeof(int), 1);
		}
	}
	{
		int count = (int)m_craftlist.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_craftlist[i]->ingredients[0], sizeof(char), 32);
			file.write(m_craftlist[i]->ingredients[1], sizeof(char), 32);
			file.write(m_craftlist[i]->ingredients[2], sizeof(char), 32);
			file.write(m_craftlist[i]->ingredients[3], sizeof(char), 32);
			file.write(m_craftlist[i]->ingredients[4], sizeof(char), 32);
			file.write(m_craftlist[i]->result, sizeof(char), 32);
		}
	}

	// write design notes
	FileSystem::writeStringWithLength(file, u_designnotes);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setShopName(char* name)
{
	strncpy(ma_shopname, name, 31);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CItemShop::getShopName()
{
	return ma_shopname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::copyOther(CItemShop* other)
{
	if (!other)
	{
		return;
	}

	// clear old data, but remember shop name
	char oldname[32];
	strcpy(oldname, ma_shopname);
	removeAll();
	setShopName(oldname);

	// copy lists
	{
		for (int i(0); i < other->m_shoplist.size(); i++)
		{
			SShopItem* item = new SShopItem(other->m_shoplist[i]->itemname, other->m_shoplist[i]->weight);
			m_shoplist.push_back(item);
		}
	}
	{
		for (int i(0); i < other->m_buylist.size(); i++)
		{
			SShopItem* item = new SShopItem(other->m_buylist[i]->itemname, other->m_buylist[i]->weight);
			m_buylist.push_back(item);
		}
	}
	{
		for (int i(0); i < other->m_craftlist.size(); i++)
		{
			SShopCraftRecepy* recepy = new SShopCraftRecepy;
			strcpy(recepy->ingredients[0], other->m_craftlist[i]->ingredients[0]);
			strcpy(recepy->ingredients[1], other->m_craftlist[i]->ingredients[1]);
			strcpy(recepy->ingredients[2], other->m_craftlist[i]->ingredients[2]);
			strcpy(recepy->ingredients[3], other->m_craftlist[i]->ingredients[3]);
			strcpy(recepy->ingredients[4], other->m_craftlist[i]->ingredients[4]);
			strcpy(recepy->result, other->m_craftlist[i]->result);
			m_craftlist.push_back(recepy);
		}
	}	

	m_totalweight = other->m_totalweight;
	m_sellcostmultiplier = other->m_sellcostmultiplier;
	m_buyvaluemultiplier = other->m_buyvaluemultiplier;
	m_buymaintype = other->m_buymaintype;
	{
		for (int i(0); i < 32; i++)
		{
			ma_buysubtypes[i] = other->ma_buysubtypes[i];
		}
	}

	m_restockcount = other->m_restockcount;
	m_maxstock = other->m_maxstock;
	m_minstock = other->m_minstock;

	m_craftcosttype = other->m_craftcosttype;
	m_craftcostmultiplier = other->m_craftcostmultiplier;
	
	m_flags = other->m_flags;
	u_designnotes = other->u_designnotes;
	m_cointype = other->m_cointype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDShopItemVec& CItemShop::getSellItems()
{
	return m_shoplist;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDShopItemVec& CItemShop::getBuyItems()
{
	return m_buylist;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::addSellItem(char* name, int weight)
{
	SShopItem* item = new SShopItem(name, weight);
	m_shoplist.push_back(item);

	recalculateSellWeight();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::removeSellItem(int index)
{
	if (index < 0 || index >= (int)m_shoplist.size())
	{
		return;
	}

	TDShopItemVec::iterator item = m_shoplist.begin();
	std::advance(item, index);
	delete (*item);
	m_shoplist.erase(item);

	recalculateSellWeight();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopItem* CItemShop::getSellItem(int index)
{
	if (index < 0 || index >= (int)m_shoplist.size())
	{
		return NULL;
	}

	return m_shoplist[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Returns a randomly chosen item from the shop, depending on the item's randomizer weight
// (a higher weight will increase the chance that the item is chosen).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopItem* CItemShop::getRandomSellItem()
{
	if (m_shoplist.empty() || m_totalweight <= 0)
	{
		return NULL;
	}

	int value = rand() % m_totalweight;

	for (int i(0); i < (int)m_shoplist.size(); i++)
	{
		value -= m_shoplist[i]->weight;
		if (value < 0)
		{
			return m_shoplist[i];
		}
	}

	// if the loop reaches this point, it's a bug!
	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::addBuyItem(char* name)
{
	// the buy list may not include duplicates
	for (int i(0); i < (int)m_buylist.size(); i++)
	{
		if (stricmp(m_buylist[i]->itemname, name) == 0)
		{
			return;
		}
	}

	SShopItem* item = new SShopItem(name, 1);
	m_buylist.push_back(item);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::removeBuyItem(char* name)
{
	// find the item and remove it
	TDShopItemVec::iterator item;
	for (item = m_buylist.begin(); item != m_buylist.end(); ++item)
	{
		if (stricmp((*item)->itemname, name) == 0)
		{
			delete (*item);
			m_buylist.erase(item);
			break;
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::removeBuyItem(int index)
{
	if (index < 0 || index >= (int)m_buylist.size())
	{
		return;
	}

	TDShopItemVec::iterator item = m_buylist.begin();
	std::advance(item, index);
	delete (*item);
	m_buylist.erase(item);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getMainTypeBuyFlags()
{
	return m_buymaintype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setMainTypeBuyFlags(int flags)
{
	m_buymaintype = flags;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getSubTypeBuyFlags(int maintype)
{
	if (maintype < 0 || maintype >= 32)
	{
		return 0;
	}
	return ma_buysubtypes[maintype];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setSubTypeBuyFlags(int maintype, int flags)
{
	if (maintype < 0 || maintype >= 32)
	{
		return;
	}
	ma_buysubtypes[maintype] = flags;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemShop::wantsToBuyItem(char* name, int maintype, int subtype)
{
	// check flags first
	if (maintype >= 0 && maintype < 32)
	{
		if ((m_buymaintype & (1 << maintype)) != 0)
		{
			// this item is covered by the maintype flags
			return true;
		}
		if ((ma_buysubtypes[maintype] & (1 << subtype)) != 0)
		{
			return true;
		}
	}

	// now check if the item is in our specific buy list
	if (name)
	{
		for (int i(0); i < (int)m_buylist.size(); i++)
		{
			if (stricmp(m_buylist[i]->itemname, name) == 0)
			{
				return true;
			}
		}
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getSellWeight()
{
	return m_totalweight;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::recalculateSellWeight()
{
	m_totalweight = 0;
	for (int i(0); i < (int)m_shoplist.size(); i++)
	{
		m_totalweight += m_shoplist[i]->weight;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getMinimumItems()
{
	return m_minstock;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setMinimumItems(int value)
{
	m_minstock = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getMaximumItems()
{
	return m_maxstock;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setMaximumItems(int value)
{
	m_maxstock = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getAddItemRate()
{
	return m_restockcount;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setAddItemRate(int value)
{
	m_restockcount = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getCoinType()
{
	return m_cointype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setCoinType(int value)
{
	m_cointype = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CItemShop::getSellCostMultiplier()
{
	return m_sellcostmultiplier;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setSellCostMultiplier(float value)
{
	m_sellcostmultiplier = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CItemShop::getBuyValueMultiplier()
{
	return m_buyvaluemultiplier;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setBuyValueMultiplier(float value)
{
	m_buyvaluemultiplier = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDShopCraftVec& CItemShop::getCraftRecepies()
{
	return m_craftlist;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopCraftRecepy* CItemShop::addNewRecepy()
{
	SShopCraftRecepy* recepy = new SShopCraftRecepy;
	m_craftlist.push_back(recepy);

	return recepy;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SShopCraftRecepy* CItemShop::getRecepy(int index)
{
	if (index < 0 || index >= (int)m_craftlist.size())
	{
		return NULL;
	}
	return m_craftlist[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::removeRecepy(int index)
{
	if (index < 0 || index >= (int)m_craftlist.size())
	{
		return;
	}

	TDShopCraftVec::iterator item = m_craftlist.begin();
	std::advance(item, index);
	delete (*item);
	m_craftlist.erase(item);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CItemShop::getCraftCostType()
{
	return m_craftcosttype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setCraftCostType(int value)
{
	m_craftcosttype = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CItemShop::getCraftCostMultiplier()
{
	return m_craftcostmultiplier;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setCraftCostMultiplier(float value)
{
	m_craftcostmultiplier = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemShop::secretCrafting()
{
	return ((m_flags & SHOPFLAG_SECRETCRAFTING) != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemShop::setSecrecy(bool value)
{
	if (value)
	{
		m_flags |= SHOPFLAG_SECRETCRAFTING;
	}
	else
	{
		m_flags &= ~SHOPFLAG_SECRETCRAFTING;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function takes 5 item names and checks if any of the craft recepies in this shop
// matches those five names.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CItemShop::getCraftedItem(char* ing[5])
{
	// get the number of empty slots first
	int emptyslots(0);
	{
		for (int i(0); i < 5; i++)
		{
			if (ing[i][0] == 0)
			{
				emptyslots++;
			}
		}
	}
	if (emptyslots >= 5)
	{
		return NULL;
	}

	// now check our recepies
	TDShopCraftVec::iterator recepy;
	for (recepy = m_craftlist.begin(); recepy != m_craftlist.end(); ++recepy)
	{
		// count the number of empty items in this recepy
		int emptyrecepy(0);
		bool taken[5];
		{
			for (int i(0); i < 5; i++)
			{
				if ((*recepy)->ingredients[i][0] == 0)
				{
					taken[i] = true;
					emptyrecepy++;
				}
				else
				{
					taken[i] = false;
				}
			}
		}

		if (emptyrecepy != emptyslots)
		{
			// not the same number of items in the recepy as in the submitted items
			continue;
		}

		{
			for (int i(0); i < 5; i++)
			{
				if (ing[i][0] != 0)
				{
					// look for this ingredient in the recepy
					int j = 0;
					for ( ; j < 5; j++)
					{
						if (taken[j])
						{
							continue;
						}
						if (stricmp(ing[i], (*recepy)->ingredients[j]) == 0)
						{
							// found a match, take it
							taken[j] = true;
							break;
						}
					}
					if (j >= 5)
					{
						// we couldn't find a match for this item
						break;
					}
				}
			}
		}

		// if all recepies are marked as "taken", we have found our match
		{
			int i(0);
			for ( ; i < 5; i++)
			{
				if (!taken[i])
				{
					// nope
					break;
				}
			}
			if (i >= 5)
			{
				// yep
				return (*recepy)->result;
			}
		}
	}

	return NULL;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

}
