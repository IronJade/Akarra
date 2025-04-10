/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The player's account file.
 **/

#include <windows.h>
#include "AkarraAccount.h"
#include <time.h>
#include "../library/filesystem/filesystem.h"

namespace Player
{

CAccount::CAccount()
:
	u_inuse(false)
{
	clear();
}

CAccount::~CAccount()
{
	removeAll();
}

bool CAccount::open(FileSystem::CFileStream& file)
{
	removeAll();

	int version;
	file.read(&version, sizeof(int), 1);

	if (version > ACCFV_021201)
	{
		file.read(u_fileinfo.name, sizeof(char), ACCOUNT_NAMELENGTH);
		file.read(u_fileinfo.password, sizeof(char), ACCOUNT_PASSWORDLENGTH);
		file.read(u_fileinfo.email, sizeof(char), ACCOUNT_EMAILLENGTH);
		file.read(&u_fileinfo.flags, sizeof(unsigned int), 1);
		file.read(&u_fileinfo.lastusedate, sizeof(unsigned int), 1);
		file.read(u_fileinfo.character, sizeof(char), ACCOUNT_CHARACTERS * CHARACTER_NAMELENGTH);
		file.read(&u_fileinfo.logins, sizeof(unsigned int), 1);
		file.read(&u_fileinfo.seconds, sizeof(unsigned int), 1);

		if (version > ACCFV_030408)
		{
			{
				int count(0);
				file.read(&count, sizeof(int), 1);

				for (int i(0); i < count; i++)
				{
					SAccountName* name = new SAccountName;
					file.read(name->name, sizeof(char), ACCOUNT_NAMELENGTH);
					u_friends.push_back(name);
				}
			}
			{
				int count(0);
				file.read(&count, sizeof(int), 1);

				for (int i(0); i < count; i++)
				{
					SAccountName* name = new SAccountName;
					file.read(name->name, sizeof(char), ACCOUNT_NAMELENGTH);
					u_ignores.push_back(name);
				}
			}
		}

		if (version > ACCFV_030515)
		{
			u_lastusehostname = FileSystem::readStringWithLength(file);
		}
	}
	else
	{
		file.read(u_fileinfo.name, sizeof(char), ACCOUNT_NAMELENGTH);
		file.read(u_fileinfo.password, sizeof(char), ACCOUNT_PASSWORDLENGTH);
		file.read(u_fileinfo.email, sizeof(char), ACCOUNT_EMAILLENGTH);
		file.read(&u_fileinfo.flags, sizeof(unsigned int), 1);
		file.read(u_fileinfo.character, sizeof(char), ACCOUNT_CHARACTERS * CHARACTER_NAMELENGTH);
		file.read(&u_fileinfo.logins, sizeof(unsigned int), 1);
		file.read(&u_fileinfo.seconds, sizeof(unsigned int), 1);

		u_fileinfo.lastusedate = time(0);
	}
	//file.read(&fileinfo, sizeof(SAccountFile), 1);
	return true;
}

bool CAccount::write(FileSystem::CFileStream& file)
{
	int version = ACCOUNT_FILEVERSION;
	file.write(&version, sizeof(int), 1);
	//file.write(&fileinfo, sizeof(SAccountFile), 1);

	u_fileinfo.lastusedate = time(0);

	file.write(u_fileinfo.name, sizeof(char), ACCOUNT_NAMELENGTH);
	file.write(u_fileinfo.password, sizeof(char), ACCOUNT_PASSWORDLENGTH);
	file.write(u_fileinfo.email, sizeof(char), ACCOUNT_EMAILLENGTH);
	file.write(&u_fileinfo.flags, sizeof(unsigned int), 1);
	file.write(&u_fileinfo.lastusedate, sizeof(unsigned int), 1);
	file.write(u_fileinfo.character, sizeof(char), ACCOUNT_CHARACTERS * CHARACTER_NAMELENGTH);
	file.write(&u_fileinfo.logins, sizeof(unsigned int), 1);
	file.write(&u_fileinfo.seconds, sizeof(unsigned int), 1);

	{
		int count = (int)u_friends.size();
		file.write(&count, sizeof(int), 1);

		TDAccountNameList::iterator name;
		for (name = u_friends.begin(); name != u_friends.end(); ++name)
		{
			file.write((*name)->name, sizeof(char), ACCOUNT_NAMELENGTH);
		}
	}
	{
		int count = (int)u_ignores.size();
		file.write(&count, sizeof(int), 1);

		TDAccountNameList::iterator name;
		for (name = u_ignores.begin(); name != u_ignores.end(); ++name)
		{
			file.write((*name)->name, sizeof(char), ACCOUNT_NAMELENGTH);
		}
	}

	FileSystem::writeStringWithLength(file, u_lastusehostname);

	return true;
}

void CAccount::copyOther(CAccount* other)
{
	removeAll();

	strcpy(u_fileinfo.character[0], other->u_fileinfo.character[0]);
	strcpy(u_fileinfo.character[1], other->u_fileinfo.character[1]);
	strcpy(u_fileinfo.character[2], other->u_fileinfo.character[2]);
	strcpy(u_fileinfo.character[3], other->u_fileinfo.character[3]);
	strcpy(u_fileinfo.character[4], other->u_fileinfo.character[4]);
	strcpy(u_fileinfo.email, other->u_fileinfo.email);
	strcpy(u_fileinfo.name, other->u_fileinfo.name);
	strcpy(u_fileinfo.password, other->u_fileinfo.password);
	u_fileinfo.flags = other->u_fileinfo.flags;
	u_fileinfo.lastusedate = other->u_fileinfo.lastusedate;
	u_lastusehostname = other->u_lastusehostname;
	u_fileinfo.logins = other->u_fileinfo.logins;
	u_fileinfo.seconds = other->u_fileinfo.seconds;
	u_inuse = other->u_inuse;

	{
		TDAccountNameList::iterator name;
		for (name = other->u_friends.begin(); name != other->u_friends.end(); ++name)
		{
			SAccountName* aname = new SAccountName;
			strcpy(aname->name, (*name)->name);
			u_friends.push_back(aname);
		}
	}
	{
		TDAccountNameList::iterator name;
		for (name = other->u_ignores.begin(); name != other->u_ignores.end(); ++name)
		{
			SAccountName* aname = new SAccountName;
			strcpy(aname->name, (*name)->name);
			u_ignores.push_back(aname);
		}
	}

}

void CAccount::clear()
{
	u_fileinfo.character[0][0] = 0;
	u_fileinfo.character[1][0] = 0;
	u_fileinfo.character[2][0] = 0;
	u_fileinfo.character[3][0] = 0;
	u_fileinfo.character[4][0] = 0;
	u_fileinfo.email[0] = 0;
	u_fileinfo.flags = 0;
	u_fileinfo.lastusedate = 0;
	u_lastusehostname = "";
	u_fileinfo.logins = 0;
	u_fileinfo.name[0] = 0;
	u_fileinfo.password[0] = 0;
	u_fileinfo.seconds = 0;
	u_inuse = false;

	u_friends.clear();
	u_ignores.clear();
}

void CAccount::removeAll()
{
	{
		TDAccountNameList::iterator name;
		for (name = u_friends.begin(); name != u_friends.end(); ++name)
		{
			delete (*name);
		}
	}
	{
		TDAccountNameList::iterator name;
		for (name = u_ignores.begin(); name != u_ignores.end(); ++name)
		{
			delete (*name);
		}
	}

	clear();
}

}
