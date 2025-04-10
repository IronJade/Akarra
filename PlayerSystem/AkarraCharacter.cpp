/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The player's character file.
 **/

#include "AkarraCharacter.h"
#include "../RpgSystem/CEffect.h"
#include "../RpgSystem/CChrEffect.h"
#include "../RpgSystem/CSkill.h"

using namespace RpgSystem;

namespace Player
{

CCharacterFile::CCharacterFile()
{
	m_clear();
}

CCharacterFile::~CCharacterFile()
{
	removeAll();
}

void CCharacterFile::m_clear()
{
	u_name[0] = 0;
	ua_accountname[0] = 0;
	u_globalx = 0;
	u_globaly = 0;
	u_steps = 0;
	u_onlinetime = 0;
	u_level50time = 0;
	u_flags = 0;
	u_muteseconds = 0;

	u_respawnx = CHARACTER_START_X;
	u_respawny = CHARACTER_START_Y;

	u_bodypose = 0;
	u_skincolor = 0;
	u_eyecolor = 0;
	u_haircolor = 0;
	u_hairstyle = 0;

	u_guild.guildname[0] = 0;
	u_guild.state = CGS_INVALID;
	u_lastguildmessageread = 0;

	u_nexteffectid = 1;
}

void CCharacterFile::removeAll()
{
	u_attribs.removeAll();
	u_inventory.removeAll();
	u_quests.removeAll();
	m_clear();
}

bool CCharacterFile::open(FileSystem::CFileStream& file)
{
	// read the file version
	int version;
	file.read(&version, sizeof(int), 1);

	// read the attributes first
	if (!u_attribs.read(file, version))
	{
		return false;
	}
	// now read the inventory
	if (!u_inventory.read(file, version))
	{
		return false;
	}
	if (!u_quests.read(file, version))
	{
		return false;
	}

	// read the name
	file.read(u_name, sizeof(char), CHARACTER_NAMELENGTH);
	// read the world position
	file.read(&u_globalx, sizeof(int), 1);
	file.read(&u_globaly, sizeof(int), 1);
	file.read(&u_steps, sizeof(int), 1);
	file.read(&u_onlinetime, sizeof(int), 1);

	if (version > CHRFV_020505)
	{
		file.read(&u_flags, sizeof(int), 1);
	}
	if (version > CHRFV_020530)
	{
		file.read(&u_level50time, sizeof(int), 1);
	}
	if (version > CHRFV_030118)
	{
		file.read(&u_muteseconds, sizeof(int), 1);
	}

	if (version > CHRFV_020629)
	{
		file.read(&u_respawnx, sizeof(int), 1);
		file.read(&u_respawny, sizeof(int), 1);
	}

	if (version > CHRFV_021002)
	{
		file.read(&u_bodypose, sizeof(int), 1);
		file.read(&u_skincolor, sizeof(int), 1);
		file.read(&u_haircolor, sizeof(int), 1);
		file.read(&u_hairstyle, sizeof(int), 1);
	}
	if (version > CHRFV_021003)
	{
		file.read(&u_eyecolor, sizeof(int), 1);
	}

	if (version > CHRFV_021009)
	{
		file.read(ua_accountname, sizeof(char), 16);
	}

	if (version > CHRFV_021208)
	{
		file.read(u_guild.guildname, sizeof(char), 32);
		file.read(&u_guild.state, sizeof(int), 1);
	}
	if (version > CHRFV_030506)
	{
		file.read(&u_lastguildmessageread, sizeof(long), 1);
	}

	if (version > CHRFV_030301)
	{
		file.read(&u_nexteffectid, sizeof(int), 1);
	}

	// in A1 there was a major attribute reset for all characters...
	if (version <= CHRFV_030506)
	{
		resetCharacterAttributes();
	}

	//recalculateAll();

	return true;
}

bool CCharacterFile::write(FileSystem::CFileStream& file)
{
	// store the file version
	int version(CHARACTER_FILEVERSION);
	file.write(&version, sizeof(int), 1);

	// save the attributes first
	if (!u_attribs.write(file, CHARACTER_FILEVERSION))
	{
		return false;
	}
	// save inventory
	if (!u_inventory.write(file))
	{
		return false;
	}

	if (!u_quests.write(file))
	{
		return false;
	}

	// save the name
	file.write(u_name, sizeof(char), CHARACTER_NAMELENGTH);
	// save the world position
	file.write(&u_globalx, sizeof(int), 1);
	file.write(&u_globaly, sizeof(int), 1);
	file.write(&u_steps, sizeof(int), 1);
	file.write(&u_onlinetime, sizeof(int), 1);

	file.write(&u_flags, sizeof(int), 1);

	file.write(&u_level50time, sizeof(int), 1);
	file.write(&u_muteseconds, sizeof(int), 1);

	file.write(&u_respawnx, sizeof(int), 1);
	file.write(&u_respawny, sizeof(int), 1);

	file.write(&u_bodypose, sizeof(int), 1);
	file.write(&u_skincolor, sizeof(int), 1);
	file.write(&u_haircolor, sizeof(int), 1);
	file.write(&u_hairstyle, sizeof(int), 1);
	file.write(&u_eyecolor, sizeof(int), 1);

	file.write(ua_accountname, sizeof(char), 16);

	file.write(u_guild.guildname, sizeof(char), 32);
	file.write(&u_guild.state, sizeof(int), 1);
	file.write(&u_lastguildmessageread, sizeof(long), 1);

	file.write(&u_nexteffectid, sizeof(int), 1);

	return true;
}

const float CHARACTER_STARTING_ATTRIBUTES[NUM_RACES * NUM_ATTRIBUTES] =
{
	// humans
	15.0f, 15.0f, 15.0f, 15.0f, 15.0f,
	// menits
	23.0f, 21.0f, 18.0f, 18.0f, 20.0f,
	// iwids
	18.0f, 18.0f, 21.0f, 23.0f, 20.0f
};

// use this function to setup a completely fresh character to the starting
// values... this function assumes that god, gender and color already has
// been set
void CCharacterFile::setNewCharacter()
{
	{
		for (int i(0); i < (int)NUM_ATTRIBUTES; i++)
		{
			u_attribs.u_stats.attributes[i].setBase(
				CHARACTER_STARTING_ATTRIBUTES[i + (u_attribs.u_stats.race * NUM_ATTRIBUTES)]);
		}
	}
	/*
	if (u_attribs.u_stats.race == CR_MENIT)
	{
		u_attribs.u_stats.attributes[BA_STRENGTH].setBase(		23.0f);	// +8
		u_attribs.u_stats.attributes[BA_CONSTITUTION].setBase(	21.0f);	// +6
		u_attribs.u_stats.attributes[BA_AGILITY].setBase(		18.0f);	// +3
		u_attribs.u_stats.attributes[BA_INTELLIGENCE].setBase(	18.0f);	// +3
		u_attribs.u_stats.attributes[BA_FAITH].setBase(			20.0f);	// +5
	}
	else if (u_attribs.u_stats.race == RpgSystem::CR_IWID)
	{
		u_attribs.u_stats.attributes[BA_STRENGTH].setBase(		18.0f);	// +3
		u_attribs.u_stats.attributes[BA_CONSTITUTION].setBase(	18.0f);	// +3
		u_attribs.u_stats.attributes[BA_AGILITY].setBase(		21.0f);	// +6
		u_attribs.u_stats.attributes[BA_INTELLIGENCE].setBase(	23.0f);	// +8
		u_attribs.u_stats.attributes[BA_FAITH].setBase(			20.0f);	// +5
	}
	else // u_attribs.u_stats.race == CR_HUMAN
	{
		u_attribs.u_stats.attributes[BA_STRENGTH].setBase(		15.0f);
		u_attribs.u_stats.attributes[BA_CONSTITUTION].setBase(	15.0f);
		u_attribs.u_stats.attributes[BA_AGILITY].setBase(		15.0f);
		u_attribs.u_stats.attributes[BA_INTELLIGENCE].setBase(	15.0f);
		u_attribs.u_stats.attributes[BA_FAITH].setBase(			15.0f);
	}
	*/

	// start in hope:
	u_globalx = CHARACTER_START_X;
	u_globaly = CHARACTER_START_Y;
	u_respawnx = CHARACTER_START_X;
	u_respawny = CHARACTER_START_Y;

	// start with a stone of each color
	u_attribs.u_stats.freegems[0] = 1;
	u_attribs.u_stats.freegems[1] = 1;
	u_attribs.u_stats.freegems[2] = 1;
	u_attribs.u_stats.freegems[3] = 1;
	u_attribs.u_stats.freegems[4] = 1;
	u_attribs.u_stats.totalgems = 5;

	recalculateCharacterStats(NULL);
	for (int i = (0); i < NUM_VARATTRIBUTES; i++)
	{
		u_attribs.u_stats.variable[i].restore();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function will use the current base values of the five attributes to determine how
// many free attribute points the character should have.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterFile::resetCharacterAttributes()
{
	int attributes = u_attribs.u_stats.newpoints;
	{
		for (int i(0); i < (int)NUM_ATTRIBUTES; i++)
		{
			attributes += (int)(
				u_attribs.u_stats.attributes[i].getBase() -
				CHARACTER_STARTING_ATTRIBUTES[i + (u_attribs.u_stats.race * NUM_ATTRIBUTES)]
				);

			// reset attribute to starting value
			u_attribs.u_stats.attributes[i].setBase(
				CHARACTER_STARTING_ATTRIBUTES[i + (u_attribs.u_stats.race * NUM_ATTRIBUTES)]);
		}
	}
	u_attribs.u_stats.newpoints = attributes;
}

// recalculates all attribute bonuses, abilities, skills, spells
// items etc based on the current character state
// TODO: Take items, skills and spells into account
void CCharacterFile::recalculateCharacterStats(RpgSystem::IAttributeObject* source)
{
	RpgSystem::SBaseStats* stats = &u_attribs.u_stats;

	static float damagelist[NUM_VARATTRIBUTES];
	for (int i = (0); i < NUM_VARATTRIBUTES; i++)
	{
		damagelist[i] = stats->variable[i].getCurrent();
		damagelist[i] = stats->variable[i].getValue() - damagelist[i];
		stats->variable[i].resetBonus();
	}
	
	// reset attribute bonuses
	for (i = (0); i < NUM_ATTRIBUTES; i++)
	{
		stats->attributes[i].resetBonus();
	}
	for (i = (0); i < NUM_ABILITIES; i++)
	{
		stats->abilities[i].resetBonus();
	}
	stats->attackspeed = 0;
	stats->movespeedmultiplier = 1.0f;
	stats->personallight = 2;
	stats->publiclight = 0;

	// Calculate stats depending on equipped items
	// 1: Check weapon hand for weapon
	CItemInstance* righthand = u_inventory.upa_equipment[ES_RIGHTHAND];
	CItemInstance* lefthand = u_inventory.upa_equipment[ES_LEFTHAND];
	if (righthand && righthand->up_type && righthand->isEnabled())
	{
		// if the weapon is a bow and we don't have any ammo, we shouldn't add the damage (only confuses people)
		if (getWeaponAmmoType(righthand->up_type->u_subtype) < 0)
		{
			// melee weapon
			stats->abilities[ABILITY_DAMAGE_MIN].addBonus(righthand->up_type->u_param1);
			stats->abilities[ABILITY_DAMAGE_MAX].addBonus(righthand->up_type->u_param2);
		}
		else if (lefthand && lefthand->up_type && (lefthand->up_type->u_subtype == getWeaponAmmoType(righthand->up_type->u_subtype)))
		{
			// ranged weapon with correct ammo
			stats->abilities[ABILITY_DAMAGE_MIN].addBonus(righthand->up_type->u_param1);
			stats->abilities[ABILITY_DAMAGE_MAX].addBonus(righthand->up_type->u_param2);
		}

		if (righthand->up_type->u_flags & IFLAG_NOVICTIMEFFECT)
		{
			// this weapon has double wielder effects instead of a victim effect
			applyEffect(righthand->ua_effects[0], source);
		}

		applyEffect(righthand->ua_effects[1], source);
	}
	// 2: Check shield hand for shield
	bool strengthdamage = true;
	if (lefthand && lefthand->up_type && lefthand->isEnabled())
	{
		// the shield-hand sometimes doesn't carry a shield...
		if (lefthand->up_type->u_maintype == IMT_SHIELD)
		{
			stats->abilities[ABILITY_ARMORCLASS].addBonus(lefthand->up_type->u_param1);
			stats->abilities[ABILITY_REDUCTION].addBonus(lefthand->up_type->u_param2);
		}
		else if (lefthand->up_type->u_maintype == IMT_AMMO)
		{
			// ammo changes the damage value if a matching weapon is carried
			if (righthand && (lefthand->up_type->u_subtype == getWeaponAmmoType(righthand->up_type->u_subtype)))
			{
				stats->abilities[ABILITY_DAMAGE_MIN].addPercentBonus(lefthand->up_type->u_param2);
				stats->abilities[ABILITY_DAMAGE_MAX].addPercentBonus(lefthand->up_type->u_param2);
				// don't allow strength damage bonus
				strengthdamage = false;
			}
		}

		applyEffect(lefthand->ua_effects[0], source);
		applyEffect(lefthand->ua_effects[1], source);
	}
	// 3: Check body for armor
	if (u_inventory.upa_equipment[ES_BODY] && u_inventory.upa_equipment[ES_BODY]->up_type &&
		u_inventory.upa_equipment[ES_BODY]->isEnabled())
	{
		float value = u_inventory.upa_equipment[ES_BODY]->up_type->u_param1;
		stats->abilities[ABILITY_ARMORCLASS].addBonus(value);
		stats->abilities[ABILITY_REDUCTION].addBonus(u_inventory.upa_equipment[ES_BODY]->up_type->u_param2);

		applyEffect(u_inventory.upa_equipment[ES_BODY]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_BODY]->ua_effects[1], source);
	}
	// 4: Check head for helm
	if (u_inventory.upa_equipment[ES_HEAD] && u_inventory.upa_equipment[ES_HEAD]->up_type &&
		u_inventory.upa_equipment[ES_HEAD]->isEnabled())
	{
		float value = u_inventory.upa_equipment[ES_HEAD]->up_type->u_param1;
		stats->abilities[ABILITY_ARMORCLASS].addBonus(value);
		stats->abilities[ABILITY_REDUCTION].addBonus(u_inventory.upa_equipment[ES_HEAD]->up_type->u_param2);

		applyEffect(u_inventory.upa_equipment[ES_HEAD]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_HEAD]->ua_effects[1], source);
	}
	// 5: Check hands for gloves
	if (u_inventory.upa_equipment[ES_HANDS] && u_inventory.upa_equipment[ES_HANDS]->up_type &&
		u_inventory.upa_equipment[ES_HANDS]->isEnabled())
	{
		float value = u_inventory.upa_equipment[ES_HANDS]->up_type->u_param1;
		stats->abilities[ABILITY_ARMORCLASS].addBonus(value);
		stats->abilities[ABILITY_REDUCTION].addBonus(u_inventory.upa_equipment[ES_HANDS]->up_type->u_param2);

		applyEffect(u_inventory.upa_equipment[ES_HANDS]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_HANDS]->ua_effects[1], source);
	}
	// 6: Check feet for boots
	if (u_inventory.upa_equipment[ES_FEET] && u_inventory.upa_equipment[ES_FEET]->up_type &&
		u_inventory.upa_equipment[ES_FEET]->isEnabled())
	{
		float value = u_inventory.upa_equipment[ES_FEET]->up_type->u_param1;
		stats->abilities[ABILITY_ARMORCLASS].addBonus(value);
		stats->abilities[ABILITY_REDUCTION].addBonus(u_inventory.upa_equipment[ES_FEET]->up_type->u_param2);

		applyEffect(u_inventory.upa_equipment[ES_FEET]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_FEET]->ua_effects[1], source);
	}

	// check rings and necklace (effects only)
	if (u_inventory.upa_equipment[ES_NECK] && u_inventory.upa_equipment[ES_NECK]->up_type &&
		u_inventory.upa_equipment[ES_NECK]->isEnabled())
	{
		applyEffect(u_inventory.upa_equipment[ES_NECK]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_NECK]->ua_effects[1], source);
	}
	if (u_inventory.upa_equipment[ES_RRING] && u_inventory.upa_equipment[ES_RRING]->up_type &&
		u_inventory.upa_equipment[ES_RRING]->isEnabled())
	{
		applyEffect(u_inventory.upa_equipment[ES_RRING]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_RRING]->ua_effects[1], source);
	}
	if (u_inventory.upa_equipment[ES_LRING] && u_inventory.upa_equipment[ES_LRING]->up_type &&
		u_inventory.upa_equipment[ES_LRING]->isEnabled())
	{
		applyEffect(u_inventory.upa_equipment[ES_LRING]->ua_effects[0], source);
		applyEffect(u_inventory.upa_equipment[ES_LRING]->ua_effects[1], source);
	}

	// check all skill effects
	{
		for (int i(0); i < u_attribs.u_skills.size(); i++)
		{
			CSkill* skill = u_attribs.u_skills[i];

			if (!skill)
			{
				continue;
			}

			CSkillType2* type = skill->getSkillType();
			CEffectType* effect = skill->getEffectType();

			if (!type || !effect || type->getVariantType() != SV_TALENT)
			{
				// we only calculate talents here
				continue;
			}
			applyEffect(effect, source);
		}
	}

	// check all runable effects
	std::list<CChrEffect*>::iterator runeffect;
	for (runeffect = u_attribs.u_runeffects.begin(); runeffect != u_attribs.u_runeffects.end(); ++runeffect)
	{
		if (!(*runeffect)->getEffect())
		{
			continue;
		}
		switch ((*runeffect)->getEffect()->u_type)
		{
			case EMT_BOOST:
			case EMT_BOOSTPERCENT:
			case EMT_WEAKEN:
			case EMT_WEAKENPERCENT:
			case EMT_IMPROVEASPEED:
			case EMT_REDUCEASPEED:
			case EMT_SPEEDPERCENT:
			case EMT_LIGHT:
				applyEffect((*runeffect)->getEffect(), source, (*runeffect)->getCastRating());
				break;
		}
	}

	// 020809: In order for special attribute-boosting effects on the items we must calculate
	// the abilities after the items. Remember that the items should change the bonus, not the
	// base value.

	// 030112: Changed the strength bonus from a static to a percentage.
	
	// recalculate abilities
	stats->abilities[ABILITY_ATTACK_RATING].setBase(stats->attributes[BA_AGILITY].getValue() * 0.75f);
	//stats->abilities[ABILITY_CAST_RATING].setBase(stats->attributes[BA_INTELLIGENCE].getValue() * 0.75f);
	stats->abilities[ABILITY_CAST_RATING].setBase(stats->attributes[BA_INTELLIGENCE].getValue() * 0.40f);
	//stats->abilities[ABILITY_DAMAGE_MIN].setBase(stats->attributes[BA_STRENGTH].getValue() * 0.125f);
	//stats->abilities[ABILITY_DAMAGE_MAX].setBase((stats->attributes[BA_STRENGTH].getValue() * 0.15f) + 1.0f);
	stats->abilities[ABILITY_DAMAGE_MIN].setBase(1.5f + stats->attributes[BA_STRENGTH].getValue() * 0.02f);
	stats->abilities[ABILITY_DAMAGE_MAX].setBase(4.0f + stats->attributes[BA_STRENGTH].getValue() * 0.04f);
	if (strengthdamage)
	{
		stats->abilities[ABILITY_DAMAGE_MIN].addPercentBonus(stats->attributes[BA_STRENGTH].getValue() * 0.0050f);
		stats->abilities[ABILITY_DAMAGE_MAX].addPercentBonus(stats->attributes[BA_STRENGTH].getValue() * 0.0050f);
	}
	stats->abilities[ABILITY_ARMORCLASS].setBase(stats->attributes[BA_AGILITY].getValue() * 0.25f);
	stats->abilities[ABILITY_RESIST_WHITE].setBase(3.0f + stats->attributes[BA_FAITH].getValue() * 0.125f);
	stats->abilities[ABILITY_RESIST_RED].setBase(3.0f + stats->attributes[BA_CONSTITUTION].getValue() * 0.125f);
	stats->abilities[ABILITY_RESIST_GREEN].setBase(3.0f + stats->attributes[BA_INTELLIGENCE].getValue() * 0.125f);
	stats->abilities[ABILITY_RESIST_BLUE].setBase(3.0f + stats->attributes[BA_CONSTITUTION].getValue() * 0.125f);
	stats->abilities[ABILITY_RESIST_YELLOW].setBase(3.0f + stats->attributes[BA_CONSTITUTION].getValue() * 0.125f);
	stats->abilities[ABILITY_REDUCTION].setBase(0);

	// increase the magic resistance, that matches the character's soul color, by 10%
	stats->abilities[ABILITY_RESIST_WHITE + stats->soulcolor].addPercentBonus(0.1f);

	// recalculate variable attributes (remember the amount of damage, though)
	stats->variable[HA_HEALTH].setBase(stats->attributes[BA_CONSTITUTION].getValue() * 2.0f);
	stats->variable[HA_MANA].setBase(stats->attributes[BA_FAITH].getValue() * 1.5f);
	stats->variable[HA_STAMINA].setBase(stats->attributes[BA_CONSTITUTION].getValue() * 3.0f);

	
	// remove the damagevalue again
	for (i = (0); i < NUM_VARATTRIBUTES; i++)
	{
		stats->variable[i].restore();
		stats->variable[i].addToCurrent(-damagelist[i]);
	}

	// recalculate inventory capacities
	//u_inventory.calculateMaxCapacities(*stats);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterFile::applyEffect(RpgSystem::CEffectType* effect, RpgSystem::IAttributeObject* source, float castrating)
{
	if (!effect)
	{
		return;
	}
	if (!effect->isApplicable(source))
	{
		return;
	}

	// normal boost
	if (effect->u_type == EMT_BOOST)
	{
		int id = (int)effect->ua_params[0];
		
		if (id >= 0 && id <= 4)
		{
			// this is an attribute boost
			u_attribs.u_stats.attributes[id].addBonus(effect->getParameter(1, castrating));
		}
		else if (id >= 5 && id <= 15)
		{
			// this is an ability boost
			u_attribs.u_stats.abilities[id - 5].addBonus(effect->getParameter(1, castrating));
		}
		else if (id >= 16 && id <= 18)
		{
			// this is an variable (hp/mp/sp) boost
			u_attribs.u_stats.variable[id - 16].addBonus(effect->getParameter(1, castrating));
		}
	}
	// percentage boost
	else if (effect->u_type == EMT_BOOSTPERCENT)
	{
		int id = (int)effect->ua_params[0];
		
		if (id >= 0 && id <= 4)
		{
			// this is an attribute boost
			u_attribs.u_stats.attributes[id].addPercentBonus(effect->getParameter(1, castrating));
		}
		else if (id >= 5 && id <= 15)
		{
			// this is an ability boost
			u_attribs.u_stats.abilities[id - 5].addPercentBonus(effect->getParameter(1, castrating));
		}
		else if (id >= 16 && id <= 18)
		{
			// this is an variable (hp/mp/sp) boost
			u_attribs.u_stats.variable[id - 16].addPercentBonus(effect->getParameter(1, castrating));
		}
	}
	else if (effect->u_type == EMT_WEAKEN)
	{
		int id = (int)effect->ua_params[0];
		
		if (id >= 0 && id <= 4)
		{
			// this is an attribute boost
			u_attribs.u_stats.attributes[id].addBonus(-effect->getParameter(1, castrating));
		}
		else if (id >= 5 && id <= 15)
		{
			// this is an ability boost
			u_attribs.u_stats.abilities[id - 5].addBonus(-effect->getParameter(1, castrating));
		}
		else if (id >= 16 && id <= 18)
		{
			// this is an variable (hp/mp/sp) boost
			u_attribs.u_stats.variable[id - 16].addBonus(-effect->getParameter(1, castrating));
		}
	}
	// percentage boost
	else if (effect->u_type == EMT_WEAKENPERCENT)
	{
		int id = (int)effect->ua_params[0];
		
		if (id >= 0 && id <= 4)
		{
			// this is an attribute boost
			u_attribs.u_stats.attributes[id].addPercentBonus(-effect->getParameter(1, castrating));
		}
		else if (id >= 5 && id <= 15)
		{
			// this is an ability boost
			u_attribs.u_stats.abilities[id - 5].addPercentBonus(-effect->getParameter(1, castrating));
		}
		else if (id >= 16 && id <= 18)
		{
			// this is an variable (hp/mp/sp) boost
			u_attribs.u_stats.variable[id - 16].addPercentBonus(-effect->getParameter(1, castrating));
		}
	}
	// attack speed effects
	else if (effect->u_type == EMT_IMPROVEASPEED)
	{
		u_attribs.u_stats.attackspeed -= (int)effect->getParameter(0, castrating);
	}
	else if (effect->u_type == EMT_REDUCEASPEED)
	{
		u_attribs.u_stats.attackspeed += (int)effect->getParameter(0, castrating);
	}
	else if (effect->u_type == EMT_SPEEDPERCENT)
	{
		float multi = 1.0f - effect->getParameter(0, castrating);
		u_attribs.u_stats.movespeedmultiplier *= multi;
	}
	else if (effect->u_type == EMT_LIGHT)
	{
		// if parameter 3 is non-zero, this only affects the local player
		if ((int)effect->getParameter(2, castrating) == 0)
		{
			u_attribs.u_stats.publiclight += (char)effect->getParameter(0, castrating);
		}
		u_attribs.u_stats.personallight += (char)effect->getParameter(0, castrating);
	}
}

} // end of Player namespace
