#include "stdafx.h"
#include "idaview.h"
#include "global.h"
#include "resource.h"
#include "disassem.h"
#include <strsafe.h>
#include <atlconv.h>
#include <Shlwapi.h>
#include <process.h>
#include "util.h"

#pragma comment(lib,"Shlwapi.lib")
#define VIEW_FRAME_LINEE_WIDTH	1
#define MIN_VIEW_VIDTH_HEIGHT 4

CIdaView::CIdaView()
	:m_hWnd(NULL)
	,m_hInst(NULL)
	,m_BackLineBrush(NULL)
{
	ZeroMemory(m_FilePath,MAX_PATH*sizeof(TCHAR));
	ZeroMemory(&m_ViewPos,MAX_ROOM*sizeof(ViewPos));
	m_MousePos.MousePt.x = 0;
	m_MousePos.MousePt.y = 0;
	m_BackLineBrush		= (HBRUSH)GetStockObject(GRAY_BRUSH);
	m_SelectLineBrush		= (HBRUSH)GetStockObject(BLACK_BRUSH);
}

CIdaView::~CIdaView()
{
	::DestroyWindow(m_ReverseView);
	::DestroyWindow(m_HexOtcDataView);
	::DestroyWindow(m_FileHeaderView);
}

CIdaView* CIdaView::Init(
	HINSTANCE hInst,
	HWND hWnd,
	LPTSTR szClass,
	LPTSTR szTitle,
	UINT Id )
{
	CIdaView* pIdaView = NULL;
	HWND MdiChildHandle = NULL;

	WNDCLASSEX wndclassMainFrame;
	wndclassMainFrame.cbSize		= sizeof(WNDCLASSEX);
	wndclassMainFrame.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassMainFrame.lpfnWndProc	= CIdaView::DefMdiWinProc;
	wndclassMainFrame.cbClsExtra	= 0;
	wndclassMainFrame.cbWndExtra	= sizeof(CIdaView*);
	wndclassMainFrame.hInstance		= hInst;
	wndclassMainFrame.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_IDA_BASE));
	wndclassMainFrame.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclassMainFrame.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclassMainFrame.lpszMenuName	= NULL;
	wndclassMainFrame.lpszClassName	= szClass;
	wndclassMainFrame.hIconSm		= NULL;

	if(!RegisterClassEx(&wndclassMainFrame))
	{
		// return if RegisterClassEx fails
		DWORD dw_LastError = GetLastError();
		if(ERROR_CLASS_ALREADY_EXISTS != dw_LastError)
		{
			// return if the error is other than "error class already exists" 
			return FALSE;
		}
	}

	//m_hInst		= hInst;

	MDICREATESTRUCT    ChildStruct;
	ChildStruct.szClass = szClass;
	ChildStruct.szTitle = szTitle;
	ChildStruct.hOwner	 = hInst;
	ChildStruct.x		 = CW_USEDEFAULT;
	ChildStruct.y		 = CW_USEDEFAULT;
	ChildStruct.cx		 = CW_USEDEFAULT;
	ChildStruct.cy		 = CW_USEDEFAULT;
	ChildStruct.style	 = 0;
	ChildStruct.lParam	 = 0;

	MdiChildHandle = (HWND)SendMessage(
		hWnd,
		WM_MDICREATE,
		(WPARAM)0,
		(LPARAM)&ChildStruct);

	pIdaView = (CIdaView*)GetWindowLongPtr( MdiChildHandle,0);

	if (pIdaView)
	{
		*pIdaView = hInst;
	}
	return pIdaView;
}

LRESULT CIdaView::DefMdiWinProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	CIdaView* pData = (CIdaView*) GetWindowLongPtr(hWnd, 0);

	switch(Msg)
	{
	case WM_CREATE:
		{

		}
		break;
	case WM_MDIACTIVATE:
		{
			if (pData)
			{
				pData->OnViewActive(hWnd);
			}			
		}
		break;
	case WM_SIZE:
		{
			if (pData)
			{
				pData->OnViewSize(lParam,wParam);
			}

		}
		break;
	case WM_MOUSEMOVE:
		{
			pData->OnViewMouseMove(lParam,wParam);
		}
		return 0;
	case WM_MOUSELEAVE:
		{
			pData->OnViewMouseLeave(lParam,wParam);
		}
		return 0;
	case WM_LBUTTONDOWN:
		{
			pData->OnViewLButtonDown(lParam,wParam);
		}
		return 0;
	case WM_CPU_VIEW_LBUTTON:
		{
			//pData->CpuViewUserLButtonDown(lParam,wParam);
		}
		return 0;
		break;
	case WM_LBUTTONUP:
		{
			pData->OnViewLButtonUp(lParam,wParam);
		}
		return 0;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);			
			pData->OnViewPaint(ps.hdc, &ps.rcPaint, ps.fErase);
			EndPaint(hWnd, &ps);
		}
		return 0;
	case WM_NCCREATE:
		{
			if (!pData)
			{
				pData = new CIdaView();
				*pData = hWnd;
				SetWindowLongPtr(hWnd,0,(LONG_PTR)pData);
			}
		}
		break;
	case WM_USER + WM_MOUSEMOVE:
		{
			//pData->CpuViewUserMouseMove(lParam,wParam);

			return 0;
		}
		break;
	case WM_CLOSE:
		{
			//DestroyWindow(hWnd);
			ShowWindow(hWnd,SW_HIDE);
		}
	case WM_DESTROY:
		{
			if (pData)
			{
				delete pData;
				pData = NULL;
			}
			SetWindowLongPtr( hWnd,0,0);

		}
		break;
	case WM_ERASEBKGND:
		{
			return 0;
		}
		break;
	}	

	return DefMDIChildProc(hWnd, Msg, wParam, lParam);
}

BOOL CIdaView::OnViewActive( HWND hWnd )
{
	if ((HWND)m_ReverseView == 0)
	{
		m_ReverseView.Init(hWnd,m_hInst);
 		m_ReverseView.ShowWindow(SW_SHOW);

		m_FileHeaderView.Init(hWnd,m_hInst);
		m_FileHeaderView.ShowWindow(SW_SHOW);

		m_HexOtcDataView.Init(hWnd,m_hInst);
		m_HexOtcDataView.ShowWindow(SW_SHOW);
		//m_pDisAsmView.SetColumnWidth(0,2*m_pDisAsmView.GetColumnWidth(0));
		//m_pDisAsmView.SetColumnWidth(1,2*m_pDisAsmView.GetColumnWidth(1));
		//m_pDisAsmView.BroadCastView(kCpuAsmView);
	}
	else
	{
		m_ReverseView.InvalidateRect();
		m_FileHeaderView.InvalidateRect();
		m_HexOtcDataView.InvalidateRect();
	}

	SendMessage(m_hWnd,WM_SIZE,0,0);
	
	return TRUE;
}

void CIdaView::OnViewPaint( HDC hDC, RECT* prcDirty, BOOL bErase)
{
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	int RoomedId = IsZoomed(m_hWnd);
	if (m_ViewPos[RoomedId].bInit)
	{
		RECT rcTopViewLine;
		rcTopViewLine.left	= m_ViewPos[RoomedId].ViewClient.left - VIEW_FRAME_LINEE_WIDTH;
		rcTopViewLine.right	= m_ViewPos[RoomedId].ViewClient.right + VIEW_FRAME_LINEE_WIDTH;
		rcTopViewLine.top	= m_ViewPos[RoomedId].ViewClient.top - VIEW_FRAME_LINEE_WIDTH;
		rcTopViewLine.bottom	= m_ViewPos[RoomedId].ViewClient.top;

		RECT rcLeftViewLine;
		rcLeftViewLine.left		= m_ViewPos[RoomedId].ViewClient.left - VIEW_FRAME_LINEE_WIDTH;
		rcLeftViewLine.right	= m_ViewPos[RoomedId].ViewClient.left;
		rcLeftViewLine.top		= m_ViewPos[RoomedId].ViewClient.top - VIEW_FRAME_LINEE_WIDTH;
		rcLeftViewLine.bottom	= m_ViewPos[RoomedId].ViewClient.top;

		RECT rcBottonViewLine;
		rcBottonViewLine.left	= m_ViewPos[RoomedId].ViewClient.left - VIEW_FRAME_LINEE_WIDTH;
		rcBottonViewLine.right	= m_ViewPos[RoomedId].ViewClient.right + VIEW_FRAME_LINEE_WIDTH;
		rcBottonViewLine.top	= m_ViewPos[RoomedId].ViewClient.bottom;
		rcBottonViewLine.bottom	= m_ViewPos[RoomedId].ViewClient.bottom + VIEW_FRAME_LINEE_WIDTH;

		RECT rcRightViewLine;
		rcRightViewLine.left	= m_ViewPos[RoomedId].ViewClient.right;
		rcRightViewLine.right	= m_ViewPos[RoomedId].ViewClient.right + VIEW_FRAME_LINEE_WIDTH;
		rcRightViewLine.top		= m_ViewPos[RoomedId].ViewClient.top - VIEW_FRAME_LINEE_WIDTH;
		rcRightViewLine.bottom	= m_ViewPos[RoomedId].ViewClient.bottom + VIEW_FRAME_LINEE_WIDTH;

		RECT rcHMiddleLine;
		rcHMiddleLine.left		= m_ViewPos[RoomedId].ViewClient.left - VIEW_FRAME_LINEE_WIDTH;
		rcHMiddleLine.right		= m_ViewPos[RoomedId].AsmView.left;
		rcHMiddleLine.top		= m_ViewPos[RoomedId].AddressView.bottom;
		rcHMiddleLine.bottom	= m_ViewPos[RoomedId].DataView.top;

		RECT rcVMiddleLine;
		rcVMiddleLine.left		= m_ViewPos[RoomedId].AsmView.left - VIEW_FRAME_LINEE_WIDTH;
		rcVMiddleLine.right		= m_ViewPos[RoomedId].AsmView.left;
		rcVMiddleLine.top		= m_ViewPos[RoomedId].ViewClient.top - VIEW_FRAME_LINEE_WIDTH;
		rcVMiddleLine.bottom	=  m_ViewPos[RoomedId].ViewClient.bottom + VIEW_FRAME_LINEE_WIDTH;

		FillRect(hDC,&rcTopViewLine,m_BackLineBrush);
		FillRect(hDC,&rcLeftViewLine,m_BackLineBrush);
		FillRect(hDC,&rcBottonViewLine,m_BackLineBrush);
		FillRect(hDC,&rcRightViewLine,m_BackLineBrush);

		FillRect(hDC,&rcHMiddleLine,m_BackLineBrush);
		FillRect(hDC,&rcVMiddleLine,m_BackLineBrush);
		
	}
	else
	{
		FillRect(hDC,&rcClient,m_BackLineBrush);		
	}

}

