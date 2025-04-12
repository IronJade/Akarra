using System;
using Akarra.Basic;
using Akarra.FileSystem;

namespace Akarra.PlayerSystem
{
    /// <summary>
    /// Character file versions
    /// </summary>
    public enum CharacterFileVersions
    {
        Original = 0,
        CHRFV_020505,    // Added stamina and hunger to SBaseStats and a flags integer to the character file
        CHRFV_020530,    // Added damina reduction and level 50-time
        CHRFV_020611,    // Added first version of CCharacterInventory
        CHRFV_020622,    // Added items to the CCharacterInventory
        CHRFV_020629,    // Added respawn point position
        CHRFV_020810,    // Added stackable items
        CHRFV_020818,    // Added new skill system
        CHRFV_020908,    // Converted coin system and decreased max inventory capacity to 30 items
        CHRFV_020913,    // Added CharacterQuests
        CHRFV_021002,    // Added new character graphics with poses and hair styles
        CHRFV_021003,    // Added eye colors too
        CHRFV_021009,    // Added account name to character file
        CHRFV_021022,    // Added "runeffects" list (CChrEffect)
        CHRFV_021102,    // Added a slot for mounts in the character's inventory
        CHRFV_021208,    // Added guild info
        CHRFV_030102,    // Added mount coordinates in the item instances
        CHRFV_030118,    // Added u_muteseconds
        CHRFV_030204,    // Changed the skillsystem
        CHRFV_030219,    // Added custom names to item instances
        CHRFV_030301,    // Added player-killer victim list and id numbers for effects
        CHRFV_030405,    // Added item storage to inventory
        CHRFV_030506,    // Added last guild message time and all characters prior to this version will have their APs reset
        CHRFV_030508,    // Skills now have 10 gems per row instead of 5
        
        CurrentVersion
    }
    
    /// <summary>
    /// Constants for character handling
    /// </summary>
    public static class Character
    {
        public const int NameLength = 16;    // Length of a character name
        
        public const string Folder = "characters/";
        public const string Extension = ".achr";
        public const string Folder2 = "characters";  // For FileSystem.CountFiles() and similar
        public const string SearchPattern = "*.achr"; // For FileSystem.CountFiles() and similar
        public const string DeletedExtension = ".~chr"; // File extension for deleted characters
        
        public const int StartX = 1056;
        public const int StartY = 7332;
        
        // Character Flags
        public const int RetroTimeBonus = 0x1;    // Is set if the retro-active time-bonus has been added
        public const int StartItems = 0x2;        // Is set when the character has received his starting items
        public const int AppearanceSet = 0x4;     // Is set when the character has legally set his appearance
        public const int RebirthUsed = 0x8;       // Set when the player has used the "rebirth" feature
        public const int TorchesAdded = 0x10;     // Added two free torches (non-dropable) for all old pA13 characters
        
        // Player Information flags (uses 0x00FF0000 of the bits)
        public const int UnlockLevel = 0x10000;       // Makes it possible for people to see the char's level
        public const int UnlockEmail = 0x20000;       // -"- to see the char's email
        public const int UnlockPosition = 0x40000;    // -"- to see the char's world position
        public const int NoStatistics = 0x80000;      // This flag removes the character from the statistics page
        
        /// <summary>
        /// Character Actions (max 255)
        /// </summary>
        public enum Actions
        {
            Idle = 0,        // No action
            
            // World interaction actions
            Stepping,        // Taking a single step
            Moving,          // Continously moving (making several steps in a row)
            Looking,         // Examining an object
            Talking,         // Talking or trading with another character/npc
            
            Follow,
            MoveTo,
            
            // Combat actions:
            Attacking,       // The character is in combat with an enemy
            Striking,        // ... is currently making a melee attack attempt (part of Attacking)
            Shooting,        // ... is currently firing a arrow/bolt/bullet (part of Attacking)
            Casting,         // ... is currently casting a spell (part of Idle)
            
            Rest,
            
            // Item actions
            DropItem,
            MoveItem,
            UseItem,
            LookItem,
            
            // Skill actions
            AddSkillGem,
            RemoveSkillGem,
            SpecializeSkill,
            
            LookNpcItem,
            
            CastTarget,      // Cast spell on target
            CastGround,      // Cast spell on ground
            
            NumCharacterActions
        }
        
        /// <summary>
        /// Character Movement Directions
        /// </summary>
        public enum Directions
        {
            Stop = 0,    // Don't move
            
            Right,
            UpRight,
            Up,
            UpLeft,
            Left,
            DownLeft,
            Down,
            DownRight
        }
        
