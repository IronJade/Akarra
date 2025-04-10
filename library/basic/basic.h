/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Simple helper functions.
 **/

#ifndef BASIC_H
#define BASIC_H

#include "handle.h"
#include "CPoint.h"
#include "CRect.h"

namespace Basic
{
	class CCharString;
	class CWideString;
	class CPoint;
	class CRect;
}

namespace Basic
{
	const double PI = 3.14159265358979323846;

	const double PI_inverted = 0.31830988618379067153; //      1 / PI

	const double PI_radians = 0.01745329251994329576;

	typedef CPoint CCellPos;
	typedef CPoint CScreenPos;

	extern int directionTableX[]; //  = { 1,  1,  0, -1, -1, -1, 0, 1 };
	extern int directionTableY[]; //  = { 0, -1, -1, -1,  0,  1, 1, 1 };

	template <class T> void swap(T &a, T &b);

	int round(double d);
	int round(float f);

	CCharString getTimeString();
	CCharString getTimeString(long time);

	const int MAX_FILENAME_LENGTH	= 256;
	const int MAX_EXTENSION_LENGTH	= 6;
	const int MAX_PATH_LENGTH		= 256;

} // end of Basic namespace

#endif

/*	
	Direction thought only

	int getDirection(CPoint& a, CPoint& b)
	{
		int dir(0);

		if(a.m_x == b.m_x)
		{
			x = 0;	
		}
		else if(a.m_x > b.m_x)
		{
			x = 2;
		}
		else
		{
			x = 8;
		}
		if(a.m_y == b.m_y)
		{
			y = 0;
		}
		else if(a.m_y > b.m_y)
		{
			y = 4;
		}
		else
		{
			y = 1;
		}
		
		return x + y;
	}


              1
		     9 3
		    8 0 2
			12 6
			  4
*/