void CIdaView::OnViewMouseLeave(LPARAM lParam,WPARAM wParam)
{
	if (GetCapture() == m_hWnd)
	{
		ReleaseCapture();
	}
}

void CIdaView::OnViewLButtonUp(LPARAM lParam,WPARAM wParam)
{
	if ( GetCapture() == m_hWnd)
	{
		ReleaseCapture();
	}

	m_LButtonDown = FALSE;
}

void CIdaView::OnViewLButtonDown(LPARAM lParam,WPARAM wParam)
{
	RECT rcClient;
	POINT MousePt;
	ULONG RoomedId;
	HCURSOR MouseCursor;	

	RoomedId = IsZoomed(m_hWnd);

	GetCursorPos(&MousePt);
	ScreenToClient(m_hWnd, &MousePt);
	SetCursor(CpuViewTraceLoadCursor(MousePt,m_ViewPos[RoomedId].TraceViewLine));
	SetCapture(m_hWnd);

	if (m_ViewPos[RoomedId].TraceViewLine)
	{
		m_MousePos.MousePt.x = MousePt.x;
		m_MousePos.MousePt.y = MousePt.y;
		m_LButtonDown = TRUE;
	}
}

void CIdaView::OnViewMouseMove(LPARAM lParam,WPARAM wParam)
{
	RECT rcClient;
	POINT MousePt;
	int RoomedId;
	int PosHorz;
	int ColMax;
	int ColCur;
	int PrePosX;
	int PreColumnWidth;
	int PosMulX;
	HCURSOR MouseCursor;
	ULONG TrackMouseView;

	GetCursorPos(&MousePt);
	ScreenToClient(m_hWnd, &MousePt);
	GetClientRect(m_hWnd, &rcClient);
	//SetCapture(m_hWnd);
	TrackMouseView = 0;
	PrePosX = 0;
	PreColumnWidth = 0;
	RoomedId = IsZoomed(m_hWnd);

	TRACKMOUSEEVENT tme;     
	tme.cbSize = sizeof(TRACKMOUSEEVENT); //监控鼠标离开     
	tme.dwFlags = TME_LEAVE/*|TME_HOVER*/;     
	tme.hwndTrack = m_hWnd;  
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);

	MouseCursor = CpuViewTraceLoadCursor(MousePt,TrackMouseView);

	{
		if (m_LButtonDown == TRUE)
		{
			if ( m_ViewPos[RoomedId].TraceViewLine == 1 )
			{			
				ULONG GapSpaceValue = abs(MousePt.x - m_MousePos.MousePt.x);

				if (MousePt.x > m_MousePos.MousePt.x)
				{
					m_ViewPos[RoomedId].AsmView.left += GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.right += GapSpaceValue;
					m_ViewPos[RoomedId].DataView.right	  += GapSpaceValue;
				}
				else if(MousePt.x < m_MousePos.MousePt.x)
				{
					m_ViewPos[RoomedId].AsmView.left -= GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.right -= GapSpaceValue;
					m_ViewPos[RoomedId].DataView.right	  -= GapSpaceValue;
				}

				m_MousePos.MousePt.x = MousePt.x;
				m_MousePos.MousePt.y = MousePt.y;

				MoveWindow(
					m_ReverseView,
					m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.top,
					m_ViewPos[RoomedId].AsmView.right - m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.bottom - m_ViewPos[RoomedId].AsmView.top,			
					FALSE);

				MoveWindow(
					m_FileHeaderView,
					m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.top,
					m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
					FALSE);

				MoveWindow(
					m_HexOtcDataView,
					m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.top,
					m_ViewPos[RoomedId].DataView.right - m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.bottom - m_ViewPos[RoomedId].DataView.top,			
					FALSE);			
			}
			else if ( m_ViewPos[RoomedId].TraceViewLine == 2 )
			{			
				ULONG GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);

				if (MousePt.y > m_MousePos.MousePt.y)
				{
// 					if (MousePt.y > (m_ViewPos[RoomedId].AddressView.bottom - MIN_VIEW_VIDTH_HEIGHT))
// 					{
// 						MousePt.y = (m_ViewPos[RoomedId].AddressView.bottom - MIN_VIEW_VIDTH_HEIGHT);
// 						GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);
// 					}

					m_ViewPos[RoomedId].AddressView.bottom		+= GapSpaceValue;
					m_ViewPos[RoomedId].DataView.top		+= GapSpaceValue;
				}
				else if(MousePt.y < m_MousePos.MousePt.y)
				{
// 					if (MousePt.y < (m_ViewPos[RoomedId].AddressView.bottom - 8*MIN_VIEW_VIDTH_HEIGHT))
// 					{
// 						MousePt.y = (m_ViewPos[RoomedId].AddressView.bottom - 8*MIN_VIEW_VIDTH_HEIGHT);
// 						GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);
// 					}

					m_ViewPos[RoomedId].AddressView.bottom	-= GapSpaceValue;
					m_ViewPos[RoomedId].DataView.top		-= GapSpaceValue;
				}

				m_MousePos.MousePt.x = MousePt.x;
				m_MousePos.MousePt.y = MousePt.y;

				MoveWindow(
					m_HexOtcDataView,
					m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.top,
					m_ViewPos[RoomedId].DataView.right - m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.bottom - m_ViewPos[RoomedId].DataView.top,			
					FALSE);

				MoveWindow(
					m_FileHeaderView,
					m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.top,
					m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
					FALSE);
			}
					
			if (m_ViewPos[RoomedId].TraceViewLine == 1 || m_ViewPos[RoomedId].TraceViewLine == 3)
			{
				MouseCursor = LoadCursor(0,IDC_SIZEWE);
			}
			else
			{
				MouseCursor = LoadCursor(0,IDC_SIZENS);
			}
			m_MousePos.MousePt.x = MousePt.x;
			m_MousePos.MousePt.y = MousePt.y;
			RedrawWindow(m_hWnd,0,0,RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);

			TrackMouseView = 1;
		}

	}

__CONTINUE:
	if (TrackMouseView == 0)
	{
		MouseCursor = LoadCursor(0,IDC_ARROW);
	}

	SetCursor(MouseCursor);
}

