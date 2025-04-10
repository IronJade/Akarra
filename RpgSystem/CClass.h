/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player character classes.
 **/

#ifndef _CCLASS_H_
#define _CCLASS_H_

#include "../library/basic/CCharString.h"

namespace RpgSystem
{
	class CCharacterAttributes;

	// class list, where each class has an unique number
	enum EClassList
	{
		CLASS_MUNDANE = 0,		// all level 0 characters

		CLASS_APPRENTICE,		// level 1-10 spell-caster
		CLASS_WIZARD,			// level 10-30
		CLASS_NECROMANCER,
		CLASS_BLACKREAPER,
		CLASS_LORDOFTHELICH,
		CLASS_MAGE,
		CLASS_SORCERER,
		CLASS_ELEMENTALIST,

		CLASS_NEOPHYTE,
		CLASS_CLERIC,
		CLASS_HEALER,
		CLASS_PRIEST,
		CLASS_RESTORER,
		CLASS_NATURALIST,
		CLASS_DRUID,
		CLASS_WARRIORMONK,

		CLASS_FIGHTER,
		CLASS_WARRIOR,
		CLASS_KNIGTH,
		CLASS_PALADIN,
		CLASS_COMMANDER,
		CLASS_WEAPONMASTER,
		CLASS_RANGER,
		CLASS_SWORDDANCER,
		CLASS_EXECUTIONER,
		CLASS_THUG,
		CLASS_SHADOW,
		CLASS_HIGHWAY,

		NUM_CLASSES			// must never be more than 32
	};

	// class list, where each class is a bit flag
	// NOTE: This list must ALWAYS match a (1 << i) loop list, where i goes through all the classes of EClassList
	enum EClassFlags
	{
		CF_MUNDANE =		0x00000001,		// mundane

		CF_APPRENTICE =		0x00000002,		// apprentice
		CF_WIZARD =			0x00000004,		// wizard
		CF_NECROMANCER =	0x00000008,		// necromancer
		CF_BLACKREAPER =	0x00000010,		// black reaper
		CF_LORDOFTHELICH =	0x00000020,		// lord of the lich
		CF_MAGE =			0x00000040,		// mage
		CF_SORCERER =		0x00000080,		// arcane sorcerer
		CF_ELEMENTALIST =	0x00000100,		// arch elementari

		CF_NEOPHYTE =		0x00000200,		// neophyte
		CF_CLERIC =			0x00000400,		// cleric
		CF_HEALER =			0x00000800,		// HEALER
		CF_PRIEST =			0x00001000,		// PRIEST
		CF_RESTORER =		0x00002000,		// RESTORER
		CF_NATURALIST =		0x00004000,		// monk
		CF_DRUID =			0x00008000,		// druid
		CF_WARRIORMONK =	0x00010000,		// WARRIORMONK

		CF_FIGHTER =		0x00020000,		// fighter
		CF_WARRIOR =		0x00040000,		// warrior
		CF_KNIGTH =			0x00080000,		// knight
		CF_PALADIN =		0x00100000,		// paladin
		CF_COMMANDER =		0x00200000,		// commander
		CF_WEAPONMASTER =	0x00400000,		// weapon master
		CF_RANGER =			0x00800000,		// ranger
		CF_SWORDDANCER =	0x01000000,		// sword-dancer
		CF_EXECUTIONER =	0x02000000,		// executioner
		CF_THUG =			0x04000000,		// thug
		CF_SHADOWFIGHTER =	0x08000000,		// shadowfighter
		CF_HIGHWAY =		0x10000000,		// HIGHWAY
	};

	// some group flags

	// all warrior-classes (including paladin and thug branches)
	const unsigned int CLASSGROUPFLAG_WARRIORS		= CF_FIGHTER | CF_WARRIOR | CF_KNIGTH | CF_PALADIN |
													CF_COMMANDER | CF_WEAPONMASTER | CF_RANGER | CF_SWORDDANCER |
													CF_EXECUTIONER | CF_THUG | CF_SHADOWFIGHTER | CF_HIGHWAY;
	// the paladin branch
	const unsigned int CLASSGROUPFLAG_PALADINS		= CF_KNIGTH | CF_PALADIN | CF_COMMANDER;

	// the weapon-master branch
	const unsigned int CLASSGROUPFLAG_WEAPONMASTERS	= CF_WEAPONMASTER | CF_RANGER | CF_SWORDDANCER | CF_EXECUTIONER;
	// thugs
	const unsigned int CLASSGROUPFLAG_THUGS			= CF_THUG | CF_SHADOWFIGHTER | CF_HIGHWAY;

	// all cleric-classes
	const unsigned int CLASSGROUPFLAG_CLERICS		= CF_NEOPHYTE | CF_CLERIC | CF_HEALER | CF_DRUID |
													CF_PRIEST | CF_RESTORER | CF_NATURALIST | CF_WARRIORMONK;
	// HEALERs
	const unsigned int CLASSGROUPFLAG_HEALERS		= CF_HEALER | CF_PRIEST | CF_RESTORER;
	// monks
	const unsigned int CLASSGROUPFLAG_MONKS			= CF_NATURALIST | CF_DRUID | CF_WARRIORMONK;

	// all spell-casters
	const unsigned int CLASSGROUPFLAG_WIZARDS		= CF_APPRENTICE | CF_WIZARD | CF_NECROMANCER | CF_BLACKREAPER |
													CF_LORDOFTHELICH | CF_MAGE | CF_SORCERER | CF_ELEMENTALIST;
	// necromancers
	const unsigned int CLASSGROUPFLAG_NECROMANCERS	= CF_NECROMANCER | CF_BLACKREAPER | CF_LORDOFTHELICH;
	// mages and sorcerers
	const unsigned int CLASSGROUPFLAG_MAGES			= CF_MAGE | CF_SORCERER | CF_ELEMENTALIST;

	const unsigned int CLASSGROUPFLAG_NOTUSED		= ~(CLASSGROUPFLAG_WARRIORS | CLASSGROUPFLAG_WIZARDS | CLASSGROUPFLAG_CLERICS);
	const unsigned int CLASSGROUPFLAG_ALL			= 0xffffffff & ~CLASSGROUPFLAG_NOTUSED;

	// static class functions

	char* getClassName(int classnum, unsigned char gender);
	char* getClassDescription(int classnum);
	Basic::CCharString getClassGroup(int classflag, unsigned char gender);

	// requirement checks:

	bool classIsAvailableFromClass(int destclass, int sourceclass);
	bool testClassRequirements(int destclass, CCharacterAttributes& attribs, Basic::CCharString& failreason);

} // end of RpgSystem namespace

#endif
