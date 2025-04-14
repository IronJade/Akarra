using System;
using RpgSystem.Effects;
using RpgSystem.Combat;

namespace RpgSystem.Entities
{
    /// <summary>
    /// Interface that defines an object that has attributes and abilities,
    /// such as a player character, NPC, or monster.
    /// </summary>
    public interface IAttributeObject
    {
        /// <summary>
        /// Gets the value of a specific ability
        /// </summary>
        /// <param name="ability">The ability to retrieve</param>
        /// <param name="special">Special calculation settings (0=normal, 1=shield only, 2=armor only)</param>
        /// <returns>The ability value</returns>
        float GetAbility(Attributes.Ability ability, int special = 0);
        
        /// <summary>
        /// Applies damage to this object from another entity
        /// </summary>
        /// <param name="damage">Amount of damage to apply</param>
        /// <param name="damageType">Type of damage (physical, magical, etc.)</param>
        /// <param name="hitType">Type of hit (normal, critical, etc.)</param>
        /// <param name="attackerId">ID of the attacker</param>
        /// <returns>Actual damage dealt after reductions</returns>
        float DealDamage(float damage, int damageType, int hitType, int attackerId);
        
        /// <summary>
        /// Applies harm to a specific attribute from an effect
        /// </summary>
        /// <param name="amount">Amount of harm to apply</param>
        /// <param name="attribute">Attribute to harm (0=health, 1=mana, 2=stamina)</param>
        /// <param name="attackerId">ID of the attacker</param>
        /// <param name="effect">Effect causing the harm</param>
        void HarmAttributeByEffect(float amount, int attribute, int attackerId, EffectType effect);
        
        /// <summary>
        /// Restores a specific attribute from an effect
        /// </summary>
        /// <param name="amount">Amount to restore</param>
        /// <param name="attribute">Attribute to restore (0=health, 1=mana, 2=stamina)</param>
        /// <param name="casterId">ID of the caster</param>
        /// <param name="effect">Effect causing the restoration</param>
        void RestoreAttributeByEffect(float amount, int attribute, int casterId, EffectType effect);
        
        /// <summary>
        /// Places an effect on this object
        /// </summary>
        /// <param name="effect">Effect type to apply</param>
        /// <param name="source">Source of the effect</param>
        /// <param name="identifier">Unique ID for this effect instance</param>
        /// <param name="skillId">ID of the skill that created this effect (-1 for no skill)</param>
        void PlaceEffect(EffectType effect, IAttributeObject source, int identifier, int skillId = -1);
        
        /// <summary>
        /// Gets the unique ID of this entity
        /// </summary>
        int GetIdNumber();
        
        /// <summary>
        /// Gets the team this entity belongs to
        /// </summary>
        int GetTeam();
        
        /// <summary>
        /// Checks if another team is hostile to this entity
        /// </summary>
        /// <param name="team">Team to check</param>
        /// <returns>True if hostile, false if friendly</returns>
        bool IsHostile(int team);
        
        /// <summary>
        /// Tests if this entity meets a requirement
        /// </summary>
        /// <param name="requirement">Requirement to test</param>
        /// <returns>True if requirement is met, false otherwise</returns>
        bool TestRequirement(Attributes.Requirement requirement);
        
        /// <summary>
        /// Tests if this entity has a specific equipment type in a slot
        /// </summary>
        /// <param name="mainType">Main type of equipment</param>
        /// <param name="subType">Sub-type of equipment</param>
        /// <returns>True if equipped, false otherwise</returns>
        bool TestEquipSlot(int mainType, int subType);
        
        /// <summary>
        /// Checks if this entity can fly
        /// </summary>
        bool IsFlying();
        
        /// <summary>
        /// Checks if this entity is ethereal (can pass through certain objects)
        /// </summary>
        bool IsEthereal();
        
        /// <summary>
        /// Gets the world coordinates of this entity
        /// </summary>
        /// <param name="x">X coordinate output</param>
        /// <param name="y">Y coordinate output</param>
        void GetCoordinates(out int x, out int y);
        
        /// <summary>
        /// Gets the accuracy at a specific distance
        /// </summary>
        /// <param name="distance">Distance in grid cells</param>
        /// <returns>Accuracy as a percentage (0.0 to 1.0)</returns>
        float GetDistanceAccuracy(int distance);
        
        /// <summary>
        /// Gets the chance for critical hits
        /// </summary>
        /// <returns>Critical hit chance as a percentage (0.0 to 1.0)</returns>
        float GetCriticalValue();
        
        /// <summary>
        /// Gets the chance for fumbles
        /// </summary>
        /// <returns>Fumble chance as a percentage (0.0 to 1.0)</returns>
        float GetFumbleValue();
        
        /// <summary>
        /// Gets special attack flags for attacking a specific target
        /// </summary>
        /// <param name="target">Target being attacked</param>
        /// <returns>Attack flags (bitfield)</returns>
        int GetAttackFlags(IAttributeObject target);
        
        /// <summary>
        /// Checks if this entity is alive
        /// </summary>
        /// <returns>True if alive, false if dead</returns>
        bool IsAlive();
        
        /// <summary>
        /// Checks if this entity is hurt (not at full health)
        /// </summary>
        /// <returns>True if hurt, false if at full health</returns>
        bool IsHurt();
        
        /// <summary>
        /// Gets the current health value
        /// </summary>
        /// <returns>Current health</returns>
        float GetHealth();
        
        /// <summary>
        /// Gets the current value of a variable attribute
        /// </summary>
        /// <param name="varAttr">Variable attribute to get</param>
        /// <returns>Current value of the attribute</returns>
        float GetCurrentVar(Attributes.VariableAttributeType varAttr);
        
        /// <summary>
        /// Gets the maximum value of a variable attribute
        /// </summary>
        /// <param name="varAttr">Variable attribute to get</param>
        /// <returns>Maximum value of the attribute</returns>
        float GetMaxVar(Attributes.VariableAttributeType varAttr);
        
        /// <summary>
        /// Applies fatigue when attacking
        /// </summary>
        /// <param name="multiplier">Stamina cost multiplier</param>
        /// <param name="miss">True if the attack missed</param>
        void ApplyAttackFatigue(float multiplier, bool miss);
        
        /// <summary>
        /// Applies fatigue when defending
        /// </summary>
        /// <param name="multiplier">Stamina cost multiplier</param>
        /// <param name="miss">True if the attack missed</param>
        void ApplyDefenseFatigue(float multiplier, bool miss);
        
        /// <summary>
        /// Notifies this entity that a friend was attacked
        /// </summary>
        /// <param name="attackerId">ID of the attacker</param>
        /// <param name="forceSocial">True to force social response</param>
        void FriendAttacked(int attackerId, bool forceSocial);
    }
}