using System;
using System.Collections.Generic;
using Akarra.FileSystem;

namespace Akarra.PlayerSystem
{
    /// <summary>
    /// Account file versions
    /// </summary>
    public enum AccountFileVersions
    {
        Original = 0,
        ACCFV_021201,    // Added creation date
        ACCFV_030408,    // Added friend and ignore lists
        ACCFV_030515,    // Added last use hostname
        
        CurrentVersion
    }
    
    /// <summary>
    /// Constants for account handling
    /// </summary>
    public static class AccountConstants
    {
        public const int NameLength = 16;        // Maximum account name length, names are stored as filenames
        public const int PasswordLength = 16;    // Max password length
        public const int EmailLength = 64;       // Max email address length
        public const int Characters = 5;         // Each account can create up to this value of characters
        
        public const string Folder = "accounts/";
        public const string Extension = ".aacc";
        public const string DeletedExtension = ".~acc";
        public const string Folder2 = "accounts";  // For FileSystem.CountFiles() and similar
        public const string SearchPattern = "*.aacc"; // For FileSystem.CountFiles() and similar
        
        // Account flags
        public const uint Banned = 0x1;     // The account has been banned
        public const uint Master = 0x2;     // The account owner has "master rights"
        public const uint PasswordLocked = 0x4; // The account may not change password
    }
    
    /// <summary>
    /// Represents an account name for friend and ignore lists
    /// </summary>
    public class AccountName
    {
        public string Name { get; set; }
        
        public AccountName()
        {
            Name = string.Empty;
        }
        
        public AccountName(string name)
        {
            Name = name ?? string.Empty;
        }
    }
    
    /// <summary>
    /// This structure is used to store a client's account information to file
    /// </summary>
    public class AccountFile
    {
        // General information
        public string Name { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }
        public uint Flags { get; set; }
        
        public uint LastUseDate { get; set; }
        
        // The account's characters
        public string[] Characters { get; set; }
        
        // Statistics
        public uint Logins { get; set; }     // Number of times the user has logged in
        public uint Seconds { get; set; }    // Time, in seconds, the user has been logged in
        
        public AccountFile()
        {
            Name = string.Empty;
            Password = string.Empty;
            Email = string.Empty;
            Flags = 0;
            LastUseDate = 0;
            Characters = new string[AccountConstants.Characters];
            Logins = 0;
            Seconds = 0;
            
            for (int i = 0; i < AccountConstants.Characters; i++)
            {
                Characters[i] = string.Empty;
            }
        }
    }
    
    /// <summary>
    /// This structure holds information about an account when the account's user is logged on
    /// </summary>
    public class Account
    {
        public AccountFile FileInfo { get; set; }
        public bool InUse { get; set; }
        
        public List<AccountName> Friends { get; set; }
        public List<AccountName> Ignores { get; set; }
        public string LastUseHostname { get; set; }
        
        public Account()
        {
            InUse = false;
            Clear();
        }
        
        ~Account()
        {
            RemoveAll();
        }
        
        public void RemoveAll()
        {
            Friends.Clear();
            Ignores.Clear();
            Clear();
        }
        
        public bool Open(FileStream file)
        {
            RemoveAll();
            
            // Read the file version
            int version = file.ReadInt();
            
            if (version > (int)AccountFileVersions.ACCFV_021201)
            {
                byte[] nameBuffer = new byte[AccountConstants.NameLength];
                file.Read(nameBuffer, AccountConstants.NameLength);
                FileInfo.Name = System.Text.Encoding.UTF8.GetString(nameBuffer).TrimEnd('\0');
                
                byte[] passwordBuffer = new byte[AccountConstants.PasswordLength];
                file.Read(passwordBuffer, AccountConstants.PasswordLength);
                FileInfo.Password = System.Text.Encoding.UTF8.GetString(passwordBuffer).TrimEnd('\0');
                
                byte[] emailBuffer = new byte[AccountConstants.EmailLength];
                file.Read(emailBuffer, AccountConstants.EmailLength);
                FileInfo.Email = System.Text.Encoding.UTF8.GetString(emailBuffer).TrimEnd('\0');
                
                file.Read(ref FileInfo.Flags, 1);
                file.Read(ref FileInfo.LastUseDate, 1);
                
                for (int i = 0; i < AccountConstants.Characters; i++)
                {
                    byte[] charBuffer = new byte[Character.NameLength];
                    file.Read(charBuffer, Character.NameLength);
                    FileInfo.Characters[i] = System.Text.Encoding.UTF8.GetString(charBuffer).TrimEnd('\0');
                }
                
                file.Read(ref FileInfo.Logins, 1);
                file.Read(ref FileInfo.Seconds, 1);
                
                if (version > (int)AccountFileVersions.ACCFV_030408)
                {
                    // Read friends list
                    int count = file.ReadInt();
                    for (int i = 0; i < count; i++)
                    {
                        AccountName name = new AccountName();
                        byte[] nameBuffer2 = new byte[AccountConstants.NameLength];
                        file.Read(nameBuffer2, AccountConstants.NameLength);
                        name.Name = System.Text.Encoding.UTF8.GetString(nameBuffer2).TrimEnd('\0');
                        Friends.Add(name);
                    }
                    
                    // Read ignore list
                    count = file.ReadInt();
                    for (int i = 0; i < count; i++)
                    {
                        AccountName name = new AccountName();
                        byte[] nameBuffer2 = new byte[AccountConstants.NameLength];
                        file.Read(nameBuffer2, AccountConstants.NameLength);
                        name.Name = System.Text.Encoding.UTF8.GetString(nameBuffer2).TrimEnd('\0');
                        Ignores.Add(name);
                    }
                }
                
                if (version > (int)AccountFileVersions.ACCFV_030515)
                {
                    LastUseHostname = FileSystem.ReadStringWithLength(file);
                }
            }
            else
            {
                byte[] nameBuffer = new byte[AccountConstants.NameLength];
                file.Read(nameBuffer, AccountConstants.NameLength);
                FileInfo.Name = System.Text.Encoding.UTF8.GetString(nameBuffer).TrimEnd('\0');
                
                byte[] passwordBuffer = new byte[AccountConstants.PasswordLength];
                file.Read(passwordBuffer, AccountConstants.PasswordLength);
                FileInfo.Password = System.Text.Encoding.UTF8.GetString(passwordBuffer).TrimEnd('\0');
                
                byte[] emailBuffer = new byte[AccountConstants.EmailLength];
                file.Read(emailBuffer, AccountConstants.EmailLength);
                FileInfo.Email = System.Text.Encoding.UTF8.GetString(emailBuffer).TrimEnd('\0');
                
                file.Read(ref FileInfo.Flags, 1);
                
                for (int i = 0; i < AccountConstants.Characters; i++)
                {
                    byte[] charBuffer = new byte[Character.NameLength];
                    file.Read(charBuffer, Character.NameLength);
                    FileInfo.Characters[i] = System.Text.Encoding.UTF8.GetString(charBuffer).TrimEnd('\0');
                }
                
                file.Read(ref FileInfo.Logins, 1);
                file.Read(ref FileInfo.Seconds, 1);
                
                FileInfo.LastUseDate = (uint)DateTimeOffset.UtcNow.ToUnixTimeSeconds();
            }
            
            return true;
        }
        
