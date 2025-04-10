/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Primitive rectangle class.
 **/

#ifndef _CRECT_H_
#define _CRECT_H_

#include "CPoint.h"

namespace Basic
{

	/* **********************************************************************************
	* CLASS CRect
	* Derived:	(base class)
	*
	* Purpose:	To handle a rectangle area for several usages.
	*
	* Usage:	Create the class using any of the constructors. Several operators are
	*			also available. The area created _doesn't_ include the right and bottom
	*			line, which means that 'Width = Right - Left' and
	*			'Height = Bottom - Top'.
	*
	* ***********************************************************************************
	* (010712): Changed the right and bottom borders so that they are not included in the
	*			area. A screen rect for 800*600 would then be legal if it had the values
	*			(0, 0, 800, 600), instead of (0, 0, 799, 599).
	* (010711): First version (rewritten from Valhalla project).
	* **********************************************************************************/
	class CRect
	{
		public:
			// public variables:

			int u_left;		// left border
			int u_top;		// top border
			int u_right;	// right border (note: not included in the area, but u_right - 1 is)
			int u_bottom;	// bottom border (note: not included in the area, but u_bottom - 1 is)

			inline int left() { return u_left; }
			inline int top() { return u_top; }
			inline int right() { return u_right; }
			inline int bottom() { return u_bottom; }
			
			// Constructors

			// default constructor
			CRect() : u_left(0), u_top(0), u_right(0), u_bottom(0) { }
			// init constructor
			CRect(int left, int top, int right, int bottom) : u_left(left), u_top(top), u_right(right), u_bottom(bottom) { } 
			// init from two points
			CRect(CPoint &upperleft, CPoint &lowerright)
				{
					u_left = upperleft.u_x;
					u_top = upperleft.u_y;
					u_right = lowerright.u_x;
					u_bottom = lowerright.u_y;
				}
			// copy constructor
			CRect(CRect *rect)
				{
					u_left = rect->u_left;
					u_top = rect->u_top;
					u_right = rect->u_right;
					u_bottom = rect->u_bottom;
				}
			CRect(CRect &rect)
				{
					u_left = rect.u_left;
					u_top = rect.u_top;
					u_right = rect.u_right;
					u_bottom = rect.u_bottom;
				}

			// Operators
			
			// assignment operators
			CRect &operator = (CRect *rect)
				{
					u_left = rect->u_left;
					u_top = rect->u_top;
					u_right = rect->u_right;
					u_bottom = rect->u_bottom;
					return *this;
				}
			CRect &operator = (CRect &rect)
				{
					u_left = rect.u_left;
					u_top = rect.u_top;
					u_right = rect.u_right;
					u_bottom = rect.u_bottom;
					return *this;
				}
			// get an rectangle with an offset
			CRect operator + (CPoint &point)
				{
					return CRect(u_left + point.u_x, u_top + point.u_y, u_right + point.u_x, u_bottom + point.u_y);
				}
			CRect operator - (CPoint &point)
				{
					return CRect(u_left - point.u_x, u_top - point.u_y, u_right - point.u_x, u_bottom - point.u_y);
				}
			// move this rectangle so its upper left corner is at the point's position
			CRect &operator = (CPoint &point) 
				{
					// save width and height
					int width = getWidth();
					int height = getHeight();

					u_left = point.u_x;
					u_top = point.u_y;
					u_right = point.u_x + width;
					u_bottom = point.u_y + height;

					return *this;
				}
			// move the rectangle with an offset
			CRect &operator += (CPoint &point)
				{
					u_left += point.u_x; u_top += point.u_y; u_right += point.u_x; u_bottom += point.u_y;
					return *this;
				}
			CRect &operator -= (CPoint &point)
				{
					u_left -= point.u_x; u_top -= point.u_y; u_right -= point.u_x; u_bottom -= point.u_y;
					return *this;
				}

			// Functions

			int getWidth() const { return (u_right - u_left); }
			int getHeight() const { return (u_bottom - u_top); }
			int getSize() { return getWidth() * getHeight(); }

			// get upper left corner
			CPoint getTopLeft() { return CPoint(u_left, u_top); }
			// get lower right corner
			CPoint getBottomRight() { return CPoint(u_right, u_bottom); }
			// get the center point
			CPoint getCenterPoint()
				{
					int width = getWidth();
					int height = getHeight();

					return CPoint(u_left + (width >> 1), u_top + (height >> 1));
				}

			// clip the rectangle within another rectangle
			void clipRect(CRect &rect)
				{
					if (u_left < rect.u_left)		u_left = rect.u_left;
					if (u_top < rect.u_top)			u_top = rect.u_top;
					if (u_right > rect.u_right)		u_right = rect.u_right;
					if (u_bottom > rect.u_bottom)	u_bottom = rect.u_bottom;
				}
			// check if the rectangle is completely within another rectangle
			bool isWithinRect(CRect &rect) { return ((u_left >= rect.u_left) && (u_top >= rect.u_top) && (u_right <= rect.u_right) && (u_bottom <= rect.u_bottom)); }
			// check if two rectangles intersect
			bool doesIntersect(CRect &rect)
				{
					return (!((u_left >= rect.u_right) || (u_right <= rect.u_left) || (u_top >= rect.u_bottom) || (u_bottom <= rect.u_top)));
				}

			// note about points:
			// if you have a screen rectangle with the size of (0, 0, 800, 600) a point inside it would be legal as
			// long as it has values inside a box of (0, 0, 799, 599)
			
			// check if a point is within the rectangle
			bool pointInRect(CPoint &point) const { return ((point.u_x >= u_left) && (point.u_x < u_right) && (point.u_y >= u_top && point.u_y < u_bottom)); }
			bool pointInRect(CPoint *point) const { return ((point->u_x >= u_left) && (point->u_x <=u_right) && (point->u_y >= u_top && point->u_y < u_bottom)); }
			// check if a coordinate is within the rectangle
			bool pointInRect(int x, int y) { return ((x >= u_left) && (x < u_right) && (y >= u_top && y < u_bottom)); }

			// set the rect
			void setRect(int x1, int y1, int x2, int y2)
				{
					u_left = x1;
					u_top = y1;
					u_right = x2;
					u_bottom = y2;

				}
			void setRect(CPoint &topleft, CPoint &bottomright)
				{
					u_left = topleft.u_x;
					u_top = topleft.u_y;
					u_right = bottomright.u_x;
					u_bottom = bottomright.u_y;
				}

		protected:

	};

} // end of 'namespace Basic'

#endif
