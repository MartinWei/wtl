// This is a part of the Widget Template Library.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Template Library product.

#pragma once

#ifdef WTL_EXPORTS
#define WTL_API //__declspec(dllexport)
#else
#define WTL_API //__declspec(dllimport)
#endif

#define BEGIN_NAMESPACE_WTL
#define END_NAMESPACE_WTL 
#define USING_NAMESPACE_WTL

#include <xstring>
#include <vector>
#include <map>
#include <algorithm>
#include <windowsx.h>
#include <CommCtrl.h>
#include <OleCtl.h>
#include <memory>
#include <GdiPlus.h>

DECLARE_HANDLE	(HWID);

#ifndef NULL
#define NULL 0
#endif

#ifndef ASSERT
#define ASSERT(expr) _ASSERT(expr)
#endif

#ifdef _DEBUG
#define TRACE __Trace
#else
#define TRACE
#define TRACEMSG _T("")
#endif

#define lengthof(x) (sizeof(x)/sizeof(*x))

void WTL_API __Trace(LPCTSTR pstrFormat, ...);

#ifndef TDEL
#define TDEL(p) if ((p) != NULL) { delete (p); (p) = NULL; }
#endif

#ifndef TDELA
#define TDELA(p) if ((p) != NULL) { delete[] (p); (p) = NULL; }
#endif

#define INVALID_HWID NULL

#define CLR_WID_BKGND_STATIC RGB(60, 60, 60)
#define CLR_WID_FRAME_STATIC RGB(100, 100, 100)
#define SIZE_SCROLLBAR 11

#define CLR_WID_BKGND_MOUSEMOVE RGB(30, 30, 30)
#define CLR_WID_FRAME_MOUSEMOVE RGB(90, 90, 90)

// State
#define WID_STATE_STATIC 0
#define WID_STATE_MOUSEOVER 1
#define WID_STATE_PUSH 2

#define WTL_BEGIN_MSG_MAP(theClass)\
	public:\
	BOOL ProcessWidMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID)\
	{\
		BOOL bHandled = TRUE;\
		(uMsg); \
		(wParam); \
		(lParam); \
		(lResult); \
		(bHandled); \
		switch(dwMsgMapID) \
		{ \
		case 0:

#define WTL_MESSAGE_HANDLER(msg, func) \
	if (uMsg == msg) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		if (bHandled = TRUE) \
			return TRUE; \
	}

#define WTL_CHAIN_MSG_MAP(theChainClass) \
	{ \
		if(theChainClass::ProcessWidMessage(uMsg, wParam, lParam, lResult)) \
		return TRUE; \
	}

#define WTL_END_MSG_MAP()\
			break; \
		default: \
			TRACE(_T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ASSERT(FALSE); \
			break; \
		} \
		return FALSE; \
	}