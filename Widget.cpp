// This is a part of the Widget Template Library.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Template Library product.

#include "StdAfx.h"
#include "Widget.h"

USING_NAMESPACE_WTL;

Widget::Widget(void)
: m_bNC(FALSE)
, m_pParent(NULL)
, m_hWid(INVALID_HWID)
, m_pDispatch(NULL)
, m_wState(WID_STATE_STATIC)
, m_hWid(INVALID_HWID)
, m_pVScrollbar(NULL)
, m_pHScrollbar(NULL)
{
}

Widget::~Widget(void)
{
	if (m_pDispatch != NULL)
	{
		m_pDispatch->Destroy(m_hWid);
	}
	TDEL(m_pVScrollbar);
	TDEL(m_pHScrollbar);
}

void Widget::GetRect( Gdiplus::Rect& rc )
{
	rc = m_rc;
}

void Widget::GetWidRect( Gdiplus::Rect& rc )
{
	rc = m_rcWid;
}

void Widget::SetWidRect( const Gdiplus::Rect& rc )
{
	m_rcWid = rc;
	SetRect(m_rcWid);
	LRESULT lResult = 0;
	ProcessWidMessage(WM_SIZE, 0, 0, lResult, 0);
}

void Widget::SetRect( const Gdiplus::Rect& rc )
{
	m_rc = rc;
}

BOOL Widget::Create( const Gdiplus::Rect& rc, MsgDispatcher* pDispatch, Widget* pParent, BOOL bNC /*= FALSE*/ )
{
	ASSERT(pHandler != NULL);
	m_pDispatch = pDispatch;
	m_pDispatch->Create(this);
	m_bNC = bNC;
	SetParent(pParent);
	SetWidRect(rc);
}

Widget* Widget::GetParent() const
{
	return m_pParent;
}

void Widget::SetParent( Widget* pParent )
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->SetParent(this, pParent);
}

ULONG Widget::GetChildren( std::vector<Widget*>& rgpChildren ) const
{
	rgpChildren = m_rgpChilren;
	return rgpChildren.size();
}

ULONG Widget::GetChildren() const
{
	m_rgpChilren.size();
}

BOOL Widget::HasChild() const
{
	return m_rgpChilren.size() != 0;
}

void Widget::InvalidWid()
{
	ASSERT(m_pDispatch != NULL);
	m_pDispatch->DrawWid(this);
}

void Widget::OnDraw( Gdiplus::Graphics& grph )
{
	Gdiplus::Rect rc;
	GetRect(rc);
	
}

void Widget::SetHwid( HWID hWid )
{
	m_hWid = hWid;
}

HWID Widget::GetHwid() const
{
	return m_hWid;
}

ULONG Widget::AddChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChilren.begin(), m_rgpChilren.end());
	if (it == m_rgpChilren.end())
	{
		m_rgpChilren.push_back(pWid);
	}
	return m_rgpChilren.size();
}

ULONG Widget::RemoveChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChilren.begin(), m_rgpChilren.end());
	if (it != m_rgpChilren.end())
	{
		m_rgpChilren.erase(it);
	}
	return m_rgpChilren.size();
}

void Widget::SetMyParent( Widget* pParent )
{
	m_pParent = pParent;
}

void Widget::EnableScrollBar( UINT wSBflags, UINT wArrows )
{

}

void Widget::SetScrollInfo( int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void Widget::GetScrollInfo( int nBar, LPSCROLLINFO lpsi )
{

}

void Widget::SetScrollBar( int nBar, ScrollBar* pScrollBar )
{

}

ScrollBar::ScrollBar( int nBar )
: m_nBar(nBar)
{
	m_pScrollInfo = new SCROLLINFO;
	memset(m_pScrollInfo, 0, sizeof(SCROLLINFO));
	m_pScrollInfo->cbSize = sizeof(SCROLLINFO);
}

ScrollBar::~ScrollBar()
{
	TDEL(m_pScrollInfo);
}

int ScrollBar::GetBar() const
{
	return m_nBar;
}

void ScrollBar::SetBar( int nBar )
{
	m_nBar = nBar;
}

void ScrollBar::GetScrollInfo( SCROLLINFO* pScrollInfo ) const
{
	ASSERT(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(pScrollInfo, m_pScrollInfo, sizeof(SCROLLINFO));
	}
}

void ScrollBar::SetScrollInfo( const SCROLLINFO* pScrollInfo )
{
	ASSERT(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(m_pScrollInfo, pScrollInfo, sizeof(SCROLLINFO));
	}
}
