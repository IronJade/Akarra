using System;
using System.IO;

namespace RpgSystem.Items
{
    /// <summary>
    /// Represents a specific instance of an item in the game
    /// </summary>
    public class ItemInstance
    {
        // Items have unique identifiers for network transmission and referencing
        private static uint _nextId = 1;
        
        // Fields
        public string TypeName { get; set; }    // Data name of the template
        public string CustomName { get; set; }  // Custom name for this instance
        public ItemType Type { get; set; }      // Reference to the template
        public int StackCount { get; set; }     // Number of items in stack
        public byte Enabled { get; set; }       // Whether the item is enabled/functioning
        public int UsesLeft { get; set; }       // For items with limited uses
        public int DecayTime { get; set; }      // Time until decay in minutes
        public int MountX { get; set; }         // For mount items - X coordinate
        public int MountY { get; set; }         // For mount items - Y coordinate
        
        // Unique identifier for this specific item instance
        private uint _id;
        
        /// <summary>
        /// Creates a new item instance
        /// </summary>
        public ItemInstance()
        {
            TypeName = string.Empty;
            CustomName = string.Empty;
            Type = null;
            StackCount = 1;
            Enabled = 1;
            UsesLeft = -1;  // -1 means unlimited
            DecayTime = -1; // -1 means no decay
            MountX = 0;
            MountY = 0;
            _id = _nextId++;
        }
        
        /// <summary>
        /// Creates a new item instance from a type
        /// </summary>
        public ItemInstance(ItemType type, int count = 1)
            : this()
        {
            Type = type;
            
            if (type != null)
            {
                TypeName = type.DataName;
                
                // Check if this item has limited uses
                if (type.MainType == (int)ItemMainType.Ammo)
                {
                    // Ammo uses Parameter1 as stack count
                    UsesLeft = (int)type.Parameters[0];
                }
                
                // Check for decaying items
                if ((type.Flags & ItemFlag.Decaying) != 0)
                {
                    DecayTime = type.DecayMinutes;
                }
                
                StackCount = count;
            }
        }
        
        /// <summary>
        /// Gets the unique ID of this item instance
        /// </summary>
        public uint GetId()
        {
            return _id;
        }
        
        /// <summary>
        /// Gets whether this item can be stacked with another
        /// </summary>
        public bool CanStackWith(ItemInstance other)
        {
            if (other == null || Type == null || other.Type == null)
                return false;
                
            // Items must be of the same type and both must be stackable
            if (string.Compare(TypeName, other.TypeName, StringComparison.OrdinalIgnoreCase) != 0)
                return false;
                
            if ((Type.Flags & ItemFlag.Stackable) == 0)
                return false;
                
            // Check for custom names (if either has a custom name, they can't stack)
            if (!string.IsNullOrEmpty(CustomName) || !string.IsNullOrEmpty(other.CustomName))
                return false;
                
            // Both must be enabled or both disabled
            if (Enabled != other.Enabled)
                return false;
                
            // Both must have the same uses left
            if (UsesLeft != other.UsesLeft)
                return false;
                
            // Both must have the same decay time left
            if (DecayTime != other.DecayTime)
                return false;
                
            return true;
        }
        
        /// <summary>
        /// Tries to stack this item with another item
        /// </summary>
        /// <returns>True if stacking succeeded, false otherwise</returns>
        public bool TryStackWith(ItemInstance other)
        {
            if (!CanStackWith(other))
                return false;
                
            // Calculate maximum stack size
            int maxStack = ItemHelper.GetItemMaxStackCount(Type.MainType, Type.SubType);
            
            if (StackCount >= maxStack)
                return false;
                
            // Calculate how many items can be transferred
            int transferAmount = Math.Min(other.StackCount, maxStack - StackCount);
            
            if (transferAmount <= 0)
                return false;
                
            // Transfer items
            StackCount += transferAmount;
            other.StackCount -= transferAmount;
            
            return true;
        }
        
