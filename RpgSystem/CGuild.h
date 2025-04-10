/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Player guild rules.
 **/

#ifndef _CGUILD_H_
#define _CGUILD_H_

#include <vector>
#include <list>
#include "../Library/basic/CCharString.h"

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{
	// NOTE: chr refers to "Character", not the type 'char'
	struct SChrName { char name[16]; };
	struct SGuildPerson
	{
		char			name[16];	// name of the member
		unsigned int	exp;		// member experience
	};
	//typedef (char[16])				TDChrName;
	typedef std::vector<SChrName>		TDChrNameVec;
	typedef std::vector<SGuildPerson>	TDGuildPersonVec;

	enum EGuildTypes
	{
		GT_GUILD_GIFTED = 0,
		GT_GUILD_SHINING,
		GT_CLAN,		// clans are smaller versions of guilds with less rules

		NUM_GUILD_TYPES
	};

	const int GUILD_TAGLENGTH	= 4;	// the length of a guild "tag"
	const int GUILD_NUMOWNERS	= 5;	// five people "own" the guild, where the first one is founder
	const int GUILD_NOSTANDING	= 666;	// should be big enough to be "outside" the table

	const int POWER_DAYS		= 5;

	const int START_POWER		= 5;		// starting power for new guilds
	const int DAILY_START_EXP	= -100000;	// this is negative so an idling clan will lose power over time (0.1 per day)
	const int DAILY_START_COINS	= 0;

	struct SGuildDiplomacySetting
	{
		char	guild[32];
		int		setting;
	};
	enum EGuildDiplomacySettings
	{
		GDS_NEUTRAL = 0,	// treat the other players as commoners
		GDS_ALLY,			// share power
		GDS_HOSTILE,		// the members of this guild can kill members of the other guild

		NUM_DIPLOMACY_SETTINGS
	};
	typedef std::vector<SGuildDiplomacySetting>	TDDiplomacyVec;

	// Guild Message Posts

	struct SGuildMessage
	{
		int					id;
		long				posttime;	// when the message was posted
		Basic::CCharString	title;
		Basic::CCharString	message;
	};
	typedef std::list<SGuildMessage*>		TDGuildMessageList;

	// *******************************************************************************************************
	enum EGuildFileVersions
	{
		GFV_ORIGINAL = 0,

		GFV_030506,		// added message list
		GFV_030802,		// the stats were bugged so clans written prior to this date will be reset

		GFV_CURRENT
	};

	class CGuild
	{
		public:

			CGuild();
			~CGuild();

			void clear();
			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			void setNewGuild(char* name, int type, int color, char* tag);
			void addOwner(char* name);

			char* getGuildName();
			char* getGuildTag();
			int getGuildTagColor();
			int getGuildType();

			bool chrIsOwner(char* name);
			bool chrIsMember(char* name);
			bool chrIsPending(char* name);

			bool addPendingChr(char* name, Basic::CCharString& failreason);
			bool denyPendingChr(char* name, char* leader, Basic::CCharString& failreason);
			bool invitePendingToMember(char* name, char* leader, Basic::CCharString& failreason);

			bool removeMember(char* name, char* leader);
			bool demoteOwner(char* name, Basic::CCharString& failreason);
			bool promoteMember(char* name, char* leader, Basic::CCharString& failreason);

			void addMemberExperience(int newexp);

			// power functions

			unsigned long getTotalExp();
			unsigned long getTotalWealth();
			float getTotalPower();
			float getBasePower();
			float getDayPower(int day);
			float getAllySharePower();
			void addAllyPower(float power);
			void updateDailyPower();

			void reportStanding(int place);
			int getStanding();
			float getStandingExpBonus(bool isowner);

			// ...

			TDGuildPersonVec& getOwnerList();
			TDGuildPersonVec& getMemberList();
			TDGuildPersonVec& getPendingList();

			// diplomacy

			TDDiplomacyVec& getDiplomacyList();
			void setGuildDiplomacy(char* guildname, int diplomacy);
			int getGuildDiplomacy(char* guildname);
			bool isHostile(char* guildname);
			bool isAlly(char* guildname);

			// Guild Messages

			void addMessage(Basic::CCharString& title, Basic::CCharString& message);
			TDGuildMessageList& getMessageList();
			SGuildMessage* getMessage(int id);

		protected:
			
			//void m_recalculatePower();

		protected:

			char				ma_guildname[32];				// guild name!
			int					m_guildtype;					// out of EGuildTypes

			int					m_tagcolor;						// 1 to 9 (used as font color of the tag)
			char				ma_tag[GUILD_TAGLENGTH + 1];	// one extra for null terminator

			TDGuildPersonVec	m_owners;						// never more than 5
			TDGuildPersonVec	m_members;
			TDGuildPersonVec	m_pending;						// characters who are waiting to get included in the guild

			// power stuff

			int					m_exptoday;						// how much exp that has been added today
			int					m_wealthtoday;					// how much money that has been added today
			unsigned long		m_totalexp;						// total experience for the whole guild's lifetime
			unsigned long		m_totalwealth;					// total amount of copper coins in the guild
			float				ma_dailypower[POWER_DAYS];		// the power for the last 5 days
			float				m_basepower;					// power that has been built up by daypowers (10% is saved each day and 2% is shared with allies)
			float				m_allypower;					// power from the last day (stored for allies to avoid multiple ally powers)

			int					m_place;
			float				m_expbonus;
			float				m_ownerexpbonus;

			// diplomacy settings towards all other guilds
			TDDiplomacyVec	m_diplomacy;

			// messages
			TDGuildMessageList	m_messages;
			int					m_nextmessageid;

	};
	typedef std::vector<CGuild*>	TDGuildVec;

	// *******************************************************************************************************
	// The guild handler loads, handles and saves all guilds for the world manager. When a player connects it
	// should request his guild info from the guild handler.
	// *******************************************************************************************************
	class CGuildHandler
	{
		public:

			CGuildHandler();
			~CGuildHandler();

			void clear();
			void removeAll();

			bool loadAllGuilds();
			bool saveAllGuilds();
			bool saveGuild(char* guildname);

			bool addGuild(CGuild* guild);
			bool removeGuild(char* guildname);

			CGuild* getGuild(char* guildname);
			TDGuildVec& getGuildList();
			int getNumGuilds();

		protected:

			TDGuildVec	m_guilds;

	};

	// *******************************************************************************************************
	enum ECharacterGuildStates
	{
		CGS_INVALID = 0,
		CGS_PENDING,		// waits to be accepted
		CGS_MEMBER,			// is a member
		CGS_OWNER,			// is an owner (is also a member)

		NUM_CHARACTER_GUILD_STATES
	};
	struct SCharacterGuildState
	{
		char	guildname[32];	// name of the guild the player has joined (or wants to join)
		int		state;
	};

}


#endif
