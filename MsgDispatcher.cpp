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
#include "Widget.h"

USING_NAMESPACE_WTL;

HWID MsgDispatcher::s_hWidBase = INVALID_HWID;

HINSTANCE MsgDispatcher::s_hInstance = NULL;

MsgDispatcher::MsgDispatcher( HWND hWnd /*= NULL*/ )
: m_hWnd(hWnd)
{

}

MsgDispatcher::~MsgDispatcher()
{

}

void MsgDispatcher::SetHWnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

HWND MsgDispatcher::GetHWnd() const
{
	return m_hWnd;
}

BOOL MsgDispatcher::Create( Widget* pThis )
{
	ASSERT(pThis != NULL);
	ASSERT(pThis->GetHwid() == INVALID_HWID);
	if (pThis->GetHwid() != INVALID_HWID)
	{
		return FALSE;
	}
	HWID hNewWid = GenerateHwid();
	ASSERT(hNewWid != INVALID_HWID);
	pThis->SetHwid(hNewWid);
	ASSERT(m_rgID2Info.find(hNewWid) == m_rgID2Info.end());
	m_rgID2Info.insert(std::make_pair(hNewWid, pThis));
	return TRUE;
}

BOOL MsgDispatcher::Destroy( HWID& hWid )
{
	std::map<HWID, Widget*>::iterator it =
		m_rgID2Info.find(hWid);
	if (it == m_rgID2Info.end())
	{
		return FALSE;
	}
	Widget* pWid = it->second;
	ASSERT(pWid != NULL);
	Widget* pParent = pWid->GetParent();
	if (pParent != NULL)
	{
		pParent->RemoveChild(pWid);
	}
	m_rgID2Info.erase(it);
	RecycleHwid(hWid);
	return TRUE;
}

HWID MsgDispatcher::GenerateHwid()
{
	return ++s_hWidBase;
}

void MsgDispatcher::RecycleHwid( HWID& hWid )
{
	hWid = INVALID_HWID;
}

void MsgDispatcher::DrawWid( Widget* pWid )
{
	ASSERT(m_hWnd != NULL);
	ASSERT(pWid != NULL);
	HDC hdc = ::GetDC(m_hWnd);
	if (hdc == NULL)
	{
		return;
	}
	
	__begin_mem_draw
	MemDC drawdc(hdc);
	DrawGen(pWid, drawdc);
	__end_mem_draw;

	::ReleaseDC(m_hWnd, hdc);
}

void MsgDispatcher::DrawGen( Widget* pWid, HDC hdc)
{
	ASSERT(pWid != NULL);
	POINT pt = {0};
	Gdiplus::RectF rcWid;
	pWid->GetWidRect(rcWid);
	RECT rcc = FromRect(rcWid);
	//::SetViewportOrgEx(hdc, rcc.left, rcc.top, &pt);
	Gdiplus::Graphics grph(hdc);
	pWid->OnDraw(grph);
	//::SetViewportOrgEx(hdc, pt.x, pt.y, NULL);
	std::vector<Widget*> rgpChildren;
	pWid->GetChildren(rgpChildren);
	for (std::vector<Widget*>::iterator it = 
		rgpChildren.begin(); it != rgpChildren.end(); ++it)
	{
		ASSERT((*it) != NULL);
		DrawGen((*it), hdc);
	}
}

LRESULT MsgDispatcher::DispatchMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 1;
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	Widget* pWid = NULL;

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			pWid = GetWidPt(pt);
			if (pWid == NULL)
			{
				pWid = FromHwid(m_hLastMouseMove);
				if (pWid != NULL)
				{
					pWid->SendWidMessage(WM_MOUSELEAVE);
					m_hLastMouseMove = INVALID_HWID;
					return lResult;
				}
			}
			else
			{
				HWID hWidNowMouse = pWid->GetHwid();
				if (m_hLastMouseMove != hWidNowMouse)
				{
					Widget* pLastMouse = FromHwid(m_hLastMouseMove);
					if (pLastMouse != NULL)
					{
						pLastMouse->SendWidMessage(WM_MOUSELEAVE);
					}
					m_hLastMouseMove = hWidNowMouse;
				}
				pWid->SendWidMessage(WM_MOUSEMOVE);
				return lResult;
			}
		}
		break;
	}
	return lResult;
}

Widget* MsgDispatcher::GetWidPt( POINT pt )
{
	Widget* pWid = NULL;
	std::vector<Widget*> rgpWidInPt;
	for (std::map<HWID, Widget*>::iterator it = m_rgID2Info.begin();
		it != m_rgID2Info.end(); ++it)
	{
		ASSERT(it->second != NULL);
		Gdiplus::RectF rcWid;
		it->second->GetWidRect(rcWid);
		if (::PtInRect(&FromRect(rcWid), pt))
		{
			rgpWidInPt.push_back(it->second);
		}
	}
	if (m_rgID2Info.size() > 0)
	{
		if (m_rgID2Info.size() != 1)
		{
			pWid = GetWidPt(rgpWidInPt);
		}
		else
		{
			pWid = m_rgID2Info[0];
		}
	}

	return pWid;
}

Widget* MsgDispatcher::GetWidPt(const std::vector<Widget*>& rgpWid)
{
	Widget* pWid = NULL;
	std::vector<Widget*>::const_iterator it = rgpWid.begin();
	if (it != rgpWid.end())
	{
		pWid = *it;
		ASSERT(pWid != NULL);
		Gdiplus::RectF rcWid;
		pWid->GetWidRect(rcWid);
		float fArea = rcWid.Width * rcWid.Height;
		float fMinArea = fArea;
		for (; it != rgpWid.end(); ++it)
		{
			pWid->GetWidRect(rcWid);
			fArea = rcWid.Width * rcWid.Height;
			if (fArea < fMinArea)
			{
				fMinArea = fArea;
				pWid = (*it);
			}
		}
	}
	return pWid;
}

HINSTANCE MsgDispatcher::GetInstance()
{
	return s_hInstance;
}

void MsgDispatcher::SetInstance( HINSTANCE hInstance )
{
	s_hInstance = hInstance;
}

BOOL MsgDispatcher::SetParent( Widget* pThis, Widget* pParent )
{
	ASSERT(pThis != NULL);
	if (pParent == NULL)
	{
		return FALSE;
	}

	ULONG nChilren = pParent->GetChildren();
	if (pParent->AddChild(pThis) != nChilren + 1)
	{
		return FALSE;
	}
	pThis->SetMyParent(pParent);
	return TRUE;
}
