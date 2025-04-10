/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A character effect is a spell effect which has been placed on an entity
 *    and needs to be updated every second.
 **/

#include "CChrEffect.h"
#include "IAttributeObject.h"
#include "CEffect.h"
#include "../PlayerSystem/AkarraCharacter.h"
#include "../library/basic/ccharstring.h"
#include "../library/FileSystem/CFileStream.h"

//#ifndef NULL
//#define NULL ((void*)0)
//#endif

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChrEffect::CChrEffect()
{
	m_attackergid = -1;
	mp_effect = NULL;
	ma_effectname[0] = 0;
	m_life = 0;
	m_seconds = 0;
	m_skillsource = -1;
	m_identifier = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChrEffect::~CChrEffect()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function is only used for player characters (not monsters). The version is the
// CAkarraCharacter file version.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChrEffect::read(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	file.read(ma_effectname, sizeof(char), 32);
	file.read(&m_life, sizeof(int), 1);
	file.read(&m_seconds, sizeof(int), 1);
	if (version > Player::CHRFV_030301)
	{
		file.read(&m_identifier, sizeof(int), 1);
		file.read(&m_skillsource, sizeof(int), 1);
		file.read(&m_sourceCR, sizeof(float), 1);
	}

	m_attackergid = -1;
	mp_effect = NULL;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function is only used for player characters (not monsters).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChrEffect::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	file.write(ma_effectname, sizeof(char), 32);
	file.write(&m_life, sizeof(int), 1);
	file.write(&m_seconds, sizeof(int), 1);
	file.write(&m_identifier, sizeof(int), 1);
	file.write(&m_skillsource, sizeof(int), 1);
	file.write(&m_sourceCR, sizeof(float), 1);

	// note: no need to save the gid since it's illegal anyway when we reload

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CChrEffect::getID()
{
	return m_identifier;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function is used to test for stacking abilities for the effects. Effects placed on
// players by monsters, and certain stackable skills, return false and usually have their
// skill id numbers set to -1. Otherwise this function only returns true if the submitted
// id is the same as the current effect. If the function returns true it means that this
// effect should be removed.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChrEffect::isOverruled(int skill)
{
	return ((m_skillsource >= 0) && (m_skillsource == skill));
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function returns true if the effect that is associated with the runable affects
// the character's statistics, such as attack rating and resistances. The point with the
// function is to detect if the server needs to recalculate the character's stats when a
// runable is added or removed.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChrEffect::affectsStats()
{
	if (!mp_effect)
	{
		return false;
	}

	switch (mp_effect->u_type)
	{
		case EMT_BOOST:
		case EMT_WEAKEN:
		case EMT_BOOSTPERCENT:
		case EMT_WEAKENPERCENT:
		case EMT_IMPROVEASPEED:
		case EMT_REDUCEASPEED:
		case EMT_SPEEDPERCENT:
			return true;
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEffectType* CChrEffect::getEffect()
{
	return mp_effect;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Since the effects are modified depending on the caster's cast rating, we must remember
// and use the correct value when we calculate the importance of the effect.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CChrEffect::getCastRating()
{
	return m_sourceCR;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CChrEffect::getEffectName()
{
	return ma_effectname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChrEffect::setEffect(CEffectType* effect)
{
	if (!effect)
	{
		return;
	}

	mp_effect = effect;

	// copy the effect's name
	strcpy(ma_effectname, effect->ua_dataname);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChrEffect::startEffect(CEffectType* effect, IAttributeObject* source, int identifier, int skill)
{
	m_identifier = identifier;
	m_skillsource = skill;
	
	if (!effect)
	{
		return;
	}
	setEffect(effect);

	// start the timer, if this effect has a duration value
	m_life = effect->getDuration(0, source);
	// since we count in milliseconds, we need to increase the value
	m_life *= 1000;

	m_attackergid = -1;
	m_sourceCR = 10.0f;
	if (source)
	{
		m_attackergid = source->getIdNumber();
		m_sourceCR = source->getAbility(ABILITY_CAST_RATING);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function decreases the effect's timer and checks if it's time to perform any kind
// of action. It returns false when it should be removed from the character.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChrEffect::runEffect(IAttributeObject* target, int difftime)
{
	if (!target || !mp_effect)
	{
		return false;
	}

	// decrease timer
	m_life -= difftime;

	if (m_life <= 0)
	{
		return false;
	}

	m_seconds += difftime;

	// perform actions
	switch (mp_effect->u_type)
	{
		case EMT_POISON:
			if (m_seconds >= 4000)	// poison deal damage every 4 seconds
			{
				m_seconds -= 4000;
				target->harmAttributeByEffect(mp_effect->getParameter(0, m_sourceCR) * 4.0f, 0, m_attackergid, mp_effect);
			}			
			break;

		case EMT_LIGHT:
			break;

		case EMT_REGENERATE:
			if (m_seconds >= 4000)
			{
				m_seconds -= 4000;
				target->restoreAttributeByEffect(mp_effect->getParameter(1, m_sourceCR) * 4.0f, (int)mp_effect->getParameter(0), m_attackergid, mp_effect);
			}			
			break;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CChrEffect::getSourceID()
{
	return m_attackergid;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CChrEffect::getRemainingLifeMillis()
{
	return m_life;
}

}
