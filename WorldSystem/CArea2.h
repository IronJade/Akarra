/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    New area class, compressed with weird-looking compression algorithm.
 **/

#ifndef _CAREA2_H_
#define _CAREA2_H_

#include "../library/FileSystem/CFileStream.h"
#include "CArea.h"

namespace WorldSystem
{

	//const int	AREA_FILEVERSION	= 1;

	const int	AREA2_WIDTH			= 32;
	const int	AREA2_HEIGHT		= 32;
	const int	AREA2_SIZE			= AREA2_WIDTH * AREA2_HEIGHT; // 1024
	const int	AREA2_BITBYTEWIDTH	= AREA2_WIDTH / 8;	// size if each tile is a single bit
	const int	AREA2_BITBYTEHEIGHT	= AREA2_HEIGHT;		// still need 64 rows
	const int	AREA2_BITBYTESIZE	= AREA2_BITBYTEWIDTH * AREA2_BITBYTEHEIGHT; // 128

    const int			AREA2_SPRITESETS		= 2;
    const unsigned char	AREA2_INVALIDSPRITESET	= 0xFF;

	// the areas doesn't really have a name, but their files do, and since we never can
	// tell how many areas there will be, we can only use this information to find neighbors
	const int	AREA2_NAMELENGTH	= 16;

	// these are the same as in CArea.h
	//enum ENeighbors
	//{
	//	N_NORTH = 0,
	//	N_EAST,
	//	N_SOUTH,
	//	N_WEST,
	//
	//	NUM_NEIGHBORS
	//};

	// neighbor bit fields
	//const unsigned char NEIGHBIT_NORTH	= 0x1;
	//const unsigned char NEIGHBIT_SOUTH	= 0x2;
	//const unsigned char NEIGHBIT_WEST	= 0x4;
	//const unsigned char NEIGHBIT_EAST	= 0x8;

	enum EAreaTypes
	{
		AT_NOTUSED = 0,

		AT_NORMAL,				// normal area type (caves etc)
		AT_WORLD,				// world area type with 16 pre-defined tiletypes
		AT_NORMALCOMPRESSED,	// same as above, but compressed
		AT_WORLDCOMPRESSED,		// - " -

		NUM_AREATYPES
	};

	enum EAreaFileVersions
	{
		AFV_ORIGINAL = 0,

		AFV_CURRENT
	};

	enum EAreaRotationTypes
	{
		AR_NOTUSED = 0,
		AR_90,
		AR_90M,
		AR_180,
		AR_180M,
		AR_270,
		AR_270M,
		AR_HORISONTAL,
		AR_VERTICAL
	};

	class CArea2Static
	{
		public:

			CArea2Static();
			~CArea2Static();

            void clear();
			void removeAll();

			int createSaveData(int type);

			bool open(char* filename, bool oldstyle);
			bool save(char* filename);

			bool read(FileSystem::CFileStream& file, bool oldstyle, bool dodFix);
			bool write(FileSystem::CFileStream& file);

			bool copyArea(CArea2Static* src, int rotation = AR_NOTUSED);
			void flipArea(int rotation);
			void swapTiles(int from, int to);

			// data access functions

			unsigned short getVersion();

			bool hasNeighbor(int neighbor);
			void setNeighbor(int neighbor, bool onoff);

			unsigned char getTileset();
			void setTileset(unsigned char set);
			unsigned char getSpriteset(int index);
			void setSpriteset(int index, unsigned char set);

			void getAreaColor(unsigned char& r, unsigned char& g, unsigned char& b);
			void setAreaColor(unsigned char r, unsigned char g, unsigned char b);

			unsigned char* getTilesReadOnly();
			void setTile(int x, int y, unsigned char value);
			unsigned char* getSpritesReadOnly();
			void setSprite(int x, int y, unsigned char sprite);

		public:

			int				u_areatype;				// out of EAreaTypes (usually set by the world cache)

		private:

			bool			m_modified;				// if this is set to true, the version count should increase when saved

			unsigned char*	mp_savedata;			// data buffer for saving
			int				m_datasize;

            unsigned short	m_version;				// area version (not file format version)

		protected:

			unsigned char	m_tileset;				// the tileset the area uses
			unsigned char	ma_areacolor[3];		// rgb value for the area (on the minimap)
            unsigned char	ma_spriteset[AREA2_SPRITESETS];	// the sprite sets (-1 to disable)

			unsigned char	ma_tiles[AREA2_SIZE];	// the tiles
			unsigned char	ma_sprites[AREA2_SIZE];	// the sprites are associated with the tileset

			char			ma_neighbors[NUM_NEIGHBORS];	// non-zero if neighbor exists
	};

	const unsigned int COMPRESS_INDEXSIZE = 32768;
	class CAreaCompress
	{
		public:
			static int compressData(unsigned char* dest, unsigned char* src, int size);
			static int deCompressData(unsigned char* dest, unsigned char* src, int size);

			static unsigned char us_indices[COMPRESS_INDEXSIZE];
	};

} // end of WorldSystem namespace

#endif
