using System;
using System.Collections.Generic;
using System.IO;
using RpgSystem.Attributes;
using RpgSystem.Effects;

namespace RpgSystem.Monsters
{
    /// <summary>
    /// Constants for monster file management
    /// </summary>
    public static class MonsterConstants
    {
        public const int NameLength = 32;
        public const string Extension = ".mon";
        public const string Directory = "Creatures";
    }

    /// <summary>
    /// File version constants for monster types
    /// </summary>
    public static class MonsterFileVersions
    {
        public const int Original = 0;
        public const int V_020530 = 1;    // Added damage reduction in character abilities
        public const int V_020530b = 2;   // Had forgotten to store monster type
        public const int V_020611 = 3;    // Added money drops
        public const int V_020626 = 4;    // Added item drops
        public const int V_020716 = 5;    // Added attack names and attack chance (ranged attacks)
        public const int V_020726 = 6;    // Added attack effect and spell list
        public const int V_020730 = 7;    // Added u_magictime
        public const int V_020803 = 8;    // Added u_aggression
        public const int V_020909 = 9;    // Changed cointype system
        public const int V_020921 = 10;   // Added chat manuscript id
        public const int V_021018 = 11;   // U_type is now the sprite id instead of monster id (multiply by 2)
        public const int V_021020 = 12;   // Added u_lightradius
        public const int V_021110 = 13;   // Added u_groupteam
        public const int V_021121 = 14;   // Added ua_dataname and ua_displayname
        public const int V_021124 = 15;   // Added u_designnotes
        public const int V_021205 = 16;   // Added u_effectchance
        public const int V_021212 = 17;   // Added chatlists
        public const int V_030120 = 18;   // Made all monsters slightly harder (AR, AC, MinDmg, MaxDmg)
        public const int V_030401 = 19;   // Rebalanced resistances to equal level of monster
        public const int V_030405 = 20;   // Another rebalance
        public const int V_030504 = 21;   // Major item rebalance (decreasing HP for monsters weaker than level 50)
        public const int V_030506 = 22;   // Added multiple item drops
        public const int V_030510 = 23;   // Another major rebalance (boosted monsters because of improved skill system)
        public const int V_030513 = 24;   // Added u_moralehealth
        public const int V_030516 = 25;   // Changed sprite info so each monster has three sprites: idle, move and attack
        public const int V_030519 = 26;   // Test attack speed

        public const int Current = V_030519;
    }

    /// <summary>
    /// Monster group types
    /// </summary>
    public enum MonsterGroup
    {
        Unknown = 0,

        Human,
        Menit,
        Iwid,
        Humanoid,

        Darkling,
        Outsider,    // Demons, angels, or other abstract monster things

        Animal,      // Mammals, birds, fish, reptiles/snakes
        Insect,      // Insects (normal and giant) and similar (such as spiders)

        Undead,      // Skeletons, zombies, wraiths, vampires etc
        Dragon,      // All dragons
        Giant,       // Giants, ogres, minotaurs, titans (large humanoids)
        Plant,       // Living trees, vines or other plants or plant-like creatures
        Fable,       // Griffon, hydra, manticore, chimera, gorgon, medusae etc

        Elemental,   // Elementars, salamanders, tempests etc
        Construct,   // Golems and other mechanical creatures

        Count
    }

    /// <summary>
    /// Monster flag constants
    /// </summary>
    [Flags]
    public enum MonsterFlags
    {
        // AI flags (uses 0x0000FFFF)
        Determined = 0x1,     // This monster will not change target once it has found one
        Social = 0x2,         // Will help other monsters if they are attacked
        MagicUser = 0x4,      // Will prefer to use magic over normal attacks
        ForceSocial = 0x8,    // Will force monsters at the same spawn to help out
        Slayer = 0x10,        // Will always attack the weakest attacker
        NoIdleMove = 0x20,    // This monster stands still while in idle mode

        // Other flags (uses 0xFFFF0000)
        Flying = 0x10000,     // Can move over "block non-flyer" tiles
        Ethereal = 0x20000,   // Can move over "block non-ethereal" tiles
        NPC = 0x40000,        // This monster is a NPC, and cannot be killed
        ShieldUser = 0x80000, // This monster carries a shield (used for certain effects)
        Darkness = 0x100000   // This monster causes darkness around itself
    }

