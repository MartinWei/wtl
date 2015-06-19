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
	WCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	wcscat_s(szBuffer, 300, L"\n");
	va_end(args);
	::OutputDebugString(szBuffer);
#endif
}

USING_NAMESPACE_WTL;

MemDC::MemDC( HDC hdc )
: m_hdc(hdc)
{

}

MemDC::~MemDC()
{

}

MemDC::operator HDC()
{
	return m_hdc;
}

GdiPlusHelper::GdiPlusHelper()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_nGdiPlusToken, &gdiplusStartupInput, NULL);
}

GdiPlusHelper::~GdiPlusHelper()
{
	Gdiplus::GdiplusShutdown(m_nGdiPlusToken);
}


BEGIN_NAMESPACE_WTL

GdiPlusHelper gdiplushelper;

END_NAMESPACE_WTL