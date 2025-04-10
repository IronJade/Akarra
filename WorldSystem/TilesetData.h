/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Tileset definitions.
 **/

#ifndef _TILESETDATA_H_
#define _TILESETDATA_H_


namespace WorldSystem
{

	int getCellTilebase(int x, int y);
	int getTypeTilebase(int type);
	int getTileHandle(int type, int x, int y);
	int getBlockType(int tiletype);

}

#endif
