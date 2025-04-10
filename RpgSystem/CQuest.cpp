/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Quest, as edited in the quest editor.
 **/

#include "CQuest.h"
#include "../library/FileSystem/CFileStream.h"
#include <string.h>

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CQuestType::CQuestType()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CQuestType::~CQuestType()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CQuestType::clear()
{
	ua_name[0] = 0;
	ua_description[0] = 0;

	{
		for (int i = 0; i < MAX_QUEST_OBJECTIVES; i++)
		{
			ua_objectives[i].type = QOT_DISABLED;
			ua_objectives[i].param1 = 0;
			ua_objectives[i].param2 = 0;
			ua_objectives[i].detail[0] = 0;
			ua_objectives[i].desc[0] = 0;
			ua_objectives[i].flags = 0;
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CQuestType::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read file version
	int version;
	file.read(&version, sizeof(int), 1);

	// read quest data
	file.read(ua_name, sizeof(char), 32);
	file.read(ua_description, sizeof(char), 32);

	{
		for (int i(0); i < MAX_QUEST_OBJECTIVES; i++)
		{
			file.read(&ua_objectives[i].type, sizeof(int), 1);
			file.read(&ua_objectives[i].param1, sizeof(int), 1);
			file.read(&ua_objectives[i].param2, sizeof(int), 1);
			file.read(ua_objectives[i].detail, sizeof(char), 32);
			file.read(ua_objectives[i].desc, sizeof(char), 64);
			if (version > QFV_030215)
			{
				file.read(&ua_objectives[i].flags, sizeof(int), 1);
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CQuestType::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write file version
	int version = QFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write quest data
	file.write(ua_name, sizeof(char), 32);
	file.write(ua_description, sizeof(char), 32);

	{
		for (int i(0); i < MAX_QUEST_OBJECTIVES; i++)
		{
			file.write(&ua_objectives[i].type, sizeof(int), 1);
			file.write(&ua_objectives[i].param1, sizeof(int), 1);
			file.write(&ua_objectives[i].param2, sizeof(int), 1);
			file.write(ua_objectives[i].detail, sizeof(char), 32);
			file.write(ua_objectives[i].desc, sizeof(char), 64);
			file.write(&ua_objectives[i].flags, sizeof(int), 1);
		}
	}

	return true;
}

	// ***************************************************************************************
	// ***************************************************************************************
	// ***************************************************************************************
	// ***************************************************************************************
	// ***************************************************************************************


// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CQuestInstance::CQuestInstance()
{
	up_questtype = NULL;
	ua_questname[0] = 0;
	u_queststate = QS_NOTSTARTED;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CQuestInstance::~CQuestInstance()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CQuestInstance::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	up_questtype = NULL;

	// read file version
	int version;
	file.read(&version, sizeof(int), 1);

	// read quest data
	file.read(ua_questname, sizeof(char), 32);
	file.read(&u_queststate, sizeof(int), 1);

	{
		for (int i(0); i < MAX_QUEST_OBJECTIVES; i++)
		{
			//int c;
			file.read(&ua_objectives[i].completed, sizeof(int), 1);
			//ua_objectives[i].completed = (c > 0);
			file.read(&ua_objectives[i].counter, sizeof(int), 1);

			if (version <= QIFV_030215)
			{
				// in this version a '0' means "not activated" instead of "not completed"
				switch (ua_objectives[i].completed)
				{
					default:
					case 0:	ua_objectives[i].completed = QOS_NOTCOMPLETED; break;
					case 1: ua_objectives[i].completed = QOS_COMPLETED; break;
				}
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CQuestInstance::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write file version
	int version = QIFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write quest data
	file.write(ua_questname, sizeof(char), 32);
	file.write(&u_queststate, sizeof(int), 1);

	{
		for (int i(0); i < MAX_QUEST_OBJECTIVES; i++)
		{
			//int c = (ua_objectives[i].completed) ? 1 : 0;
			file.write(&ua_objectives[i].completed, sizeof(int), 1);
			//file.write(&ua_objectives[i].completed, sizeof(int), 1);
			file.write(&ua_objectives[i].counter, sizeof(int), 1);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// If newquest is true, this is a newly added quest for the character (otherwise it's
// just a questtype update). New quests needs to enable or disable objectives depending
// on the questtype.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CQuestInstance::setQuestType(CQuestType* type, bool newquest)
{
	if (!type)
	{
		return;
	}

	// set the type
	up_questtype = type;

	// copy the name (so we can save this instance correctly)
	strcpy(ua_questname, up_questtype->ua_name);

	if (newquest)
	{
		// set objective states for this new quest
		for (int i(0); i < MAX_QUEST_OBJECTIVES; i++)
		{
			ua_objectives[i].completed = ((up_questtype->ua_objectives[i].flags & QOFLAG_HIDDEN) != 0) ? QOS_NOTACTIVATED : QOS_NOTCOMPLETED;
		}
	}
}


}