void CIdaView::OnViewSize(LPARAM lParam,WPARAM wParam)
{
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	InflateRect(&rcClient,0 - VIEW_FRAME_LINEE_WIDTH,0 - VIEW_FRAME_LINEE_WIDTH);

	BOOL RoomedId = IsZoomed(m_hWnd);

	int PosLeft		= rcClient.left;
	int PosHMiddle	= rcClient.left + ((float)rcClient.right - (float)rcClient.left)*(0.40f);
	int PosVMiddle  = (rcClient.bottom - rcClient.top)/2;

	int SizeA = 4;
	int SizeB = 10;

	int SizeC = 4;
	int SizeD = 10;

	if (RoomedId)
	{
		SizeA = 4;
		SizeB = 10;
	}

	BOOL bMoveRgith		= m_ViewPos[RoomedId].ViewClient.right		!= rcClient.right;
	BOOL bMoveBottom	= m_ViewPos[RoomedId].ViewClient.bottom	!= rcClient.bottom;

	int ViewRight = rcClient.left + (rcClient.right - rcClient.left)*SizeA/SizeB;
	int LeftViewMiddle = rcClient.top + (rcClient.bottom - rcClient.top)*1/2;
	
	if (m_ViewPos[RoomedId].bInit == 0)
	{
		m_ViewPos[RoomedId].bInit = TRUE;
		bMoveRgith = TRUE;
		bMoveBottom = TRUE;

	}		

	if ( bMoveRgith && bMoveBottom )
	{		
		m_ViewPos[RoomedId].ViewClient.left		= rcClient.left;
		m_ViewPos[RoomedId].ViewClient.right	= rcClient.right;
		m_ViewPos[RoomedId].ViewClient.top		= rcClient.top;
		m_ViewPos[RoomedId].ViewClient.bottom	= rcClient.bottom;

		m_ViewPos[RoomedId].AddressView.left	= rcClient.left;
		m_ViewPos[RoomedId].AddressView.top		= m_ViewPos[RoomedId].ViewClient.top;
		m_ViewPos[RoomedId].AddressView.right	= ViewRight;
		m_ViewPos[RoomedId].AddressView.bottom	= m_ViewPos[RoomedId].AddressView.top + LeftViewMiddle;
		
		m_ViewPos[RoomedId].DataView.left = rcClient.left;
		m_ViewPos[RoomedId].DataView.right = ViewRight;
		m_ViewPos[RoomedId].DataView.top  = m_ViewPos[RoomedId].AddressView.bottom + VIEW_FRAME_LINEE_WIDTH;
		m_ViewPos[RoomedId].DataView.bottom = rcClient.bottom;

		m_ViewPos[RoomedId].AsmView.left	= m_ViewPos[RoomedId].DataView.right + VIEW_FRAME_LINEE_WIDTH;
		m_ViewPos[RoomedId].AsmView.top		= rcClient.top;
		m_ViewPos[RoomedId].AsmView.right	= rcClient.right;
		m_ViewPos[RoomedId].AsmView.bottom = rcClient.bottom;		
		
	}
// 	else if(bMoveRgith)
// 	{
// 		m_ViewPos[RoomedId].ViewClient.right	= rcClient.right;
// 		m_ViewPos[RoomedId].AsmView.right	= TopViewRight;
// 		m_ViewPos[RoomedId].AddressView.right = m_ViewPos[RoomedId].AsmView.right;
// 		m_ViewPos[RoomedId].DataView.right = BottomViewRight;
// 
// 	}
// 	else if(bMoveBottom)
// 	{
// 		m_ViewPos[RoomedId].AsmView.bottom = TopViewBottom - Ratio*ADDRESS_DATA_SIZE_ITEM;
// 		m_ViewPos[RoomedId].AddressView.top  = m_ViewPos[RoomedId].AsmView.bottom + VIEW_DOCK_LINEE_WIDTH;
// 		m_ViewPos[RoomedId].AddressView.bottom = m_ViewPos[RoomedId].AddressView.top + Ratio*ADDRESS_DATA_SIZE_ITEM;
// 
// 	}

	m_ViewPos[RoomedId].ViewClient.left		= rcClient.left;
	m_ViewPos[RoomedId].ViewClient.right	= rcClient.right;
	m_ViewPos[RoomedId].ViewClient.top		= rcClient.top;
	m_ViewPos[RoomedId].ViewClient.bottom	= rcClient.bottom;

	m_ViewPos[RoomedId].AddressView.left	= rcClient.left;
	m_ViewPos[RoomedId].AddressView.top		= m_ViewPos[RoomedId].ViewClient.top;
	m_ViewPos[RoomedId].AddressView.right	= ViewRight;
	m_ViewPos[RoomedId].AddressView.bottom	= m_ViewPos[RoomedId].AddressView.top + LeftViewMiddle;

	m_ViewPos[RoomedId].DataView.left = rcClient.left;
	m_ViewPos[RoomedId].DataView.right = ViewRight;
	m_ViewPos[RoomedId].DataView.top  = m_ViewPos[RoomedId].AddressView.bottom + VIEW_FRAME_LINEE_WIDTH;
	m_ViewPos[RoomedId].DataView.bottom = rcClient.bottom;

	m_ViewPos[RoomedId].AsmView.left	= m_ViewPos[RoomedId].DataView.right + VIEW_FRAME_LINEE_WIDTH;
	m_ViewPos[RoomedId].AsmView.top		= rcClient.top;
	m_ViewPos[RoomedId].AsmView.right	= rcClient.right;
	m_ViewPos[RoomedId].AsmView.bottom = rcClient.bottom;

	MoveWindow(
		m_FileHeaderView,
		m_ViewPos[RoomedId].AddressView.left,
		m_ViewPos[RoomedId].AddressView.top,
		m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
		m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
		TRUE);
	
	MoveWindow(
		m_ReverseView,
		m_ViewPos[RoomedId].AsmView.left,
		m_ViewPos[RoomedId].AsmView.top,
		m_ViewPos[RoomedId].AsmView.right - m_ViewPos[RoomedId].AsmView.left,
		m_ViewPos[RoomedId].AsmView.bottom - m_ViewPos[RoomedId].AsmView.top,			
		TRUE);

	MoveWindow(
		m_HexOtcDataView,
		m_ViewPos[RoomedId].DataView.left,
		m_ViewPos[RoomedId].DataView.top,
		m_ViewPos[RoomedId].DataView.right - m_ViewPos[RoomedId].DataView.left,
		m_ViewPos[RoomedId].DataView.bottom - m_ViewPos[RoomedId].DataView.top,			
		TRUE);

	RedrawWindow(m_hWnd,0,0,RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);
}

HCURSOR CIdaView::CpuViewTraceLoadCursor(
								POINT& MousePt,
								ULONG& TraceViewLine)
{
	HCURSOR CurrentCursor;

	int RoomedId = IsZoomed(m_hWnd);

	if( MousePt.x >= m_ViewPos[RoomedId].AddressView.right &&
		MousePt.x <= m_ViewPos[RoomedId].AsmView.left &&
		MousePt.y >= (m_ViewPos[RoomedId].ViewClient.top) &&
		MousePt.y <= m_ViewPos[RoomedId].ViewClient.bottom)
	{
		TraceViewLine = 1;
	}
	else if( MousePt.x > ( m_ViewPos[RoomedId].AddressView.left - VIEW_FRAME_LINEE_WIDTH) &&
		MousePt.x < m_ViewPos[RoomedId].AddressView.right &&
		MousePt.y >= m_ViewPos[RoomedId].AddressView.bottom &&
		MousePt.y <= m_ViewPos[RoomedId].DataView.top)
	{
		TraceViewLine = 2;
	}
	else
	{
		CurrentCursor = ::LoadCursor(0,IDC_ARROW);
	}

	if (TraceViewLine == 1 || TraceViewLine == 3 )
	{
		CurrentCursor = ::LoadCursor(0,IDC_SIZEWE);
	}
	else if(TraceViewLine == 2 || TraceViewLine == 4 )
	{
		CurrentCursor = ::LoadCursor(0,IDC_SIZENS);
	}

	return CurrentCursor;
}


UINT CIdaView::ShowPeFileIdaInfo(void* pIdaView)
{
	/*
	CZ0BTDisasm Disassem;

	HANDLE hFile = CreateFile(*((CIdaView*)pIdaView), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER FileSize;
		FileSize.LowPart = GetFileSize(hFile,(LPDWORD)&FileSize.HighPart);

		if (FileSize.LowPart)
		{
			SetFilePointer(hFile,0,0,FILE_BEGIN);
			PBYTE FileBytes = new BYTE[FileSize.LowPart];
			if (FileBytes)
			{
				ULONG ReadLength = 0;
				if (ReadFile(hFile,FileBytes,FileSize.LowPart,&ReadLength,NULL))
				{
					LPTSTR FileName = PathFindFileName(*((CIdaView*)pIdaView));
					if (!((CIdaView*)pIdaView)->ShowFileIdaInfo(FileBytes,ReadLength,FileName))
					{

					}

				}

				delete[] FileBytes;
				FileBytes = NULL;
			}
		}
		CloseHandle(hFile);
	}
	*/
	LPTSTR FileName = (*((CIdaView*)pIdaView));
	if (!((CIdaView*)pIdaView)->ShowFileIdaInfo(FileName))
	{

	}
	

	return TRUE;
}

void CIdaView::ShowIdaView(LPTSTR PathFile)
{
	StringCchCopy(m_FilePath,MAX_PATH,PathFile);
	_beginthreadex(NULL, 0, ShowPeFileIdaInfo, this, 0, NULL); 
}

void CIdaView::ShowDosHeaderInfo(
						ULONG_PTR& Address,
						LPTSTR HeaderName,
						int Bit,
						ULONG Values,
						LPTSTR lpMagic,
						COLORREF ColorShow,						
						LPTSTR lpNote,
						int nIconType1,
						int nIconType2,
						int nIconType3,
						BOOL bIsX64)
{
	/*
	TCHAR szAddress[64] = {0};

	if (bIsX64)
	{
		StringCchPrintf(
			szAddress,
			64,
			_T(" %s:%016llX"),
			HeaderName,
			Address);	
	}
	else
	{
		StringCchPrintf(
			szAddress,
			64,
			_T(" %s:%08X"),
			HeaderName,
			Address);	
	}

#ifdef OLD_ITEM_BODY			
	int nItem = m_pDisAsmView.GetItemCount();
	m_pDisAsmView.InsertItem(nItem,Address,szAddress);
#else
	PCustomItemHead pCurItem = m_pDisAsmView.InsertItem(Address,szAddress);

	if (!pCurItem)
	{
		return;
	}

#endif

	
	if (Bit == 2 )
	{
		StringCchPrintf( szAddress,64,_T(" %04X"),Values & 0xFFFF);
	}
	else
	{
		if (bIsX64)
		{
			StringCchPrintf( szAddress,64,_T(" %016llX"),Values);
		}
		else
		{
			StringCchPrintf( szAddress,64,_T(" %08X"),Values);
		}
		
	}
#ifdef OLD_ITEM_BODY	
	m_pDisAsmView.InsertAddressColumn(nItem,3,szAddress);
	m_pDisAsmView.SetAddressColumnIcon(nItem,0,nIconType1,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(nItem,1,nIconType2,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(nItem,2,nIconType3,FUNCTION_HEADER_COLOR);
#else
	m_pDisAsmView.InsertAddressColumn(pCurItem,3,szAddress);
	m_pDisAsmView.SetAddressColumnIcon(pCurItem,0,nIconType1,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(pCurItem,1,nIconType2,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(pCurItem,2,nIconType3,FUNCTION_HEADER_COLOR);
#endif
	Address += Bit;

	tagOption * Option = new tagOption[1];
	if (!Option)
	{
		return;
	}

	ZeroMemory(Option,sizeof(tagOption));
	for ( int i = 0 ; i < 1 ; i++ )
	{
		Option[i].FontBgColor = -1;				
		Option[i].NorFontColor = ColorShow;
		Option[i].SelFontColor = ColorShow;

		
		Option[i].lpText = new TCHAR[lstrlen(lpMagic) + 1];
		if ( Option[i].lpText )
		{
			StringCchCopy( Option[i].lpText,lstrlen(lpMagic) + 1,lpMagic );
			Option[i].TextSize = GetTextSize(lpMagic);
		}
		
		Option[i].HasNonFocus = FALSE;

	}
#ifdef OLD_ITEM_BODY
	m_pDisAsmView.SetColumnItem(nItem,2,0,Option,1);
#else
	m_pDisAsmView.SetColumnItem(pCurItem,2,0,Option,1);
#endif
	
	if (lpNote)
	{
		tagOption* pNoteOption = new tagOption[2];
		if (pNoteOption)
		{
			ZeroMemory(pNoteOption,sizeof(tagOption)*2);

			for ( int i = 0 ; i < 1 ; i++ )
			{
				pNoteOption[i].FontBgColor = -1;				
				pNoteOption[i].NorFontColor = VIEW_NOTE_COLOR;
				pNoteOption[i].SelFontColor = VIEW_NOTE_COLOR;
				
				pNoteOption[i].lpText = new TCHAR[lstrlen(lpNote) + 1];
				if ( pNoteOption[i].lpText )
				{
					StringCchCopy( pNoteOption[i].lpText,lstrlen(lpNote) + 1,lpNote );
					pNoteOption[i].TextSize = GetTextSize(lpNote);
				}

				pNoteOption[i].HasNonFocus = FALSE;

			}
#ifdef OLD_ITEM_BODY
			m_pDisAsmView.SetColumnItem(nItem,3,0,pNoteOption,2);
#else
			m_pDisAsmView.SetColumnItem(pCurItem,3,0,pNoteOption,2);
#endif
		}		
	}

	*/
}

