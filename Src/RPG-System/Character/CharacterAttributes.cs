using System;
using System.Collections.Generic;
using System.IO;
using RpgSystem.Attributes;
using RpgSystem.Effects;
using RpgSystem.Skills;

namespace RpgSystem.Character
{
    /// <summary>
    /// Race types for characters
    /// </summary>
    public enum CharacterRace
    {
        Human = 0,
        Menit,
        Iwid,
        
        Count
    }

    /// <summary>
    /// Victim information for player-killing system
    /// </summary>
    public class VictimName
    {
        public string Name { get; set; }
        public int Seconds { get; set; }

        public VictimName(string name, int seconds)
        {
            Name = name;
            Seconds = seconds;
        }
    }

    /// <summary>
    /// Constants for player-killing (PK) system
    /// </summary>
    public static class PkConstants
    {
        public const int ExpireTime = 10 * 60 * 60;      // Default expire time (10 hours)
        public const int ProtectionTime = 5 * 60;        // Protection time for respawned players (5 mins)
    }

    /// <summary>
    /// Basic character statistics
    /// </summary>
    public class BaseStats
    {
        // Base attributes
        public Attribute[] Attributes { get; private set; }
        
        // Variable attributes (health, mana, stamina)
        public VariableAttribute[] Variables { get; private set; }
        
        // Abilities (attack, defense, etc.)
        public Attribute[] Abilities { get; private set; }
        
        // Basic character info
        public float Hunger { get; set; }                  // Current food value (100 is max)
        
        public ushort PlayerClass { get; set; }            // Player class
        public ushort Level { get; set; }                  // Character level
        public uint Experience { get; set; }               // Experience points
        public byte Gender { get; set; }                   // 0 for male, 1 for female
        public byte SoulColor { get; set; }                // Soul color (white, red, green, blue, yellow)
        public byte God { get; set; }                      // 0 for Gifted, 1 for Shining
        public CharacterRace Race { get; set; }            // Character race
        
        public ushort NewPoints { get; set; }              // New attribute points to spend
        public ushort TotalPoints { get; set; }            // Total attribute points received
        
        // Soul gem system
        public byte[] FreeGems { get; private set; }       // Unassigned gems
        public byte TotalGems { get; set; }                // Total gems gained
        
        // Stats that aren't stored to file
        public int AttackSpeed { get; set; }               // Added to weapon speed
        public float MoveSpeedMultiplier { get; set; }     // Movement speed modifier
        public sbyte PublicLight { get; set; }             // Light visible to others
        public sbyte PersonalLight { get; set; }           // Light visible only to this character

        public BaseStats()
        {
            Attributes = new Attribute[(int)BaseAttribute.Count];
            Variables = new VariableAttribute[(int)VariableAttributeType.Count];
            Abilities = new Attribute[(int)Ability.Count];
            FreeGems = new byte[5];
            
            Clear();
        }

        /// <summary>
        /// Resets all attributes to default values
        /// </summary>
        public void Clear()
        {
            // Initialize all objects
            for (int i = 0; i < (int)BaseAttribute.Count; i++)
            {
                if (Attributes[i] == null)
                    Attributes[i] = new Attribute();
                
                Attributes[i].SetBase(10.0f);
            }
            
            for (int i = 0; i < (int)VariableAttributeType.Count; i++)
            {
                if (Variables[i] == null)
                    Variables[i] = new VariableAttribute();
                
                Variables[i].SetBase(20.0f);
                Variables[i].Restore();
            }
            
            for (int i = 0; i < (int)Ability.Count; i++)
            {
                if (Abilities[i] == null)
                    Abilities[i] = new Attribute();
                
                Abilities[i].SetBase(10.0f);
            }

            // Reset stats
            PlayerClass = 0;
            Level = 0;
            Experience = 0;
            Gender = 0;
            SoulColor = 0;
            God = 0;
            Hunger = 100.0f;
            Race = CharacterRace.Human;
            
            NewPoints = 0;
            TotalPoints = 0;
            
            FreeGems[0] = 0;
            FreeGems[1] = 0;
            FreeGems[2] = 0;
            FreeGems[3] = 0;
            FreeGems[4] = 0;
            TotalGems = 0;
            
            AttackSpeed = 0;
            MoveSpeedMultiplier = 1.0f;
            PublicLight = 0;
            PersonalLight = 0;
        }

