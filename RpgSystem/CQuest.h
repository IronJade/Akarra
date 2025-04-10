/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Quest, as edited in the quest editor.
 **/

#ifndef _CQUEST_H_
#define _CQUEST_H_

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{

	enum EQuestState
	{
		QS_NOTSTARTED = 0,		// the character hasn't started this quest yet
		QS_INPROGRESS,			// the character has received the quest and is trying to finish the objectives
		QS_COMPLETED,			// all objectives have been met, but a reward hasn't been given
		QS_FINISHED,			// a reward has been given, close this quest

		NUM_QUEST_STATES
	};

	enum EQuestObjectiveState
	{
		QOS_NOTACTIVATED = 0,
		QOS_NOTCOMPLETED,
		QOS_COMPLETED,

		NUM_QUEST_OBJECTIVE_STATES
	};

	enum EQuestObjectiveTypes
	{
		QOT_DISABLED = -1,

		QOT_EXTERMINATE_TYPE = 0,	// exterminate a number of monsters of that type
		QOT_EXTERMINATE_SPAWN,		// exterminate a number of monsters located at a spawn with the objective's name
		QOT_REQUIREMENT,			// the character must fulfill a standard requirement (such as Strength 30)
		QOT_TIMER,					// this quest has a limited time, if the timer runs out the quest fails (param1 is number of seconds)
		QOT_ITEMONWORLD,			// this is triggered when the player uses an item on the world successfully... 'detail' is the itemtype dataname
		QOT_ERASESPAWN,				// the player must be in a fellowship which erases a certain spawnpoint from monsters completely
		QOT_ENTERAREA,				// must reach an area with a specific name (detail is area name)
		QOT_ESCORTTO,				// must reach an area while escorting a NPC... this quest objective is controlled by the NPC chat tree

		NUM_QUEST_OBJECTIVE_TYPES
	};

	struct SQuestObjective
	{
		int		type;		// out of EQuestObjectiveTypes
		int		param1;		// what "amount" of the type that is required
		int		param2;		// additional requirement parameter
		char	detail[32];	// detailed requirement used, for example, to identify monster spawn points
		char	desc[64];	// objective description as it is presented to the player
		int		flags;		// some special objective flags
	};
	const int	QOFLAG_HIDDEN	= 0x1;	// this objective is hidden from the player and will not appear unless a NPC activates it

	enum EQuestFileVersions
	{
		QFV_ORIGINAL = 0,

		QFV_030215,			// added flags on the quest objectives

		QFV_CURRENT
	};

	const int	MAX_QUEST_OBJECTIVES	= 6;

	// *****************************************************************************************
	// This is a "quest type" that describes a quest and its objectives. Quest types should be
	// created and loaded just like item and effect types, and then "bound" to a quest instance
	// with a legal pointer to the quest type. Quests are identified by their questnames, which
	// isn't the name that is sent to the player (for example, the quest "Rupert" will be
	// displayed as "Vermin in Rupert's Farm" on the player's side).
	class CQuestType
	{
		public:

			CQuestType();
			~CQuestType();

			void clear();
	
			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

		public:

			char				ua_name[32];		// name identifier

			char				ua_description[32];	// quest name as presented to the players
			SQuestObjective		ua_objectives[MAX_QUEST_OBJECTIVES];
	
	};

	struct SQuestObjectiveInstance
	{
		SQuestObjectiveInstance() : completed(QOS_NOTACTIVATED), counter(0) {}

		int		completed;	// out of EQuestObjectiveState
		int		counter;	// used to keep track of certain objectives, such as kill count
	};

	enum EQuestInstanceFileVersions
	{
		QIFV_ORIGINAL = 0,

		QIFV_030215,		// Changed the questobjective's 'completed' from a bool to an integer, where 0
							// means "not activated" instead of "not completed"

		QIFV_CURRENT
	};

	// ***************************************************************************************
	class CQuestInstance
	{
		public:

			CQuestInstance();
			~CQuestInstance();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			void setQuestType(CQuestType* type, bool newquest);

		public:

			CQuestType*				up_questtype;
			char					ua_questname[32];

			int						u_queststate;	// out of EQuestState
			SQuestObjectiveInstance	ua_objectives[MAX_QUEST_OBJECTIVES];
	};
}


#endif
