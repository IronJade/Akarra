using System;
using System.Collections.Generic;
using System.IO;

namespace RpgSystem.Effects
{
    /// <summary>
    /// Manages a collection of effect types
    /// </summary>
    public class EffectBundle
    {
        // Fields
        private List<EffectType> _effects;
        
        /// <summary>
        /// Creates a new effect bundle
        /// </summary>
        public EffectBundle()
        {
            _effects = new List<EffectType>();
        }
        
        /// <summary>
        /// Removes all effects from the bundle
        /// </summary>
        public void RemoveAll()
        {
            _effects.Clear();
        }
        
        /// <summary>
        /// Gets the list of all effect types
        /// </summary>
        public List<EffectType> GetEffectTypes()
        {
            return _effects;
        }
        
        /// <summary>
        /// Gets the number of effect types in the bundle
        /// </summary>
        public int GetEffectCount()
        {
            return _effects.Count;
        }
        
        /// <summary>
        /// Finds an effect type by its data name
        /// </summary>
        public EffectType GetEffectType(string dataName)
        {
            if (string.IsNullOrEmpty(dataName))
                return null;
                
            foreach (var effect in _effects)
            {
                if (string.Compare(effect.DataName, dataName, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return effect;
                }
            }
            
            return null;
        }
        
        /// <summary>
        /// Creates a new effect type in the bundle
        /// </summary>
        public EffectType CreateNewEffectType()
        {
            EffectType effect = new EffectType();
            _effects.Add(effect);
            return effect;
        }
        
        /// <summary>
        /// Adds an existing effect type to the bundle
        /// </summary>
        public bool AddEffectType(EffectType effect)
        {
            if (effect == null)
                return false;
                
            // Check if an effect with this name already exists
            if (GetEffectType(effect.DataName) != null)
                return false;
                
            _effects.Add(effect);
            return true;
        }
        
        /// <summary>
        /// Removes an effect type from the bundle
        /// </summary>
        public bool DeleteEffectType(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;
                
            for (int i = 0; i < _effects.Count; i++)
            {
                if (string.Compare(_effects[i].DataName, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _effects.RemoveAt(i);
                    return true;
                }
            }
            
            return false;
        }
        
        /// <summary>
        /// Reads effects from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                // Clear any existing data
                RemoveAll();
                
                // Read version
                int version = reader.ReadInt32();
                
                // Read effect count
                int count = reader.ReadInt32();
                
                // Read effects
                for (int i = 0; i < count; i++)
                {
                    EffectType effect = new EffectType();
                    
                    if (!effect.Read(reader))
                    {
                        return false;
                    }
                    
                    _effects.Add(effect);
                }
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        /// <summary>
        /// Writes effects to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write((int)EffectFileVersions.Current);
                
                // Write effect count
                writer.Write(_effects.Count);
                
                // Write effects
                foreach (var effect in _effects)
                {
                    if (!effect.Write(writer))
                    {
                        return false;
                    }
                }
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        /// <summary>
        /// Opens effects from a file
        /// </summary>
        public bool Open(string filename)
        {
            try
            {
                using (FileStream fileStream = File.OpenRead(filename))
                using (BinaryReader reader = new BinaryReader(fileStream))
                {
                    return Read(reader);
                }
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        /// <summary>
        /// Saves effects to a file
        /// </summary>
        public bool Save(string filename)
        {
            try
            {
                using (FileStream fileStream = File.Create(filename))
                using (BinaryWriter writer = new BinaryWriter(fileStream))
                {
                    return Write(writer);
                }
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}