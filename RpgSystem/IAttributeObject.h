/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A super-class for player characters and monster entities.
 **/

#ifndef _IATTRIBUTEOBJECT_H_
#define _IATTRIBUTEOBJECT_H_

namespace RpgSystem
{

	// This interface is intended to keep a uniform syntax for both player characters and
	// monsters. The interface is also a very useful way to get basic stats and other
	// information.

	class CEffectType;
	struct SRequirement;

	class IAttributeObject
	{
		public:

			// Should return the objects current ability value
			virtual float getAbility(int ability, int special = 0) = 0;

			// Should evaluate the damagetype and modify the amount of damage given (if any)
			// and return the real amount of damage given
			virtual float dealDamage(float damage, int damagetype, int hittype, int attacker) = 0;

			// ranged attacks have three "distance steps" with different chances for success...
			// return a value between 0 and 1
			//virtual float getNearAccuracy() = 0;
			//virtual float getMediumAccuracy() = 0;
			//virtual float getFarAccuracy() = 0;
			virtual float getDistanceAccuracy(float distance) = 0;
			
			// Should return the object's chance for critical hit
			virtual float getCriticalValue() = 0;

			// Should return the object's chance for critical fumble
			virtual float getFumbleValue() = 0;

			// This is used to get special attack info from the attacker
			virtual int getAttackFlags(IAttributeObject* target) = 0;

			// KHADDED
			// Should decrease the attacker's fatigue based on the weapon he's weilding
			virtual void addAttackFatigue(float multiplier, bool miss) = 0;

			// Should decrease the target's fatigue based on the armor he's carrying
			virtual void addDefenseFatigue(float multiplier, bool miss) = 0;
			// !KHADDED

			// Should return true if the object still can fight and be attacked
			virtual bool isAlive() = 0;
			virtual bool isHurt() = 0;
			virtual float getHealth() = 0;
			virtual float getCurVar(int var) = 0;	// getCurVar(0) is same as getHealth()
			virtual float getMaxVar(int var) = 0;
			virtual void friendIsAttacked(int attackerid, bool forcesocial) = 0;

			virtual void getCoordinates(int& x, int& y) = 0;
			virtual int getIdNumber() = 0;

			// Effects:

			virtual void placeEffect(RpgSystem::CEffectType* effect, float castrating, int caster, int skillid) = 0;
			virtual void restoreAttributeByEffect(float amount, int attribute, int caster, RpgSystem::CEffectType* effect) = 0;
			virtual void harmAttributeByEffect(float amount, int attribute, int attacker, RpgSystem::CEffectType* effect) = 0;

			virtual int getTeam() = 0;
			
			// requirement tests

			virtual bool isHostile(int team) = 0;
			virtual bool testRequirement(SRequirement& req) = 0;
			virtual bool testEquipSlot(int slot, int itemsubtype) = 0;

			virtual bool isFlying() = 0;
			virtual bool isEthereal() = 0;
	};

	// special getAbility() settings
	enum EGetAbilitySettings
	{
		GAS_NONE = 0,

		GAS_SHIELDONLY,		// used when we only want the armor class for the shield
		GAS_ARMORONLY,		// same, but for armor (ignores shield and agility bonus)

		NUM_GETABILITY_SETTINGS
	};

};

#endif
