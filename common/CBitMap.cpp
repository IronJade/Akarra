/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A single-bit array class.
 **/

#include "CBitMap.h"
#include "../Library/FileSystem/CFileStream.h"

namespace Common
{

CFourBitMap::CFourBitMap()
{
	m_clear();
}

CFourBitMap::~CFourBitMap()
{
	removeAll();
}

bool CFourBitMap::create(int width, int height)
{
	if (width < 4 || height < 1)
	{
		return false;
	}

	removeAll(); // to be on the safe side

	m_width = width;
	m_height = height;
	m_physicalwidth = (m_width >> 2);	// you can put 4 objects into each unsigned short

	// create the memory
	ma_map = new TDFourBit[m_physicalwidth * m_height];

	if (!ma_map)
	{
		return false;
	}

	reset();

	return true;
}

void CFourBitMap::removeAll()
{
	delete [] ma_map;

	m_clear();
}

bool CFourBitMap::setPos(int x, int y, TDFourBit value)
{
	if (!ma_map || (x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
	{
		return false;
	}

	// make sure the value is only four bits
	value &= 0x000F;
	
	// find the unsigned short for our position
	TDFourBit* pos = &ma_map[(x>>2) + (y * m_physicalwidth)];

	// clear the column
	*pos &= ~BIT4_COLUMNFLAGS[x & 0x3];

	// insert our new value in the same column
	*pos |= value << BIT4_COLUMNSHIFTS[x & 0x3];

	return true;
}

TDFourBit CFourBitMap::getPos(int x, int y)
{
	if (!ma_map || (x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
	{
		return 0;
	}

	// find the unsigned short for our position
	TDFourBit* pos = &ma_map[(x>>2) + (y * m_physicalwidth)];

	// get the value
	TDFourBit value = (*pos & BIT4_COLUMNFLAGS[x & 0x3]) >> BIT4_COLUMNSHIFTS[x & 0x3];

	return value;
}

void CFourBitMap::reset()
{
	if (ma_map)
	{
		for (int i(0); i < (m_physicalwidth * m_height); i++)
		{
			ma_map[i] = 0;
		}
	}
}

void CFourBitMap::fill(TDFourBit value)
{
	if (!ma_map)
	{
		return;
	}

	// make sure it's only four bits
	value &= 0x000F;
	// create a quad packet
	value |= (value << 12) | (value << 8) | (value << 4);

	for (int i(0); i < (m_physicalwidth * m_height); i++)
	{
		ma_map[i] = value;
	}
}

void CFourBitMap::m_clear()
{
	ma_map = 0;
	m_width = 0;
	m_physicalwidth = 0;
	m_height = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

CTwoBitMap::CTwoBitMap()
{
	m_clear();
}

CTwoBitMap::~CTwoBitMap()
{
	removeAll();
}

bool CTwoBitMap::create(int width, int height)
{
	if (width < 4 || height < 1)
	{
		return false;
	}

	removeAll(); // to be on the safe side

	m_width = width;
	m_height = height;
	m_physicalwidth = (m_width >> 2);	// you can put 4 objects into each unsigned char

	// create the memory
	ma_map = new TDTwoBit[m_physicalwidth * m_height];

	if (!ma_map)
	{
		return false;
	}

	reset();

	return true;
}

void CTwoBitMap::removeAll()
{
	delete [] ma_map;

	m_clear();
}

void CTwoBitMap::setPos(int x, int y, TDTwoBit value)
{
	if (!ma_map || (x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
	{
		return;
	}

	// make sure the value is only two bits
	value &= 0x03;
	
	// find the unsigned char for our position
	TDTwoBit* pos = &ma_map[(x>>2) + (y * m_physicalwidth)];

	// clear the column
	*pos &= ~BIT2_COLUMNFLAGS[x & 0x3];

	// insert our new value in the same column
	*pos |= value << BIT2_COLUMNSHIFTS[x & 0x3];
}

TDTwoBit CTwoBitMap::getPos(int x, int y)
{
	if (!ma_map || (x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
	{
		return 0;
	}

	// find the unsigned char for our position
	TDTwoBit* pos = &ma_map[(x>>2) + (y * m_physicalwidth)];

	// get the value
	TDTwoBit value = (*pos & BIT2_COLUMNFLAGS[x & 0x3]) >> BIT2_COLUMNSHIFTS[x & 0x3];

	return value;
}

void CTwoBitMap::reset()
{
	if (ma_map)
	{
		for (int i(0); i < (m_physicalwidth * m_height); i++)
		{
			ma_map[i] = 0;
		}
	}
}

void CTwoBitMap::m_clear()
{
	ma_map = 0;
	m_width = 0;
	m_physicalwidth = 0;
	m_height = 0;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

COneBitMap::COneBitMap()
{
	m_clear();
}

COneBitMap::~COneBitMap()
{
	removeAll();
}

bool COneBitMap::create(int width, int height)
{
	if (width < 8 || height < 1)
	{
		return false;
	}

	removeAll(); // to be on the safe side

	m_width = width;
	m_height = height;
	m_physicalwidth = (m_width >> 3);	// you can put 8 objects into each unsigned char

	// create the memory
	ma_map = new TDOneBit[m_physicalwidth * m_height];

	if (!ma_map)
	{
		return false;
	}

	reset();

	return true;
}

void COneBitMap::removeAll()
{
	delete [] ma_map;

	m_clear();
}

void COneBitMap::setPos(int x, int y, TDTwoBit value)
{
	if (!ma_map || (x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
	{
		return;
	}

	// make sure the value is only one bit
	value &= 0x01;
	
	// find the unsigned char for our position
	TDOneBit* pos = &ma_map[(x>>3) + (y * m_physicalwidth)];

	// clear the column
	*pos &= ~BIT1_COLUMNFLAGS[x & 0x7];

	// insert our new value in the same column
	*pos |= value << BIT1_COLUMNSHIFTS[x & 0x7];
}

TDTwoBit COneBitMap::getPos(int x, int y)
{
	if (!ma_map || (x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
	{
		return 1;
	}

	// find the unsigned char for our position
	TDTwoBit* pos = &ma_map[(x>>3) + (y * m_physicalwidth)];

	// get the value
	TDTwoBit value = (*pos & BIT1_COLUMNFLAGS[x & 0x7]) >> BIT1_COLUMNSHIFTS[x & 0x7];

	return value;
}

void COneBitMap::reset()
{
	if (ma_map)
	{
		for (int i(0); i < (m_physicalwidth * m_height); i++)
		{
			ma_map[i] = 0;
		}
	}
}

bool COneBitMap::read(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	// read size of bitmap
	int width, height;
	file.read(&width, sizeof(int), 1);
	file.read(&height, sizeof(int), 1);

	if (!create(width, height))
	{
		return false;
	}

	// read data
	file.read(ma_map, sizeof(TDOneBit), m_physicalwidth * m_height);

	return true;
}

bool COneBitMap::write(FileSystem::CFileStream& file)
{
	if (!file.isOk())
	{
		return false;
	}

	if (!ma_map)
	{
		return false;
	}

	// write size of bitmap
	file.write(&m_width, sizeof(int), 1);
	file.write(&m_height, sizeof(int), 1);

	// read data
	file.write(ma_map, sizeof(TDOneBit), m_physicalwidth * m_height);

	return true;
}

void COneBitMap::m_clear()
{
	ma_map = 0;
	m_width = 0;
	m_physicalwidth = 0;
	m_height = 0;
}

} // end of Common namespace
