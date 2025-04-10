
#ifndef _CCLIENTAREA_H_
#define _CCLIENTAREA_H_

#include "CArea.h"
#include "CArea2.h"
#include "../Common/CBitMap.h"
#include "CTileset.h"
#include "CSpriteset.h"

namespace Akarra
{
	class CClientWorldManager;
}

namespace WorldSystem
{

	// the server area class is a bundle of the static and trigger information
	// in an area
	/*class CClientArea
	{
		public:

			CClientArea();
			~CClientArea();

			//bool buildArea(CAreaStatic* source, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites);
			bool buildArea(CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites);

		private:

			void m_drawSpriteBlocking(int startx, int starty, unsigned char sprite, CClientSpriteset* set);

		protected:

		public:

			Common::CFourBitMap	u_tileblock;					// block information for the groud (based on tileset and sprites)
			
			CAreaStatic		u_static;						// area static information
	};*/

	class CWorldArea2Cache;

	enum EAreaPositions
	{
		AP_NORTHWEST = 0,
		AP_NORTH,
		AP_NORTHEAST,
		AP_WEST,

		AP_MAIN,

		AP_EAST,
		AP_SOUTHWEST,
		AP_SOUTH,
		AP_SOUTHEAST,

		NUM_AREAPOSITIONS
	};

	const int AREA_OFFSETS_X[NUM_AREAPOSITIONS] =
		{
			-1, 0, 1,
			-1, 0, 1,
			-1, 0, 1
		};
	const int AREA_OFFSETS_Y[NUM_AREAPOSITIONS] =
		{
			-1, -1, -1,
			 0,  0,  0,
			 1,  1,  1
		};

	const int CLIENTAREA_WIDTH	= AREA2_WIDTH * 3;
	const int CLIENTAREA_HEIGHT	= AREA2_HEIGHT * 3;
	const int CLIENTAREA_SIZE	= CLIENTAREA_WIDTH * CLIENTAREA_HEIGHT;

	const int CLIENTAREA_VERTEX_WIDTH = CLIENTAREA_WIDTH * 2;
	const int CLIENTAREA_VERTEX_HEIGHT = CLIENTAREA_HEIGHT * 2;
	const int CLIENTAREA_VERTEX_SIZE = CLIENTAREA_VERTEX_WIDTH * CLIENTAREA_VERTEX_HEIGHT;

	const int LOS_BLOCKED		= -1;

	// Note about position:
	// areax/y - Indicates the area position within a sector, ranges from 0 to SECTOR2_WIDTH / SECTOR2_HEIGHT
	// cellx/y - Indicates the global world coordinate, ranges from 0 to around 32000 somewhere
	// clientx/y - Indicates the cell position within the area bundle, ranges from 0 to CLIENTAREA_WIDTH / CLIENTAREA_HEIGHT

	// 030126: Since the world areas now use a special system with auto-bordering, I needed to change some stuff
	// in this class. For example, ma_tiles has been changed to mp_tiles so the client doesn't need to load the
	// memory if it isn't used. Similarily, I've added mp_worldhandles that is used instead of mp_tiles when the
	// player is in the world. The worldhandles are built at run-time and should be looped through until a -1 is
	// reached (it's like a linked list for each cell, but I did it this way because it's faster).

	class CClientArea2
	{
		public:

			CClientArea2();
			~CClientArea2();

			//bool buildArea(int areax, int areay, CWorldArea2Cache* sector, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites);
			bool buildArea(int areax, int areay, int sectorpos, Akarra::CClientWorldManager* world, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites);

			short* getTileHandles() { return mp_worldhandles; }; // note: if this function returns NULL you should use the normal drawing method
			
			unsigned char getTile(int clientx, int clienty);
			unsigned char* getTiles() { return mp_tiles; }
			int getTileset();
			unsigned char getSprite(int clientx, int clienty);
			int getSpriteSet(int clientx, int clienty, unsigned char sprite);
			int getBlockInfo(int clientx, int clienty);

			bool checkDeltaNeighbor(int areadx, int aready);

			// light functions:

			void resetLight(int ambience, bool allvisible);
			void placeSightOrigin(int clientx, int clienty, int radius);
			void placeLightOrigin(int clientx, int clienty, int radius, int strength = 15);
			void drawSpriteLight();
			void limitLightAmount();
			void lockLight();

			inline int getLightAmount(int clientx, int clienty) { return (ma_los[clientx + (clienty << 6) + (clienty << 5)]); }
			inline int getVertexAmount(int clientx, int clienty, int tilex, int tiley) { return (ma_vertex[(clientx*2) + tilex + (((clienty*2) + tiley) * CLIENTAREA_VERTEX_WIDTH)]); }
			inline bool isVisible(int clientx, int clienty) { return ((ma_los[clientx + (clienty << 6) + (clienty << 5)]) > 0); }

			void drawSpriteBlocking(int startx, int starty, unsigned char sprite, CClientSpriteset* set);

		private:

			int m_getAreaPosForClientPos(int clientx, int clienty);

			// line of sight drawing:

			bool m_LOSBlocked(int x, int y);
			void m_makeVisible(int x, int y);

			float m_slope(float x1, float y1, float x2, float y2);
			float m_invSlope(float x1, float y1, float x2, float y2);

			void m_scanNW2N(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanNE2N(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanNW2W(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanSW2W(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanNE2E(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanSE2E(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanSW2S(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_scanSE2S(int xc, int yc, int distance, int radius, float sslope, float eslope);

			// light drawing (very similar to LOS drawing)
			// the light drawing requires to know the center point of the start light, so it can
			// calculate the amount of light... these are stored as m_lightx, m_lighty and m_lightradius

			void m_placeLight(int x, int y);

			void m_lightNW2N(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightNE2N(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightNW2W(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightSW2W(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightNE2E(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightSE2E(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightSW2S(int xc, int yc, int distance, int radius, float sslope, float eslope);
			void m_lightSE2S(int xc, int yc, int distance, int radius, float sslope, float eslope);

		protected:

			CArea2Static		ma_static[NUM_AREAPOSITIONS];	// area static information

			// copied information:

			//unsigned char		ma_tiles[CLIENTAREA_SIZE];
			unsigned char*		mp_tiles;
			unsigned char		ma_sprites[CLIENTAREA_SIZE];
			short*				mp_worldhandles;

			Common::CFourBitMap	m_tileblock;	// block info (size is for all nine areas)

			CClientSpritesetBundle*			mp_spritesets;	// sent to us from the world handler

			// line of sight information:

			int					ma_los[CLIENTAREA_SIZE];	// negative values indicate that the area cant be seen, otherwise it's the light amount (0 can't be seen either)
			int					ma_vertex[CLIENTAREA_VERTEX_SIZE];
			int					m_ambience;					// amount of basic light
			bool				m_lightlock;
			bool				m_allvisible;				// there is no LOS

			// light drawing:

			int					m_lightx;
			int					m_lighty;
			int					m_lightradius;
			float				m_lightstrength;
	};


} // end of WorldSystem namespace


#endif
