using System;
using System.IO;

namespace RpgSystem.Effects
{
    /// <summary>
    /// File versions for effect system
    /// </summary>
    public static class EffectFileVersions
    {
        public const int Original = 0;
        public const int V_021013 = 1;   // Added difficulty
        public const int V_021015 = 2;   // Added mana cost
        public const int V_021108 = 3;   // Added target types and intelligence strength
        public const int V_021110 = 4;   // Added requirements for targets
        public const int V_021127 = 5;   // Removed u_crstrength and replaced it with parameter multipliers
        public const int V_021127b = 6;  // Added projectile and explosion information + design notes
        public const int V_021128 = 7;   // Added defense
        public const int V_030215 = 8;   // Added flags, data/display name, action/recovery time
        public const int V_030330 = 9;   // Added icon
        public const int V_030407 = 10;  // Added effect ID
        public const int V_030520 = 11;  // Speed rebalance
        
        public const int Current = V_030520;
    }

    /// <summary>
    /// Target types for effects
    /// </summary>
    public enum TargetType
    {
        Self = 0,       // The caster
        Enemies,        // Enemy units
        Allies,         // Friendly units
        
        Count
    }

    /// <summary>
    /// Target style configurations
    /// </summary>
    public enum TargetStyle
    {
        Normal = 0,     // Works normally on this target
        NoEffect,       // Cannot be cast on this target
        Rebounds,       // Rebounds on the caster
        
        Count
    }

    /// <summary>
    /// Effect alignment types
    /// </summary>
    public enum EffectAlignment
    {
        Offensive = 0,  // Damage-based spells
        Defensive,      // Boosting/healing spells
        Neutral,        // Light/scry/identify etc.
        Other,
        
        Count
    }

    /// <summary>
    /// Effect colors
    /// </summary>
    public enum EffectColor
    {
        White = 0,
        Red,
        Green,
        Blue,
        Yellow,
        
        Count
    }

    /// <summary>
    /// Effect saving throw types
    /// </summary>
    public enum EffectSavingThrow
    {
        None = 0,       // Always maximum effect
        Decrease,       // Spell always hits but has different efficiency
        Negate,         // Spell either has max effect or no effect
        
        Count
    }

    /// <summary>
    /// Effect requirement types
    /// </summary>
    public enum EffectRequirementType
    {
        Disabled = 0,           // No requirements
        Requirement,            // Normal requirement (like player stat requirement)
        Equipment,              // Target must have specific equipment
        Flying,                 // Target must be flying
        Ethereal,               // Target must be ethereal
        
        Count
    }

    /// <summary>
    /// Effect requirement structure
    /// </summary>
    public class EffectRequirement
    {
        public int MainType { get; set; }    // Type of requirement
        public int SubType { get; set; }     // Subtype for requirement
        public int SubValue { get; set; }    // Value for requirement
        
        public EffectRequirement()
        {
            MainType = (int)EffectRequirementType.Disabled;
            SubType = 0;
            SubValue = 0;
        }
    }

    /// <summary>
    /// Constants for effect system
    /// </summary>
    public static class EffectConstants
    {
        public const int NameLength = 32;
        public const int EffectFlag_StaminaDrain = 0x1;    // Drains stamina instead of mana
        public const int EffectBaseId = 0x08000000;        // Used to avoid conflicts with skill IDs
    }

    /// <summary>
    /// Defines an effect type for spells and abilities
    /// </summary>
    public class EffectType
    {
        // Basic identifiers
        public string DataName { get; set; }          // Internal name
        public string DisplayName { get; set; }       // Name shown to players
        
        // Effect mechanics
        public int Type { get; set; }                 // Effect mechanism type
        public int Alignment { get; set; }            // Offensive/defensive/neutral
        public int Color { get; set; }                // Visual color
        public int Save { get; set; }                 // Saving throw type
        public int EffectId { get; set; }             // Unique ID for stacking
        
        // Target settings
        public int[] TargetSettings { get; private set; }  // How effect affects different targets
        public EffectRequirement Requirement { get; private set; }  // Target requirement
        
        // Range and radius
        public int Range { get; set; }                // 0=self, 1=touch, >1=ranged
        public int Radius { get; set; }               // <2=single target, >=2=area
        public int Flags { get; set; }                // Various flags
        public int ActionTime { get; set; }           // Casting time delay
        public int RecoveryTime { get; set; }         // Recovery time after casting
        