    /// <summary>
    /// Monster behavior types
    /// </summary>
    public enum MonsterBehavior
    {
        Peaceful,   // Will ignore characters unless attacked
        Aggressive, // Will attack characters on sight
        Shy         // Will attempt to avoid characters unless attacked
    }

    /// <summary>
    /// Monster type definition
    /// </summary>
    public class MonsterType
    {
        // Identification
        public string DataName { get; set; }       // Internal name
        public string DisplayName { get; set; }    // Display name shown to players

        // Visual appearance
        public ushort IdleSprite { get; set; }     // Sprite used when the monster is standing still
        public ushort MoveSprite { get; set; }     // Sprite used when moving
        public ushort AttackSprite { get; set; }   // Sprite used when attacking

        // Basic stats
        public byte Level { get; set; }             // Challenge rating
        public byte GroupType { get; set; }         // Out of MonsterGroup enum
        public byte Gender { get; set; }            // 0 is male, 1 is female, 2 is random, 3 is n/a
        public byte Deity { get; set; }             // 0 is gifted, 1 is shining, 2 is none

        // Appearance details
        public byte Color { get; set; }             // Soul color (5 is random)
        public byte Red { get; set; }               // Monster color (for diffuse layer)
        public byte Green { get; set; }
        public byte Blue { get; set; }

        // Teaming
        public int GroupTeam { get; set; }          // Special group this monster belongs to

        // Rewards
        public uint Experience { get; set; }        // Amount of experience this monster is worth

        // Combat stats
        public float[] Abilities { get; private set; }
        public float MaxHealth { get; set; }        // Starting health
        public float MoraleHealth { get; set; }     // Health % where monster tries to flee (0-1)
        public uint MoveSpeed { get; set; }         // Movement speed

        // AI and behavior settings
        public MonsterFlags Flags { get; set; }
        public byte SightRange { get; set; }        // How far away monster can detect enemies
        public MonsterBehavior Behavior { get; set; }
        public int MagicTime { get; set; }          // How often the monster will cast spells (seconds)
        public int Aggression { get; set; }         // How many seconds the monster will fight/hunt a player
        public int LightRadius { get; set; }        // How far the monster spreads light

        // NPC chat settings
        public string NpcChat { get; set; }         // Name of the chat manuscript for NPCs

        // Attack settings
        public byte AttackType { get; set; }
        public byte AttackRange { get; set; }
        public byte AttackChance { get; set; }      // Chance to hit with ranged attacks (%)
        public byte AttackName { get; set; }        // "hits", "bites", "claws" etc
        public uint AttackSpeed { get; set; }
        public float CriticalChance { get; set; }
        public float FumbleChance { get; set; }

        // Attack effects
        public string AttackEffect { get; set; }
        public List<string> SpellEffects { get; private set; }
        public int EffectChance { get; set; }       // Chance for attack effect (%)

        // Loot drops
        public int[] CoinDropChance { get; private set; }   // Chance to drop each coin type
        public int[] CoinDropAmount { get; private set; }   // Max amount of each coin type

        public int ItemDropChance { get; set; }            // Chance to drop primary items
        public int ItemDropCount { get; set; }             // Number of primary items to drop
        public int SecondaryItemDropChance { get; set; }   // Chance to drop secondary items
        public int SecondaryItemDropCount { get; set; }    // Number of secondary items to drop
        public List<string> ItemNameList { get; private set; }
        public List<string> SecondaryItemNameList { get; private set; }

        // Chat/dialogue options
        public List<string> IdleChat { get; private set; }    // Random sayings
        public List<string> CombatChat { get; private set; }  // Combat dialogue
        public List<string> DeathChat { get; private set; }   // Final words

        // Other properties
        public string DesignerNotes { get; set; }         // Notes for designers
        public int RuntimeId { get; set; }                // Server-assigned ID

