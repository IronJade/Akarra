/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This is a wrapper class for all of a player character's items and money.
 **/

#include "CCharacterInventory.h"
#include "CCharacterAttributes.h"
#include "CEffect.h"
#include "../PlayerSystem/AkarraCharacter.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharacterInventory::CCharacterInventory()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharacterInventory::~CCharacterInventory()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterInventory::m_clear()
{
	u_maxbackpackslots = 5;
	{
		for (int i(0); i < NUM_MONEYTYPES; i++)
		{
			ua_coins[i] = 0;
			ua_maxcoins[i] = 0;
		}
	}
	{
		for (int i(0); i < NUM_EQUIPMENT_SLOTS; i++)
		{
			upa_equipment[i] = NULL;
		}
	}
	{
		for (int i(0); i < INVENTORY_BACKPACK; i++)
		{
			upa_backpack[i] = NULL;
		}
	}
	u_storagelist.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterInventory::removeAll()
{
	{
		for (int i(0); i < NUM_EQUIPMENT_SLOTS; i++)
		{
			delete upa_equipment[i];
		}
	}
	{
		for (int i(0); i < INVENTORY_BACKPACK; i++)
		{
			delete upa_backpack[i];
		}
	}
	{
		std::list<CItemInstance*>::iterator item;
		for (item = u_storagelist.begin(); item != u_storagelist.end(); ++item)
		{
			delete (*item);
		}
	}
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterInventory::read(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	removeAll();

	// earlier versions didn't have any inventory at all
	if (version > Player::CHRFV_020611)
	{
		file.read(ua_coins, sizeof(int), NUM_MONEYTYPES);
	}

	if (version <= Player::CHRFV_020908)
	{
		// convert coin types to the new system
		ua_coins[0] = (ua_coins[0] * 100) + (ua_coins[1] * 10) + (ua_coins[2] * 1);
		ua_coins[1] = 0;
		ua_coins[2] = 0;
	}

	// check if this version includes the inventory
	if (version > Player::CHRFV_020622)
	{
		int inventorycapacity = INVENTORY_BACKPACK;
		int equipmentcapacity = NUM_EQUIPMENT_SLOTS;

		if (version <= Player::CHRFV_020908)
		{
			// this version had max 40 slots
			inventorycapacity = 40;
		}

		if (version <= Player::CHRFV_021102)
		{
			// this file version didn't have the mounts
			equipmentcapacity = NUM_EQUIPMENT_SLOTS - 1;

			upa_equipment[ES_MOUNT] = NULL;
		}

		unsigned char* eqslots = new unsigned char[equipmentcapacity];
		unsigned char* bpslots = new unsigned char[inventorycapacity];

		file.read(eqslots, sizeof(unsigned char), equipmentcapacity);
		file.read(bpslots, sizeof(unsigned char), inventorycapacity);

		{
			for (int i(0); i < equipmentcapacity; i++)
			{
				upa_equipment[i] = NULL;
				if (eqslots[i])
				{
					upa_equipment[i] = new CItemInstance;
					upa_equipment[i]->read(file, version);
					// TODO: Find the itemtype for this item
				}
			}
		}
		{
			for (int i(0); i < INVENTORY_BACKPACK; i++)
			{
				upa_backpack[i] = NULL;
				if (bpslots[i])
				{
					upa_backpack[i] = new CItemInstance;
					upa_backpack[i]->read(file, version);
					// TODO: Find the itemtype for this item
				}
			}
		}

		delete [] eqslots;
		delete [] bpslots;

		if (version > Player::CHRFV_030405)
		{
			int count(0);
			file.read(&count, sizeof(int), 1);
			for (int i(0); i < count; i++)
			{
				CItemInstance* item = new CItemInstance;
				item->read(file, version);
				u_storagelist.push_back(item);
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterInventory::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	file.write(ua_coins, sizeof(int), NUM_MONEYTYPES);

	// write items...
	unsigned char eqslots[NUM_EQUIPMENT_SLOTS];
	unsigned char bpslots[INVENTORY_BACKPACK];
	{
		// start by writing how many items that actually exists in the inventory
		for (int i(0); i < NUM_EQUIPMENT_SLOTS; i++)
		{
			eqslots[i] = 0;
			if (upa_equipment[i] != NULL)
			{
				eqslots[i] = 1;
			}
		}
	}
	{
		// and the items in the backpack
		for (int i(0); i < INVENTORY_BACKPACK; i++)
		{
			bpslots[i] = 0;
			if (upa_backpack[i] != NULL)
			{
				bpslots[i] = 1;
			}
		}
	}
	file.write(eqslots, sizeof(unsigned char), NUM_EQUIPMENT_SLOTS);
	file.write(bpslots, sizeof(unsigned char), INVENTORY_BACKPACK);
	{
		for (int i(0); i < NUM_EQUIPMENT_SLOTS; i++)
		{
			if (upa_equipment[i] != NULL)
			{
				upa_equipment[i]->write(file);
			}
		}
	}
	{
		for (int i(0); i < INVENTORY_BACKPACK; i++)
		{
			if (upa_backpack[i] != NULL)
			{
				upa_backpack[i]->write(file);
			}
		}
	}

	// write item storage
	int count = (int)u_storagelist.size();
	file.write(&count, sizeof(int), 1);

	std::list<CItemInstance*>::iterator item;
	for (item = u_storagelist.begin(); item != u_storagelist.end(); ++item)
	{
		(*item)->write(file);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterInventory::calculateMaxCapacities(SBaseStats& stats)
{
	for (int i(0); i < NUM_MONEYTYPES; i++)
	{
		ua_maxcoins[i] = CMoney::getMaxCarry((int)stats.level, i);
	}

	// the backpack capacity starts out at 5 slots, and increases by 5 slots every 10
	// levels the character has gained (max INVENTORY_BACKPACK, ie 40 slots)
	u_maxbackpackslots = 10 + (((int)stats.level / 10) * 5);
	if (u_maxbackpackslots > INVENTORY_BACKPACK)
	{
		u_maxbackpackslots = INVENTORY_BACKPACK;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Static function that returns the equipment location of an item. If the item can't be
// equipped (such as a potion), the function returns < 0.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CCharacterInventory::getItemEqSlot(CItemInstance* item)
{
	if (!item || !item->up_type)
	{
		return -1;
	}

	switch (item->up_type->u_maintype)
	{
		default:
			return -1;

		case IMT_WEAPON:
			return ES_RIGHTHAND;
		case IMT_SHIELD:
			return ES_LEFTHAND;
		case IMT_ARMOR:
			return ES_BODY;
		case IMT_HELM:
			return ES_HEAD;
		case IMT_GLOVES:
			return ES_HANDS;
		case IMT_BOOTS:
			return ES_FEET;
		case IMT_RING:
			// this is a special case since you have two ring slots
			// return one of them and let the caller sort out this problem
			return ES_RRING;
		case IMT_NECK:
			return ES_NECK;
		case IMT_MOUNT:
			return ES_MOUNT;
		case IMT_AMMO:
			return ES_LEFTHAND;
		case IMT_LIGHT:
			return ES_LEFTHAND;
		
	}

	return -1;
}


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void SItemSlot::setItemInformation(CItemInstance* item)
{
	if (!item || !item->up_type)
	{
		// empty slot
		name[0] = 0;
		id = 0;
		eqsprite = 0;
		value = 0;

		return;
	}

	strcpy(name, item->getItemDisplayName());
	id = item->getId();
	eqsprite = (int)item->up_type->u_mainsprite;
	eqsprite |= ((unsigned int)(item->up_type->u_ired & 0xF0) << 20); // 0xF0 -> 0x0F000000
	eqsprite |= ((unsigned int)(item->up_type->u_igreen & 0xF0) << 16); // 0xF0 -> 0x00F00000
	eqsprite |= ((unsigned int)(item->up_type->u_iblue & 0xF0) << 12); // 0xF0 -> 0x000F0000

	value = item->up_type->u_value;

	// this is a hack to preserve network bandwidth... since we know that the last byte of
	// the item name never is used, except for a NULL terminator, we can stuff the stack
	// counter there... just remember to nullify it on the client, though :)
	if (item->up_type->u_maintype == IMT_MOUNT)
	{
		// mounts are a little special because they keep a second counter that often can be
		// more than 255... so we divide it by 600 so we get the count in tens of minutes
		name[31] = (char)((item->u_stack / 600) & 0xFF);
	}
	else if (item->up_type->u_maintype == IMT_LIGHT)
	{
		// lights works just as mounts
		name[31] = (char)((item->u_stack / 600) & 0xFF);
	}
	else if (item->up_type->u_flags & IFLAG_DECAYING)
	{
		// decaying items are just the same...
		name[31] = (char)((item->u_stack / 600) & 0xFF);
	}
	else
	{
		name[31] = (char)(item->u_stack & 0xFF);

		if (item->u_stack > 0)
		{
			value *= item->u_stack;

			// the price for ammo is per 10 pieces
			if (item->up_type->u_maintype == IMT_AMMO)
			{
				value = (value + 5) / 10;
			}
		}
	}
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSmallInventory::setInventory(CCharacterInventory& inventory)
{
	{
		for (int i(0); i < NUM_MONEYTYPES; i++)
		{
			ua_coins[i] = inventory.ua_coins[i];
			ua_maxcoins[i] = inventory.ua_maxcoins[i];
		}
	}

	u_maxbpsize = inventory.u_maxbackpackslots;

	{
		for (int i(0); i < NUM_EQUIPMENT_SLOTS; i++)
		{
			ua_equipment[i].setItemInformation(inventory.upa_equipment[i]);
		}
	}
	{
		for (int i(0); i < INVENTORY_BACKPACK; i++)
		{
			ua_backpack[i].setItemInformation(inventory.upa_backpack[i]);
		}
	}
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemInstance::CItemInstance()
{
	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemInstance::~CItemInstance()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemInstance::m_clear()
{
	up_type = NULL;
	ua_typename[0] = 0;
	u_gid = 0;
	u_flags = 0;
	u_stack = 0;

	u_mountx = 0;
	u_mounty = 0;

	ua_effects[0] = NULL;
	ua_effects[1] = NULL;

	ua_customname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write item info

	file.write(ua_typename, sizeof(char), 32);
	file.write(&u_gid, sizeof(unsigned int), 1);
	file.write(&u_flags, sizeof(int), 1);
	file.write(&u_stack, sizeof(int), 1);

	file.write(&u_mountx, sizeof(int), 1);
	file.write(&u_mounty, sizeof(int), 1);

	file.write(ua_customname, sizeof(char), 32);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::read(FileSystem::CFileStream& file, int version)
{
	m_clear();

	if (version <= Player::CHRFV_020622)
	{
		// there are no items in this version
		return true;
	}

	if (!file.isOk())
	{
		return false;
	}

	// read item info (note: the item type must be found before the item can be used)
	file.read(ua_typename, sizeof(char), 32);
	file.read(&u_gid, sizeof(unsigned int), 1);
	file.read(&u_flags, sizeof(int), 1);

	if (version > Player::CHRFV_020810)
	{
		file.read(&u_stack, sizeof(int), 1);
	}

	if (version > Player::CHRFV_030102)
	{
		file.read(&u_mountx, sizeof(int), 1);
		file.read(&u_mounty, sizeof(int), 1);
	}

	if (version > Player::CHRFV_030219)
	{
		file.read(ua_customname, sizeof(char), 32);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemInstance::setItemType(CItemType* type)
{
	if (!type)
	{
		return;
	}
	up_type = type;

	// this is a new item, so copy the itemtype name
	strcpy(ua_typename, type->ua_dataname);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemInstance::setNewItem(CItemType* type, unsigned int id)
{
	setItemType(type);
	u_gid = id;

	// setup the stack counter for mounts
	if (type && type->u_maintype == IMT_MOUNT)
	{
		// the item file counts in minutes, but we like to count in seconds...
		u_stack = (int)(type->u_param1 + 0.5f) * 60;
	}
	else if (type && type->u_maintype == IMT_LIGHT)
	{
		// the item file counts in minutes, but we like to count in seconds...
		u_stack = (int)(type->u_param1 + 0.5f) * 60;
	}
	else if (type && type->u_maintype == IMT_AMMO)
	{
		u_stack = (int)(type->u_param1 + 0.5f);
	}
	else if (type && type->u_flags & IFLAG_DECAYING)
	{
		u_stack = type->u_decayminutes * 60;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemInstance::setId(unsigned int gid)
{
	u_gid = gid;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned int CItemInstance::getId()
{
	return u_gid;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CItemInstance::getItemDataName()
{
	if (!up_type)
	{
		return "§1Unknown";
	}

	return up_type->ua_dataname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CItemInstance::getItemDisplayName()
{
	if (u_flags & IIFLAG_CUSTOMNAME)
	{
		return ua_customname;
	}
	if (!up_type)
	{
		return "§1Unknown";
	}

	if ((u_flags & RpgSystem::IIFLAG_IDENTIFIED) || !(up_type->u_flags & RpgSystem::IFLAG_HIDDEN))
	{
		return up_type->ua_displayname;
	}

	return "§1Unidentified Item";
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::isStackable()
{
	if (!up_type)
	{
		return false;
	}

	if (up_type->u_flags & RpgSystem::IFLAG_STACKABLE)
	{
		return true;
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function will attemtp to stack the other item ontop of this item. Stackable items
// can only be stacked on items of the same type, but since the dataname is used to
// determine different amounts of stacked items, we use the displayname to determine if
// the two items are stackable on each other.
// If the other item is successfully stacked on this item, the other item's stack count
// will tell how many items that are remaining in the other stack. If this number is 0,
// the item should be deleted (since all items have been transferred to this instance).
// The function returns false if the two items can't be stacked ontop of each other.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::addStackItem(CItemInstance* other)
{
	if (!other)
	{
		return false;
	}
	if (!isStackable() || !other->isStackable())
	{
		return false;
	}

	// now check the display names of these two items
	if (stricmp(up_type->ua_displayname, other->up_type->ua_displayname) != 0)
	{
		// not stackable
		return false;
	}

	int maxcount = getItemMaxStackCount(up_type->u_maintype, up_type->u_subtype);
	int withdraw = other->u_stack;
	if ((u_stack + withdraw) > maxcount)
	{
		withdraw = maxcount - u_stack;
		u_stack = maxcount;
	}
	else
	{
		u_stack += withdraw;
	}
	other->u_stack -= withdraw;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::isDropable()
{
	return ((u_flags & IIFLAG_NODROP) == 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function checks if the item is a decaying item. Note that mount and light item
// always are decaying and use a special system for this, so they will most likely return
// false in this function.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::isDecaying()
{
	return (up_type && (up_type->u_flags & IFLAG_DECAYING));
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::isSingleCarry()
{
	return (up_type && (up_type->u_flags & IFLAG_SINGLECARRY));
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemInstance::setEnabled(bool value)
{
	if (value)
	{
		u_flags &= ~IIFLAG_DISABLED;
	}
	else
	{
		u_flags |= IIFLAG_DISABLED;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemInstance::isEnabled()
{
	return ((u_flags & IIFLAG_DISABLED) == 0);
}



}
