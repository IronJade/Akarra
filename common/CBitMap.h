/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A single-bit array class.
 **/

#ifndef _CBITMAP_H_
#define _CBITMAP_H_

// prototype
namespace FileSystem
{
	class CFileStream;
}

namespace Common
{

	// the four bit map keeps an unsigned short for every four columns in the map,
	// so you need to use these to filter out the value you're after
	const unsigned short	BIT4_COLUMNFLAGS[4] = { 0xF000, 0x0F00, 0x00F0, 0x000F };
	const unsigned int		BIT4_COLUMNSHIFTS[4] = { 12, 8, 4, 0 };

	const unsigned char		BIT2_COLUMNFLAGS[4] = { 0xC0, 0xC0, 0x0F, 0x03 };
	const unsigned int		BIT2_COLUMNSHIFTS[4] = { 6, 4, 2, 0 };

	// the one-bit map is special because you can have 8 units in every object (8 bits of a char)
	
	const unsigned char		BIT1_COLUMNFLAGS[8] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };
	const unsigned char		BIT1_COLUMNSHIFTS[8] = { 7, 6, 5, 4, 3, 2, 1, 0 };

	typedef unsigned short	TDFourBit;
	typedef unsigned char	TDTwoBit;
	typedef unsigned char	TDOneBit;

	class CFourBitMap
	{
		public:

			CFourBitMap();
			~CFourBitMap();

			bool create(int width, int height);
			void removeAll();

			bool setPos(int x, int y, TDFourBit value);
			TDFourBit getPos(int x, int y);

			void reset();
			void fill(TDFourBit value);
	
		private:

			void m_clear();
	
		protected:

			int			m_width;			// width of map
			int			m_physicalwidth;	// "real" width (four times less than m_width)
			int			m_height;			// height of map

			TDFourBit*	ma_map;
	
	};

	class CTwoBitMap
	{
		public:

			CTwoBitMap();
			~CTwoBitMap();

			bool create(int width, int height);
			void removeAll();

			void setPos(int x, int y, TDTwoBit value);
			TDTwoBit getPos(int x, int y);

			void reset();
	
		private:

			void m_clear();
	
		protected:

			int			m_width;			// width of map
			int			m_physicalwidth;	// "real" width (four times less than m_width)
			int			m_height;			// height of map

			TDTwoBit*	ma_map;
	
	};

	class COneBitMap
	{
		public:

			COneBitMap();
			~COneBitMap();

			bool create(int width, int height);
			void removeAll();

			void setPos(int x, int y, TDOneBit value);
			TDOneBit getPos(int x, int y);

			void reset();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
	
		private:

			void m_clear();
	
		protected:

			int			m_width;			// width of map
			int			m_physicalwidth;	// "real" width (four times less than m_width)
			int			m_height;			// height of map

			TDOneBit*	ma_map;
	
	};

} // end of Common namespace


#endif