void CIdaView::ShowSectionInfo(
						ULONG_PTR& Address,
						LPTSTR HeaderName,
						ULONG Values,
						ULONG VaSize,
						const char* lpMagic,
						COLORREF ColorShow,
						LPTSTR lpNote,
						int nIconType1,
						int nIconType2,
						int nIconType3,
						BOOL bIsX64)
{
	/*
	TCHAR szAddress[64] = {0};

	if (bIsX64)
	{
		StringCchPrintf(
			szAddress,
			64,
			_T(" %s:%016llX"),
			HeaderName,
			Address);		
	}
	else
	{
		StringCchPrintf(
			szAddress,
			64,
			_T(" %s:%08X"),
			HeaderName,
			Address);	
	}

#ifdef OLD_ITEM_BODY				
	int nItem = m_pDisAsmView.GetItemCount();
	m_pDisAsmView.InsertItem(nItem,Address,szAddress);	

#else
	PCustomItemHead pCurItem = m_pDisAsmView.InsertItem(Address,szAddress);

	if (!pCurItem)
	{
		return;
	}	

#endif
	
	if (bIsX64)
	{
		StringCchPrintf( szAddress,64,_T(" RVA(%016X):RVS(%08X)"),Values,VaSize);
	}
	else
	{
		StringCchPrintf( szAddress,64,_T(" RVA(%08X):RVS(%08X)"),Values,VaSize);
	}
		
#ifdef OLD_ITEM_BODY
	m_pDisAsmView.InsertAddressColumn(nItem,3,szAddress);
	m_pDisAsmView.SetAddressColumnIcon(nItem,0,nIconType1,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(nItem,1,nIconType2,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(nItem,2,nIconType3,FUNCTION_HEADER_COLOR);

#else

	m_pDisAsmView.InsertAddressColumn(pCurItem,3,szAddress);
	m_pDisAsmView.SetAddressColumnIcon(pCurItem,0,nIconType1,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(pCurItem,1,nIconType2,FUNCTION_HEADER_COLOR);
	m_pDisAsmView.SetAddressColumnIcon(pCurItem,2,nIconType3,FUNCTION_HEADER_COLOR);
#endif
	Address += sizeof(IMAGE_SECTION_HEADER);

	tagOption * Option = new tagOption[1];
	if (!Option)
	{
		return;
	}

	ZeroMemory(Option,sizeof(tagOption));
	for ( int i = 0 ; i < 1 ; i++ )
	{
		Option[i].FontBgColor = -1;				
		Option[i].NorFontColor = ColorShow;
		Option[i].SelFontColor = ColorShow;


		Option[i].lpText = new TCHAR[strlen(lpMagic) + 1];
		if ( Option[i].lpText )
		{
			Option[i].TextSize = strlen(lpMagic);
			int j = 0;
			for (  ; j < Option[i].TextSize;j++)
			{
				Option[i].lpText[j] = lpMagic[j];
			}

			Option[i].lpText[j] = 0;
		}

		Option[i].HasNonFocus = FALSE;

	}
#ifdef OLD_ITEM_BODY
	m_pDisAsmView.SetColumnItem(nItem,2,0,Option,1);

#else
	m_pDisAsmView.SetColumnItem(pCurItem,2,0,Option,1);

#endif
	
	if (lpNote)
	{
		tagOption* pNoteOption = new tagOption[2];
		if (pNoteOption)
		{
			ZeroMemory(pNoteOption,sizeof(tagOption)*2);

			for ( int i = 0 ; i < 1 ; i++ )
			{
				pNoteOption[i].FontBgColor = -1;				
				pNoteOption[i].NorFontColor = VIEW_NOTE_COLOR;
				pNoteOption[i].SelFontColor = VIEW_NOTE_COLOR;

				pNoteOption[i].lpText = new TCHAR[lstrlen(lpNote) + 1];
				if ( pNoteOption[i].lpText )
				{
					StringCchCopy( pNoteOption[i].lpText,lstrlen(lpNote) + 1,lpNote );
					pNoteOption[i].TextSize = GetTextSize(lpNote);
				}

				pNoteOption[i].HasNonFocus = FALSE;

			}
#ifdef OLD_ITEM_BODY
			m_pDisAsmView.SetColumnItem(nItem,3,0,pNoteOption,2);
#else
			m_pDisAsmView.SetColumnItem(pCurItem,3,0,pNoteOption,2);
#endif
		}
	}
	*/
}

