
#ifndef CDIRECTX_H
#define CDIRECTX_H

#include <windows.h>

namespace Win32
{

	/* **********************************************************************************
	* CLASS CDirectX
	* Derived:	(base class)
	*
	* Purpose:	A base class for all DirectX-based objects. Includes default window
	*			and instance handles.
	*
	* Usage:	Set CDirectX::ms_window and CDirectX::ms_instance when the program starts.
	*			Derive all DirectX classes from this base class.
	*
	* ***********************************************************************************
	* (010713): First version.
	* **********************************************************************************/
	class CDirectX
	{
		public:
			// public variables

			static HWND			us_window;		// the program window
			static HINSTANCE	us_instance;	// the program instance

			// public functions

			CDirectX();

			virtual ~CDirectX();

			// returns a pointer to a string with an error description text
			static char *getErrorHelp(HRESULT hr);
			// returns a pointer to a string with the error name
			static char *getErrorName(HRESULT hr);
			// prints an error in a MessageBox
			static void showError(HRESULT hr);

		protected:

			// protected variables

			// protected functions
	};

} // end of Win32 namespace

#endif
