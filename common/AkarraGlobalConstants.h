/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Global definitions.
 **/

#ifndef _AKARRAGLOBALCONSTATS_H_
#define _AKARRAGLOBALCONSTATS_H_

#include <windows.h>

namespace Common
{

	// akarra executable versions:
	const int	AKARRA_SERVER_VERSION	= 4;
	// the client versions on the server build and client build must match if
	// the client should be able to connect
	const int	AKARRA_CLIENT_VERSION	= 21;
	// 20 - Released server
	// 21 - re-enabled the account creation

	// globally unique identifier for Akarra
	const GUID	AKARRA_GUID		= { 0x1472ca02, 0xae52, 0x47a1, { 0xb9, 0x7b, 0x98, 0x7d, 0x29, 0x1f, 0xe0, 0x8a } };

	// connection port
	const int	AKARRA_OFFICIALPORT	= 0x4ABC;
	const int	AKARRA_TESTPORT		= 0x4ABD;

	const int	AKARRA_PORT			= AKARRA_OFFICIALPORT;

	const unsigned int	LOGIN_KEY		= 0xC6A394B1;
	const unsigned int	SAVEFILE_KEY	= 0x6E85BD27;
}

#endif
