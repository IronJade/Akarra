/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class is used to save a character's quest status to file.
 **/

#include "CCharacterQuests.h"
#include "../PlayerSystem/AkarraCharacter.h"
#include "../library/FileSystem/CFileStream.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharacterQuests::CCharacterQuests()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCharacterQuests::~CCharacterQuests()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CCharacterQuests::removeAll()
{
	for (int i(0); i < u_questlist.size(); i++)
	{
		delete u_questlist[i];
	}
	u_questlist.clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterQuests::read(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	if (version <= Player::CHRFV_020913)
	{
		// quests haven't been added to this version yet
		return true;
	}

	// read number of quests
	int count = 0;
	file.read(&count, sizeof(int), 1);
	for (int i(0); i < count; i++)
	{
		CQuestInstance* quest = new CQuestInstance;
		quest->read(file);

		u_questlist.push_back(quest);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCharacterQuests::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write number of quests
	int count = u_questlist.size();
	file.write(&count, sizeof(int), 1);

	for (int i(0); i < count; i++)
	{
		u_questlist[i]->write(file);
	}

	return true;
}

}
