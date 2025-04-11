using RpgSystem.Attributes;
using RpgSystem.Combat;
using RpgSystem.Effects;

namespace RpgSystem.Entities
{
    /// <summary>
    /// Interface that defines an object that has attributes and abilities,
    /// such as a player character or monster.
    /// </summary>
    public interface IAttributeObject
    {
        /// <summary>
        /// Gets the value of a specific ability.
        /// </summary>
        /// <param name="ability">The ability to retrieve</param>
        /// <param name="special">Special calculation settings</param>
        /// <returns>The ability value</returns>
        float GetAbility(Ability ability, int special = 0);

        /// <summary>
        /// Applies damage to this object and returns the actual damage dealt.
        /// </summary>
        float DealDamage(float damage, int damageType, int hitType, int attackerId);

        /// <summary>
        /// Gets accuracy based on distance for ranged attacks.
        /// </summary>
        float GetDistanceAccuracy(float distance);

        /// <summary>
        /// Gets the chance for critical hits.
        /// </summary>
        float GetCriticalValue();

        /// <summary>
        /// Gets the chance for fumbles.
        /// </summary>
        float GetFumbleValue();

        /// <summary>
        /// Gets special attack flags for this attacker against the target.
        /// </summary>
        int GetAttackFlags(IAttributeObject target);

        /// <summary>
        /// Decreases the attacker's fatigue based on weapon being used.
        /// </summary>
        void AddAttackFatigue(float multiplier, bool miss);

        /// <summary>
        /// Decreases the target's fatigue based on armor being worn.
        /// </summary>
        void AddDefenseFatigue(float multiplier, bool miss);

        /// <summary>
        /// Determines if this object is alive and can fight.
        /// </summary>
        bool IsAlive();

        /// <summary>
        /// Determines if this object has taken damage.
        /// </summary>
        bool IsHurt();

        /// <summary>
        /// Gets the current health value.
        /// </summary>
        float GetHealth();

        /// <summary>
        /// Gets the current value of a variable attribute.
        /// </summary>
        float GetCurVar(int varAttribute);

        /// <summary>
        /// Gets the maximum value of a variable attribute.
        /// </summary>
        float GetMaxVar(int varAttribute);

        /// <summary>
        /// Called when a friend of this object is attacked.
        /// </summary>
        void FriendIsAttacked(int attackerId, bool forceSocial);

        /// <summary>
        /// Gets the coordinates of this object in the world.
        /// </summary>
        void GetCoordinates(out int x, out int y);

        /// <summary>
        /// Gets the unique ID number of this object.
        /// </summary>
        int GetIdNumber();

        /// <summary>
        /// Places an effect on this object.
        /// </summary>
        void PlaceEffect(EffectType effect, float castRating, int caster, int skillId);

        /// <summary>
        /// Restores an attribute by an effect (healing).
        /// </summary>
        void RestoreAttributeByEffect(float amount, int attribute, int caster, EffectType effect);

        /// <summary>
        /// Harms an attribute by an effect (damage).
        /// </summary>
        void HarmAttributeByEffect(float amount, int attribute, int attacker, EffectType effect);

        /// <summary>
        /// Gets the team this entity belongs to.
        /// </summary>
        int GetTeam();

        /// <summary>
        /// Determines if another team is hostile to this object.
        /// </summary>
        bool IsHostile(int team);

        /// <summary>
        /// Tests if this object meets a requirement.
        /// </summary>
        bool TestRequirement(Requirement req);

        /// <summary>
        /// Tests if this object has an equipment slot with a specific item type.
        /// </summary>
        bool TestEquipSlot(int slot, int itemSubtype);

        /// <summary>
        /// Determines if this object can fly.
        /// </summary>
        bool IsFlying();

        /// <summary>
        /// Determines if this object is ethereal.
        /// </summary>
        bool IsEthereal();
    }
}