        /// <summary>
        /// Reads base stats from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader, int version)
        {
            // Read attributes
            for (int i = 0; i < (int)BaseAttribute.Count; i++)
            {
                if (Attributes[i] == null)
                    Attributes[i] = new Attribute();
                
                Attributes[i].Read(reader, version);
            }

            // Handle old file versions with missing stamina attribute
            if (version <= PlayerFileVersions.V_020505)
            {
                // Read only health and mana
                for (int i = 0; i <= (int)VariableAttributeType.Mana; i++)
                {
                    if (Variables[i] == null)
                        Variables[i] = new VariableAttribute();
                    
                    Variables[i].Read(reader, version);
                }
                
                // Set stamina based on constitution
                if (Variables[(int)VariableAttributeType.Stamina] == null)
                    Variables[(int)VariableAttributeType.Stamina] = new VariableAttribute();
                
                Variables[(int)VariableAttributeType.Stamina].SetBase(Attributes[(int)BaseAttribute.Constitution].GetValue() * 3.0f);
            }
            else
            {
                // Read all variable attributes
                for (int i = 0; i < (int)VariableAttributeType.Count; i++)
                {
                    if (Variables[i] == null)
                        Variables[i] = new VariableAttribute();
                    
                    Variables[i].Read(reader, version);
                }
            }

            // Handle old file versions missing damage reduction
            if (version <= PlayerFileVersions.V_020530)
            {
                // Read abilities except for damage reduction
                for (int i = 0; i <= (int)Ability.ResistYellow; i++)
                {
                    if (Abilities[i] == null)
                        Abilities[i] = new Attribute();
                    
                    Abilities[i].Read(reader, version);
                }
                
                // Set damage reduction to 0
                if (Abilities[(int)Ability.DamageReduction] == null)
                    Abilities[(int)Ability.DamageReduction] = new Attribute();
                
                Abilities[(int)Ability.DamageReduction].SetBase(0);
            }
            else
            {
                // Read all abilities
                for (int i = 0; i < (int)Ability.Count; i++)
                {
                    if (Abilities[i] == null)
                        Abilities[i] = new Attribute();
                    
                    Abilities[i].Read(reader, version);
                }
            }

            // Basic character info
            PlayerClass = reader.ReadUInt16();
            Level = reader.ReadUInt16();
            Experience = reader.ReadUInt32();
            Gender = reader.ReadByte();
            SoulColor = reader.ReadByte();
            God = reader.ReadByte();
            Race = (CharacterRace)reader.ReadByte();

            // Attribute points
            NewPoints = reader.ReadUInt16();
            TotalPoints = reader.ReadUInt16();

            // Proficiency gems
            for (int i = 0; i < 5; i++)
            {
                FreeGems[i] = reader.ReadByte();
            }
            TotalGems = reader.ReadByte();

            // Handle hunger in newer file versions
            if (version > PlayerFileVersions.V_020505)
            {
                Hunger = reader.ReadSingle();
            }
            else
            {
                Hunger = 100.0f;
            }

            // Fix old file version bug with race
            if (version <= PlayerFileVersions.V_030506)
            {
                Race = CharacterRace.Human;
            }

            return true;
        }

        /// <summary>
        /// Writes base stats to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer, int version)
        {
            // Write attributes
            for (int i = 0; i < (int)BaseAttribute.Count; i++)
            {
                Attributes[i].Write(writer, version);
            }
            
            // Write variable attributes (health, mana, stamina)
            for (int i = 0; i < (int)VariableAttributeType.Count; i++)
            {
                Variables[i].Write(writer, version);
            }
            
            // Write abilities
            for (int i = 0; i < (int)Ability.Count; i++)
            {
                Abilities[i].Write(writer, version);
            }

            // Basic character info
            writer.Write(PlayerClass);
            writer.Write(Level);
            writer.Write(Experience);
            writer.Write(Gender);
            writer.Write(SoulColor);
            writer.Write(God);
            writer.Write((byte)Race);

            // Attribute points
            writer.Write(NewPoints);
            writer.Write(TotalPoints);

            // Proficiency gems
            for (int i = 0; i < 5; i++)
            {
                writer.Write(FreeGems[i]);
            }
            writer.Write(TotalGems);

            // Other
            writer.Write(Hunger);

            return true;
        }
    }