        /// <summary>
        /// Splits this stack into two stacks
        /// </summary>
        /// <returns>New item instance with the split stack</returns>
        public ItemInstance SplitStack(int amount)
        {
            if (amount <= 0 || amount >= StackCount)
                return null;
                
            // Create a new item with the specified amount
            ItemInstance newItem = new ItemInstance();
            newItem.TypeName = TypeName;
            newItem.CustomName = CustomName;
            newItem.Type = Type;
            newItem.StackCount = amount;
            newItem.Enabled = Enabled;
            newItem.UsesLeft = UsesLeft;
            newItem.DecayTime = DecayTime;
            
            // Remove the amount from this stack
            StackCount -= amount;
            
            return newItem;
        }
        
        /// <summary>
        /// Gets if this item is enabled
        /// </summary>
        public bool IsEnabled()
        {
            return Enabled != 0;
        }
        
        /// <summary>
        /// Sets the enabled state of this item
        /// </summary>
        public void SetEnabled(bool enabled)
        {
            Enabled = (byte)(enabled ? 1 : 0);
        }
        
        /// <summary>
        /// Gets the display name of this item
        /// </summary>
        public string GetItemDisplayName()
        {
            // If there's a custom name, use that
            if (!string.IsNullOrEmpty(CustomName))
                return CustomName;
                
            // Otherwise use the type's display name if available
            if (Type != null)
                return Type.DisplayName;
                
            // Fallback to type name or blank
            return TypeName ?? string.Empty;
        }
        
        /// <summary>
        /// Decreases the uses left on this item
        /// </summary>
        /// <returns>True if the item still has uses left or unlimited uses</returns>
        public bool DecreaseUses()
        {
            // If unlimited uses, return true
            if (UsesLeft < 0)
                return true;
                
            // Decrease uses
            UsesLeft--;
            
            // Return true if there are uses left
            return UsesLeft > 0;
        }
        
        /// <summary>
        /// Updates the decay timer for this item
        /// </summary>
        /// <param name="minutes">Minutes elapsed</param>
        /// <returns>True if the item has not decayed yet</returns>
        public bool UpdateDecay(int minutes)
        {
            // If no decay, return true
            if (DecayTime < 0)
                return true;
                
            // Decrease decay time
            DecayTime -= minutes;
            
            // Return true if there is decay time left
            return DecayTime > 0;
        }
        
        /// <summary>
        /// Reads item data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader, int version)
        {
            try
            {
                // Clear current state
                Type = null;
                
                // Read type name
                TypeName = new string(reader.ReadChars(ItemType.ItemNameLength)).TrimEnd('\0');
                
                // Read stack count
                StackCount = reader.ReadInt32();
                
                // Read enabled state
                Enabled = reader.ReadByte();
                
                // Read uses left
                UsesLeft = reader.ReadInt32();
                
                // Read custom name if available (newer versions)
                if (version > PlayerFileVersions.V_030219)
                {
                    CustomName = new string(reader.ReadChars(32)).TrimEnd('\0');
                }
                
                // Read mount coordinates if available (newer versions)
                if (version > PlayerFileVersions.V_030102)
                {
                    MountX = reader.ReadInt32();
                    MountY = reader.ReadInt32();
                }
                
                // Read decay time if available (newer versions)
                if (version > PlayerFileVersions.V_030504b)
                {
                    DecayTime = reader.ReadInt32();
                }
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        /// <summary>
        /// Writes item data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write type name
                WriteFixedString(writer, TypeName, ItemType.ItemNameLength);
                
                // Write stack count
                writer.Write(StackCount);
                
                // Write enabled state
                writer.Write(Enabled);
                
                // Write uses left
                writer.Write(UsesLeft);
                
                // Write custom name
                WriteFixedString(writer, CustomName, 32);
                
                // Write mount coordinates
                writer.Write(MountX);
                writer.Write(MountY);
                
                // Write decay time
                writer.Write(DecayTime);
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        /// <summary>
        /// Helper method to write fixed-length strings
        /// </summary>
        private void WriteFixedString(BinaryWriter writer, string text, int length)
        {
            char[] chars = new char[length];
            if (!string.IsNullOrEmpty(text))
            {
                for (int i = 0; i < Math.Min(text.Length, length); i++)
                {
                    chars[i] = text[i];
                }
            }
            writer.Write(chars);
        }
    }
}