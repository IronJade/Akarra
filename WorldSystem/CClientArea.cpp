
#include "CClientArea.h"
#include "CWorldAreaCache.h"
#include "TilesetData.h"
#include "../Common/AkarraUtil.h"
#include "../../Akarra/CClientWorldManager.h"

namespace WorldSystem
{

/*
CClientArea::CClientArea()
{
	u_tileblock.create(AREA_WIDTH, AREA_HEIGHT);
}

CClientArea::~CClientArea()
{
}

//bool CClientArea::buildArea(CAreaStatic* source, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites)
bool CClientArea::buildArea(CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites)
{
	//if (!source || !tilesets || !sprites)
	if (!tilesets || !sprites)
	{
		return false;
	}

	// copy static information
	//u_static = *source;

	// get tileset and spritesets
	CClientTileset* ts = tilesets->getTileset(u_static.u_tileset);
	CClientSpriteset* sets[3];
	sets[0] = sprites->getSpriteset(u_static.ua_spriteset[0]);
	sets[1] = sprites->getSpriteset(u_static.ua_spriteset[0]);
	sets[2] = sprites->getSpriteset(-1); // basic spriteset

	if (!ts || !sets[0] || !sets[1] || !sets[2])
	{
		return false;
	}

	int x, y;

	// fill our block map from the tiles first
	for (y = 0; y < AREA_HEIGHT; y++)
	{
		for (x = 0; x < AREA_WIDTH; x++)
		{
			// get tile from map
			unsigned char tile = u_static.ua_tiles[x + (y * AREA_WIDTH)];
			// get block information from the tileset
			unsigned char block = ts->ua_blockinfo[tile];
			// insert the block info in our map
			u_tileblock.setPos(x, y, (Common::TDFourBit)block);
		}
	}

	// find sprites on the area
	for (y = 0; y < AREA_HEIGHT; y++)
	{
		for (x = 0; x < AREA_WIDTH; x++)
		{
			unsigned char sprite = u_static.ua_sprites[x + (y * AREA_WIDTH)];
			if (sprite != NO_SPRITE)
			{
				// check from which spriteset the sprite has been taken
				unsigned char set = (sprite & SPRITE_SET_BITS) >> 6;
				sprite &= SPRITE_ID_BITS;

				m_drawSpriteBlocking(x - sets[set]->ua_sprites[sprite].ox, y - sets[set]->ua_sprites[sprite].oy, sprite, sets[set]);				
			}
		}
	}

	return true;
}

void CClientArea::m_drawSpriteBlocking(int x, int y, unsigned char sprite, CClientSpriteset* set)
{
	if (sprite < MIN_SPRITES || sprite >= MAX_SPRITES || !set)
    {
    	return;
    }

    if (set->ua_sprites[sprite].x != INVALID_SPRITE)
    {
    	for (int sy = 0; sy < set->ua_sprites[sprite].height; sy++)
        {
    		for (int sx = 0; sx < set->ua_sprites[sprite].width; sx++)
        	{
            	if (((sx + x) >= 0) &&
                	((sx + x) < AREA_WIDTH) &&
                    ((sy + y) >= 0) &&
                    ((sy + y) < AREA_HEIGHT))
                {
                	unsigned char block = set->ua_blocking[(sx + set->ua_sprites[sprite].x) + (sy + set->ua_sprites[sprite].y) * CELL_COLUMNS];

                    if (block != BT_BLOCKNONE)
                    {
						if (block == BT_FORCECLEAR)
						{
							u_tileblock.setPos(sx + x, sy + y, 0);
						}
						else
						{
							u_tileblock.setPos(sx + x, sy + y, (Common::TDFourBit)block);
						}
                    }
                }
        	}
        }
    }
}
*/

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientArea2::CClientArea2()
{
	m_tileblock.create(CLIENTAREA_WIDTH, CLIENTAREA_HEIGHT);
	m_lightlock = false;

	mp_tiles = NULL;
	mp_worldhandles = NULL;
	mp_spritesets = NULL;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CClientArea2::~CClientArea2()
{
	delete [] mp_tiles;
	delete [] mp_worldhandles;
}

//bool CClientArea::buildArea(CAreaStatic* source, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites)
/*
bool CClientArea2::buildArea(CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites)
{
	//if (!source || !tilesets || !sprites)
	if (!tilesets || !sprites)
	{
		return false;
	}

	// copy static information
	//u_static = *source;

	// get tileset and spritesets
	CClientTileset* ts = tilesets->getTileset(u_static.u_tileset);
	CClientSpriteset* sets[3];
	sets[0] = sprites->getSpriteset(u_static.ua_spriteset[0]);
	sets[1] = sprites->getSpriteset(u_static.ua_spriteset[0]);
	sets[2] = sprites->getSpriteset(-1); // basic spriteset

	if (!ts || !sets[0] || !sets[1] || !sets[2])
	{
		return false;
	}

	int x, y;

	// fill our block map from the tiles first
	for (y = 0; y < AREA2_HEIGHT; y++)
	{
		for (x = 0; x < AREA2_WIDTH; x++)
		{
			// get tile from map
			unsigned char tile = u_static.ua_tiles[x + (y * AREA2_WIDTH)];
			// get block information from the tileset
			unsigned char block = ts->ua_blockinfo[tile];
			// insert the block info in our map
			u_tileblock.setPos(x, y, (Common::TDFourBit)block);
		}
	}

	// find sprites on the area
	for (y = 0; y < AREA2_HEIGHT; y++)
	{
		for (x = 0; x < AREA2_WIDTH; x++)
		{
			unsigned char sprite = u_static.ua_sprites[x + (y * AREA2_WIDTH)];
			if (sprite != NO_SPRITE)
			{
				// check from which spriteset the sprite has been taken
				unsigned char set = (sprite & SPRITE_SET_BITS) >> 6;
				sprite &= SPRITE_ID_BITS;

				m_drawSpriteBlocking(x - sets[set]->ua_sprites[sprite].ox, y - sets[set]->ua_sprites[sprite].oy, sprite, sets[set]);				
			}
		}
	}

	return true;
}
*/

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//bool CClientArea2::buildArea(int areax, int areay, CWorldArea2Cache* sector, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites)
bool CClientArea2::buildArea(int areax, int areay, int sectorpos, Akarra::CClientWorldManager* world, CClientTilesetBundle* tilesets, CClientSpritesetBundle* sprites)
{
	if (!world || !tilesets || !sprites)
	{
		return false;
	}
	mp_spritesets = sprites;

	// clear all areas
	{
		for (int i(0); i < NUM_AREAPOSITIONS; i++)
		{
			ma_static[i].clear();
		}
	}

	// get areas from the sector
	{
		// get main area
		world->getAreaFromSectors(sectorpos, areax, areay, &ma_static[AP_MAIN]);

		// check neighbors and get those areas
		if (ma_static[AP_MAIN].hasNeighbor(N_NORTH))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_NORTH], areay + AREA_OFFSETS_Y[AP_NORTH], &ma_static[AP_NORTH]);
		}
		if (ma_static[AP_MAIN].hasNeighbor(N_EAST))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_EAST], areay + AREA_OFFSETS_Y[AP_EAST], &ma_static[AP_EAST]);
		}
		if (ma_static[AP_MAIN].hasNeighbor(N_SOUTH))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_SOUTH], areay + AREA_OFFSETS_Y[AP_SOUTH], &ma_static[AP_SOUTH]);
		}
		if (ma_static[AP_MAIN].hasNeighbor(N_WEST))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_WEST], areay + AREA_OFFSETS_Y[AP_WEST], &ma_static[AP_WEST]);
		}

		// check diagonal neighbors
		if (ma_static[AP_NORTH].hasNeighbor(N_WEST) || ma_static[AP_WEST].hasNeighbor(N_NORTH))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_NORTHWEST], areay + AREA_OFFSETS_Y[AP_NORTHWEST], &ma_static[AP_NORTHWEST]);
		}
		if (ma_static[AP_NORTH].hasNeighbor(N_EAST) || ma_static[AP_EAST].hasNeighbor(N_NORTH))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_NORTHEAST], areay + AREA_OFFSETS_Y[AP_NORTHEAST], &ma_static[AP_NORTHEAST]);
		}
		if (ma_static[AP_SOUTH].hasNeighbor(N_WEST) || ma_static[AP_WEST].hasNeighbor(N_SOUTH))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_SOUTHWEST], areay + AREA_OFFSETS_Y[AP_SOUTHWEST], &ma_static[AP_SOUTHWEST]);
		}
		if (ma_static[AP_SOUTH].hasNeighbor(N_EAST) || ma_static[AP_EAST].hasNeighbor(N_SOUTH))
		{
			world->getAreaFromSectors(sectorpos, areax + AREA_OFFSETS_X[AP_SOUTHEAST], areay + AREA_OFFSETS_Y[AP_SOUTHEAST], &ma_static[AP_SOUTHEAST]);
		}
	}

	delete [] mp_tiles;
	delete [] mp_worldhandles;
	mp_tiles = NULL;
	mp_worldhandles = NULL;

	// 030126: Now we need to check what kind of tileset system we are using
	// NEW WORLD SYSTEM --------------------------- ||| --------------------------- |||
	if ((ma_static[AP_MAIN].u_areatype == AT_WORLD) || (ma_static[AP_MAIN].u_areatype == AT_WORLDCOMPRESSED))
	{
		// unfortunately we need to copy the normal system until the handles have been built
		mp_tiles = new unsigned char [CLIENTAREA_SIZE];

		// copy the information to a common array
		{
			for (int i(0); i < NUM_AREAPOSITIONS; i++)
			{
				for (int y(0); y < AREA2_HEIGHT; y++)
				{
					for (int x(0); x < AREA2_WIDTH; x++)
					{
						int pos = x + AREA2_WIDTH + (AREA_OFFSETS_X[i] * AREA2_WIDTH);
						pos += (y + AREA2_HEIGHT + (AREA_OFFSETS_Y[i] * AREA2_HEIGHT)) * CLIENTAREA_WIDTH;

						mp_tiles[pos] = ma_static[i].getTilesReadOnly()[x + y * AREA2_WIDTH];
						ma_sprites[pos] = ma_static[i].getSpritesReadOnly()[x + y * AREA2_WIDTH];

						if (mp_tiles[pos] > 0xF)
						{
							mp_tiles[pos] = 0xF;
						}
					}
				}
			}
		}

		// new world system, where each tile is max 15 and the borders are automatically created
		mp_worldhandles = new short [CLIENTAREA_SIZE * 10];

		// create the tileset handles
		{
			for (int i(0); i < NUM_AREAPOSITIONS; i++)
			{
				for (int y(0); y < AREA2_HEIGHT; y++)
				{
					for (int x(0); x < AREA2_WIDTH; x++)
					{
						int pos = x + AREA2_WIDTH + (AREA_OFFSETS_X[i] * AREA2_WIDTH);
						pos += (y + AREA2_HEIGHT + (AREA_OFFSETS_Y[i] * AREA2_HEIGHT)) * CLIENTAREA_WIDTH;
						// then multiply by 10 since each position can have up to 10 handles
						int handlepos = pos * 10;

						// get terrain value
						int terrain = (int)mp_tiles[pos];
						int count = 0;

						mp_worldhandles[handlepos + count] = getTileHandle(terrain, x, y);
						count++;
						if ((x > 0 || AREA_OFFSETS_X[i] >= 0) && (x < 31 || AREA_OFFSETS_X[i] <= 0) &&
							(y > 0 || AREA_OFFSETS_Y[i] >= 0) && (y < 31 || AREA_OFFSETS_Y[i] <= 0))
						{
							for (unsigned char i(terrain + 1); i < 16; i++)
							{
								unsigned char neighbors = 0;

								// check north/south/east/west first
								if (mp_tiles[pos - 1] == i)
								{
									// west
									neighbors |= 0x01;
								}
								if (mp_tiles[pos - CLIENTAREA_WIDTH] == i)
								{
									// north (we don't need to check northwest or northeast)
									neighbors |= 0x02;
								}
								if (mp_tiles[pos + 1] == i)
								{
									// north (we don't need to check northwest or northeast)
									neighbors |= 0x04;
								}
								if (mp_tiles[pos + CLIENTAREA_WIDTH] == i)
								{
									// north (we don't need to check northwest or northeast)
									neighbors |= 0x08;
								}
								if (neighbors)
								{
									mp_worldhandles[handlepos + count] = (getTypeTilebase(i)) + (getCellTilebase(x, y) & 1) + ((int)(neighbors & 0x0F) * 4);
									count++;
								}
								// now check northeast, southwest etc
								if (((neighbors & 0x03) == 0) && (mp_tiles[pos - 1 - CLIENTAREA_WIDTH] == i))
								{
									neighbors |= 0x10;
								}
								if (((neighbors & 0x06) == 0) && (mp_tiles[pos + 1 - CLIENTAREA_WIDTH] == i))
								{
									neighbors |= 0x20;
								}
								if (((neighbors & 0x0C) == 0) && (mp_tiles[pos + 1 + CLIENTAREA_WIDTH] == i))
								{
									neighbors |= 0x40;
								}
								if (((neighbors & 0x09) == 0) && (mp_tiles[pos - 1 + CLIENTAREA_WIDTH] == i))
								{
									neighbors |= 0x80;
								}
								neighbors &= 0xF0;
								if (neighbors)
								{
									neighbors >>= 4;
									mp_worldhandles[handlepos + count] = (getTypeTilebase(i)) + (getCellTilebase(x, y) & 1) + ((int)(neighbors & 0x0F) * 4) + 2;
									count++;
								}
							}
						}

						mp_worldhandles[handlepos + count] = -1; // break handle						
					}
				}
			}
		}

		// build block info
		m_tileblock.reset();

		{
			// get tileset and spritesets
			CClientSpriteset* sets[NUM_AREAPOSITIONS * 2 + 1];	// an extra slot for the "basic" set
			sets[NUM_AREAPOSITIONS * 2] = sprites->getSpriteset(-1); // basic spriteset

			for (int i(0); i < (NUM_AREAPOSITIONS * 2); i += 2)
			{
				sets[i] = sprites->getSpriteset(ma_static[i / 2].getSpriteset(0));
				sets[i+1] = sprites->getSpriteset(ma_static[i / 2].getSpriteset(1));
			}

			for (int y(0); y < CLIENTAREA_HEIGHT; y++)
			{
				for (int x(0); x < CLIENTAREA_WIDTH; x++)
				{
					// set info for base tiles
					unsigned char block = getBlockType(mp_tiles[x + (y * CLIENTAREA_WIDTH)]);
					m_tileblock.setPos(x, y, (Common::TDFourBit)block);

					// look for sprites
					unsigned char sprite = ma_sprites[x + (y * CLIENTAREA_WIDTH)];
					if (sprite != NO_SPRITE)
					{
						unsigned char set = (sprite & SPRITE_SET_BITS) >> 6;
						sprite &= SPRITE_ID_BITS;

						if (set > 1)
						{
							// this is the basic set
							set = NUM_AREAPOSITIONS * 2;
						}
						else
						{
							// get correct set
							set += (m_getAreaPosForClientPos(x, y) * 2);
						}

						drawSpriteBlocking(x - sets[set]->ua_sprites[sprite].ox, y - sets[set]->ua_sprites[sprite].oy, sprite, sets[set]);				
					}
				}
			}
		}

		// we don't need the tiles any longer
		// 030516: YES, we need them for the minimap :P
		//delete [] mp_tiles;
		//mp_tiles = NULL;
	}
	// OLD DEFAULT SYSTEM --------------------------- ||| --------------------------- |||
	else
	{
		// normal system
		mp_tiles = new unsigned char [CLIENTAREA_SIZE];

		// copy the information to a common array
		{
			for (int i(0); i < NUM_AREAPOSITIONS; i++)
			{
				for (int y(0); y < AREA2_HEIGHT; y++)
				{
					for (int x(0); x < AREA2_WIDTH; x++)
					{
						int pos = x + AREA2_WIDTH + (AREA_OFFSETS_X[i] * AREA2_WIDTH);
						pos += (y + AREA2_HEIGHT + (AREA_OFFSETS_Y[i] * AREA2_HEIGHT)) * CLIENTAREA_WIDTH;

						mp_tiles[pos] = ma_static[i].getTilesReadOnly()[x + y * AREA2_WIDTH];
						ma_sprites[pos] = ma_static[i].getSpritesReadOnly()[x + y * AREA2_WIDTH];
					}
				}
			}
		}

		// build block info
		m_tileblock.reset();

		{
			// get tileset and spritesets
			CClientTileset* ts = tilesets->getTileset(ma_static[AP_MAIN].getTileset());
			CClientSpriteset* sets[NUM_AREAPOSITIONS * 2 + 1];	// an extra slot for the "basic" set
			sets[NUM_AREAPOSITIONS * 2] = sprites->getSpriteset(-1); // basic spriteset

			for (int i(0); i < (NUM_AREAPOSITIONS * 2); i += 2)
			{
				sets[i] = sprites->getSpriteset(ma_static[i / 2].getSpriteset(0));
				sets[i+1] = sprites->getSpriteset(ma_static[i / 2].getSpriteset(1));
			}

			if (!ts)
			{
				return false;
			}

			for (int y(0); y < CLIENTAREA_HEIGHT; y++)
			{
				for (int x(0); x < CLIENTAREA_WIDTH; x++)
				{
					// set info for base tiles
					unsigned char tile = mp_tiles[x + (y * CLIENTAREA_WIDTH)];
					unsigned char block = ts->ua_blockinfo[tile];
					m_tileblock.setPos(x, y, (Common::TDFourBit)block);

					// look for sprites
					unsigned char sprite = ma_sprites[x + (y * CLIENTAREA_WIDTH)];
					if (sprite != NO_SPRITE)
					{
						unsigned char set = (sprite & SPRITE_SET_BITS) >> 6;
						sprite &= SPRITE_ID_BITS;

						if (set > 1)
						{
							// this is the basic set
							set = NUM_AREAPOSITIONS * 2;
						}
						else
						{
							// get correct set
							set += (m_getAreaPosForClientPos(x, y) * 2);
						}

						drawSpriteBlocking(x - sets[set]->ua_sprites[sprite].ox, y - sets[set]->ua_sprites[sprite].oy, sprite, sets[set]);				
					}
				}
			}
		}
	}

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned char CClientArea2::getTile(int clientx, int clienty)
{
	return mp_tiles[clientx + (clienty * CLIENTAREA_WIDTH)];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CClientArea2::getTileset()
{
	return (int)ma_static[AP_MAIN].getTileset();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
unsigned char CClientArea2::getSprite(int clientx, int clienty)
{
	return ma_sprites[clientx + (clienty * CLIENTAREA_WIDTH)];
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CClientArea2::getSpriteSet(int clientx, int clienty, unsigned char sprite)
{
	unsigned char set = (sprite & SPRITE_SET_BITS) >> 6;
	//if (set != 0)
	//{
	//	int bakasjkdghaskjh = 0;
	//}
	if (set > 1)
	{
		return -1;
	}

	int pos = m_getAreaPosForClientPos(clientx, clienty);
	return ma_static[pos].getSpriteset(set);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CClientArea2::getBlockInfo(int clientx, int clienty)
{
	return (int)m_tileblock.getPos(clientx, clienty);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CClientArea2::checkDeltaNeighbor(int areadx, int aready)
{
	if (areadx < 0)
	{
		if (aready < 0)
		{
			if ((ma_static[AP_MAIN].hasNeighbor(N_WEST) && ma_static[AP_WEST].hasNeighbor(N_NORTH)) ||
				(ma_static[AP_MAIN].hasNeighbor(N_NORTH) && ma_static[AP_NORTH].hasNeighbor(N_WEST)))
			{
				return true;
			}
			return false;
		}
		else if (aready > 0)
		{
			if ((ma_static[AP_MAIN].hasNeighbor(N_WEST) && ma_static[AP_WEST].hasNeighbor(N_SOUTH)) ||
				(ma_static[AP_MAIN].hasNeighbor(N_SOUTH) && ma_static[AP_SOUTH].hasNeighbor(N_WEST)))
			{
				return true;
			}
			return false;
		}
		else
		{
			if (ma_static[AP_MAIN].hasNeighbor(N_WEST))
			{
				return true;
			}
			return false;
		}
	}
	else if (areadx > 0)
	{
		if (aready < 0)
		{
			if ((ma_static[AP_MAIN].hasNeighbor(N_EAST) && ma_static[AP_EAST].hasNeighbor(N_NORTH)) ||
				(ma_static[AP_MAIN].hasNeighbor(N_NORTH) && ma_static[AP_NORTH].hasNeighbor(N_EAST)))
			{
				return true;
			}
			return false;
		}
		else if (aready > 0)
		{
			if ((ma_static[AP_MAIN].hasNeighbor(N_EAST) && ma_static[AP_EAST].hasNeighbor(N_SOUTH)) ||
				(ma_static[AP_MAIN].hasNeighbor(N_SOUTH) && ma_static[AP_SOUTH].hasNeighbor(N_EAST)))
			{
				return true;
			}
			return false;
		}
		else
		{
			if (ma_static[AP_MAIN].hasNeighbor(N_EAST))
			{
				return true;
			}
			return false;
		}
	}
	else if (aready < 0)
	{
		if (ma_static[AP_MAIN].hasNeighbor(N_NORTH))
		{
			return true;
		}
		return false;
	}
	else if (aready > 0)
	{
		if (ma_static[AP_MAIN].hasNeighbor(N_SOUTH))
		{
			return true;
		}
		return false;
	}

	// main area
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::resetLight(int ambience, bool allvisible)
{
	//memset(ma_los, ambience, sizeof(int) * CLIENTAREA_SIZE);

	m_allvisible = allvisible;

	// do an integer copy
	{	
		int count = CLIENTAREA_SIZE;
		int value = (m_allvisible) ? ambience : LOS_BLOCKED;	// set everything to -1 so it is blocked
		int* dest = ma_los;

		_asm
		{
			mov		eax, value		// store color
			mov		edi, dest		// store destination
			mov		ecx, count		// store number of copies

			rep		stosd			// perform the copy
		}
		
	}

	m_lightlock = false;
	m_ambience = ambience;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::placeSightOrigin(int clientx, int clienty, int radius)
{
	if (m_lightlock || m_allvisible)
	{
		return;
	}

	// apply starting cell
	m_makeVisible(clientx, clienty);

	if(radius > 0)
	{
		// scan and apply north
		// until a blocking cell is hit or
		// until radius is reached
		int nL;
		for(nL = 1; nL <= radius; nL++)
		{
			m_makeVisible(clientx, clienty - nL);
			if(m_LOSBlocked(clientx, clienty - nL))
			{
				break;
			}
		}

		// scan and apply north east
		// until a blocking cell is hit or
		// until radius is reached
		int neL;
		for(neL = 1; neL <= radius; neL++)
		{
			m_makeVisible(clientx + neL, clienty - neL);
			if(m_LOSBlocked(clientx + neL, clienty - neL))
			{
				break;
			}
		}

		// scan and apply east
		// until a blocking cell is hit or
		// until radius is reached
		int eL;
		for(eL = 1; eL <= radius; eL++)
		{
			m_makeVisible(clientx + eL, clienty);
			if(m_LOSBlocked(clientx + eL, clienty))
			{
				break;
			}
		}

		// scan and apply south east
		// until a blocking cell is hit or
		// until radius is reached
		int seL;
		for(seL = 1; seL <= radius; seL++)
		{
			m_makeVisible(clientx + seL, clienty + seL);
			if(m_LOSBlocked(clientx + seL, clienty + seL))
			{
				break;
			}
		}

		// scan and apply south
		// until a blocking cell is hit or
		// until radius is reached
		int sL;
		for(sL = 1; sL <= radius; sL++)
		{
			m_makeVisible(clientx, clienty + sL);
			if(m_LOSBlocked(clientx, clienty + sL))
			{
				break;
			}
		}

		// scan and apply south west
		// until a blocking cell is hit or
		// until radius is reached
		int swL;
		for(swL = 1; swL <= radius; swL++)
		{
			m_makeVisible(clientx - swL, clienty + swL);
			if(m_LOSBlocked(clientx - swL, clienty + swL))
			{
				break;
			}
		}

		// scan and apply west
		// until a blocking cell is hit or
		// until radius is reached
		int wL;
		for(wL = 1; wL <= radius; wL++)
		{
			m_makeVisible(clientx - wL, clienty);
			if(m_LOSBlocked(clientx - wL, clienty))
			{
				break;
			}
		}

		// scan and apply north west
		// until a blocking cell is hit or
		// until radius is reached
		int nwL;
		for(nwL = 1; nwL <= radius; nwL++)
		{
			m_makeVisible(clientx - nwL, clienty - nwL);
			if(m_LOSBlocked(clientx - nwL, clienty - nwL))
			{
				break;
			}
		}


		// scan the octant covering the area from north west to north
		// if it isn't blocked
		if(nL != 1 || nwL != 1)
		{
			m_scanNW2N(clientx, clienty, 1, radius, 1, 0);
		}

		// scan the octant covering the area from north east to north
		// if it isn't blocked
		if(nL != 1 || neL != 1)
		{
			m_scanNE2N(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from north west to west
		// if it isn't blocked
		if(nwL != 1 || wL != 1)
		{
			m_scanNW2W(clientx, clienty, 1, radius, 1, 0);
		}

		// scan the octant covering the area from south west to west
		// if it isn't blocked
		if(swL != 1 || wL != 1)
		{
			m_scanSW2W(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from south west to south
		// if it isn't blocked
		if(swL != 1 || sL != 1)
		{
			m_scanSW2S(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from south east to south
		// if it isn't blocked
		if(seL != 1 || sL != 1)
		{
			m_scanSE2S(clientx, clienty, 1, radius, 1, 0);
		}

		// scan the octant covering the area from north east to east
		// if it isn't blocked
		if(neL != 1 || eL != 1)
		{
			m_scanNE2E(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from south east to east
		// if it isn't blocked
		if(seL != 1 || eL != 1)
		{
			m_scanSE2E(clientx, clienty, 1, radius, 1, 0);
		}

	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::placeLightOrigin(int clientx, int clienty, int radius, int strength)
{
	if (m_lightlock)
	{
		return;
	}

	m_lightx = clientx;
	m_lighty = clienty;
	m_lightradius = radius;
	m_lightstrength = (float)strength;

	// apply starting cell
	m_placeLight(clientx, clienty);

	if(radius > 0)
	{
		// scan and apply north
		// until a blocking cell is hit or
		// until radius is reached
		int nL;
		for(nL = 1; nL <= radius; nL++)
		{
			m_placeLight(clientx, clienty - nL);
			if(m_LOSBlocked(clientx, clienty - nL))
			{
				break;
			}
		}

		// scan and apply north east
		// until a blocking cell is hit or
		// until radius is reached
		int neL;
		for(neL = 1; neL <= radius; neL++)
		{
			m_placeLight(clientx + neL, clienty - neL);
			if(m_LOSBlocked(clientx + neL, clienty - neL))
			{
				break;
			}
		}

		// scan and apply east
		// until a blocking cell is hit or
		// until radius is reached
		int eL;
		for(eL = 1; eL <= radius; eL++)
		{
			m_placeLight(clientx + eL, clienty);
			if(m_LOSBlocked(clientx + eL, clienty))
			{
				break;
			}
		}

		// scan and apply south east
		// until a blocking cell is hit or
		// until radius is reached
		int seL;
		for(seL = 1; seL <= radius; seL++)
		{
			m_placeLight(clientx + seL, clienty + seL);
			if(m_LOSBlocked(clientx + seL, clienty + seL))
			{
				break;
			}
		}

		// scan and apply south
		// until a blocking cell is hit or
		// until radius is reached
		int sL;
		for(sL = 1; sL <= radius; sL++)
		{
			m_placeLight(clientx, clienty + sL);
			if(m_LOSBlocked(clientx, clienty + sL))
			{
				break;
			}
		}

		// scan and apply south west
		// until a blocking cell is hit or
		// until radius is reached
		int swL;
		for(swL = 1; swL <= radius; swL++)
		{
			m_placeLight(clientx - swL, clienty + swL);
			if(m_LOSBlocked(clientx - swL, clienty + swL))
			{
				break;
			}
		}

		// scan and apply west
		// until a blocking cell is hit or
		// until radius is reached
		int wL;
		for(wL = 1; wL <= radius; wL++)
		{
			m_placeLight(clientx - wL, clienty);
			if(m_LOSBlocked(clientx - wL, clienty))
			{
				break;
			}
		}

		// scan and apply north west
		// until a blocking cell is hit or
		// until radius is reached
		int nwL;
		for(nwL = 1; nwL <= radius; nwL++)
		{
			m_placeLight(clientx - nwL, clienty - nwL);
			if(m_LOSBlocked(clientx - nwL, clienty - nwL))
			{
				break;
			}
		}


		// scan the octant covering the area from north west to north
		// if it isn't blocked
		if(nL != 1 || nwL != 1)
		{
			m_lightNW2N(clientx, clienty, 1, radius, 1, 0);
		}

		// scan the octant covering the area from north east to north
		// if it isn't blocked
		if(nL != 1 || neL != 1)
		{
			m_lightNE2N(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from north west to west
		// if it isn't blocked
		if(nwL != 1 || wL != 1)
		{
			m_lightNW2W(clientx, clienty, 1, radius, 1, 0);
		}

		// scan the octant covering the area from south west to west
		// if it isn't blocked
		if(swL != 1 || wL != 1)
		{
			m_lightSW2W(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from south west to south
		// if it isn't blocked
		if(swL != 1 || sL != 1)
		{
			m_lightSW2S(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from south east to south
		// if it isn't blocked
		if(seL != 1 || sL != 1)
		{
			m_lightSE2S(clientx, clienty, 1, radius, 1, 0);
		}

		// scan the octant covering the area from north east to east
		// if it isn't blocked
		if(neL != 1 || eL != 1)
		{
			m_lightNE2E(clientx, clienty, 1, radius, -1, 0);
		}

		// scan the octant covering the area from south east to east
		// if it isn't blocked
		if(seL != 1 || eL != 1)
		{
			m_lightSE2E(clientx, clienty, 1, radius, 1, 0);
		}
	}

	/*
	int sx = clientx - radius;
	int ex = clientx + radius;
	int sy = clienty - radius;
	int ey = clienty + radius;

	if (ex < 0 || ey < 0 || sx >= CLIENTAREA_WIDTH || sy >= CLIENTAREA_HEIGHT)
	{
		// not within the current area region
		return;
	}
	if (sx < 0)
	{
		sx = 0;
	}
	if (sy < 0)
	{
		sy = 0;
	}
	if (ex >= CLIENTAREA_WIDTH)
	{
		ex = CLIENTAREA_WIDTH;
	}
	if (ey >= CLIENTAREA_HEIGHT)
	{
		ey = CLIENTAREA_HEIGHT;
	}

	for (int y(sy); y < (ey); y++)
	{
		for (int x(sx); x < (ex); x++)
		{
			int pos = x + (y * CLIENTAREA_WIDTH);
			if (ma_los[pos] < 0)
			{
				// this cell's line of sight is blocked
				continue;
			}

			// get a distance value
			float delta = Common::getCellDistance(x - clientx, y - clienty);
			int d2 = (int)(((1.0f - ((float)delta / (float)radius)) * 15.0f) + 0.5f);

			if (d2 < 1)
			{
				continue;
			}

			// we can safely add the value since the whole area will be limited in
			// the limitLightAmount() function later on
			ma_los[pos] += d2;
		}
	}
	*/
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function places extra light on the area depending on the value in the spritesets.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::drawSpriteLight()
{
	if (m_lightlock || !mp_spritesets)
	{
		return;
	}

	unsigned char* sprite = &ma_sprites[0];

	for (int y = 0; y < CLIENTAREA_HEIGHT; y++)
	{
		for (int x = 0; x < CLIENTAREA_WIDTH; x++)
		{
			if (*sprite)
			{
				// there's a sprite on this cell, check it out!
				int set = getSpriteSet(x, y, *sprite);

				// get the sprite's value
				WorldSystem::CClientSpriteset* sset = mp_spritesets->getSpriteset(set);
				if (sset)
				{
					WorldSystem::SSpriteInfo2* info = &sset->ua_sprites[*sprite & WorldSystem::SPRITE_ID_BITS];
					if (info->ls > 0)
					{
						placeLightOrigin(x, y, (int)info->lr, (int)info->ls);
					}
				}
			}
			sprite++;
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::limitLightAmount()
{
	if (m_lightlock)
	{
		return;
	}

	for (int i(0); i < CLIENTAREA_SIZE; i++)
	{
		if (ma_los[i] > 15)
		{
			ma_los[i] = 15;
		}
		else if (ma_los[i] < 0)
		{
			ma_los[i] = 0;
		}
	}

	// update vertex light
	{
		int* upperleft = &ma_vertex[0];
		for (int y = 0; y < CLIENTAREA_HEIGHT; y++)
		{
			for (int x = 0; x < CLIENTAREA_WIDTH; x++)
			{
				int pos = x + (y * CLIENTAREA_WIDTH);
				if ((m_allvisible) || (ma_los[pos] > 0))
				//if (true)
				{
					upperleft[0] = ma_los[pos];
					upperleft[1] = ma_los[pos];
					upperleft[CLIENTAREA_VERTEX_WIDTH] = ma_los[pos];
					upperleft[CLIENTAREA_VERTEX_WIDTH+1] = ma_los[pos];

					if (x > 0)
					{
						upperleft[0] += ma_los[pos - 1];
						upperleft[CLIENTAREA_VERTEX_WIDTH] += ma_los[pos - 1];

						if (y > 0)
						{
							upperleft[0] += ma_los[pos - 1 - CLIENTAREA_WIDTH];
						}
						if (y < (CLIENTAREA_HEIGHT - 1))
						{
							upperleft[CLIENTAREA_VERTEX_WIDTH] += ma_los[pos - 1 + CLIENTAREA_WIDTH];
						}
					}
					if (x < (CLIENTAREA_WIDTH - 1))
					{
						upperleft[1] += ma_los[pos + 1];
						upperleft[CLIENTAREA_VERTEX_WIDTH+1] += ma_los[pos + 1];

						if (y > 0)
						{
							upperleft[1] += ma_los[pos + 1 - CLIENTAREA_WIDTH];
						}
						if (y < (CLIENTAREA_HEIGHT - 1))
						{
							upperleft[CLIENTAREA_VERTEX_WIDTH+1] += ma_los[pos + 1 + CLIENTAREA_WIDTH];
						}
					}
					if (y > 0)
					{
						upperleft[0] += ma_los[pos - CLIENTAREA_WIDTH];
						upperleft[1] += ma_los[pos - CLIENTAREA_WIDTH];
					}
					if (y < (CLIENTAREA_HEIGHT - 1))
					{
						upperleft[CLIENTAREA_VERTEX_WIDTH] += ma_los[pos + CLIENTAREA_WIDTH];
						upperleft[CLIENTAREA_VERTEX_WIDTH+1] += ma_los[pos + CLIENTAREA_WIDTH];
					}
					upperleft[0] >>= 2;
					upperleft[1] >>= 2;
					upperleft[CLIENTAREA_VERTEX_WIDTH] >>= 2;
					upperleft[CLIENTAREA_VERTEX_WIDTH+1] >>= 2;

					/*
					if (upperleft[0] < 0)
					{
						upperleft[0] = 0;
					}
					if (upperleft[1] < 0)
					{
						upperleft[1] = 0;
					}
					if (upperleft[CLIENTAREA_VERTEX_WIDTH] < 0)
					{
						upperleft[CLIENTAREA_VERTEX_WIDTH] = 0;
					}
					if (upperleft[CLIENTAREA_VERTEX_WIDTH+1] < 0)
					{
						upperleft[CLIENTAREA_VERTEX_WIDTH+1] = 0;
					}
					*/
				}
				else
				{
					upperleft[0] = 0;
					upperleft[1] = 0;
					upperleft[CLIENTAREA_VERTEX_WIDTH] = 0;
					upperleft[CLIENTAREA_VERTEX_WIDTH+1] = 0;
				}
				upperleft += 2;
			}
			upperleft += CLIENTAREA_VERTEX_WIDTH;
		}
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::lockLight()
{
	m_lightlock = true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CClientArea2::m_getAreaPosForClientPos(int clientx, int clienty)
{
	if (clientx < AREA2_WIDTH)
	{
		if (clienty < AREA2_HEIGHT)
		{
			return AP_NORTHWEST;
		}
		else if (clienty >= (AREA2_HEIGHT * 2))
		{
			return AP_SOUTHWEST;
		}
		return AP_WEST;
	}

	if (clientx >= (AREA2_WIDTH * 2))
	{
		if (clienty < AREA2_HEIGHT)
		{
			return AP_NORTHEAST;
		}
		else if (clienty >= (AREA2_HEIGHT * 2))
		{
			return AP_SOUTHEAST;
		}
		return AP_EAST;
	}

	if (clienty < AREA2_HEIGHT)
	{
		return AP_NORTH;
	}
	else if (clienty >= (AREA2_HEIGHT * 2))
	{
		return AP_SOUTH;
	}
	return AP_MAIN;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::drawSpriteBlocking(int x, int y, unsigned char sprite, CClientSpriteset* set)
{
	if (sprite < MIN_SPRITES || sprite >= MAX_SPRITES || !set)
    {
    	return;
    }

    if (set->ua_sprites[sprite].x != INVALID_SPRITE)
    {
    	for (int sy = 0; sy < set->ua_sprites[sprite].height; sy++)
        {
    		for (int sx = 0; sx < set->ua_sprites[sprite].width; sx++)
        	{
            	if (((sx + x) >= 0) &&
                	((sx + x) < CLIENTAREA_WIDTH) &&
                    ((sy + y) >= 0) &&
                    ((sy + y) < CLIENTAREA_HEIGHT))
                {
                	unsigned char block = set->ua_blocking[(sx + set->ua_sprites[sprite].x) + (sy + set->ua_sprites[sprite].y) * CELL_COLUMNS];

                    if (block != BT_BLOCKNONE)
                    {
						if (block == BT_FORCECLEAR)
						{
							m_tileblock.setPos(sx + x, sy + y, 0);
						}
						else
						{
							m_tileblock.setPos(sx + x, sy + y, (Common::TDFourBit)block);
						}
                    }
                }
        	}
        }
    }
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CClientArea2::m_LOSBlocked(int x, int y)
{
	if (x < 0 || y < 0 || x >= CLIENTAREA_WIDTH || y >= CLIENTAREA_HEIGHT)
	{
		return true;
	}

	Common::TDFourBit block = m_tileblock.getPos(x, y);

	return ((block == BT_BLOCKALL) || (block == BT_BLOCKNONETHEREAL));
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_makeVisible(int x, int y)
{
	if (x < 0 || y < 0 || x >= CLIENTAREA_WIDTH || y >= CLIENTAREA_HEIGHT)
	{
		return;
	}

	ma_los[x + (y << 6) + (y << 5)] = m_ambience;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CClientArea2::m_slope(float x1, float y1, float x2, float y2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;

	if(dy != 0)
	{
		return (dx / dy);
	}

	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CClientArea2::m_invSlope(float x1, float y1, float x2, float y2)
{
	float slope = m_slope(x1, y1, x2, y2);

	if(slope != 0)
	{
		return (1 / slope);
	}

	return 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
//	scans the octant covering the area from north west to north from left to 
//	right
//	the method ignores the octants starting and ending cells since they have
//	been applied in FOV::start
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanNW2N(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs = (int)((float)xc + 0.5 - (sslope * distance));
	int xe = (int)((float)xc + 0.5 - (eslope * distance));
	int ycheck = yc - distance;

	// is the starting cell the leftmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc - (1 * distance))
	{
		m_makeVisible(xs, ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev = m_LOSBlocked(xs, ycheck);

	// scan from the cell after the starting cell (xs+1) to end cell of
	// scan (xcheck<=xe)
	for(int xcheck = xs + 1; xcheck <= xe; xcheck++)
	{
		// is the current cell the rightmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_makeVisible(xcheck, ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the left of the blocking cell
		//
		// +---+a####+---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck-0.00001,ycheck+0.99999]
		// |   |#####|   |
		// +---b#####+---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck, ycheck))
		{
			if(!prev)
			{
				m_scanNW2N(xc, yc, distance+1, radius, sslope, m_slope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck - 0.000001f, (float)ycheck + 0.999999f));
			}
			prev = true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the right
		// of the blocking cells
		//
		// #####a---++---+  @ = [xc+0.5,yc+0.5]
		// #####|   ||   |  a = new and old [xcheck,ycheck]
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope = m_slope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck, (float)ycheck);
			}
			prev = false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanNW2N(xc, yc, distance+1, radius, sslope, eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanNE2N(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs=(int)((float)xc + 0.5 - (sslope * distance));
	int xe=(int)((float)xc + 0.5 - (eslope * distance));
	int ycheck=yc - distance;

	// is starting cell the rightmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc-(-1*distance))
	{
		m_makeVisible(xs,ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xs,ycheck);

	// scan from the cell after the starting cell (xs-1) to end cell of
	// scan (xcheck>=xe)
	for(int xcheck=xs-1; xcheck>=xe; xcheck--)
	{
		// is the current cell the leftmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the right of the blocking cell
		//
		// +---+a####+---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck+0.9999,ycheck-0.00001]
		// |   |#####|   |
		// +---+#####b---+
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanNE2N(xc, yc, distance + 1, radius, sslope, m_slope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck + 1, (float)ycheck + 0.99999f));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the left
		// of the blocking cells
		//
		// +---+a---b#####  @ = [xc+0.5,yc+0.5]
		// |   ||   |#####  a = old [xcheck,ycheck]
		// |   ||   |#####  b = new [xcheck+0.99999,ycheck]
		// |   ||   |#####
		// +---++---+#####
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_slope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.9999999,(float)ycheck);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanNE2N(xc, yc, distance + 1, radius, sslope, eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanNW2W(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 - (sslope * distance));
	int ye=(int)((float)yc + 0.5 - (eslope * distance));
	int xcheck=xc - distance;

	// is starting cell the topmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc-(1*distance))
	{
		m_makeVisible(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys+1) to end cell of
	// scan (ycheck<=ye)
	for(int ycheck=ys+1; ycheck<=ye; ycheck++)
	{
		// is the current cell the bottommost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the top of the blocking cell (see fig.)
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   ||   |  b = new [xcheck+0.99999,ycheck-0.00001]
		// |   ||   ||   |
		// +---b+---++---+
		// a####+---++---+
		// #####|   ||   |
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanNW2W(xc, yc, distance + 1, radius, sslope, m_invSlope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck + 0.99999f, (float)ycheck - 0.00001f));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the bottom
		// of the blocking cells
		//
		// #####+---++---+  @ = [xc+0.5,yc+0.5]
		// #####|   ||   |  a = old and new [xcheck,ycheck]
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// a---++---++---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck,(float)ycheck);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanNW2W(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanSW2W(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 - (sslope * distance));
	int ye=(int)((float)yc + 0.5 - (eslope * distance));
	int xcheck=xc - distance;

	// is starting cell the bottommost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc-(-1*distance))
	{
		m_makeVisible(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys-1) to end cell of
	// scan (ycheck>=ye)
	for(int ycheck=ys-1; ycheck>=ye; ycheck--)
	{
		// is the current cell the topmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the bottom of the blocking cell
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   || @ |  b = new [xcheck+0.99999,ycheck+1]
		// |   ||   ||   |
		// +---++---++---+
		// a####+---++---+
		// #####|   ||   |
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// +---b+---++---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanSW2W(xc,yc,distance+1,radius,sslope,m_invSlope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck+0.99999f,(float)ycheck+1));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the top of
		// the blocking cells
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   || @ |  b = new [xcheck,ycheck+0.99999]
		// |   ||   ||   |
		// +---++---++---+
		// a---++---++---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// b---++---++---+
		// #####+---++---+
		// #####|   ||   |
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanSW2W(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanNE2E(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 + (sslope * distance));
	int ye=(int)((float)yc + 0.5 + (eslope * distance));
	int xcheck=xc + distance;

	// is starting cell the topmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc+(-1*distance))
	{
		m_makeVisible(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys+1) to end cell of
	// scan (ycheck<=ye)
	for(int ycheck=ys+1; ycheck<=ye; ycheck++)
	{
		// is the current cell the bottommost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the top of the blocking cell (see fig.)
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   ||   |  b = new [xcheck,ycheck-0.00001]
		// |   ||   ||   |
		// +---++---+b---+
		// +---++---+a####
		// |   ||   |#####
		// |   ||   |#####
		// |   ||   |#####
		// +---++---+#####
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanNE2E(xc,yc,distance+1,radius,sslope,m_invSlope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck,(float)ycheck-0.00001f));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the bottom
		// of the blocking cells
		//
		// +---++---+#####  @ = [xc+0.5,yc+0.5]
		// |   ||   |#####  a = old [xcheck,ycheck]
		// |   ||   |#####  b = new [xcheck+0.99999,ycheck]
		// |   ||   |#####
		// +---++---+#####
		// +---++---+a---b
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.99999,(float)ycheck);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanNE2E(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanSE2E(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 + (sslope * distance));
	int ye=(int)((float)yc + 0.5 + (eslope * distance));
	int xcheck=xc + distance;

	// is starting cell the bottommost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc+(1*distance))
	{
		m_makeVisible(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys-1) to end cell of
	// scan (ycheck>=ye)
	for(int ycheck=ys-1; ycheck>=ye; ycheck--)
	{
		// is the current cell the topmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the bottom of the blocking cell
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// | @ ||   ||   |  b = new [xcheck,ycheck+1]
		// |   ||   ||   |
		// +---++---++---+
		// +---++---+a####
		// |   ||   |#####
		// |   ||   |#####
		// |   ||   |#####
		// +---++---+#####
		// +---++---+b---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanSE2E(xc,yc,distance+1,radius,sslope,m_invSlope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck,(float)ycheck+1));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the top of
		// the blocking cells
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// | @ ||   ||   |  b = new [xcheck+0.99999,ycheck+0.99999]
		// |   ||   ||   |
		// +---++---++---+
		// +---++---+a---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---b
		// +---++---+#####
		// |   ||   |#####
		// |   ||   |#####
		// |   ||   |#####
		// +---++---+#####
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.99999,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanSE2E(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanSW2S(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs=(int)((float)xc + 0.5 + (sslope * distance));
	int xe=(int)((float)xc + 0.5 + (eslope * distance));
	int ycheck=yc + distance;

	// is the starting cell the leftmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc+(-1*distance))
	{
		m_makeVisible(xs,ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xs,ycheck);

	// scan from the cell after the starting cell (xs+1) to end cell of
	// scan (xcheck<=xe)
	for(int xcheck=xs+1; xcheck<=xe; xcheck++)
	{
		// is the current cell the rightmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the left of the blocking cell
		//
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		// +---ba####+---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck-0.00001,ycheck]
		// |   |#####|   |
		// +---+#####+---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanSW2S(xc,yc,distance+1,radius,sslope,m_slope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck-0.00001f,(float)ycheck));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the right
		// of the blocking cells
		//
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		// #####a---++---+  @ = [xc+0.5,yc+0.5]
		// #####|   ||   |  a = old [xcheck,ycheck]
		// #####|   ||   |  b = new [xcheck,ycheck+0.99999]
		// #####|   ||   |
		// #####b---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_slope((float)xc+0.5,(float)yc+0.5,(float)xcheck,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanSW2S(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_scanSE2S(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs=(int)((float)xc + 0.5 + (sslope * distance));
	int xe=(int)((float)xc + 0.5 + (eslope * distance));
	int ycheck=yc + distance;

	// is starting cell the rightmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc+(1*distance))
	{
		m_makeVisible(xs,ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xs,ycheck);

	// scan from the cell after the starting cell (xs-1) to end cell of
	// scan (xcheck>=xe)
	for(int xcheck=xs-1; xcheck>=xe; xcheck--)
	{
		// is the current cell the leftmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_makeVisible(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the right of the blocking cell
		//
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---+a####b---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck+1,ycheck]
		// |   |#####|   |
		// +---+#####+---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_scanSE2S(xc,yc,distance+1,radius,sslope,m_slope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck+1,(float)ycheck));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the left
		// of the blocking cells
		//
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---+a---+#####  @ = [xc+0.5,yc+0.5]
		// |   ||   |#####  a = old [xcheck,ycheck]
		// |   ||   |#####  b = new [xcheck+0.99999,ycheck+0.99999]
		// |   ||   |#####
		// +---++---b#####
		//
		else
		{
			if(prev)
			{
				sslope=m_slope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.99999,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_scanSE2S(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_placeLight(int x, int y)
{
	if (x < 0 || y < 0 || x >= CLIENTAREA_WIDTH || y >= CLIENTAREA_HEIGHT)
	{
		return;
	}

	int pos = x + (y << 6) + (y << 5);
	if (ma_los[pos] < 0)
	{
		// this cell isn't visible
		return;
	}

	// calculate the amount of light based on the light's origin and radius
	float delta = 1.0f - (Common::getCellDistance(x - m_lightx, y - m_lighty) / (float)m_lightradius);
	if (delta <= 0)
	{
		return;
	}
	int d2 = (int)((delta * m_lightstrength) + 0.5f);

	//if (d2 == 0)
	//{
	//	return;
	//}

	// we can safely add the value since the whole area will be limited in
	// the limitLightAmount() function later on
	ma_los[pos] += d2;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightNW2N(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs = (int)((float)xc + 0.5 - (sslope * distance));
	int xe = (int)((float)xc + 0.5 - (eslope * distance));
	int ycheck = yc - distance;

	// is the starting cell the leftmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc - (1 * distance))
	{
		m_placeLight(xs, ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev = m_LOSBlocked(xs, ycheck);

	// scan from the cell after the starting cell (xs+1) to end cell of
	// scan (xcheck<=xe)
	for(int xcheck = xs + 1; xcheck <= xe; xcheck++)
	{
		// is the current cell the rightmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_placeLight(xcheck, ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the left of the blocking cell
		//
		// +---+a####+---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck-0.00001,ycheck+0.99999]
		// |   |#####|   |
		// +---b#####+---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck, ycheck))
		{
			if(!prev)
			{
				m_lightNW2N(xc, yc, distance+1, radius, sslope, m_slope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck - 0.000001f, (float)ycheck + 0.999999f));
			}
			prev = true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the right
		// of the blocking cells
		//
		// #####a---++---+  @ = [xc+0.5,yc+0.5]
		// #####|   ||   |  a = new and old [xcheck,ycheck]
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope = m_slope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck, (float)ycheck);
			}
			prev = false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightNW2N(xc, yc, distance+1, radius, sslope, eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightNE2N(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs=(int)((float)xc + 0.5 - (sslope * distance));
	int xe=(int)((float)xc + 0.5 - (eslope * distance));
	int ycheck=yc - distance;

	// is starting cell the rightmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc-(-1*distance))
	{
		m_placeLight(xs,ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xs,ycheck);

	// scan from the cell after the starting cell (xs-1) to end cell of
	// scan (xcheck>=xe)
	for(int xcheck=xs-1; xcheck>=xe; xcheck--)
	{
		// is the current cell the leftmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the right of the blocking cell
		//
		// +---+a####+---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck+0.9999,ycheck-0.00001]
		// |   |#####|   |
		// +---+#####b---+
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightNE2N(xc, yc, distance + 1, radius, sslope, m_slope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck + 1, (float)ycheck + 0.99999f));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the left
		// of the blocking cells
		//
		// +---+a---b#####  @ = [xc+0.5,yc+0.5]
		// |   ||   |#####  a = old [xcheck,ycheck]
		// |   ||   |#####  b = new [xcheck+0.99999,ycheck]
		// |   ||   |#####
		// +---++---+#####
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_slope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.9999999,(float)ycheck);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightNE2N(xc, yc, distance + 1, radius, sslope, eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightNW2W(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 - (sslope * distance));
	int ye=(int)((float)yc + 0.5 - (eslope * distance));
	int xcheck=xc - distance;

	// is starting cell the topmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc-(1*distance))
	{
		m_placeLight(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys+1) to end cell of
	// scan (ycheck<=ye)
	for(int ycheck=ys+1; ycheck<=ye; ycheck++)
	{
		// is the current cell the bottommost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the top of the blocking cell (see fig.)
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   ||   |  b = new [xcheck+0.99999,ycheck-0.00001]
		// |   ||   ||   |
		// +---b+---++---+
		// a####+---++---+
		// #####|   ||   |
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightNW2W(xc, yc, distance + 1, radius, sslope, m_invSlope((float)xc + 0.5f, (float)yc + 0.5f, (float)xcheck + 0.99999f, (float)ycheck - 0.00001f));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the bottom
		// of the blocking cells
		//
		// #####+---++---+  @ = [xc+0.5,yc+0.5]
		// #####|   ||   |  a = old and new [xcheck,ycheck]
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// a---++---++---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck,(float)ycheck);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightNW2W(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightSW2W(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 - (sslope * distance));
	int ye=(int)((float)yc + 0.5 - (eslope * distance));
	int xcheck=xc - distance;

	// is starting cell the bottommost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc-(-1*distance))
	{
		m_placeLight(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys-1) to end cell of
	// scan (ycheck>=ye)
	for(int ycheck=ys-1; ycheck>=ye; ycheck--)
	{
		// is the current cell the topmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the bottom of the blocking cell
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   || @ |  b = new [xcheck+0.99999,ycheck+1]
		// |   ||   ||   |
		// +---++---++---+
		// a####+---++---+
		// #####|   ||   |
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		// +---b+---++---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightSW2W(xc,yc,distance+1,radius,sslope,m_invSlope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck+0.99999f,(float)ycheck+1));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the top of
		// the blocking cells
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   || @ |  b = new [xcheck,ycheck+0.99999]
		// |   ||   ||   |
		// +---++---++---+
		// a---++---++---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// b---++---++---+
		// #####+---++---+
		// #####|   ||   |
		// #####|   ||   |
		// #####|   ||   |
		// #####+---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightSW2W(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightNE2E(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 + (sslope * distance));
	int ye=(int)((float)yc + 0.5 + (eslope * distance));
	int xcheck=xc + distance;

	// is starting cell the topmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc+(-1*distance))
	{
		m_placeLight(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys+1) to end cell of
	// scan (ycheck<=ye)
	for(int ycheck=ys+1; ycheck<=ye; ycheck++)
	{
		// is the current cell the bottommost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the top of the blocking cell (see fig.)
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// |   ||   ||   |  b = new [xcheck,ycheck-0.00001]
		// |   ||   ||   |
		// +---++---+b---+
		// +---++---+a####
		// |   ||   |#####
		// |   ||   |#####
		// |   ||   |#####
		// +---++---+#####
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightNE2E(xc,yc,distance+1,radius,sslope,m_invSlope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck,(float)ycheck-0.00001f));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the bottom
		// of the blocking cells
		//
		// +---++---+#####  @ = [xc+0.5,yc+0.5]
		// |   ||   |#####  a = old [xcheck,ycheck]
		// |   ||   |#####  b = new [xcheck+0.99999,ycheck]
		// |   ||   |#####
		// +---++---+#####
		// +---++---+a---b
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.99999,(float)ycheck);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightNE2E(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightSE2E(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int ys=(int)((float)yc + 0.5 + (sslope * distance));
	int ye=(int)((float)yc + 0.5 + (eslope * distance));
	int xcheck=xc + distance;

	// is starting cell the bottommost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(ys != yc+(1*distance))
	{
		m_placeLight(xcheck,ys);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xcheck,ys);

	// scan from the cell after the starting cell (ys-1) to end cell of
	// scan (ycheck>=ye)
	for(int ycheck=ys-1; ycheck>=ye; ycheck--)
	{
		// is the current cell the topmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(ycheck != yc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by the bottom of the blocking cell
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// | @ ||   ||   |  b = new [xcheck,ycheck+1]
		// |   ||   ||   |
		// +---++---++---+
		// +---++---+a####
		// |   ||   |#####
		// |   ||   |#####
		// |   ||   |#####
		// +---++---+#####
		// +---++---+b---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightSE2E(xc,yc,distance+1,radius,sslope,m_invSlope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck,(float)ycheck+1));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by the top of
		// the blocking cells
		//
		// +---++---++---+  @ = [xc+0.5,yc+0.5]
		// |   ||   ||   |  a = old [xcheck,ycheck]
		// | @ ||   ||   |  b = new [xcheck+0.99999,ycheck+0.99999]
		// |   ||   ||   |
		// +---++---++---+
		// +---++---+a---+
		// |   ||   ||   |
		// |   ||   ||   |
		// |   ||   ||   |
		// +---++---++---b
		// +---++---+#####
		// |   ||   |#####
		// |   ||   |#####
		// |   ||   |#####
		// +---++---+#####
		//
		else
		{
			if(prev)
			{
				sslope=m_invSlope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.99999,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightSE2E(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightSW2S(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs=(int)((float)xc + 0.5 + (sslope * distance));
	int xe=(int)((float)xc + 0.5 + (eslope * distance));
	int ycheck=yc + distance;

	// is the starting cell the leftmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc+(-1*distance))
	{
		m_placeLight(xs,ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev=m_LOSBlocked(xs,ycheck);

	// scan from the cell after the starting cell (xs+1) to end cell of
	// scan (xcheck<=xe)
	for(int xcheck=xs+1; xcheck<=xe; xcheck++)
	{
		// is the current cell the rightmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the left of the blocking cell
		//
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		// +---ba####+---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck-0.00001,ycheck]
		// |   |#####|   |
		// +---+#####+---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightSW2S(xc,yc,distance+1,radius,sslope,m_slope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck-0.00001f,(float)ycheck));
			}
			prev=true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the right
		// of the blocking cells
		//
		// +---++---++---+
		// |   ||   ||   |
		// |   ||   || @ |
		// |   ||   ||   |
		// +---++---++---+
		// #####a---++---+  @ = [xc+0.5,yc+0.5]
		// #####|   ||   |  a = old [xcheck,ycheck]
		// #####|   ||   |  b = new [xcheck,ycheck+0.99999]
		// #####|   ||   |
		// #####b---++---+
		//
		else
		{
			if(prev)
			{
				sslope=m_slope((float)xc+0.5,(float)yc+0.5,(float)xcheck,(float)ycheck+0.99999);
			}
			prev=false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightSW2S(xc,yc,distance+1,radius,sslope,eslope);
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CClientArea2::m_lightSE2S(int xc, int yc, int distance, int radius, float sslope, float eslope)
{
	if(distance > radius)
	{
		return;
	}

	// calculate start and end cell of the scan
	int xs = (int)((float)xc + 0.5 + (sslope * distance));
	int xe = (int)((float)xc + 0.5 + (eslope * distance));
	int ycheck = yc + distance;

	// is starting cell the rightmost cell in the octant?
	// NO: call applyCell() to starting cell 
	// YES: it has already been applied in FOV::start()
	if(xs != xc+(1*distance))
	{
		m_placeLight(xs, ycheck);
	}

	// find out if starting cell blocks LOS
	bool prev = m_LOSBlocked(xs,ycheck);

	// scan from the cell after the starting cell (xs-1) to end cell of
	// scan (xcheck>=xe)
	for(int xcheck= xs - 1; xcheck >= xe; xcheck--)
	{
		// is the current cell the leftmost cell in the octant?
		// NO: call applyCell() to current cell 
		// YES: it has already been applied in FOV::start()
		if(xcheck != xc)
		{
			// apply cell
			m_placeLight(xcheck,ycheck);
		}
		
		// cell blocks LOS
		// if previous cell didn't block LOS (prev==false) we have
		// hit a 'new' section of walls. a new scan will be started with an
		// eslope that 'brushes' by to the right of the blocking cell
		//
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---+a####b---+  @ = [xc+0.5,yc+0.5]
		// |   |#####|   |  a = old [xcheck,ycheck]
		// |   |#####|   |  b = new [xcheck+1,ycheck]
		// |   |#####|   |
		// +---+#####+---+
		//
		if(m_LOSBlocked(xcheck,ycheck))
		{
			if(!prev)
			{
				m_lightSE2S(xc,yc,distance+1,radius,sslope,m_slope((float)xc+0.5f,(float)yc+0.5f,(float)xcheck+1,(float)ycheck));
			}
			prev = true;
		}

		// cell doesn't block LOS
		// if the cell is the first non-blocking cell after a section of walls
		// we need to calculate a new sslope that 'brushes' by to the left
		// of the blocking cells
		//
		// +---++---++---+
		// |   ||   ||   |
		// | @ ||   ||   |
		// |   ||   ||   |
		// +---++---++---+
		// +---+a---+#####  @ = [xc+0.5,yc+0.5]
		// |   ||   |#####  a = old [xcheck,ycheck]
		// |   ||   |#####  b = new [xcheck+0.99999,ycheck+0.99999]
		// |   ||   |#####
		// +---++---b#####
		//
		else
		{
			if(prev)
			{
				sslope = m_slope((float)xc+0.5,(float)yc+0.5,(float)xcheck+0.99999,(float)ycheck+0.99999);
			}
			prev = false;
		}
	}

	// if the last cell of the scan didn't block LOS a new scan should be
	// started
	if(!prev)
	{
		m_lightSE2S(xc, yc, distance + 1, radius, sslope, eslope);
	}
}

}
