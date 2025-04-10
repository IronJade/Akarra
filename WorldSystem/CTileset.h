/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Handles tilesets. Note that the world tileset is
 *    different from the cave ones.
 **/

#ifndef _CTILESET_H_
#define _CTILESET_H_

//#include "../library/3dgraphics/CTextures.h"
#include "../library/filesystem/CFileStream.h"
#include <vector>

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

	// tilegroups are used for easier editing of maps
	enum ETileGroupBlock
	{
		TGB_MAIN		= 0,

		TGB_LEFT		= 4,
		TGB_TOP			= 8,
		TGB_RIGHT		= 12,
		TGB_BOTTOM		= 16,
		TGB_TOPLEFT		= 20,
		TGB_TOPRIGHT	= 24,
		TGB_BOTTOMLEFT	= 28,
		TGB_BOTTOMRIGHT	= 32,
		TGB_ITOPLEFT	= 36,
		TGB_ITOPRIGHT	= 40,
		TGB_IBOTTOMLEFT	= 44,
		TGB_IBOTTOMRIGHT = 48,

		NUM_GROUPBLOCKS	= 52
	};

	enum EBlockingTypes
	{
		BT_BLOCKNONE = 0,
		BT_BLOCKALL,
		BT_BLOCKNONFLYING,
		BT_BLOCKNONETHEREAL,
		BT_BLOCKNORMAL,
		BT_FORCECLEAR
	};

	struct STileGroup
	{
		unsigned char	tiles[NUM_GROUPBLOCKS];
	};

	// 3 - added password protection
	const unsigned int TILESET_VERSION = 3;

	const int TILESET_PASSWORD_LENGTH	= 16;
	const int TILESET_PASSWORD_KEY		= 0xA8AE3F38;

	/* **********************************************************************************
	* STRUCTURE STileset
	*
	* Purpose:	
	*
	* ***********************************************************************************
	* (011124): First version.
	* **********************************************************************************/
	struct STileset
	{
		char					ma_password[TILESET_PASSWORD_LENGTH];
		unsigned char			pixels[256 * 256 * 3];
		unsigned char			blockinfo[256];
		char					animation[256];
		int						delay;
		std::vector<STileGroup>	tilegroups;

		STileset();
		bool open(FileSystem::CFileStream& file, char* password);
	};

	enum ETileSets
	{
		TILESET_WORLD = 0,
		TILESET_CAVE,
		TILESET_TEMPLE,
		TILESET_INDOORS,

		TILESET_TSET1,
		TILESET_TSET2,
		TILESET_TSET3,
		TILESET_TSET4,
		TILESET_TSET5,
		TILESET_TSET6,
		TILESET_TSET7,
		TILESET_TSET8,
		TILESET_TSET9,
		TILESET_TSET10,
		TILESET_TSET11,
		TILESET_TSET12,
		TILESET_TSET13,
		TILESET_TSET14,
		TILESET_TSET15,
		TILESET_TSET16,
		TILESET_TSET17,
		TILESET_TSET18,
		TILESET_TSET19,
		TILESET_TSET20,

		NUM_TILESETS
	};

	const int TILESET_TEXTURE_ID_BASE	= 0xF000;

	/* **********************************************************************************
	* CLASS CClientTileset
	* Derived:	(base class)
	*
	* Purpose:
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011124): First version.
	* **********************************************************************************/
	// tileset for the client application
	class CClientTileset
	{
		public:
			
			CClientTileset();
			~CClientTileset();

			void removeAll();

			//bool loadTileset(int number, Graphics3D::CTextureSystem* system);
			bool loadTileset(int number, Win32::CD3DScreen* screen);

			void update(int millisecs);

		private:

			void clear();

		public:

			//Graphics3D::CTextureSystem*		up_system;
			Graphics3D::CTextureResource*	up_texture;

			unsigned char	ua_blockinfo[256];
			char			ua_animation[256];
			int				u_delay;

			int				u_step;
			int				u_lasttime;
			
	};

	// this will hold all available tilesets as they are loaded
	class CClientTilesetBundle
	{
		public:

			CClientTilesetBundle(Win32::CD3DScreen* screen);
			~CClientTilesetBundle();

			CClientTileset* getTileset(int set);
			void unloadGraphics();

			void updateAllSets(int systemtime);

		protected:

			CClientTileset*		mpa_sets[NUM_TILESETS];
			Win32::CD3DScreen*	mp_screen;
	};

	/* **********************************************************************************
	* CLASS CServerTileset
	* Derived:	(base class)
	*
	* Purpose:
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011124): First version.
	* **********************************************************************************/
	// tileset for the server application (only needs blocking info)
	class CServerTileset
	{
		public:

			CServerTileset();
			~CServerTileset();

			void removeAll();

			bool loadTileset(int number);

		private:

			void clear();

		public:

			unsigned char	ua_blockinfo[256];
	};

} // end of WorldSystem namespace

#endif
