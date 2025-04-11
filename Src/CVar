using System;
using System.Globalization;

namespace Akarra.Basic
{
    /// <summary>
    /// A console-style variable that can be accessed as string, int, or float
    /// </summary>
    public class Var
    {
        // Fields
        private string _string;
        private float _value;
        
        // Constructors
        public Var()
        {
            _string = string.Empty;
            _value = 0.0f;
        }
        
        public Var(Var other)
        {
            _string = other._string;
            _value = other._value;
        }
        
        public Var(int value)
        {
            _string = value.ToString(CultureInfo.InvariantCulture);
            _value = value;
        }
        
        public Var(float value)
        {
            _string = value.ToString(CultureInfo.InvariantCulture);
            _value = value;
        }
        
        public Var(string value)
        {
            _string = value;
            float.TryParse(value, NumberStyles.Float, CultureInfo.InvariantCulture, out _value);
        }
        
        // Properties
        public int Int => (int)_value;
        public float Float => _value;
        public string String => _string;
        
        // Operators
        public static implicit operator int(Var var)
        {
            return var.Int;
        }
        
        public static implicit operator float(Var var)
        {
            return var.Float;
        }
        
        public static implicit operator string(Var var)
        {
            return var.String;
        }
        
        // Assignment operators
        public static Var operator =(Var left, Var right)
        {
            left._string = right._string;
            left._value = right._value;
            return left;
        }
        
        public static Var operator =(Var left, int right)
        {
            left._string = right.ToString(CultureInfo.InvariantCulture);
            left._value = right;
            return left;
        }
        
        public static Var operator =(Var left, float right)
        {
            left._string = right.ToString(CultureInfo.InvariantCulture);
            left._value = right;
            return left;
        }
        
        public static Var operator =(Var left, string right)
        {
            left._string = right;
            float.TryParse(right, NumberStyles.Float, CultureInfo.InvariantCulture, out left._value);
            return left;
        }
    }
    
    /// <summary>
    /// Settings structure for console variables
    /// </summary>
    public class VarSettings
    {
        public Var Variable { get; set; }
        public string Name { get; set; }
        public string DefaultValue { get; set; }
        public int Flags { get; set; }
        
        public VarSettings()
        {
            Variable = null;
            Name = null;
            DefaultValue = null;
            Flags = 0;
        }
    }
    
    /// <summary>
    /// Variable flags
    /// </summary>
    public static class VarFlags
    {
        public const int ReadOnly = 0x1;    // Cannot be changed by the user
        public const int System = 0x2;      // Requires a system/game restart
        public const int Cheat = 0x4;       // Cheat protected, may only be changed if cheats are allowed
        public const int NoSave = 0x8;      // This variable will never be output to an external text file
    }
}