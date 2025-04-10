/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player character classes.
 **/

#include "CClass.h"
#include "SRequirement.h"

namespace RpgSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

const char* CLASS_NAMES_MALE[NUM_CLASSES] =
{
	// Male names
	"Mundane",

	"Apprentice",
	"Wizard",
	"Necromancer",
	"Black Reaper",
	"Lord of the Lich",
	"Mage",
	"Sorcerer",
	"Elementalist",

	"Neophyte",
	"Cleric",
	"Healer",
	"Priest",
	"Restorer",
	"Naturalist",
	"Druid",
	"Shaman",
	
	"Fighter",
	"Warrior",
	"Knight",
	"Paladin",
	"Commander",
	"Weapon Master",
	"Ranger",
	"Sword-Dancer",
	"Executioner",
	"Thug",
	"Shadow",
	"Unknown"
};

const char* CLASS_NAMES_FEMALE[NUM_CLASSES] =
{
	// Female names
	"Mundane",

	"Apprentice",
	"Wizard",
	"Necromancer",
	"Black Reaper",
	"Lady of the Lich",
	"Mage",
	"Sorceress",
	"Elementalist",

	"Neophyte",
	"Cleric",
	"Healer",
	"Priestess",
	"Restorer",
	"Naturalist",
	"Druid",
	"Shaman",
	
	"Fighter",
	"Warrior",
	"Knight",
	"Paladin",
	"Commander",
	"Weapon Master",
	"Ranger",
	"Sword-Dancer",
	"Executioner",
	"Thug",
	"Shadow",
	"Unknown"
};

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// class descriptions (with color codes)

