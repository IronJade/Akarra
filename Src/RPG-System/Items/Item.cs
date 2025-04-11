using System;
using System.IO;
using RpgSystem.Effects;

namespace RpgSystem.Items
{
    /// <summary>
    /// File version constants for the item system
    /// </summary>
    public static class ItemFileVersions
    {
        public const int Original = 0;
        public const int V_020908 = 1;    // Converted the value of items to the new system
        public const int V_020925 = 2;    // Changed item value from unsigned short to int
        public const int V_021010 = 3;    // Added another color value for equipped/unequipped
        public const int V_021118 = 4;    // Added item recipes and display name and designer's notes
        public const int V_030214 = 5;    // Added special string
        public const int V_030407 = 6;    // Added effect chance
        public const int V_030504 = 7;    // Great weapon and monster rebalance
        public const int V_030504b = 8;   // Added decay minutes
        public const int V_030519 = 9;    // Test speed change
        
        public const int Current = V_030519;
    }

    /// <summary>
    /// Main item types
    /// </summary>
    public enum ItemMainType
    {
        General = 0,    // General-purpose items (money, junk, etc.)
        
        // Equipment items
        Weapon,
        Shield,
        Armor,          // Armor and clothing
        Helm,
        Gloves,
        Boots,
        Ring,           // Note: two slots
        Neck,           // Amulets and necklaces
        
        // Other
        Food,           // All edible items including potions
        Scroll,         // Skill/spell scrolls
        Mount,          // Something that can be ridden
        Ammo,           // Items used for ranged weapons
        Light,          // Torches and lanterns
        
        Count
    }

    /// <summary>
    /// Flags that can be applied to items
    /// </summary>
    [Flags]
    public enum ItemFlag
    {
        None = 0,
        Hidden = 0x1,           // This item requires identification
        TwoHanded = 0x2,        // This item requires both hands to use (weapons)
        Stackable = 0x4,        // Several items of this type can be placed in the same slot
        IgnoreShield = 0x8,     // Used for weapons that ignore the defender's shield AC
        NoVictimEffect = 0x10,  // Used for weapons with two wielder effects instead of one wielder and one victim effect
        Decaying = 0x20,        // This is a decaying item that will self-destruct if used too long
        SingleCarry = 0x40      // A character may never carry more than one of these at a time
    }

    /// <summary>
    /// General subtypes
    /// </summary>
    public enum ItemSubTypeGeneral
    {
        Valuables = 0,
        Junk,
        Material,
        Household,
        Plant,         // A plantable plant
        
        Count
    }

    /// <summary>
    /// Weapon subtypes
    /// </summary>
    public enum ItemSubTypeWeapon
    {
        // Basic weapons
        Dagger = 0,      // Daggers, knives
        SmallCrush,      // Hammers, clubs
        LargeCrush,      // Two-handed hammers, heavy clubs
        Spear,           // Spears, pole-axes, tridents
        
        // Swords
        SmallSword,      // Shortswords, scimitar
        Staff,           // Staves
        LargeSword,      // Two-handed swords
        
        // Axes
        SmallAxe,        // Axes
        LargeAxe,        // Two-handed axes, cleavers
        
        // Ranged weapons
        ShortBow,        // Shortbows, small bows
        LongBow,         // Large bows
        Crossbow,        // Crossbows
        Sling,           // Slings
        Throwing,        // Rocks, throwing daggers
        
        // Other
        NonViolent,      // Can't attack with this
        
        Count
    }

    /// <summary>
    /// Armor subtypes
    /// </summary>
    public enum ItemSubTypeArmor
    {
        Clothing = 0,
        Light,
        Medium,
        Heavy,
        Robe,
        
        Count
    }

    /// <summary>
    /// Shield subtypes
    /// </summary>
    public enum ItemSubTypeShield
    {
        Small = 0,
        Large,
        
        Count
    }

