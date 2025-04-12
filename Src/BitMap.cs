using System;

namespace Akarra.Common
{
    /// <summary>
    /// A bitmap that stores 4 bits per position
    /// </summary>
    public class FourBitMap
    {
        // Constants for column flags and shifts
        private static readonly ushort[] ColumnFlags = { 0xF000, 0x0F00, 0x00F0, 0x000F };
        private static readonly int[] ColumnShifts = { 12, 8, 4, 0 };
        
        // Fields
        private ushort[] _map;
        private int _width;
        private int _physicalWidth;
        private int _height;
        
        // Properties
        public int Width => _width;
        public int Height => _height;
        
        // Constructors
        public FourBitMap()
        {
            Clear();
        }
        
        // Methods
        public bool Create(int width, int height)
        {
            if (width < 4 || height < 1)
                return false;
                
            RemoveAll(); // To be on the safe side
            
            _width = width;
            _height = height;
            _physicalWidth = _width >> 2; // You can put 4 objects into each unsigned short
            
            // Create the memory
            _map = new ushort[_physicalWidth * _height];
            
            if (_map == null)
                return false;
                
            Reset();
            
            return true;
        }
        
        public void RemoveAll()
        {
            _map = null;
            Clear();
        }
        
        public bool SetPos(int x, int y, ushort value)
        {
            if (_map == null || x < 0 || x >= _width || y < 0 || y >= _height)
                return false;
                
            // Make sure the value is only four bits
            value &= 0x000F;
            
            // Find the unsigned short for our position
            int index = (x >> 2) + (y * _physicalWidth);
            
            // Clear the column
            _map[index] &= (ushort)~ColumnFlags[x & 0x3];
            
            // Insert our new value in the same column
            _map[index] |= (ushort)(value << ColumnShifts[x & 0x3]);
            
            return true;
        }
        
        public ushort GetPos(int x, int y)
        {
            if (_map == null || x < 0 || x >= _width || y < 0 || y >= _height)
                return 0;
                
            // Find the unsigned short for our position
            int index = (x >> 2) + (y * _physicalWidth);
            
            // Get the value
            ushort value = (ushort)((_map[index] & ColumnFlags[x & 0x3]) >> ColumnShifts[x & 0x3]);
            
            return value;
        }
        
        public void Reset()
        {
            if (_map != null)
            {
                for (int i = 0; i < (_physicalWidth * _height); i++)
                {
                    _map[i] = 0;
                }
            }
        }
        
        public void Fill(ushort value)
        {
            if (_map == null)
                return;
                
            // Make sure it's only four bits
            value &= 0x000F;
            // Create a quad packet
            ushort quadValue = (ushort)((value << 12) | (value << 8) | (value << 4) | value);
            
            for (int i = 0; i < (_physicalWidth * _height); i++)
            {
                _map[i] = quadValue;
            }
        }
        
        private void Clear()
        {
            _map = null;
            _width = 0;
            _physicalWidth = 0;
            _height = 0;
        }
    }
    
    /// <summary>
    /// A bitmap that stores 2 bits per position
    /// </summary>
    public class TwoBitMap
    {
        // Constants for column flags and shifts
        private static readonly byte[] ColumnFlags = { 0xC0, 0x30, 0x0C, 0x03 };
        private static readonly int[] ColumnShifts = { 6, 4, 2, 0 };
        
        // Fields
        private byte[] _map;
        private int _width;
        private int _physicalWidth;
        private int _height;
        
        // Properties
        public int Width => _width;
        public int Height => _height;
        
        // Constructors
        public TwoBitMap()
        {
            Clear();
        }
        
        // Methods
        public bool Create(int width, int height)
        {
            if (width < 4 || height < 1)
                return false;
                
            RemoveAll(); // To be on the safe side
            
            _width = width;
            _height = height;
            _physicalWidth = _width >> 2; // You can put 4 objects into each byte
            
            // Create the memory
            _map = new byte[_physicalWidth * _height];
            
            if (_map == null)
                return false;
                
            Reset();
            
            return true;
        }
        
        public void RemoveAll()
        {
            _map = null;
            Clear();
        }
        
        public void SetPos(int x, int y, byte value)
        {
            if (_map == null || x < 0 || x >= _width || y < 0 || y >= _height)
                return;
                
            // Make sure the value is only two bits
            value &= 0x03;
            
            // Find the byte for our position
            int index = (x >> 2) + (y * _physicalWidth);
            
            // Clear the column
            _map[index] &= (byte)~ColumnFlags[x & 0x3];
            
            // Insert our new value in the same column
            _map[index] |= (byte)(value << ColumnShifts[x & 0x3]);
        }
        
