// This is a part of the Widget Template Library.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Template Library product.

#include "stdafx.h"
#include "wtl.h"

#ifdef _DEBUG
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

void __Trace( LPCTSTR pstrFormat, ... )
{
#ifdef _DEBUG
	TCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	_tcscat_s(szBuffer, 300, _T("\n"));
	va_end(args);
	::OutputDebugString(szBuffer);
#endif
}