    /// <summary>
    /// Food subtypes
    /// </summary>
    public enum ItemSubTypeFood
    {
        Food = 0,
        Potion,
        Toxic,
        
        Count
    }

    /// <summary>
    /// Ammo subtypes
    /// </summary>
    public enum ItemSubTypeAmmo
    {
        Arrows = 0,
        Bolts,
        Bullets,
        
        Count
    }

    /// <summary>
    /// Recipe for combining items
    /// </summary>
    public class ItemRecipe
    {
        public string Source { get; set; }
        public string Result { get; set; }
        
        public ItemRecipe()
        {
            Source = "";
            Result = "";
        }
        
        public ItemRecipe(string source, string result)
        {
            Source = source;
            Result = result;
        }
    }

    /// <summary>
    /// Defines an item type in the game
    /// </summary>
    public class ItemType
    {
        // Constants
        public const int ItemNameLength = 32;
        public const int ItemPacketCheck = 0x56749676;
        public const int WeaponGroupFlagBasic = 0x0000000F;     // 1111
        public const int WeaponGroupFlagSwords = 0x00000070;    // 0111 0000
        public const int WeaponGroupFlagAxes = 0x00000180;      // 0001 1000
        public const int WeaponGroupFlagRanged = 0x00003E00;    // 0011 1110 0000
        
        // Item identification
        public string DataName { get; set; }          // Used by the server
        public string DisplayName { get; set; }       // Displayed to the character
        
        // Item type
        public int MainType { get; set; }             // Main type is item type (e.g., "weapon")
        public int SubType { get; set; }              // Subtype is specific type (e.g., "sword")
        
        // Visual properties
        public ushort MainSprite { get; set; }        // Sprite used in the interface
        public ushort WorldSprite { get; set; }       // Sprite used when item lies on the ground
        public ushort EquippedSprite { get; set; }    // Sprite used when equipped
        public byte InventoryRed { get; set; }        // Sprite color (inventory)
        public byte InventoryGreen { get; set; }
        public byte InventoryBlue { get; set; }
        public byte EquippedRed { get; set; }         // When equipped or on ground
        public byte EquippedGreen { get; set; }
        public byte EquippedBlue { get; set; }
        
        // Game properties
        public byte ItemLevel { get; set; }
        public int Value { get; set; }                // Item value
        
        // The item parameters are used differently depending on item type:
        // Weapons: param1 is min dmg, param2 is max dmg, param3 is stamina cost
        // Armor: param1 is armor class, param2 is dmg red, param3 is stamina cost
        // Mounts: param1 is life length in minutes
        // Ammo: param1 is stack count (used once), param2 is damage multiplier, param3 is range modifier
        public float Parameter1 { get; set; }
        public float Parameter2 { get; set; }
        public float Parameter3 { get; set; }
        public ItemFlag Flags { get; set; }
        
        // Weapon accuracy variables
        // For weapons: Near is critical chance, Medium is fumble chance
        public byte Near { get; set; }
        public byte Medium { get; set; }
        public byte Far { get; set; }
        
        // Item use delays (it takes time to drink potions too)
        // For mounts: ActionDelay is the speed of the mount (speed to take 2 squares)
        public ushort ActionDelay { get; set; }
        public ushort RecoveryDelay { get; set; }
        
        // Item requirements
        public Requirement[] Requirements { get; private set; }
        
        // Item effects
        public string[] Effects { get; private set; }
        public int EffectChance { get; set; }         // 0-100, chance effect takes place
        
        // Item reaction recipes
        public System.Collections.Generic.List<ItemRecipe> Combinations { get; private set; }
        public System.Collections.Generic.List<ItemRecipe> Reactions { get; private set; }
        
        public string DesignerNotes { get; set; }     // Designer's notes
        public string SpecialString { get; set; }     // Special string for certain items
        public int DecayMinutes { get; set; }         // Used for decaying items
        
