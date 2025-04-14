using System;

namespace RpgSystem.Environment
{
    /// <summary>
    /// Handles day/night cycle calculations for the game world
    /// </summary>
    public static class DayNight
    {
        // Constants for day cycle
        public const int DayCycleHours = 3;
        public const int DayCycleMinutes = DayCycleHours * 60;
        public const int DayCycleSeconds = DayCycleMinutes * 60;
        public const int DayCycleMillis = DayCycleSeconds * 1000;
        
        // Time groups in the day cycle
        public enum DayCycleTimes
        {
            Midnight = 0,
            MorningNight,
            
            Dawn,
            Morning,
            
            Noon,
            
            Afternoon,
            Evening,
            Dusk,
            
            EveningNight,
            
            Count
        }
        
        // Size of each time group in milliseconds
        public const int DayCycleGroupTime = DayCycleMillis / (int)DayCycleTimes.Count;
        
        // String names for each time group
        private static readonly string[] TimeGroupNames = new string[]
        {
            "Midnight",
            "Night",
            "Dawn",
            "Morning",
            "Noon",
            "Afternoon",
            "Evening",
            "Dusk",
            "Night"
        };
        
        /// <summary>
        /// Gets the current time group based on the millisecond time
        /// </summary>
        /// <param name="milliTime">Current time in milliseconds</param>
        /// <returns>Time group index</returns>
        public static int GetCurrentTimeGroup(int milliTime)
        {
            // Normalize time to be within one day cycle
            while (milliTime > DayCycleMillis)
            {
                milliTime -= DayCycleMillis;
            }
            while (milliTime < 0)
            {
                milliTime += DayCycleMillis;
            }
            
            // Calculate which time group we're in
            return (milliTime / DayCycleGroupTime);
        }
        
        /// <summary>
        /// Gets the name of a time group
        /// </summary>
        /// <param name="group">The time group index</param>
        /// <returns>Name of the time group</returns>
        public static string GetTimeGroupName(int group)
        {
            if (group < 0 || group >= (int)DayCycleTimes.Count)
            {
                return "Unknown";
            }
            
            return TimeGroupNames[group];
        }
        
        /// <summary>
        /// Calculates time of day (0.0 to 1.0) based on millisecond time
        /// </summary>
        /// <param name="milliTime">Current time in milliseconds</param>
        /// <returns>Time of day as a fraction (0.0 = midnight, 0.5 = noon, 1.0 = midnight)</returns>
        public static float GetTimeOfDay(int milliTime)
        {
            // Normalize time to be within one day cycle
            while (milliTime > DayCycleMillis)
            {
                milliTime -= DayCycleMillis;
            }
            while (milliTime < 0)
            {
                milliTime += DayCycleMillis;
            }
            
            // Calculate percentage through the day
            return (float)milliTime / (float)DayCycleMillis;
        }
        
        /// <summary>
        /// Determines if it's currently nighttime
        /// </summary>
        /// <param name="milliTime">Current time in milliseconds</param>
        /// <returns>True if it's night, false if it's day</returns>
        public static bool IsNight(int milliTime)
        {
            int timeGroup = GetCurrentTimeGroup(milliTime);
            
            return timeGroup == (int)DayCycleTimes.Midnight ||
                   timeGroup == (int)DayCycleTimes.MorningNight ||
                   timeGroup == (int)DayCycleTimes.EveningNight ||
                   timeGroup == (int)DayCycleTimes.Dusk;
        }
    }
}