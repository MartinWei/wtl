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

#include "wtl.h"

BEGIN_NAMESPACE_WTL

class MsgDispatcher;
class ScrollBar;

class WTL_API WidgetBase
{
public:
	virtual BOOL ProcessWidMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID) = 0;
};

class WTL_API Widget :
	public WidgetBase
{
	friend class MsgDispatcher;
public:
	Widget(void);
	virtual ~Widget(void);

public:
	BOOL Create(const Gdiplus::RectF& rc, MsgDispatcher* pDispatch,
		Widget* pParent, BOOL bNC = FALSE);

	// Position
public:
	void GetRect(Gdiplus::RectF& rc);
	void GetWidRect(Gdiplus::RectF& rc);
	void SetWidRect(const Gdiplus::RectF& rc);
	void ShowWid(WORD wShow);

private:
	void SetRect(const Gdiplus::RectF& rc);
public:
	// Generation
	Widget* GetParent() const;
	void SetParent(Widget* pParent);

	// For MsgDispatcher
protected:
	void SetMyParent(Widget* pParent);
	ULONG GetChildren(std::vector<Widget*>& rgpChildren) const;
	ULONG GetChildren() const;
	BOOL HasChild() const;
	
protected:
	ULONG AddChild(Widget* pWid);
	ULONG RemoveChild(Widget* pWid);
public:
	// Graphics
	void InvalidWid();
protected:
	virtual void OnDraw(Gdiplus::Graphics& grph);

protected:
	void SetHwid(HWID hWid);
	HWID GetHwid() const;

	// Scrollbar
public:
	void EnableScrollBar(UINT wSBflags, UINT wArrows);
	void SetScrollInfo(int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
	void GetScrollInfo(int nBar, LPSCROLLINFO lpsi);
protected:
	void SetScrollBar(int nBar, ScrollBar* pScrollBar);

	// Text
public:
	void SetText(const std::wstring& strText);
	std::wstring GetText() const;
	void SetFormat(const SharedPtr<Gdiplus::StringFormat>& pFormat);
	const SharedPtr<Gdiplus::StringFormat> GetFormat() const;
	void SetFont(const SharedPtr<Gdiplus::Font>& pFont);
	const SharedPtr<Gdiplus::Font> GetFont() const;
	void SetBkgnd(const Gdiplus::Color& clrBkgnd);
	Gdiplus::Color GetBkgnd() const;
	void SetFrameClr(const Gdiplus::Color& clrFrame);
	Gdiplus::Color GetFrameClr() const;
	void SetTextClr(const Gdiplus::Color& clrText);
	Gdiplus::Color GetTextColor() const;

public:
	BOOL SendWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostWidMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

	UINT_PTR SetWidTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillWidTimer(UINT_PTR uIDEvent);

public:
	void SetState(WORD wState);
	WORD GetState() const;

private:
	WORD m_wState;

private:
	void InitFont();
private:
	std::wstring m_strText;
	SharedPtr<Gdiplus::StringFormat> m_pFormat;
	SharedPtr<Gdiplus::Font> m_pFont;
	Gdiplus::Color m_clrBkgnd;
	Gdiplus::Color m_clrFrame;
	Gdiplus::Color m_clrText;
private:
	// Position
	Gdiplus::RectF m_rc;
	Gdiplus::RectF m_rcWid;
	BOOL m_bNC;
	WORD m_wShow;

	// Generation
	Widget* m_pParent;
	std::vector<Widget*> m_rgpChilren;

	// Identifier
	HWID m_hWid;
	MsgDispatcher* m_pDispatch;

	// Scrollbar
	ScrollBar* m_pHScrollbar;
	ScrollBar* m_pVScrollbar;
	int m_nSBflags;

	// Timers
	std::vector<UINT_PTR> m_rgTimer;
};

class WTL_API ScrollBar : public Widget
{
public:
	ScrollBar(int nBar);
	virtual ~ScrollBar();
	int GetBar() const;
	void SetBar(int nBar);
	void GetScrollInfo(SCROLLINFO* pScrollInfo) const;
	void SetScrollInfo(const SCROLLINFO* pScrollInfo);
protected:
	int m_nBar;
	SCROLLINFO* m_pScrollInfo;
};

class WTL_API MsgDispatcher
{
	friend class Widget;
public:
	MsgDispatcher(HWND hWnd = NULL);
	~MsgDispatcher();
public:
	void SetHWnd(HWND hWnd);
	HWND GetHWnd() const;
protected:
	BOOL Create(Widget* pThis);
	BOOL SetParent(Widget* pThis, Widget* pParent);
	BOOL Destroy(HWID& hWid);
	HWID GenerateHwid();
	void RecycleHwid(HWID& hWid); 
protected:
	void DrawWid(Widget* pWid);
	void DrawGen(Widget* pWid, HDC hdc);
public:
	LRESULT DispatchMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	Widget* GetWidPt(POINT pt);
	Widget* GetWidPt(const std::vector<Widget*>& rgpWid);
	Widget* FromHwid(HWID hWid) const;
public:
	static HINSTANCE GetInstance();
	static void SetInstance(HINSTANCE hInstance);
	static RECT FromRect(const Gdiplus::RectF& rc);
protected:
	HWND m_hWnd;
	std::vector<HWID> m_rghWid;
	std::map<HWID, Widget*> m_rgID2Info;
	HWID m_hLastMouseMove;
public:
	static HWID s_hWidBase;
private:
	static HINSTANCE s_hInstance;
};

class WTL_API LayoutBase
{
public:
	virtual SharedPtr<MsgDispatcher> Parse(const std::wstring& strXml);
};

END_NAMESPACE_WTL
