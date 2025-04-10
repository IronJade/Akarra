/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A "chat node" is an object which is used to handle NPC logic
 *    when a player talks to them. Chat nodes can either perform actions
 *    or work as boolean tests.
 **/

#include "CChatNodes.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/FileSystem.h"
#ifdef AKARRASERVER
#include "../../Akarra Server/ServerPlayer/CServerCharacter.h"
#endif

namespace RpgSystem             
{

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChatManuscript::CChatManuscript()
{
	mp_startnode = NULL;
	ma_name[0] = 0;
	m_nextnodenumber = 0;

	// shop info:
	ma_shopname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChatManuscript::~CChatManuscript()
{
	delete mp_startnode;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChatManuscript::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(ma_name, sizeof(char), 32);

	if (version > NFV_030215)
	{
		file.read(&m_nextnodenumber, sizeof(int), 1);
	}

	// read child
	int type;
	file.read(&type, sizeof(int), 1);
	delete mp_startnode;
	mp_startnode = AChatNode::createNode(type, this);
	if (mp_startnode)
	{
		if (!mp_startnode->read(file))
		{
			return false;
		}
	}

	// read shop information
	if (version <= NFV_030403)
	{
		int tempint;
		char tempstring[32];

		file.read(&tempint, sizeof(int), 1);
		file.read(&tempint, sizeof(int), 1);
		file.read(&tempint, sizeof(int), 1);

		if (version > NFV_021020)
		{
			file.read(&tempint, sizeof(int), 1);
		}
		if (version > NFV_021023)
		{
			file.read(&m_portrait, sizeof(int), 1);
		}

		if (version > NFV_021208)
		{
			file.read(&tempint, sizeof(int), 1);
			file.read(&tempint, sizeof(int), 1);
		}

		// read item lists
		int count = 0;
		file.read(&count, sizeof(int), 1);
		{
			for (int i(0); i < count; i++)
			{
				file.read(tempstring, sizeof(char), 32);
			}
		}
		file.read(&count, sizeof(int), 1);
		{
			for (int i(0); i < count; i++)
			{
				file.read(tempstring, sizeof(char), 32);
			}
		}
		file.read(&count, sizeof(int), 1);
		{
			for (int i(0); i < count; i++)
			{
				file.read(tempstring, sizeof(char), 32);
			}
		}
		file.read(&count, sizeof(int), 1);
		{
			for (int i(0); i < count; i++)
			{
				file.read(tempstring, sizeof(char), 32);
			}
		}
	}
	else
	{
		file.read(ma_shopname, sizeof(char), 32);
		file.read(&m_portrait, sizeof(int), 1);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChatManuscript::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(ma_name, sizeof(char), 32);
	file.write(&m_nextnodenumber, sizeof(int), 1);

	// write start node type
	int type = MNT_INVALID;

	// write start node
	if (mp_startnode)
	{
		type = mp_startnode->getNodeType();
	}
	file.write(&type, sizeof(int), 1);
	if (mp_startnode)
	{
		if (!mp_startnode->write(file))
		{
			return false;
		}
	}

	// write shop information
	file.write(ma_shopname, sizeof(char), 32);
	file.write(&m_portrait, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CChatManuscript::getStartNode()
{
	return mp_startnode;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatManuscript::setStartNode(AChatNode* start, bool shoulddelete)
{
	if (shoulddelete)
	{
		delete mp_startnode;
	}
	mp_startnode = start;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CChatManuscript::findNode(int idnumber)
{
	if (mp_startnode)
	{
		return mp_startnode->findNode(idnumber);
	}
	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CChatManuscript::getName()
{
	return ma_name;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatManuscript::setName(char* name)
{
	if (!name)
	{
		return;
	}

	for (int i(0); i < 32; i++)
	{
		ma_name[i] = name[i];
		if (!ma_name[i])
		{
			break;
		}
	}
	ma_name[31] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CChatManuscript::getPortrait()
{
	return m_portrait;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatManuscript::setPortrait(int portrait)
{
	m_portrait = portrait;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CChatManuscript::getShopName()
{
	return ma_shopname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatManuscript::setShopName(char* name)
{
	strncpy(ma_shopname, name, 31);
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* AChatNode::createNode(int type, CChatManuscript* owner)
{
	AChatNode* created = NULL;

	switch (type)
	{
		default:
			break;

		case MNT_CHATBOX:
			created = new CChatBoxNode;
			break;
		case MNT_TRADE:
			created = new CTradeNode;
			break;
		case MNT_CRAFT:
			created = new CCraftNode;
			break;
		case MNT_ITEMSTORAGE:
			created = new CStorageNode;
			break;

		case MNT_TESTATTRIBUTES:
		case MNT_TESTQUESTSTATE:
		case MNT_TESTITEM:
		case MNT_TESTCOINS:
		case MNT_TESTNODE:
			created = new CTestNode(type);
			break;
		case MNT_ESCORT:
			created = new CEscortNode;
			break;
			
		case MNT_GIVEEXP:
			created = new CGiveExpNode;
			break;
		case MNT_GIVECOINS:
			created = new CGiveCoinsNode;
			break;
		case MNT_GIVEITEM:
			created = new CGiveItemNode;
			break;
		case MNT_TAKECOINS:
			created = new CTakeCoinsNode;
			break;
		case MNT_TAKEITEM:
			created = new CTakeItemNode;
			break;
		case MNT_SETQUEST:
			created = new CSetQuestStateNode;
			break;
		case MNT_TELEPORT:
			created = new CTeleportNode;
			break;
		case MNT_SETSPAWN:
			created = new CSetSpawnNode;
			break;
		case MNT_SENDMESSAGE:
			created = new CSendMessageNode;
			break;
		case MNT_ACTIVATEOBJECTIVE:
			created = new CActivateObjectiveNode;
			break;

		case MNT_JUMP:
			created = new CJumpNode;
			break;
	};

	if (created && owner)
	{
		created->setSourceScript(owner, owner->getNewNodeNumber());
	}

	return created;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChatManuscript* AChatNode::getSourceScript()
{
	return mp_source;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void AChatNode::setSourceScript(CChatManuscript* script, int idnumber)
{
	mp_source = script;
	m_idnumber = idnumber;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int AChatNode::getIdNumber()
{
	return m_idnumber;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
ABooleanNode::ABooleanNode()
{
	mp_true = NULL;
	mp_false = NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
ABooleanNode::~ABooleanNode()
{
	delete mp_true;
	delete mp_false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool ABooleanNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	// read children types
	int type = MNT_INVALID;

	// true child
	file.read(&type, sizeof(int), 1);
	delete mp_true;
	mp_true = createNode(type, mp_source);
	if (mp_true)
	{
		if (!mp_true->read(file))
		{
			return false;
		}
	}

	// false child
   type = MNT_INVALID;
	file.read(&type, sizeof(int), 1);
	delete mp_false;
	mp_false = createNode(type, mp_source);
	if (mp_false)
	{
		if (!mp_false->read(file))
		{
			return false;
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool ABooleanNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	// write children types
	int type = MNT_INVALID;

	// write child
	if (mp_true)
	{
		type = mp_true->getNodeType();
	}
	file.write(&type, sizeof(int), 1);
	if (mp_true)
	{
		if (!mp_true->write(file))
		{
			return false;
		}
	}

	type = MNT_INVALID;

	// write child
	if (mp_false)
	{
		type = mp_false->getNodeType();
	}
	file.write(&type, sizeof(int), 1);
	if (mp_false)
	{
		if (!mp_false->write(file))
		{
			return false;
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* ABooleanNode::getTrueNode()
{
	return mp_true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* ABooleanNode::getFalseNode()
{
	return mp_false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void ABooleanNode::setTrueNode(AChatNode* node, bool shoulddelete)
{
	if (shoulddelete)
	{
		delete mp_true;
	}
	mp_true = node;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void ABooleanNode::setFalseNode(AChatNode* node, bool shoulddelete)
{
	if (shoulddelete)
	{
		delete mp_false;
	}
	mp_false = node;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* ABooleanNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}
	AChatNode* search = NULL;
	if (mp_false)
	{
		search = mp_false->findNode(idnumber);
	}
	if (!search && mp_true)
	{
		search = mp_true->findNode(idnumber);
	}
	return search;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AActionNode::AActionNode()
{
	mp_followup = NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AActionNode::~AActionNode()
{
	delete mp_followup;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool AActionNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	// read children types
	int type = MNT_INVALID;

	// true child
	file.read(&type, sizeof(int), 1);
	delete mp_followup;
	mp_followup = createNode(type, mp_source);
	if (mp_followup)
	{
		if (!mp_followup->read(file))
		{
			return false;
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool AActionNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	// write children types
	int type = MNT_INVALID;

	// write child
	if (mp_followup)
	{
		type = mp_followup->getNodeType();
	}
	file.write(&type, sizeof(int), 1);
	if (mp_followup)
	{
		if (!mp_followup->write(file))
		{
			return false;
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* AActionNode::getNextNode()
{
	return mp_followup;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void AActionNode::setNextNode(AChatNode* next, bool shoulddelete)
{
	if (shoulddelete)
	{
		delete mp_followup;
	}
	mp_followup = next;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* AActionNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}
	if (mp_followup)
	{
		return mp_followup->findNode(idnumber);
	}
	return NULL;
}


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTestNode::CTestNode(int originalnode)
:
	m_originalnode(originalnode)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTestNode::~CTestNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CTestNode::runNode(Player::CServerCharacter* character)
{
	// test the requirement
	bool success = false;
	switch (m_test.testtype)
	{
		case TNTT_REQUIREMENT:
			{
				SRequirement req;
				req.type = m_test.mainparam;
				req.value = m_test.subparam;
				success = testRequirement(character->u_file.u_attribs, req);
			}
			break;
		case TNTT_QUESTSTATE:
			{
				success = (character->getQuestState(m_test.stringparam) == m_test.mainparam);
			}
			break;
		case TNTT_QUESTOBJECTIVE:
			{
				success = (character->getQuestObjectiveState(m_test.stringparam, m_test.mainparam) == m_test.subparam);
			}
			break;
		case TNTT_ITEM:
			{
				success = character->hasItemInInventory(m_test.stringparam);
			}
			break;
		case TNTT_COINS:
			{
				success = character->hasCoins(m_test.mainparam, m_test.subparam);
			}
			break;
	}

	if (success && mp_true)
	{
		return mp_true->runNode(character);
	}
	else if (!success && mp_false)
	{
		return mp_false->runNode(character);
	}
	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTestNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// since this class is "taking over" the other test nodes, we need to read according
	// to the test type to perform the conversion

	if (m_originalnode == MNT_TESTNODE)
	{
		// this is a "real" testnode, so read it as it should be read
		file.read(&m_test.testtype, sizeof(int), 1);
		file.read(&m_test.mainparam, sizeof(int), 1);
		file.read(&m_test.subparam, sizeof(int), 1);
		file.read(m_test.stringparam, sizeof(char), 64);
	}
	else if (m_originalnode == MNT_TESTATTRIBUTES)
	{
		// this is an old "test attributes" node... so read it accordingly
		file.read(&m_test.mainparam, sizeof(int), 1);
		file.read(&m_test.subparam, sizeof(int), 1);

		m_test.testtype = TNTT_REQUIREMENT;
	}
	else if (m_originalnode == MNT_TESTQUESTSTATE)
	{
		// this is an old "test quest state" node...
		file.read(m_test.stringparam, sizeof(char), 32);
		file.read(&m_test.mainparam, sizeof(int), 1);

		m_test.testtype = TNTT_QUESTSTATE;
	}
	else if (m_originalnode == MNT_TESTITEM)
	{
		// this is an old "test if item is in inventory" node... 
		file.read(m_test.stringparam, sizeof(char), 32);

		m_test.testtype = TNTT_ITEM;
	}
	else if (m_originalnode == MNT_TESTCOINS)
	{
		// this is an old "test if enough coins" node... 
		file.read(&m_test.mainparam, sizeof(int), 1);
		file.read(&m_test.subparam, sizeof(int), 1);

		m_test.testtype = TNTT_COINS;
	}

	return ABooleanNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTestNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write test data
	file.write(&m_test.testtype, sizeof(int), 1);
	file.write(&m_test.mainparam, sizeof(int), 1);
	file.write(&m_test.subparam, sizeof(int), 1);
	file.write(m_test.stringparam, sizeof(char), 64);

	return ABooleanNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
STestNodeData& CTestNode::getTestData()
{
	return m_test;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEscortNode::CEscortNode()
:
	ABooleanNode(),
	m_left(0), m_top(0), m_right(0), m_bottom(0), m_timeout(300)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CEscortNode::~CEscortNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CEscortNode::runNode(Player::CServerCharacter* character)
{
	// first of all we need to check if this is the first time the character uses this
	// node, because if it is we must make the NPC start following the character
	if (!character->isCurrentChatNode(this))
	{
		if (!character->startNPCEscort(m_timeout))
		{
			return NULL;
		}
		return this;
	}

	// now check if the escorting has stopped... because that means we have failed
	if (!character->isEscorting())
	{
		if (mp_false)
		{
			return mp_false->runNode(character);
		}
		return NULL;
	}
	// otherwise check if the character has reached the destination
	int x, y;
	character->getCoordinates(x, y);
	if ((x >= m_left) && (x <= m_right) && (y >= m_top) && (y <= m_bottom))
	{
		character->stopEscorting();
		if (mp_true)
		{
			return mp_true->runNode(character);
		}
		return NULL;
	}

	return this;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEscortNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version = 0;
	file.read(&version, sizeof(int), 1);

	// read test data
	file.read(&m_left, sizeof(int), 1);
	file.read(&m_top, sizeof(int), 1);
	file.read(&m_right, sizeof(int), 1);
	file.read(&m_bottom, sizeof(int), 1);
	if (version > NFV_030320)
	{
		file.read(&m_timeout, sizeof(int), 1);
	}

	return ABooleanNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CEscortNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write test data
	file.write(&m_left, sizeof(int), 1);
	file.write(&m_top, sizeof(int), 1);
	file.write(&m_right, sizeof(int), 1);
	file.write(&m_bottom, sizeof(int), 1);
	file.write(&m_timeout, sizeof(int), 1);

	return ABooleanNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEscortNode::getDestRegion(int& left, int& top, int& right, int& bottom)
{
	left = m_left;
	top = m_top;
	right = m_right;
	bottom = m_bottom;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEscortNode::setDestRegion(int left, int top, int right, int bottom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CEscortNode::getTimeout()
{
	return m_timeout;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CEscortNode::setTimeout(int seconds)
{
	m_timeout = seconds;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChatBoxNode::CChatBoxNode()
{
	{
		for (int i(0); i < MAX_CHAT_OPTIONS; i++)
		{
			mpa_optionnodes[i] = NULL;
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CChatBoxNode::~CChatBoxNode()
{
	{
		for (int i(0); i < MAX_CHAT_OPTIONS; i++)
		{
			delete mpa_optionnodes[i];
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CChatBoxNode::runNode(Player::CServerCharacter* character)
{

	// first we need to find out if this is the current node for the character
	if (!character->isCurrentChatNode(this))
	{
		// this isn't the current node, so we need to send him the chat information
		char* options[6];
		options[0] = ma_optiontexts[0].getString();
		options[1] = ma_optiontexts[1].getString();
		options[2] = ma_optiontexts[2].getString();
		options[3] = ma_optiontexts[3].getString();
		options[4] = ma_optiontexts[4].getString();
		options[5] = ma_optiontexts[5].getString();

		int portrait = 0;
		if (mp_source)
		{
			portrait = mp_source->getPortrait();
		}

		character->sendNPCChat(m_chattext, options, portrait);
		
		// wait for him to make a selection
		return this;
	}
	else
	{
		int selection = character->getLastChatSelection();

		if (selection < 0)
		{
			// the character hasn't made his mind up yet
			return this;
		}

		// check the chosen selection
		if (mpa_optionnodes[selection])
		{
			return mpa_optionnodes[selection]->runNode(character);
		}
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChatBoxNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	// read chat string
	m_chattext = FileSystem::readStringWithLength(file);

	// read options
	for (int i(0); i < MAX_CHAT_OPTIONS; i++)
	{
		// read option name
		ma_optiontexts[i] = FileSystem::readStringWithLength(file);

		int type = MNT_INVALID;
		file.read(&type, sizeof(int), 1);

		// create node
		mpa_optionnodes[i] = createNode(type, mp_source);
		if (mpa_optionnodes[i])
		{
			if (!mpa_optionnodes[i]->read(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CChatBoxNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	// write chat string
	FileSystem::writeStringWithLength(file, m_chattext);

	// write options
	for (int i(0); i < MAX_CHAT_OPTIONS; i++)
	{
		// write option name
		FileSystem::writeStringWithLength(file, ma_optiontexts[i]);

		int type = MNT_INVALID;
		if (mpa_optionnodes[i])
		{
			type = mpa_optionnodes[i]->getNodeType();
		}
		file.write(&type, sizeof(int), 1);

		// write node
		if (mpa_optionnodes[i])
		{
			if (!mpa_optionnodes[i]->write(file))
			{
				return false;
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
Basic::CCharString& CChatBoxNode::getChatText()
{
	return m_chattext;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatBoxNode::setChatText(Basic::CCharString& text)
{
	m_chattext = text;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
Basic::CCharString& CChatBoxNode::getOptionText(int number)
{
	if (number < 0 || number >= MAX_CHAT_OPTIONS)
	{
		return ma_optiontexts[0];
	}

	return ma_optiontexts[number];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatBoxNode::setOptionText(int number, Basic::CCharString& text)
{
	if (number >= 0 || number < MAX_CHAT_OPTIONS)
	{
		ma_optiontexts[number] = text;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CChatBoxNode::getNode(int number)
{
	if (number < 0 || number >= MAX_CHAT_OPTIONS)
	{
		return NULL;
	}

	return mpa_optionnodes[number];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CChatBoxNode::setNode(int number, AChatNode* node, bool shoulddelete)
{
	if (number >= 0 || number < MAX_CHAT_OPTIONS)
	{
		if (shoulddelete)
		{
			delete mpa_optionnodes[number];
		}
		mpa_optionnodes[number] = node;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CChatBoxNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}

	for (int i(0); i < MAX_CHAT_OPTIONS; i++)
	{
		if (mpa_optionnodes[i])
		{
			AChatNode* node = mpa_optionnodes[i]->findNode(idnumber);
			if (node)
			{
				return node;
			}
		}
	}

	return NULL;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTradeNode::CTradeNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTradeNode::~CTradeNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CTradeNode::runNode(Player::CServerCharacter* character)
{
	// start a trade with the character
	if (character)
	{
		character->startNPCTrade();
	}

	// stop the chat line
	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTradeNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version = NFV_CURRENT;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTradeNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CTradeNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}
	return NULL;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCraftNode::CCraftNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CCraftNode::~CCraftNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CCraftNode::runNode(Player::CServerCharacter* character)
{
	// start a trade with the character
	if (character)
	{
		character->startNPCCraft();
	}

	// stop the chat line
	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCraftNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version = NFV_CURRENT;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CCraftNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CCraftNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}
	return NULL;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CStorageNode::CStorageNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CStorageNode::~CStorageNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CStorageNode::runNode(Player::CServerCharacter* character)
{
	// start a trade with the character
	if (character)
	{
		character->startNPCItemStorage();
	}

	// stop the chat line
	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CStorageNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version = NFV_CURRENT;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CStorageNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CStorageNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}
	return NULL;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CJumpNode::CJumpNode()
{
	m_destinationnode = -1;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CJumpNode::~CJumpNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CJumpNode::runNode(Player::CServerCharacter* character)
{
	// this node will just jump to another node by searching for the node's id number...
	// and for security's sake we ignore the id number if it's our own
	if (mp_source && (m_destinationnode != m_idnumber))
	{
		AChatNode* found = mp_source->findNode(m_destinationnode);
		if (found)
		{
			return found->runNode(character);
		}
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CJumpNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version = NFV_CURRENT;
	file.read(&version, sizeof(int), 1);

	if (version > NFV_030215)
	{
		file.read(&m_idnumber, sizeof(int), 1);
	}

	file.read(&m_destinationnode, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CJumpNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	file.write(&m_idnumber, sizeof(int), 1);

	file.write(&m_destinationnode, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
AChatNode* CJumpNode::findNode(int idnumber)
{
	if (idnumber == m_idnumber)
	{
		return this;
	}
	return NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CJumpNode::getDestination()
{
	return m_destinationnode;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CJumpNode::setDestination(int idnumber)
{
	m_destinationnode = idnumber;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGiveExpNode::CGiveExpNode()
:
	AActionNode(),
	m_amount(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGiveExpNode::~CGiveExpNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CGiveExpNode::runNode(Player::CServerCharacter* character)
{
	if (character && (m_amount > 0))
	{
		character->addPersonalExperience(m_amount, character->getStats().level, false);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGiveExpNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&m_amount, sizeof(int), 1);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGiveExpNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(&m_amount, sizeof(int), 1);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGiveExpNode::getAmount()
{
	return m_amount;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGiveExpNode::setAmount(int amount)
{
	m_amount = amount;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGiveCoinsNode::CGiveCoinsNode()
:
	AActionNode(),
	m_amount(0),
	m_type(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGiveCoinsNode::~CGiveCoinsNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CGiveCoinsNode::runNode(Player::CServerCharacter* character)
{
	if (character && (m_amount > 0))
	{
		character->addPersonalCoins(m_amount, m_type, true);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGiveCoinsNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&m_amount, sizeof(int), 1);
	file.read(&m_type, sizeof(int), 1);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGiveCoinsNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(&m_amount, sizeof(int), 1);
	file.write(&m_type, sizeof(int), 1);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGiveCoinsNode::getAmount()
{
	return m_amount;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGiveCoinsNode::setAmount(int amount)
{
	m_amount = amount;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CGiveCoinsNode::getType()
{
	return m_type;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGiveCoinsNode::setType(int type)
{
	m_type = type;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGiveItemNode::CGiveItemNode()
:
	AActionNode()
{
	ma_itemname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CGiveItemNode::~CGiveItemNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CGiveItemNode::runNode(Player::CServerCharacter* character)
{
	if (character && (ma_itemname[0] != 0))
	{
		character->giveNewItem(ma_itemname, true);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGiveItemNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(ma_itemname, sizeof(char), 32);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CGiveItemNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(ma_itemname, sizeof(char), 32);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CGiveItemNode::getItemTypeName()
{
	return ma_itemname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CGiveItemNode::setItemTypeName(char* name)
{
	if (!name)
	{
		return;
	}

	for (int i(0); i < 32; i++)
	{
		ma_itemname[i] = name[i];
		if (!ma_itemname[i])
		{
			break;
		}
	}
	ma_itemname[31] = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTakeCoinsNode::CTakeCoinsNode()
:
	AActionNode(),
	m_amount(0),
	m_type(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTakeCoinsNode::~CTakeCoinsNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CTakeCoinsNode::runNode(Player::CServerCharacter* character)
{
	if (character && (m_amount > 0))
	{
		character->loseCoins(m_amount, m_type, false, true);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTakeCoinsNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&m_amount, sizeof(int), 1);
	file.read(&m_type, sizeof(int), 1);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTakeCoinsNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(&m_amount, sizeof(int), 1);
	file.write(&m_type, sizeof(int), 1);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CTakeCoinsNode::getAmount()
{
	return m_amount;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CTakeCoinsNode::setAmount(int amount)
{
	m_amount = amount;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CTakeCoinsNode::getType()
{
	return m_type;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CTakeCoinsNode::setType(int type)
{
	m_type = type;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTakeItemNode::CTakeItemNode()
:
	AActionNode()
{
	ma_itemname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTakeItemNode::~CTakeItemNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CTakeItemNode::runNode(Player::CServerCharacter* character)
{
	if (character && (ma_itemname[0] != 0))
	{
		character->removeBackpackItem(ma_itemname);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTakeItemNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(ma_itemname, sizeof(char), 32);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTakeItemNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(ma_itemname, sizeof(char), 32);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CTakeItemNode::getItemTypeName()
{
	return ma_itemname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CTakeItemNode::setItemTypeName(char* name)
{
	if (!name)
	{
		return;
	}

	for (int i(0); i < 32; i++)
	{
		ma_itemname[i] = name[i];
		if (!ma_itemname[i])
		{
			break;
		}
	}
	ma_itemname[31] = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSetQuestStateNode::CSetQuestStateNode()
:
	AActionNode(),
	m_state(1)
{
	ma_questname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSetQuestStateNode::~CSetQuestStateNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CSetQuestStateNode::runNode(Player::CServerCharacter* character)
{
	if (character && (ma_questname[0] != 0))
	{
		character->setQuestState(ma_questname, m_state);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSetQuestStateNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(ma_questname, sizeof(char), 32);
	file.read(&m_state, sizeof(int), 1);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSetQuestStateNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(ma_questname, sizeof(char), 32);
	file.write(&m_state, sizeof(int), 1);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSetQuestStateNode::getQuestName()
{
	return ma_questname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSetQuestStateNode::setQuestName(char* name)
{
	if (!name)
	{
		return;
	}

	for (int i(0); i < 32; i++)
	{
		ma_questname[i] = name[i];
		if (!ma_questname[i])
		{
			break;
		}
	}
	ma_questname[31] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CSetQuestStateNode::getQuestState()
{
	return m_state;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSetQuestStateNode::setQuestState(int state)
{
	m_state = state;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTeleportNode::CTeleportNode()
:
	AActionNode(),
	m_x(0),
	m_y(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CTeleportNode::~CTeleportNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CTeleportNode::runNode(Player::CServerCharacter* character)
{
	if (character)
	{
		//character->moveToPosition(m_x, m_y, true, true, false);
		character->teleportToPosition(m_x, m_y);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTeleportNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(&m_x, sizeof(int), 1);
	file.read(&m_y, sizeof(int), 1);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CTeleportNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(&m_x, sizeof(int), 1);
	file.write(&m_y, sizeof(int), 1);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CTeleportNode::getX()
{
	return m_x;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CTeleportNode::getY()
{
	return m_y;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CTeleportNode::setX(int value)
{
	m_x = value;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CTeleportNode::setY(int value)
{
	m_y = value;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSetSpawnNode::CSetSpawnNode()
:
	AActionNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSetSpawnNode::~CSetSpawnNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CSetSpawnNode::runNode(Player::CServerCharacter* character)
{
	if (character)
	{
		character->setRespawnPoint();
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSetSpawnNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSetSpawnNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	return AActionNode::write(file);
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSendMessageNode::CSendMessageNode()
:
	AActionNode()
{
	ma_message[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CSendMessageNode::~CSendMessageNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CSendMessageNode::runNode(Player::CServerCharacter* character)
{
	if (character && (ma_message[0] != 0))
	{
		// Comm::AKARRA_GID == -2
		character->chatSystemMessage(Basic::CCharString((char*)ma_message), -2);
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSendMessageNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(ma_message, sizeof(char), 96);

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CSendMessageNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(ma_message, sizeof(char), 96);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CSendMessageNode::getMessage()
{
	return ma_message;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CSendMessageNode::setMessage(char* message)
{
	if (!message)
	{
		return;
	}

	for (int i(0); i < 96; i++)
	{
		ma_message[i] = message[i];
		if (!ma_message[i])
		{
			break;
		}
	}
	ma_message[95] = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CActivateObjectiveNode::CActivateObjectiveNode()
:
	AActionNode(),
	m_objective(0),
	m_action(0)
{
	ma_questname[0] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CActivateObjectiveNode::~CActivateObjectiveNode()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
#ifdef AKARRASERVER
AChatNode* CActivateObjectiveNode::runNode(Player::CServerCharacter* character)
{
	if (character && (ma_questname[0] != 0))
	{
		if (m_action == 0)
		{
			character->enableQuestObjective(ma_questname, m_objective);
		}
		else
		{
			character->setObjectiveReached(NULL, m_objective, ma_questname);
		}
	}

	if (mp_followup)
	{
		return mp_followup->runNode(character);
	}

	return NULL;
}
#endif

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CActivateObjectiveNode::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read node version
	int version;
	file.read(&version, sizeof(int), 1);

	// read data
	file.read(ma_questname, sizeof(char), 32);
	file.read(&m_objective, sizeof(int), 1);
	if (version > NFV_030320)
	{
		file.read(&m_action, sizeof(int), 1);
	}

	return AActionNode::read(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CActivateObjectiveNode::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write node version
	int version = NFV_CURRENT;
	file.write(&version, sizeof(int), 1);

	// write data
	file.write(ma_questname, sizeof(char), 32);
	file.write(&m_objective, sizeof(int), 1);
	file.write(&m_action, sizeof(int), 1);

	return AActionNode::write(file);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* CActivateObjectiveNode::getQuestName()
{
	return ma_questname;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CActivateObjectiveNode::setQuestName(char* name)
{
	if (!name)
	{
		return;
	}

	for (int i(0); i < 32; i++)
	{
		ma_questname[i] = name[i];
		if (!ma_questname[i])
		{
			break;
		}
	}
	ma_questname[31] = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CActivateObjectiveNode::getObjectiveNumber()
{
	return m_objective;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CActivateObjectiveNode::setObjectiveNumber(int num)
{
	m_objective = num;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CActivateObjectiveNode::getActionState()
{
	return m_action;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CActivateObjectiveNode::setActionState(int state)
{
	m_action = state;
}


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****


}
