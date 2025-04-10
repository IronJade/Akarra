
#ifndef _CUP_NPCQUESTS_H_
#define _CUP_NPCQUESTS_H_

namespace Comm
{

	// CUP_NPC_CHATBOX ---------------------------------------------------------------------------
	struct SCUPNPCChatBox
	{
		unsigned char	type;
		unsigned char	buffer[2];

		unsigned char	portrait;
		int				id;		// entity number of the NPC

		char			text[512];
		char			options[6][32];
	};

	// CUP_NPC_CLOSE -------------------------------------------------------------------------
	struct SCUPNPCClose
	{
		unsigned char	type;
		unsigned char	buffer[3];
	};

	// CUP_Q_QUESTINFO ---------------------------------------------------------------------------
	const unsigned char	INITIAL_QUEST	= 0x1;	// set when the quest info is sent on player connection
	const unsigned char	FAILED_QUEST	= 0x2;	// set when the quest info is sent on player connection
	struct SCUPQQuestInfo
	{
		unsigned char	type;
		unsigned char	buffer;

		unsigned char	flags;
		unsigned char	objstates;	// bits are set to 1 if the objective has been met

		char			desc[32];	// quest description
		char			obj[6][64];	// objective descriptions
	};

	// CUP_Q_QUESTINFODONE ---------------------------------------------------------------------
	// This packet is also sent when a quest has failed, using the FAILED_QUEST questflag.
	struct SCUPQQuestInfoDone
	{
		unsigned char	type;
		unsigned char	buffer[2];

		unsigned char	flags;

		char			desc[32];	// quest description
	};

	// CUP_Q_OBJECTIVECHANGE ---------------------------------------------------------------------
	struct SCUPQObjectiveChange
	{
		unsigned char	type;
		unsigned char	buffer;

		unsigned char	objectivenum;
		unsigned char	newstate;

		char			desc[32];	// quest description (used to identify quest)
	};

	// CUP_Q_OBJECTIVEUPDATE ---------------------------------------------------------------------
	struct SCUPQObjectiveUpdate
	{
		unsigned char	type;

		unsigned char	objectivenum;
		unsigned char	newstate;
		unsigned char	displaymessage;	// is non-zero if a message should be displayed on the clientside

		char			desc[32];	// quest description (used to identify quest)
		char			obj[64];	// new objective description
	};

}

#endif
