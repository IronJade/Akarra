/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    I don't really remember what I used this for.
 **/

#ifndef _CCHARACTERSHEET_H_
#define _CCHARACTERSHEET_H_

#include "CCharacterAttributes.h"

namespace RpgSystem
{

	/* **********************************************************************************
	* CLASS CCharacterSheet
	* Derived:	(base class)
	*
	* Purpose:	The whole character information, including name and similar stuff.
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011029): Renamed from CPlayer to CCharacterSheet.
	* (011022): First version.
	* **********************************************************************************/
	class CCharacterSheet
	{
		public:

			CCharacterSheet();
			~CCharacterSheet();

			CCharacterAttributes	u_attributes;
			char					ua_name[12];

		private:

		protected:

	};

} // end of RpgSystem namespace


#endif
