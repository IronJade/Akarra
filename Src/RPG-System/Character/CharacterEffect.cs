using System;
using System.IO;
using RpgSystem.Effects;
using RpgSystem.Entities;

namespace RpgSystem.Character
{
    /// <summary>
    /// A character effect is a temporary status effect applied to an entity
    /// such as a buff, debuff, regeneration, poison, etc.
    /// </summary>
    public class CharacterEffect
    {
        // Effect identification
        private int _identifier;           // Unique identifier for this effect instance
        private int _skillSource;          // ID of skill that created this effect (-1 for no skill)
        
        // Effect data
        private EffectType _effect;        // Reference to effect type
        private string _effectName;        // Name of effect (for serialization)
        
        // Timing
        private int _lifeTime;             // Remaining life time in milliseconds
        private int _elapsedTime;          // Time counter in milliseconds
        
        // Source information
        private int _attackerId;           // ID of entity that applied this effect
        private float _sourceCastRating;   // Cast rating when effect was applied
        
        /// <summary>
        /// Gets the ID of this effect instance
        /// </summary>
        public int Id => _identifier;
        
        /// <summary>
        /// Gets the name of this effect
        /// </summary>
        public string EffectName => _effectName;
        
        /// <summary>
        /// Gets the cast rating used when this effect was applied
        /// </summary>
        public float CastRating => _sourceCastRating;
        
        /// <summary>
        /// Gets the entity ID of the effect source
        /// </summary>
        public int SourceId => _attackerId;
        
        /// <summary>
        /// Gets the remaining life time in milliseconds
        /// </summary>
        public int RemainingLifeMillis => _lifeTime;
        
        /// <summary>
        /// Gets the effect type
        /// </summary>
        public EffectType Effect => _effect;

        /// <summary>
        /// Creates a new character effect
        /// </summary>
        public CharacterEffect()
        {
            _attackerId = -1;
            _effect = null;
            _effectName = string.Empty;
            _lifeTime = 0;
            _elapsedTime = 0;
            _skillSource = -1;
            _sourceCastRating = 0;
            _identifier = 0;
        }

        /// <summary>
        /// Reads effect data from a binary stream (for player character persistence)
        /// </summary>
        public bool Read(BinaryReader reader, int version)
        {
            try
            {
                // Read effect name (32 chars)
                _effectName = new string(reader.ReadChars(32)).TrimEnd('\0');
                
                // Read timers
                _lifeTime = reader.ReadInt32();
                _elapsedTime = reader.ReadInt32();
                
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
        /// Writes effect data to a binary stream (for player character persistence)
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
                writer.Write(_elapsedTime);
                
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
        public int GetId()
        {
            return _identifier;
        }

        /// <summary>
        /// Checks if this effect is overruled by a new effect with the same skill source
        /// </summary>
        /// <param name="skill">Skill ID to check against</param>
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
        /// <param name="deltaTime">Time elapsed in milliseconds</param>
        /// <returns>True if effect should continue, false if it should be removed</returns>
        public bool RunEffect(IAttributeObject target, int deltaTime)
        {
            if (target == null || _effect == null)
                return false;
                
            // Decrease timer
            _lifeTime -= deltaTime;
            
            if (_lifeTime <= 0)
                return false;
                
            _elapsedTime += deltaTime;
            
            // Process effect based on type
            switch (_effect.Type)
            {
                case (int)EffectMechanismType.Poison:
                    if (_elapsedTime >= 4000)  // Poison deals damage every 4 seconds
                    {
                        _elapsedTime -= 4000;
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
                    if (_elapsedTime >= 4000)  // Regeneration occurs every 4 seconds
                    {
                        _elapsedTime -= 4000;
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
    }
}