        // Direction tables for movement
        public static readonly int[] DirectionX = { 0, 1, 1, 0, -1, -1, -1, 0, 1 };
        public static readonly int[] DirectionY = { 0, 0, -1, -1, -1, 0, 1, 1, 1 };
        
        // Command characters
        public const char Command1 = '/';
        public const char Command2 = '.';
        
        /// <summary>
        /// Skin colors for character appearance
        /// </summary>
        public enum SkinColors
        {
            Pale = 0,
            Sunburned,
            Chestnut,
            Dark,
            
            NumSkinColors
        }
        
/// <summary>
/// Eye colors for character appearance
/// </summary>
public enum EyeColors
{
    Blue = 0,
    Green,
    Cyan,
    Brown,
    Black,
    White,
            
    NumEyeColors
}

/// <summary>
/// Hair colors for character appearance
/// </summary>
public enum HairColors
{
    White = 0,
    Grey,
    Brown,
    Yellow,
    LightBrown,
    Black,
    Red,
    Orange,
            
    NumHairColors
}
    
// There are 3 bits for hair styles (8 different styles)
}

/// <summary>
/// Represents a minimal character's guild state
/// </summary>
public class CharacterGuildState
{
    public string GuildName { get; set; }
    public int State { get; set; }
        
    public CharacterGuildState()
    {
        GuildName = string.Empty;
        State = 0; // Invalid
    }
}

/// <summary>
/// This class holds most information about a character
/// </summary>
public class CharacterFile
{
    // Character basic info
    public string Name { get; set; }
    public string AccountName { get; set; }
    public RpgSystem.CharacterAttributes Attributes { get; set; }
    public RpgSystem.CharacterInventory Inventory { get; set; }
    public RpgSystem.CharacterQuests Quests { get; set; }
        
    // Position
    public int GlobalX { get; set; }
    public int GlobalY { get; set; }
    public int Steps { get; set; }
    public int OnlineTime { get; set; }
    public int Level50Time { get; set; }
        
    // Flags and state
    public int Flags { get; set; }
    public int MuteSeconds { get; set; }
        
    // Respawn point
    public int RespawnX { get; set; }
    public int RespawnY { get; set; }
        
    // Appearance info
    public int BodyPose { get; set; }
    public int SkinColor { get; set; }
    public int EyeColor { get; set; }
    public int HairColor { get; set; }
    public int HairStyle { get; set; }
        
    // Guild info
    public CharacterGuildState Guild { get; set; }
    public long LastGuildMessageRead { get; set; }
        
    // Effect info
    public int NextEffectId { get; set; }
        
    public CharacterFile()
    {
        Clear();
    }
        
    ~CharacterFile()
    {
        RemoveAll();
    }
        
    public void RemoveAll()
    {
        Attributes?.RemoveAll();
        Inventory?.RemoveAll();
        Quests?.RemoveAll();
        Clear();
    }
        
    public bool Open(FileStream file)
    {
        // Read the file version
        int version = file.ReadInt();
            
        // Read the attributes first
        if (!Attributes.Read(file, version))
        {
            return false;
        }
            
        // Now read the inventory
        if (!Inventory.Read(file, version))
        {
            return false;
        }
            
        if (!Quests.Read(file, version))
        {
            return false;
        }
            
        // Read the name
        byte[] nameBuffer = new byte[Character.NameLength];
        file.Read(nameBuffer, Character.NameLength);
        Name = System.Text.Encoding.UTF8.GetString(nameBuffer).TrimEnd('\0');
            
        // Read the world position
        GlobalX = file.ReadInt();
        GlobalY = file.ReadInt();
        Steps = file.ReadInt();
        OnlineTime = file.ReadInt();
            
        if (version > (int)CharacterFileVersions.CHRFV_020505)
        {
            Flags = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_020530)
        {
            Level50Time = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_030118)
        {
            MuteSeconds = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_020629)
        {
            RespawnX = file.ReadInt();
            RespawnY = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_021002)
        {
            BodyPose = file.ReadInt();
            SkinColor = file.ReadInt();
            HairColor = file.ReadInt();
            HairStyle = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_021003)
        {
            EyeColor = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_021009)
        {
            byte[] accountBuffer = new byte[16];
            file.Read(accountBuffer, 16);
            AccountName = System.Text.Encoding.UTF8.GetString(accountBuffer).TrimEnd('\0');
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_021208)
        {
            byte[] guildBuffer = new byte[32];
            file.Read(guildBuffer, 32);
            Guild.GuildName = System.Text.Encoding.UTF8.GetString(guildBuffer).TrimEnd('\0');
            Guild.State = file.ReadInt();
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_030506)
        {
            // C++ uses long for this which is typically 4 bytes on 32-bit systems
            // In C# we'll use long (Int64) since that's 8 bytes like a C++ long on 64-bit systems
            byte[] longBytes = new byte[8]; // Assuming 8-byte long
            file.Read(longBytes, 8);
            LastGuildMessageRead = BitConverter.ToInt64(longBytes, 0);
        }
            
        if (version > (int)CharacterFileVersions.CHRFV_030301)
        {
            NextEffectId = file.ReadInt();
        }
            
        // In A1 there was a major attribute reset for all characters...
        if (version <= (int)CharacterFileVersions.CHRFV_030506)
        {
            ResetCharacterAttributes();
        }
            
        return true;
    }
        
