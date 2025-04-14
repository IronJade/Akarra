using System;
using System.Collections.Generic;
using System.IO;

namespace RpgSystem.Items
{
    /// <summary>
    /// File version constants for item shops
    /// </summary>
    public static class ItemShopFileVersions
    {
        public const int Original = 0;
        public const int Current = Original;
    }

    /// <summary>
    /// Cost calculation types for crafting items
    /// </summary>
    public enum ShopCraftCostType
    {
        Free = 0,           // No extra cost (except the items in the recipe)
        ResultValue,        // Pay the value of the constructed item
        Difference          // Cost is value of constructed item minus value of recipe items
    }

    /// <summary>
    /// Flag constants for item shops
    /// </summary>
    [Flags]
    public enum ShopFlags
    {
        SecretCrafting = 0x1    // Players cannot see recipe results
    }

    /// <summary>
    /// Represents an item that a shop sells or buys
    /// </summary>
    public class ShopItem
    {
        public string ItemName { get; set; }
        public int Weight { get; set; }     // For random selection

        /// <summary>
        /// Creates a new shop item with default values
        /// </summary>
        public ShopItem()
        {
            ItemName = string.Empty;
            Weight = 1;
        }

        /// <summary>
        /// Creates a new shop item with specified values
        /// </summary>
        public ShopItem(string name, int weight = 1)
        {
            ItemName = name ?? string.Empty;
            Weight = weight;
        }
    }

    /// <summary>
    /// Represents a crafting recipe in a shop
    /// </summary>
    public class ShopCraftRecipe
    {
        public const int MaxIngredients = 5;

        public string[] Ingredients { get; private set; }
        public string Result { get; set; }

        /// <summary>
        /// Creates a new shop crafting recipe
        /// </summary>
        public ShopCraftRecipe()
        {
            Ingredients = new string[MaxIngredients];
            Result = string.Empty;
        }
    }

    /// <summary>
    /// Defines an item shop/merchant in the game
    /// </summary>
    public class ItemShop
    {
        // Shop identification
        public string ShopName { get; set; }

        // Shop items
        private List<ShopItem> _shopList;
        private int _totalWeight;

        // Shop buy settings
        private List<ShopItem> _buyList;
        private int _buyMainType;
        private int[] _buySubTypes;

        // Shop currency settings
        public float SellCostMultiplier { get; set; }  // Shop to player
        public float BuyValueMultiplier { get; set; }  // Player to shop
        public int CoinType { get; set; }

        // Shop stock settings
        public int RestockCount { get; set; }  // Items added when shop restocks
        public int MaxStock { get; set; }      // Maximum number of items
        public int MinStock { get; set; }      // Minimum number of items

        // Shop crafting
        private List<ShopCraftRecipe> _craftList;
        public ShopCraftCostType CraftCostType { get; set; }
        public float CraftCostMultiplier { get; set; }

        // Shop flags
        public ShopFlags Flags { get; set; }

        // Other information
        public string DesignerNotes { get; set; }

        /// <summary>
        /// Creates a new item shop
        /// </summary>
        public ItemShop()
        {
            _shopList = new List<ShopItem>();
            _buyList = new List<ShopItem>();
            _buySubTypes = new int[32];
            _craftList = new List<ShopCraftRecipe>();

            Clear();
        }

        /// <summary>
        /// Resets all shop properties to default values
        /// </summary>
        public void Clear()
        {
            ShopName = string.Empty;

            _shopList.Clear();
            _totalWeight = 0;
            SellCostMultiplier = 1.6f;
            BuyValueMultiplier = 0.3f;
            _buyList.Clear();
            _buyMainType = 0;

            for (int i = 0; i < 32; i++)
            {
                _buySubTypes[i] = 0;
            }

            RestockCount = 6;
            MaxStock = 20;
            MinStock = 0;

            _craftList.Clear();
            CraftCostType = ShopCraftCostType.Difference;
            CraftCostMultiplier = 1.6f;

            Flags = 0;
            DesignerNotes = string.Empty;
            CoinType = 0;
        }

        /// <summary>
        /// Removes all shop data and frees resources
        /// </summary>
        public void RemoveAll()
        {
            _shopList.Clear();
            _buyList.Clear();
            _craftList.Clear();
            Clear();
        }

