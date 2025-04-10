/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Requirement structure used by items etc.
 **/

#include "SRequirement.h"
#include "CSkill.h"
#include "CClass.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool testRequirement(CCharacterAttributes& attribs, SRequirement& req)
{
	switch (req.type)
	{
		case RT_LEVEL:
			{
				if ((int)attribs.u_stats.level >= req.value)
				{
					return true;
				}
			}
			break;

		case RT_DEITY:
			{
				if ((!attribs.u_stats.god && !req.value) ||
					(attribs.u_stats.god && req.value))
				{
					return true;
				}
			}
			break;

		case RT_GENDER:
			{
				if ((!attribs.u_stats.gender && !req.value) ||
					(attribs.u_stats.gender && req.value))
				{
					return true;
				}
			}
			break;

		case RT_COLOR:
			{
				if ((int)attribs.u_stats.soulcolor == req.value)
				{
					return true;
				}
			}
			break;

		case RT_CLASS:
			{
				if (((1 << attribs.u_stats.pclass) & req.value) != 0)
				{
					return true;
				}
			}
			break;

		case RT_SKILL:
			{
				if (attribs.hasSkill(req.value))
				{
					return true;
				}
			}
			break;

		case RT_LOCKED:
			{
				if (attribs.hasSkill(req.value, 0, true))
				{
					return true;
				}
			}
			break;

		case RT_STR:
			{
				if ((int)(attribs.u_stats.attributes[BA_STRENGTH].getValue()) >= req.value)
				{
					return true;
				}
			}
			break;

		case RT_CON:
			{
				if ((int)(attribs.u_stats.attributes[BA_CONSTITUTION].getValue()) >= req.value)
				{
					return true;
				}
			}
			break;

		case RT_AGI:
			{
				if ((int)(attribs.u_stats.attributes[BA_AGILITY].getValue()) >= req.value)
				{
					return true;
				}
			}
			break;

		case RT_INT:
			{
				if ((int)(attribs.u_stats.attributes[BA_INTELLIGENCE].getValue()) >= req.value)
				{
					return true;
				}
			}
			break;

		case RT_FTH:
			{
				if ((int)(attribs.u_stats.attributes[BA_FAITH].getValue()) >= req.value)
				{
					return true;
				}
			}
			break;

		default:
			return true;

	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
Basic::CCharString getRequirementString(SRequirement& req)
{
	Basic::CCharString string;

	switch(req.type)
	{
		case RT_LEVEL:
			{
				string = "Level ";
				string += req.value;
				string += " Minimum";
			}
			break;

		case RT_DEITY:
			{
				if (!req.value)
				{
					string = "Gifted Only";
				}
				else
				{
					string = "Shining Only";
				}
			}
			break;

		case RT_GENDER:
			{
				if (!req.value)
				{
					string = "Male Only";
				}
				else
				{
					string = "Female Only";
				}
			}
			break;

		case RT_COLOR:
			{
				string += getGemColorName(req.value);
				string += " Soul Color Only";
			}
			break;

		case RT_CLASS:
			{
				string += getClassGroup(req.value, 0);
				string += " Only";
			}
			break;

		case RT_SKILL:
			{
				string += CSkillType::getSkillName(req.value);
				string += " Required";
			}
			break;

		case RT_LOCKED:
			{
				string += "Specialized ";
				string += CSkillType::getSkillName(req.value);
				string += " Required";
			}
			break;

		case RT_STR:
		case RT_CON:
		case RT_AGI:
		case RT_INT:
		case RT_FTH:
			{
				string = req.value;
				string += " ";
				string += getAttributeName(req.type - RT_STR);
			}
			break;

	}

	return string;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
Basic::CCharString borlandRequirementString(SRequirement& req)
{
	Basic::CCharString string;

	switch(req.type)
	{
		case RT_LEVEL:
			{
				string = "Level ";
				string += req.value;
				string += " Minimum";
			}
			break;

		case RT_DEITY:
			{
				if (!req.value)
				{
					string = "Gifted Only";
				}
				else
				{
					string = "Shining Only";
				}
			}
			break;

		case RT_GENDER:
			{
				if (!req.value)
				{
					string = "Male Only";
				}
				else
				{
					string = "Female Only";
				}
			}
			break;

		case RT_COLOR:
			{
				switch (req.value)
				{
					case 0: string += "White"; break;
					case 1: string += "Red"; break;
					case 2: string += "Green"; break;
					case 3: string += "Blue"; break;
					case 4: string += "Yellow"; break;
				}
				string += " Soul Color Only";
			}
			break;

		case RT_CLASS:
			{
				string += getClassGroup(req.value, 0);
				string += " Only";
			}
			break;

		case RT_SKILL:
			{
				string += "Skill #";
				string += req.value;
				string += " Required";
			}
			break;

		case RT_LOCKED:
			{
				string += "Specialized #";
				string += req.value;
				string += " Required";
			}
			break;

		case RT_STR:
		case RT_CON:
		case RT_AGI:
		case RT_INT:
		case RT_FTH:
			{
				string = req.value;
				switch (req.type - RT_STR)
				{
					case 0: string += " Strength"; break;
					case 1: string += " Constitution"; break;
					case 2: string += " Agility"; break;
					case 3: string += " Intelligence"; break;
					case 4: string += " Faith"; break;
				}
			}
			break;

	}

	return string;
}

}
