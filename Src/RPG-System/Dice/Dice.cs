using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace RpgSystem.Dice
{
    /// <summary>
    /// Represents a single dice roll of N dice with S sides.
    /// </summary>
    public class DiceRoll
    {
        private static readonly Random random = new Random();
        private int numberOfDice;
        private int sides;

        /// <summary>
        /// Creates a new dice roll with default values.
        /// </summary>
        public DiceRoll()
        {
            numberOfDice = 0;
            sides = 0;
        }

        /// <summary>
        /// Creates a new dice roll with specific number of dice and sides.
        /// </summary>
        public DiceRoll(int numberOfDice, int sides)
        {
            this.numberOfDice = numberOfDice;
            this.sides = sides;
        }

        /// <summary>
        /// Rolls the dice and returns the result.
        /// </summary>
        /// <returns>Value from [numberOfDice] to [numberOfDice * sides]</returns>
        public int Roll()
        {
            if (sides <= 1)
            {
                return numberOfDice;
            }

            int count = 0;
            for (int i = 0; i < numberOfDice; i++)
            {
                count += random.Next(1, sides + 1);
            }
            return count;
        }

        /// <summary>
        /// Gets the maximum possible value from the dice.
        /// </summary>
        public int GetMax()
        {
            if (sides <= 1)
            {
                return numberOfDice;
            }
            return numberOfDice * sides;
        }

        /// <summary>
        /// Gets the minimum possible value from the dice.
        /// </summary>
        public int GetMin()
        {
            return numberOfDice;
        }

        /// <summary>
        /// Sets the number of dice and sides.
        /// </summary>
        public void Set(int numberOfDice, int sides)
        {
            this.numberOfDice = numberOfDice;
            this.sides = sides;
        }
    }

    /// <summary>
    /// Represents a collection of dice rolls, which can be parsed from a string like "2d6+1d4+2".
    /// </summary>
    public class DiceList
    {
        private static readonly char DiceIdentifier = 'd';
        private List<DiceRoll> diceRolls = new List<DiceRoll>();

        /// <summary>
        /// Rolls all dice in the list and returns the sum.
        /// </summary>
        public int Roll()
        {
            int total = 0;
            foreach (var dice in diceRolls)
            {
                total += dice.Roll();
            }
            return total;
        }

        /// <summary>
        /// Gets the maximum possible roll value.
        /// </summary>
        public int RollMax()
        {
            int total = 0;
            foreach (var dice in diceRolls)
            {
                total += dice.GetMax();
            }
            return total;
        }

        /// <summary>
        /// Gets the minimum possible roll value.
        /// </summary>
        public int RollMin()
        {
            int total = 0;
            foreach (var dice in diceRolls)
            {
                total += dice.GetMin();
            }
            return total;
        }

        /// <summary>
        /// Creates a dice list from a string like "2d6+1d4+2".
        /// </summary>
        public void CreateList(string diceString)
        {
            // Clear existing dice
            diceRolls.Clear();

            if (string.IsNullOrEmpty(diceString))
                return;

            // Using a more modern regex approach for dice parsing
            string pattern = @"(\d+)(d(\d+))?";
            var matches = Regex.Matches(diceString, pattern);

            foreach (Match match in matches)
            {
                int numberOfDice = int.Parse(match.Groups[1].Value);
                
                // If this is just a constant (no 'd'), sides = 1
                int sides = 1;
                if (match.Groups[2].Success)
                {
                    sides = int.Parse(match.Groups[3].Value);
                }

                diceRolls.Add(new DiceRoll(numberOfDice, sides));
            }
        }
    }
}