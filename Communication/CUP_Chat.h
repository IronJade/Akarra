
#ifndef _CUP_CHAT_H_
#define _CUP_CHAT_H_

namespace Comm
{

	// CUP_M_CHATMESSAGE -----------------------------------------------------

	enum EChatMessageTypes
	{
		// standard chat messages

		CMT_SAY = 0,	// global say.			'Khaile says, "Hello there"'
		CMT_TELL,		// personal whispering. 'Khaile tells you, "Hello there"'
		CMT_TELLOTHER,	// wispering to other	'You told Khaile, "Hello there"'
		CMT_GUILD,		// guild chat.			'Khaile says, "Hello all!"'
		CMT_FELLOWSHIP,	// fellowship chat.		'Khaile says, "Let's go!"'
		CMT_ACTION,		// an emote command.	'* Khaile jumps up and down'

		CMT_EVENT,		// global admin shout	'Khaile shouts, "Event will start soon!"'
		CMT_OOC,		// global send.			'OOC: Khaile says, "HELLO?"'
		CMT_TRADE,		// global send.			'Trade: Khaile says, "HELLO?"'

		// system messages:

		CMT_ADMIN,		// system messages.			'<ADMIN> Error: Khaile not found'
		CMT_DEFAULT,	// default messages without prefix	'You don't see anything here'
		CMT_COMBAT,
	};

	enum EChatInfoTypes
	{
		CIT_SAY = 0,
		CIT_TELL,			// what others tell you
		CIT_GUILD,			// guild chat
		CIT_FELLOWSHIP,		// fellowship chat
		CIT_ACTION,			// an emote command

		CIT_EVENT,			// global send
		CIT_OOC,			// global send
		CIT_TRADE,			// global send

		// system messages:

		CIT_ADMIN,		// system messages.			'<ADMIN> Error: Khaile not found'
		CIT_COMBAT,
		CIT_ERROR,		// standard error messages (flood protection etc)
		CIT_COMMAND,	// command information (can be errors too)
		CIT_TEMPFEEDBACK,	// temporary feedback message ("Examining item...")

		// other messages

		CIT_STATSCHANGE,
		CIT_EXPERIENCE,
		CIT_DEATH,
		CIT_MONEY,

		NUM_CHATINFOTYPES
	};


	const int	CHATFLAG_AFK	= 0x1;	// the target is afk
	const int	CHATFLAG_MUTE	= 0x2;	// the target is globally muted
	enum EChatSendSizes
	{
		CSS_TINY	= 16,
		CSS_SMALL	= 32,
		CSS_NORMAL	= 64,
		CSS_BIG		= 96,
		CSS_HUGE	= 128
	};

	struct SCUPMChatMessage_Tiny
	{
		unsigned char	type;
		unsigned char	chattype;		// what kind of chat type (shout, tell, emote)
		unsigned char	messagetype;	// what kind of event (monster death, combat text, admin, player info)

		unsigned char	flags;
		int				id;				// the source entity
		char			message[CSS_TINY];
	};
	struct SCUPMChatMessage_Small
	{
		unsigned char	type;
		unsigned char	chattype;		// what kind of chat type (shout, tell, emote)
		unsigned char	messagetype;	// what kind of event (monster death, combat text, admin, player info)

		unsigned char	flags;
		int				id;				// the source entity
		char			message[CSS_SMALL];
	};
	struct SCUPMChatMessage_Normal
	{
		unsigned char	type;
		unsigned char	chattype;		// what kind of chat type (shout, tell, emote)
		unsigned char	messagetype;	// what kind of event (monster death, combat text, admin, player info)

		unsigned char	flags;
		int				id;				// the source entity
		char			message[CSS_NORMAL];
	};
	struct SCUPMChatMessage_Big
	{
		unsigned char	type;
		unsigned char	chattype;		// what kind of chat type (shout, tell, emote)
		unsigned char	messagetype;	// what kind of event (monster death, combat text, admin, player info)

		unsigned char	flags;
		int				id;				// the source entity
		char			message[CSS_BIG];
	};
	struct SCUPMChatMessage_Huge
	{
		unsigned char	type;
		unsigned char	chattype;		// what kind of chat type (shout, tell, emote)
		unsigned char	messagetype;	// what kind of event (monster death, combat text, admin, player info)

		unsigned char	flags;
		int				id;				// the source entity
		char			message[CSS_HUGE];
	};


}


#endif
