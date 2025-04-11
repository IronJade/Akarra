using System;
using System.IO;

namespace RpgSystem.Effects
{
    /// <summary>
    /// A character effect is a spell effect which has been placed on an entity
    /// and needs to be updated regularly. Effects can be temporary buffs, debuffs,
    /// regeneration, poison, etc.
    /// </summary>
    public class CharacterEffect
    {
        // Fields
        private int _attackerId;       // ID of the entity that caused this effect
        private EffectType _effect;    // The effect definition
        private string _effectName;    // Name of the effect (for serialization)
        private int _lifeTime;         // Remaining time in milliseconds
        private int _elapsedTime;      // Time counter in milliseconds
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
        /// Reads effect data from a binary stream (used for player characters)
        /// </summary>
        public bool Read(BinaryReader reader, int version)
        {
            _effectName = new string(reader.ReadChars(32)).TrimEnd('\0');
            _lifeTime = reader.ReadInt32();
            _elapsedTime = reader.ReadInt32();

            if (version > PlayerFileVersions.V_030301)
            {
                _identifier = reader.ReadInt32();
                _skillSource = reader.ReadInt32();
                _sourceCastRating = reader.ReadSingle();
            }

            _attackerId = -1;
            _effect = null;

            return true;
        }

        /// <summary>
        /// Writes effect data to a binary stream (used for player characters)
        /// </summary>
        public bool Write(BinaryWriter writer)
        {
            // Write effect name
            char[] nameChars = new char[32];
            for (int i = 0; i < Math.Min(_effectName.Length, 32); i++)
            {
                nameChars[i] = _effectName[i];
            }
            writer.Write(nameChars);

            writer.Write(_lifeTime);
            writer.Write(_elapsedTime);
            writer.Write(_identifier);
            writer.Write(_skillSource);
            writer.Write(_sourceCastRating);

            return true;
        }

        /// <summary>
        /// Returns the ID of this effect
        /// </summary>
        public int GetId()
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

            // Set the timer based on effect duration
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
        /// Processes the effect over time
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

            // Perform actions based on effect type
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
                    // Light effects don't need periodic updates
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

    /// <summary>
    /// Types of effect mechanisms
    /// </summary>
    public enum EffectMechanismType
    {
        Unknown = 0,

        Heal,           // [0 = hp, 1 = mp, 2 = sp], [min amount], [max amount]
        Harm,           // [0 = hp, 1 = mp, 2 = sp], [min amount], [max amount]

        CurePoison,     // [duration amount decreased]
        Poison,         // [dmg/sec], [duration]

        CureDisease,    // 
        Disease,        // 

        Boost,          // [0 = attr, 1 = abi, 2 = var], [amount], [duration]
        Weaken,         // [0 = attr, 1 = abi, 2 = var], [amount], [duration]

        Light,          // [strength], [duration], [non-zero if only affects target]
        IdentifyItem,   // [max item level]

        Summon,
        Teleport,

        Regenerate,     // [hp/mp/sp], [regain/sec], [duration]
        Nightmare,      // [duration] (makes the character unable to rest)

        TransparentDamage, // "transparent" damage booster on weapons

        BoostPercent,   // same as Boost, but uses percentage instead
        WeakenPercent,  // same as Weaken, but uses percentage instead

        ImproveAttackSpeed, // increases attack speed [milliseconds], [duration]
        ReduceAttackSpeed,  // decreases attack speed
        SpeedPercent,       // modifies movement speed [+ multiplier], [duration]

        RewardExperience     // adds [amount] experience to the target
    }
}