    public bool Write(FileStream file)
    {
        // Store the file version
        int version = (int)CharacterFileVersions.CurrentVersion;
        file.Write(version);
            
        // Save the attributes first
        if (!Attributes.Write(file, version))
        {
            return false;
        }
            
        // Save inventory
        if (!Inventory.Write(file))
        {
            return false;
        }
            
        if (!Quests.Write(file))
        {
            return false;
        }
            
        // Save the name
        byte[] nameBytes = new byte[Character.NameLength];
        System.Text.Encoding.UTF8.GetBytes(Name).CopyTo(nameBytes, 0);
        file.Write(nameBytes, nameBytes.Length);
            
        // Save the world position
        file.Write(GlobalX);
        file.Write(GlobalY);
        file.Write(Steps);
        file.Write(OnlineTime);
            
        file.Write(Flags);
        file.Write(Level50Time);
        file.Write(MuteSeconds);
            
        file.Write(RespawnX);
        file.Write(RespawnY);
            
        file.Write(BodyPose);
        file.Write(SkinColor);
        file.Write(HairColor);
        file.Write(HairStyle);
        file.Write(EyeColor);
            
        byte[] accountBytes = new byte[16];
        System.Text.Encoding.UTF8.GetBytes(AccountName).CopyTo(accountBytes, 0);
        file.Write(accountBytes, accountBytes.Length);
            
        byte[] guildBytes = new byte[32];
        System.Text.Encoding.UTF8.GetBytes(Guild.GuildName).CopyTo(guildBytes, 0);
        file.Write(guildBytes, guildBytes.Length);
        file.Write(Guild.State);
            
        // Write the 8-byte long value
        byte[] longBytes = BitConverter.GetBytes(LastGuildMessageRead);
        file.Write(longBytes, longBytes.Length);
            
        file.Write(NextEffectId);
            
        return true;
    }
        
    /// <summary>
    /// Sets up a completely fresh character to the starting values
    /// </summary>
    public void SetNewCharacter()
    {
        float[] characterStartingAttributes = {
            // Humans
            15.0f, 15.0f, 15.0f, 15.0f, 15.0f,
            // Menits
            23.0f, 21.0f, 18.0f, 18.0f, 20.0f,
            // Iwids
            18.0f, 18.0f, 21.0f, 23.0f, 20.0f
        };
            
        for (int i = 0; i < (int)RpgSystem.BaseAttributes.NumAttributes; i++)
        {
            Attributes.Stats.Attributes[i].SetBase(
                characterStartingAttributes[i + (Attributes.Stats.Race * (int)RpgSystem.BaseAttributes.NumAttributes)]);
        }
            
        // Start in hope:
        GlobalX = Character.StartX;
        GlobalY = Character.StartY;
        RespawnX = Character.StartX;
        RespawnY = Character.StartY;
            
        // Start with a stone of each color
        Attributes.Stats.FreeGems[0] = 1;
        Attributes.Stats.FreeGems[1] = 1;
        Attributes.Stats.FreeGems[2] = 1;
        Attributes.Stats.FreeGems[3] = 1;
        Attributes.Stats.FreeGems[4] = 1;
        Attributes.Stats.TotalGems = 5;
            
        RecalculateCharacterStats(null);
        for (int i = 0; i < (int)RpgSystem.VarAttributes.NumVarAttributes; i++)
        {
            Attributes.Stats.Variables[i].Restore();
        }
    }
        
