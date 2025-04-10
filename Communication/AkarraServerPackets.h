
#ifndef _AKARRASERVERPACKETS_H_
#define _AKARRASERVERPACKETS_H_

#include "AkarraPackets.h"
#include "SGenericPacket.h"
#include "../PlayerSystem/AkarraAccount.h"
#include "../PlayerSystem/AkarraCharacter.h"

namespace Comm
{

	// PRE-GAME PACKETS ******************************************************************************

	// AP_S_WELCOME
	struct SSPWelcome
	{
		SGenericPacket			header;
		Player::SAccountFile	account;	// remember to clear the password before you send this packet
		int						code;		// welcome code, depending on latest action
		int						users;		// number of connected users
	};
	enum ESPWelcomeCodes
	{
		SPWC_UNKNOWN = 0,
		SPWC_LOGGEDIN,				// the user has successfully logged in to an existing account
		SPWC_CREATEDACCOUNT,		// the user has created a new account
		SPWC_CREATEDCHARACTER,		// the user has created a new character
		SPWC_DELETEDCHARACTER,		// the user has deleted one of his characters
		SPWC_CHANGEDPASSWORD,		// the user has changed his password

		SPWC_ACTIONFAILED = 10,		// the last action failed
	};

	// AP_S_GOODBYE
	struct SSPGoodbye
	{
		SGenericPacket	header;
		int				reason;
	};
	enum ESPGoodbyeReasons
	{
		SPGR_UNKNOWN = 0,
		SPGR_NORMAL,				// the user has disconnected by normal means
		SPGR_INCORRECTVERSION,		// the user hasn't got the latest client exe

		// account creation codes
		SPGR_AC_ALREADYEXISTS = 10,	// the account name already exists
		SPGR_AC_INVALIDNAME,		// the user name is not acceptable (the client should filter these)
		SPGR_AC_INVALIDPASS,		// the password is not acceptable (the client should filter these)
		SPGR_AC_INVALIDEMAIL,		// the email is not acceptable (the client should filter these)
		SPGR_AC_UNABLETOCREATE,		// unable to create the account, unknown reason

		// account logging in codes
		SPGR_AL_DOESNTEXIST = 20,	// can't find account name
		SPGR_AL_INVALIDNAME,
		SPGR_AL_INVALIDPASS,		// password is incorrect
		SPGR_AL_ALREADYLOGGEDIN,	// can't login to an account that already is in use
		SPGR_AL_BANNED,				// the account has been banned

		// other
		SPGR_KICKED = 30			// the user has been kicked by the administration crew
	};

	// AP_S_CHARACTERRESULT
	struct SSPCharacterResult
	{
		SGenericPacket	header;
		int				result;
	};
	enum ESPCharacterResults
	{
		SPCR_UNKNOWN = 0,			// count this message as a failed attempt

		// success codes
		SPCR_S_SUCCESS = 10,

		// failure codes
		SPCR_F_INVALIDNAME = 20,	// the character name is not acceptable (should be filtered by the client)
		SPCR_F_ALREADYEXISTS,		// the character name already exists
		SPCR_F_ISINVALID,			// the character information is invalid for some reason (hack attempt)
		SPCR_F_SLOTISTAKEN,			// there already is a character in that account slot
		SPCR_F_UNABLETOCREATE,		// unknown error
	};

	// AP_S_WORLDRESULT
	struct SSPWorldResult
	{
		SGenericPacket	header;
		int				result;
	};
	enum ESPWorldResults
	{
		SPWR_UNKNOWN = 0,

		SPWR_S_SUCCESS = 10,

		SPWR_F_EMPTYSLOT = 20,	// there's no character in the selected slot for that account
		SPWR_F_INVALIDSLOT,
		SPWR_F_INVALIDCHARACTER,	// unable to load the selected character
		SPWR_F_UNKOWNERROR,		// unknown error
	};

	// GAME PACKETS **********************************************************************************

	// AP_S_ECHO
	struct SSPEcho
	{
		SGenericPacket	header;
		int				echotype;
		unsigned char	message[128];
	};
	enum ESPEchoTypes
	{
		SPET_NORMAL = 0,	// normal message
		SPET_TELL,			// a 'tell' message from one player to another
		SPET_FELLOWSHIP,	// a 'fellowship' message to all fellowship members
		SPET_GUILD,			// a message to all guild members
		SPET_SYSTEM			// a system message
	};

	// AP_S_SYSTEMNOTICE
	struct SSPSystemNotice
	{
		SGenericPacket	header;
		unsigned char	message[128*4];
	};

	// AP_S_CHRINFOFULL
	//struct SSPCharacterInfoFull
	//{
	//	SGenericPacket			header;
	//	Player::CCharacterFile	character;	// the character information, note that this class doesn't
	//										// have a predictable size
	//};

	// AP_S_CHRINFO
	//struct SSPCharacterInfo
	//{
	//	SGenericPacket	header;
	//	char			u_name[Player::CHARACTER_NAMELENGTH];	// the character name
	//	char			pclass;									// class
	//	short			hp;
	//	short			maxhp;
	//	short			mp;
	//	short			maxmp;
	//};

	// AP_S_CHRTRANSER
	//struct SSPCharacterTransfer
	//{
	//	SGenericPacket	header;
	//	unsigned int	globalx;	// is converted to a sector id and area number
	//	unsigned int	globaly;
	//};

	// AP_S_THISISYOU
	struct SSPThisIsYou
	{
		SGenericPacket	header;
		int				id;			// entity id number

		unsigned char	addflags;	// some flags (same as when another player is added)
		unsigned char	padding[2];

		// note: since this packet is sent before the character joins the world, this information may or
		// may not be correct (most of the time it is correct, though)
		unsigned char	guildcolor;
		char			guildtag[4];

		Player::CMinimumCharacterInfo stats;	// character information
	};

	// AP_S_CHRUPDATE
	// This packet is actually only a header for a much larger data buffer
	struct SSPCharacterUpdate
	{
		SGenericPacket	header;
		unsigned int	systemtime;	// server system time, required since packets might arrive in any order
	};

	// AP_S_UPDATESTATS
	struct SSPUpdateStats
	{
		SGenericPacket			header;
		
		RpgSystem::CSmallStats	stats;
	};

	// AP_S_UPDATEINVENTORY
	struct SSPUpdateInventory
	{
		SGenericPacket				header;
		RpgSystem::CSmallInventory	inventory;
	};

	// AP_S_MOTD256
	struct SSPMotd256
	{
		SGenericPacket				header;
		char						motd[256];
	};
	// AP_S_MOTD512
	struct SSPMotd512
	{
		SGenericPacket				header;
		char						motd[512];
	};
	// AP_S_MOTD1024
	struct SSPMotd1024
	{
		SGenericPacket				header;
		char						motd[1024];
	};
	// AP_S_MOTD2048
	struct SSPMotd2048
	{
		SGenericPacket				header;
		char						motd[2048];
	};

}


#endif