        // Casting parameters
        public float BasePower { get; set; }          // Base cast rating if used from item
        public float ManaCost { get; set; }           // Magic points required
        public float[] Parameters { get; private set; }    // Parameters for the effect
        public string StringParam { get; set; }       // String parameter (for summons etc.)
        
        // Difficulty settings
        public float Difficulty { get; set; }         // Multiplier added to caster's rating
        public float Defense { get; set; }            // Protection value caster must beat
        
        // Cast rating improvements
        public float[] ParamMultipliers { get; private set; } // Added to params based on cast rating
        public int RangeIncrease { get; set; }        // Range increase per cast rating
        public int RadiusIncrease { get; set; }       // Radius increase per cast rating
        
        // Visuals
        public int Icon { get; set; }                 // Icon displayed for runable effects
        public int ProjectileType { get; set; }       // 0 if disabled
        public int ProjectileSprite { get; set; }
        public int ProjectileSound { get; set; }
        public int ExplosionType { get; set; }        // 0 if disabled
        public int ExplosionSprite { get; set; }
        public int ExplosionSound { get; set; }
        
        // Other
        public string DesignNotes { get; set; }       // Developer notes
        
        /// <summary>
        /// Creates a new effect type
        /// </summary>
        public EffectType()
        {
            DataName = string.Empty;
            DisplayName = string.Empty;
            
            TargetSettings = new int[(int)TargetType.Count];
            Requirement = new EffectRequirement();
            Parameters = new float[3];
            ParamMultipliers = new float[3];
            StringParam = string.Empty;
            DesignNotes = string.Empty;
            
            Clear();
        }
        
        /// <summary>
        /// Resets all effect properties to defaults
        /// </summary>
        public void Clear()
        {
            DataName = string.Empty;
            DisplayName = string.Empty;
            
            Type = 0;
            Alignment = 0;
            Color = 0;
            Save = 0;
            EffectId = -1;
            
            for (int i = 0; i < (int)TargetType.Count; i++)
            {
                TargetSettings[i] = (int)TargetStyle.Normal;
            }
            
            Requirement.MainType = (int)EffectRequirementType.Disabled;
            Requirement.SubType = 0;
            Requirement.SubValue = 0;
            
            Range = 0;
            Radius = 0;
            Flags = 0;
            ActionTime = 500;
            RecoveryTime = 500;
            
            BasePower = 0;
            Difficulty = 1.0f;
            ManaCost = 0;
            Parameters[0] = 0;
            Parameters[1] = 0;
            Parameters[2] = 0;
            StringParam = string.Empty;
            
            Defense = 0;
            
            ParamMultipliers[0] = 0;
            ParamMultipliers[1] = 0;
            ParamMultipliers[2] = 0;
            RangeIncrease = 0;
            RadiusIncrease = 0;
            
            Icon = 0;
            ProjectileType = 0;
            ProjectileSprite = 0;
            ProjectileSound = 0;
            ExplosionType = 0;
            ExplosionSprite = 0;
            ExplosionSound = 0;
            
            DesignNotes = string.Empty;
        }
        
        /// <summary>
        /// Gets the duration of this effect
        /// </summary>
        public int GetDuration(float castRating = 0, IAttributeObject caster = null)
        {
            if (caster != null)
            {
                castRating = caster.GetAbility(Attributes.Ability.CastRating);
            }
            
            switch (Type)
            {
                case (int)EffectMechanismType.Poison:
                case (int)EffectMechanismType.Light:
                case (int)EffectMechanismType.ImproveAttackSpeed:
                case (int)EffectMechanismType.ReduceAttackSpeed:
                case (int)EffectMechanismType.SpeedPercent:
                    return (int)(GetParameter(1, castRating, caster) + 0.5f);
                    
                case (int)EffectMechanismType.Nightmare:
                    return (int)(GetParameter(0, castRating, caster) + 0.5f);
                    
                case (int)EffectMechanismType.Regenerate:
                case (int)EffectMechanismType.Boost:
                case (int)EffectMechanismType.BoostPercent:
                case (int)EffectMechanismType.Weaken:
                case (int)EffectMechanismType.WeakenPercent:
                    return (int)(GetParameter(2, castRating, caster) + 0.5f);
                    
                default:
                    return 0;
            }
        }
        
