#include "stdafx.h"
#include "cmdbar.h"
#include "resource.h"
#include "common.h"

#define COMBOBOX_SIZE 200
#define CONTROL_GAP 5

CCmdBar::CCmdBar()
	:m_hWnd(0)
	,m_hInst(0)
	,m_StaticName(0)
	,m_ComBoxCmd(0)
	,m_StaticStatus(0)
{
	m_BrushBkg.CreateSolidBrush(kCustomBkg);

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
}

CCmdBar::~CCmdBar()
{

}

CCmdBar::operator HWND()
{
	return m_hWnd;
}

BOOL CCmdBar::Init( HWND hWnd,HINSTANCE hInst)
{
	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass,sizeof(WNDCLASSEX));
	wndclass.cbSize		= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= CCmdBar::WinProc;
	wndclass.cbClsExtra	= 0;
	wndclass.cbWndExtra	= sizeof(CCmdBar*);
	wndclass.hInstance		= hInst;
	wndclass.hIcon			= 0;
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= _T("Z0BCmdBarBar");
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
		WS_EX_DLGMODALFRAME,
		_T("Z0BCmdBarBar"),
		NULL, 
		WS_VISIBLE |
		WS_CHILD,
		0,
		0,
		0,
		0,
		hWnd,
		(HMENU)IDD_DIALOG_CMDLINE,
		hInst,
		0);

	if (m_hWnd)
	{
		SetWindowLongPtr(m_hWnd,0,(LONG_PTR)this);

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		CClientRect rcClient(m_hWnd);
		
		m_rcTextBar.left = rcClient.Left();
		m_rcTextBar.top = rcClient.Top();
		m_rcTextBar.bottom = rcClient.Bottom();
		m_rcTextBar.right = 80;

// 		if(m_StaticName = CreateWindowEx(
// 							0,
// 							_T("Static"),
// 							_T("命令:"), 
// 							WS_VISIBLE|WS_CHILD | SS_CENTERIMAGE,
// 							0,
// 							0,
// 							0,
// 							0,
// 							m_hWnd,
// 							(HMENU)IDC_STATIC_CMD_NAME,
// 							hInst,
// 							0))
// 		{
// 			   
// 			::SendMessage(m_StaticName,WM_SETFONT,(WPARAM)hFont,1);  //设置控件字体    
// 			
// 			SetWindowPos(
// 					m_StaticName,
// 					NULL,
// 					0,
// 					0,
// 					80,
// 					25,
// 					SWP_SHOWWINDOW
// 					);
// 		}

		if(m_ComBoxCmd = CreateWindowEx(
			0,
			_T("ComboBox"),
			NULL, 
			WS_VSCROLL |0x10E42 |WS_CHILD,
			0,
			0,
			0,
			0,
			m_hWnd,
			(HMENU)IDC_COMBO_CMDLINE,
			hInst,
			0))
		{
			::SendMessage(m_ComBoxCmd,WM_SETFONT,(WPARAM)hFont,1);  //设置控件字体    

			SetWindowPos(
				m_ComBoxCmd,
				NULL,
				80,
				0,
				80,
				25,
				SWP_SHOWWINDOW
				);
		}

// 		if(m_StaticStatus = CreateWindowEx(
// 			0,
// 			_T("Static"),
// 			NULL, 
// 			WS_VISIBLE|0x1000|WS_CHILD,
// 			0,
// 			0,
// 			0,
// 			0,
// 			m_hWnd,
// 			(HMENU)IDC_STATIC_CMDLINE_INFO,
// 			hInst,
// 			0))
// 		{
// 			::SendMessage(m_StaticStatus,WM_SETFONT,(WPARAM)hFont,1);  //设置控件字体 
// 			SetWindowPos(
// 				m_StaticStatus,
// 				NULL,
// 				120,
// 				0,
// 				80,
// 				18,
// 				SWP_SHOWWINDOW
// 				);
// 		}

		ShowWindow(m_hWnd,SW_SHOW);
	}

	return TRUE;
}

void CCmdBar::OnPaint(HDC hDC, RECT* prcDirty, BOOL bErase)
{
	RECT rcClient;
	::GetClientRect(m_hWnd,&rcClient);
	FillRect(hDC,&rcClient,m_BrushBkg);

	SetTextColor(hDC,kCustomBlackColor);
	SelectObject(hDC,m_hPaintFont);
	::DrawText(
		hDC,
		_T("命令:"), 
		-1,
		&m_rcTextBar,
		DT_EXPANDTABS |
		DT_TABSTOP |
		DT_VCENTER |
		DT_CENTER |
		DT_SINGLELINE);
}

void CCmdBar::OnSize(WPARAM wParam,LPARAM lParam)
{
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	
	m_rcTextBar.left = rcClient.left;
	m_rcTextBar.top = rcClient.top;
	m_rcTextBar.bottom = rcClient.bottom;
	m_rcTextBar.right = 80;
// 	SetWindowPos(
// 		m_StaticName,
// 		NULL,
// 		rcClient.left,
// 		rcClient.top,
// 		80,
// 		18,
// 		SWP_SHOWWINDOW
// 		);

	SetWindowPos(
		m_ComBoxCmd,
		NULL,
		80 + CONTROL_GAP,
		rcClient.top,
		COMBOBOX_SIZE,
		rcClient.bottom - rcClient.top - 5,
		SWP_SHOWWINDOW
		);

// 	SetWindowPos(
// 		m_StaticStatus,
// 		NULL,
// 		80 + COMBOBOX_SIZE + 2*CONTROL_GAP,
// 		rcClient.top,
// 		rcClient.right - rcClient.left - (80 + COMBOBOX_SIZE + 2*CONTROL_GAP),
// 		rcClient.bottom - rcClient.top,
// 		SWP_SHOWWINDOW
// 		);
}

LRESULT CCmdBar::WinProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	CCmdBar* pData = (CCmdBar*)GetWindowLongPtr(hWnd,0);
	switch(uMsg)
	{
	case WM_SIZE:
		{
			if (pData)
			{
				pData->OnSize(wParam,lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			pData->OnPaint(ps.hdc, &ps.rcPaint, ps.fErase);
			EndPaint(hWnd, &ps);
		}
		return 0;
	case WM_ERASEBKGND:
		{
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}