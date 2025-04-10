/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Old money rules.
 **/

#ifndef _CMONEY_H_
#define _CMONEY_H_

#include "../library/Basic/CCharString.h"

namespace RpgSystem
{

	enum EMoneyTypes
	{
		MT_NORMAL = 0,	// "normal" human currency
		MT_MENIT,		// menit/gifted currency
		MT_IWID,		// iwid/shining currency

		NUM_MONEYTYPES
	};

	class CMoney
	{
		public:

			static int getMaxCarry(int level, int moneytype);
			static char* getMoneyName(int moneytype);
			static Basic::CCharString getMoneyString(int amount, int type);
			static Basic::CCharString getMoneyValue(int amount, int type);
	};
	
}


#endif