        /// <summary>
        /// Checks if the effect can be applied to a target
        /// </summary>
        public bool IsApplicable(IAttributeObject target)
        {
            if (target == null)
                return false;
                
            // Check if there's a requirement
            if (Requirement.MainType == (int)EffectRequirementType.Disabled)
                return true;
                
            switch (Requirement.MainType)
            {
                case (int)EffectRequirementType.Requirement:
                    {
                        Attributes.Requirement req = new Attributes.Requirement
                        {
                            Type = Requirement.SubType,
                            Value = Requirement.SubValue
                        };
                        return target.TestRequirement(req);
                    }
                    
                case (int)EffectRequirementType.Equipment:
                    return target.TestEquipSlot(Requirement.SubType, Requirement.SubValue);
                    
                case (int)EffectRequirementType.Flying:
                    return target.IsFlying();
                    
                case (int)EffectRequirementType.Ethereal:
                    return target.IsEthereal();
                    
                default:
                    return true;
            }
        }
        
        /// <summary>
        /// Checks if the target is protected from this effect
        /// </summary>
        public bool IsProtected(IAttributeObject target, IAttributeObject source)
        {
            if (target == null || source == null)
                return false;
                
            if (target == source)
            {
                if (TargetSettings[(int)TargetType.Self] == (int)TargetStyle.NoEffect)
                    return true;
            }
            
            if (source.IsHostile(target.GetTeam()))
            {
                if (TargetSettings[(int)TargetType.Enemies] == (int)TargetStyle.NoEffect)
                    return true;
            }
            else if (TargetSettings[(int)TargetType.Allies] == (int)TargetStyle.NoEffect)
            {
                return true;
            }
            
            return false;
        }
        
        /// <summary>
        /// Checks if effect should rebound on the caster
        /// </summary>
        public bool IsRebounded(IAttributeObject target, IAttributeObject source)
        {
            if (target == null || source == null)
                return false;
                
            if (target == source)
                return false;  // Effects can't rebound on yourself
                
            if (source.IsHostile(target.GetTeam()))
            {
                if (TargetSettings[(int)TargetType.Enemies] == (int)TargetStyle.Rebounds)
                    return true;
            }
            else if (TargetSettings[(int)TargetType.Allies] == (int)TargetStyle.Rebounds)
            {
                return true;
            }
            
            return false;
        }
        
        /// <summary>
        /// Gets the range based on cast rating
        /// </summary>
        public int GetRange(float castRating = 0, IAttributeObject caster = null)
        {
            if (caster != null)
            {
                castRating = caster.GetAbility(Attributes.Ability.CastRating);
            }
            
            if (castRating > 0 && RangeIncrease > 0)
            {
                return Range + ((int)(castRating + 0.5f) / RangeIncrease);
            }
            
            return Range;
        }
        
        /// <summary>
        /// Gets the radius based on cast rating
        /// </summary>
        public int GetRadius(float castRating = 0, IAttributeObject caster = null)
        {
            if (caster != null)
            {
                castRating = caster.GetAbility(Attributes.Ability.CastRating);
            }
            
            if (castRating > 0 && RadiusIncrease > 0)
            {
                return Radius + ((int)(castRating + 0.5f) / RadiusIncrease);
            }
            
            return Radius;
        }
        
        /// <summary>
        /// Gets a parameter value adjusted by cast rating
        /// </summary>
        public float GetParameter(int number, float castRating = 0, IAttributeObject caster = null)
        {
            if (caster != null)
            {
                castRating = caster.GetAbility(Attributes.Ability.CastRating);
            }
            
            if (number < 0 || number >= Parameters.Length)
                return 0;
                
            if (castRating > 0)
            {
                return Parameters[number] + (castRating * ParamMultipliers[number]);
            }
            
            return Parameters[number];
        }
        
        /// <summary>
        /// Tests if the caster can successfully cast the spell
        /// </summary>
        public bool TestSpellSuccess(float castRating = 0, IAttributeObject caster = null)
        {
            if (Defense < 1)
                return true;
                
            if (caster != null)
            {
                castRating = caster.GetAbility(Attributes.Ability.CastRating);
            }
            
            castRating *= Difficulty;
            
            // Calculate the different values
            float upper = castRating * castRating;
            float lower = upper + (1.75f * Defense * Defense);
            float ratio = upper / lower;
            
            // Never below 5% chance
            ratio = Math.Max(0.05f, ratio);
            
            // Roll against ratio
            float roll = (float)new Random().NextDouble();
            
            return roll <= ratio;
        }
        