        /// <summary>
        /// Creates a new monster type
        /// </summary>
        public MonsterType()
        {
            Abilities = new float[(int)Ability.Count];
            CoinDropChance = new int[Money.Constants.MoneyTypeCount];
            CoinDropAmount = new int[Money.Constants.MoneyTypeCount];
            SpellEffects = new List<string>();
            ItemNameList = new List<string>();
            SecondaryItemNameList = new List<string>();
            IdleChat = new List<string>();
            CombatChat = new List<string>();
            DeathChat = new List<string>();

            Clear();
        }

        /// <summary>
        /// Resets all properties to default values
        /// </summary>
        public void Clear()
        {
            DataName = string.Empty;
            DisplayName = string.Empty;

            IdleSprite = 0;
            MoveSprite = 0;
            AttackSprite = 0;

            Level = 0;
            GroupType = 0;
            Gender = 2;      // Random gender
            Deity = 2;       // No deity

            Color = 5;       // Random color
            Red = 0xFF;
            Green = 0xFF;
            Blue = 0xFF;

            GroupTeam = 0;

            Experience = 1;

            for (int i = 0; i < (int)Ability.Count; i++)
            {
                Abilities[i] = 1;
            }

            MaxHealth = 10;
            MoraleHealth = 0;
            MoveSpeed = 1000;

            Flags = MonsterFlags.Social;
            SightRange = 10;
            Behavior = MonsterBehavior.Aggressive;

            MagicTime = 0;
            Aggression = 30;
            LightRadius = 0;

            NpcChat = string.Empty;

            AttackType = 0;
            AttackRange = 1;
            AttackSpeed = 5000;
            CriticalChance = 0.1f;
            FumbleChance = 0.1f;
            AttackName = 0;
            AttackChance = 100;

            AttackEffect = string.Empty;
            SpellEffects.Clear();
            EffectChance = 0;

            for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
            {
                CoinDropChance[i] = 0;
                CoinDropAmount[i] = 0;
            }

            ItemDropChance = 0;
            ItemDropCount = 1;
            SecondaryItemDropChance = 0;
            SecondaryItemDropCount = 0;
            ItemNameList.Clear();
            SecondaryItemNameList.Clear();

            IdleChat.Clear();
            CombatChat.Clear();
            DeathChat.Clear();

            DesignerNotes = string.Empty;
        }

        /// <summary>
        /// Reads monster type data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                Clear();

                // Read file version
                int version = reader.ReadInt32();

                // Read names
                if (version > MonsterFileVersions.V_021121)
                {
                    DataName = ReadFixedString(reader, MonsterConstants.NameLength);
                    DisplayName = ReadFixedString(reader, MonsterConstants.NameLength);
                }
                else
                {
                    DataName = ReadFixedString(reader, MonsterConstants.NameLength);
                    DisplayName = DataName;
                }

                // Read sprite info
                if (version > MonsterFileVersions.V_020530b)
                {
                    IdleSprite = reader.ReadUInt16();
                    if (version > MonsterFileVersions.V_030516)
                    {
                        MoveSprite = reader.ReadUInt16();
                        AttackSprite = reader.ReadUInt16();
                    }
                    else
                    {
                        MoveSprite = IdleSprite;
                        AttackSprite = IdleSprite;
                    }
                }
                else
                {
                    IdleSprite = 0;
                }

                if (version <= MonsterFileVersions.V_021018)
                {
                    // Changed to be sprite number instead of monster type number
                    IdleSprite *= 2;
                }

                // Read basic properties
                Level = reader.ReadByte();
                GroupType = reader.ReadByte();
                Gender = reader.ReadByte();
                Deity = reader.ReadByte();

                Color = reader.ReadByte();
                Red = reader.ReadByte();
                Green = reader.ReadByte();
                Blue = reader.ReadByte();

                if (version > MonsterFileVersions.V_021110)
                {
                    GroupTeam = reader.ReadInt32();
                }

                Experience = reader.ReadUInt32();

                // Read abilities
                if (version <= MonsterFileVersions.V_020530)
                {
                    // This version didn't have damage reduction ability
                    for (int i = 0; i < (int)Ability.Count - 1; i++)
                    {
                        Abilities[i] = reader.ReadSingle();
                    }
                    Abilities[(int)Ability.DamageReduction] = 0;
                }
                else
                {
                    for (int i = 0; i < (int)Ability.Count; i++)
                    {
                        Abilities[i] = reader.ReadSingle();
                    }
                }