        /// <summary>
        /// Reads shop data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                // Remove old data
                RemoveAll();

                // Read version
                int version = reader.ReadInt32();

                // Read shop settings
                ShopName = new string(reader.ReadChars(32)).TrimEnd('\0');
                SellCostMultiplier = reader.ReadSingle();
                BuyValueMultiplier = reader.ReadSingle();
                CoinType = reader.ReadInt32();
                _buyMainType = reader.ReadInt32();

                for (int i = 0; i < 32; i++)
                {
                    _buySubTypes[i] = reader.ReadInt32();
                }

                RestockCount = reader.ReadInt32();
                MaxStock = reader.ReadInt32();
                MinStock = reader.ReadInt32();
                Flags = (ShopFlags)reader.ReadInt32();

                CraftCostMultiplier = reader.ReadSingle();
                CraftCostType = (ShopCraftCostType)reader.ReadInt32();

                // Read item lists
                int count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                {
                    ShopItem item = new ShopItem
                    {
                        ItemName = new string(reader.ReadChars(32)).TrimEnd('\0'),
                        Weight = reader.ReadInt32()
                    };
                    _shopList.Add(item);
                }

                count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                {
                    ShopItem item = new ShopItem
                    {
                        ItemName = new string(reader.ReadChars(32)).TrimEnd('\0'),
                        Weight = reader.ReadInt32()
                    };
                    _buyList.Add(item);
                }

                count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                {
                    ShopCraftRecipe recipe = new ShopCraftRecipe();
                    
                    for (int j = 0; j < ShopCraftRecipe.MaxIngredients; j++)
                    {
                        recipe.Ingredients[j] = new string(reader.ReadChars(32)).TrimEnd('\0');
                    }
                    
                    recipe.Result = new string(reader.ReadChars(32)).TrimEnd('\0');
                    _craftList.Add(recipe);
                }

                // Read designer notes
                int notesLength = reader.ReadInt32();
                if (notesLength > 0)
                {
                    char[] notesChars = reader.ReadChars(notesLength);
                    DesignerNotes = new string(notesChars);
                }

                RecalculateSellWeight();

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes shop data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write(ItemShopFileVersions.Current);

                // Write shop settings
                WriteFixedString(writer, ShopName, 32);
                writer.Write(SellCostMultiplier);
                writer.Write(BuyValueMultiplier);
                writer.Write(CoinType);
                writer.Write(_buyMainType);

                for (int i = 0; i < 32; i++)
                {
                    writer.Write(_buySubTypes[i]);
                }

                writer.Write(RestockCount);
                writer.Write(MaxStock);
                writer.Write(MinStock);
                writer.Write((int)Flags);

                writer.Write(CraftCostMultiplier);
                writer.Write((int)CraftCostType);

                // Write item lists
                writer.Write(_shopList.Count);
                foreach (var item in _shopList)
                {
                    WriteFixedString(writer, item.ItemName, 32);
                    writer.Write(item.Weight);
                }

                writer.Write(_buyList.Count);
                foreach (var item in _buyList)
                {
                    WriteFixedString(writer, item.ItemName, 32);
                    writer.Write(item.Weight);
                }

                writer.Write(_craftList.Count);
                foreach (var recipe in _craftList)
                {
                    for (int i = 0; i < ShopCraftRecipe.MaxIngredients; i++)
                    {
                        WriteFixedString(writer, recipe.Ingredients[i], 32);
                    }
                    WriteFixedString(writer, recipe.Result, 32);
                }

