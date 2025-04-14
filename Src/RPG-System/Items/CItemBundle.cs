using System;
using System.Collections.Generic;
using System.IO;
using RpgSystem.Items;

namespace RpgSystem.Items
{
    /// <summary>
    /// File version constants for ItemTypeBundle
    /// </summary>
    public static class ItemTypeBundleVersions
    {
        public const int Original = 0;
        public const int V_021119 = 1;   // Added item families
        public const int V_030403 = 2;   // Added item shops
        
        public const int Current = V_030403;
    }
    
    /// <summary>
    /// File version constants for ItemTypeFamily
    /// </summary>
    public static class ItemTypeFamilyVersions
    {
        public const int Original = 0;
        public const int V_021123 = 1;   // Added designer's notes and changed the requirement data
        
        public const int Current = V_021123;
    }

    /// <summary>
    /// Manages a collection of item types, families, and shops
    /// </summary>
    public class ItemTypeBundle
    {
        private List<ItemType> _items;
        private List<ItemTypeFamily> _families;
        private List<ItemShop> _shops;

        /// <summary>
        /// Creates a new item type bundle
        /// </summary>
        public ItemTypeBundle()
        {
            _items = new List<ItemType>();
            _families = new List<ItemTypeFamily>();
            _shops = new List<ItemShop>();
        }

        /// <summary>
        /// Removes all item types, families, and shops
        /// </summary>
        public void RemoveAll()
        {
            _items.Clear();
            _families.Clear();
            _shops.Clear();
        }

        /// <summary>
        /// Reads item type bundle from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                // Clear existing data
                RemoveAll();

                // Read version
                int version = reader.ReadInt32();

                // Read item types
                int count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                {
                    ItemType item = new ItemType();
                    if (!item.Read(reader))
                    {
                        return false;
                    }
                    _items.Add(item);
                }

                // Read item families (if version supports it)
                if (version > ItemTypeBundleVersions.V_021119)
                {
                    count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        ItemTypeFamily family = new ItemTypeFamily();
                        if (!family.Read(reader))
                        {
                            return false;
                        }
                        _families.Add(family);
                    }
                }

                // Read item shops (if version supports it)
                if (version > ItemTypeBundleVersions.V_030403)
                {
                    count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        ItemShop shop = new ItemShop();
                        if (!shop.Read(reader))
                        {
                            return false;
                        }
                        _shops.Add(shop);
                    }
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes item type bundle to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write(ItemTypeBundleVersions.Current);

                // Write item types
                writer.Write(_items.Count);
                foreach (var item in _items)
                {
                    if (!item.Write(writer))
                    {
                        return false;
                    }
                }

                // Write item families
                writer.Write(_families.Count);
                foreach (var family in _families)
                {
                    if (!family.Write(writer))
                    {
                        return false;
                    }
                }

