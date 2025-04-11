using System;
using System.Collections.Generic;
using System.IO;
using RpgSystem.Items;
using RpgSystem.Character;
using RpgSystem.Effects;

namespace RpgSystem.Character
{
    /// <summary>
    /// Equipment slots available to characters
    /// </summary>
    public enum EquipmentSlot
    {
        RightHand = 0,   // Weapon hand
        LeftHand,        // Shield hand
        Body,            // Armor
        Head,            // Helms/headgear
        Hands,           // Gloves
        Feet,            // Boots etc.
        Neck,            // Necklaces or amulets
        RightRing,       // Right ring
        LeftRing,        // Left ring
        Mount,           // The player's mount
        
        Count
    }

    /// <summary>
    /// Manages a character's inventory including equipment, backpack and storage
    /// </summary>
    public class CharacterInventory
    {
        /// <summary>
        /// Constants for the inventory system
        /// </summary>
        public static class Constants
        {
            public const int BackpackSize = 30;  // Number of slots in backpack
        }

        // Money
        public int[] Coins { get; private set; }          // Current money
        public int[] MaxCoins { get; private set; }       // Carry capacity
        
        public int MaxBackpackSlots { get; set; }         // Number of slots character may use
        public ItemInstance[] Equipment { get; private set; }
        public ItemInstance[] Backpack { get; private set; }
        
        // Character item storage
        public List<ItemInstance> StorageList { get; private set; }

        public CharacterInventory()
        {
            Coins = new int[Money.Constants.MoneyTypeCount];
            MaxCoins = new int[Money.Constants.MoneyTypeCount];
            Equipment = new ItemInstance[(int)EquipmentSlot.Count];
            Backpack = new ItemInstance[Constants.BackpackSize];
            StorageList = new List<ItemInstance>();
        }

        /// <summary>
        /// Clears all inventory data
        /// </summary>
        private void Clear()
        {
            // Clear coins
            for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
            {
                Coins[i] = 0;
                MaxCoins[i] = 0;
            }

            // Clear equipment
            for (int i = 0; i < (int)EquipmentSlot.Count; i++)
            {
                Equipment[i] = null;
            }

            // Clear backpack
            for (int i = 0; i < Constants.BackpackSize; i++)
            {
                Backpack[i] = null;
            }

            // Clear storage
            StorageList.Clear();
            
            MaxBackpackSlots = Constants.BackpackSize;
        }

        /// <summary>
        /// Removes all inventory data and frees resources
        /// </summary>
        public void RemoveAll()
        {
            // Free all item instances
            for (int i = 0; i < (int)EquipmentSlot.Count; i++)
            {
                Equipment[i] = null;
            }

            for (int i = 0; i < Constants.BackpackSize; i++)
            {
                Backpack[i] = null;
            }

            StorageList.Clear();
            Clear();
        }

