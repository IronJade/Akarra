
#include "CTrigger.h"
#include "../library/FileSystem/FileSystem.h"

namespace WorldSystem
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

bool ATrigger::saveId(FileSystem::CFileStream& file)
{
	file.write(&u_id, sizeof(unsigned char), 1);

	return true;
}

bool ATrigger::openId(FileSystem::CFileStream& file, int version)
{
	file.read(&u_id, sizeof(unsigned char), 1);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

bool ATriggerDescription::saveDescription(FileSystem::CFileStream& file)
{
	FileSystem::writeStringWithLength(file, u_description);

	return true;
}

bool ATriggerDescription::openDescription(FileSystem::CFileStream& file, int version)
{
	u_description = FileSystem::readStringWithLength(file);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

bool ATriggerLocation::saveLocation(FileSystem::CFileStream& file)
{
	file.write(u_locarea, sizeof(char), AREA_NAMELENGTH);
	file.write(&u_locx, sizeof(int), 1);
	file.write(&u_locy, sizeof(int), 1);

	return true;
}

bool ATriggerLocation::openLocation(FileSystem::CFileStream& file, int version)
{
	file.read(u_locarea, sizeof(char), AREA_NAMELENGTH);
	file.read(&u_locx, sizeof(int), 1);
	file.read(&u_locy, sizeof(int), 1);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

bool ATriggerFlags::saveFlags(FileSystem::CFileStream& file)
{
	file.write(&u_flags, sizeof(int), 1);

	return true;
}

bool ATriggerFlags::openFlags(FileSystem::CFileStream& file, int version)
{
	file.read(&u_flags, sizeof(int), 1);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

bool CSignTrigger::save(FileSystem::CFileStream& file)
{
	if (!saveId(file))
	{
		return false;
	}
	if (!saveDescription(file))
	{
		return false;
	}

	return true;
}

bool CSignTrigger::open(FileSystem::CFileStream& file, int version)
{
	if (!openId(file, version))
	{
		return false;
	}
	if (!openDescription(file, version))
	{
		return false;
	}

	return true;
}

void CSignTrigger::action_Look(Player::CServerCharacter* target)
{
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

unsigned int CAttentionTrigger::TRIGGERFLAGS_PLAYSOUND	= 0x1;

bool CAttentionTrigger::save(FileSystem::CFileStream& file)
{
	if (!saveId(file))
	{
		return false;
	}
	if (!saveDescription(file))
	{
		return false;
	}
	if (!saveFlags(file))
	{
		return false;
	}

	return true;
}

bool CAttentionTrigger::open(FileSystem::CFileStream& file, int version)
{
	if (!openId(file, version))
	{
		return false;
	}
	if (!openDescription(file, version))
	{
		return false;
	}
	if (!openFlags(file, version))
	{
		return false;
	}

	return true;
}

void CAttentionTrigger::action_Walk(Player::CServerCharacter* target)
{
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

bool CEntranceTrigger::save(FileSystem::CFileStream& file)
{
	if (!saveId(file))
	{
		return false;
	}
	if (!saveDescription(file))
	{
		return false;
	}
	if (!saveLocation(file))
	{
		return false;
	}

	return true;
}

bool CEntranceTrigger::open(FileSystem::CFileStream& file, int version)
{
	if (!openId(file, version))
	{
		return false;
	}
	if (!openDescription(file, version))
	{
		return false;
	}
	if (!openLocation(file, version))
	{
		return false;
	}

	return true;
}

void CEntranceTrigger::action_Look(Player::CServerCharacter* target)
{
}

void CEntranceTrigger::action_Enter(Player::CServerCharacter* target)
{
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

unsigned int CTeleportTrigger::TRIGGERFLAGS_PLAYSOUND	= 0x1;

bool CTeleportTrigger::save(FileSystem::CFileStream& file)
{
	if (!saveId(file))
	{
		return false;
	}
	if (!saveDescription(file))
	{
		return false;
	}
	if (!saveLocation(file))
	{
		return false;
	}
	if (!saveFlags(file))
	{
		return false;
	}

	return true;
}

bool CTeleportTrigger::open(FileSystem::CFileStream& file, int version)
{
	if (!openId(file, version))
	{
		return false;
	}
	if (!openDescription(file, version))
	{
		return false;
	}
	if (!openLocation(file, version))
	{
		return false;
	}
	if (!openFlags(file, version))
	{
		return false;
	}

	return true;
}

void CTeleportTrigger::action_Walk(Player::CServerCharacter* target)
{
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****




} // end of WorldSystem namespace
