
// =============================================================================
// THIS FILE IS
// a cut-down version of the real area. It doesn't include the encryption
// functions to prevent people from hacking the real game's sector files.
// =============================================================================


#include "CArea2.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/filesystem.h"

#include <mem.h>

namespace WorldSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CArea2Static::CArea2Static()
{
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CArea2Static::~CArea2Static()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::clear()
{
	m_tileset = 0;
    ma_spriteset[0] = 0;
    ma_spriteset[1] = 0;
    m_version = 0;

	memset(ma_areacolor, 0, sizeof(unsigned char) * 3);
	memset(ma_tiles, 0xFF, sizeof(unsigned char) * AREA2_SIZE);
	memset(ma_sprites, 0, sizeof(unsigned char) * AREA2_SIZE);
	memset(ma_neighbors, 0, sizeof(char) * NUM_NEIGHBORS);

	u_areatype = AT_NORMAL;
	mp_savedata = NULL;
	m_datasize = 0;

	m_modified = false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::removeAll()
{
	delete [] mp_savedata;
	
	clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CArea2Static::createSaveData(int type)
{
	// remove old data
	delete [] mp_savedata;
	mp_savedata = NULL;

	// if the area has been modified we should increase the version number
	if (m_modified)
	{
		m_version++;
	}
	m_modified = false;

	// create a huge buffer that is big enough to hold all information
	unsigned char* buffer = new unsigned char [AREA2_SIZE * 4];
	int size = sizeof(int); // note: makes place for size

	// start by storing a version number
	int version = AFV_CURRENT;
	memcpy(&buffer[size], &version, sizeof(int));
	size += sizeof(int);

	// store area type
	memcpy(&buffer[size], &type, sizeof(int));
	size += sizeof(int);
	u_areatype = type;

	// now use different methods depending on the area type
	if (type == AT_NORMAL)
	{
		// just copy everything

		memcpy(&buffer[size], &m_version, sizeof(unsigned short));
		size += sizeof(unsigned short);
		memcpy(&buffer[size], &m_tileset, sizeof(unsigned char));
		size += sizeof(unsigned char);
		memcpy(&buffer[size], ma_areacolor, sizeof(unsigned char) * 3);
		size += sizeof(unsigned char) * 3;
		memcpy(&buffer[size], ma_spriteset, sizeof(unsigned char) * AREA2_SPRITESETS);
		size += sizeof(unsigned char) * AREA2_SPRITESETS;
		memcpy(&buffer[size], ma_tiles, sizeof(unsigned char) * AREA2_SIZE);
		size += sizeof(unsigned char) * AREA2_SIZE;
		memcpy(&buffer[size], ma_sprites, sizeof(unsigned char) * AREA2_SIZE);
		size += sizeof(unsigned char) * AREA2_SIZE;
		memcpy(&buffer[size], ma_neighbors, sizeof(char) * NUM_NEIGHBORS);
		size += sizeof(char) * NUM_NEIGHBORS;
	}
	else if (type == AT_WORLD)
	{
		// the world type has the special feature that the maximum tile value is 15 (0xF), so we
		// can stuff two tiles into the same byte, but we need a buffer for this too
		unsigned char* tilebuffer = new unsigned char [AREA2_SIZE / 2];
		for (int i(0); i < (AREA2_SIZE / 2); i++)
		{
			unsigned char value = (ma_tiles[i * 2] & 0xF) << 4;
			value |= (ma_tiles[(i * 2) + 1] & 0xF);
			tilebuffer[i] = value;
		}

		// copy everything
		memcpy(&buffer[size], &m_version, sizeof(unsigned short));
		size += sizeof(unsigned short);
		memcpy(&buffer[size], &m_tileset, sizeof(unsigned char));
		size += sizeof(unsigned char);
		memcpy(&buffer[size], ma_areacolor, sizeof(unsigned char) * 3);
		size += sizeof(unsigned char) * 3;
		memcpy(&buffer[size], ma_spriteset, sizeof(unsigned char) * AREA2_SPRITESETS);
		size += sizeof(unsigned char) * AREA2_SPRITESETS;
		memcpy(&buffer[size], tilebuffer, sizeof(unsigned char) * (AREA2_SIZE / 2));
		size += sizeof(unsigned char) * (AREA2_SIZE / 2);
		memcpy(&buffer[size], ma_sprites, sizeof(unsigned char) * AREA2_SIZE);
		size += sizeof(unsigned char) * AREA2_SIZE;
		memcpy(&buffer[size], ma_neighbors, sizeof(char) * NUM_NEIGHBORS);
		size += sizeof(char) * NUM_NEIGHBORS;

		delete [] tilebuffer;
	}
	//else if (type == AT_NORMALCOMPRESSED)
	//else if (type == AT_WORLDCOMPRESSED)
	else
	{
		// huh?
		delete [] buffer;
		return 0;
	}

	// align our size to 4 bytes and insert it into the buffer
	size = (size + 4) & ~3;
	memcpy(&buffer[0], &size, sizeof(int));

	// create a new buffer where we store the final results
	mp_savedata = new unsigned char [size];
	memcpy(mp_savedata, buffer, size);
	delete [] buffer;

	m_datasize = size;
	return size;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CArea2Static::open(char* filename, bool oldstyle)
{
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "rb");

    if (!file.isOk())
    {
    	return false;
    }

	bool r = read(file, oldstyle);

	FileSystem::directClose(file);

	return r;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CArea2Static::save(char* filename)
{
	if (!mp_savedata)
	{
		return false;
	}

	FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");

    if (!file.isOk())
    {
    	return false;
    }

	bool r = write(file);

	FileSystem::directClose(file);

	return r;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CArea2Static::read(FileSystem::CFileStream& file, bool oldstyle)
{
	if (!file.isOk())
	{
		return false;
	}

	if (oldstyle)
    {
		file.read(&m_version, sizeof(unsigned short), 1);
		file.read(&m_tileset, sizeof(unsigned char), 1);
		file.read(ma_areacolor, sizeof(unsigned char), 3);
		file.read(ma_spriteset, sizeof(unsigned char), AREA2_SPRITESETS);
		file.read(ma_tiles, sizeof(unsigned char), AREA2_SIZE);
		file.read(ma_sprites, sizeof(unsigned char), AREA2_SIZE);
		file.read(ma_neighbors, sizeof(char), NUM_NEIGHBORS);

		if (ma_spriteset[0] == AREA2_INVALIDSPRITESET)
		{
			ma_spriteset[0] = 0;
		}
		if (ma_spriteset[1] == AREA2_INVALIDSPRITESET)
		{
			ma_spriteset[1] = 0;
		}
	}
	else
	{
		// read size of buffer
		int size = 0;
		file.read(&size, sizeof(int), 1);

		if (size < 8)
		{
			return false;
		}

		// create a buffer
		unsigned char* buffer = new unsigned char [size];
		// read the whole buffer, but skip the first integer since we already have read that one
		// (the value 'size' includes itself)
		file.read(buffer, sizeof(unsigned char), size - sizeof(int));
		size = 0;

		// now get some values
		int version;
		memcpy(&version, &buffer[size], sizeof(int));
		size += sizeof(int);
		memcpy(&u_areatype, &buffer[size], sizeof(int));
		size += sizeof(int);

		// now we need to choose different loading styles depending on the area type
		if (u_areatype == AT_NORMAL)
		{
			// just copy everything
			memcpy(&m_version, &buffer[size], sizeof(unsigned short));
			size += sizeof(unsigned short);
			memcpy(&m_tileset, &buffer[size], sizeof(unsigned char));
			size += sizeof(unsigned char);
			memcpy(ma_areacolor, &buffer[size], sizeof(unsigned char) * 3);
			size += sizeof(unsigned char) * 3;
			memcpy(ma_spriteset, &buffer[size], sizeof(unsigned char) * AREA2_SPRITESETS);
			size += sizeof(unsigned char) * AREA2_SPRITESETS;
			memcpy(ma_tiles, &buffer[size], sizeof(unsigned char) * AREA2_SIZE);
			size += sizeof(unsigned char) * AREA2_SIZE;
			memcpy(ma_sprites, &buffer[size], sizeof(unsigned char) * AREA2_SIZE);
			size += sizeof(unsigned char) * AREA2_SIZE;
			memcpy(ma_neighbors, &buffer[size], sizeof(char) * NUM_NEIGHBORS);
			size += sizeof(char) * NUM_NEIGHBORS;
		}
		else if (u_areatype == AT_WORLD)
		{
			unsigned char* tilebuffer = new unsigned char [AREA2_SIZE / 2];
			
			// copy everything
			memcpy(&m_version, &buffer[size], sizeof(unsigned short));
			size += sizeof(unsigned short);
			memcpy(&m_tileset, &buffer[size], sizeof(unsigned char));
			size += sizeof(unsigned char);
			memcpy(ma_areacolor, &buffer[size], sizeof(unsigned char) * 3);
			size += sizeof(unsigned char) * 3;
			memcpy(ma_spriteset, &buffer[size], sizeof(unsigned char) * AREA2_SPRITESETS);
			size += sizeof(unsigned char) * AREA2_SPRITESETS;
			memcpy(tilebuffer, &buffer[size], sizeof(unsigned char) * (AREA2_SIZE / 2));
			size += sizeof(unsigned char) * (AREA2_SIZE / 2);
			memcpy(ma_sprites, &buffer[size], sizeof(unsigned char) * AREA2_SIZE);
			size += sizeof(unsigned char) * AREA2_SIZE;
			memcpy(ma_neighbors, &buffer[size], sizeof(char) * NUM_NEIGHBORS);
			size += sizeof(char) * NUM_NEIGHBORS;

			// unstuff
			for (int i(0); i < (AREA2_SIZE / 2); i++)
			{
				ma_tiles[i * 2] = (tilebuffer[i] & 0xF0) >> 4;
				ma_tiles[(i * 2) + 1] = (tilebuffer[i] & 0xF);
			}
			delete [] tilebuffer;
		}
		//else if (u_areatype == AT_NORMALCOMPRESSED)
		//else if (u_areatype == AT_WORLDCOMPRESSED)
		else
		{
			// wtf...
			delete [] buffer;
			return false;
		}

		// we're done here
		delete [] buffer;
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CArea2Static::write(FileSystem::CFileStream& file)
{
	if (!mp_savedata)
	{
		return false;
	}
	if (!file.isOk())
	{
		return false;
	}

    // write version
    //unsigned char version = AREA_FILEVERSION;
	//file.write(&version, sizeof(unsigned char), 1);

    //unsigned short newver = m_version + 1;
	/*
    file.write(&m_version, sizeof(unsigned short), 1);
	file.write(&m_tileset, sizeof(unsigned char), 1);
	file.write(ma_areacolor, sizeof(unsigned char), 3);
	file.write(ma_spriteset, sizeof(unsigned char), AREA2_SPRITESETS);
	file.write(ma_tiles, sizeof(unsigned char), AREA2_SIZE);
	file.write(ma_sprites, sizeof(unsigned char), AREA2_SIZE);
	file.write(ma_neighbors, sizeof(char), NUM_NEIGHBORS);
	*/

	file.write(mp_savedata, sizeof(char), m_datasize);	

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Function to copy a area to another. 
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CArea2Static::copyArea(CArea2Static* src, int rotation)
{
	if (!src)
	{
		return false;
	}
	removeAll();

	// copy all info
	m_tileset = src->m_tileset;
    ma_spriteset[0] = src->ma_spriteset[0];
    ma_spriteset[1] = src->ma_spriteset[1];
    m_version = src->m_version;

	memcpy(ma_areacolor, src->ma_areacolor, sizeof(unsigned char) * 3);
	memcpy(ma_tiles, src->ma_tiles, sizeof(unsigned char) * AREA2_SIZE);
	memcpy(ma_sprites, src->ma_sprites, sizeof(unsigned char) * AREA2_SIZE);
	memcpy(ma_neighbors, src->ma_neighbors, sizeof(char) * NUM_NEIGHBORS);

	u_areatype = src->u_areatype;

	// check if we want to rotate the areas tiles
	if (rotation != AR_NOTUSED)
	{
		switch (rotation)
		{
			case AR_90:
				flipArea(AR_90);
				break;

			case AR_90M:
				flipArea(AR_90);
				flipArea(AR_VERTICAL);
				break;

			case AR_180:
				flipArea(AR_VERTICAL);
				flipArea(AR_HORISONTAL);
				break;

			case AR_180M:
				flipArea(AR_VERTICAL);
				break;

			case AR_270:
				flipArea(AR_90);
				flipArea(AR_90);
				flipArea(AR_90);
				break;

			case AR_270M:
				flipArea(AR_90);
				flipArea(AR_90);
				flipArea(AR_90);
				flipArea(AR_VERTICAL);
				break;
		};
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------

void CArea2Static::flipArea(int rotation)
{
	// Variables used for area rotations
	int index, index2;
	int x, y;
	int mid;

	// Flip the tileset horisontal
	if (rotation == AR_HORISONTAL)
	{
		mid = AREA2_WIDTH / 2;

		// Loop through the tiles we want to flip
		for (x = 0; x < mid; x++)
		{
			for (y = 0; y < AREA2_HEIGHT; y++)
			{
				// Find the current tile we want to flip in the tile array
				index = (AREA2_WIDTH * y) + x;

				// Find the destination position for the tile we want to flip
				index2 = (AREA2_WIDTH * y) + (AREA2_WIDTH - (x + 1));

				// Swap the tiles position with its destination position tile
				swapTiles(index, index2);
			}
		}
	}
	// Flip the tileset vertical
	else if (rotation == AR_VERTICAL)
	{
		mid = AREA2_HEIGHT / 2;

		// Loop through the tiles we want to flip
		for (y = 0; y < mid; y++)
		{
			for (x = 0; x < AREA2_WIDTH; x++)
			{
				// Find the current tile we want to flip in the tile array
				index = (AREA2_WIDTH * y) + x;

				// Find the destination position for the tile we want to flip
				index2 = (AREA2_WIDTH * (AREA2_WIDTH - (y + 1))) + x;

				// Swap the tiles position with its destination position tile
				swapTiles(index, index2);
			}
		}
	}
	// Flip the tileset 90 degrees
	else if (rotation == AR_90)
	{
		unsigned char tiles[AREA2_SIZE];
		unsigned char sprites[AREA2_SIZE];

		// calculate the 90 degree rotation of the tiles and sprites in temporary arrays
		for (y = 0; y < AREA2_HEIGHT; y++)
		{
			for (x = 0; x < AREA2_WIDTH; x++)
			{
				index = (AREA2_WIDTH * y) + x;
				index2 = (AREA2_WIDTH * x) + ((AREA2_WIDTH - 1) - y);

				tiles[index] = ma_tiles[index2];
				sprites[index] = ma_sprites[index2];
			}
		}

		// copy the result to the actual tile and sprite arrays
		memcpy(ma_tiles, tiles, sizeof(unsigned char) * AREA2_SIZE);
		memcpy(ma_sprites, sprites, sizeof(unsigned char) * AREA2_SIZE);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------

void CArea2Static::swapTiles(int from, int to)
{
	// copy all info
	unsigned char temp;

	// Swap the tiles
	temp = ma_tiles[from];
	ma_tiles[from] = ma_tiles[to];

	// Swap the tiles sprites
	temp = ma_sprites[from];
	ma_sprites[from] = ma_sprites[to];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned short CArea2Static::getVersion()
{
	return m_version;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CArea2Static::hasNeighbor(int neighbor)
{
	if (neighbor < 0 || neighbor >= NUM_NEIGHBORS)
	{
		return false;
	}

	return (ma_neighbors[neighbor] != 0);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::setNeighbor(int neighbor, bool onoff)
{
	if (neighbor < 0 || neighbor >= NUM_NEIGHBORS)
	{
		return;
	}

	ma_neighbors[neighbor] = (onoff) ? 1 : 0;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned char CArea2Static::getTileset()
{
	return m_tileset;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::setTileset(unsigned char set)
{
	m_tileset = set;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned char CArea2Static::getSpriteset(int index)
{
	if (index < 0 || index >= AREA2_SPRITESETS)
	{
		return 0;
	}
	return ma_spriteset[index];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::setSpriteset(int index, unsigned char set)
{
	if (index < 0 || index >= AREA2_SPRITESETS)
	{
		return;
	}
	ma_spriteset[index] = set;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::getAreaColor(unsigned char& r, unsigned char& g, unsigned char& b)
{
	r = ma_areacolor[0];
	g = ma_areacolor[1];
	b = ma_areacolor[2];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::setAreaColor(unsigned char r, unsigned char g, unsigned char b)
{
	ma_areacolor[0] = r;
	ma_areacolor[1] = g;
	ma_areacolor[2] = b;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned char* CArea2Static::getTilesReadOnly()
{
	return ma_tiles;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::setTile(int x, int y, unsigned char value)
{
	if ((x < 0) || (x >= AREA2_WIDTH) || (y < 0) || (y > AREA2_HEIGHT))
	{
		return;
	}
	ma_tiles[x + (y << 5)] = value;
	m_modified = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned char* CArea2Static::getSpritesReadOnly()
{
	return ma_sprites;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CArea2Static::setSprite(int x, int y, unsigned char sprite)
{
	if ((x < 0) || (x >= AREA2_WIDTH) || (y < 0) || (y > AREA2_HEIGHT))
	{
		return;
	}
	ma_sprites[x + (y << 5)] = sprite;
	m_modified = true;
}

// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************

unsigned char CAreaCompress::us_indices[COMPRESS_INDEXSIZE];
// macro to calculate index in pctable from previous 2 characters
#define INDEX(p1,p2) (((unsigned)(unsigned char)p1<<7)^(unsigned char)p2)

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAreaCompress::compressData(unsigned char* dest, unsigned char* src, int size)
{
	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CAreaCompress::deCompressData(unsigned char* dest, unsigned char* src, int size)
{
	return 0;
}


} // end of WorldSystem namespace
