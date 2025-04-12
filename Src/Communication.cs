using System;
using Akarra.PlayerSystem;

namespace Akarra.Communication
{
    /// <summary>
    /// Packet types for Akarra protocol
    /// </summary>
    public enum AkarraPackets
    {
        Generic = 0,     // Never actually used, but should be here for reference
        
        // Server to client packets
        Server = 1000,   // Not a real message, just here as starter
        
        S_Welcome,       // This message tells the client that he has successfully logged in or
                         // created a new account
        S_Goodbye,       // Message that is sent before the client is disconnected, includes reason
        
        S_CharacterResult,    // This message is sent back to a client when the client wants to
                             // create a new character
        S_WorldResult,        // This message is sent when the player tries to enter the world
        
        // Message packets
        S_Echo,          // Small message that will appear in the player's chat box
        S_SystemNotice,  // A message that will appear as a flashing icon in the player's game view
        
        // In-game character packets
        S_ThisIsYou,      // First character packet the client will receive
        S_CharUpdate,     // Update packet sent periodically
        S_UpdateStats,    // Updates the player's character stats
        S_UpdateInventory, // Updates the player's inventory
        
        // Message of the day packets
        S_Motd256 = 1050, // A motd with less than 256 characters
        S_Motd512,
        S_Motd1024,
        S_Motd2048,
        
        // Admin packets
        SA_DownloadData = 1100, // Sends a spawn, monster or item data packet to the client
        
        // Client to server packets
        Client = 2000,    // Not a real message, just here as starter
        
        C_NewAccount,     // The client wants to create a new account
        C_Login,          // The client wants to login to the account
        
        // Client account administration packets
        C_NewCharacter,   // Message that is sent when the player wants to create his character
        C_DelCharacter,   // The client wants to delete one of his characters
        C_SetPassword,    // Is sent if the user wants to set a new password
        
        // Client packets for the real game
        C_JoinWorld,      // The user has selected a character and wants to join the world with it
        C_AreaVersions,   // Sends the version numbers for all areas in the character's active region
        
        // Action packets
        C_Move,           // Moves the character to a new position
        C_Enter,          // Enters a building, cave or any other entrance
        
        C_Command_Tiny,   // Utters a command (such as "/say")
        C_Command_Small,
        C_Command_Normal,
        C_Command_Big,
        C_Command_Huge,
        
        C_RequestEntity,  // Requests entity info based on a global id
        C_RequestSkill,   // If the client's skill data mismatches the server's version
        
        C_DefaultAction,  // "default action" packet, used for everything that only requires three ints
        C_ThreeAction,    // Same as "default action", but with an extra parameter
        
        C_RequestMonsterType,
        
        // Admin stuff
        CA_UploadData = 2100 // Uploads a dynamic spawn, item or monster to the server
    }
    
    /// <summary>
    /// Welcome codes for server messages
    /// </summary>
    public enum WelcomeCodes
    {
        Unknown = 0,
        LoggedIn,           // The user has successfully logged in to an existing account
        CreatedAccount,     // The user has created a new account
        CreatedCharacter,   // The user has created a new character
        DeletedCharacter,   // The user has deleted one of his characters
        ChangedPassword,    // The user has changed his password
        
        ActionFailed = 10   // The last action failed
    }
    
    /// <summary>
    /// Goodbye reasons for server messages
    /// </summary>
    public enum GoodbyeReasons
    {
        Unknown = 0,
        Normal,              // The user has disconnected by normal means
        IncorrectVersion,    // The user hasn't got the latest client exe
        
        // Account creation codes
        AC_AlreadyExists = 10, // The account name already exists
        AC_InvalidName,        // The user name is not acceptable
        AC_InvalidPassword,    // The password is not acceptable
        AC_InvalidEmail,       // The email is not acceptable
        AC_UnableToCreate,     // Unable to create the account, unknown reason
        
