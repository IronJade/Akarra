namespace RpgSystem.Attributes
{
    /// <summary>
    /// Enumeration of base attributes in the game system.
    /// </summary>
    public enum BaseAttribute
    {
        Strength = 0,
        Constitution,
        Agility,
        Intelligence,
        Faith,
        
        Count
    }

    /// <summary>
    /// Enumeration of variable attributes in the game system.
    /// </summary>
    public enum VariableAttributeType
    {
        Health = 0,
        Mana,
        Stamina,
        
        Count
    }

    /// <summary>
    /// Abilities are derived from attributes and used in various game calculations.
    /// </summary>
    public enum Ability
    {
        // Combat Skills
        AttackRating = 0,
        CastRating,

        // Damage
        DamageMinimum,
        DamageMaximum,

        // Defense
        ArmorClass,

        // Resistances
        ResistWhite,
        ResistRed,
        ResistGreen,
        ResistBlue,
        ResistYellow,

        // Secondary defense
        DamageReduction,

        Count
    }

    /// <summary>
    /// Special settings for ability calculations.
    /// </summary>
    public enum GetAbilitySettings
    {
        None = 0,
        ShieldOnly,   // Used when we only want the armor class for the shield
        ArmorOnly,    // Same, but for armor (ignores shield and agility bonus)
        
        Count
    }

    /// <summary>
    /// Static helper class for attribute-related functions.
    /// </summary>
    public static class AttributeHelper
    {
        private static readonly string[] AttributeNames = new string[]
        {
            "Strength",
            "Constitution",
            "Agility",
            "Intelligence",
            "Faith"
        };

        private static readonly string[] VariableAttributeNames = new string[]
        {
            "Health",
            "Mana",
            "Stamina"
        };

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
        /// Gets the name of a base attribute.
        /// </summary>
        public static string GetAttributeName(BaseAttribute attribute)
        {
            return AttributeNames[(int)attribute];
        }

        /// <summary>
        /// Gets the name of a variable attribute.
        /// </summary>
        public static string GetVariableAttributeName(VariableAttributeType attribute)
        {
            return VariableAttributeNames[(int)attribute];
        }

        /// <summary>
        /// Gets the name of an ability.
        /// </summary>
        public static string GetAbilityName(Ability ability)
        {
            return AbilityNames[(int)ability];
        }
    }
}