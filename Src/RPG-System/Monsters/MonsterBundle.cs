using System;
using System.Collections.Generic;
using System.IO;

namespace RpgSystem.Monsters
{
    /// <summary>
    /// File version constants for monster bundles
    /// </summary>
    public static class MonsterBundleVersions
    {
        public const int Original = 0;
        public const int Current = Original;
    }

    /// <summary>
    /// Monster family data for grouping and balancing monster types
    /// </summary>
    public class MonsterFamily
    {
        // Family identification
        public string Name { get; set; }

        // Ability scaling
        public float[] BaseAbilities { get; private set; }
        public float[] MultiAbilities { get; private set; }
        
        // Health scaling
        public float BaseHP { get; set; }
        public float MultiHP { get; set; }
        
        // Experience scaling
        public float ExpMultiplier { get; set; }
        
        // Design notes
        public string DesignerNotes { get; set; }

        /// <summary>
        /// Creates a new monster family
        /// </summary>
        public MonsterFamily()
        {
            BaseAbilities = new float[(int)Attributes.Ability.Count];
            MultiAbilities = new float[(int)Attributes.Ability.Count];
            
            Clear();
        }

        /// <summary>
        /// Resets all properties to default values
        /// </summary>
        public void Clear()
        {
            Name = string.Empty;

            // Setup default balance values
            BaseAbilities[(int)Attributes.Ability.AttackRating] = 10.0f;
            MultiAbilities[(int)Attributes.Ability.AttackRating] = 2.0f;
            BaseAbilities[(int)Attributes.Ability.CastRating] = 5.0f;
            MultiAbilities[(int)Attributes.Ability.CastRating] = 2.0f;
            BaseAbilities[(int)Attributes.Ability.DamageMinimum] = 1.0f * 1.25f;
            MultiAbilities[(int)Attributes.Ability.DamageMinimum] = 0.22f * 1.25f;
            BaseAbilities[(int)Attributes.Ability.DamageMaximum] = 2.0f * 1.25f;
            MultiAbilities[(int)Attributes.Ability.DamageMaximum] = 0.55f * 1.25f;
            BaseAbilities[(int)Attributes.Ability.ArmorClass] = 5.0f;
            MultiAbilities[(int)Attributes.Ability.ArmorClass] = 1.5f;
            BaseAbilities[(int)Attributes.Ability.ResistWhite] = 2.0f;
            MultiAbilities[(int)Attributes.Ability.ResistWhite] = 1.0f;
            BaseAbilities[(int)Attributes.Ability.ResistRed] = 2.0f;
            MultiAbilities[(int)Attributes.Ability.ResistRed] = 1.0f;
            BaseAbilities[(int)Attributes.Ability.ResistGreen] = 2.0f;
            MultiAbilities[(int)Attributes.Ability.ResistGreen] = 1.0f;
            BaseAbilities[(int)Attributes.Ability.ResistBlue] = 2.0f;
            MultiAbilities[(int)Attributes.Ability.ResistBlue] = 1.0f;
            BaseAbilities[(int)Attributes.Ability.ResistYellow] = 2.0f;
            MultiAbilities[(int)Attributes.Ability.ResistYellow] = 1.0f;
            BaseAbilities[(int)Attributes.Ability.DamageReduction] = 0.5f;
            MultiAbilities[(int)Attributes.Ability.DamageReduction] = 0.1f;

            BaseHP = -10.0f;
            MultiHP = 8.0f;

            ExpMultiplier = 1.0f;

            DesignerNotes = string.Empty;
        }

        /// <summary>
        /// Reads family data from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                Clear();

                // Read version
                int version = reader.ReadInt32();

                // Read name
                Name = new string(reader.ReadChars(32)).TrimEnd('\0');

                if (version > 0) // MFFV_030401
                {
                    for (int i = 0; i < (int)Attributes.Ability.Count; i++)
                    {
                        BaseAbilities[i] = reader.ReadSingle();
                        MultiAbilities[i] = reader.ReadSingle();
                    }

                    BaseHP = reader.ReadSingle();
                    MultiHP = reader.ReadSingle();
                    ExpMultiplier = reader.ReadSingle();

                    int designerNotesLength = reader.ReadInt32();
                    if (designerNotesLength > 0)
                    {
                        char[] notesChars = reader.ReadChars(designerNotesLength);
                        DesignerNotes = new string(notesChars);
                    }
                }

