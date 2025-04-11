using System;
using RpgSystem.Entities;

namespace RpgSystem.Combat
{
    /// <summary>
    /// Possible hit results for melee and ranged attacks
    /// </summary>
    public enum HitResultType
    {
        Normal,
        Critical,
        Glancing,
        Miss,
        Fumble,
        FarMiss     // Ranged attack result - never even reached target
    }

    /// <summary>
    /// Damage types from various sources
    /// </summary>
    public enum DamageType
    {
        Unknown = 0,
        
        // Physical damage types
        Slashing,
        Piercing,
        Crushing,
        Chopping,
        
        // Magical damage types
        White,
        Red,
        Green,
        Blue,
        Yellow,
        
        Count
    }

    /// <summary>
    /// Attack types for weapons
    /// </summary>
    public enum AttackType
    {
        // Available to players and monsters
        NormalMelee = 0,
        Bullet,             // Ranged bullet attack (slings)
        Arrow,              // Ranged arrow attack (bows and crossbows)
        
        // Monster-only attacks
        DoubleMelee,        // Will attack twice at the same time
        TripleMelee         // Will attack three times at the same time
    }

    /// <summary>
    /// Attack names for flavor text
    /// </summary>
    public enum AttackName
    {
        Hits = 0,
        Slashes,
        Crushes,
        Chops,
        Pierces,
        Punches,
        Bites,
        Claws,
        Squeezes,
        Burns,
        Freezes,
        Shocks,
        Blasts,
        Drains,
        Stings,
        Whips,
        
        Count
    }

    /// <summary>
    /// Flags that can be applied to attacks
    /// </summary>
    [Flags]
    public enum AttackFlag
    {
        None = 0,
        IgnoreShield = 0x1  // Ignores the defender's shield AC
    }

    /// <summary>
    /// Constants for attack calculations
    /// </summary>
    public static class AttackConstants
    {
        public const int FumbleRecoveryMultiplier = 2;  // Extra time upon fumbles
    }

    /// <summary>
    /// Static class for combat calculations
    /// </summary>
    public static class AttackMath
    {
        private static readonly Random random = new Random();
        private static readonly string[] AttackNameSingular = new string[]
        {
            "hit",
            "slash",
            "crush",
            "chop",
            "pierce",
            "punch",
            "bite",
            "claw",
            "squeez",
            "burn",
            "freez",
            "shock",
            "blast",
            "drain",
            "sting",
            "whip"
        };

        private static readonly string[] AttackNamePlural = new string[]
        {
            "hits",
            "slashes",
            "crushes",
            "chops",
            "pierces",
            "punches",
            "bites",
            "claws",
            "squeezes",
            "burns",
            "freezes",
            "shocks",
            "blasts",
            "drains",
            "stings",
            "whips"
        };

