#pragma once

#ifndef _OPCTESTCLIENT_STDAFX_H
#define _OPCTESTCLIENT_STDAFX_H

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxcview.h>
#include <afxmt.h>			// MFC support for multithreaded synchronization objects
#include <afxadv.h>
#include <process.h>
#include "ocidl.h"
#include "comcat.h"

#include "globals.h"
#include "safelock.h"
#include "safearray.h"
#include "opcda.h"
#include "opccomn.h"
#include "opcerrors.h"
#include "opcprops.h"
#include "opcquality.h"
#include "server.h"
#include "group.h"
#include "item.h"

 #ifndef tstring
 #include <string>
 #if defined(_UNICODE) || defined(UNICODE)
 	#define tstring	std::wstring
 #else
 	#define tstring	std::string
 #endif
 #endif

#endif // _OPCTESTCLIENT_STDAFX_H
