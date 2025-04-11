using System;
using System.IO;

namespace RpgSystem.Attributes
{
    /// <summary>
    /// Base attribute class that handles the core functionality of all attributes.
    /// Attributes have a base value, a bonus value, and a percentage bonus.
    /// </summary>
    public class Attribute
    {
        protected float baseValue;
        protected float bonus;
        protected float percentBonus;

        public Attribute()
        {
            baseValue = 0;
            bonus = 0;
            percentBonus = 0;
        }

        /// <summary>
        /// Sets the base value of the attribute.
        /// </summary>
        public virtual void SetBase(float value)
        {
            baseValue = value;
        }

        /// <summary>
        /// Resets all bonuses to zero.
        /// </summary>
        public virtual void ResetBonus()
        {
            bonus = 0;
            percentBonus = 0;
        }

        /// <summary>
        /// Adds a flat bonus to the attribute.
        /// </summary>
        public virtual void AddBonus(float bonusValue)
        {
            bonus += bonusValue;
        }

        /// <summary>
        /// Adds a percentage bonus to the attribute (0.1 = 10%).
        /// </summary>
        public virtual void AddPercentBonus(float percentValue)
        {
            percentBonus += percentValue;
        }

        /// <summary>
        /// Gets the base value of the attribute.
        /// </summary>
        public virtual float GetBase()
        {
            return baseValue;
        }

        /// <summary>
        /// Gets the bonus value of the attribute.
        /// </summary>
        public virtual float GetBonus()
        {
            return bonus;
        }

        /// <summary>
        /// Gets the percentage bonus of the attribute.
        /// </summary>
        public virtual float GetPercentBonus()
        {
            return percentBonus;
        }

        /// <summary>
        /// Gets the total value of the attribute (base + bonus) * (1 + percentBonus).
        /// Never returns less than 0.
        /// </summary>
        public virtual float GetValue()
        {
            float value = (baseValue + bonus) * (percentBonus + 1.0f);
            return value < 0 ? 0.0f : value;
        }

        /// <summary>
        /// Reads attribute data from a binary stream.
        /// </summary>
        public virtual bool Read(BinaryReader reader, int version)
        {
            baseValue = reader.ReadSingle();
            bonus = reader.ReadSingle();
            percentBonus = reader.ReadSingle();
            return true;
        }

        /// <summary>
        /// Writes attribute data to a binary stream.
        /// </summary>
        public virtual bool Write(BinaryWriter writer, int version)
        {
            writer.Write(baseValue);
            writer.Write(bonus);
            writer.Write(percentBonus);
            return true;
        }
    }
}