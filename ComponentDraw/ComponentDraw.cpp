// ComponentDraw.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "afx.h"
#include <afxdllx.h>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#define new DEBUG_NEW
#endif


// The following symbol used to force inclusion of this module for _USRDLL

#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif


static AFX_EXTENSION_MODULE WGFDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	//gdi+使用的变量
	GdiplusStartupInput gdiplusStartupInput;
	unsigned long * pGdiToken = NULL;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("COMPONENTDRAW.DLL Initializing!\n");

		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(WGFDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(WGFDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("WGF.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(WGFDLL);
	}
	return 1;   // ok
}