                // Write designer notes
                if (string.IsNullOrEmpty(DesignerNotes))
                {
                    writer.Write(0);
                }
                else
                {
                    writer.Write(DesignerNotes.Length);
                    writer.Write(DesignerNotes.ToCharArray());
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Sets the shop name
        /// </summary>
        public void SetShopName(string name)
        {
            ShopName = name ?? string.Empty;
        }

        /// <summary>
        /// Gets the shop name
        /// </summary>
        public string GetShopName()
        {
            return ShopName;
        }

        /// <summary>
        /// Copies data from another shop
        /// </summary>
        public void CopyFrom(ItemShop other)
        {
            if (other == null)
                return;

            // Remember original shop name
            string oldName = ShopName;
            RemoveAll();
            ShopName = oldName;

            // Copy lists
            foreach (var item in other._shopList)
            {
                _shopList.Add(new ShopItem(item.ItemName, item.Weight));
            }

            foreach (var item in other._buyList)
            {
                _buyList.Add(new ShopItem(item.ItemName, item.Weight));
            }

            foreach (var recipe in other._craftList)
            {
                ShopCraftRecipe newRecipe = new ShopCraftRecipe();
                
                for (int i = 0; i < ShopCraftRecipe.MaxIngredients; i++)
                {
                    newRecipe.Ingredients[i] = recipe.Ingredients[i];
                }
                
                newRecipe.Result = recipe.Result;
                _craftList.Add(newRecipe);
            }

            _totalWeight = other._totalWeight;
            SellCostMultiplier = other.SellCostMultiplier;
            BuyValueMultiplier = other.BuyValueMultiplier;
            _buyMainType = other._buyMainType;
            
            for (int i = 0; i < 32; i++)
            {
                _buySubTypes[i] = other._buySubTypes[i];
            }

            RestockCount = other.RestockCount;
            MaxStock = other.MaxStock;
            MinStock = other.MinStock;

            CraftCostType = other.CraftCostType;
            CraftCostMultiplier = other.CraftCostMultiplier;

            Flags = other.Flags;
            DesignerNotes = other.DesignerNotes;
            CoinType = other.CoinType;
        }

        #region Sell Items

        /// <summary>
        /// Gets all items the shop sells
        /// </summary>
        public List<ShopItem> GetSellItems()
        {
            return _shopList;
        }

        /// <summary>
        /// Adds an item the shop sells
        /// </summary>
        public void AddSellItem(string name, int weight = 1)
        {
            ShopItem item = new ShopItem(name, weight);
            _shopList.Add(item);

            RecalculateSellWeight();
        }

        /// <summary>
        /// Removes an item the shop sells by index
        /// </summary>
        public void RemoveSellItem(int index)
        {
            if (index < 0 || index >= _shopList.Count)
                return;

            _shopList.RemoveAt(index);

            RecalculateSellWeight();
        }

        /// <summary>
        /// Gets a sell item by index
        /// </summary>
        public ShopItem GetSellItem(int index)
        {
            if (index < 0 || index >= _shopList.Count)
                return null;

            return _shopList[index];
        }

        /// <summary>
        /// Gets a random sell item based on weights
        /// </summary>
        public ShopItem GetRandomSellItem()
        {
            if (_shopList.Count == 0 || _totalWeight <= 0)
                return null;

            int value = new Random().Next(_totalWeight);

            foreach (var item in _shopList)
            {
                value -= item.Weight;
                if (value < 0)
                {
                    return item;
                }
            }

            // Should never reach here
            return null;
        }

        /// <summary>
        /// Gets the total weight of all sell items
        /// </summary>
        public int GetSellWeight()
        {
            return _totalWeight;
        }

        /// <summary>
        /// Recalculates the total weight of all sell items
        /// </summary>
        public void RecalculateSellWeight()
        {
            _totalWeight = 0;
            foreach (var item in _shopList)
            {
                _totalWeight += item.Weight;
            }
        }

        #endregion

        #region Buy Items

        /// <summary>
        /// Gets all items the shop buys
        /// </summary>
        public List<ShopItem> GetBuyItems()
        {
            return _buyList;
        }

        /// <summary>
        /// Adds an item the shop buys
        /// </summary>
        public void AddBuyItem(string name)
        {
            // Check for duplicates
            foreach (var item in _buyList)
            {
                if (string.Compare(item.ItemName, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return;
                }
            }

            ShopItem item = new ShopItem(name, 1);
            _buyList.Add(item);
        }

        /// <summary>
        /// Removes an item the shop buys by name
        /// </summary>
        public void RemoveBuyItem(string name)
        {
            for (int i = 0; i < _buyList.Count; i++)
            {
                if (string.Compare(_buyList[i].ItemName, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _buyList.RemoveAt(i);
                    return;
                }
            }
        }

        /// <summary>
        /// Removes an item the shop buys by index
        /// </summary>
        public void RemoveBuyItem(int index)
        {
            if (index < 0 || index >= _buyList.Count)
                return;

            _buyList.RemoveAt(index);
        }

        /// <summary>
        /// Gets the main type buy flags
        /// </summary>
        public int GetMainTypeBuyFlags()
        {
            return _buyMainType;
        }

        /// <summary>
        /// Sets the main type buy flags
        /// </summary>
        public void SetMainTypeBuyFlags(int flags)
        {
            _buyMainType = flags;
        }

        /// <summary>
        /// Gets the subtype buy flags for a specific main type
        /// </summary>
        public int GetSubTypeBuyFlags(int mainType)
        {
            if (mainType < 0 || mainType >= 32)
                return 0;

            return _buySubTypes[mainType];
        }

        /// <summary>
        /// Sets the subtype buy flags for a specific main type
        /// </summary>
        public void SetSubTypeBuyFlags(int mainType, int flags)
        {
            if (mainType < 0 || mainType >= 32)
                return;

            _buySubTypes[mainType] = flags;
        }

        /// <summary>
        /// Checks if the shop wants to buy a specific item
        /// </summary>
        public bool WantsToBuyItem(string name, int mainType, int subType)
        {
            // Check type flags first
            if (mainType >= 0 && mainType < 32)
            {
                if ((_buyMainType & (1 << mainType)) != 0)
                {
                    // Covered by main type flags
                    return true;
                }

                if ((_buySubTypes[mainType] & (1 << subType)) != 0)
                {
                    return true;
                }
            }

            // Check specific buy list
            if (!string.IsNullOrEmpty(name))
            {
                foreach (var item in _buyList)
                {
                    if (string.Compare(item.ItemName, name, StringComparison.OrdinalIgnoreCase) == 0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        #endregion

        #region Shop Stock

        /// <summary>
        /// Gets the minimum number of items the shop should have
        /// </summary>
        public int GetMinimumItems()
        {
            return MinStock;
        }

        /// <summary>
        /// Sets the minimum number of items the shop should have
        /// </summary>
        public void SetMinimumItems(int value)
        {
            MinStock = value;
        }

        /// <summary>
        /// Gets the maximum number of items the shop can have
        /// </summary>
        public int GetMaximumItems()
        {
            return MaxStock;
        }

        /// <summary>
        /// Sets the maximum number of items the shop can have
        /// </summary>
        public void SetMaximumItems(int value)
        {
            MaxStock = value;
        }

        /// <summary>
        /// Gets the number of items added when restocking
        /// </summary>
        public int GetAddItemRate()
        {
            return RestockCount;
        }

        /// <summary>
        /// Sets the number of items added when restocking
        /// </summary>
        public void SetAddItemRate(int value)
        {
            RestockCount = value;
        }

        #endregion

        #region Shop Currency

        /// <summary>
        /// Gets the coin type used by this shop
        /// </summary>
        public int GetCoinType()
        {
            return CoinType;
        }

        /// <summary>
        /// Sets the coin type used by this shop
        /// </summary>
        public void SetCoinType(int value)
        {
            CoinType = value;
        }

        /// <summary>
        /// Gets the multiplier applied to items the shop sells
        /// </summary>
        public float GetSellCostMultiplier()
        {
            return SellCostMultiplier;
        }

        /// <summary>
        /// Sets the multiplier applied to items the shop sells
        /// </summary>
        public void SetSellCostMultiplier(float value)
        {
            SellCostMultiplier = value;
        }

        /// <summary>
        /// Gets the multiplier applied to items the shop buys
        /// </summary>
        public float GetBuyValueMultiplier()
        {
            return BuyValueMultiplier;
        }

        /// <summary>
        /// Sets the multiplier applied to items the shop buys
        /// </summary>
        public void SetBuyValueMultiplier(float value)
        {
            BuyValueMultiplier = value;
        }

        #endregion

        #region Crafting

        /// <summary>
        /// Gets all crafting recipes
        /// </summary>
        public List<ShopCraftRecipe> GetCraftRecipes()
        {
            return _craftList;
        }

        /// <summary>
        /// Creates a new crafting recipe
        /// </summary>
        public ShopCraftRecipe AddNewRecipe()
        {
            ShopCraftRecipe recipe = new ShopCraftRecipe();
            _craftList.Add(recipe);
            return recipe;
        }

        /// <summary>
        /// Gets a crafting recipe by index
        /// </summary>
        public ShopCraftRecipe GetRecipe(int index)
        {
            if (index < 0 || index >= _craftList.Count)
                return null;

            return _craftList[index];
        }

        /// <summary>
        /// Removes a crafting recipe by index
        /// </summary>
        public void RemoveRecipe(int index)
        {
            if (index < 0 || index >= _craftList.Count)
                return;

            _craftList.RemoveAt(index);
        }

        /// <summary>
        /// Gets the crafting cost type
        /// </summary>
        public ShopCraftCostType GetCraftCostType()
        {
            return CraftCostType;
        }

        /// <summary>
        /// Sets the crafting cost type
        /// </summary>
        public void SetCraftCostType(ShopCraftCostType value)
        {
            CraftCostType = value;
        }

        /// <summary>
        /// Gets the crafting cost multiplier
        /// </summary>
        public float GetCraftCostMultiplier()
        {
            return CraftCostMultiplier;
        }

        /// <summary>
        /// Sets the crafting cost multiplier
        /// </summary>
        public void SetCraftCostMultiplier(float value)
        {
            CraftCostMultiplier = value;
        }

        /// <summary>
        /// Checks if the shop has secret crafting
        /// </summary>
        public bool HasSecretCrafting()
        {
            return Flags.HasFlag(ShopFlags.SecretCrafting);
        }

        /// <summary>
        /// Sets whether the shop has secret crafting
        /// </summary>
        public void SetSecretCrafting(bool value)
        {
            if (value)
            {
                Flags |= ShopFlags.SecretCrafting;
            }
            else
            {
                Flags &= ~ShopFlags.SecretCrafting;
            }
        }

        /// <summary>
        /// Looks up a crafting recipe by ingredients
        /// </summary>
        public string GetCraftedItem(string[] ingredients)
        {
            if (ingredients == null || ingredients.Length != 5)
                return null;

            // Count empty slots in ingredients
            int emptySlots = 0;
            foreach (string ing in ingredients)
            {
                if (string.IsNullOrEmpty(ing))
                {
                    emptySlots++;
                }
            }

            if (emptySlots >= 5)
                return null;

            // Check each recipe
            foreach (var recipe in _craftList)
            {
                // Count empty slots in recipe
                int emptyRecipe = 0;
                bool[] taken = new bool[5];

                for (int i = 0; i < 5; i++)
                {
                    if (string.IsNullOrEmpty(recipe.Ingredients[i]))
                    {
                        taken[i] = true;
                        emptyRecipe++;
                    }
                    else
                    {
                        taken[i] = false;
                    }
                }

                // If recipes have different numbers of ingredients, they can't match
                if (emptyRecipe != emptySlots)
                    continue;

                // Try to match all ingredients
                foreach (string ing in ingredients)
                {
                    if (!string.IsNullOrEmpty(ing))
                    {
                        // Look for this ingredient in the recipe
                        bool found = false;
                        for (int j = 0; j < 5; j++)
                        {
                            if (!taken[j])
                            {
                                if (string.Compare(ing, recipe.Ingredients[j], StringComparison.OrdinalIgnoreCase) == 0)
                                {
                                    // Found a match
                                    taken[j] = true;
                                    found = true;
                                    break;
                                }
                            }
                        }

                        if (!found)
                        {
                            // Couldn't find this ingredient in the recipe
                            break;
                        }
                    }
                }

                // Check if all ingredients were matched
                bool allMatched = true;
                for (int i = 0; i < 5; i++)
                {
                    if (!taken[i])
                    {
                        allMatched = false;
                        break;
                    }
                }

                if (allMatched)
                {
                    return recipe.Result;
                }
            }

            return null;
        }

        #endregion

        #region Helper Methods

        /// <summary>
        /// Writes a fixed-length string to a binary writer
        /// </summary>
        private static void WriteFixedString(BinaryWriter writer, string text, int length)
        {
            char[] chars = new char[length];
            if (!string.IsNullOrEmpty(text))
            {
                for (int i = 0; i < Math.Min(text.Length, length); i++)
                {
                    chars[i] = text[i];
                }
            }
            writer.Write(chars);
        }

        #endregion
    }
}