/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Obsolete area class, some definitions are still used though.
 **/

#include "CArea.h"
#include "../library/FileSystem/CFileStream.h"
#include "../library/FileSystem/filesystem.h"

namespace WorldSystem
{

/*
CAreaStatic::CAreaStatic()
{
	u_tileset = 0;
    //ua_spriteset[0] = AREA_INVALIDSPRITESET;
	ua_spriteset[0] = 0;
    //ua_spriteset[1] = AREA_INVALIDSPRITESET;
	ua_spriteset[1] = 0;
    u_version = 0;

	memset(ua_areacolor, 0, sizeof(unsigned char) * 3);
	memset(ua_tiles, 0xFF, sizeof(unsigned char) * AREA_SIZE);
	memset(ua_sprites, 0, sizeof(unsigned char) * AREA_SIZE);
	memset(ua_neighbors, 0, sizeof(char) * NUM_NEIGHBORS);
}

CAreaStatic::~CAreaStatic()
{
}

void CAreaStatic::clear()
{
	u_tileset = 0;
    //ua_spriteset[0] = AREA_INVALIDSPRITESET;
    //ua_spriteset[1] = AREA_INVALIDSPRITESET;
    ua_spriteset[0] = 0;
    ua_spriteset[1] = 0;
    u_version = 0;

	memset(ua_areacolor, 0, sizeof(unsigned char) * 3);
	memset(ua_tiles, 0xFF, sizeof(unsigned char) * AREA_SIZE);
	memset(ua_sprites, 0, sizeof(unsigned char) * AREA_SIZE);
	memset(ua_neighbors, 0, sizeof(char) * NUM_NEIGHBORS);
}

bool CAreaStatic::open(char* filename)
{
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "rb");

    if (!file.isOk())
    {
    	return false;
    }

	bool r = read(file);

	FileSystem::directClose(file);

	return r;
}

bool CAreaStatic::save(char* filename)
{
	FileSystem::CFileStream file = FileSystem::directOpen(filename, "wb");

    if (!file.isOk())
    {
    	return false;
    }

	bool r = write(file);

	FileSystem::directClose(file);

	return r;
}

bool CAreaStatic::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

    // read file version
    //unsigned char version;
    //file.read(&version, sizeof(unsigned char), 1);

    file.read(&u_version, sizeof(unsigned short), 1);
	file.read(&u_tileset, sizeof(unsigned char), 1);
	file.read(ua_areacolor, sizeof(unsigned char), 3);
	file.read(ua_spriteset, sizeof(char), AREA_SPRITESETS);
	file.read(ua_tiles, sizeof(unsigned char), AREA_SIZE);
	file.read(ua_sprites, sizeof(unsigned char), AREA_SIZE);
	file.read(ua_neighbors, sizeof(char), NUM_NEIGHBORS);

	if (ua_spriteset[0] == AREA_INVALIDSPRITESET)
	{
		ua_spriteset[0] = 0;
	}
	if (ua_spriteset[1] == AREA_INVALIDSPRITESET)
	{
		ua_spriteset[1] = 0;
	}

	return true;
}

bool CAreaStatic::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

    // write version
    //unsigned char version = AREA_FILEVERSION;
	//file.write(&version, sizeof(unsigned char), 1);

    //unsigned short newver = u_version + 1;
    file.write(&u_version, sizeof(unsigned short), 1);
	file.write(&u_tileset, sizeof(unsigned char), 1);
	file.write(ua_areacolor, sizeof(unsigned char), 3);
	file.write(ua_spriteset, sizeof(char), AREA_SPRITESETS);
	file.write(ua_tiles, sizeof(unsigned char), AREA_SIZE);
	file.write(ua_sprites, sizeof(unsigned char), AREA_SIZE);
	file.write(ua_neighbors, sizeof(char), NUM_NEIGHBORS);

	return true;
}
*/
} // end of WorldSystem namespace
