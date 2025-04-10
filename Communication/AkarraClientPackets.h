
#ifndef _AKARRACLIENTPACKETS_H_
#define _AKARRACLIENTPACKETS_H_

#include "AkarraPackets.h"
#include "SGenericPacket.h"
#include "../PlayerSystem/AkarraAccount.h"

namespace Comm
{

	// PRE-GAME PACKETS ******************************************************************************

	// AP_C_NEWACCOUNT
	struct SCPNewAccount
	{
		SGenericPacket	header;
		int				clientversion;	// the version of the client exe
		char			password[Player::ACCOUNT_PASSWORDLENGTH];
		char			email[Player::ACCOUNT_EMAILLENGTH];
	};

	// AP_C_LOGIN
	struct SCPLogin
	{
		SGenericPacket	header;
		int				clientversion;	// the version of the client exe
		char			password[Player::ACCOUNT_PASSWORDLENGTH];	// encrypt this?
	};

	// AP_C_NEWCHARACTER
	struct SCPNewCharacter
	{
		SGenericPacket	header;
		char			name[Player::ACCOUNT_NAMELENGTH];	// character name
		char			slot;	// player slot (there are 5 to choose from)
		char			god;	// shining or gifted?
		char			soul;	// white, blue, green, red or yellow?
		char			gender;	// male or beautiful?

		// appearance parameters:
		
		unsigned char	pose;
		unsigned char	skin;	// this is used for both skin and eye color
		unsigned char	hair;
		unsigned char	style;
	};

	// AP_C_DELCHARACTER
	struct SCPDeleteCharacter
	{
		SGenericPacket	header;
		char			name[Player::ACCOUNT_NAMELENGTH];	// character name
		int				position;					// character position (so mistakes can be avoided)
	};

	// AP_C_SETPASSWORD
	struct SCPSetPassword
	{
		SGenericPacket	header;
		char			oldp[Player::ACCOUNT_PASSWORDLENGTH];	// encrypt this?
		char			newp[Player::ACCOUNT_PASSWORDLENGTH];	// encrypt this?
	};

	// GAME PACKETS **********************************************************************************

	// AP_C_JOINWORLD
	struct SCPJoinWorld
	{
		SGenericPacket	header;
		char			slot;	// selected character slot
	};

	// AP_C_AREAVERSIONS
	struct SCPAreaVersions
	{
		SGenericPacket	header;
		unsigned char	reserved[2];	// to keep the struct aligned by 4 bytes
		unsigned short	center;
		unsigned short	north;
		unsigned short	south;
		unsigned short	west;
		unsigned short	east;
		unsigned short	northwest;
		unsigned short	northeast;
		unsigned short	southwest;
		unsigned short	southeast;
	};

	// AP_C_MOVE
	struct SCPMove
	{
		SGenericPacket	header;
		unsigned char	movetype;	// bits: xxxx yyyy, where xxxx is movement type and yyyy is direction
	};
	const unsigned char	MT_RIGHT		= 0x01;
	const unsigned char	MT_UPRIGHT		= 0x02;
	const unsigned char	MT_UP			= 0x03;
	const unsigned char	MT_UPLEFT		= 0x04;
	const unsigned char	MT_LEFT			= 0x05;
	const unsigned char	MT_DOWNLEFT		= 0x06;
	const unsigned char	MT_DOWN			= 0x07;
	const unsigned char	MT_DOWNRIGHT	= 0x08;

	const unsigned char	MT_SINGLESTEP	= 0x10;
	const unsigned char	MT_CONTINOUS	= 0x20;

	// AP_C_ENTER
	struct SCPEnter
	{
		SGenericPacket	header;
		unsigned int	globalx;	// world position of the target entrance trigger
		unsigned int	globaly;
	};

	enum ECommandLengths
	{
		CL_TINY = 16,
		CL_SMALL = 32,
		CL_NORMAL = 64,
		CL_BIG = 96,
		CL_HUGE = 128
	};
	// AP_C_COMMAND
	struct SCPCommand_Tiny
	{
		SGenericPacket	header;
		char			command[CL_TINY];
	};
	struct SCPCommand_Small
	{
		SGenericPacket	header;
		char			command[CL_SMALL];
	};
	struct SCPCommand_Normal
	{
		SGenericPacket	header;
		char			command[CL_NORMAL];
	};
	struct SCPCommand_Big
	{
		SGenericPacket	header;
		char			command[CL_BIG];
	};
	struct SCPCommand_Huge
	{
		SGenericPacket	header;
		char			command[CL_HUGE];
	};

