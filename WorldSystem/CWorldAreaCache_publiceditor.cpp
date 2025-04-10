
// =============================================================================
// THIS FILE IS
// a cut-down version of the real area. It doesn't include the encryption
// functions to prevent people from hacking the real game's sector files.
// =============================================================================

#include "CWorldAreaCache.h"
#include <stdlib.h>

namespace WorldSystem
{

// *********************************************************************************************
// *********************************************************************************************
// *********************************************************************************************
// *********************************************************************************************
// *********************************************************************************************
// *********************************************************************************************

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWorldArea2Cache::CWorldArea2Cache(FileSystem::IFileManager* manager, char* filename)
{
	m_clear();

	m_filename = filename;
	mp_manager = manager;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CWorldArea2Cache::~CWorldArea2Cache()
{
	removeAll();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWorldArea2Cache::m_clear()
{
	mp_manager = NULL;
	m_filename = "";
	m_isok = 0;
	mpa_areas = NULL;
	m_areatype = AT_NORMAL;

	for (int i(0); i < SECTOR2_SIZE; i++)
	{
		ma_offsets[i] = 0;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWorldArea2Cache::removeAll()
{
	if (m_isok && mp_manager)
	{
		mp_manager->close(m_filestream);
	}

	if (mpa_areas)
	{
		for (int i(0); i < SECTOR2_SIZE; i++)
		{
			delete mpa_areas[i];
		}
	}
	delete [] mpa_areas;

	m_clear();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::openFile()
{
	int version = 0;
	if (!mp_manager)
	{
    	m_isok = 0;
		return false;
	}

	// attempt to open the file
	m_filestream = mp_manager->open(m_filename);

	if (!m_filestream.isOk())
	{
		// the file didn't exist, so we'll attempt to create it
		m_filestream = mp_manager->openWrite(m_filename);

		if (!m_filestream.isOk())
		{
        	m_isok = 0;
			return false;
		}

		version = WCFV_CURRENT;
		m_filestream.write(&version, sizeof(int), 1);
		m_filestream.write(&m_areatype, sizeof(int), 1);

		// write our offsets (all should be zero at this time)
		m_filestream.write(ma_offsets, sizeof(unsigned int), SECTOR2_SIZE);

		// close the file and re-open it as readonly
		mp_manager->close(m_filestream);
		m_filestream = mp_manager->open(m_filename);

		if (!m_filestream.isOk())
		{
			// we give up
			m_isok = 0;
			return false;
		}
	}

	version = 0;
	m_filestream.read(&version, sizeof(int), 1);

	if (version > WCFV_030125)
	{
		m_filestream.read(&m_areatype, sizeof(int), 1);
	}

	// read the file offsets
	m_filestream.read(ma_offsets, sizeof(unsigned int), SECTOR2_SIZE);
    
    m_isok = 1;
	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CWorldArea2Cache::closeFile()
{
	if (mp_manager)
	{
		mp_manager->close(m_filestream);
	}

	m_isok = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CWorldArea2Cache::fileIsOk()
{
	return m_isok;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::areaExists(int x, int y)
{
	if ((x < 0) || (x >= SECTOR2_WIDTH) || (y < 0) || (y >= SECTOR2_HEIGHT))
	{
		return false;
	}

	if (mpa_areas)
	{
		return (mpa_areas[x + y * SECTOR2_WIDTH] != NULL);
	}

	if (ma_offsets[x + y * SECTOR2_WIDTH] != 0)
	{
		return true;
	}

	return false;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// returns -1 if the area doesn't exist
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CWorldArea2Cache::getAreaVersion(int x, int y)
{
	if ((x < 0) || (x >= SECTOR2_WIDTH) || (y < 0) || (y >= SECTOR2_HEIGHT))
	{
		return -1;
	}

	if (mpa_areas)
	{
		if (mpa_areas[x + y * SECTOR2_WIDTH] != NULL)
		{
			return (int)mpa_areas[x + y * SECTOR2_WIDTH]->getVersion();
		}
		return -1;
	}

	int offset = ma_offsets[x + y * SECTOR2_WIDTH];

	if (!offset || !m_isok)
	{
		return -1;
	}

	// place our file pointer at the beginning of the area
	m_filestream.seekStart(offset);

	// read the area
	CArea2Static area;
	if (!area.read(m_filestream, false))
	{
		return -1;
	}

	// return area version number
	return (int)area.getVersion();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::getArea(int x, int y, CArea2Static* dest)
{
	if ((x < 0) || (x >= SECTOR2_WIDTH) || (y < 0) || (y >= SECTOR2_HEIGHT))
	{
		return false;
	}

	if (mpa_areas)
	{
		if (mpa_areas[x + y * SECTOR2_WIDTH] != NULL)
		{
			return dest->copyArea(mpa_areas[x + y * SECTOR2_WIDTH]);
		}
		return false;
	}

	int offset = ma_offsets[x + y * SECTOR2_WIDTH];

	if (!offset || !m_isok)
	{
		return false;
	}

	// place our file pointer at the beginning of the area
	m_filestream.seekStart(offset);

	// read the area and return the results
	return dest->read(m_filestream, false);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This function creates all areas so we can save the file. It is only meant to be used
// when you need to be able to save the file later, such as in editors. Both the server
// and client only needs to load areas on request.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::createAreas()
{
	// delete old areas if available
	if (mpa_areas)
	{
		for (int i(0); i < SECTOR2_SIZE; i++)
		{
			delete mpa_areas[i];
		}
	}
	delete [] mpa_areas;
	mpa_areas = NULL;

	if (!fileIsOk())
	{
		if (!openFile())
		{
			return false;
		}
	}

	// create new, but since the areaExists() and getArea() functions wants to use the
	// mpa_areas pointer in first case, we should keep a temporary pointer for now
	CArea2Static** temp = new CArea2Static* [SECTOR2_SIZE];

	// load all areas
	{
		for (int j(0); j < SECTOR2_HEIGHT; j++)
		{
			for (int i(0); i < SECTOR2_WIDTH; i++)
			{
				if (areaExists(i, j))
				{
					temp[i + (j * SECTOR2_WIDTH)] = new CArea2Static;
					getArea(i, j, temp[i + (j * SECTOR2_WIDTH)]);
				}
				else
				{
					temp[i + (j * SECTOR2_WIDTH)] = NULL;
				}
			}
		}
	}

	// get the pointer
	mpa_areas = temp;

	// we no longer need to keep the file open
	closeFile();

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// 030125: This function only works if createAreas() has been called
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::updateArea(int x, int y, CArea2Static* source)
{
	// our new strategy is to use the pre-loaded data files to create a huge memory buffer
	// and overwrite our current file
	if ((x < 0) || (x >= SECTOR2_WIDTH) || (y < 0) || (y >= SECTOR2_HEIGHT))
	{
		return false;
	}
	// NOTE: m_isok must be false because we do not want to keep the area file open
	if (!mp_manager || m_isok || !mpa_areas)
	{
		return false;
	}

	// create our file as writeable and store our file version
	m_filestream = mp_manager->openWrite((const char*)m_filename.getString());
	if (!m_filestream.isOk())
	{
		return false;
	}

	int version = WCFV_CURRENT;
	m_filestream.write(&version, sizeof(int), 1);
	m_filestream.write(&m_areatype, sizeof(int), 1);
	int startheadersize = (sizeof(int) * 2);
	
	// create the offset header
	// set the starting offset to point at the place after the 'ma_offsets' header
	unsigned int offset((sizeof(unsigned int) * SECTOR2_SIZE) + startheadersize);

	{
		for (int j(0); j < SECTOR2_HEIGHT; j++)
		{
			for (int i(0); i < SECTOR2_WIDTH; i++)
			{
				if ((i == x) && (j == y))
				{
					// this is the area we want to add to the cache
					if (!areaExists(i, j))
					{
						mpa_areas[i + j * SECTOR2_WIDTH] = new CArea2Static;
					}
					// update the area from our source
					mpa_areas[i + j * SECTOR2_WIDTH]->copyArea(source);

					ma_offsets[i + j * SECTOR2_WIDTH] = offset;
					offset += mpa_areas[i + j * SECTOR2_WIDTH]->createSaveData(m_areatype);
				}
				else if (areaExists(i, j))
				{
					ma_offsets[i + j * SECTOR2_WIDTH] = offset;
					offset += mpa_areas[i + j * SECTOR2_WIDTH]->createSaveData(m_areatype);
				}
				else
				{
					ma_offsets[i + j * SECTOR2_WIDTH] = 0;
				}
			}
		}
	}

	// write header
	m_filestream.write(ma_offsets, sizeof(unsigned int), SECTOR2_SIZE);

	// write all areas
	{
		for (int j(0); j < SECTOR2_HEIGHT; j++)
		{
			for (int i(0); i < SECTOR2_WIDTH; i++)
			{
				if (mpa_areas[i + j * SECTOR2_WIDTH] != NULL)
				{
					if (!mpa_areas[i + j * SECTOR2_WIDTH]->write(m_filestream))
					{
						return false;
					}
				}
			}
		}
	}
	mp_manager->close(m_filestream);

	return true;

	/*
	if ((x < 0) || (x >= SECTOR2_WIDTH) || (y < 0) || (y >= SECTOR2_HEIGHT))
	{
		return false;
	}
	if (!mp_manager || !m_isok)
	{
		return false;
	}

	// the stratgy we're using here is to go through the current cache and
	// look for available areas. When an area is found, we write it to
	// another, temporary cache. Unfortunately you must write the file in
	// correct order, which means you can't alter the header when something
	// else has been written. To get around this we'll compute the file
	// offsets before they even exist.

	// temporary offset header:
	unsigned int offsets[SECTOR2_SIZE];
	// set the starting offset to point at the place after the 'ma_offsets' header
	unsigned int offset(sizeof(unsigned int) * SECTOR2_SIZE);

	{
		for (int j(0); j < SECTOR2_HEIGHT; j++)
		{
			for (int i(0); i < SECTOR2_WIDTH; i++)
			{
				if ((i == x) && (j == y))
				{
					// this is the area we want to add to the cache
					offsets[i + j * SECTOR2_WIDTH] = offset;
					offset += sizeof(CArea2Static);
					//offset += 
				}
				else if (areaExists(i, j))
				{
					offsets[i + j * SECTOR2_WIDTH] = offset;
					offset += sizeof(CArea2Static);
				}
				else
				{
					offsets[i + j * SECTOR2_WIDTH] = 0;
				}
			}
		}
	}

	// now we want to create the temporary cache file
	Basic::CCharString name("~t");
	name += (int)(rand()%12000);
	name += ".tmp";

	// open the file
	FileSystem::CFileStream file = mp_manager->openWrite(name);

	if (!file.isOk())
	{
		return false;
	}

	// write header
	file.write(offsets, sizeof(unsigned int), SECTOR2_SIZE);

	// now copy all areas to the new cache
	{
		for (int j(0); j < SECTOR2_HEIGHT; j++)
		{
			for (int i(0); i < SECTOR2_WIDTH; i++)
			{
				if ((i == x) && (j == y))
				{
					// this is the area we want to add to the cache
					source->write(file);
				}
				else if (areaExists(i, j))
				{
					// copy this area from the original cache
					CArea2Static area;
					getArea(i, j, &area);
					area.write(file);
				}
			}
		}
	}

	// now the new cache has all the areas, so close it
	mp_manager->close(file);

	// and finally, the tricky part... replace our old cache
	// with the new one by removing the old and renaming the new
	mp_manager->close(m_filestream);
	mp_manager->deleteFile((const char*)m_filename.getString());
	mp_manager->renameFile(name, m_filename);

    // to maintain the file status we need to reload the file
    return openFile();
	*/

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// this function will convert all areas to world style (16 tile types)
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::convertToWorldSector()
{
	// 0  - Water (animated, no edges)
	// 1  - Slime (animated, no edges)
	// 2  - 
	// 3  - Sand
	// 4  - Dirt
	// 5  - Dark/Rocky Dirt
	// 6  - Swampy Ground
	// 7  - Grass
	// 8  - Dark Grass
	// 9  - 
	// 10 - Roadstones
	// 11 - Ice
	// 12 - Snow
	// 13 - 
	// 14 - Mountain Rocks
	// 15 - Blackness (nothing)
	const unsigned char NEWTILEVALUES[256] =
		{
			7,	7,	7,	7,		8,	8,	8,	8,		7,	7,	7,	7,		7,	7,	7,	7,
			7,	7,	7,	7,		7,	7,	7,	7,		7,	7,	7,	7,		7,	7,	7,	7,
			4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,
			4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,

			10,	10,	10,	10,		10,	10,	10,	10,		4,	4,	4,	4,		4,	4,	4,	4,
			4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,
			4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,
			4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,		4,	4,	4,	4,
		
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
		
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
			0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,
			7,	7,	7,	7,		7,	7,	7,	7,		7,	7,	7,	7,		7,	7,	7,	15
		};

	// make sure the areas are loaded
	//createAreas();

	for (int y = 0; y < SECTOR2_HEIGHT; y++)
	{
		for (int x = 0; x < SECTOR2_WIDTH; x++)
		{
			// work directly on the pointers instead of loading area files
			if (mpa_areas[x + (y * SECTOR2_WIDTH)] == NULL)
			{
				continue;
			}

			CArea2Static* area = mpa_areas[x + (y * SECTOR2_WIDTH)];
			unsigned char* source = area->getTilesReadOnly();

			// convert tiles according to the old worldarea tileset
			for (int i(0); i < AREA2_SIZE; i++)
			{
				// "i & 31" is the same as "i % 32"
				// "i >> 5" is the same as "i / 32"
				area->setTile(i & 31, i >> 5, NEWTILEVALUES[source[i]]);
			}
		}
	}
	// change area type
	m_areatype = AT_WORLD;

	return true;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
bool CWorldArea2Cache::isWorldSector()
{
	return ((m_areatype == AT_WORLDCOMPRESSED) || (m_areatype == AT_WORLD));
}


}