        public ItemType()
        {
            DataName = "";
            DisplayName = "";
            Requirements = new Requirement[3];
            Effects = new string[2];
            Combinations = new System.Collections.Generic.List<ItemRecipe>();
            Reactions = new System.Collections.Generic.List<ItemRecipe>();
            DesignerNotes = "";
            SpecialString = "";
            
            for (int i = 0; i < 3; i++)
            {
                Requirements[i] = new Requirement();
            }
            
            for (int i = 0; i < 2; i++)
            {
                Effects[i] = "";
            }
            
            Clear();
        }
        
        /// <summary>
        /// Resets all item properties to defaults
        /// </summary>
        public void Clear()
        {
            DataName = "";
            DisplayName = "";
            MainType = 0;
            SubType = 0;
            
            MainSprite = 0;
            WorldSprite = 0;
            EquippedSprite = 0;
            InventoryRed = 0;
            InventoryGreen = 0;
            InventoryBlue = 0;
            EquippedRed = 0;
            EquippedGreen = 0;
            EquippedBlue = 0;
            
            ItemLevel = 0;
            Value = 0;
            
            Parameter1 = 0;
            Parameter2 = 0;
            Parameter3 = 0;
            Flags = ItemFlag.None;
            
            Near = 0;
            Medium = 0;
            Far = 0;
            
            ActionDelay = 1000;
            RecoveryDelay = 1000;
            
            for (int i = 0; i < 3; i++)
            {
                Requirements[i].Type = -1;
                Requirements[i].Value = 0;
            }
            
            for (int i = 0; i < 2; i++)
            {
                Effects[i] = "";
            }
            
            EffectChance = 100;
            
            Combinations.Clear();
            Reactions.Clear();
            
            DesignerNotes = "";
            SpecialString = "";
            DecayMinutes = 0;
        }
        
        /// <summary>
        /// Reads item type from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                Clear();
                
                // Read file version
                int version = reader.ReadInt32();
                
                // Read name
                DataName = new string(reader.ReadChars(ItemNameLength)).TrimEnd('\0');
                
                if (version > ItemFileVersions.V_021118)
                {
                    DisplayName = new string(reader.ReadChars(ItemNameLength)).TrimEnd('\0');
                }
                else
                {
                    DisplayName = DataName;
                }
                
                // Read type
                MainType = reader.ReadInt32();
                SubType = reader.ReadInt32();
                
                // Read sprite
                MainSprite = reader.ReadUInt16();
                WorldSprite = reader.ReadUInt16();
                EquippedSprite = reader.ReadUInt16();
                InventoryRed = reader.ReadByte();
                InventoryGreen = reader.ReadByte();
                InventoryBlue = reader.ReadByte();
                
                if (version > ItemFileVersions.V_021010)
                {
                    EquippedRed = reader.ReadByte();
                    EquippedGreen = reader.ReadByte();
                    EquippedBlue = reader.ReadByte();
                }
                else
                {
                    EquippedRed = InventoryRed;
                    EquippedGreen = InventoryGreen;
                    EquippedBlue = InventoryBlue;
                }
                
                // Read shop stats
                ItemLevel = reader.ReadByte();
                
                if (version > ItemFileVersions.V_020925)
                {
                    Value = reader.ReadInt32();
                }
                else if (version > ItemFileVersions.V_020908)
                {
                    ushort value = reader.ReadUInt16();
                    Value = value;
                }
                else
                {
                    ushort[] values = new ushort[3];
                    for (int i = 0; i < 3; i++)
                    {
                        values[i] = reader.ReadUInt16();
                    }
                    
                    Value = values[0] * 100 + values[1] * 10 + values[2];
                }
                
                // Read parameters
                Parameter1 = reader.ReadSingle();
                Parameter2 = reader.ReadSingle();
                Parameter3 = reader.ReadSingle();
                Flags = (ItemFlag)reader.ReadInt32();
                