                // Write item shops
                writer.Write(_shops.Count);
                foreach (var shop in _shops)
                {
                    if (!shop.Write(writer))
                    {
                        return false;
                    }
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Opens item type bundle from a file
        /// </summary>
        public bool Open(string filename)
        {
            try
            {
                using (FileStream fileStream = System.IO.File.OpenRead(filename))
                using (BinaryReader reader = new BinaryReader(fileStream))
                {
                    return Read(reader);
                }
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Saves item type bundle to a file
        /// </summary>
        public bool Save(string filename)
        {
            try
            {
                using (FileStream fileStream = System.IO.File.Create(filename))
                using (BinaryWriter writer = new BinaryWriter(fileStream))
                {
                    return Write(writer);
                }
            }
            catch (Exception)
            {
                return false;
            }
        }

        #region Item Types

        /// <summary>
        /// Gets all item types
        /// </summary>
        public List<ItemType> GetItems()
        {
            return _items;
        }

        /// <summary>
        /// Gets the number of item types
        /// </summary>
        public int GetNumItems()
        {
            return _items.Count;
        }

        /// <summary>
        /// Finds an item type by name
        /// </summary>
        public ItemType GetItem(string dataName)
        {
            if (string.IsNullOrEmpty(dataName))
                return null;

            foreach (var item in _items)
            {
                if (string.Compare(item.DataName, dataName, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return item;
                }
            }

            return null;
        }

        /// <summary>
        /// Creates a new item type
        /// </summary>
        public ItemType CreateNewItem()
        {
            ItemType item = new ItemType();
            _items.Add(item);
            return item;
        }

        /// <summary>
        /// Adds an existing item type
        /// </summary>
        public bool AddItem(ItemType item)
        {
            if (item == null)
                return false;

            // Check if an item with this name already exists
            if (GetItem(item.DataName) != null)
                return false;

            _items.Add(item);
            return true;
        }

        /// <summary>
        /// Removes an item type by name
        /// </summary>
        public bool DeleteItem(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            for (int i = 0; i < _items.Count; i++)
            {
                if (string.Compare(_items[i].DataName, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _items.RemoveAt(i);
                    return true;
                }
            }

            return false;
        }

        #endregion

        #region Item Families

        /// <summary>
        /// Gets all item families
        /// </summary>
        public List<ItemTypeFamily> GetFamilies()
        {
            return _families;
        }

        /// <summary>
        /// Gets the number of item families
        /// </summary>
        public int GetNumFamilies()
        {
            return _families.Count;
        }

        /// <summary>
        /// Finds an item family by name
        /// </summary>
        public ItemTypeFamily GetFamily(string dataName)
        {
            if (string.IsNullOrEmpty(dataName))
                return null;

            foreach (var family in _families)
            {
                if (string.Compare(family.Name, dataName, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return family;
                }
            }

            return null;
        }

        /// <summary>
        /// Adds an existing item family
        /// </summary>
        public bool AddFamily(ItemTypeFamily family)
        {
            if (family == null)
                return false;

            // Check if a family with this name already exists
            if (GetFamily(family.Name) != null)
                return false;

            _families.Add(family);
            return true;
        }

        /// <summary>
        /// Removes an item family by name
        /// </summary>
        public bool DeleteFamily(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            for (int i = 0; i < _families.Count; i++)
            {
                if (string.Compare(_families[i].Name, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _families.RemoveAt(i);
                    return true;
                }
            }

            return false;
        }

        #endregion

        #region Item Shops

        /// <summary>
        /// Gets all item shops
        /// </summary>
        public List<ItemShop> GetShops()
        {
            return _shops;
        }

        /// <summary>
        /// Gets the number of item shops
        /// </summary>
        public int GetNumShops()
        {
            return _shops.Count;
        }

        /// <summary>
        /// Finds an item shop by name
        /// </summary>
        public ItemShop GetShop(string shopName)
        {
            if (string.IsNullOrEmpty(shopName))
                return null;

            foreach (var shop in _shops)
            {
                if (string.Compare(shop.GetShopName(), shopName, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return shop;
                }
            }

            return null;
        }

        /// <summary>
        /// Gets an item shop by index
        /// </summary>
        public ItemShop GetShop(int index)
        {
            if (index < 0 || index >= _shops.Count)
                return null;
                
            return _shops[index];
        }

        /// <summary>
        /// Creates a new item shop
        /// </summary>
        public ItemShop AddNewShop(string shopName)
        {
            if (string.IsNullOrEmpty(shopName))
                return null;

            // Check if a shop with this name already exists
            if (GetShop(shopName) != null)
                return null;

            ItemShop shop = new ItemShop();
            shop.SetShopName(shopName);
            _shops.Add(shop);

            return shop;
        }

        /// <summary>
        /// Removes an item shop by name
        /// </summary>
        public bool DeleteShop(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            for (int i = 0; i < _shops.Count; i++)
            {
                if (string.Compare(_shops[i].GetShopName(), name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _shops.RemoveAt(i);
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Removes an item shop by index
        /// </summary>
        public bool DeleteShop(int index)
        {
            if (index < 0 || index >= _shops.Count)
                return false;

            _shops.RemoveAt(index);
            return true;
        }

        #endregion
    }

    /// <summary>
    /// Represents a family or category of items with shared properties
    /// </summary>
    public class ItemTypeFamily
    {
        // Identification
        public string Name { get; set; }

        // Stat Modifiers
        public float[] BaseParams { get; private set; }  // Base parameters
        public float[] MulParams { get; private set; }   // Multiplier parameters
        public float Value { get; set; }                 // Base value modifier

        // Equipment Properties
        public int[] Ranges { get; private set; }        // Range values for equipment
        public int ActionDelay { get; set; }             // Action delay for weapons
        public int RecoveryDelay { get; set; }           // Recovery delay for weapons

        // Requirements
        public int[] ReqTypes { get; private set; }      // Requirement types
        public float[] ReqMulti { get; private set; }    // Requirement multipliers
        public int[] ReqBase { get; private set; }       // Requirement base values

        // Other
        public string Notes { get; set; }                // Designer notes

        /// <summary>
        /// Creates a new item type family
        /// </summary>
        public ItemTypeFamily()
        {
            BaseParams = new float[3];
            MulParams = new float[3];
            Ranges = new int[3];
            ReqTypes = new int[3];
            ReqMulti = new float[3];
            ReqBase = new int[3];

            Clear();
        }

        /// <summary>
        /// Resets all properties to default values
        /// </summary>
        public void Clear()
        {
            Name = string.Empty;

            for (int i = 0; i < 3; i++)
            {
                BaseParams[i] = 0;
                MulParams[i] = 0;
                Ranges[i] = 0;
                ReqTypes[i] = -1;
                ReqMulti[i] = 0;
                ReqBase[i] = 0;
            }

            Value = 1.0f;
            ActionDelay = 1000;
            RecoveryDelay = 1000;
            Notes = string.Empty;
        }

        /// <summary>
        /// Reads family data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                // Read version
                int version = reader.ReadInt32();

                // Read name
                Name = new string(reader.ReadChars(32)).TrimEnd('\0');

                // Read parameters
                for (int i = 0; i < 3; i++)
                {
                    BaseParams[i] = reader.ReadSingle();
                }
                for (int i = 0; i < 3; i++)
                {
                    MulParams[i] = reader.ReadSingle();
                }
                Value = reader.ReadSingle();

                // Read ranges
                for (int i = 0; i < 3; i++)
                {
                    Ranges[i] = reader.ReadInt32();
                }
                ActionDelay = reader.ReadInt32();
                RecoveryDelay = reader.ReadInt32();

                // Read requirements
                for (int i = 0; i < 3; i++)
                {
                    ReqTypes[i] = reader.ReadInt32();
                }
                for (int i = 0; i < 3; i++)
                {
                    ReqMulti[i] = reader.ReadSingle();
                }
                for (int i = 0; i < 3; i++)
                {
                    ReqBase[i] = reader.ReadInt32();
                }

                // Read notes (if version supports it)
                if (version > ItemTypeFamilyVersions.V_021123)
                {
                    int notesLength = reader.ReadInt32();
                    if (notesLength > 0)
                    {
                        Notes = new string(reader.ReadChars(notesLength));
                    }
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes family data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write(ItemTypeFamilyVersions.Current);

                // Write name
                WriteFixedString(writer, Name, 32);

                // Write parameters
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(BaseParams[i]);
                }
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(MulParams[i]);
                }
                writer.Write(Value);

                // Write ranges
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(Ranges[i]);
                }
                writer.Write(ActionDelay);
                writer.Write(RecoveryDelay);

                // Write requirements
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(ReqTypes[i]);
                }
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(ReqMulti[i]);
                }
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(ReqBase[i]);
                }

                // Write notes
                if (string.IsNullOrEmpty(Notes))
                {
                    writer.Write(0);
                }
                else
                {
                    writer.Write(Notes.Length);
                    writer.Write(Notes.ToCharArray());
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Helper method to write a fixed-length string
        /// </summary>
        private void WriteFixedString(BinaryWriter writer, string text, int length)
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
    }
}