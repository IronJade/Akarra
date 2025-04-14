using System;

namespace RpgSystem.Attributes
{
    /// <summary>
    /// Constants, enumerations and utilities for the abilities system.
    /// Abilities are derived from attributes and are used in most calculations.
    /// </summary>
    public static class AbilitySystem
    {
        /// <summary>
        /// Abilities used in combat and other calculations
        /// </summary>
        public enum Abilities
        {
            // Combat Skills
            AttackRating = 0,    // Accuracy in combat
            CastRating,          // Spell effectiveness
            
            // Damage
            DamageMin,           // Minimum damage in combat
            DamageMax,           // Maximum damage in combat
            
            // Defense
            ArmorClass,          // Base defense value
            
            // Resistances
            ResistWhite,         // Magic resistance (white)
            ResistRed,           // Magic resistance (red)
            ResistGreen,         // Magic resistance (green)
            ResistBlue,          // Magic resistance (blue)
            ResistYellow,        // Magic resistance (yellow)
            
            // Damage Reduction
            Reduction,           // Physical damage reduction
            
            Count                // Number of abilities
        }
        
        // Name lookup table for abilities
        private static readonly string[] AbilityNames = new string[]
        {
            "Attack Rating",
            "Cast Rating",
            "Minimum Damage",
            "Maximum Damage",
            "Armor Class",
            "Resist White",
            "Resist Red",
            "Resist Green",
            "Resist Blue",
            "Resist Yellow",
            "Damage Reduction"
        };
        
        /// <summary>
        /// Gets the name of an ability
        /// </summary>
        /// <param name="ability">The ability index</param>
        /// <returns>Name of the ability</returns>
        public static string GetAbilityName(int ability)
        {
            if (ability < 0 || ability >= (int)Abilities.Count)
            {
                return "Unknown Ability";
            }
            
            return AbilityNames[ability];
        }
        
        /// <summary>
        /// Gets the name of an ability
        /// </summary>
        /// <param name="ability">The ability enum value</param>
        /// <returns>Name of the ability</returns>
        public static string GetAbilityName(Abilities ability)
        {
            return GetAbilityName((int)ability);
        }
        
        /// <summary>
        /// Calculates attribute abilities based on base attributes
        /// </summary>
        /// <param name="attributes">Array of base attributes</param>
        /// <param name="abilities">Array to store calculated abilities</param>
        public static void CalculateAbilities(CAttribute[] attributes, CAttribute[] abilities)
        {
            if (attributes == null || abilities == null || 
                attributes.Length < (int)BaseAttributes.Count || 
                abilities.Length < (int)Abilities.Count)
            {
                return;
            }
            
            // Attack Rating is based on Agility
            abilities[(int)Abilities.AttackRating].SetBase(
                attributes[(int)BaseAttributes.Agility].GetValue() * 0.75f);
            
            // Cast Rating is based on Intelligence
            abilities[(int)Abilities.CastRating].SetBase(
                attributes[(int)BaseAttributes.Intelligence].GetValue() * 0.40f);
            
            // Damage is based on Strength
            abilities[(int)Abilities.DamageMin].SetBase(
                1.5f + attributes[(int)BaseAttributes.Strength].GetValue() * 0.02f);
            abilities[(int)Abilities.DamageMax].SetBase(
                4.0f + attributes[(int)BaseAttributes.Strength].GetValue() * 0.04f);
            
            // Strength also gives percentage bonus to damage
            abilities[(int)Abilities.DamageMin].AddPercentBonus(
                attributes[(int)BaseAttributes.Strength].GetValue() * 0.0050f);
            abilities[(int)Abilities.DamageMax].AddPercentBonus(
                attributes[(int)BaseAttributes.Strength].GetValue() * 0.0050f);
            
            // Armor Class is based on Agility
            abilities[(int)Abilities.ArmorClass].SetBase(
                attributes[(int)BaseAttributes.Agility].GetValue() * 0.25f);
            
            // Resistances are based on different attributes
            abilities[(int)Abilities.ResistWhite].SetBase(
                3.0f + attributes[(int)BaseAttributes.Faith].GetValue() * 0.125f);
            abilities[(int)Abilities.ResistRed].SetBase(
                3.0f + attributes[(int)BaseAttributes.Constitution].GetValue() * 0.125f);
            abilities[(int)Abilities.ResistGreen].SetBase(
                3.0f + attributes[(int)BaseAttributes.Intelligence].GetValue() * 0.125f);
            abilities[(int)Abilities.ResistBlue].SetBase(
                3.0f + attributes[(int)BaseAttributes.Constitution].GetValue() * 0.125f);
            abilities[(int)Abilities.ResistYellow].SetBase(
                3.0f + attributes[(int)BaseAttributes.Constitution].GetValue() * 0.125f);
            
            // Damage Reduction starts at 0
            abilities[(int)Abilities.Reduction].SetBase(0);
        }
        
        /// <summary>
        /// Calculates variable attributes (health, mana, stamina) based on base attributes
        /// </summary>
        /// <param name="attributes">Array of base attributes</param>
        /// <param name="variables">Array to store calculated variable attributes</param>
        public static void CalculateVariableAttributes(CAttribute[] attributes, CVariableAttribute[] variables)
        {
            if (attributes == null || variables == null || 
                attributes.Length < (int)BaseAttributes.Count || 
                variables.Length < (int)VarAttributes.Count)
            {
                return;
            }
            
            // Health is based on Constitution
            variables[(int)VarAttributes.Health].SetBase(
                attributes[(int)BaseAttributes.Constitution].GetValue() * 2.0f);
            
            // Mana is based on Faith
            variables[(int)VarAttributes.Mana].SetBase(
                attributes[(int)BaseAttributes.Faith].GetValue() * 1.5f);
            
            // Stamina is based on Constitution
            variables[(int)VarAttributes.Stamina].SetBase(
                attributes[(int)BaseAttributes.Constitution].GetValue() * 3.0f);
        }
    }
}