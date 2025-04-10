/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This is a wrapper class for all of a player character's items and money.
 **/

#ifndef _CCHARACTERINVENTORY_H_
#define _CCHARACTERINVENTORY_H_

#include "CMoney.h"
#include "CItem.h"
#include "../library/FileSystem/CFileStream.h"

namespace RpgSystem
{
	struct SBaseStats;
	class CItemInstance; // see below

	class CEffectType;

	const int	INVENTORY_BACKPACK	= 30;	// number of slots in the backpack

	// equipment slots:
	enum EEquipmentSlots
	{
		ES_RIGHTHAND = 0,	// weapon hand
		ES_LEFTHAND,		// shield hand

		ES_BODY,			// armor
		ES_HEAD,			// helms / headgear
		ES_HANDS,			// gloves
		ES_FEET,			// boots etc

		ES_NECK,			// necklaces or amulets
		ES_RRING,			// right ring
		ES_LRING,			// (wrong? ho ho) ring

		ES_MOUNT,			// the player's horse, wolf, pig, griffon or whatever

		NUM_EQUIPMENT_SLOTS
	};

	// **********************************************************************************
	class CCharacterInventory
	{
		public:

			CCharacterInventory();
			~CCharacterInventory();

			void removeAll();

			bool read(FileSystem::CFileStream& file, int version = 0);
			bool write(FileSystem::CFileStream& file);

			void calculateMaxCapacities(SBaseStats& stats);

			static int getItemEqSlot(CItemInstance* item);

		private:

			void m_clear();

		public:

			int		ua_coins[NUM_MONEYTYPES];		// current money
			int		ua_maxcoins[NUM_MONEYTYPES];		// carry capacity money

			int		u_maxbackpackslots;				// number of slots that the character may use
			CItemInstance*	upa_equipment[NUM_EQUIPMENT_SLOTS];
			CItemInstance*	upa_backpack[INVENTORY_BACKPACK];

			// character item storage
			std::list<CItemInstance*>	u_storagelist;

	};

	// **********************************************************************************
	// Small structure used for the inventory send info.
	struct SItemSlot
	{
		void setItemInformation(CItemInstance* item);

		char			name[32];
		unsigned int	id;
		unsigned int	eqsprite;
		int				value;		// unfortunately we need to keep this information client-side too :-/
	};

	// **********************************************************************************
	// this is a smaller inventory version, designed to be sent across the 'net
	class CSmallInventory
	{
		public:

			void setInventory(CCharacterInventory& inventory);

		public:

			int			ua_coins[NUM_MONEYTYPES];		// current money
			int			ua_maxcoins[NUM_MONEYTYPES];	// carry capacity money

			int			u_maxbpsize;
			SItemSlot	ua_equipment[NUM_EQUIPMENT_SLOTS];
			SItemSlot	ua_backpack[INVENTORY_BACKPACK];
	};

	// **********************************************************************************
	// This class is used when an item has been created of an item type. It is saved
	// together with characters, which means the file versions are from
	// Player::ECharacterFileVersions, and not CItem.h.
	class CItemInstance
	{
		public:

			CItemInstance();
			~CItemInstance();

			bool write(FileSystem::CFileStream& file);
			bool read(FileSystem::CFileStream& file, int version);

			void setItemType(CItemType* type);
			void setNewItem(CItemType* type, unsigned int id);
			void setId(unsigned int gid);
			unsigned int getId();

			char* getItemDataName();
			char* getItemDisplayName();
			bool isDropable();
			bool isDecaying();
			bool isSingleCarry();

			bool isStackable();
			bool addStackItem(CItemInstance* other);

			void setEnabled(bool value);
			bool isEnabled();

		private:
			
			void m_clear();
			unsigned int	u_gid;				// item global identifier

		public:

			CItemType*		up_type;

			char			ua_typename[32];	// required so we can find the type
			char			ua_customname[32];
			
			int				u_flags;
			int				u_stack;			// if several items are stacked ontop of each other

			// this is used for the mounts so we can remember their world coordinates where they
			// were dropped
			int				u_mountx;
			int				u_mounty;

			// since the items may have special effects applied to them we load them in the
			// item instance instead of the item type, because the item type class must be
			// compatible with the borland compiler... so let's not screw things up
			CEffectType*	ua_effects[2];

	};

	const int IIFLAG_IDENTIFIED		= 0x1;	// this item has been identified by a player
	const int IIFLAG_DISABLED		= 0x2;	// an equipped item may be disabled if the character doesn't fulfill its requirements
	const int IIFLAG_NODROP			= 0x4;	// this item can't be dropped or traded (it can be discarded, though)
	const int IIFLAG_CUSTOMNAME		= 0x8;	// this item has been assigned a "custom" name which overrides the datatype displayname

}


#endif
