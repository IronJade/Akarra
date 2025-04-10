/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Primitive two-dimensional point class.
 **/

#ifndef CPOINT_H
#define CPOINT_H

//#include "CRect.h"


namespace Basic
{

	class CRect;

	/* **********************************************************************************
	* CLASS CPoint
	* Derived:	(base class)
	*
	* Purpose:	To handle a coordinate for several usages.
	*
	* Usage:	Create the class using any of the constructors. Several operators are
	*			also available. (No special conditions.)
	*
	* ***********************************************************************************
	* (010711): First version (rewritten from Valhalla project).
	* **********************************************************************************/
	class CPoint
	{
		public:
			// public variables:
			int		u_x;	// x-wise coordinate
			int		u_y;	// y-wise coordinate
			
			inline int x() { return u_x; }
			inline int y() { return u_y; }
			
			// Constructors

			// default constructor
			CPoint() : u_x(0), u_y(0) { }
			// init constructor
			CPoint(int x, int y) : u_x(x), u_y(y) { }
			// copy constructor
			CPoint(CPoint *point) : u_x(point->u_x), u_y(point->u_y) { }
			// copy constructor 2
			CPoint(CPoint &point) : u_x(point.u_x), u_y(point.u_y) { }

			// Operators

			// assignment operators
			CPoint	&operator = (CPoint *point)			{ u_x = point->u_x; u_y = point->u_y; return *this; }
			CPoint	&operator = (CPoint &point)			{ u_x = point.u_x; u_y = point.u_y; return *this; }
			// is point equal to another point?
			bool	operator  == (CPoint &point) const	{ return (u_x == point.u_x && u_y == point.u_y); }
			// is point not equal to another point?
			bool	operator  != (CPoint &point) const	{ return (u_x != point.u_x || u_y != point.u_y); }
			// add points together
			CPoint	&operator += (CPoint &point)		{ u_x += point.u_x; u_y += point.u_y; return *this; }
			// subtract point
			CPoint	&operator -= (CPoint &point)		{ u_x -= point.u_x; u_y -= point.u_y; return *this; }
			// return an inverted point
			CPoint	operator  - () const				{ return CPoint(-u_x, -u_y); }
			// return an added point
			CPoint	operator  + (CPoint point) const	{ return CPoint(u_x + point.u_x, u_y + point.u_y); }
			// return a subtracted point
			CPoint	operator  - (CPoint point) const	{ return CPoint(u_x - point.u_x, u_y - point.u_y); }
			// return a point that is multiplied with int i
			CPoint	operator  * (int i) const			{ return CPoint(u_x * i, u_y * i); }
			

			// Functions
			
			// move the point
			void move(int x, int y) { u_x += x; u_y += y; }
			// capsulize the point within an area 
			void capsulize(int x1, int y1, int x2, int y2) 
				{ 
					if (u_x < x1)		u_x = x1;
					else if (u_x > x2)	u_x = x2;
					if (u_y < y1)		u_y = y1;
					else if (u_y > y2)	u_y = y2;
				}
			void capsulize(CRect &rect);
			// check if point is within an area
			bool isWithin(int x1, int y1, int x2, int y2)
				{
					return ((u_x >= x1) && (u_x < x2) && (u_y >= y1) && (u_y < y2));
				}
			bool isWithin(CRect &rect);

		protected:

	};

} // end of 'namespace Basic'

#endif