        /// <summary>
        /// Calculates the result of an attack between attacker and target
        /// </summary>
        public static HitResultType GetAttackResult(IAttributeObject attacker, IAttributeObject target)
        {
            if (attacker == null || target == null)
            {
                return HitResultType.Miss;
            }

            // Check distance to target
            int x1, y1, x2, y2;
            attacker.GetCoordinates(out x1, out y1);
            target.GetCoordinates(out x2, out y2);

            x2 -= x1;
            y2 -= y1;

            float distanceChance = attacker.GetDistanceAccuracy(Common.GetCellDistance(x2, y2));
            if (distanceChance <= 0)
            {
                return HitResultType.FarMiss;
            }

            // Check if the attack lands at all based on distance
            float r = (float)random.NextDouble();
            if (r > distanceChance)
            {
                return HitResultType.FarMiss;
            }

            // Main attack calculation
            float attackRating = attacker.GetAbility(Attributes.Ability.AttackRating);
            float armorRating = target.GetAbility(Attributes.Ability.ArmorClass);

            AttackFlag flags = (AttackFlag)attacker.GetAttackFlags(target);
            if (flags.HasFlag(AttackFlag.IgnoreShield))
            {
                armorRating -= target.GetAbility(Attributes.Ability.ArmorClass, (int)Attributes.GetAbilitySettings.ShieldOnly);
            }

            // Calculate hit ratio
            float ratio = (attackRating / (attackRating + (armorRating * 2.0f))) + 
                         ((attackRating - (armorRating * 1.5f)) * 0.0030f);

            // Limit the ratio between 5% and 95%
            ratio = Math.Max(0.05f, Math.Min(0.95f, ratio));

            // Determine hit or miss
            r = (float)random.NextDouble();
            if (r <= ratio)
            {
                // Hit! Check for critical
                float criticalHit = attacker.GetCriticalValue();
                r = (float)random.NextDouble();
                
                if (r <= criticalHit)
                {
                    return HitResultType.Critical;
                }
                
                return HitResultType.Normal;
            }
            else
            {
                // Miss - Check for glancing hit
                ratio = (attackRating / (attackRating + (armorRating * 1.5f)));
                ratio = Math.Max(0.05f, Math.Min(0.75f, ratio));
                
                r = (float)random.NextDouble();
                if (r <= ratio)
                {
                    return HitResultType.Glancing;
                }

                // Check for fumble
                float fumble = attacker.GetFumbleValue();
                r = (float)random.NextDouble();
                if (r <= fumble)
                {
                    return HitResultType.Fumble;
                }
            }

            return HitResultType.Miss;
        }

        /// <summary>
        /// Calculates damage amount based on attack result
        /// </summary>
        public static float GetDamageAmount(float min, float max, HitResultType attackResult)
        {
            switch (attackResult)
            {
                case HitResultType.Critical:
                    return max;
                    
                case HitResultType.Miss:
                case HitResultType.Fumble:
                case HitResultType.FarMiss:
                    return 0;
                    
                case HitResultType.Normal:
                    {
                        float delta = max - min;
                        float amount = min;
                        
                        if (delta > 0)
                        {
                            float percentage = (float)random.NextDouble();
                            amount += delta * percentage;
                        }
                        
                        return Math.Max(1, amount);
                    }
                    
                case HitResultType.Glancing:
                    {
                        float delta = max - min;
                        float amount = min;
                        
                        if (delta > 0)
                        {
                            float percentage = (float)random.NextDouble();
                            amount += delta * percentage;
                        }
                        
                        amount *= 0.33f;
                        return Math.Max(0, amount);
                    }
                    
                default:
                    return 0;
            }
        }

        /// <summary>
        /// Returns true if a spell effect is negated by target's resistance
        /// </summary>
        public static bool TestNegateEffect(float castRating, float resistance)
        {
            // Calculate chance of effect succeeding
            float upper = (castRating * castRating);
            float lower = upper + (1.75f * resistance * resistance);
            float ratio = (upper / lower);

            // Never below 5% chance
            ratio = Math.Max(0.05f, ratio);

            // Roll for effect
            float roll = (float)random.NextDouble();
            return roll > ratio; // True means negated
        }

        /// <summary>
        /// Tests a decreasing effect against resistance and returns the multiplier
        /// </summary>
        public static float TestDecreaseEffect(float castRating, float resistance)
        {
            if (resistance <= 0)
            {
                return 2.5f;
            }

            float damage = castRating / (resistance * 2);

            // Calculate maximum multiplier based on resistance
            float maxMultiplier = 2.5f + (resistance / 20.0f);
            damage = Math.Min(damage, maxMultiplier);

            // Random factor
            float roll = (float)random.NextDouble();
            if (roll < 0.05f)
            {
                damage *= 0.5f;
            }
            else if (roll >= 0.95f)
            {
                damage *= 1.5f;
            }

            return damage;
        }

        /// <summary>
        /// Gets the name of an attack for display purposes
        /// </summary>
        public static string GetAttackName(AttackName name, bool plural)
        {
            if ((int)name < 0 || (int)name >= (int)AttackName.Count)
            {
                return plural ? "hits" : "hit";
            }

            return plural ? AttackNamePlural[(int)name] : AttackNameSingular[(int)name];
        }
    }
}