        public bool Write(FileStream file)
        {
            int version = (int)AccountFileVersions.CurrentVersion;
            file.Write(version);
            
            FileInfo.LastUseDate = (uint)DateTimeOffset.UtcNow.ToUnixTimeSeconds();
            
            byte[] nameBytes = new byte[AccountConstants.NameLength];
            System.Text.Encoding.UTF8.GetBytes(FileInfo.Name).CopyTo(nameBytes, 0);
            file.Write(nameBytes, nameBytes.Length);
            
            byte[] passwordBytes = new byte[AccountConstants.PasswordLength];
            System.Text.Encoding.UTF8.GetBytes(FileInfo.Password).CopyTo(passwordBytes, 0);
            file.Write(passwordBytes, passwordBytes.Length);
            
            byte[] emailBytes = new byte[AccountConstants.EmailLength];
            System.Text.Encoding.UTF8.GetBytes(FileInfo.Email).CopyTo(emailBytes, 0);
            file.Write(emailBytes, emailBytes.Length);
            
            file.Write(FileInfo.Flags);
            file.Write(FileInfo.LastUseDate);
            
            for (int i = 0; i < AccountConstants.Characters; i++)
            {
                byte[] charBytes = new byte[Character.NameLength];
                System.Text.Encoding.UTF8.GetBytes(FileInfo.Characters[i]).CopyTo(charBytes, 0);
                file.Write(charBytes, charBytes.Length);
            }
            
            file.Write(FileInfo.Logins);
            file.Write(FileInfo.Seconds);
            
            // Write friends list
            file.Write(Friends.Count);
            foreach (AccountName name in Friends)
            {
                byte[] nameBytes2 = new byte[AccountConstants.NameLength];
                System.Text.Encoding.UTF8.GetBytes(name.Name).CopyTo(nameBytes2, 0);
                file.Write(nameBytes2, nameBytes2.Length);
            }
            
            // Write ignore list
            file.Write(Ignores.Count);
            foreach (AccountName name in Ignores)
            {
                byte[] nameBytes2 = new byte[AccountConstants.NameLength];
                System.Text.Encoding.UTF8.GetBytes(name.Name).CopyTo(nameBytes2, 0);
                file.Write(nameBytes2, nameBytes2.Length);
            }
            
            // Write hostname
            FileSystem.WriteStringWithLength(file, LastUseHostname);
            
            return true;
        }
        
        public void CopyOther(Account other)
        {
            RemoveAll();
            
            for (int i = 0; i < AccountConstants.Characters; i++)
            {
                FileInfo.Characters[i] = other.FileInfo.Characters[i];
            }
            
            FileInfo.Email = other.FileInfo.Email;
            FileInfo.Name = other.FileInfo.Name;
            FileInfo.Password = other.FileInfo.Password;
            FileInfo.Flags = other.FileInfo.Flags;
            FileInfo.LastUseDate = other.FileInfo.LastUseDate;
            LastUseHostname = other.LastUseHostname;
            FileInfo.Logins = other.FileInfo.Logins;
            FileInfo.Seconds = other.FileInfo.Seconds;
            InUse = other.InUse;
            
            foreach (AccountName name in other.Friends)
            {
                AccountName aname = new AccountName(name.Name);
                Friends.Add(aname);
            }
            
            foreach (AccountName name in other.Ignores)
            {
                AccountName aname = new AccountName(name.Name);
                Ignores.Add(aname);
            }
        }
        
        private void Clear()
        {
            FileInfo = new AccountFile();
            LastUseHostname = string.Empty;
            Friends = new List<AccountName>();
            Ignores = new List<AccountName>();
        }
    }
}