                // Read health and movement
                MaxHealth = reader.ReadSingle();
                if (version > MonsterFileVersions.V_030513)
                {
                    MoraleHealth = reader.ReadSingle();
                }
                else
                {
                    // Set default morale based on monster type
                    switch (GroupType)
                    {
                        case (byte)MonsterGroup.Undead:
                        case (byte)MonsterGroup.Plant:
                        case (byte)MonsterGroup.Construct:
                            MoraleHealth = 0.0f;
                            break;
                        case (byte)MonsterGroup.Outsider:
                        case (byte)MonsterGroup.Elemental:
                            MoraleHealth = 0.10f;
                            break;
                        default:
                            MoraleHealth = 0.25f;
                            break;
                    }
                }
                MoveSpeed = reader.ReadUInt32();

                // Read AI flags
                Flags = (MonsterFlags)reader.ReadUInt32();
                SightRange = reader.ReadByte();
                Behavior = (MonsterBehavior)reader.ReadByte();

                if (version > MonsterFileVersions.V_020730)
                {
                    MagicTime = reader.ReadInt32();
                }
                if (version > MonsterFileVersions.V_020803)
                {
                    Aggression = reader.ReadInt32();
                }
                if (version > MonsterFileVersions.V_021020)
                {
                    LightRadius = reader.ReadInt32();
                }
                if (version > MonsterFileVersions.V_020921)
                {
                    NpcChat = ReadFixedString(reader, 32);
                }

                // Read attack properties
                AttackType = reader.ReadByte();
                AttackRange = reader.ReadByte();
                AttackSpeed = reader.ReadUInt32();
                CriticalChance = reader.ReadSingle();
                FumbleChance = reader.ReadSingle();

                if (version > MonsterFileVersions.V_020716)
                {
                    AttackName = reader.ReadByte();
                    AttackChance = reader.ReadByte();
                }
                if (version > MonsterFileVersions.V_021205)
                {
                    EffectChance = reader.ReadInt32();
                }

                // Read money drops
                if (version > MonsterFileVersions.V_020611)
                {
                    for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
                    {
                        CoinDropChance[i] = reader.ReadInt32();
                    }
                    for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
                    {
                        CoinDropAmount[i] = reader.ReadInt32();
                    }

                    if (version <= MonsterFileVersions.V_020909)
                    {
                        CoinDropAmount[0] = (CoinDropAmount[0] * 100) + (CoinDropAmount[1] * 10) + CoinDropAmount[2];
                        CoinDropChance[0] = (CoinDropChance[2] > CoinDropChance[0]) ? 
                            ((CoinDropChance[2] > CoinDropChance[1]) ? CoinDropChance[2] : CoinDropChance[1]) :
                            ((CoinDropChance[0] > CoinDropChance[1]) ? CoinDropChance[0] : CoinDropChance[1]);
                        CoinDropAmount[1] = 0;
                        CoinDropAmount[2] = 0;
                        CoinDropChance[1] = 0;
                        CoinDropChance[2] = 0;
                    }
                }

