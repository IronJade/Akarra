namespace RpgSystem
{
    /// <summary>
    /// Version constants for player character files
    /// </summary>
    public static class PlayerFileVersions
    {
        public const int Original = 0;
        public const int V_020505 = 1;     // Added stamina and hunger to BaseStats
        public const int V_020530 = 2;     // Added damage reduction and level 50 time
        public const int V_020611 = 3;     // Added first version of CharacterInventory
        public const int V_020622 = 4;     // Added items to the CharacterInventory
        public const int V_020629 = 5;     // Added respawn point position
        public const int V_020810 = 6;     // Added stackable items
        public const int V_020818 = 7;     // Added new skill system
        public const int V_020908 = 8;     // Converted coin system and decreased max inventory capacity
        public const int V_020913 = 9;     // Added CharacterQuests
        public const int V_021002 = 10;    // Added new character graphics with poses and hair styles
        public const int V_021003 = 11;    // Added eye colors
        public const int V_021009 = 12;    // Added account name to character file
        public const int V_021022 = 13;    // Added "runeffects" list (for character effects)
        public const int V_021102 = 14;    // Added a slot for mounts in inventory
        public const int V_021208 = 15;    // Added guild info
        public const int V_030102 = 16;    // Added mount coordinates in item instances
        public const int V_030118 = 17;    // Added mute seconds
        public const int V_030204 = 18;    // Changed the skill system
        public const int V_030219 = 19;    // Added custom names to item instances
        public const int V_030301 = 20;    // Added player-killer victim list and IDs for effects
        public const int V_030405 = 21;    // Added item storage to inventory
        public const int V_030506 = 22;    // Added last guild message time and reset attribute points
        public const int V_030508 = 23;    // Skills now have 10 gems per row instead of 5
        
        public const int Current = V_030508;
    }
}