const char* CLASS_DESCRIPTIONS[NUM_CLASSES] =
{
	//"Mundane",
	"All the people of Akarra begin their journey as commoners, living their lives in "
	"peace, performing everyday duties. The mundane know a little of everything, but "
	"need to specialize if they want to become more advanced in an art.§n§n"
	"Key skills: Fighting I, Dogde I, Magic Use I§n§n"
	"Requirements: None§n§n"
	"Further advancement: Fighter, Neophyte, Apprentice§n§n",

	//"Apprentice",
	"A mundane who has advanced to the status of an §3Apprentice§7 has taken the first "
	"step towards becomming a powerful wizard. The apprentice will have to give up some "
	"of his, or her, physical power in order to control the mystical energies of magic.§n§n"
	"Key skills: Magic Arrow, Flare, Magic Use II§n§n"
	"Requirements: Magic Use I§n§n"
	"Further advancement: Wizard§n§n",
	//"Wizard",
	"Sooner or later the apprentice will know enough of the magical arts to advance to the "
	"next tier, §3Wizard§7. A wizard has a wider range of spells and can use them more "
	"efficiently.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 25, Flare§n§n"
	"§7Further advancement: Mage, Necromancer§n§n",
	//"Necromancer",
	"Some wizards choose to specialize in the darker arts of magic and become a §3Necromancer§7."
	" Necromancers have focused their powers on controlling the dead and bring them back to an "
	"unholy afterlife. They also use their powers to inflict unhealthy curses and diseases upon "
	"their victims.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Lord of the Lich, Black Reaper§n§n",
	//"Black Reaper",
	"Some necromancers fall so deeply in their black magic that they are transformed to their "
	"own creation. A §3Black Reaper§7 is a foul abomination of a creature who is half man, half "
	"wraith. The black reapers become ethereal and can move through walls and obstacles, at the "
	"price of physical strength. The black reapers also lose the ability to use most of the "
	"magical powers they knew during their human life.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Lord of the Lich",
	"The greatest of necromancers become Lords, or Ladies, of their craft. These dark wizards "
	"can not only summon minions from the dead, they can also invoke the powers of greater "
	"unholiness to cause destruction for those who oppose them.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Mage",
	"Some wizards choose to continue with their studies and advance to the wisdom of a §3Mage§7. "
	"Mages know more spells than wizards, of both elemental, enchanting and summoning kinds.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Elementalist, Sorcerer§n§n",
	//"Sorcerer",
	"When a mage specializes in the enchanting and summoning crafts, he will become a §3Sorcerer§7. "
	"Sorcerers have very powerful spells that can completely transform a creature or item. They also "
	"have the powers to create something out of nothing, using this for both good and evil.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Elementalist",
	"The most forceful of all wizards are the elementalists. They have focused all their skills on "
	"creating as great physical powers as possible. An elementalist can create flamestorms, blizzards "
	"and other massive means of destruction. Elementalists play with fire.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",

	//"Neophyte",
	"Some mundane people choose the path of faith and knowledge, seeking to find a way to "
	"reach their spiritual guidance. These become neophytes, who focuses their knowledge on "
	"prayers, healing and protective skills, thinking more of others than of themselves.§n§n"
	"Key skills: Cure Wounds, Protection Spells§n§n"
	"Requirements: Curse Resistance§n§n"
	"§7Further advancement: Cleric§n§n",
	//"Cleric",
	"As their skills improve, neophytes will advance to the status of a §3Cleric§7. Clerics "
	"have learnt more about the healing and protective powers and is able to use these to a "
	"greater extent.§n§n"
	"Key skills: Revitalize, Cure Poison§n§n"
	"Requirements: Level 25, Cure Wounds§n§n"
	"§7Further advancement: Healer, Naturalist§n§n",
	//"Healer",
	"A §3Healer§7 has further improved his knowledge of the healing and restoring crafts. "
	"The healer has also found a path to reach higher divinity, and perhaps even a chance "
	"to reach clairvoyance.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Priest, Restorer§n§n",
	//"Priest",
	"This is the ultimate goal for those who once became neophytes for religious purposes. "
	"A §3Priest§7 is the greatest of the spiritual leaders and will guide the masses to come closer "
	"to §3Benedek§7 or §3Ambus§7, whichever the priest prefers.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Restorer",
	"Instead of seeking the ultimate faith, some Healers choose to continue helping others and become "
	"§3Restorers§7. These are men and women who has specialized in healing and restoring the wounded, "
	"ill or even killed people of Akarra.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Naturalist",
	"Some clerics choose to find their answers among the woods, on the seas or in the plains of the world. "
	"They become §3Naturalists§7 and have a sense for the lives of the animals and plants, which they use "
	"to perform good.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Druid, Warrior Monk§n§n",
	//"Druid",
	"Naturalists who wants to specialize in their knowledge of the naturelife become §3Druids§7. Druids "
	"know how to understand, invoke and control the powers of nature, and use this to favor themselves and "
	"those they care for.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Warrior Monk",
	//"Some naturalists finds a way back to the path of the faithful and become §3Warrior Monks§7. These "
	//"are wandering priests who use their healing and nature powers in combination with new knowledge of "
	//"combat arts.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	
	//"Fighter",
	"§3Fighters§7 are people who has focused on brute force and weaponry to survive in the world. "
	"A fighter relies on his arms and armor, but will be able to learn more sophisticated tactics as "
	"they learn more skills.§n§n"
	"Key skills: Fighting II, Armor Use I§n§n"
	"Requirements: Fighting I§n§n"
	"§7Further advancement: Warrior§n§n",
	//"Warrior",
	"§3Warriors§7 were once brutal fighters but have improved their skills and fight with more "
	"cunning and finesse. A trained warrior is a fearsome foe in close combat.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 25, Toughness§n§n"
	"§7Further advancement: Weapon Master, Knight, Thug§n§n",
	//"Knight",
	"§3Knights§7 are expert warriors, weilding weapons and carrying heavy armors. Knights are also "
	"skilled at horseback fighting and has knowledge of leadership. Knights usually are, but not "
	"necessarily, very noble combatants.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Paladin, Commander§n§n",
	//"Paladin",
	"Some knights choose to follow their faith and pray heavily to §3Benedek§7 or §3Ambus§7, "
	"seeking out the divine part of the world. These knights become §3Paladins§7 and are favored "
	"with healing and celestial spells and skills.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Commander",
	"A §3Commander§7 is a knight of great leadership and tactical cunning. They enforce the "
	"abilities of all they lead by just displaying great courage and wisdom. Commanders are "
	"skilled in leading others in fellowships and other gatherings.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Weapon Master",
	"A warrior who specializes in weapon combat becomes a weapon master. These are exceptional "
	"figthers, especially when they are weilding a weapon of choice.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Ranger, Sword-Dancer, Executioner§n§n",
	//"Ranger",
	"§3Rangers§7 are weapon masters who prefer their bow and arrow above all other weapons. "
	"They are skilled foresters and how to track and how to hunt. Their close connection to "
	"nature also allows them to use certain spells.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Sword-Dancer",
	"Weapon masters who prefer agility and blades become §3Sword-dancers§7. These fighters "
	"are quick and dexterious, and are able to weild two weapons at once. Their favored weapon "
	"is the sword.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Executioner",
	"Those warriors who enjoys bruteness and force above all becomes §3Executioners§7, always "
	"weilding heavy two-handed weapons to destroy their foes. Executioners have skills and "
	"tricks which are specialized in the purpose to deal a lot of damage.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Thug",
	"Some warriors fall off the noble path and choose to survive using cheap tricks and foul "
	"play. These become §3Thugs§7 and learn to backstab, sneak, evade and use other unorthodox "
	"methods of fighting.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 50§n§n"
	"§7Further advancement: Shadow, Highwayman§n§n",
	//"Shadow",
	"The §3Shadow§7 has perfected the darker side of the warrior's path. Shadows are skilled at "
	"hiding in the shadows and attacking from behind. They also have knowledge of various poisons "
	"and venoms.§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n",
	//"Highwayman"
	"§n§n"
	"Key skills: To Be Determined§n§n"
	"Requirements: Level 90§n§n"
};

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

