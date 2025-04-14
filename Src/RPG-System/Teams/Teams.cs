using System;

namespace RpgSystem.Teams
{
    /// <summary>
    /// Teams and team management system for the game
    /// A team is an integer that consists of 3 blocks:
    /// 000000FF = main team (0xFF mask)
    /// 0000FF00 = religion team (0xFF00 mask)
    /// FFFF0000 = group team (0xFFFF0000 mask)
    /// </summary>
    public static class TeamSystem
    {
        // Bit shift and mask constants for team components
        public const int MainTeamShift = 0;
        public const int MainTeamMask = 0xFF;
        public const int ReligionTeamShift = 8;
        public const int ReligionTeamMask = 0xFF00;
        public const int GroupTeamShift = 16;
        public const int GroupTeamMask = 0xFFFF0000;
        
        /// <summary>
        /// Main team types
        /// </summary>
        public enum MainTeams
        {
            Invalid = 0,
            Players = 1,
            Monsters,
            NPCs,
            
            Count
        }
        
        /// <summary>
        /// Religion-based team types
        /// </summary>
        public enum ReligionTeams
        {
            Invalid = 0,
            Gifted = 1,
            Shining,
            Faithless,
            
            Count
        }
        
        /// <summary>
        /// Special group teams (used with MainTeams)
        /// </summary>
        public enum GroupTeams
        {
            None = 0,      // No special conditions (default)
            
            // Monster special groups
            PlayerSummoned, // Summoned by a player, uses player rules
            
            // Monster special teams (starting at 100)
            Daerak = 100,  // Daerak darklings are enemies towards arags
            Arag,          // Arag darklings are enemies towards daerak
        }
        
        /// <summary>
        /// Extracts the main team component from a team value
        /// </summary>
        public static int GetMainTeam(int team)
        {
            return (team & MainTeamMask) >> MainTeamShift;
        }
        
        /// <summary>
        /// Extracts the religion team component from a team value
        /// </summary>
        public static int GetReligionTeam(int team)
        {
            return (team & ReligionTeamMask) >> ReligionTeamShift;
        }
        
        /// <summary>
        /// Extracts the group team component from a team value
        /// </summary>
        public static int GetGroupTeam(int team)
        {
            return (team & GroupTeamMask) >> GroupTeamShift;
        }
        
        /// <summary>
        /// Creates a complete team value from components
        /// </summary>
        public static int CreateTeam(int mainTeam, int religionTeam, int groupTeam)
        {
            return ((mainTeam & MainTeamMask) << MainTeamShift) |
                   ((religionTeam & 0xFF) << ReligionTeamShift) |
                   ((groupTeam & 0xFFFF) << GroupTeamShift);
        }
        
        /// <summary>
        /// Checks if two teams are hostile to each other
        /// </summary>
        public static bool AreHostile(int team1, int team2)
        {
            // Extract team components
            int main1 = GetMainTeam(team1);
            int main2 = GetMainTeam(team2);
            int religion1 = GetReligionTeam(team1);
            int religion2 = GetReligionTeam(team2);
            int group1 = GetGroupTeam(team1);
            int group2 = GetGroupTeam(team2);
            
            // Player-summoned monsters are always friendly to players
            if (main1 == (int)MainTeams.Players && main2 == (int)MainTeams.Monsters && 
                group2 == (int)GroupTeams.PlayerSummoned)
            {
                return false;
            }
            
            if (main2 == (int)MainTeams.Players && main1 == (int)MainTeams.Monsters && 
                group1 == (int)GroupTeams.PlayerSummoned)
            {
                return false;
            }
            
            // NPCs are never hostile
            if (main1 == (int)MainTeams.NPCs || main2 == (int)MainTeams.NPCs)
            {
                return false;
            }
            
            // Players and monsters are always hostile to each other
            if ((main1 == (int)MainTeams.Players && main2 == (int)MainTeams.Monsters) ||
                (main2 == (int)MainTeams.Players && main1 == (int)MainTeams.Monsters))
            {
                return true;
            }
            
            // Special group hostilities
            if (main1 == (int)MainTeams.Monsters && main2 == (int)MainTeams.Monsters)
            {
                // Daerak and Arag are enemies
                if ((group1 == (int)GroupTeams.Daerak && group2 == (int)GroupTeams.Arag) ||
                    (group2 == (int)GroupTeams.Daerak && group1 == (int)GroupTeams.Arag))
                {
                    return true;
                }
            }
            
            // Players with different religions might be hostile
            if (main1 == (int)MainTeams.Players && main2 == (int)MainTeams.Players)
            {
                // Shining and Gifted are hostile to each other
                if ((religion1 == (int)ReligionTeams.Shining && religion2 == (int)ReligionTeams.Gifted) ||
                    (religion2 == (int)ReligionTeams.Shining && religion1 == (int)ReligionTeams.Gifted))
                {
                    return true;
                }
            }
            
            // Default: not hostile
            return false;
        }
    }
}