        // Account logging in codes
        AL_DoesntExist = 20,   // Can't find account name
        AL_InvalidName,
        AL_InvalidPassword,    // Password is incorrect
        AL_AlreadyLoggedIn,    // Can't login to an account that already is in use
        AL_Banned,             // The account has been banned
        
        // Other
        Kicked = 30            // The user has been kicked by the administration crew
    }
    
    /// <summary>
    /// Character result codes for server messages
    /// </summary>
    public enum CharacterResults
    {
        Unknown = 0,           // Count this message as a failed attempt
        
        // Success codes
        S_Success = 10,
        
        // Failure codes
        F_InvalidName = 20,    // The character name is not acceptable
        F_AlreadyExists,       // The character name already exists
        F_IsInvalid,           // The character information is invalid for some reason
        F_SlotIsTaken,         // There already is a character in that account slot
        F_UnableToCreate       // Unknown error
    }
    
    /// <summary>
    /// World result codes for server messages
    /// </summary>
    public enum WorldResults
    {
        Unknown = 0,
        
        S_Success = 10,
        
        F_EmptySlot = 20,      // There's no character in the selected slot for that account
        F_InvalidSlot,
        F_InvalidCharacter,    // Unable to load the selected character
        F_UnknownError         // Unknown error
    }
    
    /// <summary>
    /// Echo types for server messages
    /// </summary>
    public enum EchoTypes
    {
        Normal = 0,    // Normal message
        Tell,          // A 'tell' message from one player to another
        Fellowship,    // A 'fellowship' message to all fellowship members
        Guild,         // A message to all guild members
        System         // A system message
    }
    
    /// <summary>
    /// Chat message types
    /// </summary>
    public enum ChatMessageTypes
    {
        // Standard chat messages
        Say = 0,       // Global say. 'Khaile says, "Hello there"'
        Tell,          // Personal whispering. 'Khaile tells you, "Hello there"'
        TellOther,     // Whispering to other 'You told Khaile, "Hello there"'
        Guild,         // Guild chat. 'Khaile says, "Hello all!"'
        Fellowship,    // Fellowship chat. 'Khaile says, "Let's go!"'
        Action,        // An emote command. '* Khaile jumps up and down'
        
        Event,         // Global admin shout 'Khaile shouts, "Event will start soon!"'
        OOC,           // Global send. 'OOC: Khaile says, "HELLO?"'
        Trade,         // Global send. 'Trade: Khaile says, "HELLO?"'
        
        // System messages:
        Admin,         // System messages. '<ADMIN> Error: Khaile not found'
        Default,       // Default messages without prefix 'You don't see anything here'
        Combat
    }
    
    /// <summary>
    /// Chat info types
    /// </summary>
    public enum ChatInfoTypes
    {
        Say = 0,
        Tell,           // What others tell you
        Guild,          // Guild chat
        Fellowship,     // Fellowship chat
        Action,         // An emote command
        
        Event,          // Global send
        OOC,            // Global send
        Trade,          // Global send
        
        // System messages:
        Admin,          // System messages
        Combat,
        Error,          // Standard error messages (flood protection etc)
        Command,        // Command information (can be errors too)
        TempFeedback,   // Temporary feedback message ("Examining item...")
        
        // Other messages
        StatsChange,
        Experience,
        Death,
        Money,
        
        NumChatInfoTypes
    }
    
    /// <summary>
    /// Base packet for server welcome message
    /// </summary>
    public struct ServerWelcomePacket
    {
        public GenericPacket Header;
        public AccountFile Account;   // Remember to clear the password before you send this packet
        public int Code;              // Welcome code, depending on latest action
        public int Users;             // Number of connected users
    }
    
    /// <summary>
    /// Base packet for server goodbye message
    /// </summary>
    public struct ServerGoodbyePacket
    {
        public GenericPacket Header;
        public int Reason;
    }
    
    /// <summary>
    /// Base packet for character creation result
    /// </summary>
    public struct ServerCharacterResultPacket
    {
        public GenericPacket Header;
        public int Result;
    }
    
