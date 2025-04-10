/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Obsolete area class, some definitions are still used though.
 **/

#ifndef _CAREA_H_
#define _CAREA_H_

#include "../library/FileSystem/CFileStream.h"

namespace WorldSystem
{

	//const int	AREA_FILEVERSION	= 1;

	//const int	AREA_WIDTH			= 64;
	//const int	AREA_HEIGHT			= 64;
	//const int	AREA_SIZE			= AREA_WIDTH * AREA_HEIGHT; // 4096
	//const int	AREA_BITBYTEWIDTH	= AREA_WIDTH / 8;	// size if each tile is a single bit
	//const int	AREA_BITBYTEHEIGHT	= AREA_HEIGHT;		// still need 64 rows
	//const int	AREA_BITBYTESIZE	= AREA_BITBYTEWIDTH * AREA_BITBYTEHEIGHT; // 512

    //const int	AREA_SPRITESETS		= 2;
    //const unsigned char	AREA_INVALIDSPRITESET	= 0xFF;

	// the areas doesn't really have a name, but their files do, and since we never can
	// tell how many areas there will be, we can only use this information to find neighbors
	//const int	AREA_NAMELENGTH	= 16;

	enum ENeighbors
	{
		N_NORTH = 0,
		N_EAST,
		N_SOUTH,
		N_WEST,

		NUM_NEIGHBORS
	};

	// neighbor bit fields
	const unsigned char NEIGHBIT_NORTH	= 0x1;
	const unsigned char NEIGHBIT_SOUTH	= 0x2;
	const unsigned char NEIGHBIT_WEST	= 0x4;
	const unsigned char NEIGHBIT_EAST	= 0x8;

	// When an area hasn't been loaded into the game it consists of two blocks, the static block
	// and the dynamic block. The static block is the information that seldom (almost never) changes,
	// such as the tileset and static sprites (trees, rocks etc).
	// The dynamic block consists of the items that interact with the world, moves around or just
	// is affected by time.
	// The server keeps control of both blocks, while the client only knows about the static
	// block and lets the server tell it what the current dynamic block looks like.

	// At this point (011106) I'm not sure what the areas will look like when the game is running,
	// but probably they will be a combination of both blocks (but the dynamic items could also be
	// handled in a global list, since they are able to move between areas). The area classes
	// described here are intended for the area's file system.

	// Update (020113): I'm still not 100% sure about the structure of the areas in the server, but
	// it will most likely be three blocks instead of two. The first block is the static information
	// (tiles and sprites) that are downloaded to the client. The second block will also be static, 
	// but is called "triggers" instead. These doesn't change over time, but will affect the player
	// when he walks over them or "looks" at them. For example, a sign might have a "Description
	// Trigger" that doesn't change over time but isn't downloaded either. The third block will be
	// the pure dynamic block. The items in those blocks aren't bound to specific areas, since they
	// can move across area borders.

	// Mental note: Write these files so the Borland compiler understands them.

	/*

	class CAreaStatic
	{
		public:

			CAreaStatic();
			~CAreaStatic();

            void clear();
			bool open(char* filename);
			bool save(char* filename);

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			unsigned char	u_tileset;				// the tileset the area uses
			unsigned char	ua_areacolor[3];		// rgb value for the area (on the minimap)
            unsigned char	ua_spriteset[AREA_SPRITESETS];	// the sprite sets (-1 to disable)
            unsigned short	u_version;				// area version (not file format version)

			unsigned char	ua_tiles[AREA_SIZE];	// the tiles
			unsigned char	ua_sprites[AREA_SIZE];	// the sprites are associated with the tileset

			char			ua_neighbors[NUM_NEIGHBORS];	// non-zero if neighbor exists

		private:

		protected:
	};
	*/

    // each tileset has 127 tileset sprites, numbered from 1 up to 127
    // sprites numbered from 128 to 255 are so-called "basic sprites" that
    // have other uses than just drawing images (such as changing blocking info)
    const int BASIC_SPRITES_START = 0x80;

    // blocking info changers
    enum EBasicSpriteBlockers
    {
    	BSB_CLEAR = BASIC_SPRITES_START,
        BSB_ALL,			// block all
        BSB_NONFLIERS,
        BSB_NONETHEREAL,
        BSB_NORMAL,			// block non-ethereal and non-flying

        NUM_BLOCKERS = 4
    };

	// Area Triggers ********************************************************************************

	/*

	const unsigned char AREATRIGGER_NOTRIGGER		= 0xFF;

	class CAreaTriggers
	{
		public:

			CAreaTriggers();
			~CAreaTriggers();

			void clear();
			bool open(char* filename);
			bool save(char* filename);

			unsigned char ua_triggers[AREA_SIZE];

		private:

		protected:
	};
	*/

} // end of WorldSystem namespace

#endif