        /// <summary>
        /// Checks if this effect has a projectile
        /// </summary>
        public bool HasProjectile()
        {
            return ProjectileType > 0;
        }
        
        /// <summary>
        /// Checks if this effect has an explosion
        /// </summary>
        public bool HasExplosion()
        {
            return ExplosionType > 0;
        }
        
        /// <summary>
        /// Copies properties from another effect
        /// </summary>
        public void CopyFrom(EffectType other)
        {
            if (other == null)
                return;
                
            DataName = other.DataName;
            DisplayName = other.DisplayName;
            
            Type = other.Type;
            Alignment = other.Alignment;
            Color = other.Color;
            Save = other.Save;
            EffectId = other.EffectId;
            
            for (int i = 0; i < (int)TargetType.Count; i++)
            {
                TargetSettings[i] = other.TargetSettings[i];
            }
            
            Requirement.MainType = other.Requirement.MainType;
            Requirement.SubType = other.Requirement.SubType;
            Requirement.SubValue = other.Requirement.SubValue;
            
            Range = other.Range;
            Radius = other.Radius;
            Flags = other.Flags;
            ActionTime = other.ActionTime;
            RecoveryTime = other.RecoveryTime;
            
            BasePower = other.BasePower;
            Difficulty = other.Difficulty;
            ManaCost = other.ManaCost;
            
            for (int i = 0; i < Parameters.Length; i++)
            {
                Parameters[i] = other.Parameters[i];
            }
            
            StringParam = other.StringParam;
            Defense = other.Defense;
            
            for (int i = 0; i < ParamMultipliers.Length; i++)
            {
                ParamMultipliers[i] = other.ParamMultipliers[i];
            }
            
            RangeIncrease = other.RangeIncrease;
            RadiusIncrease = other.RadiusIncrease;
            
            Icon = other.Icon;
            ProjectileType = other.ProjectileType;
            ProjectileSprite = other.ProjectileSprite;
            ProjectileSound = other.ProjectileSound;
            ExplosionType = other.ExplosionType;
            ExplosionSprite = other.ExplosionSprite;
            ExplosionSound = other.ExplosionSound;
            
            DesignNotes = other.DesignNotes;
        }
        
        /// <summary>
        /// Writes effect to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write((int)EffectFileVersions.Current);
                
                // Write names
                WriteFixedString(writer, DataName, EffectConstants.NameLength);
                WriteFixedString(writer, DisplayName, EffectConstants.NameLength);
                
                // Write basic info
                writer.Write(Type);
                writer.Write(Alignment);
                writer.Write(Color);
                writer.Write(Save);
                
                // Write target settings
                writer.Write(TargetSettings[(int)TargetType.Self]);
                writer.Write(TargetSettings[(int)TargetType.Enemies]);
                writer.Write(TargetSettings[(int)TargetType.Allies]);
                
                // Write requirements
                writer.Write(Requirement.MainType);
                writer.Write(Requirement.SubType);
                writer.Write(Requirement.SubValue);
                
                // Write range and radius
                writer.Write(Range);
                writer.Write(Radius);
                writer.Write(Flags);
                writer.Write(ActionTime);
                writer.Write(RecoveryTime);
                
                // Write power settings
                writer.Write(BasePower);
                writer.Write(Difficulty);
                
                // Write parameter multipliers
                writer.Write(ParamMultipliers[0]);
                writer.Write(ParamMultipliers[1]);
                writer.Write(ParamMultipliers[2]);
                writer.Write(RangeIncrease);
                writer.Write(RadiusIncrease);
                
                // Write mana cost
                writer.Write(ManaCost);
                
                // Write parameters
                writer.Write(Parameters[0]);
                writer.Write(Parameters[1]);
                writer.Write(Parameters[2]);
                WriteFixedString(writer, StringParam, 32);
                
                // Write visuals
                writer.Write(Icon);
                writer.Write(ProjectileType);
                writer.Write(ProjectileSprite);
                writer.Write(ProjectileSound);
                writer.Write(ExplosionType);
                writer.Write(ExplosionSprite);
                writer.Write(ExplosionSound);
                
