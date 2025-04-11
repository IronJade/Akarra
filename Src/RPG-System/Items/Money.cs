using System;
using System.Text;

namespace RpgSystem.Items
{
    /// <summary>
    /// Currency types in the game
    /// </summary>
    public enum MoneyType
    {
        Normal = 0,  // "Normal" human currency
        Menit,       // Menit/gifted currency
        Iwid,        // Iwid/shining currency
        
        Count
    }
    
    /// <summary>
    /// Constants for the money system
    /// </summary>
    public static class Money
    {
        /// <summary>
        /// Constants for the money system
        /// </summary>
        public static class Constants
        {
            public const int MoneyTypeCount = 3;  // Number of currency types
        }
    }
    
    /// <summary>
    /// Static helper for money calculations
    /// </summary>
    public static class MoneyCalculator
    {
        private static readonly string[] MoneyNames = new string[]
        {
            "Coins",
            "Drengel",
            "Quats"
        };
        
        /// <summary>
        /// Calculates the maximum amount of currency a character can carry
        /// </summary>
        public static int GetMaxCarry(int level, int moneyType)
        {
            int value = 0;
            
            if (moneyType == (int)MoneyType.Normal)
            {
                value = level * level * 5;
            }
            else
            {
                value = level * level * 2;
            }
            
            return Math.Min(value, 50000);
        }
        
        /// <summary>
        /// Gets the name of a currency type
        /// </summary>
        public static string GetMoneyName(int moneyType)
        {
            if (moneyType < 0 || moneyType >= (int)MoneyType.Count)
            {
                return "Coins";
            }
            
            return MoneyNames[moneyType];
        }
        
        /// <summary>
        /// Formats an amount of money as a display string
        /// </summary>
        public static string GetMoneyString(int amount, int type)
        {
            if (type == (int)MoneyType.Menit)
            {
                return $"{amount} Drengel";
            }
            else if (type == (int)MoneyType.Iwid)
            {
                if (amount == 1)
                {
                    return $"{amount} Quat";
                }
                else
                {
                    return $"{amount} Quats";
                }
            }
            
            // "Normal" type is more complicated (gold, silver, copper)
            if (amount == 0)
            {
                return "0 Coins";
            }
            
            string result = "";
            
            // Calculate denominations
            int gold = amount / 1000;
            int silver = (amount % 1000) / 100;
            int copper = (amount % 1000) % 100;
            
            // Format gold
            if (gold > 0)
            {
                result += $"{gold} Gold";
                
                if (silver < 1 && copper < 1)
                {
                    if (gold == 1)
                    {
                        result += " Coin";
                    }
                    else
                    {
                        result += " Coins";
                    }
                    return result;
                }
            }
            
            // Format silver
            if (silver > 0)
            {
                if (gold > 0 && copper > 0)
                {
                    result += ", ";
                }
                else if (gold > 0)
                {
                    result += " & ";
                }
                
                result += $"{silver} Silver";
                
                if (copper < 1)
                {
                    if (silver == 1)
                    {
                        result += " Coin";
                    }
                    else
                    {
                        result += " Coins";
                    }
                    return result;
                }
            }
            
            // Format copper
            if (copper > 0)
            {
                if (gold > 0 || silver > 0)
                {
                    result += " & ";
                }
                
                result += $"{copper} Copper Coin";
                
                if (copper != 1)
                {
                    result += "s";
                }
            }
            
            return result;
        }
        
        /// <summary>
        /// Gets a short-form money value representation
        /// For normal money: "5gc, 3sc, 2cc" format
        /// For other types: "5dc" or "10qc" format
        /// </summary>
        public static string GetMoneyValue(int amount, int type)
        {
            if (type == (int)MoneyType.Menit)
            {
                return $"{amount}dc";
            }
            
            if (type == (int)MoneyType.Iwid)
            {
                return $"{amount}qc";
            }
            
            // Normal currency - convert to gc, sc, cc
            int gold = amount / 1000;
            int silver = (amount % 1000) / 100;
            int copper = (amount % 1000) % 100;
            
            var sb = new StringBuilder();
            
            if (gold > 0)
            {
                sb.Append($"{gold}gc");
            }
            
            if (silver > 0)
            {
                if (gold > 0)
                {
                    sb.Append(", ");
                }
                sb.Append($"{silver}sc");
            }
            
            if (copper > 0)
            {
                if (gold > 0 || silver > 0)
                {
                    sb.Append(", ");
                }
                sb.Append($"{copper}cc");
            }
            
            return sb.ToString();
        }
    }
}