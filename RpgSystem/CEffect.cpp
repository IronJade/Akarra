/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class defines spell and item effects. Is used by the effect editor.
 **/

#include "CEffect.h"
#include "IAttributeObject.h"
#include "SRequirement.h"
#include "../library/Filesystem/CFileStream.h"
#include "../library/Filesystem/FileSystem.h"

#ifndef AKARRASERVER
#include <stdlib.h>
#endif

namespace RpgSystem
{


CEffectType::CEffectType()
{
	m_clear();
}

CEffectType::~CEffectType()
{
	removeAll();
}

void CEffectType::m_clear()
{
	ua_dataname[0] = 0;
	ua_displayname[0] = 0;

	u_type = 0;
	u_alignment = 0;
	u_color = 0;
	u_save = 0;
	u_effectid = -1;

	u_range = 0;
	u_radius = 0;
	u_basepower = 0;
	u_difficulty = 1.0f;
	u_mpcost = 0;
	ua_params[0] = 0;
	ua_params[1] = 0;
	ua_params[2] = 0;
	ua_stringparam[0] = 0;
	u_flags = 0;
	u_actiontime = 500;
	u_recoverytime = 500;

	u_defense = 0;

	ua_parammulti[0] = 0.0f;
	ua_parammulti[1] = 0.0f;
	ua_parammulti[2] = 0.0f;

	u_rangeinc = 0;
	u_radiusinc = 0;

	ua_targets[TT_SELF] = TTS_NORMAL;
	ua_targets[TT_ENEMIES] = TTS_NORMAL;
	ua_targets[TT_ALLIES] = TTS_NORMAL;

	u_req.maintype = ERT_DISABLED;
	u_req.subtype = 0;
	u_req.subvalue = 0;

	u_icon = 0;
	u_projtype = 0;
	u_projsprite = 0;
	u_projsound = 0;
	u_exptype = 0;
	u_expsprite = 0;
	u_expsound = 0;

	u_designnotes = "";
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEffectType::copyOther(CEffectType* effect)
{
	strcpy(ua_dataname, effect->ua_dataname);
	strcpy(ua_displayname, effect->ua_displayname);

	u_type = effect->u_type;
	u_alignment = effect->u_alignment;
	u_color = effect->u_color;
	u_save = effect->u_save;
	u_effectid = effect->u_effectid;

	u_range = effect->u_range;
	u_radius = effect->u_radius;
	u_basepower = effect->u_basepower;
	u_difficulty = effect->u_difficulty;
	u_mpcost = effect->u_mpcost;
	ua_params[0] = effect->ua_params[0];
	ua_params[1] = effect->ua_params[1];
	ua_params[2] = effect->ua_params[2];
	strcpy(ua_stringparam, effect->ua_stringparam);
	u_flags = effect->u_flags;
	u_actiontime = effect->u_actiontime;
	u_recoverytime = effect->u_recoverytime;

	u_defense = effect->u_defense;

	ua_parammulti[0] = effect->ua_parammulti[0];
	ua_parammulti[1] = effect->ua_parammulti[1];
	ua_parammulti[2] = effect->ua_parammulti[2];

	u_rangeinc = effect->u_rangeinc;
	u_radiusinc = effect->u_radiusinc;

	ua_targets[TT_SELF] = effect->ua_targets[TT_SELF];
	ua_targets[TT_ENEMIES] = effect->ua_targets[TT_ENEMIES];
	ua_targets[TT_ALLIES] = effect->ua_targets[TT_ALLIES];

	u_req.maintype = effect->u_req.maintype;
	u_req.subtype = effect->u_req.subtype;
	u_req.subvalue = effect->u_req.subvalue;

	u_icon = effect->u_icon;
	u_projtype = effect->u_projtype;
	u_projsprite = effect->u_projsprite;
	u_projsound = effect->u_projsound;
	u_exptype = effect->u_exptype;
	u_expsprite = effect->u_expsprite;
	u_expsound = effect->u_expsound;

	u_designnotes = effect->u_designnotes;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEffectType::removeAll()
{
}

bool CEffectType::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}
	m_clear();

	// read version
	int version;
	file.read(&version, sizeof(int), 1);

	file.read(ua_dataname, sizeof(char), 32);
	if (version > EFV_030215)
	{
		file.read(ua_displayname, sizeof(char), 32);
	}
	else
	{
		strcpy(ua_displayname, ua_dataname);
	}

	file.read(&u_type, sizeof(int), 1);
	file.read(&u_alignment, sizeof(int), 1);
	file.read(&u_color, sizeof(int), 1);
	file.read(&u_save, sizeof(int), 1);

	if (version > EFV_021108)
	{
		file.read(&ua_targets[TT_SELF], sizeof(int), 1);
		file.read(&ua_targets[TT_ENEMIES], sizeof(int), 1);
		file.read(&ua_targets[TT_ALLIES], sizeof(int), 1);
	}

	if (version > EFV_021110)
	{
		file.read(&u_req.maintype, sizeof(int), 1);
		file.read(&u_req.subtype, sizeof(int), 1);
		file.read(&u_req.subvalue, sizeof(int), 1);
	}

	file.read(&u_range, sizeof(int), 1);
	file.read(&u_radius, sizeof(int), 1);
	if (version > EFV_030215)
	{
		file.read(&u_flags, sizeof(int), 1);
		file.read(&u_actiontime, sizeof(int), 1);
		file.read(&u_recoverytime, sizeof(int), 1);
	}

	file.read(&u_basepower, sizeof(float), 1);
	if (version > EFV_021013)
	{
		file.read(&u_difficulty, sizeof(float), 1);
	}
	if (version > EFV_021108)
	{
		if (version > EFV_021127)
		{
			file.read(&ua_parammulti[0], sizeof(float), 1);
			file.read(&ua_parammulti[1], sizeof(float), 1);
			file.read(&ua_parammulti[2], sizeof(float), 1);
			file.read(&u_rangeinc, sizeof(int), 1);
			file.read(&u_radiusinc, sizeof(int), 1);
		}
		else
		{
			float temp;
			file.read(&temp, sizeof(float), 1);
			ua_parammulti[0] = temp;
			ua_parammulti[1] = temp;
			ua_parammulti[2] = temp;

			u_rangeinc = 0;
			u_radiusinc = 0;
		}
	}

	if (version > EFV_021015)
	{
		file.read(&u_mpcost, sizeof(float), 1);
	}

	file.read(&ua_params[0], sizeof(float), 1);
	file.read(&ua_params[1], sizeof(float), 1);
	file.read(&ua_params[2], sizeof(float), 1);
	file.read(ua_stringparam, sizeof(char), 32);

	if (version > EFV_030330)
	{
		file.read(&u_icon, sizeof(int), 1);
	}
	if (version > EFV_021127b)
	{
		file.read(&u_projtype, sizeof(int), 1);
		file.read(&u_projsprite, sizeof(int), 1);
		file.read(&u_projsound, sizeof(int), 1);
		file.read(&u_exptype, sizeof(int), 1);
		file.read(&u_expsprite, sizeof(int), 1);
		file.read(&u_expsound, sizeof(int), 1);

		u_designnotes = FileSystem::readStringWithLength(file);
	}
	
	if (version > EFV_021128)
	{
		file.read(&u_defense, sizeof(float), 1);
	}

	if (version > EFV_030407)
	{
		file.read(&u_effectid, sizeof(int), 1);
	}

	if (version <= EFV_030520)
	{
		u_recoverytime = (int)((float)u_recoverytime * 0.70f);
	}
	
	return true;
	
}

bool CEffectType::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write version
	int version = EFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(ua_dataname, sizeof(char), 32);
	file.write(ua_displayname, sizeof(char), 32);

