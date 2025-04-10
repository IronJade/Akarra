/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Item class, as defined by the item editor.
 **/

#ifndef _CITEM_H_
#define _CITEM_H_

#include "CMoney.h"
#include "SRequirement.h"
#include "../library/Basic/CCharString.h"
#include <vector>

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{
	enum EItemTypeVersions
	{
		ITV_ORIGINAL = 0,

		ITV_020908,		// converted the value of items to the new system
		ITV_020925,		// changed item value from unsigned short to int
		ITV_021010,		// added another color value because some items need different colors when equipped/unequipped
		ITV_021118,		// added item recepies and display name and designer's notes
		ITV_030214,		// added u_specialstring
		ITV_030407,		// added u_effectchance
		ITV_030504,		// great weapon and monster rebalance
		ITV_030504b,	// added u_decayminutes
		ITV_030519,		// test speed change

		CURRENT_ITEMTYPE_VERSION
	};

	const int ITEM_NAMELENGTH	= 32;

	// this structure is used for item recepies
	struct SItemRecepy
	{
		char	source[ITEM_NAMELENGTH];	// the source reaction trigger
		char	result[ITEM_NAMELENGTH];	// reaction result
	};
	typedef std::vector<SItemRecepy>	TDRecepyVec;

	// the item type class...
	class CItemType
	{
		public:

			CItemType();
			~CItemType();

			void clear();

			bool write(FileSystem::CFileStream& file);
			bool read(FileSystem::CFileStream& file);

			void copyOther(CItemType* other);

			// network functions

			bool createData(unsigned char** dest, int& size, Basic::CCharString& failreason);
			bool loadData(unsigned char* source, int size, Basic::CCharString& failreason);

		public:

			char			ua_dataname[ITEM_NAMELENGTH];		// used by the server
			char			ua_displayname[ITEM_NAMELENGTH];	// displayed to the character

			int				u_maintype;		// main type is item type (such as "weapon")
			int				u_subtype;		// sub type is specific type (such as "sword")

			unsigned short	u_mainsprite;	// sprite used in the interface
			unsigned short	u_worldsprite;	// sprite used when the item lies on the ground
			unsigned short	u_eqsprite;		// sprite used when the item is equipped (on the character)
			unsigned char	u_ired;			// sprite color (inventory sprite)
			unsigned char	u_igreen;
			unsigned char	u_iblue;
			unsigned char	u_ered;			// when equipped or on the ground
			unsigned char	u_egreen;
			unsigned char	u_eblue;

			unsigned char	u_itemlevel;
			int				u_value;		// item value

			// the item parameters are used differently depending on item type
			// Weapons:	param1 is min dmg, param2 is max dmg, param3 is stamina cost
			// Armor: param1 is armor class, param2 is dmg red, param3 is stamina cost
			// Mounts: param1 is life length in minutes
			// Ammo: param1 is stack count (only used once), param2 is damage multiplier, param3 is range modifier
			float			u_param1, u_param2, u_param3;
			int				u_flags;

			// bow accuracy variables
			// Weapons: u_near is critical chance, u_medium is fumble chance
			unsigned char	u_near, u_medium, u_far;

			// item use delays (it takes time to drink poitions too)
			// Mounts: u_action is the speed of the mount (speed to take 2 squares)
			unsigned short	u_action, u_recovery;

			// item requirements
			SRequirement	ua_req[3];

			// item effects
			char			ua_effects[2][32];
			int				u_effectchance;		// 0-100, chance that the effect takes place (currently only weapon attack effects)

			// item reaction recepies

			TDRecepyVec		u_combinations;	// combination recepies (this + source = result)
			TDRecepyVec		u_reactions;	// reaction recepies (this + source = result + source)

			Basic::CCharString	u_designnotes;	// designer's notes about this item

			// this is a special string used in certain occasions... plants, for example, use it to
			// determine in which areas they can be planted (the string must equal the area name)
			Basic::CCharString	u_specialstring;

			// used for decaying items
			int					u_decayminutes;
	};

	// item flags
	const int IFLAG_HIDDEN			= 0x1;	// this item requires to be identified
	const int IFLAG_TWOHANDED		= 0x2;	// this item requires both hands to use (weapons)
	const int IFLAG_STACKABLE		= 0x4;	// several items of this type can be placed in the same slot
	const int IFLAG_IGNORESHIELD	= 0x8;	// used for weapons that ignores the defender's shield AC
	const int IFLAG_NOVICTIMEFFECT	= 0x10;	// used for weapons if they have two wielder effects instead of one wielder and one victim effect
	const int IFLAG_DECAYING		= 0x20;	// this is a decaying item that will self-destruct if used too long
	const int IFLAG_SINGLECARRY		= 0x40;	// a character may never carry more than one of these at the same time

	const int ITEMPACKET_CHECK	= 0x56749676;

	char* getItemTypeName(int type);
	char* getItemSubName(int type, int sub);
	int getItemMaxStackCount(int type, int sub);
	int getWeaponAmmoType(int weaponsubtype);

	// item "main" types
	enum EItemMainTypes
	{
		IMT_GENERAL = 0,	// general-purpose items (money, junk etc)

		// equipment items:

		IMT_WEAPON,
		IMT_SHIELD,
		IMT_ARMOR,			// armor and clothing
		IMT_HELM,
		IMT_GLOVES,
		IMT_BOOTS,
		IMT_RING,			// note: two slots
		IMT_NECK,			// amulets and necklaces
		
		// other

		IMT_FOOD,			// all eat-able items... including potions and similar
		IMT_SCROLL,			// skill/spell scroll

		IMT_MOUNT,			// something that can be ridden

		IMT_AMMO,			// items that are used when shooting ranged weapons
		IMT_LIGHT,			// torches and laterns

		NUM_ITEMTYPES
	};

	// general sub-types
	enum EItemSubTypes_General
	{
		ITS_G_VALUABLES = 0,
		ITS_G_JUNK,
		ITS_G_MATERIAL,
		ITS_G_HOUSEHOLD,
		ITS_G_PLANT,			// a plantable plant

		NUM_GENERAL_ITEMTYPES
	};

	// weapon sub-types
	enum EItemSubTypes_Weapon
	{
		// basic weapons

		ITS_W_DAGGER,		// daggers, knives
		ITS_W_SMALLCRUSH,	// hammers, clubs
		ITS_W_LARGECRUSH,	// two-handed hammers (sledges), heavy clubs
		ITS_W_SPEAR,		// spears, pole-axes, tridents

		// swords

		ITS_W_SMALLSWORD,	// shortswords, scimitar(?)
		ITS_W_STAFF,		// 030427: Changed this to a stave class since it wasn't used
		ITS_W_LARGESWORD,	// two-handed swords

		// axes

		ITS_W_SMALLAXE,		// axes
		ITS_W_LARGEAXE,		// two-handed axes, cleavers

		// ballistic weapons

		ITS_W_SHORTBOW,		// shortbows, small bows
		ITS_W_LONGBOW,		// large bows
		ITS_W_CROSSBOW,		// crossbows
		ITS_W_SLING,		// slings
		ITS_W_THROWING,		// rocks, throwing daggers

		// other

		ITS_W_NONVIOLENT,	// can't attack with this one

		NUM_WEAPON_ITEMTYPES
	};
	const int WEAPONGROUPFLAG_BASIC		= 0x0000000F; // 1111
	const int WEAPONGROUPFLAG_SWORDS	= 0x00000070; // 0111 0000
	const int WEAPONGROUPFLAG_AXES		= 0x00000180; // 0001 1000
	const int WEAPONGROUPFLAG_RANGED	= 0x00003E00; // 0011 1110 0000

	// armor sub-types
	enum EItemSubTypes_Armor
	{
		ITS_A_CLOTHING = 0,

		ITS_A_LIGHT,
		ITS_A_MEDIUM,
		ITS_A_HEAVY,

		ITS_A_ROBE,

		NUM_ARMOR_ITEMTYPES
	};

	// head-wear sub-types
	enum EItemSubTypes_Helm
	{
		ITS_H_HELM = 0,
		ITS_H_CROWN,
		ITS_H_DIADEM,

		NUM_HELM_ITEMTYPES
	};

	// shield sub-types
	enum EItemSubTypes_Shield
	{
		ITS_S_SMALL = 0,
		ITS_S_LARGE,

		NUM_SHIELD_ITEMTYPES
	};

	// neck sub-types
	enum EItemSubTypes_Neck
	{
		ITS_N_AMULET = 0,
		ITS_N_NECKLACE,

		NUM_NECK_ITEMTYPES
	};

	// scroll syb-types
	enum EItemSubTypes_Scroll
	{
		ITS_S_SKILL = 0,
		ITS_S_SPELL,

		NUM_SCROLL_ITEMTYPES
	};

	enum EItemSubTypes_Food
	{
		ITS_F_FOOD = 0,
		ITS_F_POTION,
		ITS_F_TOXIC,

		NUM_FOOD_ITEMTYPES
	};

	enum EItemSubTypes_Ammo
	{
		ITS_M_ARROWS = 0,		// bows
		ITS_M_BOLTS,			// crossbows
		ITS_M_BULLETS,			// slings

		NUM_AMMO_ITEMTYPES
	};

	enum EItemSubTypes_Light
	{
		ITS_L_TORCH = 0,
		ITS_L_LATERN,
		ITS_L_CANDLE,

		NUM_LIGHT_ITEMTYPES
	};

} // end of RpgSystem namespace

#endif

