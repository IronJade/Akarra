/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class defines spell and item effects. Is used by the effect editor.
 **/

#ifndef _CEFFECT_H_
#define _CEFFECT_H_

#include "../library/Basic/CCharString.h"

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{
	class IAttributeObject;

	enum EEffectFileVersion
	{
		EFV_ORIGINAL = 0,

		EFV_021013,			// added difficulty
		EFV_021015,			// added added mana cost
		EFV_021108,			// added target types and intelligence strength
		EFV_021110,			// added requirements for targets
		EFV_021127,			// removed u_crstrength and replaced it with ua_parammulti[3], u_radiusinc and u_rangeinc
		EFV_021127b,		// added projectile and explosion information + u_designnotes
		EFV_021128,			// added u_defense
		EFV_030215,			// added u_flags, ua_dataname/ua_displayname, u_actiontime, u_recoverytime
		EFV_030330,			// added u_icon
		EFV_030407,			// added u_effectid
		EFV_030520,			// speed rebalance

		EFV_CURRENT
	};

	const int EFFECT_NAMELENGTH = 32;

	enum ETargetTypes
	{
		TT_SELF = 0,	// the caster
		TT_ENEMIES,
		TT_ALLIES,

		NUM_TARGET_TYPES
	};

	// effects have a special kind of requirement structure
	struct SEffectRequirement
	{
		int maintype;
		int subtype;
		int subvalue;
	};

	enum EEffectRequirementTypes
	{
		ERT_DISABLED = 0,	// this effect has no requirements to take effect

		ERT_REQUIREMENT,	// normal SRequirement, where subtype is type and subvalue is value
		ERT_EQUIPMENT,		// subtype is which item maintype and subvalue is which item subtype it should be (such as "the target must have a IMT_WEAPON with the subtype ITS_W_SMALLAXE")
		ERT_FLYING,			// target must be flying
		ERT_ETHEREAL,		// target must be ethereal

		NUM_EFFECTREQ_TYPES
	};

	// 021122: Reworked the effect system (again)... Is now based on an interface and then
	// extended depending on effect type:
	// CEffect_Impact - Effects used for weapon and monster hits.
	// CEffect_Spell - Effects used for monster spells
	//

	// *************************************************************************************
	class CEffectType
	{
		public:

			CEffectType();
			~CEffectType();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			// helper functions:

			int getDuration(float castrating = 0, IAttributeObject* caster = NULL);

			bool isApplicable(IAttributeObject* target);
			bool isProtected(IAttributeObject* target, IAttributeObject* source);
			bool isRebounded(IAttributeObject* target, IAttributeObject* source);

			int getRange(float castrating = 0, IAttributeObject* caster = NULL);
			int getRadius(float castrating = 0, IAttributeObject* caster = NULL);
			float getParameter(int number, float castrating = 0, IAttributeObject* caster = NULL);

			bool testSpellSuccess(float castrating = 0, IAttributeObject* caster = NULL);

			bool hasProjectile();
			bool hasExplosion();

			void copyOther(CEffectType* effect);

		protected:

			void m_clear();

		public:

			char	ua_dataname[EFFECT_NAMELENGTH];
			char	ua_displayname[EFFECT_NAMELENGTH];

			int		u_type;				// EEffectMainType 
			int		u_alignment;		// EEffectAlignment
			int		u_color;			// EEffectColor
			int		u_save;				// EEffectSavingThrowTypes
			int		u_effectid;			// used by items and monsters when determing if a effect can be
										// stacked on a target (default is -1, which means it can be stacked)

			int		ua_targets[NUM_TARGET_TYPES];	// sets how the effect affects different kinds of targets
			SEffectRequirement	u_req;				// sets if the target can be affected by this effect

			int		u_range;			// 0 is on self, 1 is by touch, >1 is ranged
			int		u_radius;			// <2 is a single target, 2 is a 3x3 square, >2 is a circular shape
			int		u_flags;
			int		u_actiontime;		// actiontime is used to delay spellcasting for players
			int		u_recoverytime;

			float	u_basepower;		// this is the cast rating of the effect if used from an item
			float	u_mpcost;			// how much magic points that is required to use the effect
			float	ua_params[3];		// parameters, see below
			char	ua_stringparam[32];	// string parameter (for summon effects etc)

			// difficulties and tests

			float	u_difficulty;		// a multiplier that is added to the caster's castrating (default is 1.0f)
			float	u_defense;			// the "protection value" the caster must beat to take effect

			// cr improvements:

			float	ua_parammulti[3];	// this value is multiplied with the caster's CR and added ontop of the parameters (used as damage bonus)
			int		u_rangeinc;			// if non-zero, the range is increased by 1 for every (CR/u_rangeinc) the caster has
			int		u_radiusinc;		// if non-zero, the radius is increased by 1 for every (CR/u_radiusinc) the caster has

			// visuals...

			int		u_icon;				// icon that is displayed for "runable" effects
			int		u_projtype;			// 0 if disabled
			int		u_projsprite;
			int		u_projsound;
			int		u_exptype;			// 0 if disabled
			int		u_expsprite;
			int		u_expsound;

			// other

			Basic::CCharString	u_designnotes;
	};

	const int	EFFECTFLAG_STAMINADRAIN		= 0x1;	// this effect drains stamina instead of magic points

	const int	EFFECT_BASE_ID				= 0x08000000;	// used to avoid conflicts with skill id numbers


	enum EEffectAlignment
	{
		EA_OFFENSIVE = 0,	// damage-based spells
		EA_DEFENSIVE,		// boosting/healing spells
		EA_NEUTRAL,			// light/scry/identify item etc
		EA_OTHER,

		NUM_EFFECTALIGNMENTS
	};

	// this is the main spell type, each type use the parameters differently
	enum EEffectMainType
	{
		EMT_UNKNOWN = 0,

		EMT_HEAL,			// [0 = hp, 1 = mp, 2 = sp], [min amount], [max amount]
		EMT_HARM,			// [0 = hp, 1 = mp, 2 = sp], [min amount], [max amount]

		EMT_CUREPOISON,		// [duration amount decreased]
		EMT_POISON,			// [dmg/sec], [duration]

		EMT_CUREDISEASE,	// 
		EMT_DISEASE,		// 

		EMT_BOOST,			// [0 = attr, 1 = abi, 2 = var], [amount], [duration]
		EMT_WEAKEN,			// [0 = attr, 1 = abi, 2 = var], [amount], [duration]

		EMT_LIGHT,			// [strength], [duration], [non-zero if only affects target]
		EMT_IDENITIFYITEM,	// [max item level]

		EMT_SUMMON,
		EMT_TELEPORT,

		EMT_REGENERATE,		// [hp/mp/sp], [regain/sec], [duration]
		EMT_NIGHTMARE,		// [duration] (makes the character unable to rest)

		EMT_TRANSPARENTDAMAGE,	// "transparent" damage booster on weapons, will increase the damage by a percentage if the target (not the weilder) fulfills the effect's requirements

		EMT_BOOSTPERCENT,	// same as EMT_BOOST, but boosts with a percentage instead (0.1 is +10%)
		EMT_WEAKENPERCENT,	// yet again...

		EMT_IMPROVEASPEED,	// increases attack speed [milliseconds], [duration]
		EMT_REDUCEASPEED,	// decreases attack speed
		EMT_SPEEDPERCENT,	// modifies movement speed [+ multiplier], [duration] (note: remember to set as "offensive" if the multiplier decreases speed, -0.1 for example (decreases by 10%))

		EMT_REWARDEXPERIENCE,		// adds [amount] experience to the target (note: never use negative exp values!)

		NUM_EFFECTTYPES
	};

	// note: this one should match all other colors in the game (as in CSkill.h)
	enum EEffectColor
	{
		EC_WHITE = 0,
		EC_RED,
		EC_GREEN,
		EC_BLUE,
		EC_YELLOW,

		NUM_EFFECT_COLORS
	};

	enum EEffectSavingThrowTypes
	{
		EST_NONE = 0,		// always maximum effect
		EST_DECREASE,		// the spell always hits, but has different amounts of efficiency
		EST_NEGATE,			// the spell either has max effect or no effect at all

		NUM_EFFECT_SAVINGTHROWS
	};

	// these are the different styles the ETargetTypes may have. For example, a fireball
	// as TTS_NORMAL on TT_SELF and TT_ENEMIES, but TTS_REBOUNDS on TT_ALLIES. A sphere-shaped
	// healing spell could have TTS_NORMAL on TT_SELF and TT_ALLIES, but TTS_NOEFFECT on
	// TT_ENEMIES, for example.
	enum ETargetTypeStyle
	{
		TTS_NORMAL = 0,		// works normally on this target
		TTS_NOEFFECT,		// this effect cant be cast upon this target
		TTS_REBOUNDS,		// rebounds on the caster

		NUM_TARGET_TYPE_STYLES
	};
}

#endif