    /// <summary>
    /// Base packet for world join result
    /// </summary>
    public struct ServerWorldResultPacket
    {
        public GenericPacket Header;
        public int Result;
    }
    
    /// <summary>
    /// Base packet for echo messages
    /// </summary>
    public struct ServerEchoPacket
    {
        public GenericPacket Header;
        public int EchoType;
        public byte[] Message;  // 128 bytes
        
        public ServerEchoPacket(int echoType, string message)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.S_Echo,
                Size = (uint)(sizeof(uint) * 3 + 128) // Header (2 uints) + EchoType + Message
            };
            
            EchoType = echoType;
            Message = new byte[128];
            
            if (!string.IsNullOrEmpty(message))
            {
                byte[] messageBytes = System.Text.Encoding.UTF8.GetBytes(message);
                int length = Math.Min(messageBytes.Length, 128);
                Array.Copy(messageBytes, Message, length);
            }
        }
    }
    
    /// <summary>
    /// Base packet for system notice messages
    /// </summary>
    public struct ServerSystemNoticePacket
    {
        public GenericPacket Header;
        public byte[] Message;  // 512 bytes (128*4)
        
        public ServerSystemNoticePacket(string message)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.S_SystemNotice,
                Size = (uint)(sizeof(uint) * 2 + 512) // Header (2 uints) + Message
            };
            
            Message = new byte[512];
            
            if (!string.IsNullOrEmpty(message))
            {
                byte[] messageBytes = System.Text.Encoding.UTF8.GetBytes(message);
                int length = Math.Min(messageBytes.Length, 512);
                Array.Copy(messageBytes, Message, length);
            }
        }
    }
    
    /// <summary>
    /// Base packet for "this is you" message
    /// </summary>
    public struct ServerThisIsYouPacket
    {
        public GenericPacket Header;
        public int Id;               // Entity id number
        public byte AddFlags;        // Some flags (same as when another player is added)
        public byte Padding1;
        public byte Padding2;
        public byte GuildColor;
        public byte[] GuildTag;      // 4 bytes
        public MinimumCharacterInfo Stats;  // Character information
        
        public ServerThisIsYouPacket(int id, byte addFlags, byte guildColor, string guildTag, MinimumCharacterInfo stats)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.S_ThisIsYou,
                Size = 0 // Would be calculated based on actual struct size
            };
            
            Id = id;
            AddFlags = addFlags;
            Padding1 = 0;
            Padding2 = 0;
            GuildColor = guildColor;
            GuildTag = new byte[4];
            
            if (!string.IsNullOrEmpty(guildTag))
            {
                byte[] tagBytes = System.Text.Encoding.UTF8.GetBytes(guildTag);
                int length = Math.Min(tagBytes.Length, 4);
                Array.Copy(tagBytes, GuildTag, length);
            }
            
            Stats = stats ?? new MinimumCharacterInfo();
        }
    }
    
    /// <summary>
    /// Entity add flags for network packets
    /// </summary>
    public static class EntityAddFlags
    {
        public const byte Dead = 0x1;    // The entity is already dead
        public const byte Resting = 0x2; // The entity is currently resting
        public const byte Gifted = 0x4;  // The entity is gifted
        public const byte Shining = 0x8; // The entity is shining
        public const byte NPC = 0x10;    // This is a NPC, which cannot be killed (but talked to)
        public const byte AFK = 0x20;    // Used for players when they are AFK
        public const byte Darkness = 0x20; // Used for monsters if they cause darkness instead of brightness
        public const byte PK = 0x40;     // Entity is a player-killer
        public const byte War = 0x80;    // Entity is a member of a hostile clan
    }
    
    /// <summary>
    /// Base packet for character update messages
    /// </summary>
    public struct ServerCharacterUpdatePacket
    {
        public GenericPacket Header;
        public uint SystemTime;  // Server system time, required since packets might arrive in any order
        
        public ServerCharacterUpdatePacket(uint systemTime)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.S_CharUpdate,
                Size = (uint)(sizeof(uint) * 2 + sizeof(uint)) // Header (2 uints) + SystemTime
            };
            
            SystemTime = systemTime;
        }
    }
    
    /// <summary>
    /// Base packet for client new account messages
    /// </summary>
    public struct ClientNewAccountPacket
    {
        public GenericPacket Header;
        public int ClientVersion;    // The version of the client exe
        public byte[] Password;      // AccountConstants.PasswordLength bytes
        public byte[] Email;         // AccountConstants.EmailLength bytes
        
        public ClientNewAccountPacket(int clientVersion, string password, string email)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.C_NewAccount,
                Size = (uint)(sizeof(uint) * 2 + sizeof(int) + AccountConstants.PasswordLength + AccountConstants.EmailLength)
            };
            
            ClientVersion = clientVersion;
            Password = new byte[AccountConstants.PasswordLength];
            Email = new byte[AccountConstants.EmailLength];
            
            if (!string.IsNullOrEmpty(password))
            {
                byte[] passwordBytes = System.Text.Encoding.UTF8.GetBytes(password);
                int length = Math.Min(passwordBytes.Length, AccountConstants.PasswordLength);
                Array.Copy(passwordBytes, Password, length);
            }
            
            if (!string.IsNullOrEmpty(email))
            {
                byte[] emailBytes = System.Text.Encoding.UTF8.GetBytes(email);
                int length = Math.Min(emailBytes.Length, AccountConstants.EmailLength);
                Array.Copy(emailBytes, Email, length);
            }
        }
    }
    
    /// <summary>
    /// Base packet for client login messages
    /// </summary>
    public struct ClientLoginPacket
    {
        public GenericPacket Header;
        public int ClientVersion;    // The version of the client exe
        public byte[] Password;      // AccountConstants.PasswordLength bytes
        
        public ClientLoginPacket(int clientVersion, string password)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.C_Login,
                Size = (uint)(sizeof(uint) * 2 + sizeof(int) + AccountConstants.PasswordLength)
            };
            
            ClientVersion = clientVersion;
            Password = new byte[AccountConstants.PasswordLength];
            
            if (!string.IsNullOrEmpty(password))
            {
                byte[] passwordBytes = System.Text.Encoding.UTF8.GetBytes(password);
                int length = Math.Min(passwordBytes.Length, AccountConstants.PasswordLength);
                Array.Copy(passwordBytes, Password, length);
            }
        }
    }
    
    /// <summary>
    /// Base packet for client new character messages
    /// </summary>
    public struct ClientNewCharacterPacket
    {
        public GenericPacket Header;
        public byte[] Name;       // Character name (AccountConstants.NameLength bytes)
        public byte Slot;         // Player slot (there are 5 to choose from)
        public byte God;          // Shining or gifted?
        public byte Soul;         // White, blue, green, red or yellow?
        public byte Gender;       // Male or beautiful?
        
        // Appearance parameters
        public byte Pose;
        public byte Skin;         // This is used for both skin and eye color
        public byte Hair;
        public byte Style;
        
        public ClientNewCharacterPacket(string name, byte slot, byte god, byte soul, byte gender, 
                                         byte pose, byte skin, byte hair, byte style)
        {
            Header = new GenericPacket
            {
                Type = (uint)AkarraPackets.C_NewCharacter,
                Size = (uint)(sizeof(uint) * 2 + AccountConstants.NameLength + 8) // Header + name + 8 bytes
            };
            
            Name = new byte[AccountConstants.NameLength];
            if (!string.IsNullOrEmpty(name))
            {
                byte[] nameBytes = System.Text.Encoding.UTF8.GetBytes(name);
                int length = Math.Min(nameBytes.Length, AccountConstants.NameLength);
                Array.Copy(nameBytes, Name, length);
            }
            
            Slot = slot;
            God = god;
            Soul = soul;
            Gender = gender;
            Pose = pose;
            Skin = skin;
            Hair = hair;
            Style = style;
        }
    }
}