
#include "CDirectX.h"
#include <Dxerr8.h>

#pragma comment(lib, "dxerr8.lib")

namespace Win32
{

// set these variables as soon as the program has started
HWND		CDirectX::us_window;	// the program window
HINSTANCE	CDirectX::us_instance;	// the program instance

CDirectX::CDirectX()
{
}

CDirectX::~CDirectX()
{
} 

// returns a pointer to a string with an error description text
char *CDirectX::getErrorHelp(HRESULT hr)
{
	// TODO: Implement this function!
	return "Unknown error";
}

// returns a pointer to a string with the error name
char *CDirectX::getErrorName(HRESULT hr)
{
	return (char *)DXGetErrorString8(hr);
}

void CDirectX::showError(HRESULT hr)
{
	::MessageBox(us_window, getErrorHelp(hr), getErrorName(hr), MB_ICONERROR);
}

} // end of Win32 namespace
