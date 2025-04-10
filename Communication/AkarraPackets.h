
#ifndef _AKARRAPACKETS_H_
#define _AKARRAPACKETS_H_

// this header file holds the list of all available akarra packets,
// both server->client and client->server

namespace Comm
{

	enum EAkarraPackets
	{
		AP_GENERIC = 0,		// never actually used, but should be here for reference

		// *** Server to client packets ***

			AP_SERVER = 1000,	// not a real message, just here as starter

			AP_S_WELCOME,	// this message tells the client that he has successfully logged in or
							// created a new account. This message holds all information about the
							// account, such as character names and login count.
			AP_S_GOODBYE,	// message that is sent before the client is disconnected, includes reason

			AP_S_CHARACTERRESULT,	// this message is sent back to a client when the client wants to
							// create a new character. The message either says success or a reason for
							// failure. When the client receives a success, he will return to the
							// character selection screen, otherwise he will be asked for a new name.
			AP_S_WORLDRESULT,	// this message is sent when the player tries to enter the world
								// with one of his characters, this message will be followed by a
								// AP_S_CHRTRANSER message if the character joins the world correctly

			// message packets

			AP_S_ECHO,			// small message that will appear in the player's chat box, can be
								// a character chat message, a 'tell' message or any kind of system
								// message
			AP_S_SYSTEMNOTICE,	// a message that will appear as a flashing icon in the player's game
								// view, includes more important messages and game guide messages
							
			// in-game character packets

			//AP_S_CHRINFOFULL,	// "full" character information, will include character stats, skills
								// and items. Note: this packet can have any size, depending on the
								// character's current status, so it must be checked properly when
								// recieved.
			//AP_S_CHRINFO,		// "limited" character information, used when a player examines another
								// player, will show name and class. This packet also holds slots for
								// health and mana, but will only be non-zero if the two characters are
								// in the same party.
			//AP_S_CHRTRANSER,	// teleports the character to a new position, used when the player enters
								// the world, when a character enters a building/cave/entrance or when a
								// character is affected by a teleport effect
			//AP_S_CHRSTATS,		// refreshes the character's stats
			//AP_S_CHRHEALTH,		// refreshes the character's health, stamina and mana (part of stats)
			//AP_S_CHRINVENTORY,	// refreshes the character's inventory

			AP_S_THISISYOU,		// First character packet the client will receive; includes information
								// about the chosen character and the character's id number.
			AP_S_CHRUPDATE,		// Each time the server updates (every 40ms or so) the characters will
								// a whole lot of messages to their clients, so instead of sending these
								// multiple messages I've decided to stuff them all into a single packet.
								// The AP_S_CHRUPDATE packet will have a header, a time reference (so the
								// client can tell which one is the most accurate) and then a data buffer
								// with variable length.
			AP_S_UPDATESTATS,	// updates the player's character stats (RpgSystem::CSmallStats)
			AP_S_UPDATEINVENTORY,	// updates the player's inventory (RpgSystem::CSmallInventory)

			// area packets

			AP_S_AREADATA,		// includes a complete area and its sector position

			// action packets

			//AP_S_NEWMOVEPOS,	// the character has moved, also includes some flags about blocking

			// entity packets

			//AP_S_NEWENTITY,		// add an entity to the player's list of active entities (an entity can
								// be a monster, another character, an item or a spell effect (etc))
			//AP_S_REMOVEENTITY,	// removes an entity from the player's list of active entities
			//AP_S_UPDATEENTITY,	// refreshes an entity's position, health or other interesting aspects

			// message of the day packets:

			AP_S_MOTD256	= 1050,	// a motd with less than 256 characters
			AP_S_MOTD512,
			AP_S_MOTD1024,
			AP_S_MOTD2048,

			// some admin packets

			AP_SA_DOWNLOADDATA = 1100,	// sends a spawn, monster or item data packet to the client, no struct because it's created on the fly

		// *** Client to server packets ***

			AP_CLIENT = 2000,	// not a real message, just here as starter

			AP_C_NEWACCOUNT,	// The client wants to create a new account, the account name will be
								// copied from DirectPlay while password and similar information is stored
								// in this packet. The server will reply with either AP_S_WELCOME or
								// AP_S_GOODBYE (success or failure)
			AP_C_LOGIN,			// the clients wants to login to the account with the same name as the
								// player. The packet holds the password and the server will either reply
								// with AP_S_WELCOME or AP_S_GOODBYE.

			// Client account administration packets. All of these packets will spawn a new
			// A_S_WELCOME packet from the server, so the client's account information will be kept
			// updated.

			AP_C_NEWCHARACTER,	// message that is sent when the player wants to create his character.
								// The packet holds all information about the new character (name, diety
								// and soul color), and the server will reply with AP_S_CHARACTERRESULT,
								// before the new A_S_WELCOME (which is only sent on creation success).
			AP_C_DELCHARACTER,	// The client wants to delete one of his characters. This message will
								// hold both the name of the character and the character's array position,
								// just to prevent mistakes. The server will not actually remove the
								// character file, just move it to a "safe location".
			AP_C_SETPASSWORD,	// Is sent if the user wants to set a new password. This packet holds both
								// the old password and the new one, for verification.

			// Client packets for the real game

			AP_C_JOINWORLD,		// the user has selected a character and wants to join the world with it
			AP_C_AREAVERSIONS,	// sends the version numbers for all areas in the character's active region,
								// will be followed by a AP_S_AREADATA from the server if the version
								// numbers are mis-matching compared to the server's areas.

			// Action packets

			AP_C_MOVE,			// moves the character to a new position (can initiate a "travel" or make
								// a single step).
			AP_C_ENTER,			// enters a building, cave or any other entrance

			AP_C_COMMAND_TINY,	// utters a command (such as "/say")
			AP_C_COMMAND_SMALL,
			AP_C_COMMAND_NORMAL,
			AP_C_COMMAND_BIG,
			AP_C_COMMAND_HUGE,

			AP_C_REQUESTENTITY,	// requests entity info based on a global id
			AP_C_REQUESTSKILL,	// if the client's skill data mismatches the server's version, the client must
								// request the new version. The reply will be send in a character update
								// packet, so the client must have joined the world with a character in
								// order to receive the skill information.

			AP_C_DEFAULTACTION,	// "default action" packet, used for everything that only requires three ints
			AP_C_THREEACTION,	// same as "default action", but with an extra parameter

			AP_C_REQUESTMONSTERTYPE,

			// some admin stuff

			AP_CA_UPLOADDATA = 2100,	// uploads a dynamic spawn, item or monster to the server (has no struct, is created on the fly)

	};
}

#endif