                // Read item drops
                if (version > MonsterFileVersions.V_020626)
                {
                    ItemDropChance = reader.ReadInt32();

                    int count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        string name = ReadFixedString(reader, 32);
                        ItemNameList.Add(name);
                    }
                }
                if (version > MonsterFileVersions.V_030506)
                {
                    ItemDropCount = reader.ReadInt32();
                    SecondaryItemDropChance = reader.ReadInt32();
                    SecondaryItemDropCount = reader.ReadInt32();

                    int count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        string name = ReadFixedString(reader, 32);
                        SecondaryItemNameList.Add(name);
                    }
                }

                // Read attack effects
                if (version > MonsterFileVersions.V_020726)
                {
                    AttackEffect = ReadFixedString(reader, 32);
                    
                    int count = reader.ReadInt32();
                    for (int i = 0; i < count; i++)
                    {
                        string name = ReadFixedString(reader, 32);
                        SpellEffects.Add(name);
                    }
                }

                // Read designer notes
                if (version > MonsterFileVersions.V_021124)
                {
                    DesignerNotes = ReadString(reader);
                }
                
                // Read chat lists
                if (version > MonsterFileVersions.V_021212)
                {
                    int idleChatCount = reader.ReadInt32();
                    for (int i = 0; i < idleChatCount; i++)
                    {
                        IdleChat.Add(ReadString(reader));
                    }

                    int combatChatCount = reader.ReadInt32();
                    for (int i = 0; i < combatChatCount; i++)
                    {
                        CombatChat.Add(ReadString(reader));
                    }

                    int deathChatCount = reader.ReadInt32();
                    for (int i = 0; i < deathChatCount; i++)
                    {
                        DeathChat.Add(ReadString(reader));
                    }
                }

                // Apply balance changes for older versions
                if (version <= MonsterFileVersions.V_030120)
                {
                    Abilities[(int)Ability.AttackRating] *= 1.1f;
                    Abilities[(int)Ability.ArmorClass] *= 1.1f;
                    Abilities[(int)Ability.DamageMinimum] *= 1.1f;
                    Abilities[(int)Ability.DamageMaximum] *= 1.1f;
                }
                if (version <= MonsterFileVersions.V_030401)
                {
                    Abilities[(int)Ability.ResistWhite] = 2.0f + Level;
                    Abilities[(int)Ability.ResistRed] = 2.0f + Level;
                    Abilities[(int)Ability.ResistGreen] = 2.0f + Level;
                    Abilities[(int)Ability.ResistBlue] = 2.0f + Level;
                    Abilities[(int)Ability.ResistYellow] = 2.0f + Level;
                }
                if (version <= MonsterFileVersions.V_030405)
                {
                    Abilities[(int)Ability.ResistWhite] += 3.0f;
                    Abilities[(int)Ability.ResistRed] += 3.0f;
                    Abilities[(int)Ability.ResistGreen] += 3.0f;
                    Abilities[(int)Ability.ResistBlue] += 3.0f;
                    Abilities[(int)Ability.ResistYellow] += 3.0f;
                }
                if (version <= MonsterFileVersions.V_030504)
                {
                    // Decrease HP for monsters below level 50
                    if (Level < 50)
                    {
                        float decreaseAmount = 0.25f;
                        decreaseAmount -= (Level / 10) * 0.05f;
                        decreaseAmount = 1.0f - decreaseAmount;
                        MaxHealth *= decreaseAmount;
                    }
                }
                if (version <= MonsterFileVersions.V_030510)
                {
                    float percentageBoost = (100.0f + Level) * 0.01f;
                    for (int i = 0; i < (int)Ability.Count; i++)
                    {
                        Abilities[i] *= percentageBoost;
                    }
                    MaxHealth *= percentageBoost;

                    // Add extra boost to damage because of CON boost (for characters)
                    Abilities[(int)Ability.DamageMaximum] *= 1.25f;
                    Abilities[(int)Ability.DamageMinimum] *= 1.25f;
                }
                if (version <= MonsterFileVersions.V_030519)
                {
                    AttackSpeed = (uint)(AttackSpeed * 0.70f);
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes monster type data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write file version
                writer.Write(MonsterFileVersions.Current);

                // Write names
                WriteFixedString(writer, DataName, MonsterConstants.NameLength);
                WriteFixedString(writer, DisplayName, MonsterConstants.NameLength);

                // Write sprite info
                writer.Write(IdleSprite);
                writer.Write(MoveSprite);
                writer.Write(AttackSprite);

                // Write basic properties
                writer.Write(Level);
                writer.Write(GroupType);
                writer.Write(Gender);
                writer.Write(Deity);

                writer.Write(Color);
                writer.Write(Red);
                writer.Write(Green);
                writer.Write(Blue);

                writer.Write(GroupTeam);

                writer.Write(Experience);

                // Write abilities
                for (int i = 0; i < (int)Ability.Count; i++)
                {
                    writer.Write(Abilities[i]);
                }

                // Write health and movement
                writer.Write(MaxHealth);
                writer.Write(MoraleHealth);
                writer.Write(MoveSpeed);

                // Write AI flags
                writer.Write((uint)Flags);
                writer.Write(SightRange);
                writer.Write((byte)Behavior);

                writer.Write(MagicTime);
                writer.Write(Aggression);
                writer.Write(LightRadius);

                WriteFixedString(writer, NpcChat, 32);

                // Write attack properties
                writer.Write(AttackType);
                writer.Write(AttackRange);
                writer.Write(AttackSpeed);
                writer.Write(CriticalChance);
                writer.Write(FumbleChance);

                writer.Write(AttackName);
                writer.Write(AttackChance);
                writer.Write(EffectChance);

                // Write money drops
                for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
                {
                    writer.Write(CoinDropChance[i]);
                }
                for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
                {
                    writer.Write(CoinDropAmount[i]);
                }

                // Write item drops
                writer.Write(ItemDropChance);
                writer.Write(ItemNameList.Count);
                foreach (string name in ItemNameList)
                {
                    WriteFixedString(writer, name, 32);
                }

                writer.Write(ItemDropCount);
                writer.Write(SecondaryItemDropChance);
                writer.Write(SecondaryItemDropCount);
                writer.Write(SecondaryItemNameList.Count);
                foreach (string name in SecondaryItemNameList)
                {
                    WriteFixedString(writer, name, 32);
                }

                // Write attack effects
                WriteFixedString(writer, AttackEffect, 32);
                writer.Write(SpellEffects.Count);
                foreach (string effect in SpellEffects)
                {
                    WriteFixedString(writer, effect, 32);
                }

                // Write designer notes
                WriteString(writer, DesignerNotes);

                // Write chat lists
                writer.Write(IdleChat.Count);
                foreach (string chat in IdleChat)
                {
                    WriteString(writer, chat);
                }

                writer.Write(CombatChat.Count);
                foreach (string chat in CombatChat)
                {
                    WriteString(writer, chat);
                }

                writer.Write(DeathChat.Count);
                foreach (string chat in DeathChat)
                {
                    WriteString(writer, chat);
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Copies properties from another monster type
        /// </summary>
        public void CopyFrom(MonsterType other)
        {
            if (other == null)
                return;

            Clear();

            DataName = other.DataName;
            DisplayName = other.DisplayName;

            IdleSprite = other.IdleSprite;
            MoveSprite = other.MoveSprite;
            AttackSprite = other.AttackSprite;

            Level = other.Level;
            GroupType = other.GroupType;
            Gender = other.Gender;
            Deity = other.Deity;

            Color = other.Color;
            Red = other.Red;
            Green = other.Green;
            Blue = other.Blue;

            GroupTeam = other.GroupTeam;

            Experience = other.Experience;

            for (int i = 0; i < (int)Ability.Count; i++)
            {
                Abilities[i] = other.Abilities[i];
            }

            MaxHealth = other.MaxHealth;
            MoraleHealth = other.MoraleHealth;
            MoveSpeed = other.MoveSpeed;

            Flags = other.Flags;
            SightRange = other.SightRange;
            Behavior = other.Behavior;

            MagicTime = other.MagicTime;
            Aggression = other.Aggression;
            LightRadius = other.LightRadius;

            NpcChat = other.NpcChat;

            AttackType = other.AttackType;
            AttackRange = other.AttackRange;
            AttackSpeed = other.AttackSpeed;
            CriticalChance = other.CriticalChance;
            FumbleChance = other.FumbleChance;
            AttackName = other.AttackName;
            AttackChance = other.AttackChance;

            AttackEffect = other.AttackEffect;
            EffectChance = other.EffectChance;
            
            foreach (string effect in other.SpellEffects)
            {
                SpellEffects.Add(effect);
            }

            for (int i = 0; i < Money.Constants.MoneyTypeCount; i++)
            {
                CoinDropChance[i] = other.CoinDropChance[i];
                CoinDropAmount[i] = other.CoinDropAmount[i];
            }

            ItemDropChance = other.ItemDropChance;
            ItemDropCount = other.ItemDropCount;
            SecondaryItemDropChance = other.SecondaryItemDropChance;
            SecondaryItemDropCount = other.SecondaryItemDropCount;
            
            foreach (string item in other.ItemNameList)
            {
                ItemNameList.Add(item);
            }
            
            foreach (string item in other.SecondaryItemNameList)
            {
                SecondaryItemNameList.Add(item);
            }

            foreach (string chat in other.IdleChat)
            {
                IdleChat.Add(chat);
            }
            
            foreach (string chat in other.CombatChat)
            {
                CombatChat.Add(chat);
            }
            
            foreach (string chat in other.DeathChat)
            {
                DeathChat.Add(chat);
            }

            DesignerNotes = other.DesignerNotes;
        }

        /// <summary>
        /// Creates network data for this monster type
        /// </summary>
        public byte[] CreateNetworkData(out int size)
        {
            size = 0;
            using (MemoryStream ms = new MemoryStream())
            {
                using (BinaryWriter writer = new BinaryWriter(ms))
                {
                    // Skip the packet header space
                    const int headerSize = 12; // Sizeof(GenericPacket) + sizeof(int)
                    writer.BaseStream.Position = headerSize;

                    // Write version
                    writer.Write(MonsterFileVersions.Current);

                    // Write the monster data as normal
                    Write(writer);

                    // Write a checksum value
                    writer.Write(0x23344556); // MONSTERPACKET_CHECK constant

                    // Get the size and data
                    size = (int)writer.BaseStream.Length;
                    return ms.ToArray();
                }
            }
        }

        /// <summary>
        /// Loads monster type from network data
        /// </summary>
        public bool LoadFromNetworkData(byte[] data, int size)
        {
            if (data == null || size < 16)
                return false;

            try
            {
                Clear();

                using (MemoryStream ms = new MemoryStream(data, 0, size))
                {
                    using (BinaryReader reader = new BinaryReader(ms))
                    {
                        // Skip the packet header
                        const int headerSize = 12; // Sizeof(GenericPacket) + sizeof(int)
                        reader.BaseStream.Position = headerSize;

                        // Get monster information version
                        int monsterVersion = reader.ReadInt32();

                        // Read the monster data
                        if (!Read(reader))
                            return false;

                        // Verify the checksum
                        int checksum = reader.ReadInt32();
                        if (checksum != 0x23344556) // MONSTERPACKET_CHECK
                            return false;

                        return true;
                    }
                }
            }
            catch (Exception)
            {
                return false;
            }
        }

        #region Helper Methods

        private static string ReadFixedString(BinaryReader reader, int length)
        {
            char[] chars = reader.ReadChars(length);
            string result = new string(chars);
            int nullIndex = result.IndexOf('\0');
            return nullIndex >= 0 ? result.Substring(0, nullIndex) : result;
        }

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

        private static string ReadString(BinaryReader reader)
        {
            int length = reader.ReadInt32();
            if (length <= 0)
                return string.Empty;

            return new string(reader.ReadChars(length));
        }

        private static void WriteString(BinaryWriter writer, string text)
        {
            if (string.IsNullOrEmpty(text))
            {
                writer.Write(0);
                return;
            }

            writer.Write(text.Length);
            writer.Write(text.ToCharArray());
        }

        #endregion
    }

    /// <summary>
    /// Represents a monster type instance on the client
    /// </summary>
    public class MonsterTypeInstance
    {
        public int RuntimeId { get; set; }
        public bool DataIsOk { get; set; }
        public string Name { get; set; }
        public uint Appearance { get; set; }    // Holds sprite and colors
        public ushort MoveSprite { get; set; }
        public ushort AttackSprite { get; set; }
        public int LastRequestTime { get; set; }

        /// <summary>
        /// Creates a new monster type instance with minimal data
        /// </summary>
        public MonsterTypeInstance(int runtimeId)
        {
            RuntimeId = runtimeId;
            DataIsOk = false;
            Name = "Lagmonster";  // Default name until proper data is received
            Appearance = 0xFFFF0000;  // This will look like a white rabbit
            LastRequestTime = 0;
        }

        /// <summary>
        /// Creates a new monster type instance with complete data
        /// </summary>
        public MonsterTypeInstance(int runtimeId, uint appearance, ushort moveSprite, ushort attackSprite, string name)
        {
            RuntimeId = runtimeId;
            DataIsOk = true;
            Name = name ?? "Unknown";
            Appearance = appearance;
            MoveSprite = moveSprite;
            AttackSprite = attackSprite;
            LastRequestTime = 0;
        }
    }
}