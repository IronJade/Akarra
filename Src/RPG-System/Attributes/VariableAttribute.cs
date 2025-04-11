using System;
using System.IO;

namespace RpgSystem.Attributes
{
    /// <summary>
    /// An attribute that has a current value that can fluctuate independently of the maximum value.
    /// Used for health, mana, stamina, etc.
    /// </summary>
    public class VariableAttribute : Attribute
    {
        protected float current;

        public VariableAttribute() : base()
        {
            current = 0;
        }

        /// <summary>
        /// Sets the base value and updates current value if needed.
        /// </summary>
        public override void SetBase(float value)
        {
            base.SetBase(value);

            // If the current value exceeds the new maximum, adjust it
            if (current > GetValue())
            {
                current = GetValue();
            }
        }

        /// <summary>
        /// Resets bonuses and adjusts current value if necessary.
        /// </summary>
        public override void ResetBonus()
        {
            // Get value before reset to calculate the difference
            float oldValue = GetValue();
            
            base.ResetBonus();
            
            // Calculate the delta and adjust current value
            float delta = GetValue() - oldValue;
            current += delta;
            
            // Ensure we don't exceed the new maximum
            if (current > GetValue())
            {
                current = GetValue();
            }
        }

        /// <summary>
        /// Adds a bonus and adjusts current value proportionally.
        /// </summary>
        public override void AddBonus(float bonusValue)
        {
            float oldValue = GetValue();
            
            base.AddBonus(bonusValue);
            
            float delta = GetValue() - oldValue;
            current += delta;
            
            if (current > GetValue())
            {
                current = GetValue();
            }
        }

        /// <summary>
        /// Adds a percentage bonus and adjusts current value proportionally.
        /// </summary>
        public override void AddPercentBonus(float percentValue)
        {
            float oldValue = GetValue();
            
            base.AddPercentBonus(percentValue);
            
            float delta = GetValue() - oldValue;
            current += delta;
            
            if (current > GetValue())
            {
                current = GetValue();
            }
        }

        /// <summary>
        /// Gets the current value.
        /// </summary>
        public virtual float GetCurrent()
        {
            return current;
        }

        /// <summary>
        /// Sets the current value. Never exceeds the maximum value.
        /// </summary>
        public virtual void SetCurrent(float value)
        {
            current = Math.Min(value, GetValue());
        }

        /// <summary>
        /// Adds to the current value and returns the new value.
        /// </summary>
        public virtual float AddToCurrent(float amount)
        {
            current += amount;
            
            if (current > GetValue())
            {
                current = GetValue();
            }
            
            return current;
        }

        /// <summary>
        /// Restores the current value to the maximum value.
        /// </summary>
        public virtual void Restore()
        {
            current = GetValue();
        }

        /// <summary>
        /// Reads data from a binary stream.
        /// </summary>
        public override bool Read(BinaryReader reader, int version)
        {
            current = reader.ReadSingle();
            return base.Read(reader, version);
        }

        /// <summary>
        /// Writes data to a binary stream.
        /// </summary>
        public override bool Write(BinaryWriter writer, int version)
        {
            writer.Write(current);
            return base.Write(writer, version);
        }
    }
}