	file.write(&u_type, sizeof(int), 1);
	file.write(&u_alignment, sizeof(int), 1);
	file.write(&u_color, sizeof(int), 1);
	file.write(&u_save, sizeof(int), 1);

	file.write(&ua_targets[TT_SELF], sizeof(int), 1);
	file.write(&ua_targets[TT_ENEMIES], sizeof(int), 1);
	file.write(&ua_targets[TT_ALLIES], sizeof(int), 1);

	file.write(&u_req.maintype, sizeof(int), 1);
	file.write(&u_req.subtype, sizeof(int), 1);
	file.write(&u_req.subvalue, sizeof(int), 1);

	file.write(&u_range, sizeof(int), 1);
	file.write(&u_radius, sizeof(int), 1);
	file.write(&u_flags, sizeof(int), 1);
	file.write(&u_actiontime, sizeof(int), 1);
	file.write(&u_recoverytime, sizeof(int), 1);

	file.write(&u_basepower, sizeof(float), 1);
	file.write(&u_difficulty, sizeof(float), 1);

	file.write(&ua_parammulti[0], sizeof(float), 1);
	file.write(&ua_parammulti[1], sizeof(float), 1);
	file.write(&ua_parammulti[2], sizeof(float), 1);
	file.write(&u_rangeinc, sizeof(int), 1);
	file.write(&u_radiusinc, sizeof(int), 1);

	file.write(&u_mpcost, sizeof(float), 1);

	file.write(&ua_params[0], sizeof(float), 1);
	file.write(&ua_params[1], sizeof(float), 1);
	file.write(&ua_params[2], sizeof(float), 1);
	file.write(ua_stringparam, sizeof(char), 32);