                // Write design notes
                if (string.IsNullOrEmpty(DesignNotes))
                {
                    writer.Write(0);
                }
                else
                {
                    writer.Write(DesignNotes.Length);
                    writer.Write(DesignNotes.ToCharArray());
                }
                
                // Write defense and effect ID
                writer.Write(Defense);
                writer.Write(EffectId);
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
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
        
        /// <summary>
        /// Reads an effect from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                Clear();
                
                // Read version
                int version = reader.ReadInt32();
                
                // Read name
                DataName = new string(reader.ReadChars(EffectConstants.NameLength)).TrimEnd('\0');
                
                if (version > EffectFileVersions.V_030215)
                {
                    DisplayName = new string(reader.ReadChars(EffectConstants.NameLength)).TrimEnd('\0');
                }
                else
                {
                    DisplayName = DataName;
                }
                
                Type = reader.ReadInt32();
                Alignment = reader.ReadInt32();
                Color = reader.ReadInt32();
                Save = reader.ReadInt32();
                
                if (version > EffectFileVersions.V_021108)
                {
                    TargetSettings[(int)TargetType.Self] = reader.ReadInt32();
                    TargetSettings[(int)TargetType.Enemies] = reader.ReadInt32();
                    TargetSettings[(int)TargetType.Allies] = reader.ReadInt32();
                }
                
                if (version > EffectFileVersions.V_021110)
                {
                    Requirement.MainType = reader.ReadInt32();
                    Requirement.SubType = reader.ReadInt32();
                    Requirement.SubValue = reader.ReadInt32();
                }
                
                Range = reader.ReadInt32();
                Radius = reader.ReadInt32();
                
                if (version > EffectFileVersions.V_030215)
                {
                    Flags = reader.ReadInt32();
                    ActionTime = reader.ReadInt32();
                    RecoveryTime = reader.ReadInt32();
                }
                
                BasePower = reader.ReadSingle();
                
                if (version > EffectFileVersions.V_021013)
                {
                    Difficulty = reader.ReadSingle();
                }
                
                if (version > EffectFileVersions.V_021108)
                {
                    if (version > EffectFileVersions.V_021127)
                    {
                        ParamMultipliers[0] = reader.ReadSingle();
                        ParamMultipliers[1] = reader.ReadSingle();
                        ParamMultipliers[2] = reader.ReadSingle();
                        RangeIncrease = reader.ReadInt32();
                        RadiusIncrease = reader.ReadInt32();
                    }
                    else
                    {
                        // Old format used a single value for all multipliers
                        float temp = reader.ReadSingle();
                        ParamMultipliers[0] = temp;
                        ParamMultipliers[1] = temp;
                        ParamMultipliers[2] = temp;
                        
                        RangeIncrease = 0;
                        RadiusIncrease = 0;
                    }
                }
                
                if (version > EffectFileVersions.V_021015)
                {
                    ManaCost = reader.ReadSingle();
                }
                
                Parameters[0] = reader.ReadSingle();
                Parameters[1] = reader.ReadSingle();
                Parameters[2] = reader.ReadSingle();
                StringParam = new string(reader.ReadChars(32)).TrimEnd('\0');
                
                if (version > EffectFileVersions.V_030330)
                {
                    Icon = reader.ReadInt32();
                }
                
                if (version > EffectFileVersions.V_021127b)
                {
                    ProjectileType = reader.ReadInt32();
                    ProjectileSprite = reader.ReadInt32();
                    ProjectileSound = reader.ReadInt32();
                    ExplosionType = reader.ReadInt32();
                    ExplosionSprite = reader.ReadInt32();
                    ExplosionSound = reader.ReadInt32();
                    
                    // Read design notes
                    int notesLength = reader.ReadInt32();
                    if (notesLength > 0)
                    {
                        char[] notesChars = reader.ReadChars(notesLength);
                        DesignNotes = new string(notesChars);
                    }
                }
                
                if (version > EffectFileVersions.V_021128)
                {
                    Defense = reader.ReadSingle();
                }
                
                if (version > EffectFileVersions.V_030407)
                {
                    EffectId = reader.ReadInt32();
                }
                
                if (version <= EffectFileVersions.V_030520)
                {
                    RecoveryTime = (int)(RecoveryTime * 0.70f);
                }
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}