        public byte GetPos(int x, int y)
        {
            if (_map == null || x < 0 || x >= _width || y < 0 || y >= _height)
                return 0;
                
            // Find the byte for our position
            int index = (x >> 2) + (y * _physicalWidth);
            
            // Get the value
            byte value = (byte)((_map[index] & ColumnFlags[x & 0x3]) >> ColumnShifts[x & 0x3]);
            
            return value;
        }
        
        public void Reset()
        {
            if (_map != null)
            {
                for (int i = 0; i < (_physicalWidth * _height); i++)
                {
                    _map[i] = 0;
                }
            }
        }
        
        private void Clear()
        {
            _map = null;
            _width = 0;
            _physicalWidth = 0;
            _height = 0;
        }
    }
    
    /// <summary>
    /// A bitmap that stores 1 bit per position
    /// </summary>
    public class OneBitMap
    {
        // Constants for column flags and shifts
        private static readonly byte[] ColumnFlags = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
        private static readonly int[] ColumnShifts = { 7, 6, 5, 4, 3, 2, 1, 0 };
        
        // Fields
        private byte[] _map;
        private int _width;
        private int _physicalWidth;
        private int _height;
        
        // Properties
        public int Width => _width;
        public int Height => _height;
        
        // Constructors
        public OneBitMap()
        {
            Clear();
        }
        
        // Methods
        public bool Create(int width, int height)
        {
            if (width < 8 || height < 1)
                return false;
                
            RemoveAll(); // To be on the safe side
            
            _width = width;
            _height = height;
            _physicalWidth = _width >> 3; // You can put 8 objects into each byte
            
            // Create the memory
            _map = new byte[_physicalWidth * _height];
            
            if (_map == null)
                return false;
                
            Reset();
            
            return true;
        }
        
        public void RemoveAll()
        {
            _map = null;
            Clear();
        }
        
        public void SetPos(int x, int y, byte value)
        {
            if (_map == null || x < 0 || x >= _width || y < 0 || y >= _height)
                return;
                
            // Make sure the value is only one bit
            value &= 0x01;
            
            // Find the byte for our position
            int index = (x >> 3) + (y * _physicalWidth);
            
            // Clear the column
            _map[index] &= (byte)~ColumnFlags[x & 0x7];
            
            // Insert our new value in the same column
            _map[index] |= (byte)(value << ColumnShifts[x & 0x7]);
        }
        
        public byte GetPos(int x, int y)
        {
            if (_map == null || x < 0 || x >= _width || y < 0 || y >= _height)
                return 1;
                
            // Find the byte for our position
            int index = (x >> 3) + (y * _physicalWidth);
            
            // Get the value
            byte value = (byte)((_map[index] & ColumnFlags[x & 0x7]) >> ColumnShifts[x & 0x7]);
            
            return value;
        }
        
        public void Reset()
        {
            if (_map != null)
            {
                for (int i = 0; i < (_physicalWidth * _height); i++)
                {
                    _map[i] = 0;
                }
            }
        }
        
        public bool Read(Stream stream)
        {
            if (stream == null)
                return false;
                
            // Read size of bitmap
            byte[] intBytes = new byte[4];
            if (stream.Read(intBytes, 0, 4) != 4)
                return false;
            int width = BitConverter.ToInt32(intBytes, 0);
            
            if (stream.Read(intBytes, 0, 4) != 4)
                return false;
            int height = BitConverter.ToInt32(intBytes, 0);
            
            if (!Create(width, height))
                return false;
                
            // Read data
            return stream.Read(_map, 0, _physicalWidth * _height) == _physicalWidth * _height;
        }
        
        public bool Write(Stream stream)
        {
            if (stream == null || _map == null)
                return false;
                
            // Write size of bitmap
            byte[] widthBytes = BitConverter.GetBytes(_width);
            stream.Write(widthBytes, 0, 4);
            
            byte[] heightBytes = BitConverter.GetBytes(_height);
            stream.Write(heightBytes, 0, 4);
            
            // Write data
            stream.Write(_map, 0, _physicalWidth * _height);
            
            return true;
        }
        
        private void Clear()
        {
            _map = null;
            _width = 0;
            _physicalWidth = 0;
            _height = 0;
        }
    }
}