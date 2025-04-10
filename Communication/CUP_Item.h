
#ifndef _CUP_ITEM_H_
#define _CUP_ITEM_H_

namespace Comm
{

	// CUP_I_FULLITEMINFO -----------------------------------------------------------------
	struct SCUPIFullItemInfo
	{
		unsigned char	type;

		unsigned char	itemtype;
		unsigned char	itemsubtype;
		unsigned char	flags;			// TODO: Increase this if necessary

		unsigned char	itemspeed;
		// hit ratios for ranged weapons, critical/fumble chances for melee
		unsigned char	inear;
		unsigned char	imedium;
		unsigned char	ifar;

		unsigned int	itemidentifier;	// id number of this item

		float			itemparams[3];

		int				reqtypes[3];
		int				reqparams[3];

		char			effects[2][32];

		char			name[32];
	};

	// CUP_I_NEWSLOTINFO -----------------------------------------------------------------
	const unsigned char SLOTINFOFLAG_NOSOUND	= 0x1;	// don't play a sound
	struct SCUPINewSlotInfo
	{
		unsigned char	type;

		unsigned char	flags;
		unsigned char	location;		// 1 is equipment, 2 is backpack
		unsigned char	slot;			// 0-8 for eq, 0-29 for backpack

		RpgSystem::SItemSlot	iteminfo;	// item slot info
	};

	// CUP_I_OPENTRADE -----------------------------------------------------------------
	const unsigned char		TRADEFLAG_THISPLAYERISPK	= 0x1;	// "i owe the other a pk fellowny"
	const unsigned char		TRADEFLAG_OTHERPLAYERISPK	= 0x2;	// "you owe me a pk fellony"
	struct SCUPIOpenTrade
	{
		unsigned char	type;
		unsigned char	reserved[2];

		unsigned char	pkflags;	// if anybody has killed the other
		int				gid;		// player id of the character that is trading with you
	};

	// CUP_I_CLOSETRADE -----------------------------------------------------------------
	struct SCUPICloseTrade
	{
		unsigned char	type;
		unsigned char	reserved[3];
	};

	// CUP_I_TRADESTATE -----------------------------------------------------------------
	struct SCUPITradeState
	{
		unsigned char	type;

		unsigned char	pkflags;	// these are set when somebody offers to clear the other's name
		unsigned char	mystate;
		unsigned char	hisstate;
	};

	// CUP_I_TRADEMONEY ---------------------------------------------------------------------
	struct SCUPITradeMoney
	{
		unsigned char	type;
		unsigned char	reserved[3];

		unsigned int	mymoney[3];
		unsigned int	hismoney[3];
	};

	// CUP_I_UPDATEMONEY ------------------------------------------------------------------------
	struct SCUPIUpdateMoney
	{
		unsigned char	type;
		unsigned char	reserved[3];

		unsigned int	current[3];
	};

	// CUP_I_STARTNPCTRADE -----------------------------------------------------------------------
	// Note: Items are added to this dialog later on, through CUP_I_NEWSLOTINFO packets.
	const unsigned char		NPCTRADEFLAG_ITEMSTORAGE	= 0x1;	// the character is "trading" with his own storage
	struct SCUPIStartNPCTrade
	{
		unsigned char	type;
		unsigned char	padding;

		unsigned char	flags;
		unsigned char	currency;	// normal, menit or iwid
		unsigned short	buyratio;	// how much items costs to buy FROM the NPC
		unsigned short	sellratio;	// how much items are worth selling TO the NPC

		int				entity;		// whom we're trading with
	};

	// CUP_I_REMOVENPCITEM ---------------------------------------------------------------------
	struct SSCUPIRemoveNPCItem
	{
		unsigned char	type;
		unsigned char	buffer[3];

		unsigned int	itemid;		// which item to remove
	};

	// CUP_I_DISCARDITEM ----------------------------------------------------------------------
	struct SCUPIDiscardItem
	{
		unsigned char	type;
		unsigned char	padding[3];

		unsigned int	itemid;		// which item to discard (is located in the backpack)
	};

	// CUP_I_OPENCRAFT ----------------------------------------------------------------------
	struct SCUPIOpenCraft
	{
		unsigned char	type;
		unsigned char	buffer[2];
		unsigned char	currency;	// normal, menit or iwid

		int				entity;		// whom we're trading with
	};

	// CUP_I_NEWCRAFTITEM ----------------------------------------------------------------------
	const unsigned char CRAFTITEMFLAG_SECRET	= 0x1;	// the results from this NPC is secret
	struct SCUPINewCraftItem
	{
		unsigned char	type;

		unsigned char	flags;
		unsigned char	padding[2];		// 1 is equipment, 2 is backpack

		RpgSystem::SItemSlot	iteminfo;	// item slot info
	};

}

#endif
