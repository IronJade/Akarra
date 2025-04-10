/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The player's account file.
 **/

#ifndef _AKARRAACCOUNT_H_
#define _AKARRAACCOUNT_H_

#include "../library/FileSystem/CFileStream.h"
#include "../library/Basic/CCharString.h"
#include "AkarraCharacter.h"
#include <list>

namespace Player
{

	// constants

	enum EAccountFileVersions
	{
		ACCFV_ORIGINAL = 0,

		// add new versions here

		ACCFV_021201,		// added creation date
		ACCFV_030408,		// added friend and ignore lists
		ACCFV_030515,		// added lastusehostname

		ACCOUNT_FILEVERSION
	};

	const int ACCOUNT_NAMELENGTH		= 16;	// maximum account name length, names are stored as filenames
	const int ACCOUNT_PASSWORDLENGTH	= 16;	// max password length
	const int ACCOUNT_EMAILLENGTH		= 64;	// max email address length

	struct SAccountName
	{
		char name[ACCOUNT_NAMELENGTH];
	};
	typedef std::list<SAccountName*>	TDAccountNameList;

	const int ACCOUNT_CHARACTERS		= 5;	// each account can create up to this value of characters

	const char * const ACCOUNT_FOLDER	= "accounts/";
	const char * const ACCOUNT_EXT		= ".aacc";
	const char * const ACCOUNT_DELEXT	= ".~acc";
	const char * const ACCOUNT_FOLDER2	= "accounts";	// for FileSystem::countFiles() and similar
	const char * const ACCOUNT_SEARCH	= "*.aacc";		// for FileSystem::countFiles() and similar

	const unsigned int ACCOUNTFLAG_BANNED	= 0x1;	// the account has been banned
	const unsigned int ACCOUNTFLAG_MASTER	= 0x2;	// the account owner has "master rights"
	const unsigned int ACCOUNTFLAG_PWLOCK	= 0x4;	// the account may not change password

	// this structure is used to store a client's account information to file
	struct SAccountFile
	{
		// general information

		char			name[ACCOUNT_NAMELENGTH];
		char			password[ACCOUNT_PASSWORDLENGTH];
		char			email[ACCOUNT_EMAILLENGTH];
		unsigned int	flags;

		unsigned int		lastusedate;

		// the account's characters

		char			character[ACCOUNT_CHARACTERS][CHARACTER_NAMELENGTH];

		// some statistics

		unsigned int	logins;		// number of times the user has logged in
		unsigned int	seconds;	// time, in seconds, the user has been logged in
	};
	// sizeof(SAccountFile) == 16+16+16+(5*16)+4+4 = 136

	// this structure holds information about an account when the account's user is
	// logged on
	struct CAccount
	{
		public:

			CAccount();
			~CAccount();
			void removeAll();

			bool open(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			void copyOther(CAccount* other);

		public:

			SAccountFile		u_fileinfo;
			bool				u_inuse;		// true if somebody has logged in with this account

			TDAccountNameList	u_friends;
			TDAccountNameList	u_ignores;
			Basic::CCharString	u_lastusehostname;

		protected:

			void clear();
	};

} // end of Admin namespace

#endif
