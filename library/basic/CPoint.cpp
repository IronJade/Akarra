/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Primitive two-dimensional point class.
 **/

#include "CPoint.h"
#include "CRect.h"

namespace Basic
{
	// khaile 010827: the reason the CPoint functions are placed here is because
	// the cpoint's header already is included in this header, and that adding crect's
	// header to cpoint's header causes compiler errors
	void CPoint::capsulize(CRect &rect)
	{ 
		if (u_x < rect.u_left)			u_x = rect.u_left;
		else if (u_x >= rect.u_right)	u_x = rect.u_right - 1;
		if (u_y < rect.u_top)			u_y = rect.u_top;
		else if (u_y >= rect.u_bottom)	u_y = rect.u_bottom - 1;
	}
	bool CPoint::isWithin(CRect &rect)
	{
		return ((u_x >= rect.u_left) && (u_x < rect.u_right) && (u_y >= rect.u_top) && (u_y < rect.u_bottom));
	}
	// !khaile

}
