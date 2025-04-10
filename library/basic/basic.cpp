/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Simple helper functions.
 **/
#include "basic.h"
#include "CCharString.h"
#include <time.h>


namespace Basic
{

	/*
	*	directions:
	*
	*	   321
	*      4 0		
	*      567
	*/

	int directionTableX[] = { 1,  1,  0, -1, -1, -1, 0, 1 };
	int directionTableY[] = { 0, -1, -1, -1,  0,  1, 1, 1 };

	template <class T> void swap(T &a, T &b)
	{
		T c(a);
		a = b;
		b = c;
	}

	int round(double d)
	{
		return (d >= 0.0) ? ((int) (d + 0.5)) : ((int) (d - 0.5));
	}

	int round(float f)
	{
		return (f >= 0.0f) ? ((int) (f + 0.5f)) : ((int) (f - 0.5f));
	}

	CCharString getTimeString()
	{
		char buffer[20];
		
		long timenow = time(0);
		struct tm *local;
		local = localtime(&timenow);

		strftime(buffer, 17, "%H:%M %Y-%m-%d", local);

		return CCharString(buffer);
	}

} // End namespace