        /// <summary>
        /// Reads inventory data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader, int version = 0)
        {
            try
            {
                // Clear existing data
                RemoveAll();

                // Read coins
                for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
                {
                    Coins[i] = reader.ReadInt32();
                    MaxCoins[i] = reader.ReadInt32();
                }

                // Read max backpack slots
                MaxBackpackSlots = reader.ReadInt32();

                // Read equipment
                for (int i = 0; i < (int)EquipmentSlot.Count; i++)
                {
                    bool hasItem = reader.ReadBoolean();
                    if (hasItem)
                    {
                        Equipment[i] = new ItemInstance();
                        Equipment[i].Read(reader, version);
                    }
                }

                // Read backpack
                for (int i = 0; i < Constants.BackpackSize; i++)
                {
                    bool hasItem = reader.ReadBoolean();
                    if (hasItem)
                    {
                        Backpack[i] = new ItemInstance();
                        Backpack[i].Read(reader, version);
                    }
                }

                // Read storage
                int storageCount = reader.ReadInt32();
                for (int i = 0; i < storageCount; i++)
                {
                    ItemInstance item = new ItemInstance();
                    item.Read(reader, version);
                    StorageList.Add(item);
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes inventory data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write coins
                for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
                {
                    writer.Write(Coins[i]);
                    writer.Write(MaxCoins[i]);
                }

                // Write max backpack slots
                writer.Write(MaxBackpackSlots);

                // Write equipment
                for (int i = 0; i < (int)EquipmentSlot.Count; i++)
                {
                    bool hasItem = Equipment[i] != null;
                    writer.Write(hasItem);
                    if (hasItem)
                    {
                        Equipment[i].Write(writer);
                    }
                }

                // Write backpack
                for (int i = 0; i < Constants.BackpackSize; i++)
                {
                    bool hasItem = Backpack[i] != null;
                    writer.Write(hasItem);
                    if (hasItem)
                    {
                        Backpack[i].Write(writer);
                    }
                }

                // Write storage
                writer.Write(StorageList.Count);
                foreach (var item in StorageList)
                {
                    item.Write(writer);
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Calculates maximum carrying capacities based on character stats
        /// </summary>
        public void CalculateMaxCapacities(BaseStats stats)
        {
            // Calculate maximum coins based on level
            for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
            {
                MaxCoins[i] = Money.MoneyCalculator.GetMaxCarry((int)stats.Level, i);
            }
        }

        /// <summary>
        /// Gets the appropriate equipment slot for an item
        /// </summary>
        public static int GetItemEquipSlot(ItemInstance item)
        {
            if (item == null || item.Type == null)
                return -1;
            
            switch (item.Type.MainType)
            {
                case (int)ItemMainType.Weapon:
                    return (int)EquipmentSlot.RightHand;
                
                case (int)ItemMainType.Shield:
                    return (int)EquipmentSlot.LeftHand;
                
                case (int)ItemMainType.Armor:
                    return (int)EquipmentSlot.Body;
                
                case (int)ItemMainType.Helm:
                    return (int)EquipmentSlot.Head;
                
                case (int)ItemMainType.Gloves:
                    return (int)EquipmentSlot.Hands;
                
                case (int)ItemMainType.Boots:
                    return (int)EquipmentSlot.Feet;
                
                case (int)ItemMainType.Neck:
                    return (int)EquipmentSlot.Neck;
                
                case (int)ItemMainType.Ring:
                    return (int)EquipmentSlot.RightRing; // Default to right ring
                
                case (int)ItemMainType.Mount:
                    return (int)EquipmentSlot.Mount;
                
                default:
                    return -1; // Not equippable
            }
        }
    }

    /// <summary>
    /// Small structure for network transmission of inventory item info
    /// </summary>
    public class ItemSlot
    {
        public string Name { get; set; }
        public uint Id { get; set; }
        public uint EquippedSprite { get; set; }
        public int Value { get; set; }
        
        public ItemSlot()
        {
            Name = "";
            Id = 0;
            EquippedSprite = 0;
            Value = 0;
        }

        /// <summary>
        /// Sets item information from an item instance
        /// </summary>
        public void SetItemInformation(ItemInstance item)
        {
            if (item == null)
            {
                Name = "";
                Id = 0;
                EquippedSprite = 0;
                Value = 0;
                return;
            }

            if (item.Type != null)
            {
                Name = item.GetItemDisplayName();
                EquippedSprite = item.Type.EquippedSprite;
                Value = item.Type.Value;
            }
            else
            {
                Name = item.TypeName;
                EquippedSprite = 0;
                Value = 0;
            }
            
            Id = item.GetId();
        }
    }

    /// <summary>
    /// Smaller version of inventory for network transmission
    /// </summary>
    public class SmallInventory
    {
        public int[] Coins { get; private set; }
        public int[] MaxCoins { get; private set; }
        public int MaxBackpackSize { get; set; }
        public ItemSlot[] Equipment { get; private set; }
        public ItemSlot[] Backpack { get; private set; }

        public SmallInventory()
        {
            Coins = new int[Money.Constants.MoneyTypeCount];
            MaxCoins = new int[Money.Constants.MoneyTypeCount];
            Equipment = new ItemSlot[(int)EquipmentSlot.Count];
            Backpack = new ItemSlot[CharacterInventory.Constants.BackpackSize];
            
            // Initialize all item slots
            for (int i = 0; i < (int)EquipmentSlot.Count; i++)
            {
                Equipment[i] = new ItemSlot();
            }
            
            for (int i = 0; i < CharacterInventory.Constants.BackpackSize; i++)
            {
                Backpack[i] = new ItemSlot();
            }
        }

        /// <summary>
        /// Sets inventory data from full inventory
        /// </summary>
        public void SetInventory(CharacterInventory inventory)
        {
            // Copy money
            for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
            {
                Coins[i] = inventory.Coins[i];
                MaxCoins[i] = inventory.MaxCoins[i];
            }

            MaxBackpackSize = inventory.MaxBackpackSlots;

            // Copy equipment
            for (int i = 0; i < (int)EquipmentSlot.Count; i++)
            {
                Equipment[i].SetItemInformation(inventory.Equipment[i]);
            }

            // Copy backpack
            for (int i = 0; i < CharacterInventory.Constants.BackpackSize; i++)
            {
                Backpack[i].SetItemInformation(inventory.Backpack[i]);
            }
        }
    }
}