    /// <summary>
    /// This function will use the current base values of the five attributes 
    /// to determine how many free attribute points the character should have.
    /// </summary>
    public void ResetCharacterAttributes()
    {
        float[] characterStartingAttributes = {
            // Humans
            15.0f, 15.0f, 15.0f, 15.0f, 15.0f,
            // Menits
            23.0f, 21.0f, 18.0f, 18.0f, 20.0f,
            // Iwids
            18.0f, 18.0f, 21.0f, 23.0f, 20.0f
        };
            
        int attributes = Attributes.Stats.NewPoints;
        {
            for (int i = 0; i < (int)RpgSystem.BaseAttributes.NumAttributes; i++)
            {
                attributes += (int)(
                    Attributes.Stats.Attributes[i].GetBase() -
                    characterStartingAttributes[i + (Attributes.Stats.Race * (int)RpgSystem.BaseAttributes.NumAttributes)]
                );
                    
                // Reset attribute to starting value
                Attributes.Stats.Attributes[i].SetBase(
                    characterStartingAttributes[i + (Attributes.Stats.Race * (int)RpgSystem.BaseAttributes.NumAttributes)]
                );
            }
        }
        Attributes.Stats.NewPoints = attributes;
    }
        
    /// <summary>
    /// Recalculates all attribute bonuses, abilities, skills, spells
    /// items etc based on the current character state
    /// </summary>
    public void RecalculateCharacterStats(RpgSystem.IAttributeObject source)
    {
        RpgSystem.BaseStats stats = Attributes.Stats;
            
        float[] damageList = new float[(int)RpgSystem.VarAttributes.NumVarAttributes];
        for (int i = 0; i < (int)RpgSystem.VarAttributes.NumVarAttributes; i++)
        {
            damageList[i] = stats.Variables[i].GetCurrent();
            damageList[i] = stats.Variables[i].GetValue() - damageList[i];
            stats.Variables[i].ResetBonus();
        }
            
        // Reset attribute bonuses
        for (int i = 0; i < (int)RpgSystem.BaseAttributes.NumAttributes; i++)
        {
            stats.Attributes[i].ResetBonus();
        }
            
        for (int i = 0; i < (int)RpgSystem.Abilities.NumAbilities; i++)
        {
            stats.Abilities[i].ResetBonus();
        }
            
        stats.AttackSpeed = 0;
        stats.MoveSpeedMultiplier = 1.0f;
        stats.PersonalLight = 2;
        stats.PublicLight = 0;
            
        // Calculate stats depending on equipped items
        // For simplicity, this implementation assumes RpgSystem.ItemInstance and related classes are defined elsewhere
        // The actual implementation would need to apply item effects and bonuses
            
        // The code here would be similar to the original C++ implementation, applying item effects
        // from equipped items to the character's stats
            
        // Check all skill effects
        // For simplicity, this implementation assumes RpgSystem.Skill and related classes are defined elsewhere
            
        // Check all runable effects
        // For simplicity, this implementation assumes RpgSystem.Effect and related classes are defined elsewhere
            
        // Recalculate abilities
        stats.Abilities[(int)RpgSystem.Abilities.AttackRating].SetBase(stats.Attributes[(int)RpgSystem.BaseAttributes.Agility].GetValue() * 0.75f);
        stats.Abilities[(int)RpgSystem.Abilities.CastRating].SetBase(stats.Attributes[(int)RpgSystem.BaseAttributes.Intelligence].GetValue() * 0.40f);
        stats.Abilities[(int)RpgSystem.Abilities.DamageMin].SetBase(1.5f + stats.Attributes[(int)RpgSystem.BaseAttributes.Strength].GetValue() * 0.02f);
        stats.Abilities[(int)RpgSystem.Abilities.DamageMax].SetBase(4.0f + stats.Attributes[(int)RpgSystem.BaseAttributes.Strength].GetValue() * 0.04f);
            
        bool strengthDamage = true; // This value would be set based on equipped weapons
        if (strengthDamage)
        {
            stats.Abilities[(int)RpgSystem.Abilities.DamageMin].AddPercentBonus(stats.Attributes[(int)RpgSystem.BaseAttributes.Strength].GetValue() * 0.0050f);
            stats.Abilities[(int)RpgSystem.Abilities.DamageMax].AddPercentBonus(stats.Attributes[(int)RpgSystem.BaseAttributes.Strength].GetValue() * 0.0050f);
        }
            
        stats.Abilities[(int)RpgSystem.Abilities.ArmorClass].SetBase(stats.Attributes[(int)RpgSystem.BaseAttributes.Agility].GetValue() * 0.25f);
        stats.Abilities[(int)RpgSystem.Abilities.ResistWhite].SetBase(3.0f + stats.Attributes[(int)RpgSystem.BaseAttributes.Faith].GetValue() * 0.125f);
        stats.Abilities[(int)RpgSystem.Abilities.ResistRed].SetBase(3.0f + stats.Attributes[(int)RpgSystem.BaseAttributes.Constitution].GetValue() * 0.125f);
        stats.Abilities[(int)RpgSystem.Abilities.ResistGreen].SetBase(3.0f + stats.Attributes[(int)RpgSystem.BaseAttributes.Intelligence].GetValue() * 0.125f);
        stats.Abilities[(int)RpgSystem.Abilities.ResistBlue].SetBase(3.0f + stats.Attributes[(int)RpgSystem.BaseAttributes.Constitution].GetValue() * 0.125f);
        stats.Abilities[(int)RpgSystem.Abilities.ResistYellow].SetBase(3.0f + stats.Attributes[(int)RpgSystem.BaseAttributes.Constitution].GetValue() * 0.125f);
        stats.Abilities[(int)RpgSystem.Abilities.Reduction].SetBase(0);
            
        // Increase the magic resistance, that matches the character's soul color, by 10%
        stats.Abilities[(int)RpgSystem.Abilities.ResistWhite + stats.SoulColor].AddPercentBonus(0.1f);
            
        // Recalculate variable attributes (remember the amount of damage, though)
        stats.Variables[(int)RpgSystem.VarAttributes.Health].SetBase(stats.Attributes[(int)RpgSystem.BaseAttributes.Constitution].GetValue() * 2.0f);
        stats.Variables[(int)RpgSystem.VarAttributes.Mana].SetBase(stats.Attributes[(int)RpgSystem.BaseAttributes.Faith].GetValue() * 1.5f);
        stats.Variables[(int)RpgSystem.VarAttributes.Stamina].SetBase(stats.Attributes[(int)RpgSystem.BaseAttributes.Constitution].GetValue() * 3.0f);
            
        // Remove the damage value again
        for (int i = 0; i < (int)RpgSystem.VarAttributes.NumVarAttributes; i++)
        {
            stats.Variables[i].Restore();
            stats.Variables[i].AddToCurrent(-damageList[i]);
        }
            
        // Recalculate inventory capacities
        // This would call a method on the inventory object
    }
        
