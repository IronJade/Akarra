/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The directplay interface.
 **/

#ifndef _CDIRECTPLAYSERVER_H_
#define _CDIRECTPLAYSERVER_H_


#include "CDirectX.h"
#include <dplay8.h>

namespace Win32
{

	// this function must be created by the game that uses the class
	extern HRESULT WINAPI ServerMessageHandler(void* usercontext, unsigned long message, void* buffer);
	
	class CDirectPlayServer : public CDirectX
	{
		public:

			CDirectPlayServer();
			~CDirectPlayServer();

			void removeAll();
			
			int getError();
						
			bool createServer();
			bool startHost(unsigned long port, int maxplayers, wchar_t* servername, wchar_t* sessionname, const GUID& guid);
			inline IDirectPlay8Server* getDevice() { return mp_dpserver; }

		private:

			void clear();

		protected:

			int						m_error;		// last error code

			IDirectPlay8Server*		mp_dpserver;	// directplay server interface
			IDirectPlay8Address*	mp_dpaddress;	// directplay address used for communication

			int						m_numplayers;	// number of players allowed

			
	};

} // end of Win32 namespace



#endif
