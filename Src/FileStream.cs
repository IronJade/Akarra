using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace Akarra.FileSystem
{
    /// <summary>
    /// Class for reading and writing to file streams
    /// </summary>
    public class FileStream
    {
        // Fields
        private System.IO.FileStream _file;
        private long _start;
        private long _end;
        private bool _unicode;
        
        // Properties
        public bool IsOk => _file != null;
        public bool IsUnicode => _unicode;
        
        // Constructors
        public FileStream(System.IO.FileStream file, long start, long end, bool unicode)
        {
            _file = file;
            _start = start;
            _end = end;
            _unicode = unicode;
        }
        
        // Methods
        public bool Read(byte[] buffer, int length)
        {
            if (_file == null)
                return false;
                
            return _file.Read(buffer, 0, length) == length;
        }
        
        public bool Read<T>(ref T value) where T : struct
        {
            if (_file == null)
                return false;
                
            int size = System.Runtime.InteropServices.Marshal.SizeOf<T>();
            byte[] buffer = new byte[size];
            
            if (_file.Read(buffer, 0, size) != size)
                return false;
                
            GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            value = (T)System.Runtime.InteropServices.Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(T));
            handle.Free();
            
            return true;
        }
        
        public int ReadInt()
        {
            if (_file == null)
                return 0;
                
            byte[] buffer = new byte[4];
            _file.Read(buffer, 0, 4);
            return BitConverter.ToInt32(buffer, 0);
        }
        
public char ReadChar()
{
    if (_file == null)
        return '\0';
            
    int value = _file.ReadByte();
    return value == -1 ? '\0' : (char)value;
}

public void ReadTextLine(StringBuilder buffer, int maxLen)
{
    if (_file == null)
        return;
            
    int count = 0;
    char c;
            
    while (count < maxLen)
    {
        c = ReadChar();
                
        if (c == '\n' || c == '\r' || c == '\0')
        {
            // Hit end of line
            return;
        }
                
        buffer.Append(c);
        count++;
    }
}

public string ReadString(int maxLen)
{
    if (_file == null)
        return string.Empty;
            
    StringBuilder sb = new StringBuilder(maxLen);
    int count = 0;
            
    while (count < maxLen)
    {
        char c = ReadChar();
                
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0')
        {
            // End of string
            break;
        }
                
        sb.Append(c);
        count++;
    }
            
    return sb.ToString();
}

public bool Write(byte[] buffer, int length)
{
    if (_file == null)
        return false;
                
    _file.Write(buffer, 0, length);
    return true;
}

public bool Write<T>(T value) where T : struct
{
    if (_file == null)
        return false;
                
    int size = System.Runtime.InteropServices.Marshal.SizeOf<T>();
    byte[] buffer = new byte[size];
                
    GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
    System.Runtime.InteropServices.Marshal.StructureToPtr(value, handle.AddrOfPinnedObject(), false);
    handle.Free();
                
    _file.Write(buffer, 0, size);
    return true;
}

public void Write(int value)
{
    if (_file == null)
        return;
                
    byte[] buffer = BitConverter.GetBytes(value);
    _file.Write(buffer, 0, buffer.Length);
}

public void WriteChar(char c)
{
    if (_file == null)
        return;
                
    _file.WriteByte((byte)c);
}

public long GetSize()
{
    return _end - _start;
}

public long GetSizeLeft()
{
    return _end - _file.Position;
}

public long GetPosition()
{
    return _start + _file.Position;
}

public void SeekStart(int offset)
{
    if (_file == null)
        return;
                
    _file.Seek(_start + offset, SeekOrigin.Begin);
}

public void SeekEnd(int offset)
{
    if (_file == null)
        return;
                
    _file.Seek(_end - offset, SeekOrigin.Begin);
}

public void SeekCurrent(int offset)
{
    if (_file == null)
        return;
                
    _file.Seek(offset, SeekOrigin.Current);
}

public bool IsEof()
{
    return _file.Position >= _end;
}

public bool FindString(string searchString, bool placeEnd)
{
    if (_file == null || string.IsNullOrEmpty(searchString))
        return false;
                
    _file.Seek(_start, SeekOrigin.Begin);
                
    int counter = 0;
    int length = searchString.Length;
    long pointerCount = _start;
                
    while (pointerCount++ < _end)
    {
        char c = ReadChar();
                
        if (char.ToLower(c) == char.ToLower(searchString[counter]))
        {
            counter++;
            if (counter == length)
            {
                if (!placeEnd)
                {
                    // Set file pointer to beginning of string
                    _file.Seek(-counter, SeekOrigin.Current);
                }
                return true;
            }
        }
        else
        {
            counter = 0;
        }
    }
                
    return false;
}

public void Close()
{
    if (_file != null)
    {
        _file.Close();
        _file = null;
    }
}