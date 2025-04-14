using System;
using System.IO;

namespace RpgSystem.Attributes
{
    /// <summary>
    /// Base attribute class that forms the foundation of the character attribute system.
    /// Attributes have a base value, bonus value, and percentage bonus.
    /// </summary>
    public class CAttribute
    {
        // Fields
        protected float _baseValue;
        protected float _bonus;
        protected float _percentBonus;

        /// <summary>
        /// Creates a new attribute with default values (0)
        /// </summary>
        public CAttribute()
        {
            _baseValue = 0;
            _bonus = 0;
            _percentBonus = 0;
        }

        /// <summary>
        /// Creates a new attribute with a specified base value
        /// </summary>
        public CAttribute(float baseValue)
        {
            _baseValue = baseValue;
            _bonus = 0;
            _percentBonus = 0;
        }

        /// <summary>
        /// Sets the base value of the attribute
        /// </summary>
        public virtual void SetBase(float value)
        {
            _baseValue = value;
        }

        /// <summary>
        /// Resets all bonuses to zero
        /// </summary>
        public virtual void ResetBonus()
        {
            _bonus = 0;
            _percentBonus = 0;
        }

        /// <summary>
        /// Adds a flat bonus to the attribute
        /// </summary>
        public virtual void AddBonus(float value)
        {
            _bonus += value;
        }

        /// <summary>
        /// Adds a percentage bonus to the attribute (0.1 = 10%)
        /// </summary>
        public virtual void AddPercentBonus(float value)
        {
            _percentBonus += value;
        }

        /// <summary>
        /// Gets the base value of the attribute
        /// </summary>
        public virtual float GetBase()
        {
            return _baseValue;
        }

        /// <summary>
        /// Gets the flat bonus value of the attribute
        /// </summary>
        public virtual float GetBonus()
        {
            return _bonus;
        }

        /// <summary>
        /// Gets the percentage bonus of the attribute
        /// </summary>
        public virtual float GetPercentBonus()
        {
            return _percentBonus;
        }

        /// <summary>
        /// Gets the total value of the attribute (base + bonus) * (1 + percentBonus)
        /// </summary>
        public virtual float GetValue()
        {
            float value = (_baseValue + _bonus) * (1.0f + _percentBonus);
            return value < 0 ? 0 : value;
        }

        /// <summary>
        /// Reads attribute data from a binary stream
        /// </summary>
        public virtual bool Read(BinaryReader reader, int version)
        {
            try
            {
                _baseValue = reader.ReadSingle();
                _bonus = reader.ReadSingle();
                _percentBonus = reader.ReadSingle();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes attribute data to a binary stream
        /// </summary>
        public virtual bool Write(BinaryWriter writer, int version)
        {
            try
            {
                writer.Write(_baseValue);
                writer.Write(_bonus);
                writer.Write(_percentBonus);
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
    }

    /// <summary>
    /// Extended attribute class that has a current value that can be different from its maximum.
    /// Used for health, mana, stamina, etc.
    /// </summary>
    public class CVariableAttribute : CAttribute
    {
        // Fields
        protected float _current;

        /// <summary>
        /// Creates a new variable attribute with default values
        /// </summary>
        public CVariableAttribute() : base()
        {
            _current = 0;
        }

        /// <summary>
        /// Creates a new variable attribute with a specified base value
        /// </summary>
        public CVariableAttribute(float baseValue) : base(baseValue)
        {
            _current = GetValue();
        }

        /// <summary>
        /// Sets the base value and updates current value if needed
        /// </summary>
        public override void SetBase(float value)
        {
            float oldMax = GetValue();
            base.SetBase(value);
            float newMax = GetValue();

            // If max value increased, increase current value by the same amount
            if (newMax > oldMax)
            {
                _current += (newMax - oldMax);
            }
            // If max value decreased and current is now greater than max, cap it
            else if (_current > newMax)
            {
                _current = newMax;
            }
        }

        /// <summary>
        /// Resets bonuses and adjusts current value if necessary
        /// </summary>
        public override void ResetBonus()
        {
            float oldMax = GetValue();
            base.ResetBonus();
            float newMax = GetValue();

            // Adjust current value proportionally to change in max value
            if (oldMax > 0)
            {
                _current = (_current / oldMax) * newMax;
            }
            else
            {
                _current = newMax;
            }
        }

        /// <summary>
        /// Adds a bonus and adjusts current value proportionally
        /// </summary>
        public override void AddBonus(float value)
        {
            float oldMax = GetValue();
            base.AddBonus(value);
            float newMax = GetValue();
            float diff = newMax - oldMax;

            // If max value increased, increase current by the same amount
            if (diff > 0)
            {
                _current += diff;
            }
            // If max value decreased and current is now greater than max, cap it
            else if (_current > newMax)
            {
                _current = newMax;
            }
        }

        /// <summary>
        /// Adds a percentage bonus and adjusts current value proportionally
        /// </summary>
        public override void AddPercentBonus(float value)
        {
            float oldMax = GetValue();
            base.AddPercentBonus(value);
            float newMax = GetValue();
            float diff = newMax - oldMax;

            // If max value increased, increase current by the same amount
            if (diff > 0)
            {
                _current += diff;
            }
            // If max value decreased and current is now greater than max, cap it
            else if (_current > newMax)
            {
                _current = newMax;
            }
        }

        /// <summary>
        /// Gets the current value
        /// </summary>
        public virtual float GetCurrent()
        {
            return _current;
        }

        /// <summary>
        /// Sets the current value (capped at maximum)
        /// </summary>
        public virtual void SetCurrent(float value)
        {
            float max = GetValue();
            _current = value > max ? max : value;
        }

        /// <summary>
        /// Adds to the current value (capped at maximum)
        /// </summary>
        public virtual float AddToCurrent(float value)
        {
            _current += value;
            float max = GetValue();

            if (_current > max)
            {
                _current = max;
            }
            else if (_current < 0)
            {
                _current = 0;
            }

            return _current;
        }

        /// <summary>
        /// Restores the current value to maximum
        /// </summary>
        public virtual void Restore()
        {
            _current = GetValue();
        }

        /// <summary>
        /// Gets the percentage of current to maximum value (0.0 to 1.0)
        /// </summary>
        public virtual float GetPercentage()
        {
            float max = GetValue();
            return max > 0 ? _current / max : 0;
        }

        /// <summary>
        /// Reads variable attribute data from a binary stream
        /// </summary>
        public override bool Read(BinaryReader reader, int version)
        {
            try
            {
                _current = reader.ReadSingle();
                return base.Read(reader, version);
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes variable attribute data to a binary stream
        /// </summary>
        public override bool Write(BinaryWriter writer, int version)
        {
            try
            {
                writer.Write(_current);
                return base.Write(writer, version);
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}