using System;
using System.IO;
using RpgSystem.Effects;
using RpgSystem.Entities;

namespace RpgSystem.Effects
{
    /// <summary>
    /// A character effect is a spell effect which has been placed on an entity
    /// and needs to be updated regularly. Effects can be temporary buffs, debuffs,
    /// regeneration, poison, etc.
    /// </summary>
    public class CChrEffect
    {
        // Fields
        private int _attackerId;       // ID of the entity that caused this effect
        private EffectType _effect;    // The effect definition
        private string _effectName;    // Name of the effect (for serialization)
        private int _lifeTime;         // Remaining time in milliseconds
        private int _seconds;          // Time counter in milliseconds (renamed to match C++ code)
        private int _skillSource;      // Source skill ID that created this effect
        private float _sourceCastRating; // Cast rating when effect was applied
        private int _identifier;       // Unique identifier for network transmission

        // Properties
        public int Id => _identifier;
        public string EffectName => _effectName;
        public float CastRating => _sourceCastRating;
        public int SourceId => _attackerId;
        public int RemainingLifeMillis => _lifeTime;
        public EffectType Effect => _effect;

        /// <summary>
        /// Creates a new character effect
        /// </summary>
        public CChrEffect()
        {
            _attackerId = -1;
            _effect = null;
            _effectName = string.Empty;
            _lifeTime = 0;
            _seconds = 0;
            _skillSource = -1;
            _sourceCastRating = 0;
            _identifier = 0;
        }

        /// <summary>
        /// Reads effect data from a binary stream (used for player characters)
        /// </summary>
        public bool Read(BinaryReader reader, int version)
        {
            try
            {
                // Read effect name (32 chars)
                _effectName = new string(reader.ReadChars(32)).TrimEnd('\0');
                
                // Read timers
                _lifeTime = reader.ReadInt32();
                _seconds = reader.ReadInt32();
                
                // Read other data if version supports it
                if (version > PlayerFileVersions.V_030301)
                {
                    _identifier = reader.ReadInt32();
                    _skillSource = reader.ReadInt32();
                    _sourceCastRating = reader.ReadSingle();
                }
                
                // These values are not saved (entity IDs are not persistent)
                _attackerId = -1;
                _effect = null;
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Writes effect data to a binary stream (used for player characters)
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            try
            {
                // Write effect name (32 chars)
                char[] nameChars = new char[32];
                if (!string.IsNullOrEmpty(_effectName))
                {
                    for (int i = 0; i < Math.Min(_effectName.Length, 32); i++)
                    {
                        nameChars[i] = _effectName[i];
                    }
                }
                writer.Write(nameChars);
                
                // Write timers
                writer.Write(_lifeTime);
                writer.Write(_seconds);
                
                // Write other data
                writer.Write(_identifier);
                writer.Write(_skillSource);
                writer.Write(_sourceCastRating);
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Gets the ID of this effect
        /// </summary>
        public int GetID()
        {
            return _identifier;
        }

        /// <summary>
        /// Checks if this effect is overruled by a new effect with the same skill ID
        /// </summary>
        /// <returns>True if this effect should be removed</returns>
        public bool IsOverruled(int skill)
        {
            return (_skillSource >= 0) && (_skillSource == skill);
        }

        /// <summary>
        /// Checks if this effect modifies character stats
        /// </summary>
        public bool AffectsStats()
        {
            if (_effect == null)
                return false;

            switch (_effect.Type)
            {
                case (int)EffectMechanismType.Boost:
                case (int)EffectMechanismType.Weaken:
                case (int)EffectMechanismType.BoostPercent:
                case (int)EffectMechanismType.WeakenPercent:
                case (int)EffectMechanismType.ImproveAttackSpeed:
                case (int)EffectMechanismType.ReduceAttackSpeed:
                case (int)EffectMechanismType.SpeedPercent:
                    return true;
                default:
                    return false;
            }
        }

        /// <summary>
        /// Gets the effect type
        /// </summary>
        public EffectType GetEffect()
        {
            return _effect;
        }

        /// <summary>
        /// Gets the cast rating that was used when the effect started
        /// </summary>
        public float GetCastRating()
        {
            return _sourceCastRating;
        }

        /// <summary>
        /// Gets the effect name
        /// </summary>
        public string GetEffectName()
        {
            return _effectName;
        }
        
        /// <summary>
        /// Sets the effect type
        /// </summary>
        public void SetEffect(EffectType effect)
        {
            if (effect == null)
                return;
                
            _effect = effect;
            _effectName = effect.DataName;
        }

        /// <summary>
        /// Starts a new effect on a target
        /// </summary>
        public void StartEffect(EffectType effect, IAttributeObject source, int identifier, int skill = -1)
        {
            _identifier = identifier;
            _skillSource = skill;
            
            if (effect == null)
                return;
                
            SetEffect(effect);
            
            // Start the timer based on effect duration
            _lifeTime = effect.GetDuration(0, source) * 1000;
            
            _attackerId = -1;
            _sourceCastRating = 10.0f;
            
            if (source != null)
            {
                _attackerId = source.GetIdNumber();
                _sourceCastRating = source.GetAbility(Attributes.Ability.CastRating);
            }
        }

        /// <summary>
        /// Updates the effect and applies its ongoing effects to the target
        /// </summary>
        /// <param name="target">Target affected by this effect</param>
        /// <param name="diffTime">Time elapsed in milliseconds</param>
        /// <returns>True if effect should continue, false if it should be removed</returns>
        public bool RunEffect(IAttributeObject target, int diffTime)
        {
            if (target == null || _effect == null)
                return false;
                
            // Decrease timer
            _lifeTime -= diffTime;
            
            if (_lifeTime <= 0)
                return false;
                
            _seconds += diffTime;
            
            // Process effect based on type
            switch (_effect.Type)
            {
                case (int)EffectMechanismType.Poison:
                    if (_seconds >= 4000)  // Poison deals damage every 4 seconds
                    {
                        _seconds -= 4000;
                        target.HarmAttributeByEffect(
                            _effect.GetParameter(0, _sourceCastRating) * 4.0f,
                            0,
                            _attackerId,
                            _effect);
                    }
                    break;
                    
                case (int)EffectMechanismType.Light:
                    // Light effects are passive and don't need periodic updates
                    break;
                    
                case (int)EffectMechanismType.Regenerate:
                    if (_seconds >= 4000)  // Regeneration occurs every 4 seconds
                    {
                        _seconds -= 4000;
                        target.RestoreAttributeByEffect(
                            _effect.GetParameter(1, _sourceCastRating) * 4.0f,
                            (int)_effect.GetParameter(0),
                            _attackerId,
                            _effect);
                    }
                    break;
            }
            
            return true;
        }

        /// <summary>
        /// Gets the source ID of the entity that applied this effect
        /// </summary>
        public int GetSourceID()
        {
            return _attackerId;
        }

        /// <summary>
        /// Gets the remaining life time in milliseconds
        /// </summary>
        public int GetRemainingLifeMillis()
        {
            return _lifeTime;
        }
    }
}