	file.write(&u_icon, sizeof(int), 1);
	file.write(&u_projtype, sizeof(int), 1);
	file.write(&u_projsprite, sizeof(int), 1);
	file.write(&u_projsound, sizeof(int), 1);
	file.write(&u_exptype, sizeof(int), 1);
	file.write(&u_expsprite, sizeof(int), 1);
	file.write(&u_expsound, sizeof(int), 1);

	FileSystem::writeStringWithLength(file, u_designnotes);

	file.write(&u_defense, sizeof(float), 1);

	file.write(&u_effectid, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CEffectType::getDuration(float castrating, IAttributeObject* caster)
{
	switch (u_type)
	{
		default:
			return 0;

		case EMT_IMPROVEASPEED:
		case EMT_REDUCEASPEED:
		case EMT_SPEEDPERCENT:
		case EMT_POISON:
		case EMT_LIGHT:
			return (int)(getParameter(1, castrating, caster) + 0.5f);

		case EMT_NIGHTMARE:
			return (int)(getParameter(0, castrating, caster) + 0.5f);

		case EMT_REGENERATE:
		case EMT_BOOST:
		case EMT_BOOSTPERCENT:
		case EMT_WEAKENPERCENT:
		case EMT_WEAKEN:
			return (int)(getParameter(2, castrating, caster) + 0.5f);

	}

	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectType::isApplicable(IAttributeObject* target)
{
	if (!target)
	{
		return false;
	}

	// check if there is a requirement on this effect
	if (u_req.maintype == ERT_DISABLED)
	{
		// nope
		return true;
	}

	switch (u_req.maintype)
	{
		case ERT_REQUIREMENT:
			{
				SRequirement req;
				req.type = u_req.subtype;
				req.value = u_req.subvalue;

				return target->testRequirement(req);
			}
	
		case ERT_EQUIPMENT:
			{
				return target->testEquipSlot(u_req.subtype, u_req.subvalue);
			}

		case ERT_FLYING:
			{
				return target->isFlying();
			}
		case ERT_ETHEREAL:
			{
				return target->isEthereal();
			}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectType::isProtected(IAttributeObject* target, IAttributeObject* source)
{
	if (!target || !source)
	{
		return false;
	}

	if (target == source)
	{
		if (ua_targets[TT_SELF] == TTS_NOEFFECT)
		{
			return true;
		}
	}

	if (source->isHostile(target->getTeam()))
	{
		if (ua_targets[TT_ENEMIES] == TTS_NOEFFECT)
		{
			return true;
		}
	}

	else if (ua_targets[TT_ALLIES] == TTS_NOEFFECT)
	{
		return true;
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectType::isRebounded(IAttributeObject* target, IAttributeObject* source)
{
	if (!target || !source)
	{
		return false;
	}

	if (target == source)
	{
		// effects can't rebounce on yourself
		return false;
	}

	if (source->isHostile(target->getTeam()))
	{
		if (ua_targets[TT_ENEMIES] == TTS_REBOUNDS)
		{
			return true;
		}
	}

	else if (ua_targets[TT_ALLIES] == TTS_REBOUNDS)
	{
		return true;
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CEffectType::getRange(float castrating, IAttributeObject* caster)
{
	if ((castrating > 0) && (u_rangeinc > 0))
	{
		return (u_range + ((int)(castrating + 0.5f)/ u_rangeinc));
	}

	return u_range;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CEffectType::getRadius(float castrating, IAttributeObject* caster)
{
	if ((castrating > 0) && (u_radiusinc > 0))
	{
		return (u_radius + ((int)(castrating + 0.5f) / u_radiusinc));
	}

	return u_radius;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CEffectType::getParameter(int number, float castrating, IAttributeObject* caster)
{
	if (caster)
	{
		castrating = caster->getAbility(ABILITY_CAST_RATING);
	}
	if (castrating > 0)
	{
		return (ua_params[number] + (castrating * ua_parammulti[number]));
	}

	return ua_params[number];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function is used to test if the caster is able to use the spell. The cast rating
// is compared against the spell's "defense value", which works in a similar fashion as
// an attack roll (AR vs AC). Remember that if the spell succeeds, the spell might still
// need to be tested against the target's resistance values according to the spell's
// save type.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectType::testSpellSuccess(float castrating, IAttributeObject* caster)
{
	if (u_defense < 1)
	{
		return true;
	}

	castrating *= u_difficulty;

	// calculate the different values
	float upper = (castrating * castrating);
	float lower = upper + (1.75f * u_defense * u_defense);
	float ratio = (upper / lower);

	// the chance to hit is never below 5%
	if (ratio < 0.05f)
	{
		ratio = 0.05f;
	}

	// randomize a value between 0 and 1 and compare it to our ratio
	float roll = ((float)(rand()%1000) * 0.001f);

	return (roll <= ratio);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectType::hasProjectile()
{
	return (u_projtype > 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEffectType::hasExplosion()
{
	return (u_exptype > 0);
}

}