char* getClassName(int classnum, unsigned char gender)
{
	if (gender)
	{
		return (char*)CLASS_NAMES_FEMALE[classnum];
	}

	return (char*)CLASS_NAMES_MALE[classnum];
}

char* getClassDescription(int classnum)
{
	return (char*)CLASS_DESCRIPTIONS[classnum];
}

Basic::CCharString getClassGroup(int classflag, unsigned char gender)
{
	// these flags are used to find what flags that can be cleared once the current one has been
	// found... For example, when "Wizard" is found, you can clear all those that are beneath it
	// in the class tree.
	const int CCF_MUNDANE =		0xFFFFFFFF;

	const int CCF_APPRENTICE =		CF_APPRENTICE | CF_WIZARD | CF_NECROMANCER | CF_BLACKREAPER | CF_LORDOFTHELICH | CF_MAGE | CF_SORCERER | CF_ELEMENTALIST;
	const int CCF_WIZARD =			CF_WIZARD | CF_NECROMANCER | CF_BLACKREAPER | CF_LORDOFTHELICH | CF_MAGE | CF_SORCERER | CF_ELEMENTALIST;
	const int CCF_NECROMANCER =		CF_NECROMANCER | CF_BLACKREAPER | CF_LORDOFTHELICH;
	const int CCF_MAGE =			CF_MAGE | CF_SORCERER | CF_ELEMENTALIST;

	const int CCF_NEOPHYTE =		CF_NEOPHYTE | CF_CLERIC | CF_HEALER | CF_PRIEST | CF_RESTORER | CF_NATURALIST | CF_DRUID | CF_WARRIORMONK;
	const int CCF_CLERIC =			CF_CLERIC | CF_HEALER | CF_PRIEST | CF_RESTORER | CF_NATURALIST | CF_DRUID | CF_WARRIORMONK;
	const int CCF_HEALER =			CF_HEALER | CF_PRIEST | CF_RESTORER;
	const int CCF_NATURALIST =		CF_NATURALIST | CF_DRUID | CF_WARRIORMONK;

	const int CCF_FIGHTER =			CF_FIGHTER | CF_WARRIOR | CF_KNIGTH | CF_PALADIN | CF_COMMANDER | CF_WEAPONMASTER | CF_RANGER | CF_SWORDDANCER | CF_EXECUTIONER | CF_THUG | CF_SHADOWFIGHTER | CF_HIGHWAY;
	const int CCF_WARRIOR =			CF_WARRIOR | CF_KNIGTH | CF_PALADIN | CF_COMMANDER | CF_WEAPONMASTER | CF_RANGER | CF_SWORDDANCER | CF_EXECUTIONER | CF_THUG | CF_SHADOWFIGHTER | CF_HIGHWAY;
	const int CCF_KNIGTH =			CF_KNIGTH | CF_PALADIN | CF_COMMANDER;
	const int CCF_WEAPONMASTER =	CF_WEAPONMASTER | CF_RANGER | CF_SWORDDANCER | CF_EXECUTIONER;
	const int CCF_THUG =			CF_THUG | CF_SHADOWFIGHTER | CF_HIGHWAY;

	// remove flags that aren't used
	classflag &= ~CLASSGROUPFLAG_NOTUSED;

	if (!classflag || (classflag == CLASSGROUPFLAG_ALL) || ((classflag & CF_MUNDANE) != 0))
	{
		return Basic::CCharString("All classes");
	}

	Basic::CCharString string;
	int i;
	bool first = true;

	// add classes while there are more flags in the
	while (classflag)
	{
		Basic::CCharString current;

		// check warrior branch
		if (classflag & CF_FIGHTER)
		{
			current = getClassName(CLASS_FIGHTER, gender);
			classflag &= ~CCF_FIGHTER;
		}
		else if (classflag & CF_WARRIOR)
		{
			current = getClassName(CLASS_WARRIOR, gender);
			classflag &= ~CCF_WARRIOR;
		}
		else if (classflag & CF_KNIGTH)
		{
			current = getClassName(CLASS_KNIGTH, gender);
			classflag &= ~CCF_KNIGTH;
		}
		else if (classflag & CF_WEAPONMASTER)
		{
			current = getClassName(CLASS_WEAPONMASTER, gender);
			classflag &= ~CCF_WEAPONMASTER;
		}
		else if (classflag & CF_THUG)
		{
			current = getClassName(CLASS_THUG, gender);
			classflag &= ~CCF_THUG;
		}
		else if (classflag & CF_NEOPHYTE)
		{
			current = getClassName(CLASS_NEOPHYTE, gender);
			classflag &= ~CCF_NEOPHYTE;
		}
		else if (classflag & CF_CLERIC)
		{
			current = getClassName(CLASS_CLERIC, gender);
			classflag &= ~CCF_CLERIC;
		}
		else if (classflag & CF_HEALER)
		{
			current = getClassName(CLASS_HEALER, gender);
			classflag &= ~CCF_HEALER;
		}
		else if (classflag & CF_NATURALIST)
		{
			current = getClassName(CLASS_NATURALIST, gender);
			classflag &= ~CCF_NATURALIST;
		}
		else if (classflag & CF_APPRENTICE)
		{
			current = getClassName(CLASS_APPRENTICE, gender);
			classflag &= ~CCF_APPRENTICE;
		}
		else if (classflag & CF_WIZARD)
		{
			current = getClassName(CLASS_WIZARD, gender);
			classflag &= ~CCF_WIZARD;
		}
		else if (classflag & CF_NECROMANCER)
		{
			current = getClassName(CLASS_NECROMANCER, gender);
			classflag &= ~CCF_NECROMANCER;
		}
		else if (classflag & CF_MAGE)
		{
			current = getClassName(CLASS_MAGE, gender);
			classflag &= ~CCF_MAGE;
		}
		else
		{
			// we didn't find any of the branch splitters, so we need to look for the specific
			// leaf classes instead
			for (i = 0; i < 32; i++)
			{
				if (classflag & (1 << i))
				{
					current = getClassName(i, gender);
					classflag &= ~(1 << i);
					break;
				}
			}
		}

		if (!first)
		{
			if (!classflag)
			{
				string += " & ";
			}
			else
			{
				string += ", ";
			}
		}
		first = false;

		string += current;
	}

	return Basic::CCharString(string);

	/*
	if (classflag == CLASSGROUPFLAG_ALL)
	{
		return Basic::CCharString("All classes");
	}
	if (classflag == CLASSGROUPFLAG_WARRIORS)
	{
		return Basic::CCharString("Warriors");
	}
	if (classflag == CLASSGROUPFLAG_PALADINS)
	{
		return Basic::CCharString("Knights");
	}
	if (classflag == CLASSGROUPFLAG_WEAPONMASTERS)
	{
		return Basic::CCharString("Weapon Masters");
	}
	if (classflag == CLASSGROUPFLAG_THUGS)
	{
		return Basic::CCharString("Thugs");
	}
	if (classflag == CLASSGROUPFLAG_CLERICS)
	{
		return Basic::CCharString("Clerics");
	}
	if (classflag == CLASSGROUPFLAG_HEALERS)
	{
		return Basic::CCharString("Priests");
	}
	if (classflag == CLASSGROUPFLAG_MONKS)
	{
		return Basic::CCharString("Monks");
	}
	if (classflag == CLASSGROUPFLAG_WIZARDS)
	{
		return Basic::CCharString("Wizards");
	}
	if (classflag == CLASSGROUPFLAG_MAGES)
	{
		return Basic::CCharString("Mages");
	}
	if (classflag == CLASSGROUPFLAG_NECROMANCERS)
	{
		return Basic::CCharString("Necromancers");
	}

	// no luck finding specific groups, so build a list
	Basic::CCharString string;
	int i;
	bool first = true;

	for (i = 0; i < 32; i++)
	{
		if (classflag & (1 << i))
		{
			if (!first)
			{
				string += "\n";
			}
			first = false;

			string += getClassName(i, gender);
		}
	}

	return Basic::CCharString(string);
	*/
}

