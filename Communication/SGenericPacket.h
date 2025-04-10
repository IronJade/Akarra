
#ifndef _SGENERICPACKET_H_
#define _SGENERICPACKET_H_

// The generic packet is a packet header that identifies the packet type.
// It also holds some other interesting information, such as size and
// send time.

namespace Comm
{

	struct SGenericPacket
	{
		unsigned int	type;	// message identifier
		unsigned int	size;	// size of message, including the generic header
		//unsigned int	time;	// game time when the packet was sent
	};

} // end of Comm namespace

#endif
