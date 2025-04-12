using System;
using System.Text;
using System.Globalization;
using System.IO;
using System.Collections.Generic;

namespace Akarra.Basic
{
    /// <summary>
    /// String utility class with various string manipulation functions
    /// </summary>
    public class StringUtils
    {
        // Constants
        public const int MaxFilenameLength = 256;
        public const int MaxExtensionLength = 6;
        public const int MaxPathLength = 256;
        
        // Extension methods can be added to string directly
        
        /// <summary>
        /// Extracts the filename (without extension) from a path
        /// </summary>
        public static string ExtractFileName(string path)
        {
            if (string.IsNullOrEmpty(path))
                return string.Empty;
                
            string fileName = Path.GetFileNameWithoutExtension(path);
            return fileName;
        }
        
        /// <summary>
        /// Extracts the extension (without the dot) from a path
        /// </summary>
        public static string ExtractExtension(string path)
        {
            if (string.IsNullOrEmpty(path))
                return string.Empty;
                
            string extension = Path.GetExtension(path);
            if (string.IsNullOrEmpty(extension))
                return string.Empty;
                
            return extension.TrimStart('.');
        }
        
        /// <summary>
        /// Extracts the directory path from a file path
        /// </summary>
        public static string ExtractPath(string path)
        {
            if (string.IsNullOrEmpty(path))
                return string.Empty;
                
            string directory = Path.GetDirectoryName(path);
            return directory ?? string.Empty;
        }
        
        /// <summary>
        /// Checks if the string contains an extension
        /// </summary>
        public static bool HasExtension(string path)
        {
            if (string.IsNullOrEmpty(path))
                return false;
                
            return !string.IsNullOrEmpty(Path.GetExtension(path));
        }
        
        /// <summary>
        /// Replaces all occurrences of a character with another character
        /// </summary>
        public static string Replace(string text, char oldChar, char newChar)
        {
            if (string.IsNullOrEmpty(text))
                return string.Empty;
                
            return text.Replace(oldChar, newChar);
        }
        
        /// <summary>
        /// Tokenizes a string into parts using a delimiter
        /// </summary>
        public static List<string> TokenizeString(string text, char delimiter, bool respectQuotes = false)
        {
            List<string> tokens = new List<string>();
            
            if (string.IsNullOrEmpty(text))
                return tokens;
                
            if (!respectQuotes)
            {
                tokens.AddRange(text.Split(new[] { delimiter }, StringSplitOptions.RemoveEmptyEntries));
                return tokens;
            }
            
            bool inQuotes = false;
            StringBuilder currentToken = new StringBuilder();
            
            foreach (char c in text)
            {
                if (c == '"')
                {
                    inQuotes = !inQuotes;
                    // Skip the quote character
                    continue;
                }
                
                if (c == delimiter && !inQuotes)
                {
                    if (currentToken.Length > 0)
                    {
                        tokens.Add(currentToken.ToString());
                        currentToken.Clear();
                    }
                    continue;
                }
                
                currentToken.Append(c);
            }
            
            // Add the last token if there is one
            if (currentToken.Length > 0)
            {
                tokens.Add(currentToken.ToString());
            }
            
            return tokens;
        }
        
        /// <summary>
        /// Gets the current time as a formatted string
        /// </summary>
        public static string GetTimeString()
        {
            return DateTime.Now.ToString("HH:mm yyyy-MM-dd");
        }
        
        /// <summary>
        /// Gets a formatted time string from a timestamp
        /// </summary>
        public static string GetTimeString(long timestamp)
        {
            DateTime time = new DateTime(1970, 1, 1).AddSeconds(timestamp);
            return time.ToString("HH:mm yyyy-MM-dd");
        }
        
        /// <summary>
        /// Formats an integer with a delimiter every three digits
        /// </summary>
        public static string FormatWithDelimiter(int value, char delimiter)
        {
            return string.Format(CultureInfo.InvariantCulture, "{0:#,0}", value).Replace(',', delimiter);
        }
    }
}