BOOL CIdaView::ShowFileIdaInfo(LPTSTR FilePath)
{
	if(m_FileInfo.ReadFile(FilePath))
	{
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PBYTE		pBase = 0 ;
		uint64_t	FileSize = 0;

		m_FileInfo.GetFileData(pBase,FileSize);
		g_mainStatusBar.ShowStatusInfo(_T("  读取PE文件....."),0.01f);
		if ( FileSize > ( sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS))&& pBase )
		{
			BOOL IsAmd64 = FALSE;
			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBase;
			g_mainStatusBar.ShowStatusInfo(_T("  解析PE头信息....."),0.02f);
			if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE )
			{
				pNtHeader = (PIMAGE_NT_HEADERS)&((const unsigned char *)(pBase))[pDosHeader->e_lfanew];
				if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
				{
					ULONG_PTR nAddress = 0;
					PIMAGE_FILE_HEADER pFileHeader = &pNtHeader->FileHeader;
					PIMAGE_OPTIONAL_HEADER32	pOption = (PIMAGE_OPTIONAL_HEADER32)&pNtHeader->OptionalHeader;
					PIMAGE_OPTIONAL_HEADER64    pOption64 = (PIMAGE_OPTIONAL_HEADER64)&pNtHeader->OptionalHeader;

					PIMAGE_DATA_DIRECTORY	pDataDirectory = (PIMAGE_DATA_DIRECTORY)&pNtHeader->OptionalHeader.DataDirectory;
					PIMAGE_SECTION_HEADER	pSection = (PIMAGE_SECTION_HEADER)( (PBYTE)pNtHeader + sizeof(IMAGE_NT_HEADERS));

					TCHAR szAddress[64] = {0};				
					if(pFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64 ||
						pFileHeader->Machine == IMAGE_FILE_MACHINE_IA64)
					{
						IsAmd64 = TRUE;
					}

					uint64_t ImageBase = pOption->ImageBase;
					uint64_t ImageSize = pOption->SizeOfImage;
					uint64_t ImageImportRva = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;			
					int		 ImageImportSize = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
					uint64_t ImageRelocationRva = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
					int      ImageRelocationSize = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

					if (IsAmd64)
					{
						ImageBase = pOption64->ImageBase;
						ImageSize = pOption64->SizeOfImage;
						ImageImportRva			= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
						ImageImportSize			= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

						ImageRelocationRva	= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
						ImageRelocationSize		= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
						pSection = (PIMAGE_SECTION_HEADER)( (PBYTE)pNtHeader + sizeof(IMAGE_NT_HEADERS64));;
					}

					LPTSTR FileName = PathFindFileName(FilePath);
					g_mainStatusBar.ShowStatusInfo(_T("  设置PE符号....."),0.03f);
					//设置符号
					SetSymbol(ImageBase,FileName);
					AddSymbolMoudle(ImageBase,ImageSize,FileName);
					g_mainStatusBar.ShowStatusInfo(_T("  解析导入表....."),0.035f);
					GetImportTable(pBase,ImageBase,ImageImportRva,pSection,pNtHeader->FileHeader.NumberOfSections,IsAmd64);
					//GetRelocationTable(pBase,ImageBase,ImageRelocationRva,pSection,pNtHeader->FileHeader.NumberOfSections,IsAmd64);
					m_FileHeaderView.SetWinExtFileData(pBase,FileSize,FileName);
					m_HexOtcDataView.SetMode(IsAmd64);
					m_HexOtcDataView.SetMoudleBaseAddress((UINT64)pBase,FileSize);
					m_ReverseView.SetSectionCount(pNtHeader->FileHeader.NumberOfSections);
					m_HexOtcDataView.SetSectionCount(pNtHeader->FileHeader.NumberOfSections +1);
					m_HexOtcDataView.SetSectionInfo(
						0,
						(uint64_t)pBase,
						FileSize,
						ImageBase,
						ImageSize);
					m_ReverseView.ReadData(
						pBase,
						FileSize,
						kReverseReadFromMem);
					m_ReverseView.SetMode(IsAmd64?64:32);
					m_ReverseView.SetImageBase(ImageBase);
					m_ReverseView.SetResolverSymbol(CIdaView::ResolverSymbol,this);

					g_mainStatusBar.ShowStatusInfo(_T("  解析PE节汇编....."),0.10f);
					float flLeftProcess = 0.90/(float)pNtHeader->FileHeader.NumberOfSections;
					for ( int i = 0 ; i < pNtHeader->FileHeader.NumberOfSections;i++)
					{
						uint32_t Characterristics = pSection[i].Characteristics;
						g_mainStatusBar.ShowStatusInfo(_T("  解析PE节汇编....."),(0.10f + flLeftProcess*(float)i));
						//if (IMAGE_SCN_CNT_CODE & Characterristics 
							// 						||
							// 						( (IMAGE_SCN_CNT_INITIALIZED_DATA & Characterristics) && !(IMAGE_SCN_MEM_DISCARDABLE & Characterristics)) ||
							// 						( (IMAGE_SCN_CNT_UNINITIALIZED_DATA & Characterristics) && !(IMAGE_SCN_MEM_DISCARDABLE & Characterristics))
							//)
						{
							m_ReverseView.SetSectionInfo(
													i,
													(uint64_t)pBase + pSection[i].PointerToRawData,
													pSection[i].SizeOfRawData,
													ImageBase + pSection[i].VirtualAddress,
													pSection[i].Misc.VirtualSize);
							m_HexOtcDataView.SetSectionInfo(
													i+1,
													(uint64_t)pBase + pSection[i].PointerToRawData,
													pSection[i].SizeOfRawData,
													ImageBase + pSection[i].VirtualAddress,
													pSection[i].Misc.VirtualSize);

							m_ReverseView.DecodeFromMem(
												pBase + pSection[i].PointerToRawData,
												pSection[i].SizeOfRawData,
												flLeftProcess,
												ImageBase + pSection[i].VirtualAddress,
												FALSE);
// 							if (Disassem.Disassem(
// 								pBase + pSection[i].PointerToRawData,
// 								pSection[i].SizeOfRawData,
// 								ImageBase + pSection[i].VirtualAddress,
// 								(CClassViewBase *)this,
// 								szSectionName))
// 							{
// 
// 							}
						}					
					}

					g_mainStatusBar.ShowStatusInfo(_T("  分析完毕....."),100.0f);
				}
				else
				{
					g_mainStatusBar.ShowStatusInfo(_T("  非Windows可执行文件....."),100.0f);
				}
			}
			else
			{
				g_mainStatusBar.ShowStatusInfo(_T("  非Windows可执行文件....."),100.0f);
			}
		}
		else
		{
			g_mainStatusBar.ShowStatusInfo(_T("  读取PE文件错误....."),100.0f);
		}
	}
	/*
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	if ( FileSize > ( sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS))&& pBase )
	{
		BOOL IsAmd64 = FALSE;
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBase;
		if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE )
		{
			pNtHeader = (PIMAGE_NT_HEADERS)&((const unsigned char *)(pBase))[pDosHeader->e_lfanew];
			if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
			{
				int nItem = m_pDisAsmView.GetItemCount();
				ULONG_PTR nAddress = 0;
				PIMAGE_FILE_HEADER pFileHeader = &pNtHeader->FileHeader;
				PIMAGE_OPTIONAL_HEADER32	pOption = (PIMAGE_OPTIONAL_HEADER32)&pNtHeader->OptionalHeader;
				PIMAGE_OPTIONAL_HEADER64    pOption64 = (PIMAGE_OPTIONAL_HEADER64)&pNtHeader->OptionalHeader;
				PIMAGE_DATA_DIRECTORY	pDataDirectory = (PIMAGE_DATA_DIRECTORY)&pNtHeader->OptionalHeader.DataDirectory;
				PIMAGE_SECTION_HEADER	pSection = (PIMAGE_SECTION_HEADER)( (PBYTE)pNtHeader + sizeof(IMAGE_NT_HEADERS));
				TCHAR szAddress[64] = {0};				
				if(pFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64 ||
				pFileHeader->Machine == IMAGE_FILE_MACHINE_IA64)
				{
					IsAmd64 = TRUE;
				}

				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_magic),pDosHeader->e_magic,_T("e_magic"),JL_FONT_INS_COLOR,_T("PE头标志"),FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cblp),pDosHeader->e_cblp,_T("e_cblp"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cp),pDosHeader->e_cp,_T("e_cp"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_crlc),pDosHeader->e_crlc,_T("e_crlc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cparhdr),pDosHeader->e_cparhdr,_T("e_cparhdr"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_minalloc),pDosHeader->e_minalloc,_T("e_minalloc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_maxalloc),pDosHeader->e_maxalloc,_T("e_maxalloc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_ss),pDosHeader->e_ss,_T("e_ss"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_sp),pDosHeader->e_sp,_T("e_sp"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_csum),pDosHeader->e_csum,_T("e_csum"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_ip),pDosHeader->e_ip,_T("e_ip"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cs),pDosHeader->e_cs,_T("e_cs"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_lfarlc),pDosHeader->e_lfarlc,_T("e_lfarlc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_ovno),pDosHeader->e_ovno,_T("e_ovno"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[0]),pDosHeader->e_res[0],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[1]),pDosHeader->e_res[1],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[2]),pDosHeader->e_res[2],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[3]),pDosHeader->e_res[3],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_oemid),pDosHeader->e_oemid,_T("e_oemid"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_oeminfo),pDosHeader->e_oeminfo,_T("e_oeminfo"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[0]),pDosHeader->e_res2[0],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[1]),pDosHeader->e_res2[1],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[2]),pDosHeader->e_res2[2],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[3]),pDosHeader->e_res2[3],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[4]),pDosHeader->e_res2[4],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[5]),pDosHeader->e_res2[5],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[6]),pDosHeader->e_res2[6],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[7]),pDosHeader->e_res2[7],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[8]),pDosHeader->e_res2[8],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[9]),pDosHeader->e_res2[9],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_lfanew),pDosHeader->e_lfanew,_T("e_lfanew"),JL_FONT_INS_COLOR,_T("NT头偏移"),FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
				nAddress = pDosHeader->e_lfanew;
				

				ShowDosHeaderInfo(nAddress,_T(".NtHeader"),sizeof(pNtHeader->Signature),pNtHeader->Signature,_T("Signature"),JL_FONT_INS_COLOR,_T("NT头标志"),0,0,FUNCTION_HREADER_FLAGS,IsAmd64);

				TCHAR szCpuInfo[30] = {0};
				switch (pFileHeader->Machine)
				{
				case IMAGE_FILE_MACHINE_I386:
					StringCchCopy(szCpuInfo,30,_T("Intel 386"));
					break;
				case IMAGE_FILE_MACHINE_R3000:
					StringCchCopy(szCpuInfo,30,_T("MIPS little"));
					break;
				case IMAGE_FILE_MACHINE_R4000:
					StringCchCopy(szCpuInfo,30,_T(" MIPS little"));
					break;
				case IMAGE_FILE_MACHINE_R10000:
					StringCchCopy(szCpuInfo,30,_T("MIPS little"));
					break;
				case IMAGE_FILE_MACHINE_WCEMIPSV2:
					StringCchCopy(szCpuInfo,30,_T("MIPS little V2"));
					break;
				case IMAGE_FILE_MACHINE_ALPHA:
					StringCchCopy(szCpuInfo,30,_T("Alpha_AXP"));
					break;
				case IMAGE_FILE_MACHINE_SH3:
					StringCchCopy(szCpuInfo,30,_T("SH3 little"));
					break;
				case IMAGE_FILE_MACHINE_ARM:
					StringCchCopy(szCpuInfo,30,_T("ARM Little"));
					break;
				case IMAGE_FILE_MACHINE_IA64:
					StringCchCopy(szCpuInfo,30,_T("Intel 64"));
					IsAmd64 = TRUE;
					break;
				case IMAGE_FILE_MACHINE_ALPHA64:
					StringCchCopy(szCpuInfo,30,_T("ALPHA64"));
					break;
				case IMAGE_FILE_MACHINE_AMD64:
					StringCchCopy(szCpuInfo,30,_T("AMD64 (K8)"));
					IsAmd64 = TRUE;
					break;
				default:
					StringCchCopy(szCpuInfo,30,_T("未知"));
				}

				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->Machine),pFileHeader->Machine,_T("Machine"),JL_FONT_INS_COLOR,szCpuInfo,FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->NumberOfSections),pFileHeader->NumberOfSections,_T("NumberOfSections"),FILE_HEADER_SHOW_COLOR,_T("节段数量"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->TimeDateStamp),pFileHeader->TimeDateStamp,_T("TimeDateStamp"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->PointerToSymbolTable),pFileHeader->PointerToSymbolTable,_T("PointerToSymbolTable"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->NumberOfSymbols),pFileHeader->NumberOfSymbols,_T("NumberOfSymbols"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->SizeOfOptionalHeader),pFileHeader->SizeOfOptionalHeader,_T("SizeOfOptionalHeader"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->Characteristics),pFileHeader->Characteristics,_T("Characteristics"),FILE_HEADER_SHOW_COLOR,NULL,FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);

				if (IsAmd64)
				{
#ifndef _WIN64
					TCHAR szAddressEntry[64] = {0};
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->Magic),pOption64->Magic,_T("Magic"),JL_FONT_INS_COLOR,_T("pe可选头标志"),FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorLinkerVersion),pOption64->MajorLinkerVersion,_T("MajorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MinorLinkerVersion),pOption64->MinorLinkerVersion,_T("MinorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfCode),pOption64->SizeOfCode,_T("SizeOfCode"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfInitializedData),pOption64->SizeOfInitializedData,_T("SizeOfInitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfUninitializedData),pOption64->SizeOfUninitializedData,_T("SizeOfUninitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("%s.%016llX"),FileName,pOption64->AddressOfEntryPoint + pOption64->ImageBase);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->AddressOfEntryPoint),pOption64->AddressOfEntryPoint,szAddressEntry,JL_FONT_INS_COLOR,_T("入口点地址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->BaseOfCode),pOption64->BaseOfCode,_T("BaseOfCode"),JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					//ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->BaseOfData),pOption64->BaseOfData,_T("BaseOfData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS);
					StringCchPrintf(szAddressEntry,64,_T("%s.%016llX"),FileName,pOption64->ImageBase);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->ImageBase),pOption64->ImageBase,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件基址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SectionAlignment),pOption64->SectionAlignment,_T("SectionAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->FileAlignment),pOption64->FileAlignment,_T("FileAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorOperatingSystemVersion),pOption64->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorOperatingSystemVersion),pOption64->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorImageVersion),pOption64->MajorImageVersion,_T("MajorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MinorImageVersion),pOption64->MinorImageVersion,_T("MinorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorSubsystemVersion),pOption64->MajorSubsystemVersion,_T("MajorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MinorSubsystemVersion),pOption64->MinorSubsystemVersion,_T("MinorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->Win32VersionValue),pOption64->Win32VersionValue,_T("Win32VersionValue"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("0x%016llX"),pOption64->SizeOfImage);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfImage),pOption64->SizeOfImage,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件大小"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfHeaders),pOption64->SizeOfHeaders,_T("SizeOfHeaders"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->CheckSum),pOption64->CheckSum,_T("CheckSum"),OPTION_HEADER_SHOW_COLOR,_T("镜像文件校验和"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->Subsystem),pOption64->Subsystem,_T("Subsystem"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->DllCharacteristics),pOption64->DllCharacteristics,_T("DllCharacteristics"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfStackReserve),pOption64->SizeOfStackReserve,_T("SizeOfStackReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfStackCommit),pOption64->SizeOfStackCommit,_T("SizeOfStackCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfHeapReserve),pOption64->SizeOfHeapReserve,_T("SizeOfHeapReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfHeapCommit),pOption64->SizeOfHeapCommit,_T("SizeOfHeapCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->LoaderFlags),pOption64->LoaderFlags,_T("LoaderFlags"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->NumberOfRvaAndSizes),pOption64->NumberOfRvaAndSizes,_T("NumberOfRvaAndSizes"),OPTION_HEADER_SHOW_COLOR,NULL,FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
#endif

				}
				else
				{
					TCHAR szAddressEntry[64] = {0};
					
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->Magic),pOption->Magic,_T("Magic"),JL_FONT_INS_COLOR,_T("pe可选头标志"),FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorLinkerVersion),pOption->MajorLinkerVersion,_T("MajorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MinorLinkerVersion),pOption->MinorLinkerVersion,_T("MinorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfCode),pOption->SizeOfCode,_T("SizeOfCode"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfInitializedData),pOption->SizeOfInitializedData,_T("SizeOfInitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfUninitializedData),pOption->SizeOfUninitializedData,_T("SizeOfUninitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("%s.%X"),FileName,pOption->AddressOfEntryPoint + pOption->ImageBase);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->AddressOfEntryPoint),pOption->AddressOfEntryPoint,szAddressEntry,JL_FONT_INS_COLOR,_T("入口点地址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->BaseOfCode),pOption->BaseOfCode,_T("BaseOfCode"),JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->BaseOfData),pOption->BaseOfData,_T("BaseOfData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("%s.%X"),FileName,pOption->ImageBase);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->ImageBase),pOption->ImageBase,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件基址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SectionAlignment),pOption->SectionAlignment,_T("SectionAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->FileAlignment),pOption->FileAlignment,_T("FileAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorOperatingSystemVersion),pOption->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorOperatingSystemVersion),pOption->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorImageVersion),pOption->MajorImageVersion,_T("MajorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MinorImageVersion),pOption->MinorImageVersion,_T("MinorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorSubsystemVersion),pOption->MajorSubsystemVersion,_T("MajorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MinorSubsystemVersion),pOption->MinorSubsystemVersion,_T("MinorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->Win32VersionValue),pOption->Win32VersionValue,_T("Win32VersionValue"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("0x%X"),pOption->SizeOfImage);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfImage),pOption->SizeOfImage,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件大小"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfHeaders),pOption->SizeOfHeaders,_T("SizeOfHeaders"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->CheckSum),pOption->CheckSum,_T("CheckSum"),OPTION_HEADER_SHOW_COLOR,_T("镜像文件校验和"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->Subsystem),pOption->Subsystem,_T("Subsystem"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->DllCharacteristics),pOption->DllCharacteristics,_T("DllCharacteristics"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfStackReserve),pOption->SizeOfStackReserve,_T("SizeOfStackReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfStackCommit),pOption->SizeOfStackCommit,_T("SizeOfStackCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfHeapReserve),pOption->SizeOfHeapReserve,_T("SizeOfHeapReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfHeapCommit),pOption->SizeOfHeapCommit,_T("SizeOfHeapCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->LoaderFlags),pOption->LoaderFlags,_T("LoaderFlags"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->NumberOfRvaAndSizes),pOption->NumberOfRvaAndSizes,_T("NumberOfRvaAndSizes"),OPTION_HEADER_SHOW_COLOR,NULL,FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);

				}
				
				ShowDosHeaderInfo(
							nAddress,
							_T(".Directory"),
							sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress),
							pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
							_T("Export Directory RVA"),
							JL_FONT_INS_COLOR,
							_T("导出表RVA"),
							FUCTION_BODY_HEADER_FLAGS,
							0,
							FUNCTION_HREADER_FLAGS,
							IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size,
					_T("Export Directory Size"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
					_T("Import Directory RVA"),
					JL_FONT_INS_COLOR,
					_T("导入表RVA"),FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size,
					_T("Import Directory Size"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress,
					_T("Resource Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("资源表RVA"),FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size,
					_T("Resource Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress,
					_T("Exception Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("异常链表RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size,
					_T("Exception Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress,
					_T("Security Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("安全属性节RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size,
					_T("Security Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,
					_T("Relocation Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("重定向表RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size,
					_T("Relocation Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress,
					_T("Debug Directory RVA"),
					PUSH_POP_FONT_INS_COLOR
					,_T("调试符号表RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size,
					_T("Debug Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress,
					_T("Architecture Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size,
					_T("Architecture Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress,
					_T("Reserved"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size,
					_T("Reserved"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress,
					_T("TLS Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size,
					_T("TLS Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress,
					_T("Configuration Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size,
					_T("Configuration Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress,
					_T("Bound Import Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size,
					_T("Bound Import Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress,
					_T("Import Address Table Directory RVA"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size,
					_T("Import Address Table Directory Size"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress,
					_T("Delay Import Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size,
					_T("Delay Import Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress,
					_T(".Net MetaData Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size,
					_T(".Net MetaData Directory Size"),
					PUSH_POP_FONT_INS_COLOR,
					NULL,
					FUCTION_RET_FLAGS,
					0,
					FUNCTION_HREADER_FLAGS,IsAmd64);				

				uint64_t ImageBase = pOption->ImageBase;
				uint64_t ImageSize = pOption->SizeOfImage;
				uint64_t ImageImportRva = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;			
				int		 ImageImportSize = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
				uint64_t ImageRelocationRva = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
				int      ImageRelocationSize = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
				if (IsAmd64)
				{
					ImageBase = pOption64->ImageBase;
					ImageSize = pOption64->SizeOfImage;
					ImageImportRva			= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
					ImageImportSize			= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

					ImageRelocationRva	= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
					ImageRelocationSize		= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
					pSection = (PIMAGE_SECTION_HEADER)( (PBYTE)pNtHeader + sizeof(IMAGE_NT_HEADERS64));;
				}

				nAddress = (ULONG_PTR)((UCHAR*)pSection - (UCHAR*)pBase);
				for ( int i = 0 ; i < pNtHeader->FileHeader.NumberOfSections;i++)
				{
					TCHAR szNote[64] = {0};
					StringCchPrintf(
									szNote,
									64,
									_T("RA:%08X RS:%08X %s:%08X"),
									pSection[i].PointerToRawData,
									pSection[i].SizeOfRawData,
									_T("属性"),
									pSection[i].Characteristics);
					int nType1 = 0;
					int nType2 = 0;
					int nType3 = 0;

					if ( i == 0)
					{
						nType1 = FUCTION_BODY_HEADER_FLAGS;
						nType3 = FUNCTION_HREADER_FLAGS;
					}
					else if ( i == (pNtHeader->FileHeader.NumberOfSections - 1))
					{
						nType1 = FUCTION_RET_FLAGS;
						nType3 = FUNCTION_HREADER_FLAGS;
					}
					else
					{
						nType1 = FUNCTION_BODY_FLAGS;
					}

					if ( pSection[i].VirtualAddress <= ImageImportRva &&
						ImageImportRva <= pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
					{
						ImageImportRva = ( ImageImportRva - pSection[i].VirtualAddress ) + pSection[i].PointerToRawData;
					}

					if ( pSection[i].VirtualAddress <= ImageRelocationRva &&
						ImageRelocationRva <= pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
					{
						ImageRelocationRva = ( ImageRelocationRva - pSection[i].VirtualAddress ) + pSection[i].PointerToRawData;
					}

					
					ShowSectionInfo(
						nAddress,
						_T(".Section"),
						pSection[i].VirtualAddress,
						pSection[i].Misc.VirtualSize,
						(const char*)pSection[i].Name,
						SECTION_NAME_SHOW_COLOR,
						szNote,
						nType1,
						nType2,
						nType3,
						IsAmd64);
				}

				
				//设置符号
				SetSymbol(ImageBase,FileName);
				AddSymbolMoudle(ImageBase,ImageSize,FileName);
				GetImportTable(pBase,ImageBase,ImageImportRva,pSection,pNtHeader->FileHeader.NumberOfSections,IsAmd64);
				GetRelocationTable(pBase,ImageBase,ImageRelocationRva,pSection,pNtHeader->FileHeader.NumberOfSections,IsAmd64);

				for ( int i = 0 ; i < pNtHeader->FileHeader.NumberOfSections;i++)
				{
					uint32_t Characterristics = pSection[i].Characteristics;

					if (IMAGE_SCN_CNT_CODE & Characterristics 
// 						||
// 						( (IMAGE_SCN_CNT_INITIALIZED_DATA & Characterristics) && !(IMAGE_SCN_MEM_DISCARDABLE & Characterristics)) ||
// 						( (IMAGE_SCN_CNT_UNINITIALIZED_DATA & Characterristics) && !(IMAGE_SCN_MEM_DISCARDABLE & Characterristics))
						)
					{
						CZ0BTDisasm Disassem;

						if (IsAmd64 )
						{
							Disassem.SetMode(64);
						}

						Disassem.SetMoudleInfo(ImageBase,ImageSize);
						Disassem.SetResolverSymbol(CIdaView::ResolverSymbol,this);
						Disassem.SetResolverSkipjmp(CIdaView::ReloveSkipJmp,this);
						TCHAR szSectionName[50] = {0};
						for ( int j = 0;j < IMAGE_SIZEOF_SHORT_NAME ; j++)
						{
							szSectionName[j] = pSection[i].Name[j];
						}

						if (Disassem.Disassem(
							pBase + pSection[i].PointerToRawData,
							pSection[i].SizeOfRawData,
							ImageBase + pSection[i].VirtualAddress,
							(CClassViewBase *)this,
							szSectionName))
						{

						}
					}					
				}
				
				
				m_pDisAsmView.InvalidateRect();

				return TRUE;
			}
		}
	}
	*/

	return FALSE;
}

