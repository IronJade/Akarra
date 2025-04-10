/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A "chat node" is an object which is used to handle NPC logic
 *    when a player talks to them. Chat nodes can either perform actions
 *    or work as boolean tests.
 **/

#ifndef _NPCCHAT_H_
#define _NPCCHAT_H_

#include "SRequirement.h"
#include "../library/Basic/CCharString.h"

// Since this file is read by Borland too we need to "hide" some functions,
// which may cause this file to look really ugly :(

namespace Player
{
	class CServerCharacter;
}

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{

	enum ENodeFileVersions
	{
		NFV_ORIGINAL = 0,

		NFV_021020,			// added buy flags
		NFV_021023,			// added portrait
		NFV_021208,			// added numitems and maxitems
		NFV_030215,			// added id numbers for each chatnode, so you can "jump" in a chat tree
		NFV_030320,			// added timeout for escort nodes, and objective state for activateobjective nodes
		NFV_030403,			// all shop info has been moved to its own class, CItemShop

		NFV_CURRENT
	};

	// this is just a name structure used to make shop keeping easier
	struct SItemName
	{
		char	name[32];
	};

	enum EShopListNumbers
	{
		SLN_COMMON = 0,
		SLN_UNCOMMON,
		SLN_RARE,
		SLN_BUY
	};
	
	class AChatNode;
	// ***************************************************************************************
	// This is the base class for the chat nodes. It is used to define where the chat starts.
	// The chat manuscript also includes shop information that the non-player characters use
	// when players want to buy/sell items. (The current list of items is stored in the NPC's
	// monster entity, though)
	class CChatManuscript
	{
		public:

			CChatManuscript();
			~CChatManuscript();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			AChatNode* getStartNode();
			void setStartNode(AChatNode* start, bool shoulddelete);
			int getNewNodeNumber() { return m_nextnodenumber++; }
			AChatNode* findNode(int idnumber);

			char* getName();
			void setName(char* name);

			int getPortrait();
			void setPortrait(int portrait);

			char* getShopName();
			void setShopName(char* shopname);

		protected:

			AChatNode*	mp_startnode;
			int			m_nextnodenumber;		// a counter that is used to give id numbers to chatnodes
			char		ma_name[32];
			int			m_portrait;				// portrait sprite shown on the client (max 255)

			// shop information:
			char		ma_shopname[32];		// which shop script the NPC should load
	};
	// ***************************************************************************************


	enum EMainNodeTypes
	{
		MNT_INVALID = -1,

		// interaction nodes
		
		MNT_CHATBOX = 0,		// this is a message box with up to 6 different answers (6 subnodes)
		MNT_TRADE,				// this is an endnode that brings up a tradebox
		MNT_CRAFT,				// same as trade, but for the craftbox instead
		MNT_ITEMSTORAGE,		// same again, but for the character's own item storage

		// test nodes

		MNT_TESTNODE = 19,			// Generic test node (the nodes below are converted to this)
		MNT_TESTATTRIBUTES = 20,	// tests a SRequirement and selects the true way if it's successful
		MNT_TESTQUESTSTATE,			// tests a Quest state (Started, Completed or Finished?)
		MNT_TESTITEM,				// tests if the character has a certain item
		MNT_TESTCOINS,				// checks if the character has enough money
		MNT_ESCORT,					// this node will make the NPC start following the player around until a certain
									// coordinate is reached... it will enter the "true" node when the area is reached
									// and "false" node if the NPC stops following the player for some reason

		// action nodes

		MNT_GIVEEXP = 40,	// gives experience to the character
		MNT_GIVECOINS,		// gives coins to the character
		MNT_GIVEITEM,		// gives an item to the character
		MNT_TAKECOINS,		// takes coins from the character
		MNT_TAKEITEM,		// takes an item from the character
		MNT_SETQUEST,		// sets a quest status for the character (will add the quest if it doesn't exist)
		MNT_TELEPORT,		// transports the character to a position
		MNT_SETSPAWN,		// sets the chararacters respawn point (to the character's own point)
		MNT_SENDMESSAGE,	// sends a one-line message to the character
		MNT_ACTIVATEOBJECTIVE,	// activates a quest objective
		MNT_SPAWNMONSTER,	// spawns a monster in the world

		// special nodes
		MNT_JUMP = 60,		// "jumps" within the chattree to a node with a matching id number

	};

	// ****************************************************************************************
	// This is the main Node class that all nodes are derived from
	class AChatNode
	{
		public:
#ifdef AKARRASERVER
			virtual AChatNode* runNode(Player::CServerCharacter* character) = 0;
#endif

			virtual bool read(FileSystem::CFileStream& file) = 0;
			virtual bool write(FileSystem::CFileStream& file) = 0;

			virtual int getNodeType() { return MNT_INVALID;	}

			static AChatNode* createNode(int type, CChatManuscript* owner);

			virtual CChatManuscript* getSourceScript();
			virtual void setSourceScript(CChatManuscript* script, int idnumber);
			virtual int getIdNumber();

			virtual AChatNode* findNode(int idnumber) = 0;

		protected:

			CChatManuscript*	mp_source;
			int					m_idnumber;

	};

	class ABooleanNode : public AChatNode
	{
		public:

			ABooleanNode();
			~ABooleanNode();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			AChatNode* getTrueNode();
			AChatNode* getFalseNode();

			void setTrueNode(AChatNode* node, bool shoulddelete);
			void setFalseNode(AChatNode* node, bool shoulddelete);

			virtual AChatNode* findNode(int idnumber);

		protected:

			AChatNode*		mp_true;
			AChatNode*		mp_false;
	};

	class AActionNode : public AChatNode
	{
		public:

			AActionNode();
			~AActionNode();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			AChatNode* getNextNode();
			void setNextNode(AChatNode* next, bool shoulddelete);

			virtual AChatNode* findNode(int idnumber);

		protected:

			AChatNode*		mp_followup;
	};

	// ****************************************************************************************
	enum ETestNodeTestTypes
	{
		TNTT_REQUIREMENT = 0,	// mainparam is requirement type, subparam is requirement value
		TNTT_QUESTSTATE,		// stringparam is questname, mainparam is quest state
		TNTT_QUESTOBJECTIVE,	// stringparam is questname, mainparam is objective number, subparam is objective state
		TNTT_ITEM,				// stringparam is itemname
		TNTT_COINS,				// mainparam is number of coins, subparam is cointype
	};
	// The testnode uses a structure to keep all possible kinds of test data. This data is used
	// differently depending on the test that is performed
	struct STestNodeData
	{
		int		testtype;			// out of ETestNodeTestTypes
		int		mainparam;
		int		subparam;
		char	stringparam[64];

		STestNodeData() : testtype(0), mainparam(0), subparam(0) { stringparam[0] = 0; }
	};
	class CTestNode : public ABooleanNode
	{
		public:

			CTestNode(int originalnode);
			~CTestNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_TESTNODE;	}

			STestNodeData& getTestData();

		protected:

			int				m_originalnode;

			STestNodeData	m_test;
	};

	// ****************************************************************************************
	class CEscortNode : public ABooleanNode
	{
		public:

			CEscortNode();
			~CEscortNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_ESCORT;	}

			void getDestRegion(int& left, int& top, int& right, int& bottom);
			void setDestRegion(int left, int top, int right, int bottom);
			int getTimeout();
			void setTimeout(int seconds);

		protected:

			// the destination region:
			int		m_left, m_top, m_right, m_bottom;
			int		m_timeout;

	};

	// ****************************************************************************************
	const int MAX_CHAT_OPTIONS = 6;

	enum EAccountChatOptions
	{
		ACO_SUCCESSFUL = 0,
		ACO_INVALIDPARAMS,
		ACO_ALREADYTAKEN,
		ACO_DOUBLEACCOUNTS,
		ACO_SYSTEMERROR
	};

	class CChatBoxNode : public AChatNode
	{
		public:

			CChatBoxNode();
			~CChatBoxNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_CHATBOX;	}

			Basic::CCharString& getChatText();
			void setChatText(Basic::CCharString& text);

			Basic::CCharString& getOptionText(int number);
			void setOptionText(int number, Basic::CCharString& text);

			AChatNode* getNode(int number);
			void setNode(int number, AChatNode* node, bool shoulddelete);

			virtual AChatNode* findNode(int idnumber);

		protected:

			Basic::CCharString	m_chattext;
			
			Basic::CCharString	ma_optiontexts[MAX_CHAT_OPTIONS];
			AChatNode*			mpa_optionnodes[MAX_CHAT_OPTIONS];
	};

	// ****************************************************************************************
	class CTradeNode : public AChatNode
	{
		public:

			CTradeNode();
			~CTradeNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_TRADE;	}

			virtual AChatNode* findNode(int idnumber);

		protected:

	};

	// ****************************************************************************************
	class CCraftNode : public AChatNode
	{
		public:

			CCraftNode();
			~CCraftNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_CRAFT;	}

			virtual AChatNode* findNode(int idnumber);

		protected:

	};

	// ****************************************************************************************
	class CStorageNode : public AChatNode
	{
		public:

			CStorageNode();
			~CStorageNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_ITEMSTORAGE;	}

			virtual AChatNode* findNode(int idnumber);

		protected:

	};

	// ****************************************************************************************
	class CJumpNode : public AChatNode
	{
		public:

			CJumpNode();
			~CJumpNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_JUMP;	}

			virtual AChatNode* findNode(int idnumber);

			int getDestination();
			void setDestination(int idnumber);

		protected:

			int		m_destinationnode;

	};

	// ****************************************************************************************
	class CGiveExpNode : public AActionNode
	{
		public:

			CGiveExpNode();
			~CGiveExpNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_GIVEEXP;	}

			int getAmount();
			void setAmount(int amount);

		protected:

			int				m_amount;

	};

	// ****************************************************************************************
	class CGiveCoinsNode : public AActionNode
	{
		public:

			CGiveCoinsNode();
			~CGiveCoinsNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_GIVECOINS;	}

			int getAmount();
			void setAmount(int amount);
			int getType();
			void setType(int type);

		protected:

			int				m_amount;
			int				m_type;		// type of coins, 0 = human, 1 = gifted, 2 = iwid
	};

	// ****************************************************************************************
	class CGiveItemNode : public AActionNode
	{
		public:

			CGiveItemNode();
			~CGiveItemNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_GIVEITEM;	}

			char* getItemTypeName();
			void setItemTypeName(char* name);

		protected:

			char			ma_itemname[32];

	};

	// ****************************************************************************************
	class CTakeCoinsNode : public AActionNode
	{
		public:

			CTakeCoinsNode();
			~CTakeCoinsNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_TAKECOINS;	}

			int getAmount();
			void setAmount(int amount);
			int getType();
			void setType(int type);

		protected:

			int				m_amount;
			int				m_type;		// type of coins, 0 = human, 1 = gifted, 2 = iwid
	};

	// ****************************************************************************************
	class CTakeItemNode : public AActionNode
	{
		public:

			CTakeItemNode();
			~CTakeItemNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_TAKEITEM;	}

			char* getItemTypeName();
			void setItemTypeName(char* name);

		protected:

			char			ma_itemname[32];

	};

	// ****************************************************************************************
	class CSetQuestStateNode : public AActionNode
	{
		public:

			CSetQuestStateNode();
			~CSetQuestStateNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_SETQUEST;	}

			char* getQuestName();
			void setQuestName(char* name);
			int getQuestState();
			void setQuestState(int state);

		protected:

			char		ma_questname[32];
			int			m_state;			// required quest state (out of EQuestState)

	};

	// ****************************************************************************************
	class CTeleportNode : public AActionNode
	{
		public:

			CTeleportNode();
			~CTeleportNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_TELEPORT;	}

			void setX(int value);
			void setY(int value);
			int getX();
			int getY();

		protected:

			int				m_x;
			int				m_y;
	};

	// ****************************************************************************************
	class CSetSpawnNode : public AActionNode
	{
		public:

			CSetSpawnNode();
			~CSetSpawnNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_SETSPAWN;	}

		protected:

	};

	// ****************************************************************************************
	class CSendMessageNode : public AActionNode
	{
		public:

			CSendMessageNode();
			~CSendMessageNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_SENDMESSAGE;	}

			char* getMessage();
			void setMessage(char* message);

		protected:

			char			ma_message[96];

	};

	// ****************************************************************************************
	class CActivateObjectiveNode : public AActionNode
	{
		public:

			CActivateObjectiveNode();
			~CActivateObjectiveNode();

#ifdef AKARRASERVER
			AChatNode* runNode(Player::CServerCharacter* character);
#endif

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			virtual int getNodeType() { return MNT_ACTIVATEOBJECTIVE;	}

			char* getQuestName();
			void setQuestName(char* name);
			int getObjectiveNumber();
			void setObjectiveNumber(int num);
			int getActionState();
			void setActionState(int state);

		protected:

			char		ma_questname[32];
			int			m_objective;			// objective number (0 to 5)
			int			m_action;				// 0 is "activate", 1 is "complete"

	};
}

#endif
