#include "stdafx.h"
#include "statusbar.h"
#include "resource.h"
#include <strsafe.h>

CStatusBar::CStatusBar()
	:m_hWnd(NULL)
	,m_nPercentProcess(100.0f)
	,m_colorProcess(RGB(255,255,0))
{
	m_PannelBrush.CreateSolidBrush(STATUS_PANNEL_COLOR);
	m_ScrollBrush.CreateSolidBrush(m_colorProcess);
	m_PannelPen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	m_hPaintFont = CreateFont(
		0 - 12,
		0 - 12/2,
		0,
		0,
		400,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET, 
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 
		DEFAULT_QUALITY,
		FF_DONTCARE,
		_T("Terminal 6")
		);
	StringCchCopy((STRSAFE_LPWSTR)m_pszText,MAX_PATH,_T("×¼±¸¾ÍÐ÷"));
}

CStatusBar::~CStatusBar()
{

}

CStatusBar::operator HWND()
{
	return m_hWnd;
}

BOOL CStatusBar::Init( HWND hWnd,HINSTANCE hInst)
{
	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass,sizeof(WNDCLASSEX));
	wndclass.cbSize		= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= CStatusBar::WinProc;
	wndclass.cbClsExtra	= 0;
	wndclass.cbWndExtra	= sizeof(CStatusBar*);
	wndclass.hInstance		= hInst;
	wndclass.hIcon			= 0;
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= _T("Z0BStatusBar");
	wndclass.hIconSm		= NULL;

	if(!RegisterClassEx(&wndclass))
	{
		// return if RegisterClassEx fails
		DWORD dw_LastError = GetLastError();
		if(ERROR_CLASS_ALREADY_EXISTS != dw_LastError)
		{
			// return if the error is other than "error class already exists" 
			return FALSE;
		}
	}

	m_hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("Z0BStatusBar"),
		NULL, 
		WS_VISIBLE |
		WS_CHILD |
		WS_TABSTOP,
		0,
		0,
		0,
		0,
		hWnd,
		(HMENU)IDC_Z0B_STATUSBAR,
		hInst,
		0);

	if (m_hWnd)
	{
		SetWindowLongPtr(m_hWnd,0,(LONG_PTR)this);
		ShowWindow(m_hWnd,SW_SHOW);
	}

	return TRUE;
}

LRESULT CStatusBar::WinProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	CStatusBar* pData = (CStatusBar*)GetWindowLongPtr(hWnd,0);
	if (pData)
	{
		switch(uMsg)
		{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hWnd, &ps);
				pData->OnPaint(ps.hdc, &ps.rcPaint, ps.fErase);
				EndPaint(hWnd, &ps);
			}
			return 0;
		}
	}
	
	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void CStatusBar::OnPaint(HDC hDC, RECT* prcDirty, BOOL bErase)
{
	RECT rcClient;
	::GetClientRect(m_hWnd,&rcClient);
	FillRect(hDC,&rcClient,m_PannelBrush);

	CRect rcPercent = rcClient;
	rcPercent.left = rcClient.left;
	rcPercent.right = rcClient.left + m_nPercentProcess*((float)rcClient.right - (float)rcClient.left);

	FillRect(hDC,rcPercent,m_ScrollBrush);
	SetBkMode(hDC,TRANSPARENT);
	SelectObject(hDC,m_hPaintFont);
	SetTextColor(hDC,RGB(255,0,0));
	::DrawText(
		hDC,
		(LPCWSTR)&m_pszText[0], 
		-1,
		&rcClient,
		DT_EXPANDTABS |
		DT_TABSTOP |
		DT_VCENTER |
		DT_LEFT |
		DT_SINGLELINE);
}

void CStatusBar::ShowStatusInfo(LPTSTR pszText,float Percent)
{
	m_nPercentProcess = Percent;
	StringCchCopy((STRSAFE_LPWSTR)m_pszText,MAX_PATH,pszText);
	InvalidateRect(m_hWnd,NULL,FALSE);
}

void CStatusBar::ShowStatusInfo(LPTSTR pszText)
{
	StringCchCopy((STRSAFE_LPWSTR)m_pszText,MAX_PATH,pszText);
	InvalidateRect(m_hWnd,NULL,FALSE);
}

void CStatusBar::AddProcess(float Percent)
{
	m_nPercentProcess += Percent;
	InvalidateRect(m_hWnd,NULL,FALSE);
}