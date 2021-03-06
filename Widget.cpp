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
, m_pDispatch(NULL)
, m_wState(WID_STATE_STATIC)
, m_hWid(INVALID_HWID)
, m_pVScrollbar(NULL)
, m_pHScrollbar(NULL)
, m_strText(WID_DEFAULT_TEXT)
, m_nSBflags(-1)
, m_wShow(SW_SHOW)
{
	InitFont();
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

void Widget::GetRect( Gdiplus::RectF& rc )
{
	rc = m_rc;
}

void Widget::GetWidRect( Gdiplus::RectF& rc )
{
	rc = m_rcWid;
}

void Widget::SetWidRect( const Gdiplus::RectF& rc )
{
	m_rcWid = rc;
	SetRect(m_rcWid);
	LRESULT lResult = 0;
	ProcessWidMessage(WM_SIZE, 0, 0, lResult, 0);
}

void Widget::SetRect( const Gdiplus::RectF& rc )
{
	m_rc = rc;
}

BOOL Widget::Create( const Gdiplus::RectF& rc, MsgDispatcher* pDispatch, Widget* pParent, BOOL bNC /*= FALSE*/ )
{
	ASSERT(pDispatch != NULL);
	m_pDispatch = pDispatch;
	m_pDispatch->Create(this);
	m_bNC = bNC;
	SetParent(pParent);
	SetWidRect(rc);
	return TRUE;
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
	return m_rgpChilren.size();
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
	Gdiplus::RectF rc;
	GetRect(rc);
	Gdiplus::SolidBrush bkgnd(m_clrBkgnd);
	grph.FillRectangle(&bkgnd, rc.X, rc.Y, rc.Width, rc.Height);
	Gdiplus::SolidBrush text(m_clrText);
	grph.DrawString(m_strText.c_str(), m_strText.size(), m_pFont.get(),
		rc, m_pFormat.get(), &text);
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
		std::find(m_rgpChilren.begin(), m_rgpChilren.end(), pWid);
	if (it == m_rgpChilren.end())
	{
		m_rgpChilren.push_back(pWid);
	}
	return m_rgpChilren.size();
}

ULONG Widget::RemoveChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChilren.begin(), m_rgpChilren.end(), pWid);
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

void Widget::SetText( const std::wstring& strText )
{
	m_strText = strText;
}

std::wstring Widget::GetText() const
{
	return m_strText;
}

void Widget::SetFormat( const SharedPtr<Gdiplus::StringFormat>& pFormat )
{
	m_pFormat = pFormat;
}

const SharedPtr<Gdiplus::StringFormat> Widget::GetFormat() const
{
	return m_pFormat;
}

void Widget::SetFont( const SharedPtr<Gdiplus::Font>& pFont )
{
	m_pFont = pFont;
}

const SharedPtr<Gdiplus::Font> Widget::GetFont() const
{
	return m_pFont;
}

void Widget::SetBkgnd( const Gdiplus::Color& clrBkgnd )
{
	m_clrBkgnd = clrBkgnd;
}

Gdiplus::Color Widget::GetBkgnd() const
{
	return m_clrBkgnd;
}

void Widget::SetFrameClr( const Gdiplus::Color& clrFrame )
{
	m_clrFrame = clrFrame;
}

Gdiplus::Color Widget::GetFrameClr() const
{
	return m_clrFrame;
}

void Widget::SetState( WORD wState )
{
	if (wState == m_wState)
	{
		return;
	}

	m_wState = wState;

	SendWidMessage(WM_UPDATEUISTATE, m_wState);
}

WORD Widget::GetState() const
{
	return m_wState;
}

void Widget::SetTextClr( const Gdiplus::Color& clrText )
{
	m_clrText = clrText;
}

Gdiplus::Color Widget::GetTextColor() const
{
	return m_clrText;
}

void Widget::InitFont()
{
	m_pFormat.reset(new Gdiplus::StringFormat);
	m_pFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
	m_pFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	m_pFont.reset(new Gdiplus::Font(WID_FONT_STATIC, 
		WID_FSIZE_STATIC));
	m_clrBkgnd.SetFromCOLORREF(WID_BKGND_STATIC);
	m_clrFrame.SetFromCOLORREF(WID_FRAME_STATIC);
	m_clrText.SetFromCOLORREF(WID_TEXT_STATIC);
}

void Widget::ShowWid( WORD wShow )
{
	m_wShow = wShow;
}

BOOL Widget::SendWidMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return ProcessWidMessage(uMsg, wParam, lParam, lResult, 0);
}

BOOL Widget::PostWidMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return ProcessWidMessage(uMsg, wParam, lParam, lResult, 0);
}

UINT_PTR Widget::SetWidTimer( UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc )
{
	ASSERT(FALSE);
	return 1;
}

BOOL Widget::KillWidTimer( UINT_PTR uIDEvent )
{
	ASSERT(FALSE);
	return 1;
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

Widget* MsgDispatcher::FromHwid( HWID hWid ) const
{
	return NULL;
}

RECT MsgDispatcher::FromRect( const Gdiplus::RectF& rc )
{
	RECT rcc = {0};
	rcc.left = (LONG)rc.X;
	rcc.top = (LONG)rc.Y;
	rcc.right = rcc.left + (LONG)rc.Width;
	rcc.bottom = rcc.top + (LONG)rc.Height;
	return rcc;
}
