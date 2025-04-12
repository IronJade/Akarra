using System;
using System.Text;
using System.IO;
using System.Collections.Generic;

namespace Akarra.FileSystem
{
    /// <summary>
    /// Utility functions for file system operations
    /// </summary>
    public static class FileSystem
    {
        private const int StringBufferLength = 1024;
        
        /// <summary>
        /// Reads a string with a preset length from a file
        /// </summary>
        public static string ReadStringWithLength(FileStream file)
        {
            // Read length
            int stringLength = file.ReadInt();
            if (stringLength <= 0)
                return string.Empty;
                
            StringBuilder tempString = new StringBuilder(stringLength);
            byte[] stringBuffer = new byte[StringBufferLength];
                
            while (stringLength >= StringBufferLength)
            {
                file.Read(stringBuffer, StringBufferLength - 1);
                stringBuffer[StringBufferLength - 1] = 0;
                    
                tempString.Append(Encoding.UTF8.GetString(stringBuffer, 0, StringBufferLength - 1));
                stringLength -= StringBufferLength - 1;
            }
                
            // Read the remaining string
            file.Read(stringBuffer, stringLength);
            stringBuffer[stringLength] = 0;
                
            tempString.Append(Encoding.UTF8.GetString(stringBuffer, 0, stringLength));
            return tempString.ToString();
        }
            
        /// <summary>
        /// Reads a string with a terminating NULL character
        /// </summary>
        public static string ReadTerminatedString(FileStream file)
        {
            int index = 0;
            char c = ' ';
            byte[] stringBuffer = new byte[StringBufferLength];
            StringBuilder tempString = new StringBuilder();
                
            // Read until end of string
            while (c != 0)
            {
                c = file.ReadChar();
                stringBuffer[index++] = (byte)c;
                    
                // Check if the string buffer overflows
                if (index == (StringBufferLength - 1))
                {
                    // Copy the current string buffer
                    stringBuffer[index] = 0;
                    tempString.Append(Encoding.UTF8.GetString(stringBuffer, 0, index));
                        
                    // Reset counter
                    index = 0;
                }
            }
                
            // Add remaining string to result
            tempString.Append(Encoding.UTF8.GetString(stringBuffer, 0, index));
            return tempString.ToString();
        }
            
        /// <summary>
        /// Writes a string to a file, saving the length for faster reads
        /// </summary>
        public static void WriteStringWithLength(FileStream file, string text)
        {
            file.Write(text.Length);
            byte[] textBytes = Encoding.UTF8.GetBytes(text);
            file.Write(textBytes, textBytes.Length);
        }
            
        /// <summary>
        /// Writes a string to a file, including the terminating NULL character
        /// </summary>
        public static void WriteTerminatedString(FileStream file, string text)
        {
            byte[] textBytes = Encoding.UTF8.GetBytes(text + '\0');
            file.Write(textBytes, textBytes.Length);
        }
            
        /// <summary>
        /// Opens a file directly using standard C# functions
        /// </summary>
        public static FileStream DirectOpen(string path, string mode)
        {
            System.IO.FileMode fileMode;
            System.IO.FileAccess fileAccess;
                
            switch (mode.ToLowerInvariant())
            {
                case "r":
                case "rb":
                    fileMode = System.IO.FileMode.Open;
                    fileAccess = System.IO.FileAccess.Read;
                    break;
                case "w":
                case "wb":
                    fileMode = System.IO.FileMode.Create;
                    fileAccess = System.IO.FileAccess.Write;
                    break;
                default:
                    return null;
            }
                
            try
            {
                System.IO.FileStream fs = new System.IO.FileStream(path, fileMode, fileAccess);
                long start = fs.Position;
                long end = fs.Length;
                    
                bool isUnicode = false;
                if (fileAccess.HasFlag(System.IO.FileAccess.Read) && fs.Length >= 2)
                {
                    byte[] buf = new byte[2];
                    fs.Read(buf, 0, 2);
                        
                    if (buf[0] == 0xFF && buf[1] == 0xFE)
                    {
                        isUnicode = true;
                    }
                    else
                    {
                        fs.Seek(0, SeekOrigin.Begin);
                    }
                }
                    
                return new FileStream(fs, start, end, isUnicode);
            }
            catch
            {
                return null;
            }
        }
            
        /// <summary>
        /// Closes a file stream
        /// </summary>
        public static void DirectClose(FileStream file)
        {
            file?.Close();
        }
            
        /// <summary>
        /// Checks if a file exists
        /// </summary>
        public static bool FileExists(string path)
        {
            return File.Exists(path);
        }
            
        /// <summary>
        /// Checks if a directory exists
        /// </summary>
        public static bool DirectoryExists(string name)
        {
            return Directory.Exists(name);
        }
            
        /// <summary>
        /// Counts all files that match the filename pattern in the path
        /// </summary>
        public static int CountFiles(string path, string filename, bool recursive)
        {
            if (!Directory.Exists(path))
                return 0;
                
            int count = 0;
                
            if (recursive)
            {
                try
                {
                    foreach (string dir in Directory.GetDirectories(path))
                    {
                        if (!dir.EndsWith(".") && !dir.EndsWith(".."))
                        {
                            count += CountFiles(dir, filename, true);
                        }
                    }
                }
                catch
                {
                    // Ignore errors
                }
            }
                
            try
            {
                count += Directory.GetFiles(path, filename).Length;
            }
            catch
            {
                // Ignore errors
            }
                
            return count;
        }
            
        /// <summary>
        /// Gets all filenames that match the pattern in the path
        /// </summary>
        public static void GetFilenames(List<string> results, string path, string filename, bool recursive)
        {
            if (!Directory.Exists(path))
                return;
                
            if (recursive)
            {
                try
                {
                    foreach (string dir in Directory.GetDirectories(path))
                    {
                        if (!dir.EndsWith(".") && !dir.EndsWith(".."))
                        {
                            GetFilenames(results, dir, filename, true);
                        }
                    }
                }
                catch
                {
                    // Ignore errors
                }
            }
                
            try
            {
                foreach (string file in Directory.GetFiles(path, filename))
                {
                    results.Add(Path.Combine(path, file));
                }
            }
            catch
            {
                // Ignore errors
            }
        }
            
        /// <summary>
        /// Gets all filenames (without paths) that match the pattern in the path
        /// </summary>
        public static void GetFilenamesNoPath(List<string> results, string path, string filename, bool recursive)
        {
            if (!Directory.Exists(path))
                return;
                
            if (recursive)
            {
                try
                {
                    foreach (string dir in Directory.GetDirectories(path))
                    {
                        if (!dir.EndsWith(".") && !dir.EndsWith(".."))
                        {
                            GetFilenamesNoPath(results, dir, filename, true);
                        }
                    }
                }
                catch
                {
                    // Ignore errors
                }
            }
                
            try
            {
                foreach (string file in Directory.GetFiles(path, filename))
                {
                    results.Add(Path.GetFileName(file));
                }
            }
            catch
            {
                // Ignore errors
            }
        }
    }
}