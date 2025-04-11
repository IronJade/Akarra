using System;
using System.Text;

namespace RpgSystem.Attributes
{
    /// <summary>
    /// Types of requirements for items, effects, and other systems
    /// </summary>
    public enum RequirementType
    {
        Disabled = -1,      // Requirement is not used
        Level = 0,          // Character must have a minimum level
        Deity,              // Character must follow a specific deity (Gifted or Shining)
        Gender,             // Character must be a specific gender
        Color,              // Character must have a specific soul color
        
        Class,              // Character must be of a specific class
        Skill,              // Character must have a specific skill
        Locked,             // Character must have specialized in a skill
        
        Strength,           // Character must have minimum Strength
        Constitution,       // Character must have minimum Constitution
        Agility,            // Character must have minimum Agility
        Intelligence,       // Character must have minimum Intelligence
        Faith,              // Character must have minimum Faith
        
        Count
    }
    
    /// <summary>
    /// Represents a requirement that must be met by a character
    /// </summary>
    public class Requirement
    {
        // Fields
        public int Type { get; set; }   // Type of requirement
        public int Value { get; set; }  // Value for the requirement
        
        /// <summary>
        /// Creates a new requirement
        /// </summary>
        public Requirement()
        {
            Type = (int)RequirementType.Disabled;
            Value = 0;
        }
        
        /// <summary>
        /// Creates a new requirement with specified values
        /// </summary>
        public Requirement(int type, int value)
        {
            Type = type;
            Value = value;
        }
        
        /// <summary>
        /// Gets a string description of this requirement
        /// </summary>
        public string GetDescription()
        {
            StringBuilder sb = new StringBuilder();
            
            switch (Type)
            {
                case (int)RequirementType.Level:
                    sb.Append("Level ");
                    sb.Append(Value);
                    sb.Append(" Minimum");
                    break;
                    
                case (int)RequirementType.Deity:
                    if (Value == 0)
                        sb.Append("Gifted Only");
                    else
                        sb.Append("Shining Only");
                    break;
                    
                case (int)RequirementType.Gender:
                    if (Value == 0)
                        sb.Append("Male Only");
                    else
                        sb.Append("Female Only");
                    break;
                    
                case (int)RequirementType.Color:
                    sb.Append(GetSoulColorName(Value));
                    sb.Append(" Soul Color Only");
                    break;
                    
                case (int)RequirementType.Class:
                    sb.Append(GetClassName(Value));
                    sb.Append(" Only");
                    break;
                    
                case (int)RequirementType.Skill:
                    sb.Append(GetSkillName(Value));
                    sb.Append(" Required");
                    break;
                    
                case (int)RequirementType.Locked:
                    sb.Append("Specialized ");
                    sb.Append(GetSkillName(Value));
                    sb.Append(" Required");
                    break;
                    
                case (int)RequirementType.Strength:
                case (int)RequirementType.Constitution:
                case (int)RequirementType.Agility:
                case (int)RequirementType.Intelligence:
                case (int)RequirementType.Faith:
                    sb.Append(Value);
                    sb.Append(" ");
                    sb.Append(GetAttributeName(Type - (int)RequirementType.Strength));
                    break;
                    
                default:
                    return string.Empty;
            }
            
            return sb.ToString();
        }
        
        /// <summary>
        /// Gets the name of a soul color
        /// </summary>
        private string GetSoulColorName(int color)
        {
            switch (color)
            {
                case 0: return "White";
                case 1: return "Red";
                case 2: return "Green";
                case 3: return "Blue";
                case 4: return "Yellow";
                default: return "Unknown";
            }
        }
        
        /// <summary>
        /// Gets the name of a character class
        /// </summary>
        private string GetClassName(int classValue)
        {
            // This would typically reference the class system
            // For simplicity, we're returning "Class" + the bit value
            return "Class " + classValue;
        }
        
        /// <summary>
        /// Gets the name of a skill
        /// </summary>
        private string GetSkillName(int skillId)
        {
            // This would typically reference the skill system
            // For simplicity, we're returning "Skill" + the ID
            return "Skill " + skillId;
        }
        
        /// <summary>
        /// Gets the name of an attribute
        /// </summary>
        private string GetAttributeName(int attribute)
        {
            switch (attribute)
            {
                case 0: return "Strength";
                case 1: return "Constitution";
                case 2: return "Agility";
                case 3: return "Intelligence";
                case 4: return "Faith";
                default: return "Unknown";
            }
        }
        
        /// <summary>
        /// Tests if a character meets this requirement
        /// </summary>
        public bool TestCharacter(Character.CharacterAttributes attributes)
        {
            if (Type == (int)RequirementType.Disabled)
                return true;
                
            if (attributes == null)
                return false;
                
            switch (Type)
            {
                case (int)RequirementType.Level:
                    return attributes.Stats.Level >= Value;
                    
                case (int)RequirementType.Deity:
                    return (attributes.Stats.God == 0 && Value == 0) || 
                           (attributes.Stats.God == 1 && Value == 1);
                    
                case (int)RequirementType.Gender:
                    return (attributes.Stats.Gender == 0 && Value == 0) ||
                           (attributes.Stats.Gender == 1 && Value == 1);
                    
                case (int)RequirementType.Color:
                    return attributes.Stats.SoulColor == Value;
                    
                case (int)RequirementType.Class:
                    return ((1 << attributes.Stats.PlayerClass) & Value) != 0;
                    
                case (int)RequirementType.Skill:
                    return attributes.HasSkill(Value);
                    
                case (int)RequirementType.Locked:
                    return attributes.HasSkill(Value, true);
                    
                case (int)RequirementType.Strength:
                    return attributes.Stats.Attributes[(int)BaseAttribute.Strength].GetValue() >= Value;
                    
                case (int)RequirementType.Constitution:
                    return attributes.Stats.Attributes[(int)BaseAttribute.Constitution].GetValue() >= Value;
                    
                case (int)RequirementType.Agility:
                    return attributes.Stats.Attributes[(int)BaseAttribute.Agility].GetValue() >= Value;
                    
                case (int)RequirementType.Intelligence:
                    return attributes.Stats.Attributes[(int)BaseAttribute.Intelligence].GetValue() >= Value;
                    
                case (int)RequirementType.Faith:
                    return attributes.Stats.Attributes[(int)BaseAttribute.Faith].GetValue() >= Value;
                    
                default:
                    return true;
            }
        }
        
        /// <summary>
        /// Creates a copy of this requirement
        /// </summary>
        public Requirement Clone()
        {
            return new Requirement(Type, Value);
        }
    }
}