uint64_t CIdaView::GetRwaDataOffset( PIMAGE_SECTION_HEADER pSection, int SectionNums, uint64_t VirtualAddr )
{

	uint64_t RawDataAddr = VirtualAddr;

	for ( int i = 0 ; i < SectionNums;i++)
	{
		if ( pSection[i].VirtualAddress <= RawDataAddr &&
			RawDataAddr <= pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
		{
			RawDataAddr = ( RawDataAddr - pSection[i].VirtualAddress ) + pSection[i].PointerToRawData;

			break;
		}
	}

	return RawDataAddr;
}

uint64_t CIdaView::GetVirtualOffset( PIMAGE_SECTION_HEADER pSection, int SectionNums, uint64_t RawAddr )
{

	uint64_t VirtualAddr = RawAddr;

	for ( int i = 0 ; i < SectionNums;i++)
	{
		if ( pSection[i].PointerToRawData <= VirtualAddr &&
			VirtualAddr <= pSection[i].PointerToRawData + pSection[i].Misc.VirtualSize)
		{
			VirtualAddr = ( VirtualAddr - pSection[i].PointerToRawData ) + pSection[i].VirtualAddress;

			break;
		}
	}

	return VirtualAddr;
}

void CIdaView::GetImportTable(PBYTE pBase,uint64_t ImageBase, uint64_t ImportTableRva,PIMAGE_SECTION_HEADER pSection,int SectionNums,BOOL bIsX64)
{
	PIMAGE_IMPORT_DESCRIPTOR ImportTableVa;

	if (!ImportTableRva)
	{
		return;
	}

	ImportTableVa = (PIMAGE_IMPORT_DESCRIPTOR)(pBase + GetRwaDataOffset( pSection,SectionNums,ImportTableRva));
	//uint64_t AddressOfData1 = NULL;
	uint64_t AddressOfData2 = NULL;
	PIMAGE_IMPORT_BY_NAME ImageImportByNameVa = NULL;
	uint64_t FirstThunk = GetRwaDataOffset(pSection,SectionNums,ImportTableVa->FirstThunk);
	uint64_t OriginalFirstThunk = GetRwaDataOffset(pSection,SectionNums,ImportTableVa->OriginalFirstThunk);
	uint64_t MoudleNameThunk = GetRwaDataOffset(pSection,SectionNums,ImportTableVa->Name);

	while( ImportTableVa && FirstThunk)
	{
		PIMAGE_THUNK_DATA32 pFTsIat32 = (PIMAGE_THUNK_DATA32)(pBase + GetRwaDataOffset(pSection,SectionNums,FirstThunk));
		PIMAGE_THUNK_DATA64 pFTsIat64 = (PIMAGE_THUNK_DATA64)(pBase + FirstThunk);

		PIMAGE_THUNK_DATA32 pOFTs32 = (PIMAGE_THUNK_DATA32)(pBase + OriginalFirstThunk);
		PIMAGE_THUNK_DATA64 pOFTs64 = (PIMAGE_THUNK_DATA64)(pBase + OriginalFirstThunk);

		
		TCHAR szMoudleName[200] = {0};

		if (ImportTableVa == 0)
		{
			break;
		}

		if (bIsX64)
		{
			if (FirstThunk)
			{
				AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pFTsIat64->u1.AddressOfData);
			}
			else
			{
				AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pOFTs64->u1.AddressOfData);
			}
			
		}
		else
		{
			if (FirstThunk)
			{
				AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pFTsIat32->u1.AddressOfData);
			}
			else
			{
				AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pOFTs32->u1.AddressOfData);
			}
			
		}

		PUCHAR pMoudleName = (PUCHAR) (pBase + MoudleNameThunk);
		if (pMoudleName)
		{
			for (int k = 0;k < 200 && pMoudleName[k];k++)
			{
				szMoudleName[k] = pMoudleName[k];
			}
		}

		while(AddressOfData2)
		{
			uint64_t AddressOffset = (GetVirtualOffset(pSection,SectionNums,(PBYTE)pFTsIat32 - pBase)) + ImageBase;
			if (bIsX64)
			{
				AddressOffset = (GetVirtualOffset(pSection,SectionNums,(PBYTE)pFTsIat64 - pBase)) + ImageBase;
			}

			ImageImportByNameVa = (PIMAGE_IMPORT_BY_NAME)( pBase + AddressOfData2);
			TCHAR szImportName[200] = { 0};
			if (IsBadReadPtr(ImageImportByNameVa,4) == TRUE)
			{
				StringCchPrintf(szImportName,200,_T("%s.Ordinal:%08d"),szMoudleName,AddressOfData2 & 0xFFFF);
			}
			else
			{
				int j = 0;
				StringCchPrintf(szImportName,200,_T("%s."),szMoudleName);
				int nOffset = lstrlen(szImportName);
				while ( ImageImportByNameVa->Name && ImageImportByNameVa->Name[j] )
				{
					if ( ( j + nOffset ) >= 200 )
					{
						break;
					}
					szImportName[j + nOffset] = ImageImportByNameVa->Name[j];
					j++;
				}				
			}			

			SetSymbol(AddressOffset,szImportName);

			pFTsIat32++;
			pFTsIat64++;
			pOFTs32++;
			pOFTs64++;
			
			if (bIsX64)
			{
				if (pFTsIat64->u1.AddressOfData != 0 )
				{
					AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pFTsIat64->u1.AddressOfData);
				}
				else if(pOFTs64->u1.AddressOfData != 0)
				{
					AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pOFTs64->u1.AddressOfData);
				}
				else
				{
					AddressOfData2 = 0;
				}
			}
			else
			{
				if (pFTsIat32->u1.AddressOfData != 0 )
				{					
					AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pFTsIat32->u1.AddressOfData);					
				}
				else if(pOFTs32->u1.AddressOfData != 0)
				{					
					AddressOfData2 = GetRwaDataOffset(pSection,SectionNums,pOFTs32->u1.AddressOfData);					
				}
				else
				{
					AddressOfData2 = 0;
				}				
			}			
		}

		ImportTableVa++;

		if (ImportTableVa->FirstThunk)
		{
			FirstThunk = GetRwaDataOffset(pSection,SectionNums,ImportTableVa->FirstThunk);
		}
		else
		{
			FirstThunk = 0;
		}
		
		if (ImportTableVa->OriginalFirstThunk)
		{
			OriginalFirstThunk = GetRwaDataOffset(pSection,SectionNums,ImportTableVa->OriginalFirstThunk);
		}
		else
		{
			OriginalFirstThunk = 0;
		}
		
		if (ImportTableVa->Name)
		{
			MoudleNameThunk = GetRwaDataOffset(pSection,SectionNums,ImportTableVa->Name);
		}
		else
		{
			MoudleNameThunk = 0;
		}
	}
	
}

