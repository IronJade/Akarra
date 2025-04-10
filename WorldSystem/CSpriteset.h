/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Handles spritesets (items which are placed on top of
 *    the tilesets).
 **/

#ifndef _CSPRITESET_H_
#define _CSPRITESET_H_

//#include "../library/3dgraphics/CTextures.h"
#include "../library/filesystem/CFileStream.h"

namespace Win32
{
	class CD3DScreen;
}
namespace Graphics3D
{
	class CTextureResource;
}

namespace WorldSystem
{
	const int	MAX_SPRITES		= 64;
	const int	MIN_SPRITES		= 1;	// index 0 is never used, so the real number of sprites is 63*2 

	const unsigned char	SPRITE_ID_BITS	= 0x3F;	// 0011 1111, used to get the sprite number in an area
	const unsigned char	SPRITE_SET_BITS	= 0xC0;	// 1100 0000, used to get the spriteset in an area

	const int	CELL_WIDTH		= 16;
	const int	CELL_HEIGHT		= 16;
	const int	CELL_COLUMNS	= 16;
	const int	CELL_ROWS		= 16;
	const int	NUM_CELLS		= CELL_COLUMNS * CELL_ROWS;

	const int	TEXTURE_WIDTH	= CELL_WIDTH * CELL_COLUMNS;
	const int	TEXTURE_HEIGHT	= CELL_HEIGHT * CELL_ROWS;
	const int	TEXTURE_DEPTH	= 4; // rgba
	const int	TEXTURE_SIZE	= TEXTURE_WIDTH * TEXTURE_HEIGHT * TEXTURE_DEPTH;

	const char	INVALID_SPRITE	= -1;
	const unsigned char NO_SPRITE = 0;

	extern const char *SPRITESET_FILENAMES[];

	/*struct SSpriteInfo
	{
		char	x, y;
		char	width, height;
		char	ox, oy;		// origin offset

		SSpriteInfo();
		void clear();
	};*/

	struct SSpriteInfo2
	{
		char	x, y;
		char	width, height;
		char	ox, oy;		// origin offset
		char	lr, ls;		// light radius and light strength

		SSpriteInfo2();
		void clear();
	};

	enum ETilesetSpritesFileVersion
	{
		TSFV_ORIGINAL = 0,

		TSFV_050713,	// added password protection

		TSFV_CURRENT
	};
	const int SPRITESET_PASSWORD_LENGTH	= 16;
	const int SPRITESET_PASSWORD_KEY		= 0x67AE023B;

	/* **********************************************************************************
	* STRUCTURE STilesetSprites
	*
	* Purpose:	
	*
	* ***********************************************************************************
	* (011124): First version.
	* **********************************************************************************/
	struct STilesetSprites
	{
		char			ma_password[SPRITESET_PASSWORD_LENGTH];
		unsigned char	texture[TEXTURE_SIZE];
		unsigned char	blocking[NUM_CELLS];
		SSpriteInfo2	sprites[MAX_SPRITES];

		STilesetSprites();

		void clear();

		bool save(FileSystem::CFileStream& file);
		bool open(FileSystem::CFileStream& file, char* password);
	};

	enum ESpriteSets
	{
		SPRITESET_FOREST,
		SPRITESET_BUILDINGS,
		SPRITESET_DARKWOODS,
		SPRITESET_INTERIOR,
		SPRITESET_WALLS,
		SPRITESET_CITY,
		SPRITESET_INTERIOR2,
		SPRITESET_WETCAVE,

		SPRITESET_SSET1,
		SPRITESET_SSET2,
		SPRITESET_SSET3,
		SPRITESET_SSET4,
		SPRITESET_SSET5,
		SPRITESET_SSET6,
		SPRITESET_SSET7,
		SPRITESET_SSET8,
		SPRITESET_SSET9,
		SPRITESET_SSET10,
		SPRITESET_SSET11,
		SPRITESET_SSET12,
		SPRITESET_SSET13,
		SPRITESET_SSET14,
		SPRITESET_SSET15,
		SPRITESET_SSET16,
		SPRITESET_SSET17,
		SPRITESET_SSET18,
		SPRITESET_SSET19,
		SPRITESET_SSET20,

		NUM_SPRITESETS
	};
	const int SPRITESET_TEXTURE_ID_BASE = 0xF200;


	/* **********************************************************************************
	* CLASS CClientSpriteset
	* Derived:	(base class)
	*
	* Purpose:
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011125): First version.
	* **********************************************************************************/
	class CClientSpriteset
	{
		public:

			CClientSpriteset();
			~CClientSpriteset();

			void removeAll();

			//bool loadSpriteset(int number, Graphics3D::CTextureSystem* system);
			bool loadSpriteset(int number, Win32::CD3DScreen* screen);

		private:

			void clear();

		public:

			//Graphics3D::CTextureSystem*		up_system;
			Graphics3D::CTextureResource*	up_texture;

			unsigned char	ua_blocking[NUM_CELLS];
			SSpriteInfo2	ua_sprites[MAX_SPRITES];
	};

	// this will hold all available spritesets as they are loaded
	class CClientSpritesetBundle
	{
		public:

			CClientSpritesetBundle(Win32::CD3DScreen* screen);
			~CClientSpritesetBundle();

			CClientSpriteset* getSpriteset(int set);
			void unloadGraphics();

		protected:

			CClientSpriteset*	mpa_sets[NUM_SPRITESETS];
			CClientSpriteset*	mp_basic;	// the basic spriteset
			Win32::CD3DScreen*	mp_screen;
	};

	/* **********************************************************************************
	* CLASS CClientSpriteset
	* Derived:	(base class)
	*
	* Purpose:
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011125): First version.
	* **********************************************************************************/
	class CServerSpriteset
	{
		public:

			CServerSpriteset();
			~CServerSpriteset();

			void removeAll();

			bool loadSpriteset(int number);

		private:

			void clear();

		public:

			unsigned char	ua_blocking[NUM_CELLS];
			SSpriteInfo2	ua_sprites[MAX_SPRITES];
	};

} // end of WorldSystem namespace


#endif