                // Read accuracy
                Near = reader.ReadByte();
                Medium = reader.ReadByte();
                Far = reader.ReadByte();
                
                // Read delays
                ActionDelay = reader.ReadUInt16();
                RecoveryDelay = reader.ReadUInt16();
                
                // Item requirements
                for (int i = 0; i < 3; i++)
                {
                    Requirements[i].Type = reader.ReadInt32();
                    Requirements[i].Value = reader.ReadInt32();
                }
                
                // Item effects
                for (int i = 0; i < 2; i++)
                {
                    Effects[i] = new string(reader.ReadChars(32)).TrimEnd('\0');
                }
                
                if (version > ItemFileVersions.V_030407)
                {
                    EffectChance = reader.ReadInt32();
                }
                
                // Item recipes
                if (version > ItemFileVersions.V_021118)
                {
                    // Combinations
                    int count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        string source = new string(reader.ReadChars(ItemNameLength)).TrimEnd('\0');
                        string result = new string(reader.ReadChars(ItemNameLength)).TrimEnd('\0');
                        Combinations.Add(new ItemRecipe(source, result));
                    }
                    
                    // Reactions
                    count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        string source = new string(reader.ReadChars(ItemNameLength)).TrimEnd('\0');
                        string result = new string(reader.ReadChars(ItemNameLength)).TrimEnd('\0');
                        Reactions.Add(new ItemRecipe(source, result));
                    }
                    
