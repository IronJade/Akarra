/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    The directplay interface.
 **/

#include "CDirectPlayServer.h"

#pragma comment(lib, "dplayx.lib")
//#pragma comment(lib, "dpnaddr.lib")
#pragma comment(lib, "dxguid.lib")


namespace Win32
{

CDirectPlayServer::CDirectPlayServer()
{
	clear();
}

CDirectPlayServer::~CDirectPlayServer()
{
	removeAll();
}

void CDirectPlayServer::removeAll()
{
	if (mp_dpaddress)
	{
		mp_dpaddress->Release();
	}
	if (mp_dpserver)
	{
		mp_dpserver->Release();
	}

	clear();
}

int CDirectPlayServer::getError()
{
	return m_error;
}

bool CDirectPlayServer::createServer()
{
	// initialize COM
	CoInitialize(NULL);

	// create server interface
	if (FAILED(m_error = CoCreateInstance(CLSID_DirectPlay8Server, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Server, (void**)&mp_dpserver)))
	{
		return false;
	}

	// initialize the server interface, sets the message handler function
	if (FAILED(m_error = mp_dpserver->Initialize(NULL, ServerMessageHandler, 0)))
	{
		return false;
	}

	// create an address interface
	if (FAILED(m_error = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_ALL,
		IID_IDirectPlay8Address, (void**)&mp_dpaddress)))
	{
		return false;
	}

	// set the communication provider to TCP/IP
	if (FAILED(m_error = mp_dpaddress->SetSP(&CLSID_DP8SP_TCPIP)))
	{
		return false;
	}

	return true;
}

bool CDirectPlayServer::startHost(unsigned long port, int maxplayers, wchar_t* servername, wchar_t* sessionname, const GUID& guid)
{
	if (!mp_dpserver || !mp_dpaddress)
	{
		return false;
	}

	// setup player information
	DPN_PLAYER_INFO pinfo;
	ZeroMemory(&pinfo, sizeof(DPN_PLAYER_INFO));
	pinfo.dwSize = sizeof(DPN_PLAYER_INFO);
	pinfo.dwInfoFlags = DPNINFO_NAME;
	pinfo.pwszName = servername;

	if (FAILED(m_error = mp_dpserver->SetServerInfo(&pinfo, NULL, NULL, DPNSETSERVERINFO_SYNC)))
	{
		return false;
	}

	// connect the address to a port
	if (FAILED(m_error = mp_dpaddress->AddComponent(DPNA_KEY_PORT, &port, sizeof(DWORD), DPNA_DATATYPE_DWORD)))
	{
		return false;
	}

	// setup server information
	DPN_APPLICATION_DESC appinfo;
	ZeroMemory(&appinfo, sizeof(DPN_APPLICATION_DESC));
	appinfo.dwSize = sizeof(DPN_APPLICATION_DESC);
	appinfo.guidApplication = guid;
	appinfo.pwszSessionName = sessionname;
	appinfo.dwMaxPlayers = maxplayers;
	appinfo.dwFlags = DPNSESSION_CLIENT_SERVER;

	// start the server
	if (FAILED(m_error = mp_dpserver->Host(&appinfo, &mp_dpaddress, 1, NULL, NULL, NULL, NULL)))
	{
		return false;
	}

	return true;
}

void CDirectPlayServer::clear()
{
	m_error = 0;
	mp_dpserver = NULL;
	mp_dpaddress = NULL;
	m_numplayers = 0;
}


} // end of Win32 namespace
