/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Handles tilesets. Note that the world tileset is
 *    different from the cave ones.
 **/

#include "CTileset.h"
#include "../library/filesystem/FileSystem.h"
//#include "../library/odebug/debug.h"
#include "../Common/AkarraUtil.h"

namespace WorldSystem
{

const char* TILESET_FILENAMES[NUM_TILESETS] =
{
	"gfx/world.ats",
	"gfx/cave.ats",
	"gfx/temple.ats",
	"gfx/indoors.ats",
	"gfx/tset1.ats",
	"gfx/tset2.ats",
	"gfx/tset3.ats",
	"gfx/tset4.ats",
	"gfx/tset5.ats",
	"gfx/tset6.ats",
	"gfx/tset7.ats",
	"gfx/tset8.ats",
	"gfx/tset9.ats",
	"gfx/tset10.ats",
	"gfx/tset11.ats",
	"gfx/tset12.ats",
	"gfx/tset13.ats",
	"gfx/tset14.ats",
	"gfx/tset15.ats",
	"gfx/tset16.ats",
	"gfx/tset17.ats",
	"gfx/tset18.ats",
	"gfx/tset19.ats",
	"gfx/tset20.ats"
};

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

STileset::STileset()
{
	memset(pixels, 0, 256 * 256 * 0);
	memset(blockinfo, 0, 256);
	memset(animation, 0, 256);
	delay = 250;
	ma_password[0] = 0;
}

bool STileset::open(FileSystem::CFileStream& file, char* password)
{
	unsigned int version;
	file.read(&version, sizeof(unsigned int), 1);

	if (version >= 3)
	{
		file.read(ma_password, sizeof(char), TILESET_PASSWORD_LENGTH);
		Common::xorData((unsigned int*)ma_password, TILESET_PASSWORD_LENGTH / sizeof(int), TILESET_PASSWORD_KEY);
		if (password)
		{
			if (strcmp(ma_password, password) != 0)
			{
				return false;
			}
		}
	}

	// read image information
	file.read(pixels, sizeof(unsigned char), 256 * 256 * 3);
	// read block information
	file.read(blockinfo, sizeof(unsigned char), 256);

	if (version >= 2)
	{
		// read animation data
		file.read(animation, sizeof(char), 256);
		file.read(&delay, sizeof(int), 1);
	}

	// store number of tileset groups
	int groups;
	file.read(&groups, sizeof(int), 1);

	tilegroups.clear();

	// store each group
	while (groups > 0)
	{
		groups--;

		STileGroup group;
		file.read(&group, sizeof(STileGroup), 1);
		tilegroups.push_back(group);
	}

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef AKARRASERVER

CClientTileset::CClientTileset()
{
	clear();
}

CClientTileset::~CClientTileset()
{
	removeAll();
}

void CClientTileset::removeAll()
{
	//if (up_system && up_texture)
	//{
	//	up_system->removeTexture(up_texture->u_id);
	//}
	delete up_texture;

	clear();
}

//bool CClientTileset::loadTileset(int number, Graphics3D::CTextureSystem* system)
bool CClientTileset::loadTileset(int number, Win32::CD3DScreen* screen)
{
	//up_system = system;

	//if (up_system)
	{
		// open the file
		FileSystem::CFileStream file;
		file = FileSystem::directOpen(TILESET_FILENAMES[number], "rb");

		if (!file.isOk())
		{
			return false;
		}

		// create a tileset info structure
		STileset tiles;
		// read information
		tiles.open(file, NULL);

		// close file
		FileSystem::directClose(file);

		// copy information to the class
		memcpy(ua_blockinfo, tiles.blockinfo, 256);
		memcpy(ua_animation, tiles.animation, 256);
		u_delay = tiles.delay;

		// create a texture for the tileset
		//up_texture = up_system->createTexture(TILESET_TEXTURE_ID_BASE + number, tiles.pixels, 256, 256, 3);
		up_texture = new Graphics3D::CTextureResource;
		up_texture->create16bitTexture(screen->getDevice(), tiles.pixels, 256, 256, 3);

		if (up_texture)
		{
			return true;
		}
	}

	return false;
}

void CClientTileset::update(int millisecs)
{
	if (u_lasttime == 0)
	{
		u_lasttime = millisecs;
	}
	else if ((millisecs - u_lasttime) >= u_delay)
	{
		u_lasttime = millisecs;

		u_step++;
		if (u_step >= 4)
		{
			u_step = 0;
		}
	}
}

void CClientTileset::clear()
{
	//up_system = NULL;
	up_texture = NULL;

	memset(ua_blockinfo, 0, 256);
	memset(ua_animation, 0, 256);
	u_delay = 250;

	u_step = 0;
	u_lasttime = 0;
}

#endif


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef AKARRASERVER

CClientTilesetBundle::CClientTilesetBundle(Win32::CD3DScreen* screen)
{
	mp_screen = screen;

	for (int i(0); i < NUM_TILESETS; i++)
	{
		mpa_sets[i] = NULL;
	}
}

CClientTilesetBundle::~CClientTilesetBundle()
{
	for (int i(0); i < NUM_TILESETS; i++)
	{
		delete mpa_sets[i];
	}
}

CClientTileset* CClientTilesetBundle::getTileset(int set)
{
	if (mpa_sets[set] != NULL)
	{
		return mpa_sets[set];
	}

	mpa_sets[set] = new CClientTileset;
	if (!mpa_sets[set]->loadTileset(set, mp_screen))
	{
		delete mpa_sets[set];
		mpa_sets[set] = NULL;

		DebugOut("Unable to load tileset: %d\n", set);
	}

	return mpa_sets[set];
}

void CClientTilesetBundle::unloadGraphics()
{
	for (int i(0); i < NUM_TILESETS; i++)
	{
		if (mpa_sets[i] != NULL)
		{
			if (mpa_sets[i]->up_texture != NULL)
			{
				mpa_sets[i]->up_texture->invalidateTexture();
			}
		}
	}
}

void CClientTilesetBundle::updateAllSets(int systemtime)
{
	for (int i(0); i < NUM_TILESETS; i++)
	{
		if (mpa_sets[i] != NULL)
		{
			mpa_sets[i]->update(systemtime);
		}
	}
}

#endif

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****


CServerTileset::CServerTileset()
{
	clear();
}

CServerTileset::~CServerTileset()
{
	removeAll();
}

void CServerTileset::removeAll()
{
	clear();
}

bool CServerTileset::loadTileset(int number)
{
	// open the file
	FileSystem::CFileStream file;
	file = FileSystem::directOpen(TILESET_FILENAMES[number], "rb");

	if (!file.isOk())
	{
		return false;
	}

	// create a tileset info structure
	STileset tiles;
	// read information
	tiles.open(file, NULL);

	// close file
	FileSystem::directClose(file);

	// copy information to the class
	memcpy(ua_blockinfo, tiles.blockinfo, 256);

	return true;
}

void CServerTileset::clear()
{
	memset(ua_blockinfo, 0, 256);
}



} // end of WorldSystem namespace