                    // Read designer notes
                    int noteLength = reader.ReadInt32();
                    if (noteLength > 0)
                    {
                        char[] noteChars = reader.ReadChars(noteLength);
                        DesignerNotes = new string(noteChars);
                    }
                }
                
                // Special string
                if (version > ItemFileVersions.V_030214)
                {
                    int specialLength = reader.ReadInt32();
                    if (specialLength > 0)
                    {
                        char[] specialChars = reader.ReadChars(specialLength);
                        SpecialString = new string(specialChars);
                    }
                }
                
                if (version > ItemFileVersions.V_030504b)
                {
                    DecayMinutes = reader.ReadInt32();
                }
                
                // Apply balance changes for older versions
                if (version <= ItemFileVersions.V_030504)
                {
                    if (MainType == (int)ItemMainType.Weapon)
                    {
                        // Min and max damage reduction
                        Parameter1 *= 0.75f;
                        Parameter2 *= 0.75f;
                    }
                }
                
                if (version <= ItemFileVersions.V_030519)
                {
                    if (MainType == (int)ItemMainType.Weapon)
                    {
                        // Recovery delay reduction
                        RecoveryDelay = (ushort)((float)RecoveryDelay * 0.70f);
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
        /// Writes item type to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write file version
                writer.Write((int)ItemFileVersions.Current);
                
                // Write names
                WriteFixedString(writer, DataName, ItemNameLength);
                WriteFixedString(writer, DisplayName, ItemNameLength);
                
                // Write type
                writer.Write(MainType);
                writer.Write(SubType);
                
                // Write sprite
                writer.Write(MainSprite);
                writer.Write(WorldSprite);
                writer.Write(EquippedSprite);
                writer.Write(InventoryRed);
                writer.Write(InventoryGreen);
                writer.Write(InventoryBlue);
                writer.Write(EquippedRed);
                writer.Write(EquippedGreen);
                writer.Write(EquippedBlue);
                
                // Write shop stats
                writer.Write(ItemLevel);
                writer.Write(Value);
                
                // Write parameters
                writer.Write(Parameter1);
                writer.Write(Parameter2);
                writer.Write(Parameter3);
                writer.Write((int)Flags);
                
                // Write accuracy
                writer.Write(Near);
                writer.Write(Medium);
                writer.Write(Far);
                
                // Write delays
                writer.Write(ActionDelay);
                writer.Write(RecoveryDelay);
                
                // Write requirements
                for (int i = 0; i < 3; i++)
                {
                    writer.Write(Requirements[i].Type);
                    writer.Write(Requirements[i].Value);
                }
                
                // Write effects
                for (int i = 0; i < 2; i++)
                {
                    WriteFixedString(writer, Effects[i], 32);
                }
                
                writer.Write(EffectChance);
                
                // Write recipes
                writer.Write(Combinations.Count);
                foreach (var recipe in Combinations)
                {
                    WriteFixedString(writer, recipe.Source, ItemNameLength);
                    WriteFixedString(writer, recipe.Result, ItemNameLength);
                }
                
                writer.Write(Reactions.Count);
                foreach (var recipe in Reactions)
                {
                    WriteFixedString(writer, recipe.Source, ItemNameLength);
                    WriteFixedString(writer, recipe.Result, ItemNameLength);
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
                
                // Write special string
                if (string.IsNullOrEmpty(SpecialString))
                {
                    writer.Write(0);
                }
                else
                {
                    writer.Write(SpecialString.Length);
                    writer.Write(SpecialString.ToCharArray());
                }
                
                writer.Write(DecayMinutes);
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        /// <summary>
        /// Copies properties from another item type
        /// </summary>
        public void CopyFrom(ItemType other)
        {
            if (other == null)
                return;
                
            DataName = other.DataName;
            DisplayName = other.DisplayName;
            MainType = other.MainType;
            SubType = other.SubType;
            
            MainSprite = other.MainSprite;
            WorldSprite = other.WorldSprite;
            EquippedSprite = other.EquippedSprite;
            InventoryRed = other.InventoryRed;
            InventoryGreen = other.InventoryGreen;
            InventoryBlue = other.InventoryBlue;
            EquippedRed = other.EquippedRed;
            EquippedGreen = other.EquippedGreen;
            EquippedBlue = other.EquippedBlue;
            
            ItemLevel = other.ItemLevel;
            Value = other.Value;
            
            Parameter1 = other.Parameter1;
            Parameter2 = other.Parameter2;
            Parameter3 = other.Parameter3;
            Flags = other.Flags;
            
            Near = other.Near;
            Medium = other.Medium;
            Far = other.Far;
            
            ActionDelay = other.ActionDelay;
            RecoveryDelay = other.RecoveryDelay;
            
            for (int i = 0; i < 3; i++)
            {
                Requirements[i].Type = other.Requirements[i].Type;
                Requirements[i].Value = other.Requirements[i].Value;
            }
            
            for (int i = 0; i < 2; i++)
            {
                Effects[i] = other.Effects[i];
            }
            
            EffectChance = other.EffectChance;
            
            Combinations.Clear();
            foreach (var recipe in other.Combinations)
            {
                Combinations.Add(new ItemRecipe(recipe.Source, recipe.Result));
            }
            
            Reactions.Clear();
            foreach (var recipe in other.Reactions)
            {
                Reactions.Add(new ItemRecipe(recipe.Source, recipe.Result));
            }
            
            DesignerNotes = other.DesignerNotes;
            SpecialString = other.SpecialString;
            DecayMinutes = other.DecayMinutes;
        }
        
        /// <summary>
        /// Helper method to write fixed-length strings
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
    
    /// <summary>
    /// Static helper methods for items
    /// </summary>
    public static class ItemHelper
    {
        private static readonly string[] ItemTypeNames = new string[]
        {
            "General",
            "Weapon",
            "Shield",
            "Armor",
            "Helm",
            "Gloves",
            "Boots",
            "Ring",
            "Necklace",
            "Edible",
            "Scroll",
            "Mount",
            "Ammo",
            "Light"
        };
        
        private static readonly string[] WeaponSubtypeNames = new string[]
        {
            "Dagger",
            "Small Crushing",
            "Large Crushing",
            "Spear",
            "Small Sword",
            "Staff",
            "Large Sword",
            "Small Axe",
            "Large Axe",
            "Short Bow",
            "Long Bow",
            "Crossbow",
            "Sling",
            "Throwing",
            "Non-Violent"
        };
        
        private static readonly string[] GeneralSubtypeNames = new string[]
        {
            "Valuables",
            "Junk",
            "Material",
            "Household Item",
            "Plantable"
        };
        
        private static readonly string[] ArmorSubtypeNames = new string[]
        {
            "Clothing",
            "Light",
            "Medium",
            "Heavy",
            "Robe"
        };
        
        private static readonly string[] ShieldSubtypeNames = new string[]
        {
            "Light",
            "Heavy"
        };
        
        private static readonly string[] FoodSubtypeNames = new string[]
        {
            "Food",
            "Potion",
            "Toxic"
        };
        
        private static readonly string[] AmmoSubtypeNames = new string[]
        {
            "Arrows",
            "Bolts",
            "Bullets"
        };
        
        /// <summary>
        /// Gets the name of an item type
        /// </summary>
        public static string GetItemTypeName(int type)
        {
            if (type < 0 || type >= (int)ItemMainType.Count)
                return "Item";
                
            return ItemTypeNames[type];
        }
        
        /// <summary>
        /// Gets the name of an item subtype
        /// </summary>
        public static string GetItemSubName(int type, int subType)
        {
            switch (type)
            {
                case (int)ItemMainType.General:
                    if (subType >= 0 && subType < GeneralSubtypeNames.Length)
                        return GeneralSubtypeNames[subType];
                    break;
                    
                case (int)ItemMainType.Weapon:
                    if (subType >= 0 && subType < WeaponSubtypeNames.Length)
                        return WeaponSubtypeNames[subType];
                    break;
                    
                case (int)ItemMainType.Armor:
                    if (subType >= 0 && subType < ArmorSubtypeNames.Length)
                        return ArmorSubtypeNames[subType];
                    break;
                    
                case (int)ItemMainType.Shield:
                    if (subType >= 0 && subType < ShieldSubtypeNames.Length)
                        return ShieldSubtypeNames[subType];
                    break;
                    
                case (int)ItemMainType.Food:
                    if (subType >= 0 && subType < FoodSubtypeNames.Length)
                        return FoodSubtypeNames[subType];
                    break;
                    
                case (int)ItemMainType.Ammo:
                    if (subType >= 0 && subType < AmmoSubtypeNames.Length)
                        return AmmoSubtypeNames[subType];
                    break;
                    
                case (int)ItemMainType.Gloves:
                    return "Gloves";
                    
                case (int)ItemMainType.Boots:
                    return "Boots";
                    
                case (int)ItemMainType.Ring:
                    return "Ring";
                    
                case (int)ItemMainType.Neck:
                    return "Neckitem";
                    
                case (int)ItemMainType.Mount:
                    return "Mount";
            }
            
            return "Item";
        }
        
        /// <summary>
        /// Gets the maximum stack count for stackable items
        /// </summary>
        public static int GetItemMaxStackCount(int type, int subType)
        {
            if (type == (int)ItemMainType.Ammo)
            {
                return 100;
            }
            return 15;
        }
        
        /// <summary>
        /// Gets the ammo type needed for a weapon
        /// </summary>
        public static int GetWeaponAmmoType(int weaponSubtype)
        {
            switch (weaponSubtype)
            {
                case (int)ItemSubTypeWeapon.ShortBow:
                case (int)ItemSubTypeWeapon.LongBow:
                    return (int)ItemSubTypeAmmo.Arrows;
                    
                case (int)ItemSubTypeWeapon.Crossbow:
                    return (int)ItemSubTypeAmmo.Bolts;
                    
                case (int)ItemSubTypeWeapon.Sling:
                    return (int)ItemSubTypeAmmo.Bullets;
                    
                default:
                    return -1;
            }
        }
    }
}