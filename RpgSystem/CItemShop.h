/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A definition for NPC shops.
 **/

#ifndef _CITEMSHOP_H_
#define _CITEMSHOP_H_

#include <vector>
#include "../library/basic/CCharString.h"

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{

	// a weighed item in a shop
	struct SShopItem
	{
		char	itemname[32];
		int		weight;

		SShopItem();
		SShopItem(char* name, int w);
	};
	typedef std::vector<SShopItem*>		TDShopItemVec;

	// a possible craft combination
	struct SShopCraftRecepy
	{
		char	ingredients[5][32];
		char	result[32];

		SShopCraftRecepy();
	};
	typedef std::vector<SShopCraftRecepy*>		TDShopCraftVec;

	enum EShopCraftCostTypes
	{
		SCCT_FREE = 0,		// no extra cost (except the items in the recepy)
		SCCT_RESULTVALUE,	// you have to pay the value of the constructed item
		SCCT_DIFFERENCE,	// the cost is the value of the constructed item, minus the value of the recepy items
	};

	enum EItemShopFileVersions
	{
		ISFV_ORIGINAL = 0,

		ISFV_CURRENT
	};

	class CItemShop
	{
		public:

			CItemShop();
			~CItemShop();

			void clear();
			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			void setShopName(char* name);
			char* getShopName();

			void copyOther(CItemShop* other);

			// shop items:

			TDShopItemVec& getSellItems();
			TDShopItemVec& getBuyItems();

			void addSellItem(char* name, int weight);
			void removeSellItem(int index);
			SShopItem* getSellItem(int index);
			SShopItem* getRandomSellItem();

			void addBuyItem(char* name);
			void removeBuyItem(char* name);
			void removeBuyItem(int index);
			int getMainTypeBuyFlags();
			void setMainTypeBuyFlags(int flags);
			int getSubTypeBuyFlags(int maintype);
			void setSubTypeBuyFlags(int maintype, int flags);
			bool wantsToBuyItem(char* name, int maintype, int subtype);

			int getSellWeight();
			void recalculateSellWeight();

			int getMinimumItems();
			void setMinimumItems(int value);
			int getMaximumItems();
			void setMaximumItems(int value);
			int getAddItemRate();
			void setAddItemRate(int value);

			int getCoinType();
			void setCoinType(int value);
			float getSellCostMultiplier();
			void setSellCostMultiplier(float value);
			float getBuyValueMultiplier();
			void setBuyValueMultiplier(float value);

			// crafting:

			TDShopCraftVec& getCraftRecepies();

			SShopCraftRecepy* addNewRecepy();
			SShopCraftRecepy* getRecepy(int index);
			void removeRecepy(int index);

			int getCraftCostType();
			void setCraftCostType(int value);
			float getCraftCostMultiplier();
			void setCraftCostMultiplier(float value);

			bool secretCrafting();
			void setSecrecy(bool value);

			char* getCraftedItem(char* ing[5]);

		protected:

			char			ma_shopname[32];		// shop identifier

			int				m_cointype;				// what kind of currency used here
			float			m_sellcostmultiplier;	// from shop to player multiplier
			float			m_buyvaluemultiplier;	// from player to shop multiplier

			TDShopItemVec	m_shoplist;				// list of items that the shop can sell
			int				m_totalweight;			// calculated "weight" of all items
			TDShopItemVec	m_buylist;				// items the shop wants to buy from the player
			int				m_buymaintype;			// maintype flags the shop accepts
			int				ma_buysubtypes[32];		// subtype flags for each maintype

			int				m_restockcount;			// how many items that are added when the shop updates
			int				m_maxstock;				// maximum number of items in the shop
			int				m_minstock;				// minimum items in the shop (will fill up to this value)

			TDShopCraftVec	m_craftlist;			// list of craft recepies
			int				m_craftcosttype;		// what kind of cost the shop will calculate
			float			m_craftcostmultiplier;	// multiplier to that cost
			
			int				m_flags;				// various shop flags (see flags below)

		public:

			Basic::CCharString	u_designnotes;
	};

	const int	SHOPFLAG_SECRETCRAFTING	= 0x1;	// the players will not be able to see what the results are
};

#endif