    /// <summary>
    /// Applies an effect to the character
    /// </summary>
    public void ApplyEffect(RpgSystem.EffectType effect, RpgSystem.IAttributeObject source, float castRating = 0)
    {
        // This method would apply various effects to the character
        // For simplicity, this implementation assumes RpgSystem.EffectType and related classes are defined elsewhere
    }
        
    private void Clear()
    {
        Name = string.Empty;
        AccountName = string.Empty;
        GlobalX = 0;
        GlobalY = 0;
        Steps = 0;
        OnlineTime = 0;
        Level50Time = 0;
        Flags = 0;
        MuteSeconds = 0;
            
        RespawnX = Character.StartX;
        RespawnY = Character.StartY;
            
        BodyPose = 0;
        SkinColor = 0;
        EyeColor = 0;
        HairColor = 0;
        HairStyle = 0;
            
        Guild = new CharacterGuildState();
        LastGuildMessageRead = 0;
            
        NextEffectId = 1;
            
        Attributes = new RpgSystem.CharacterAttributes();
        Inventory = new RpgSystem.CharacterInventory();
        Quests = new RpgSystem.CharacterQuests();
    }
}

/// <summary>
/// Minimal character information structure for networking
/// </summary>
public class MinimumCharacterInfo
{
    public string Name { get; set; }
    public RpgSystem.SmallStats Stats { get; set; }
    public RpgSystem.SmallInventory Inventory { get; set; }
    public int GlobalX { get; set; }
    public int GlobalY { get; set; }
    public byte ExpProgressType { get; set; }
    public byte ExpProgress { get; set; }
    public byte GfxLibrary { get; set; }
    public byte SkinColor { get; set; }
    public byte HairStyle { get; set; }
    public byte Mount { get; set; }
    public sbyte PublicLight { get; set; }
    public sbyte PersonalLight { get; set; }
    public uint BodyAppearance { get; set; }
    public uint WeaponAppearance { get; set; }
    public uint ShieldAppearance { get; set; }
    public uint HelmAppearance { get; set; }
    public uint HandsAppearance { get; set; }
    public uint FeetAppearance { get; set; }
        
    public MinimumCharacterInfo()
    {
        Name = string.Empty;
        Stats = new RpgSystem.SmallStats();
        Inventory = new RpgSystem.SmallInventory();
    }
}