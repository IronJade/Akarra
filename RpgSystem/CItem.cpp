/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Item class, as defined by the item editor.
 **/

#include "CItem.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/FileSystem.h"
#include "../Communication/SGenericPacket.h"

#ifndef AKARRASERVER
#ifndef AKARRACLIENT
#include <mem.h>
#endif
#endif

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemType::CItemType()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CItemType::~CItemType()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemType::clear()
{
	ua_dataname[0] = 0;
	ua_displayname[0] = 0;
	u_maintype = 0;
	u_subtype = 0;

	u_mainsprite = 0;
	u_worldsprite = 0;
	u_eqsprite = 0;
	u_ired = 0;
	u_igreen = 0;
	u_iblue = 0;
	u_ered = 0;
	u_egreen = 0;
	u_eblue = 0;

	u_itemlevel = 0;
	u_value = 0;

	u_param1 = 0;
	u_param2 = 0;
	u_param3 = 0;
	u_flags = 0;

	u_near = 0;
	u_medium = 0;
	u_far = 0;

	u_action = 1000;
	u_recovery = 1000;

	{
		for (int i(0); i < 3; i++)
		{
			ua_req[i].type = -1;
			ua_req[i].value = 0;
		}
	}

	ua_effects[0][0] = 0;
	ua_effects[1][0] = 0;
	u_effectchance = 100;

	u_combinations.clear();
	u_reactions.clear();

	u_designnotes = "";
	u_specialstring = "";
	u_decayminutes = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemType::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// write file version
	int version = CURRENT_ITEMTYPE_VERSION;
	file.write(&version, sizeof(int), 1);

	// name
	file.write(ua_dataname, sizeof(char), ITEM_NAMELENGTH);
	file.write(ua_displayname, sizeof(char), ITEM_NAMELENGTH);

	// type
	file.write(&u_maintype, sizeof(int), 1);
	file.write(&u_subtype, sizeof(int), 1);

	// sprite
	file.write(&u_mainsprite, sizeof(unsigned short), 1);
	file.write(&u_worldsprite, sizeof(unsigned short), 1);
	file.write(&u_eqsprite, sizeof(unsigned short), 1);
	file.write(&u_ired, sizeof(unsigned char), 1);
	file.write(&u_igreen, sizeof(unsigned char), 1);
	file.write(&u_iblue, sizeof(unsigned char), 1);
	file.write(&u_ered, sizeof(unsigned char), 1);
	file.write(&u_egreen, sizeof(unsigned char), 1);
	file.write(&u_eblue, sizeof(unsigned char), 1);

	// shop stats
	file.write(&u_itemlevel, sizeof(unsigned char), 1);
	file.write(&u_value, sizeof(int), 1);

	// parameters
	file.write(&u_param1, sizeof(float), 1);
	file.write(&u_param2, sizeof(float), 1);
	file.write(&u_param3, sizeof(float), 1);
	file.write(&u_flags, sizeof(int), 1);

	// accuracy
	file.write(&u_near, sizeof(unsigned char), 1);
	file.write(&u_medium, sizeof(unsigned char), 1);
	file.write(&u_far, sizeof(unsigned char), 1);

	// delays
	file.write(&u_action, sizeof(unsigned short), 1);
	file.write(&u_recovery, sizeof(unsigned short), 1);

	// item reqs
	{
		for (int i(0); i < 3; i++)
		{
			file.write(&ua_req[i].type, sizeof(int), 1);
			file.write(&ua_req[i].value, sizeof(int), 1);
		}
	}

	// item effects
	{
		for (int i(0); i < 2; i++)
		{
			file.write(ua_effects[i], sizeof(char), 32);
		}
		file.write(&u_effectchance, sizeof(int), 1);
	}

	// item recepies
	{
		int count = (int)u_combinations.size();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			file.write(u_combinations[i].source, sizeof(char), ITEM_NAMELENGTH);
			file.write(u_combinations[i].result, sizeof(char), ITEM_NAMELENGTH);
		}
	}
	{
		int count = (int)u_reactions.size();
		file.write(&count, sizeof(int), 1);

		for (int i(0); i < count; i++)
		{
			file.write(u_reactions[i].source, sizeof(char), ITEM_NAMELENGTH);
			file.write(u_reactions[i].result, sizeof(char), ITEM_NAMELENGTH);
		}
	}

	// write design notes
	FileSystem::writeStringWithLength(file, u_designnotes);

	// write special string
	FileSystem::writeStringWithLength(file, u_specialstring);

	file.write(&u_decayminutes, sizeof(int), 1);

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemType::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	clear();

	// read file version
	int version;
	file.read(&version, sizeof(int), 1);

	// name
	file.read(ua_dataname, sizeof(char), ITEM_NAMELENGTH);
	if (version > ITV_021118)
	{
		file.read(ua_displayname, sizeof(char), ITEM_NAMELENGTH);
	}
	else
	{
		strcpy(ua_displayname, ua_dataname);
	}

	// type
	file.read(&u_maintype, sizeof(int), 1);
	file.read(&u_subtype, sizeof(int), 1);

	// sprite
	file.read(&u_mainsprite, sizeof(unsigned short), 1);
	file.read(&u_worldsprite, sizeof(unsigned short), 1);
	file.read(&u_eqsprite, sizeof(unsigned short), 1);
	file.read(&u_ired, sizeof(unsigned char), 1);
	file.read(&u_igreen, sizeof(unsigned char), 1);
	file.read(&u_iblue, sizeof(unsigned char), 1);

	if (version > ITV_021010)
	{
		file.read(&u_ered, sizeof(unsigned char), 1);
		file.read(&u_egreen, sizeof(unsigned char), 1);
		file.read(&u_eblue, sizeof(unsigned char), 1);
	}
	else
	{
		u_ered = u_ired;
		u_egreen = u_igreen;
		u_eblue = u_iblue;
	}

	// shop stats
	file.read(&u_itemlevel, sizeof(unsigned char), 1);
	if (version > ITV_020925)
	{
		file.read(&u_value, sizeof(int), 1);
	}
	else if (version > ITV_020908)
	{
		unsigned short value;
		file.read(&value, sizeof(unsigned short), 1);
		u_value = value;
	}
	else
	{
		unsigned short values[3];
		file.read(values, sizeof(unsigned short), NUM_MONEYTYPES);

		u_value = values[0] * 100 + values[1] * 10 + values[2];
	}

	// parameters
	file.read(&u_param1, sizeof(float), 1);
	file.read(&u_param2, sizeof(float), 1);
	file.read(&u_param3, sizeof(float), 1);
	file.read(&u_flags, sizeof(int), 1);

	// accuracy
	file.read(&u_near, sizeof(unsigned char), 1);
	file.read(&u_medium, sizeof(unsigned char), 1);
	file.read(&u_far, sizeof(unsigned char), 1);

	// delays
	file.read(&u_action, sizeof(unsigned short), 1);
	file.read(&u_recovery, sizeof(unsigned short), 1);

	// item reqs
	{
		for (int i(0); i < 3; i++)
		{
			file.read(&ua_req[i].type, sizeof(int), 1);
			file.read(&ua_req[i].value, sizeof(int), 1);
		}
	}

	// item effects
	{
		for (int i(0); i < 2; i++)
		{
			file.read(ua_effects[i], sizeof(char), 32);
		}
	}
	if (version > ITV_030407)
	{
		file.read(&u_effectchance, sizeof(int), 1);
	}

	// item recepies
	if (version > ITV_021118)
	{
		{
			int count = 0;
			file.read(&count, sizeof(int), 1);

			for (int i(0); i < count; i++)
			{
				SItemRecepy recepy;
				file.read(recepy.source, sizeof(char), ITEM_NAMELENGTH);
				file.read(recepy.result, sizeof(char), ITEM_NAMELENGTH);

				u_combinations.push_back(recepy);
			}
		}
		{
			int count = 0;
			file.read(&count, sizeof(int), 1);

			for (int i(0); i < count; i++)
			{
				SItemRecepy recepy;
				file.read(recepy.source, sizeof(char), ITEM_NAMELENGTH);
				file.read(recepy.result, sizeof(char), ITEM_NAMELENGTH);

				u_reactions.push_back(recepy);
			}
		}
	}

	// read design notes
	if (version > ITV_021118)
	{
		u_designnotes = FileSystem::readStringWithLength(file);
	}
	// and special string
	if (version > ITV_030214)
	{
		u_specialstring = FileSystem::readStringWithLength(file);
	}

	if (version > ITV_030504b)
	{
		file.read(&u_decayminutes, sizeof(int), 1);
	}

	// i made a major rebalance to weapons... decreasing their damage by 25% to boost the
	// importance of clerics as well as giving me a greater possibility to add skill boosts
	if (version <= ITV_030504)
	{
		if (u_maintype == IMT_WEAPON)
		{
			// min and max damage
			u_param1 *= 0.75f;
			u_param2 *= 0.75f;
		}
	}

	if (version <= ITV_030519)
	{
		if (u_maintype == IMT_WEAPON)
		{
			// min and max damage
			//u_action = (unsigned short)((float)u_action * 0.75f);
			u_recovery = (unsigned short)((float)u_recovery * 0.70f);
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CItemType::copyOther(CItemType* other)
{
	strcpy(ua_dataname, other->ua_dataname);
	strcpy(ua_displayname, other->ua_displayname);
	u_maintype = other->u_maintype;
	u_subtype = other->u_subtype;

	u_mainsprite = other->u_mainsprite;
	u_worldsprite = other->u_worldsprite;
	u_eqsprite = other->u_eqsprite;
	u_ired = other->u_ired;
	u_igreen = other->u_igreen;
	u_iblue = other->u_iblue;
	u_ered = other->u_ered;
	u_egreen = other->u_egreen;
	u_eblue = other->u_eblue;

	u_itemlevel = other->u_itemlevel;
	u_value = other->u_value;

	u_param1 = other->u_param1;
	u_param2 = other->u_param2;
	u_param3 = other->u_param3;
	u_flags = other->u_flags;

	u_near = other->u_near;
	u_medium = other->u_medium;
	u_far = other->u_far;

	u_action = other->u_action;
	u_recovery = other->u_recovery;

	{
		for (int i(0); i < 3; i++)
		{
			ua_req[i].type = other->ua_req[i].type;
			ua_req[i].value = other->ua_req[i].value;
		}
	}

	strcpy(ua_effects[0], other->ua_effects[0]);
	strcpy(ua_effects[1], other->ua_effects[1]);
	u_effectchance = other->u_effectchance;

	u_combinations = other->u_combinations;
	u_reactions = other->u_reactions;

	u_designnotes = other->u_designnotes;
	u_specialstring = other->u_specialstring;

	u_decayminutes = other->u_decayminutes;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemType::createData(unsigned char** dest, int& size, Basic::CCharString& failreason)
{
	*dest = NULL;
	size = 0;

	// create a temporary buffer that is insanely big
	unsigned char* data = new unsigned char[1024 * 16];

	if (!data)
	{
		failreason = "Unable to create data buffer";
		return false;
	}

	// skip the first 8 bytes to allow the caller to insert a packet header since this
	// buffer will be used as a network message
	// 030106: Also make room for another integer that identifies this as a spawnpoint,
	// since new items and monsters will use the same packet type
	memset(data, 0, sizeof(Comm::SGenericPacket) + sizeof(int));
	size += sizeof(Comm::SGenericPacket) + sizeof(int);

	// insert version info so the client can send spawns to the server even if the server
	// has been updated
	int version = CURRENT_ITEMTYPE_VERSION;
	memcpy(&data[size], &version, sizeof(int));
	size += sizeof(int);

	// name
	memcpy(&data[size], ua_dataname, sizeof(char) * ITEM_NAMELENGTH);
	size += sizeof(char) * ITEM_NAMELENGTH;
	memcpy(&data[size], ua_displayname, sizeof(char) * ITEM_NAMELENGTH);
	size += sizeof(char) * ITEM_NAMELENGTH;

	// type
	memcpy(&data[size], &u_maintype, sizeof(int) * 1);
	size += sizeof(int) * 1;
	memcpy(&data[size], &u_subtype, sizeof(int) * 1);
	size += sizeof(int) * 1;

	// sprite
	memcpy(&data[size], &u_mainsprite, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;
	memcpy(&data[size], &u_worldsprite, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;
	memcpy(&data[size], &u_eqsprite, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;
	memcpy(&data[size], &u_ired, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_igreen, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_iblue, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_ered, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_egreen, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_eblue, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;

	// shop stats
	memcpy(&data[size], &u_itemlevel, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_value, sizeof(int) * 1);
	size += sizeof(int) * 1;

	// parameters
	memcpy(&data[size], &u_param1, sizeof(float) * 1);
	size += sizeof(float) * 1;
	memcpy(&data[size], &u_param2, sizeof(float) * 1);
	size += sizeof(float) * 1;
	memcpy(&data[size], &u_param3, sizeof(float) * 1);
	size += sizeof(float) * 1;
	memcpy(&data[size], &u_flags, sizeof(int) * 1);
	size += sizeof(int) * 1;

	// accuracy
	memcpy(&data[size], &u_near, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_medium, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;
	memcpy(&data[size], &u_far, sizeof(unsigned char) * 1);
	size += sizeof(unsigned char) * 1;

	// delays
	memcpy(&data[size], &u_action, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;
	memcpy(&data[size], &u_recovery, sizeof(unsigned short) * 1);
	size += sizeof(unsigned short) * 1;

	// item reqs
	{
		for (int i(0); i < 3; i++)
		{
			memcpy(&data[size], &ua_req[i].type, sizeof(int) * 1);
			size += sizeof(int) * 1;
			memcpy(&data[size], &ua_req[i].value, sizeof(int) * 1);
			size += sizeof(int) * 1;
		}
	}

	// item effects
	{
		for (int i(0); i < 2; i++)
		{
			memcpy(&data[size], ua_effects[i], sizeof(char) * 32);
			size += sizeof(char) * 32;
		}
	}
	memcpy(&data[size], &u_effectchance, sizeof(int) * 1);
	size += sizeof(int) * 1;

	// item recepies
	{
		int count = (int)u_combinations.size();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;

		for (int i(0); i < count; i++)
		{
			memcpy(&data[size], u_combinations[i].source, sizeof(char) * ITEM_NAMELENGTH);
			size += sizeof(char) * ITEM_NAMELENGTH;
			memcpy(&data[size], u_combinations[i].result, sizeof(char) * ITEM_NAMELENGTH);
			size += sizeof(char) * ITEM_NAMELENGTH;
		}
	}
	{
		int count = (int)u_reactions.size();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;

		for (int i(0); i < count; i++)
		{
			memcpy(&data[size], u_reactions[i].source, sizeof(char) * ITEM_NAMELENGTH);
			size += sizeof(char) * ITEM_NAMELENGTH;
			memcpy(&data[size], u_reactions[i].result, sizeof(char) * ITEM_NAMELENGTH);
			size += sizeof(char) * ITEM_NAMELENGTH;
		}
	}

	// write design notes
	{
		int count = (int)u_designnotes.getStringLength();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;
		memcpy(&data[size], u_designnotes.getString(), sizeof(char) * count);
		size += sizeof(char) * count;
	}

	// write special
	{
		int count = (int)u_specialstring.getStringLength();
		memcpy(&data[size], &count, sizeof(int) * 1);
		size += sizeof(int) * 1;
		memcpy(&data[size], u_specialstring.getString(), sizeof(char) * count);
		size += sizeof(char) * count;
	}

	memcpy(&data[size], &u_decayminutes, sizeof(int) * 1);
	size += sizeof(int) * 1;

	// place a checkpoint (to make sure that the packet is received correctly)
	int checkpoint = ITEMPACKET_CHECK;
	memcpy(&data[size], &checkpoint, sizeof(int));
	size += sizeof(int);

	// copy buffer
	*dest = new unsigned char[size];
	memcpy(*dest, data, size);
	delete [] data;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CItemType::loadData(unsigned char* source, int size, Basic::CCharString& failreason)
{
	if (!source || (size < 12))
	{
		failreason = "Incorrect parameters in packet";
		return false;
	}

	// reset this monster
	clear();

	int pos = sizeof(Comm::SGenericPacket) + sizeof(int);	// skip the header

	// get monster information version
	int itemversion;
	memcpy(&itemversion, &source[pos], sizeof(int));
	pos += sizeof(int);

	// name
	memcpy(ua_dataname, &source[pos], sizeof(char) * ITEM_NAMELENGTH);
	pos += sizeof(char) * ITEM_NAMELENGTH;
	memcpy(ua_displayname, &source[pos], sizeof(char) * ITEM_NAMELENGTH);
	pos += sizeof(char) * ITEM_NAMELENGTH;

	// type
	memcpy(&u_maintype, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;
	memcpy(&u_subtype, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	// sprite
	memcpy(&u_mainsprite, &source[pos], sizeof(unsigned short) * 1);
	pos += sizeof(unsigned short) * 1;
	memcpy(&u_worldsprite, &source[pos], sizeof(unsigned short) * 1);
	pos += sizeof(unsigned short) * 1;
	memcpy(&u_eqsprite, &source[pos], sizeof(unsigned short) * 1);
	pos += sizeof(unsigned short) * 1;
	memcpy(&u_ired, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_igreen, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_iblue, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_ered, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_egreen, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_eblue, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;

	// shop stats
	memcpy(&u_itemlevel, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_value, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	// parameters
	memcpy(&u_param1, &source[pos], sizeof(float) * 1);
	pos += sizeof(float) * 1;
	memcpy(&u_param2, &source[pos], sizeof(float) * 1);
	pos += sizeof(float) * 1;
	memcpy(&u_param3, &source[pos], sizeof(float) * 1);
	pos += sizeof(float) * 1;
	memcpy(&u_flags, &source[pos], sizeof(int) * 1);
	pos += sizeof(int) * 1;

	// accuracy
	memcpy(&u_near, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_medium, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;
	memcpy(&u_far, &source[pos], sizeof(unsigned char) * 1);
	pos += sizeof(unsigned char) * 1;

	// delays
	memcpy(&u_action, &source[pos], sizeof(unsigned short) * 1);
	pos += sizeof(unsigned short) * 1;
	memcpy(&u_recovery, &source[pos], sizeof(unsigned short) * 1);
	pos += sizeof(unsigned short) * 1;

	// item reqs
	{
		for (int i(0); i < 3; i++)
		{
			memcpy(&ua_req[i].type, &source[pos], sizeof(int) * 1);
			pos += sizeof(int) * 1;
			memcpy(&ua_req[i].value, &source[pos], sizeof(int) * 1);
			pos += sizeof(int) * 1;
		}
	}

	// item effects
	{
		for (int i(0); i < 2; i++)
		{
			memcpy(ua_effects[i], &source[pos], sizeof(char) * 32);
			pos += sizeof(char) * 32;
		}
	}
	if (itemversion > ITV_030407)
	{
		memcpy(&u_effectchance, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
	}

	// item recepies
	{
		int count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;

		for (int i(0); i < count; i++)
		{
			SItemRecepy newrecepy;
			memcpy(newrecepy.source, &source[pos], sizeof(char) * ITEM_NAMELENGTH);
			pos += sizeof(char) * ITEM_NAMELENGTH;
			memcpy(newrecepy.result, &source[pos], sizeof(char) * ITEM_NAMELENGTH);
			pos += sizeof(char) * ITEM_NAMELENGTH;

			u_combinations.push_back(newrecepy);
		}
	}
	{
		int count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;

		for (int i(0); i < count; i++)
		{
			SItemRecepy newrecepy;
			memcpy(newrecepy.source, &source[pos], sizeof(char) * ITEM_NAMELENGTH);
			pos += sizeof(char) * ITEM_NAMELENGTH;
			memcpy(newrecepy.result, &source[pos], sizeof(char) * ITEM_NAMELENGTH);
			pos += sizeof(char) * ITEM_NAMELENGTH;

			u_reactions.push_back(newrecepy);
		}
	}

	// write design notes
	{
		int count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		{
			char* string = new char[count + 1];
			memcpy(string, &source[pos], sizeof(char) * count);
			pos += sizeof(char) * count;
			string[count] = 0;

			u_designnotes = string;

			delete [] string;
		}
	}

	// special string
	if (itemversion > ITV_030214)
	{
		int count = 0;
		memcpy(&count, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
		{
			char* string = new char[count + 1];
			memcpy(string, &source[pos], sizeof(char) * count);
			pos += sizeof(char) * count;
			string[count] = 0;

			u_specialstring = string;

			delete [] string;
		}
	}

	if (itemversion > ITV_030504b)
	{
		memcpy(&u_decayminutes, &source[pos], sizeof(int) * 1);
		pos += sizeof(int) * 1;
	}

	// check the checkpoint
	int checkpoint = 0;
	memcpy(&checkpoint, &source[pos], sizeof(int));
	pos += sizeof(int);

	if (checkpoint != ITEMPACKET_CHECK)
	{
		failreason = "CRC check failed";
		return false;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* getItemTypeName(int type)
{
	const char* const ITEMTYPE_NAMES[NUM_ITEMTYPES] =
	{
		"General",
		"Weapon",
		"Shield",
		"Armor",
		"Helm",
		"Gloves",
		"Boots",
		"Ring",
		"Necklace",
		"Edible",
		"Scroll",
		"Mount",
		"Ammo",
		"Light"
	};

	return (char*)ITEMTYPE_NAMES[type];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
char* getItemSubName(int type, int sub)
{
	const char* const ITEMTYPE_WEAPONS[NUM_WEAPON_ITEMTYPES] =
	{
		"Dagger",
		"Small Crushing",
		"Large Crushing",
		"Spear",
		"Small Sword",
		"Staff",
		"Large Sword",
		"Small Axe",
		"Large Axe",
		"Short Bow",
		"Long Bow",
		"Crossbow",
		"Sling",
		"Throwing",
		"Non-Violent"
	};

	const char* const ITEMTYPE_GENERAL[NUM_GENERAL_ITEMTYPES] =
	{
		"Valuables",
		"Junk",
		"Material",
		"Household Item",
		"Plantable"
	};

	const char* const ITEMTYPE_ARMOR[NUM_ARMOR_ITEMTYPES] =
	{
		"Clothing",
		"Light",
		"Medium",
		"Heavy",
		"Robe"
	};

	const char* const ITEMTYPE_SHIELD[NUM_SHIELD_ITEMTYPES] =
	{
		"Light",
		"Heavy"
	};

	const char* const ITEMTYPE_HELM[NUM_HELM_ITEMTYPES] =
	{
		"Helm",
		"Crown",
		"Diadem"
	};

	const char* const ITEMTYPE_SCROLL[NUM_SCROLL_ITEMTYPES] =
	{
		"Skill Scroll",
		"Spell Scroll"
	};

	const char* const ITEMTYPE_FOOD[NUM_FOOD_ITEMTYPES] =
	{
		"Food",
		"Potion",
		"Toxic"
	};

	const char* const ITEMTYPE_AMMO[NUM_AMMO_ITEMTYPES] =
	{
		"Arrows",
		"Bolts",
		"Bullets"
	};

	const char* const ITEMTYPE_LIGHT[NUM_LIGHT_ITEMTYPES] =
	{
		"Torch",
		"Latern",
		"Candle"
	};

	switch (type)
	{
		default:
			return "Item";

		case IMT_GENERAL:
			return (char*)ITEMTYPE_GENERAL[sub];

		case IMT_WEAPON:
			return (char*)ITEMTYPE_WEAPONS[sub];

		case IMT_ARMOR:
			return (char*)ITEMTYPE_ARMOR[sub];
			
		case IMT_SHIELD:
			return (char*)ITEMTYPE_SHIELD[sub];
			
		case IMT_HELM:
			return (char*)ITEMTYPE_HELM[sub];

		case IMT_SCROLL:
			return (char*)ITEMTYPE_SCROLL[sub];

		case IMT_FOOD:
			return (char*)ITEMTYPE_FOOD[sub];

		case IMT_AMMO:
			return (char*)ITEMTYPE_AMMO[sub];

		case IMT_LIGHT:
			return (char*)ITEMTYPE_LIGHT[sub];

		case IMT_GLOVES:
			return "Gloves";
		case IMT_BOOTS:
			return "Boots";
		case IMT_RING:
			return "Ring";
		case IMT_NECK:
			return "Neckitem";
		case IMT_MOUNT:
			return "Mount";
	}

	return "Item";
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function returns the maximum stack count value for stackable items. This value may
// never exceed 255, because that will make the client-side information incorrect.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int getItemMaxStackCount(int type, int sub)
{
	if (type == IMT_AMMO)
	{
		return 100;
	}
	return 15;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int getWeaponAmmoType(int weaponsubtype)
{
	switch (weaponsubtype)
	{
		case ITS_W_SHORTBOW:
		case ITS_W_LONGBOW:
			return ITS_M_ARROWS;
		case ITS_W_CROSSBOW:
			return ITS_M_BOLTS;
		case ITS_W_SLING:
			return ITS_M_BULLETS;
	
		default:
			break;
	}

	return -1;
}

}