    /// <summary>
    /// Stores all character attributes, skills, and effects
    /// </summary>
    public class CharacterAttributes
    {
        public BaseStats Stats { get; private set; }
        public List<Skill> Skills { get; private set; }
        public List<CharacterEffect> RunEffects { get; private set; }
        public List<VictimName> Victims { get; private set; }

        public CharacterAttributes()
        {
            Stats = new BaseStats();
            Skills = new List<Skill>();
            RunEffects = new List<CharacterEffect>();
            Victims = new List<VictimName>();
        }

        /// <summary>
        /// Clears all character data
        /// </summary>
        private void Clear()
        {
            Skills.Clear();
            RunEffects.Clear();
            Victims.Clear();
        }

        /// <summary>
        /// Removes all character data and frees resources
        /// </summary>
        public void RemoveAll()
        {
            Skills.Clear();
            RunEffects.Clear();
            Victims.Clear();
        }

        /// <summary>
        /// Reads character data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader, int version)
        {
            // Clear old data
            RemoveAll();

            // Read base stats
            if (!Stats.Read(reader, version))
            {
                return false;
            }

            // Handle legacy skill system in oldest versions
            if (version <= PlayerFileVersions.V_020818)
            {
                // Skip old skill data
                int dummy = reader.ReadInt32();
            }
            else
            {
                // Read skills
                int skillCount = reader.ReadInt32();
                for (int i = 0; i < skillCount; i++)
                {
                    Skill skill = new Skill();
                    skill.Read(reader, version);
                    Skills.Add(skill);
                }
            }

            // Read run effects for newer versions
            if (version > PlayerFileVersions.V_021022)
            {
                int effectCount = reader.ReadInt32();
                for (int i = 0; i < effectCount; i++)
                {
                    CharacterEffect effect = new CharacterEffect();
                    effect.Read(reader, version);
                    RunEffects.Add(effect);
                }
            }

            // Read victims for newer versions
            if (version > PlayerFileVersions.V_030301)
            {
                int victimCount = reader.ReadInt32();
                for (int i = 0; i < victimCount; i++)
                {
                    string name = new string(reader.ReadChars(16)).TrimEnd('\0');
                    int seconds = reader.ReadInt32();
                    Victims.Add(new VictimName(name, seconds));
                }
            }

            return true;
        }

        /// <summary>
        /// Writes character data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer, int version)
        {
            // Write base stats
            if (!Stats.Write(writer, version))
            {
                return false;
            }

            // Write skills
            writer.Write(Skills.Count);
            foreach (var skill in Skills)
            {
                skill.Write(writer);
            }

            // Write run effects
            writer.Write(RunEffects.Count);
            foreach (var effect in RunEffects)
            {
                effect.Write(writer);
            }

            // Write victims
            writer.Write(Victims.Count);
            foreach (var victim in Victims)
            {
                // Write fixed-length 16 character name
                char[] nameChars = new char[16];
                for (int i = 0; i < 16; i++)
                {
                    if (i < victim.Name.Length)
                        nameChars[i] = victim.Name[i];
                    else
                        nameChars[i] = '\0';
                }
                writer.Write(nameChars);
                writer.Write(victim.Seconds);
            }

            return true;
        }

        /// <summary>
        /// Checks if the character has a specific skill
        /// </summary>
        public bool HasSkill(int skillNumber, out int saveIndex, bool mustBeLocked = false)
        {
            saveIndex = -1;
            
            for (int i = 0; i < Skills.Count; i++)
            {
                if (Skills[i].GetSkillId() == skillNumber)
                {
                    saveIndex = i;
                    
                    if (mustBeLocked && !Skills[i].IsSpecialized())
                    {
                        return false;
                    }
                    
                    return true;
                }
            }
            
            return false;
        }

        /// <summary>
        /// Overload for HasSkill that doesn't output the index
        /// </summary>
        public bool HasSkill(int skillNumber, bool mustBeLocked = false)
        {
            int unused;
            return HasSkill(skillNumber, out unused, mustBeLocked);
        }

        /// <summary>
        /// Adds a new skill to the character
        /// </summary>
        public bool AddSkill(SkillType2 skillType, int startRow)
        {
            if (skillType == null)
            {
                return false;
            }

            // Check if the character already has this skill
            if (HasSkill(skillType.GetId()))
            {
                return false;
            }

            // Create the skill
            Skill skill = new Skill();
            skill.SetNewSkill(skillType, startRow);

            // Add skill to list
            Skills.Add(skill);

            return true;
        }