void CIdaView::GetRelocationTable(
							PBYTE pBase,
							uint64_t ImageBase,
							uint64_t RelocationRva,
							PIMAGE_SECTION_HEADER pSection,
							int SectionNums,
							BOOL bIsX64)
{
	PIMAGE_BASE_RELOCATION BaseRelocation = (PIMAGE_BASE_RELOCATION)( pBase + RelocationRva);

	if (BaseRelocation)
	{
		PIMAGE_BASE_RELOCATION NextOffset = BaseRelocation;
		while (NextOffset && NextOffset->SizeOfBlock)
		{
			uint32_t BlockSize = (NextOffset->SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION)/2;
			unsigned char* BlockBaseOffset = (unsigned char*)( pBase + NextOffset->VirtualAddress );
			
			unsigned short *RelRva = (unsigned short *)((unsigned char *)NextOffset +	IMAGE_SIZEOF_BASE_RELOCATION);
			uint64_t RelocationValue = 0;
			for ( int i = 0;i < BlockSize;i++,RelRva++)
			{
				int type, offset;

				__try
				{
					type = *RelRva >> 12;
					offset = *RelRva & 0xfff;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					break;
				}
				
				switch (type)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					// skip relocation
					break;

				case IMAGE_REL_BASED_HIGHLOW:
					{
						// change complete 32 bit address
						//RelocationValue = (*((uint32_t*) ( BlockBaseOffset + offset )));
						//TCHAR VarSymbol[64] = {0};
						//StringCchPrintf(VarSymbol,64,_T("GlobalVar_%08X"),RelocationValue);
						//SetSymbol(RelocationValue,VarSymbol);
						//*patch_addr_hl += delta;
					}

					break;

				case IMAGE_REL_BASED_DIR64:
					{
						//patchAddr64 = (unsigned __int64 *) (dest + offset);
					//	RelocationValue = *(uint64_t*) ( BlockBaseOffset + offset );
						//*patchAddr64 += delta;
					}									
					break;
				default:
					//printf("Unknown relocation: %d\n", type);
					break;
				}
			}

			NextOffset = (PIMAGE_BASE_RELOCATION)((char*)NextOffset + NextOffset->SizeOfBlock);

		}
	}
}