                // Apply balance changes for older file versions
                if (version <= 2) // MFFV_030513
                {
                    BaseAbilities[(int)Attributes.Ability.DamageMinimum] *= 1.25f;
                    MultiAbilities[(int)Attributes.Ability.DamageMinimum] *= 1.25f;
                    BaseAbilities[(int)Attributes.Ability.DamageMaximum] *= 1.25f;
                    MultiAbilities[(int)Attributes.Ability.DamageMaximum] *= 1.25f;
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes family data to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write(MonsterBundleVersions.Current);

                // Write name (fixed 32 chars)
                char[] nameChars = new char[32];
                if (!string.IsNullOrEmpty(Name))
                {
                    for (int i = 0; i < Math.Min(Name.Length, 32); i++)
                    {
                        nameChars[i] = Name[i];
                    }
                }
                writer.Write(nameChars);

                // Write ability data
                for (int i = 0; i < (int)Attributes.Ability.Count; i++)
                {
                    writer.Write(BaseAbilities[i]);
                    writer.Write(MultiAbilities[i]);
                }

                // Write health and experience
                writer.Write(BaseHP);
                writer.Write(MultiHP);
                writer.Write(ExpMultiplier);

                // Write designer notes
                if (string.IsNullOrEmpty(DesignerNotes))
                {
                    writer.Write(0);
                }
                else
                {
                    writer.Write(DesignerNotes.Length);
                    writer.Write(DesignerNotes.ToCharArray());
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
    }

    /// <summary>
    /// Manages a collection of monster types and families
    /// </summary>
    public class MonsterBundle
    {
        private List<MonsterType> _monsters;
        private List<MonsterFamily> _families;
        private int _nextId;

        /// <summary>
        /// Creates a new monster bundle
        /// </summary>
        public MonsterBundle()
        {
            _monsters = new List<MonsterType>();
            _families = new List<MonsterFamily>();
            _nextId = 1;
        }

        /// <summary>
        /// Removes all monster types and families
        /// </summary>
        public void RemoveAll()
        {
            _monsters.Clear();
            _families.Clear();
            _nextId = 1;
        }

        /// <summary>
        /// Reads monster bundle from a binary stream
        /// </summary>
        public bool Read(BinaryReader reader)
        {
            try
            {
                // Clear existing data
                RemoveAll();

                // Read version
                int version = reader.ReadInt32();

                // Read monster types
                int count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                {
                    MonsterType monster = new MonsterType();
                    if (!monster.Read(reader))
                    {
                        return false;
                    }

                    // Set runtime ID
                    monster.RuntimeId = _nextId++;

                    // Add to collection
                    _monsters.Add(monster);
                }

                // Read monster families
                count = reader.ReadInt32();
                for (int i = 0; i < count; i++)
                {
                    MonsterFamily family = new MonsterFamily();
                    if (!family.Read(reader))
                    {
                        return false;
                    }

                    _families.Add(family);
                }

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes monster bundle to a binary stream
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write version
                writer.Write(MonsterBundleVersions.Current);

                // Write monster types
                writer.Write(_monsters.Count);
                foreach (var monster in _monsters)
                {
                    if (!monster.Write(writer))
                    {
                        return false;
                    }
                }

                // Write monster families
                writer.Write(_families.Count);
                foreach (var family in _families)
                {
                    if (!family.Write(writer))
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
        /// Opens monster bundle from a file
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
        /// Saves monster bundle to a file
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

        #region Monster Types

        /// <summary>
        /// Gets all monster types
        /// </summary>
        public List<MonsterType> GetMonsterTypes()
        {
            return _monsters;
        }

        /// <summary>
        /// Gets the number of monster types
        /// </summary>
        public int GetMonsterTypeCount()
        {
            return _monsters.Count;
        }

        /// <summary>
        /// Finds a monster type by name
        /// </summary>
        public MonsterType GetMonsterType(string name)
        {
            if (string.IsNullOrEmpty(name))
                return null;

            foreach (var monster in _monsters)
            {
                if (string.Compare(monster.DataName, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return monster;
                }
            }

            return null;
        }

        /// <summary>
        /// Creates a new monster type
        /// </summary>
        public MonsterType CreateNewMonsterType()
        {
            MonsterType monster = new MonsterType();
            monster.RuntimeId = _nextId++;

            _monsters.Add(monster);
            return monster;
        }

        /// <summary>
        /// Adds an existing monster type
        /// </summary>
        public bool AddMonsterType(MonsterType monster)
        {
            if (monster == null)
                return false;

            // Check if a monster with this name already exists
            if (GetMonsterType(monster.DataName) != null)
                return false;

            // Set runtime ID
            monster.RuntimeId = _nextId++;

            _monsters.Add(monster);
            return true;
        }

        /// <summary>
        /// Removes a monster type by name
        /// </summary>
        public bool DeleteMonsterType(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            for (int i = 0; i < _monsters.Count; i++)
            {
                if (string.Compare(_monsters[i].DataName, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _monsters.RemoveAt(i);
                    return true;
                }
            }

            return false;
        }

        #endregion

        #region Monster Families

        /// <summary>
        /// Gets all monster families
        /// </summary>
        public List<MonsterFamily> GetFamilies()
        {
            return _families;
        }

        /// <summary>
        /// Gets the number of monster families
        /// </summary>
        public int GetFamilyCount()
        {
            return _families.Count;
        }

        /// <summary>
        /// Finds a monster family by name
        /// </summary>
        public MonsterFamily GetFamily(string name)
        {
            if (string.IsNullOrEmpty(name))
                return null;

            foreach (var family in _families)
            {
                if (string.Compare(family.Name, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    return family;
                }
            }

            return null;
        }

        /// <summary>
        /// Gets a monster family by index
        /// </summary>
        public MonsterFamily GetFamily(int index)
        {
            if (index < 0 || index >= _families.Count)
                return null;

            return _families[index];
        }

        /// <summary>
        /// Adds a monster family
        /// </summary>
        public bool AddFamily(MonsterFamily family)
        {
            if (family == null)
                return false;

            // Check if a family with this name already exists
            if (GetFamily(family.Name) != null)
                return false;

            _families.Add(family);
            return true;
        }

        /// <summary>
        /// Removes a monster family by name
        /// </summary>
        public bool DeleteFamily(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            for (int i = 0; i < _families.Count; i++)
            {
                if (string.Compare(_families[i].Name, name, StringComparison.OrdinalIgnoreCase) == 0)
                {
                    _families.RemoveAt(i);
                    return true;
                }
            }

            return false;
        }

        #endregion
    }
}