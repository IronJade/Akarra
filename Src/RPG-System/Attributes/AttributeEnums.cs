using System;

namespace RpgSystem.Attributes
{
    /// <summary>
    /// Primary attributes that form the basis of a character or entity.
    /// </summary>
    public enum BaseAttributes
    {
        Strength = 0,        // Physical power, affects damage
        Constitution,        // Physical resilience, affects health
        Agility,             // Physical dexterity and speed, affects attack rating
        Intelligence,        // Mental acuity, affects magic capability
        Faith,               // Spiritual power, affects divine abilities
        
        Count                // Number of base attributes
    }
    
    /// <summary>
    /// Variable attributes that fluctuate during gameplay.
    /// These are resources consumed or replenished during gameplay.
    /// </summary>
    public enum VarAttributes
    {
        Health = 0,          // Physical wellbeing, reduced by damage
        Mana,                // Magical energy, consumed by spells
        Stamina,             // Physical energy, consumed by actions
        
        Count                // Number of variable attributes
    }
    
    /// <summary>
    /// Utilities for working with base attributes and variable attributes
    /// </summary>
    public static class AttributeUtils
    {
        // Name lookup tables for attributes
        private static readonly string[] BaseAttributeNames = new string[]
        {
            "Strength",
            "Constitution",
            "Agility",
            "Intelligence",
            "Faith"
        };
        
        private static readonly string[] VarAttributeNames = new string[]
        {
            "Health",
            "Mana",
            "Stamina"
        };
        
        /// <summary>
        /// Gets the name of a base attribute
        /// </summary>
        /// <param name="attribute">The attribute index</param>
        /// <returns>Name of the attribute</returns>
        public static string GetBaseAttributeName(int attribute)
        {
            if (attribute < 0 || attribute >= (int)BaseAttributes.Count)
            {
                return "Unknown Attribute";
            }
            
            return BaseAttributeNames[attribute];
        }
        
        /// <summary>
        /// Gets the name of a base attribute
        /// </summary>
        /// <param name="attribute">The attribute enum value</param>
        /// <returns>Name of the attribute</returns>
        public static string GetBaseAttributeName(BaseAttributes attribute)
        {
            return GetBaseAttributeName((int)attribute);
        }
        
        /// <summary>
        /// Gets the name of a variable attribute
        /// </summary>
        /// <param name="attribute">The attribute index</param>
        /// <returns>Name of the attribute</returns>
        public static string GetVarAttributeName(int attribute)
        {
            if (attribute < 0 || attribute >= (int)VarAttributes.Count)
            {
                return "Unknown Variable";
            }
            
            return VarAttributeNames[attribute];
        }
        
        /// <summary>
        /// Gets the name of a variable attribute
        /// </summary>
        /// <param name="attribute">The attribute enum value</param>
        /// <returns>Name of the attribute</returns>
        public static string GetVarAttributeName(VarAttributes attribute)
        {
            return GetVarAttributeName((int)attribute);
        }
        
        /// <summary>
        /// Gets the default starting value for a base attribute based on race
        /// </summary>
        /// <param name="attribute">The attribute</param>
        /// <param name="race">Character race (0=Human, 1=Menit, 2=Iwid)</param>
        /// <returns>Default starting value</returns>
        public static float GetStartingValue(BaseAttributes attribute, int race)
        {
            // Starting attribute values by race
            float[,] startingValues = new float[,]
            {
                // Human
                { 15.0f, 15.0f, 15.0f, 15.0f, 15.0f },
                // Menit
                { 23.0f, 21.0f, 18.0f, 18.0f, 20.0f },
                // Iwid
                { 18.0f, 18.0f, 21.0f, 23.0f, 20.0f }
            };
            
            if (race < 0 || race > 2 || (int)attribute < 0 || (int)attribute >= (int)BaseAttributes.Count)
            {
                return 15.0f; // Default value
            }
            
            return startingValues[race, (int)attribute];
        }
        
        /// <summary>
        /// Calculates the attribute points required for the next level based on current level
        /// </summary>
        /// <param name="level">Current character level</param>
        /// <returns>Attribute points gained per level</returns>
        public static int GetPointsPerLevel(int level)
        {
            if (level < 100)
            {
                return 5; // 5 points per level up to level 100
            }
            else if (level < 125)
            {
                return 3; // 3 points per level from 100-124
            }
            else
            {
                return 0; // No more attribute points after level 125
            }
        }
    }
}