        /// <summary>
        /// Adds a victim to the player-killing system
        /// </summary>
        public void AddVictim(string characterName)
        {
            // Search for existing victim entry to avoid duplicates
            foreach (var victim in Victims)
            {
                if (string.Compare(victim.Name, characterName, true) == 0)
                {
                    // Already exists, reset timer
                    victim.Seconds = PkConstants.ExpireTime;
                    return;
                }
            }

            // Add new victim
            Victims.Add(new VictimName(characterName, PkConstants.ExpireTime));
        }

        /// <summary>
        /// Clears a victim from the player-killing list
        /// </summary>
        public bool ClearVictim(string characterName)
        {
            for (int i = 0; i < Victims.Count; i++)
            {
                if (string.Compare(Victims[i].Name, characterName, true) == 0)
                {
                    Victims.RemoveAt(i);
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Checks if a character is in the victim list
        /// </summary>
        public bool HasVictim(string characterName)
        {
            foreach (var victim in Victims)
            {
                if (string.Compare(victim.Name, characterName, true) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Checks if this character is a player-killer
        /// </summary>
        public bool IsPKer()
        {
            return Victims.Count > 0;
        }
    }

    /// <summary>
    /// Smaller version of stats for network transmission
    /// </summary>
    public class SmallStats
    {
        public float[] Attributes { get; private set; }
        public float[] VarCurrent { get; private set; }
        public float[] VarMax { get; private set; }
        public float[] Abilities { get; private set; }

        // Basic character info
        public float Hunger { get; set; }
        public ushort PlayerClass { get; set; }
        public ushort Level { get; set; }
        public byte Gender { get; set; }
        public byte SoulColor { get; set; }
        public byte God { get; set; }
        public byte Race { get; set; }
        public ushort NewPoints { get; set; }
        public ushort TotalPoints { get; set; }
        public byte[] FreeGems { get; private set; }
        public byte TotalGems { get; set; }
        public byte ExpProgressType { get; set; }
        public byte ExpProgress { get; set; }
        public ushort ItemDisabled { get; set; }

        public SmallStats()
        {
            Attributes = new float[(int)BaseAttribute.Count];
            VarCurrent = new float[(int)VariableAttributeType.Count];
            VarMax = new float[(int)VariableAttributeType.Count];
            Abilities = new float[(int)Ability.Count];
            FreeGems = new byte[5];
        }

        /// <summary>
        /// Populates small stats from full character stats and inventory
        /// </summary>
        public void SetStats(BaseStats stats, CharacterInventory inventory)
        {
            // Copy attributes
            for (int i = 0; i < (int)BaseAttribute.Count; i++)
            {
                Attributes[i] = stats.Attributes[i].GetValue();
            }

            // Copy variable attributes
            for (int i = 0; i < (int)VariableAttributeType.Count; i++)
            {
                VarCurrent[i] = stats.Variables[i].GetCurrent();
                VarMax[i] = stats.Variables[i].GetValue();
            }

            // Copy abilities
            for (int i = 0; i < (int)Ability.Count; i++)
            {
                Abilities[i] = stats.Abilities[i].GetValue();
            }

            // Copy basic character info
            Hunger = stats.Hunger;
            PlayerClass = stats.PlayerClass;
            Level = stats.Level;
            Gender = stats.Gender;
            SoulColor = stats.SoulColor;
            God = stats.God;
            Race = (byte)stats.Race;
            NewPoints = stats.NewPoints;
            TotalPoints = stats.TotalPoints;

            // Copy gems
            for (int i = 0; i < 5; i++)
            {
                FreeGems[i] = stats.FreeGems[i];
            }
            TotalGems = stats.TotalGems;

            // Calculate experience progress
            ExpProgressType = (byte)Experience.ExperienceMath.GetProgressType(Level);
            ExpProgress = (byte)Experience.ExperienceMath.GetProgressPosition(Level, (int)stats.Experience);

            // Set disabled items flags
            ItemDisabled = 0;
            if (inventory != null)
            {
                for (int i = 0; i < (int)EquipmentSlot.Count; i++)
                {
                    if (inventory.Equipment[i] != null && !inventory.Equipment[i].IsEnabled())
                    {
                        ItemDisabled |= (ushort)(1 << i);
                    }
                }
            }
        }
    }
}