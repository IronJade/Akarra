/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player guild rules.
 **/

#include "CGuild.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/FileSystem.h"
#include "../library/Basic/CCharString.h"
#include <time.h>

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGuild::CGuild()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGuild::~CGuild()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::clear()
{
	ma_guildname[0] = 0;
	m_guildtype = 0;
	m_tagcolor = 0;
	ma_tag[0] = 0;

	m_owners.clear();
	m_members.clear();
	m_pending.clear();

	m_exptoday = 0;
	m_wealthtoday = 0;
	m_totalexp = 0;
	m_totalwealth = 0;
	m_basepower = START_POWER;
	m_allypower = 0;
	//m_expchanged = true;
	{
		for (int i(0); i < (int)POWER_DAYS; i++)
		{
			ma_dailypower[i] = 0;
		}
	}

	m_expbonus = 1.0f;
	m_ownerexpbonus = 1.0f;
	m_place = GUILD_NOSTANDING;

	m_diplomacy.clear();

	m_nextmessageid = 0;
	m_messages.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::removeAll()
{
	// delete messages
	TDGuildMessageList::iterator message;
	for (message = m_messages.begin(); message != m_messages.end(); ++message)
	{
		delete (*message);
	}

	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// clear old data
	removeAll();

	// read file version
	int version;
	file.read(&version, sizeof(int), 1);

	// read owners
	{
		int count;
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SGuildPerson name;
			file.read(name.name, sizeof(char), 16);
			file.read(&name.exp, sizeof(unsigned int), 1);
			m_owners.push_back(name);
		}
	}
	// read members
	{
		int count;
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SGuildPerson name;
			file.read(name.name, sizeof(char), 16);
			file.read(&name.exp, sizeof(unsigned int), 1);
			m_members.push_back(name);
		}
	}
	// read pending
	{
		int count;
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SGuildPerson name;
			file.read(name.name, sizeof(char), 16);
			file.read(&name.exp, sizeof(unsigned int), 1);
			m_pending.push_back(name);
		}
	}
	// read diplomacy
	{
		int count;
		file.read(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			SGuildDiplomacySetting diplomacy;
			file.read(diplomacy.guild, sizeof(char), 32);
			file.read(&diplomacy.setting, sizeof(int), 1);
			m_diplomacy.push_back(diplomacy);
		}
	}

	// read other data
	file.read(ma_guildname, sizeof(char), 32);
	file.read(ma_tag, sizeof(char), GUILD_TAGLENGTH + 1);
	file.read(&m_guildtype, sizeof(int), 1);
	file.read(&m_tagcolor, sizeof(int), 1);

	file.read(&m_totalexp, sizeof(unsigned long), 1);
	file.read(&m_totalwealth, sizeof(unsigned long), 1);
	file.read(&m_basepower, sizeof(float), 1);
	file.read(&m_allypower, sizeof(float), 1);
	if (version <= GFV_030506)
	{
		// this is the new power format, be generous and keep 25% of the old power
		m_basepower *= 0.25f;

		m_exptoday = 0;
		m_wealthtoday = 0;
		for (int i(0); i < (int)POWER_DAYS; i++)
		{
			ma_dailypower[i] = 0;
		}
	}
	if (version > GFV_030506)
	{
		file.read(&m_exptoday, sizeof(int), 1);
		file.read(&m_wealthtoday, sizeof(int), 1);
		for (int i(0); i < (int)POWER_DAYS; i++)
		{
			file.read(&ma_dailypower[i], sizeof(float), 1);
		}
	}

	file.read(&m_place, sizeof(int), 1);
	file.read(&m_expbonus, sizeof(float), 1);
	file.read(&m_ownerexpbonus, sizeof(float), 1);

	if (version > GFV_030506)
	{
		// KHADDED
		long daytime = time(0);
		// !KHADDED

		// messages...
		file.read(&m_nextmessageid, sizeof(int), 1);
		{
			int count(0);
			file.read(&count, sizeof(int), 1);

			for (int i(0); i < (int)count; i++)
			{
				SGuildMessage* gm = new SGuildMessage;

				file.read(&gm->id, sizeof(int), 1);
				file.read(&gm->posttime, sizeof(long), 1);
				gm->title = FileSystem::readStringWithLength(file);
				gm->message = FileSystem::readStringWithLength(file);

				// KHADDED
				// don't add messages that are more than one month old
				if (gm->posttime < (daytime - (30 * 24 * 60 * 60)))
				{
					delete gm;
				}
				else
				{
					m_messages.push_back(gm);
				}
				// !KHADDED
			}
		}
	}

	if (version <= GFV_030802)
	{
		// reset stats
		m_basepower = START_POWER;
		m_allypower = 0;
		{
			for (int i(0); i < (int)POWER_DAYS; i++)
			{
				ma_dailypower[i] = 0;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write file version
	int version = GFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write owners
	{
		int count = (int)m_owners.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_owners[i].name, sizeof(char), 16);
			file.write(&m_owners[i].exp, sizeof(unsigned int), 1);
		}
	}
	// write members
	{
		int count = (int)m_members.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_members[i].name, sizeof(char), 16);
			file.write(&m_members[i].exp, sizeof(unsigned int), 1);
		}
	}
	// write pending
	{
		int count = (int)m_pending.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_pending[i].name, sizeof(char), 16);
			file.write(&m_pending[i].exp, sizeof(unsigned int), 1);
		}
	}
	// write diplomacy
	{
		int count = (int)m_diplomacy.size();
		file.write(&count, sizeof(int), 1);
		for (int i(0); i < count; i++)
		{
			file.write(m_diplomacy[i].guild, sizeof(char), 32);
			file.write(&m_diplomacy[i].setting, sizeof(int), 1);
		}
	}

	// write other data
	file.write(ma_guildname, sizeof(char), 32);
	file.write(ma_tag, sizeof(char), GUILD_TAGLENGTH + 1);
	file.write(&m_guildtype, sizeof(int), 1);
	file.write(&m_tagcolor, sizeof(int), 1);

	file.write(&m_totalexp, sizeof(unsigned long), 1);
	file.write(&m_totalwealth, sizeof(unsigned long), 1);
	file.write(&m_basepower, sizeof(float), 1);
	file.write(&m_allypower, sizeof(float), 1);
	file.write(&m_exptoday, sizeof(int), 1);
	file.write(&m_wealthtoday, sizeof(int), 1);
	{
		for (int i(0); i < (int)POWER_DAYS; i++)
		{
			file.write(&ma_dailypower[i], sizeof(float), 1);
		}
	}

	file.write(&m_place, sizeof(int), 1);
	file.write(&m_expbonus, sizeof(float), 1);
	file.write(&m_ownerexpbonus, sizeof(float), 1);

	// messages...
	file.write(&m_nextmessageid, sizeof(int), 1);
	{
		int count = (int)m_messages.size();
		file.write(&count, sizeof(int), 1);

		TDGuildMessageList::iterator message;
		for (message = m_messages.begin(); message != m_messages.end(); ++message)
		{
			file.write(&(*message)->id, sizeof(int), 1);
			file.write(&(*message)->posttime, sizeof(long), 1);
			FileSystem::writeStringWithLength(file, (*message)->title);
			FileSystem::writeStringWithLength(file, (*message)->message);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::setNewGuild(char* name, int type, int color, char* tag)
{
	strcpy(ma_guildname, name);
	strcpy(ma_tag, tag);
	m_guildtype = type;
	m_tagcolor = color;

	// log a message
	{
		Basic::CCharString title;
		Basic::CCharString message("The ");
		switch (m_guildtype)
		{
			case GT_GUILD_GIFTED:
				title = "GUILD WAS FOUNDED!";
				message += "Gifted guild ";
				break;
			case GT_GUILD_SHINING:
				title = "GUILD WAS FOUNDED!";
				message += "Shining guild ";
				break;
			case GT_CLAN:
				title = "CLAN WAS FOUNDED!";
				message += "clan ";
				break;
		}
		message += ma_guildname;
		message += " (§";
		message += (char)('0' + m_tagcolor);
		message += ma_tag;
		message += "§7) was founded.";

		addMessage(title, message);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::addOwner(char* name)
{
	SGuildPerson owner;
	strcpy(owner.name, name);
	owner.exp = 0;
	m_owners.push_back(owner);
	m_members.push_back(owner);

	//m_expchanged = true;

	// message log
	{
		Basic::CCharString message("§3");
		message += name;
		message += " §7joined the guild/clan as a new leader.";
		addMessage(Basic::CCharString("New leader!"), message);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CGuild::getGuildName()
{
	return ma_guildname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CGuild::getGuildTag()
{
	return ma_tag;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGuild::getGuildTagColor()
{
	return m_tagcolor;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGuild::getGuildType()
{
	return m_guildtype;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::chrIsOwner(char* name)
{
	{
		for (int i(0); i < m_owners.size(); i++)
		{
			if (stricmp(m_owners[i].name, name) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::chrIsMember(char* name)
{
	{
		for (int i(0); i < m_members.size(); i++)
		{
			if (stricmp(m_members[i].name, name) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::chrIsPending(char* name)
{
	{
		for (int i(0); i < m_pending.size(); i++)
		{
			if (stricmp(m_pending[i].name, name) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::addPendingChr(char* name, Basic::CCharString& failreason)
{
	{
		for (int i(0); i < m_pending.size(); i++)
		{
			if (stricmp(m_pending[i].name, name) == 0)
			{
				failreason = name;
				failreason += " is already in the guild's pending list";
				return false;
			}
		}
	}

	SGuildPerson newpending;
	strcpy(newpending.name, name);
	newpending.exp = 0;
	m_pending.push_back(newpending);

	// message log
	{
		Basic::CCharString message("§3");
		message += name;
		// KHADDED
		//message += " §7added himself to the pending list and is awaiting an invitation from one of the leaders.";
		message += " §7added herself/himself to the pending list and is awaiting an invitation.";
		// !KHADDED
		Basic::CCharString title("§3");
		title += name;
		title += " §7wants to join!";
		addMessage(title, message);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::denyPendingChr(char* name, char* leader, Basic::CCharString& failreason)
{
	{
		TDGuildPersonVec::iterator nameit = m_pending.begin();
		for ( ; nameit != m_pending.end(); ++nameit)
		{
			if (stricmp((*nameit).name, name) == 0)
			{
				// remove this player from the pending list
				m_pending.erase(nameit);
				
				// message log
				{
					Basic::CCharString message("§3");
					message += name;
					if (leader)
					{
						message += " §7was denied membership and was removed from the pending list by §3";
						message += leader;
						message += "§7.";
					}
					else
					{
						message += " §7left the pending list volunteerily.";
					}
					Basic::CCharString title("§3");
					title += name;
					if (leader)
					{
						title += " §7wasn't allowed to join";
					}
					else
					{
						title += " §7left the pending list";
					}
					addMessage(title, message);
				}
				return true;
			}
		}
	}

	failreason = name;
	failreason += " is not in the guild's pending list";
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::invitePendingToMember(char* name, char* leader, Basic::CCharString& failreason)
{
	{
		for (int i(0); i < m_members.size(); i++)
		{
			if (stricmp(m_members[i].name, name) == 0)
			{
				failreason = name;
				failreason += " is already a member of this guild";
				return false;
			}
		}
	}
	{
		TDGuildPersonVec::iterator nameit = m_pending.begin();
		for ( ; nameit != m_pending.end(); ++nameit)
		{
			if (stricmp((*nameit).name, name) == 0)
			{
				// promote this member
				(*nameit).exp = 0;
				m_members.push_back((*nameit));
				m_pending.erase(nameit);
				
				//m_expchanged = true;

				// message log
				{
					Basic::CCharString message("§3");
					message += name;
					message += " §7was invited by §3";
					message += leader;
					message += "§7 and is now a full member!";
					Basic::CCharString title("§3");
					title += name;
					title += " §7was invited!";
					addMessage(title, message);
				}
				return true;
			}
		}
	}

	failreason = name;
	failreason += " is not in the guild's pending list";
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::removeMember(char* name, char* leader)
{
	{
		TDGuildPersonVec::iterator nameit = m_members.begin();
		for ( ; nameit != m_members.end(); ++nameit)
		{
			if (stricmp((*nameit).name, name) == 0)
			{
				// remove this member
				m_members.erase(nameit);
				//m_expchanged = true;

				// message log
				if (leader)
				{
					Basic::CCharString message("§3");
					message += name;
					message += " §7was removed from the guild/clan by §3";
					message += leader;
					message += "§7.";
					Basic::CCharString title("§3");
					title += name;
					title += " §7was kicked!";
					addMessage(title, message);
				}
				else
				{
					Basic::CCharString message("§3");
					message += name;
					message += " §7left the guild/clan.";
					Basic::CCharString title("§3");
					title += name;
					title += " §7left!";
					addMessage(title, message);
				}
				return true;
			}
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::demoteOwner(char* name, Basic::CCharString& failreason)
{
	{
		TDGuildPersonVec::iterator nameit = m_owners.begin();
		for ( ; nameit != m_owners.end(); ++nameit)
		{
			if (stricmp((*nameit).name, name) == 0)
			{
				// demote this player
				m_owners.erase(nameit);

				// message log
				{
					Basic::CCharString message("§3");
					message += name;
					message += " §7demoted herself/himself from leadership.";
					Basic::CCharString title("§3");
					title += name;
					title += " §7left leadership";
					addMessage(title, message);
				}
				return true;
			}
		}
	}

	failreason = name;
	failreason += " is not an owner of this guild";
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::promoteMember(char* name, char* leader, Basic::CCharString& failreason)
{
	if (m_owners.size() >= 5)
	{
		failreason = "Guild already has 5 owners";
		return false;
	}
	{
		for (int i(0); i < m_owners.size(); i++)
		{
			if (stricmp(m_owners[i].name, name) == 0)
			{
				failreason = name;
				failreason += " already is an owner of this guild";
				return false;
			}
		}
	}
	{
		for (int i(0); i < m_members.size(); i++)
		{
			if (stricmp(m_members[i].name, name) == 0)
			{
				// add this member to our list of owners
				m_owners.push_back(m_members[i]);

				// message log
				{
					Basic::CCharString message("§3");
					message += name;
					message += " §7was promoted to leadership by §3";
					message += leader;
					message += "§7.";
					Basic::CCharString title("§3");
					title += name;
					title += " §7was promoted";
					addMessage(title, message);
				}
				return true;
			}
		}
	}

	failreason = name;
	failreason += " is not a member of this guild";
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::addMemberExperience(int newexp)
{
	// find the character
	//for (int i(0); i < m_members.size(); i++)
	//{
	//	if (stricmp(m_members[i].name, name) == 0)
	//	{
	//		m_members[i].exp = current;
	//		m_expchanged = true;
	//		break;
	//	}
	//}

	// prevent some bugs (integer overflow and such)
	if ((newexp < 0) || (m_exptoday > (0x7FFFFFFF - newexp)))
	{
		return;
	}

	m_exptoday += newexp;
	m_totalexp += (unsigned long)newexp;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned long CGuild::getTotalExp()
{
	//if (m_expchanged)
	//{
	//	// recalculate exp
	//	m_totalexp = 0;
	//	for (int i(0); i < (int)m_members.size(); i++)
	//	{
	//		m_totalexp += (unsigned long)m_members[i].exp;
	//	}
	//	m_expchanged = false;
	//}
	return m_totalexp;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned long CGuild::getTotalWealth()
{
	return m_totalwealth;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CGuild::getTotalPower()
{
	float total = m_basepower;
	for (int i(0); i < (int)POWER_DAYS; i++)
	{
		total += ma_dailypower[i];
	}
	return total;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CGuild::getBasePower()
{
	return m_basepower;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CGuild::getDayPower(int day)
{
	return ma_dailypower[day];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CGuild::getAllySharePower()
{
	return m_allypower;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function is used like this: Request the "ally power" (getAllySharePower()) from
// the allied guild and call this function. This guild will then add a bonus to his
// previous day (day 0) based on that power.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::addAllyPower(float power)
{
	if (power > 0)
	{
		ma_dailypower[0] += power * 0.1f;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function should be called once per day. It will recalculate the total power for
// the guild and store the power which should be shared with allies.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::updateDailyPower()
{
	// calculate today's power and store it in allypower
	m_allypower = 0;
	m_allypower += (float)(m_exptoday + DAILY_START_EXP) * 0.000001f; // 1,000,000 exp is worth 1 point
	m_allypower += (float)(m_wealthtoday + DAILY_START_COINS) * 0.0001f; // 10 gc is worth 1 point
	m_exptoday = 0;
	m_wealthtoday = 0;

	// 10% of the last day's power is kept in our base power
	m_basepower += ma_dailypower[POWER_DAYS - 1] * 0.1f;

	// move all powers one day down 
	{
		for (int i(POWER_DAYS - 1); i >= 1; i--)
		{
			ma_dailypower[i] = ma_dailypower[i - 1];
		}
	}
	// and store our new power in the current day (note: more power will be added later if
	// this guild has any allies)
	ma_dailypower[0] = m_allypower;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::reportStanding(int place)
{
	m_place = place;

	// the top 5 guilds get an experience bonus
	if (place < 5)
	{
		m_expbonus = 1;
		m_ownerexpbonus = (5 - place) + 3;
	}
	else if (place < 10)
	{
		m_expbonus = 1;
		m_ownerexpbonus = 2;
	}
	else
	{
		m_expbonus = 0;
		m_ownerexpbonus = 0;
	}
	//m_ownerexpbonus = m_expbonus * 1.5f;

	// convert into a percentage
	m_expbonus += 100.0f;
	m_ownerexpbonus += 100.0f;
	m_expbonus *= 0.01f;
	m_ownerexpbonus *= 0.01f;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGuild::getStanding()
{
	return m_place;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CGuild::getStandingExpBonus(bool isowner)
{
	return ((isowner) ? m_ownerexpbonus : m_expbonus);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDGuildPersonVec& CGuild::getOwnerList()
{
	return m_owners;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDGuildPersonVec& CGuild::getMemberList()
{
	return m_members;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDGuildPersonVec& CGuild::getPendingList()
{
	return m_pending;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDDiplomacyVec& CGuild::getDiplomacyList()
{
	return m_diplomacy;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function should post a message in the guild's log, but since we need to double-
// post messages (the other guild needs to be informed too), it's better if the character
// manages that part himself.
// 031219: If the diplomacy is set to -1 the guild reference will be removed instead of
//         added.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::setGuildDiplomacy(char* guildname, int diplomacy)
{
	// find guild...
	for (int i(0); i < m_diplomacy.size(); i++)
	{
		if (stricmp(m_diplomacy[i].guild, guildname) == 0)
		{
			if (diplomacy == -1)
			{
				TDDiplomacyVec::iterator it = m_diplomacy.begin();
				std::advance(it, i);
				m_diplomacy.erase(it);
			}
			else
			{
				m_diplomacy[i].setting = diplomacy;
			}
			return;
		}
	}
	if (diplomacy == -1)
	{
		return;
	}

	// we couldn't find the guild, so we should add it
	SGuildDiplomacySetting guild;
	strcpy(guild.guild, guildname);
	guild.setting = diplomacy;
	m_diplomacy.push_back(guild);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGuild::getGuildDiplomacy(char* guildname)
{
	// find guild...
	for (int i(0); i < m_diplomacy.size(); i++)
	{
		if (stricmp(m_diplomacy[i].guild, guildname) == 0)
		{
			return m_diplomacy[i].setting;
		}
	}

	return GDS_NEUTRAL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::isHostile(char* guildname)
{
	// find guild...
	for (int i(0); i < m_diplomacy.size(); i++)
	{
		if (stricmp(m_diplomacy[i].guild, guildname) == 0)
		{
			return (m_diplomacy[i].setting == GDS_HOSTILE);
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuild::isAlly(char* guildname)
{
	// find guild...
	for (int i(0); i < m_diplomacy.size(); i++)
	{
		if (stricmp(m_diplomacy[i].guild, guildname) == 0)
		{
			return (m_diplomacy[i].setting == GDS_ALLY);
		}
	}
	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuild::addMessage(Basic::CCharString& title, Basic::CCharString& message)
{
	// create a new message struct
	SGuildMessage* gm = new SGuildMessage;

	// set some info
	gm->id = m_nextmessageid++;
	gm->posttime = time(0);
	gm->title = title;
	gm->message = message;

	// push it in
	m_messages.push_back(gm);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDGuildMessageList& CGuild::getMessageList()
{
	return m_messages;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
SGuildMessage* CGuild::getMessage(int id)
{
	TDGuildMessageList::iterator message;
	for (message = m_messages.begin(); message != m_messages.end(); ++message)
	{
		if (id == (*message)->id)
		{
			return (*message);
		}
	}

	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//void CGuild::m_recalculatePower()
//{
//	// each million of exp is worth one point, but we keep the count in 0.1s
//	unsigned int hundredsofthousands = getTotalExp() / 100000;
//	m_power = (float)hundredsofthousands * 0.1f;
//
//	// each 10 gold coins is worth one point
//	unsigned int goldcoins = getTotalWealth() / 1000;
//	m_power += (float)goldcoins * 0.1f;
//}

// *******************************************************************************************************
// *******************************************************************************************************
// *******************************************************************************************************
// *******************************************************************************************************
// *******************************************************************************************************
// *******************************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGuildHandler::CGuildHandler()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGuildHandler::~CGuildHandler()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuildHandler::clear()
{
	m_guilds.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGuildHandler::removeAll()
{
	{
		for (int i(0); i < m_guilds.size(); i++)
		{
			delete m_guilds[i];
		}
	}
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuildHandler::loadAllGuilds()
{
	removeAll();

	std::vector<Basic::CCharString> filenames;
	FileSystem::getFilenamesNoPath(filenames, "guilds", "*.guild", false);

	for (int i(0); i < filenames.size(); i++)
	{
		// attempt to open the file
		Basic::CCharString filename("guilds/");
		filename += filenames[i];

		FileSystem::CFileStream file = FileSystem::directOpen(filename, "rb");

		if (file.isOk())
		{
			CGuild* guild = new CGuild;

			if (!guild->read(file))
			{
				return false;
			}
			else
			{
				m_guilds.push_back(guild);
			}

			FileSystem::directClose(file);
		}
	}
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuildHandler::saveAllGuilds()
{
	for (int i(0); i < m_guilds.size(); i++)
	{
		// attempt to open the file
		Basic::CCharString filename("guilds/");
		filename += m_guilds[i]->getGuildName();
		filename += ".guild";

		FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");

		if (file.isOk())
		{
			if (!m_guilds[i]->write(file))
			{
				return false;
			}

			FileSystem::directClose(file);
		}
		else
		{
			return false;
		}
	}
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuildHandler::saveGuild(char* guildname)
{
	CGuild* guild = getGuild(guildname);

	if (!guild)
	{
		return false;
	}

	Basic::CCharString filename("guilds/");
	filename += guild->getGuildName();
	filename += ".guild";

	FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");

	if (file.isOk())
	{
		if (!guild->write(file))
		{
			return false;
		}

		FileSystem::directClose(file);
	}
	else
	{
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuildHandler::addGuild(CGuild* guild)
{
	if (!guild)
	{
		return false;
	}
	if (getGuild(guild->getGuildName()) != NULL)
	{
		// guild already exists
		return false;
	}
	m_guilds.push_back(guild);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGuildHandler::removeGuild(char* guildname)
{
	// make a copy of the guild name (don't trust the source pointer)
	Basic::CCharString name = guildname;

	// find the guild
	int i = 0;
	for ( ; i < m_guilds.size(); i++)
	{
		if (stricmp(m_guilds[i]->getGuildName(), name.getString()) == 0)
		{
			break;
		}
	}
	if (i >= m_guilds.size())
	{
		return false;
	}

	// delete the guild from our list
	TDGuildVec::iterator it = m_guilds.begin();
	std::advance(it, i);
	// error-check
	if (stricmp((*it)->getGuildName(), name.getString()))
	{
		return false;
	}
	delete (*it);
	m_guilds.erase(it);

	// now go through our guilds and remove the reference to the removed guild
	for (i = 0; i < m_guilds.size(); i++)
	{
		m_guilds[i]->setGuildDiplomacy(name.getString(), -1);
	}
	// and finally rename the guild file
	Basic::CCharString newname("guilds/");
	Basic::CCharString oldname("guilds/");
	newname += name + ".~guild";
	oldname += name + ".guild";
	remove(newname);
	rename(oldname, newname);
	
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGuild* CGuildHandler::getGuild(char* guildname)
{
	for (int i(0); i < m_guilds.size(); i++)
	{
		if (stricmp(m_guilds[i]->getGuildName(), guildname) == 0)
		{
			return m_guilds[i];
		}
	}
	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
TDGuildVec& CGuildHandler::getGuildList()
{
	return m_guilds;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGuildHandler::getNumGuilds()
{
	return (int)m_guilds.size();
}

}
