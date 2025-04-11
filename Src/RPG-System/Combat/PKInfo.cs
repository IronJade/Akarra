using System;
using System.Collections.Generic;

namespace RpgSystem
{
    /// <summary>
    /// Contains definitions for player-killing related structures and constants
    /// </summary>
    public static class PKInfo
    {
        /// <summary>
        /// Default expiration time for PK status (10 hours in seconds)
        /// </summary>
        public const int ExpireTime = 10 * 60 * 60;
        
        /// <summary>
        /// Protection time for newly respawned players (5 minutes in seconds)
        /// </summary>
        public const int ProtectionTime = 5 * 60;
        
        /// <summary>
        /// Structure to hold information about a player's PK victims
        /// </summary>
        public class VictimName
        {
            /// <summary>
            /// Name of the victim
            /// </summary>
            public string Name { get; set; }
            
            /// <summary>
            /// Time (in seconds) until the PK flag expires
            /// </summary>
            public int Seconds { get; set; }
            
            public VictimName(string name, int seconds = ExpireTime)
            {
                Name = name;
                Seconds = seconds;
            }
        }
        
        /// <summary>
        /// A list of PK victims
        /// </summary>
        public class VictimList : List<VictimName>
        {
            /// <summary>
            /// Adds a new victim to the list or refreshes an existing one
            /// </summary>
            public void AddVictim(string name)
            {
                // Check if this victim already exists
                foreach (var victim in this)
                {
                    if (string.Equals(victim.Name, name, StringComparison.OrdinalIgnoreCase))
                    {
                        // Reset the timer if victim already exists
                        victim.Seconds = ExpireTime;
                        return;
                    }
                }
                
                // Add as a new victim
                Add(new VictimName(name));
            }
            
            /// <summary>
            /// Clears a victim from the list
            /// </summary>
            public bool ClearVictim(string name)
            {
                for (int i = 0; i < Count; i++)
                {
                    if (string.Equals(this[i].Name, name, StringComparison.OrdinalIgnoreCase))
                    {
                        RemoveAt(i);
                        return true;
                    }
                }
                
                return false;
            }
            
            /// <summary>
            /// Checks if a player is on the victim list
            /// </summary>
            public bool HasVictim(string name)
            {
                foreach (var victim in this)
                {
                    if (string.Equals(victim.Name, name, StringComparison.OrdinalIgnoreCase))
                    {
                        return true;
                    }
                }
                
                return false;
            }
            
            /// <summary>
            /// Updates the expiration times for all victims
            /// </summary>
            public void UpdateExpirations(int secondsElapsed)
            {
                for (int i = Count - 1; i >= 0; i--)
                {
                    this[i].Seconds -= secondsElapsed;
                    
                    if (this[i].Seconds <= 0)
                    {
                        RemoveAt(i);
                    }
                }
            }
        }
    }
}