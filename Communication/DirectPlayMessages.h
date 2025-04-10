
#ifndef _DIRECTPLAYMESSAGES_H_
#define _DIRECTPLAYMESSAGES_H_

#include <dplay8.h>

namespace Comm
{
	struct SReceiveMessage
	{
		DPNHANDLE		handle;
		//DPNID			sender;
		void*			buffer;	// message buffer (located in DirectPlay until it's released)
		int				size;	// this is the size that DirectPlay says...
	};
};

#endif