	// AP_C_REQUESTENTITY
	struct SCPRequestEntity
	{
		SGenericPacket	header;
		int				id;
	};

	// AP_C_REQUESTSKILL
	struct SCPRequestSkillData
	{
		SGenericPacket	header;
		int				skillnumber;	// which skill that needs to be updated
	};

	// AP_C_REQUESTMONSTERTYPE
	struct SCPRequestMonsterType
	{
		SGenericPacket	header;
		int				monsterid;	// which monster runtime id that needs to be updated
	};

	//AP_C_DEFAULTACTION
	const int	DEFAULTACTION_MASK				= 0xFF;		// max 255 different default actions
	const int	DEFAULTACTION_SPELLNUMBER_MASK	= 0xFFFF00;	// used for DA_CASTSPELLTARGET and DA_CASTSPELLPOS
	struct SCPDefaultAction
	{
		SGenericPacket	header;

		int				action;
		int				parameter;
		int				additional;	// second parameter
	};
	enum EDefaultActions
	{
		DA_NONE = 0,

		DA_USEATTRIBUTE,	// the player wants to increase an attribute (parameter tells which one)

		DA_ATTACKPLAYER,	// temporary action that is used to attack another player (parameter tells which one by gid)
		DA_RESPAWN,			// the clients wants to return to life
		DA_REST,			// the client wants to lie down and rest

		DA_LOOK,			// the client wants to look at a cell at (parameter, additional) world position
		DA_FOLLOWPLAYER,	// follows a player around the world
		DA_USE,				// use an cell
		DA_ENTER,			// "enter" a cell (entrance triggers)

		DA_MOVEITEM,		// moves an item in the inventory
		DA_USEITEM,			// uses an item in the inventory
		DA_PICKUP,			// pick ups any item that lies at the cell (parameter, additional)
		DA_LOOKITEM,		// investigates the details about an item

		DA_TRADEPLAYER,		// starts a trading session with another player
		DA_CANCELTRADE,		// stops trading without transferring any items or money
		DA_SETTRADESTATE,	// changes between "accepting" and "not accepting" the trade offer
		DA_SETTRADEMONEY,	// sets the amount of money the player wishes to trade
		DA_SETTRADEOFFERNAME,	// sets if the player offers to clear the other player's PK flag

		DA_GENERICTILE,		// "generic tile action", the player doesn't know what it wants to do, so we choose for it

		DA_ADDSKILLGEM,
		DA_REMOVESKILLGEM,
		DA_SPECIALIZESKILL,

		DA_TALKNPC,			// starts a npc chat script with the targetted npc
		DA_MAKEOPTION,		// used during npc chat sessions to make a chat option

		DA_LOOKNPCITEM,		// looks at an item in the npc trade list
		DA_BUYNPCITEM,		// buys an item from the npc trade list
		DA_SELLITEM,		// sells an item from the backpack to the npc

		DA_CASTSPELLTARGET,	// casts a spell on a specific target (such as a monster)
		DA_CASTSPELLPOS,	// casts a spell on a cell in the world

		DA_FEEDMOUNT,		// restores a mount's lifetime (for a prize)
		DA_DISCARDITEM,		// destroys an item in the backpack (param is item id)

		DA_CHANGECLASS,		// changes the character's class

		DA_CRCCHECK,		// anti-hacking

		NUM_DEFAULTACTIONS
	};

	// AP_C_THREEACTION
	struct SCPThreeAction
	{
		SGenericPacket	header;

		int				action;
		int				param1;
		int				param2;
		int				param3;
	};
	enum EThreeActions
	{
		TA_NONE = 0,

		TA_USEITEMONWORLD,	// uses an item on a world coordinate

		NUM_THREEACTIONS
	};

	// AP_CA_UPLOADDATA

	// This packet hasn't got a structure, but is created on the fly depending on the object's
	// size. The packet has a normal generic header and then another integer that indentifies
	// the data type, according to this enum:
	enum EUploadDataTypes
	{
		UDT_SPAWN = 0,		// a dynamic spawn
		UDT_ITEM,			// item type
		UDT_MONSTER,		// monster type
		UDT_EFFECT,			// effect type

		NUM_UPLOADDATATYPES
	};
}


#endif
