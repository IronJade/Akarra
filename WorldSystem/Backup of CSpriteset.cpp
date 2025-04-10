
#include "CSpriteset.h"
#include "../library/filesystem/filesystem.h"
#include "../library/odebug/debug.h"

namespace WorldSystem
{

const char* SPRITESET_FILENAMES[NUM_SPRITESETS] =
{
	"gfx/woodland.ss2",
	"gfx/buildings.ss2",
	"gfx/darkwood.ss2",
	"gfx/interior.ss2",
	"gfx/walls.ss2",
	"gfx/city.ss2",
	"gfx/interior2.ss2",
	"gfx/wetcave.ss2",

	"gfx/sset1.ss2",
	"gfx/sset2.ss2",
	"gfx/sset3.ss2",
	"gfx/sset4.ss2",
	"gfx/sset5.ss2",
	"gfx/sset6.ss2",
	"gfx/sset7.ss2",
	"gfx/sset8.ss2",
	"gfx/sset9.ss2",
	"gfx/sset10.ss2",
	"gfx/sset11.ss2",
	"gfx/sset12.ss2",
	"gfx/sset13.ss2",
	"gfx/sset14.ss2",
	"gfx/sset15.ss2",
	"gfx/sset16.ss2",
	"gfx/sset17.ss2",
	"gfx/sset18.ss2",
	"gfx/sset19.ss2",
	"gfx/sset20.ss2"
};
const char* BASIC_SPRITESET_FILENAME = "gfx/basic.ss2";

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

/*
SSpriteInfo::SSpriteInfo()
{
    clear();
}

void SSpriteInfo::clear()
{
	x = INVALID_SPRITE;
	y = 0;
	width = 1;
	height = 1;
	ox = 0;
	oy = 0;
}
*/

SSpriteInfo2::SSpriteInfo2()
{
    clear();
}

void SSpriteInfo2::clear()
{
	x = INVALID_SPRITE;
	y = 0;
	width = 1;
	height = 1;
	ox = 0;
	oy = 0;
	ls = 0;
	lr = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

STilesetSprites::STilesetSprites()
{
    clear();
}

void STilesetSprites::clear()
{
    memset(texture, 0, sizeof(unsigned char) * TEXTURE_SIZE);
	memset(blocking, 0, sizeof(unsigned char) * NUM_CELLS);

	for (int i = 0; i < MAX_SPRITES; i++)
	{
        sprites[i].clear();
	}
}

bool STilesetSprites::save(FileSystem::CFileStream& file)
{
	// write version first
	int version = TSFV_CURRENT;
	file.write(&version, sizeof(int), 1);

    file.write(texture, sizeof(unsigned char), TEXTURE_SIZE);
	file.write(blocking, sizeof(unsigned char), NUM_CELLS);
	file.write(sprites, sizeof(SSpriteInfo2), MAX_SPRITES);

	return true;
}

bool STilesetSprites::open(FileSystem::CFileStream& file)
{
	int version = 0;
	file.read(&version, sizeof(int), 1);

    file.read(texture, sizeof(unsigned char), TEXTURE_SIZE);
	file.read(blocking, sizeof(unsigned char), NUM_CELLS);
	file.read(sprites, sizeof(SSpriteInfo2), MAX_SPRITES);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef AKARRASERVER

CClientSpriteset::CClientSpriteset()
{
	clear();
}

CClientSpriteset::~CClientSpriteset()
{
	removeAll();
}

void CClientSpriteset::removeAll()
{
	//if (up_system && up_texture)
	//{
	//	up_system->removeTexture(up_texture->u_id);
	//}
	delete up_texture;

	clear();
}

//bool CClientSpriteset::loadSpriteset(int number, Graphics3D::CTextureSystem* system)
bool CClientSpriteset::loadSpriteset(int number, Win32::CD3DScreen* screen)
{
	//up_system = system;

	//if (up_system)
	{
		// open the file
		FileSystem::CFileStream file;
		if (number == -1)
		{
			// load basic spriteset
			file = FileSystem::directOpen(BASIC_SPRITESET_FILENAME, "rb");
		}
		else
		{
			file = FileSystem::directOpen(SPRITESET_FILENAMES[number], "rb");
		}

		if (!file.isOk())
		{
			return false;
		}

		// create a tileset info structure
		STilesetSprites sprites;
		// read information
		sprites.open(file);

		// close file
		FileSystem::directClose(file);

		// copy information to the class
		memcpy(ua_blocking, sprites.blocking, 256);
		memcpy(ua_sprites, sprites.sprites, MAX_SPRITES * sizeof(SSpriteInfo2));

		// create a texture for the tileset
		up_texture = new Graphics3D::CTextureResource;
		up_texture->create16bitTexture(screen->getDevice(), sprites.texture, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH);
		//up_texture = up_system->createTexture(SPRITESET_TEXTURE_ID_BASE + number, sprites.texture, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH);

		if (up_texture)
		{
			return true;
		}
	}

	return false;
}

void CClientSpriteset::clear()
{
	//up_system = NULL;
	up_texture = NULL;

	memset(ua_blocking, 0, NUM_CELLS);
	for (int i = 0; i < MAX_SPRITES; i++)
	{
		ua_sprites[i].clear();
	}
}

#endif


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef AKARRASERVER

CClientSpritesetBundle::CClientSpritesetBundle(Win32::CD3DScreen* screen)
{
	mp_screen = screen;

	for (int i(0); i < NUM_SPRITESETS; i++)
	{
		mpa_sets[i] = NULL;
	}
	mp_basic = NULL;
}

CClientSpritesetBundle::~CClientSpritesetBundle()
{
	for (int i(0); i < NUM_SPRITESETS; i++)
	{
		delete mpa_sets[i];
	}
	delete mp_basic;
}

CClientSpriteset* CClientSpritesetBundle::getSpriteset(int set)
{
	if (set == -1)
	{
		if (mp_basic != NULL)
		{
			return mp_basic;
		}

		mp_basic = new CClientSpriteset;
		if (!mp_basic->loadSpriteset(-1, mp_screen))
		{
			delete mp_basic;
			mp_basic = NULL;

			DebugOut("Unable to load spriteset: %d\n", set);
		}

		return mp_basic;
	}

	if (mpa_sets[set] != NULL)
	{
		return mpa_sets[set];
	}

	mpa_sets[set] = new CClientSpriteset;
	if (!mpa_sets[set]->loadSpriteset(set, mp_screen))
	{
		delete mpa_sets[set];
		mpa_sets[set] = NULL;

		DebugOut("Unable to load spriteset: %d\n", set);
	}

	return mpa_sets[set];
}

void CClientSpritesetBundle::unloadGraphics()
{
	for (int i(0); i < NUM_SPRITESETS; i++)
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

#endif

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

CServerSpriteset::CServerSpriteset()
{
	clear();
}

CServerSpriteset::~CServerSpriteset()
{
	removeAll();
}

void CServerSpriteset::removeAll()
{
	clear();
}

bool CServerSpriteset::loadSpriteset(int number)
{
	// open the file
	FileSystem::CFileStream file;
	if (number == -1)
	{
		// load basic spriteset
		file = FileSystem::directOpen(BASIC_SPRITESET_FILENAME, "rb");
	}
	else
	{
		file = FileSystem::directOpen(SPRITESET_FILENAMES[number], "rb");
	}

	if (!file.isOk())
	{
		return false;
	}

	// create a tileset info structure
	STilesetSprites sprites;
	// read information
	sprites.open(file);

	// close file
	FileSystem::directClose(file);

	// copy information to the class
	memcpy(ua_blocking, sprites.blocking, 256);
	memcpy(ua_sprites, sprites.sprites, MAX_SPRITES * sizeof(SSpriteInfo2));

	return true;
}

void CServerSpriteset::clear()
{
	memset(ua_blocking, 0, NUM_CELLS);
	for (int i = 0; i < MAX_SPRITES; i++)
	{
		ua_sprites[i].clear();
	}
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

} // end of WorldSystem namespace