const LPTSTR CIdaView::ResolverSymbol(struct ud* udobj,	uint64_t addr,int64_t *offset)
{
	CIdaView* idaview = (CIdaView*)ud_get_user_opaque_data(udobj);
	return idaview->GetSymbol(addr,offset);
}

void* CIdaView::ReloveSkipJmp( struct ud& ud_obj, void* cur_node,uint64_t skip_jmp , int JmpType )
{
	CIdaView* idaview = (CIdaView*)ud_get_user_opaque_data(&ud_obj);
	return idaview->ReloveSkipJmp(cur_node,skip_jmp,JmpType);
}

const LPTSTR CIdaView::GetSymbol(	uint64_t addr,int64_t *offset)
{
	if (offset)
	{
		*offset = 0;
	}

	const LPTSTR lpRetName = m_Symbol.GetSymbol(addr);
	if (lpRetName)
	{
		return lpRetName;
	}

	return  m_Symbol.GetMoudleSymbolName(addr,offset);
}

void CIdaView::SetSymbol( uint64_t u64Ptr,LPTSTR SymbolName)
{
	m_Symbol.SetSymbol(u64Ptr,SymbolName);
}

void CIdaView::AddSymbolMoudle(uint64_t u64ptr,uint64_t uSize , LPTSTR lpMoudleName)
{
	m_Symbol.AddSymbolMoudle(u64ptr,uSize,lpMoudleName);
}

void* CIdaView::ReloveSkipJmp( void* cur_node,uint64_t skip_jmp, int JmpType)
{
	if ( cur_node)
	{
		PCustomItemHead pItem = (PCustomItemHead)cur_node;

		if ( JmpType == kJmpOther)
		{
			if (skip_jmp && pItem->GetPtrValue() > skip_jmp )
			{
				PCustomItemHead pPreItem = pItem; 
				while(pPreItem)
				{
					if ( pPreItem->GetPtrValue() == skip_jmp)
					{
						pItem->tagJmpTo = pPreItem;
						pPreItem->SetAddressColumnIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
						break;
					}
					else if(pPreItem->GetPtrValue() > skip_jmp)
					{
						pPreItem->SetAddressColumnIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
					}
					else
					{
						break;
					}

					pPreItem = pPreItem->tagPre;
				}
			}
			else if ( skip_jmp && pItem->GetPtrValue() < skip_jmp)
			{
				//pItem->tagJmpTo = pItem;
				tagSkipJmpTable* SkipJmpTable = new tagSkipJmpTable();
				if (SkipJmpTable)
				{
					SkipJmpTable->SelItem = pItem;
					SkipJmpTable->ptrJmpAddress = skip_jmp;
					SkipJmpTable->_mNext = SkipJmpTable->_mPrev = NULL;
					SkipJmpTable->JmpType = kJmpOther;
					m_SkipJmpTable.Push(SkipJmpTable);
				}
				pItem->SetAddressColumnIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
			}
			else if ( skip_jmp == 0)
			{
				tagSkipJmpTable* SkipJmpTable = m_SkipJmpTable.First();
				while(SkipJmpTable)
				{
					if (pItem->GetPtrValue() == SkipJmpTable->ptrJmpAddress )
					{
						// tagSkipJmpTable* pOldSkip = SkipJmpTable;
						SkipJmpTable->SelItem->tagJmpTo = pItem;

						if (SkipJmpTable->SelItem->GetInstructionType() == kInstructionJmp)
						{
							pItem->SetAddressColumnIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
							SkipJmpTable = m_SkipJmpTable.Remove(SkipJmpTable);

							if (SkipJmpTable)
							{
								delete SkipJmpTable;
								SkipJmpTable = NULL;
							}

							SkipJmpTable = m_SkipJmpTable.First();
							continue;
						}
						else if (SkipJmpTable->SelItem->GetInstructionType() == kInstructionCall)
						{
							pItem->SetAddressColumnIcon(0,FUCTION_BODY_HEADER_FLAGS,0,0);
						}
					}
					else if (pItem->GetPtrValue() < SkipJmpTable->ptrJmpAddress &&
						pItem->GetPtrValue() > SkipJmpTable->SelItem->GetPtrValue())
					{
						if (SkipJmpTable->SelItem->GetInstructionType() == kInstructionJmp)
						{
							pItem->SetAddressColumnIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
						}
						
					}

					SkipJmpTable = m_SkipJmpTable.Next(SkipJmpTable);
				}
			}
		}
		else if ( JmpType == kCallFar )
		{
			if ( skip_jmp && pItem->GetPtrValue() > skip_jmp)
			{
				PCustomItemHead pPreItem = pItem; 
				while(pPreItem)
				{
					if ( pPreItem->GetPtrValue() == skip_jmp)
					{
						pItem->tagJmpTo = pPreItem;
						pPreItem->SetAddressColumnIcon(0,FUCTION_BODY_HEADER_FLAGS,0,0);
						break;
					}
					else if(pPreItem->GetPtrValue() > skip_jmp)
					{
						//pPreItem->SetAddressColumnIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
					}
					else
					{
						break;
					}

					pPreItem = pPreItem->tagPre;
				}
			}
			else if( skip_jmp)
			{
				tagSkipJmpTable* SkipJmpTable = new tagSkipJmpTable();
				if (SkipJmpTable)
				{
					SkipJmpTable->SelItem = pItem;
					SkipJmpTable->ptrJmpAddress = skip_jmp;
					SkipJmpTable->_mNext = SkipJmpTable->_mPrev = NULL;
					SkipJmpTable->JmpType = kCallFar;
					m_SkipJmpTable.Push(SkipJmpTable);
				}
			}
		}
		else if( JmpType == kRetType )
		{
			PCustomItemHead pPreItem = pItem->tagPre; 
			while(pPreItem)
			{
				if ( pPreItem->GetetAddressColumnIcon(0) == FUCTION_BODY_HEADER_FLAGS )
				{					
					//pPreItem->SetAddressColumnIcon(0,FUNCTION_BODY_FLAGS,0,0);
					break;
				}
				else if(pPreItem->GetInstructionType() == kInstructionRet )
				{
					if (pPreItem->tagNext && pPreItem->tagNext->GetInstructionType() != kInstructionInt3)
					{
						pPreItem->tagNext->SetAddressColumnIcon(0,FUCTION_BODY_HEADER_FLAGS,0,0);
					}
					break;
				}
				else if(pPreItem->GetInstructionType() == kInstruction0000)
				{
					if (pPreItem->tagNext && pPreItem->tagNext->GetInstructionType() != kInstructionInt3)
					{
						pPreItem->tagNext->SetAddressColumnIcon(0,FUCTION_BODY_HEADER_FLAGS,0,0);
					}

					break;
				}
				else if( pPreItem->GetInstructionType() == kInstructionInt3 )
				{
					if (pPreItem->tagNext && pPreItem->tagNext->GetInstructionType() != kInstructionInt3)
					{
						pPreItem->tagNext->SetAddressColumnIcon(0,FUCTION_BODY_HEADER_FLAGS,0,0);
					}

					if (pPreItem->tagNext && pPreItem->tagNext->GetInstructionType() == kInstructionInt3 )
					{
						break;
					}
				}
				else if( pPreItem->GetInstructionType() != kInstructionInt3 )
				{
					pPreItem->SetAddressColumnIcon(0,FUNCTION_BODY_FLAGS,0,0);
				}
				else if (pPreItem->GetInstructionType() == kInstructionInvalid )
				{
					break;
				}
				
				
				pPreItem = pPreItem->tagPre;
			}
		}
		
	}
	return NULL;
}