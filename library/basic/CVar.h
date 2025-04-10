/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Console-style variable class. Not sure if it's used anywhere.
 **/

#ifndef _CVAR_H_
#define _CVAR_H_

#include "CCharString.h"

namespace Basic
{
	// A CVar is a "console variable" that always accepts strings as
	// a parameter. This string is then translated into both integers
	// and floating variables, though this conversion usually results
	// in "0" if the user hasn't submitted a legal value.
	class CVar
	{
		public:

			CVar();

			CVar(const CVar& rhs);
			CVar(const CVar* rhs);

			CVar(const int rhs);
			CVar(const float rhs);
			CVar(const char* rhs);

			CVar& operator = (const CVar& rhs);
			CVar& operator = (const CVar* rhs);

			CVar& operator = (const int rhs);
			CVar& operator = (const float rhs);
			CVar& operator = (const char* rhs);

			inline int getInt()
				{
					return (int)m_value;
				}
			inline float getFloat()
				{
					return m_value;
				}
			inline CCharString& getString()
				{
					return m_string;
				}

		protected:

			CCharString		m_string;
			float			m_value;
	};

	// this structure binds a CVar to a name and a set of flags
	struct SVarSettings
	{
		CVar*	variable;
		char*	name;
		char*	defvalue;	// default value
		int		flags;

		//SVarSettings() : variable(0), name(0), flags(0) { }
	};

	// variable flags:
	const int	CVARFLAG_READONLY		= 0x1;	// cannot be changed by the user
	const int	CVARFLAG_SYSTEM			= 0x2;	// requires a system/game restart
	const int	CVARFLAG_CHEAT			= 0x4;	// cheat protected, may only be changed if cheats are allowed
	const int	CVARFLAG_NOSAVE			= 0x8;	// this variable will never be outputted to an external textfile

} // end of Basic namespace

#endif