bool classIsAvailableFromClass(int destclass, int sourceclass)
{
	const int SOURCE_CLASSES[NUM_CLASSES] =
	{
		-1,

		CLASS_MUNDANE,
		CLASS_APPRENTICE,
		CLASS_WIZARD,
		CLASS_NECROMANCER,
		CLASS_NECROMANCER,
		CLASS_WIZARD,
		CLASS_MAGE,
		CLASS_MAGE,

		CLASS_MUNDANE,
		CLASS_NEOPHYTE,
		CLASS_CLERIC,
		CLASS_HEALER,
		CLASS_HEALER,
		CLASS_CLERIC,
		CLASS_NATURALIST,
		CLASS_NATURALIST,

		CLASS_MUNDANE,
		CLASS_FIGHTER,
		CLASS_WARRIOR,
		CLASS_KNIGTH,
		CLASS_KNIGTH,
		CLASS_WARRIOR,
		CLASS_WEAPONMASTER,
		CLASS_WEAPONMASTER,
		CLASS_WEAPONMASTER,
		CLASS_WARRIOR,
		CLASS_THUG,
		CLASS_THUG
	};

	return (SOURCE_CLASSES[destclass] == sourceclass);
}

bool testClassRequirements(int destclass, CCharacterAttributes& attribs, Basic::CCharString& failreason)
{
	if (!classIsAvailableFromClass(destclass, attribs.u_stats.pclass))
	{
		failreason = "Can't progress from the current class";
		return false;
	}

	if (destclass == CLASS_APPRENTICE)
	{
		// must know "Magic Use I"
		SRequirement req;
		req.type = RT_SKILL;
		req.value = 15;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		return true;
	}
	else if (destclass == CLASS_WIZARD)
	{
		// must know "Flare" and level 25
		SRequirement req;
		req.type = RT_LEVEL;
		req.value = 25;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		req.type = RT_SKILL;
		req.value = 29;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		return true;
	}
	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

	else if (destclass == CLASS_NEOPHYTE)
	{
		// must know "Curse Resistance"
		SRequirement req;
		req.type = RT_SKILL;
		req.value = 7;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		return true;
	}
	else if (destclass == CLASS_CLERIC)
	{
		// must know "Cure Wounds" and level 25
		SRequirement req;
		req.type = RT_LEVEL;
		req.value = 25;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		req.type = RT_SKILL;
		req.value = 14;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		return true;
	}

	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

	else if (destclass == CLASS_FIGHTER)
	{
		// must know "Fighting I"
		SRequirement req;
		req.type = RT_SKILL;
		req.value = 0;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		return true;
	}
	else if (destclass == CLASS_WARRIOR)
	{
		// must know "Toughness I" and level 25
		SRequirement req;
		req.type = RT_LEVEL;
		req.value = 25;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		req.type = RT_SKILL;
		req.value = 16;
		if (!testRequirement(attribs, req))
		{
			failreason = getRequirementString(req);
			return false;
		}
		return true;
	}

	switch (destclass)
	{
		//case CLASS_APPRENTICE:
		//case CLASS_NEOPHYTE:
		//case CLASS_FIGHTER:

		//case CLASS_WIZARD:
		//case CLASS_CLERIC:
		//case CLASS_WARRIOR:
			
		case CLASS_MAGE:
		case CLASS_NECROMANCER:
		case CLASS_NATURALIST:
		case CLASS_HEALER:
		case CLASS_KNIGTH:
		case CLASS_WEAPONMASTER:
		case CLASS_THUG:
			{
				SRequirement req;
				req.type = RT_LEVEL;
				req.value = 50;
				if (!testRequirement(attribs, req))
				{
					failreason = getRequirementString(req);
					return false;
				}
				return true;
			}

		case CLASS_BLACKREAPER:
		case CLASS_LORDOFTHELICH:
		case CLASS_SORCERER:
		case CLASS_ELEMENTALIST:
		case CLASS_PRIEST:
		case CLASS_RESTORER:
		case CLASS_DRUID:
		case CLASS_WARRIORMONK:
		case CLASS_PALADIN:
		case CLASS_COMMANDER:
		case CLASS_RANGER:
		case CLASS_SWORDDANCER:
		case CLASS_EXECUTIONER:
		case CLASS_SHADOW:
		case CLASS_HIGHWAY:
			{
				SRequirement req;
				req.type = RT_LEVEL;
				req.value = 90;
				if (!testRequirement(attribs, req))
				{
					failreason = getRequirementString(req);
					return false;
				}
				return true;
			}
	}

	failreason = "The class is not available in the current version";
	return false;
}


} // end of RpgSystem namespace
