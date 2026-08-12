#include "stdafx.h"
#include "cpuview.h"
#include "resource.h"
#include "global.h"
#include "disassem.h"
#include <Shlwapi.h>

#define ADDRESS_DATA_SIZE_ITEM 12
#define DATA_VIEW_WIDTH_ITEM	40
#define VIEW_DOCK_LINEE_WIDTH	2

#define WM_CHILD_MESSAGE 0x999
#define SELECT_LINE_COLOR RGB(0,0,0)
#define SELECT_LINE_WIDTH 2

#define MIN_VIEW_VIDTH_HEIGHT 6

CCpuView::CCpuView()
	:m_hWnd(NULL)
	,m_hInst(NULL)
	,m_SelectView(kCpuAsmView)
	,m_LButtonDown(FALSE)
	,m_bIsClose(TRUE)
{
	ZeroMemory(&m_ViewPos,MAX_ROOM*sizeof(ViewPos));
	m_MousePos.MousePt.x = 0;
	m_MousePos.MousePt.y = 0;
	m_BackLineBrush		= (HBRUSH)GetStockObject(GRAY_BRUSH);
	m_SelectLineBrush		= (HBRUSH)GetStockObject(BLACK_BRUSH);
}

CCpuView::~CCpuView()
{

}

CCpuView* CCpuView::Init(
						HINSTANCE hInst,
						HWND hWnd,
						LPTSTR szClass,
						LPTSTR szTitle,
						BOOL bClose)
{
	CCpuView* pCpuView = NULL;
	HWND MdiChildHandle = NULL;

	WNDCLASSEX wndclassMainFrame;
	wndclassMainFrame.cbSize		= sizeof(WNDCLASSEX);
	wndclassMainFrame.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassMainFrame.lpfnWndProc	= CCpuView::DefMdiWinProc;
	wndclassMainFrame.cbClsExtra	= 0;
	wndclassMainFrame.cbWndExtra	= sizeof(CCpuView*);
	wndclassMainFrame.hInstance		= hInst;
	wndclassMainFrame.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_CPU_VIEW));
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

	pCpuView = (CCpuView*)GetWindowLongPtr( MdiChildHandle,0);

	if (pCpuView)
	{
		*pCpuView = hInst;
		pCpuView->OnClose(bClose);
		//ShowWindow(*pCpuView,SW_SHOWMAXIMIZED);
	}
	return pCpuView;
}

LRESULT CCpuView::DefMdiWinProc(
							HWND hWnd,
							UINT Msg,
							WPARAM wParam,
							LPARAM lParam)
{
	CCpuView* pData = (CCpuView*) GetWindowLongPtr(hWnd, 0);

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
				pData->CpuViewActive(hWnd);
			}			
		}
		break;
	case WM_SIZE:
		{
			if (pData)
			{
				pData->CpuViewSize(lParam,wParam);
			}
			
		}
		break;
	case WM_MOUSEMOVE:
		{
			pData->CpuViewMouseMove(lParam,wParam);
		}
		return 0;
	case WM_MOUSELEAVE:
		{
			pData->CpuViewMouseLeave(lParam,wParam);
		}
		return 0;
	case WM_LBUTTONDOWN:
		{
			pData->CpuViewLButtonDown(lParam,wParam);
		}
		return 0;
	case WM_CPU_VIEW_LBUTTON:
		{
			pData->CpuViewUserLButtonDown(lParam,wParam);
		}
		return 0;
		break;
	case WM_LBUTTONUP:
		{
			pData->CpuViewLButtonUp(lParam,wParam);
		}
		return 0;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);			
			pData->CpuViewPaint(ps.hdc, &ps.rcPaint, ps.fErase);
			EndPaint(hWnd, &ps);
		}
 		return 0;
	case WM_NCCREATE:
		{
			if (!pData)
			{
				pData = new CCpuView();
				*pData = hWnd;
				SetWindowLongPtr(hWnd,0,(LONG_PTR)pData);
			}
		}
		break;
	case WM_OPEN_DEBUG:
		{
			if (wParam && lParam == sizeof(OpenProcessDebug))
			{
				pData->CpuOpenProcessDebug(*((OpenProcessDebug*)wParam));
			}
			
		}
		return 0;
	case WM_USER + WM_MOUSEMOVE:
		{
			pData->CpuViewUserMouseMove(lParam,wParam);

			return 0;
		}
		break;
	case WM_CLOSE:
		{
			if (pData->OnClose() == FALSE)
			{
				ShowWindow(hWnd,SW_HIDE);
				return 0;
			}
		}
		break;
	case WM_DESTROY:
		{

			if (pData->OnClose())
			{
				SetWindowLongPtr( hWnd,0,0);

				if (pData)
				{
					delete pData;
					pData = NULL;
				}
			}
			else
			{
				return 0;
			}
			
			
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


BOOL CCpuView::CpuViewActive( HWND hWnd )
{

	if ((HWND)m_CpuAsmView == 0)
	{
		m_CpuAsmView.Init(m_hInst,hWnd);
		m_DataView.Init(m_hInst,hWnd);
		m_RegistryView.Init(m_hInst,hWnd);
		m_AddressView.Init(m_hInst,hWnd);
		m_StackView.Init(m_hInst,hWnd);
		CpuViewBroadCast(kCpuAsmView);
	}
	else
	{
		m_CpuAsmView.InvalidateRect();
		m_DataView.InvalidateRect();
		m_RegistryView.InvalidateRect();
		m_AddressView.InvalidateRect();
		m_StackView.InvalidateRect();
	}
		
//	ShowWindow(m_CpuAsmView,SW_HIDE);
	SendMessage(m_hWnd,WM_SIZE,0,0);
	//ShowWindow(m_hWnd,SW_SHOWMAXIMIZED);
	return FALSE;
}

void CCpuView::CpuViewSize(LPARAM lParam,WPARAM wParam)
{
	if (m_hWnd)
	{
		BOOL RoomedId = IsZoomed(m_hWnd);
		RECT rcClient;
		GetClientRect(m_hWnd,&rcClient);
		rcClient.left += VIEW_DOCK_LINEE_WIDTH;
		rcClient.top += VIEW_DOCK_LINEE_WIDTH;
		rcClient.bottom -= VIEW_DOCK_LINEE_WIDTH;
		rcClient.right -= VIEW_DOCK_LINEE_WIDTH;

		int SizeA = 3;
		int SizeB = 5;
		int SizeC = 3;
		int SizeD = 5;
		int Ratio = 1;
		if (RoomedId)
		{
			SizeA = 3;
			SizeB = 5;
			Ratio = 4;
		}

		BOOL bMoveRgith		= m_ViewPos[RoomedId].ViewClient.right		!= rcClient.right;
		BOOL bMoveBottom	= m_ViewPos[RoomedId].ViewClient.bottom	!= rcClient.bottom;

		int TopViewRight = rcClient.left + (rcClient.right - rcClient.left)*SizeA/SizeB + DATA_VIEW_WIDTH_ITEM;
		int TopViewBottom = rcClient.top + (rcClient.bottom - rcClient.top)*SizeC/SizeD + DATA_VIEW_WIDTH_ITEM;
		int BottomViewRight = rcClient.left + (rcClient.right - rcClient.left)*SizeA/SizeB - 
							DATA_VIEW_WIDTH_ITEM;

		if (m_ViewPos[RoomedId].bInit == 0)
		{
			m_ViewPos[RoomedId].bInit = TRUE;
			bMoveRgith = TRUE;
			bMoveBottom = TRUE;
								
		}		
		
		if ( bMoveRgith && bMoveBottom )
		{
			m_ViewPos[RoomedId].ViewClient.left	= rcClient.left;
			m_ViewPos[RoomedId].ViewClient.right	= rcClient.right;
			m_ViewPos[RoomedId].ViewClient.top		= rcClient.top;
			m_ViewPos[RoomedId].ViewClient.bottom	= rcClient.bottom;

			m_ViewPos[RoomedId].AsmView.left	= rcClient.left;
			m_ViewPos[RoomedId].AsmView.top	= rcClient.top;
			m_ViewPos[RoomedId].AsmView.right	= TopViewRight;
			m_ViewPos[RoomedId].AsmView.bottom = TopViewBottom - Ratio*ADDRESS_DATA_SIZE_ITEM;

			m_ViewPos[RoomedId].AddressView.left = rcClient.left;
			m_ViewPos[RoomedId].AddressView.top  = m_ViewPos[RoomedId].AsmView.bottom + VIEW_DOCK_LINEE_WIDTH;
			m_ViewPos[RoomedId].AddressView.right = m_ViewPos[RoomedId].AsmView.right;
			m_ViewPos[RoomedId].AddressView.bottom = m_ViewPos[RoomedId].AddressView.top + Ratio*ADDRESS_DATA_SIZE_ITEM;

			m_ViewPos[RoomedId].DataView.left = rcClient.left;
			m_ViewPos[RoomedId].DataView.right = BottomViewRight;
			m_ViewPos[RoomedId].DataView.top  = m_ViewPos[RoomedId].AddressView.bottom + VIEW_DOCK_LINEE_WIDTH;
			m_ViewPos[RoomedId].DataView.bottom = rcClient.bottom;
		}
		else if(bMoveRgith)
		{
			m_ViewPos[RoomedId].ViewClient.right	= rcClient.right;
			m_ViewPos[RoomedId].AsmView.right	= TopViewRight;
			m_ViewPos[RoomedId].AddressView.right = m_ViewPos[RoomedId].AsmView.right;
			m_ViewPos[RoomedId].DataView.right = BottomViewRight;
			
		}
		else if(bMoveBottom)
		{
			m_ViewPos[RoomedId].AsmView.bottom = TopViewBottom - Ratio*ADDRESS_DATA_SIZE_ITEM;
			m_ViewPos[RoomedId].AddressView.top  = m_ViewPos[RoomedId].AsmView.bottom + VIEW_DOCK_LINEE_WIDTH;
			m_ViewPos[RoomedId].AddressView.bottom = m_ViewPos[RoomedId].AddressView.top + Ratio*ADDRESS_DATA_SIZE_ITEM;

		}

		m_ViewPos[RoomedId].ViewClient.left	= rcClient.left;
		m_ViewPos[RoomedId].ViewClient.right	= rcClient.right;
		m_ViewPos[RoomedId].ViewClient.top		= rcClient.top;
		m_ViewPos[RoomedId].ViewClient.bottom	= rcClient.bottom;

		m_ViewPos[RoomedId].DataView.left = rcClient.left;		
		m_ViewPos[RoomedId].DataView.top  = m_ViewPos[RoomedId].AddressView.bottom + VIEW_DOCK_LINEE_WIDTH;
		m_ViewPos[RoomedId].DataView.bottom = rcClient.bottom;

		m_ViewPos[RoomedId].RegView.left = m_ViewPos[RoomedId].AsmView.right + VIEW_DOCK_LINEE_WIDTH;
		m_ViewPos[RoomedId].RegView.top  = rcClient.top;
		m_ViewPos[RoomedId].RegView.right = rcClient.right;
		m_ViewPos[RoomedId].RegView.bottom = m_ViewPos[RoomedId].AddressView.bottom;

		m_ViewPos[RoomedId].StackView.left = m_ViewPos[RoomedId].DataView.right + VIEW_DOCK_LINEE_WIDTH;
		m_ViewPos[RoomedId].StackView.right = rcClient.right;
		m_ViewPos[RoomedId].StackView.top  = m_ViewPos[RoomedId].DataView.top;
		m_ViewPos[RoomedId].StackView.bottom = rcClient.bottom;	

		MoveWindow(
			m_CpuAsmView,
			m_ViewPos[RoomedId].AsmView.left,
			m_ViewPos[RoomedId].AsmView.top,
			m_ViewPos[RoomedId].AsmView.right - m_ViewPos[RoomedId].AsmView.left,
			m_ViewPos[RoomedId].AsmView.bottom - m_ViewPos[RoomedId].AsmView.top,			
			FALSE);

		MoveWindow(
			m_AddressView,
			m_ViewPos[RoomedId].AddressView.left,
			m_ViewPos[RoomedId].AddressView.top,
			m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
			m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
			FALSE);

		MoveWindow(
			m_RegistryView,
			m_ViewPos[RoomedId].RegView.left,
			m_ViewPos[RoomedId].RegView.top,
			m_ViewPos[RoomedId].RegView.right - m_ViewPos[RoomedId].RegView.left,
			m_ViewPos[RoomedId].RegView.bottom - m_ViewPos[RoomedId].RegView.top,			
			FALSE);			

		MoveWindow(
			m_DataView,
			m_ViewPos[RoomedId].DataView.left,
			m_ViewPos[RoomedId].DataView.top,
			m_ViewPos[RoomedId].DataView.right - m_ViewPos[RoomedId].DataView.left,
			m_ViewPos[RoomedId].DataView.bottom - m_ViewPos[RoomedId].DataView.top,			
			FALSE);

		MoveWindow(
			m_StackView,
			m_ViewPos[RoomedId].StackView.left,
			m_ViewPos[RoomedId].StackView.top,
			m_ViewPos[RoomedId].StackView.right - m_ViewPos[RoomedId].StackView.left,
			m_ViewPos[RoomedId].StackView.bottom - m_ViewPos[RoomedId].StackView.top,			
			FALSE);

		RedrawWindow(m_hWnd,0,0,RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);
	}
}

void CCpuView::CpuViewMouseLeave(LPARAM lParam,WPARAM wParam)
{
	if (GetCapture() == m_hWnd)
	{
		ReleaseCapture();
	}
}

void CCpuView::CpuViewMouseMove(LPARAM lParam,WPARAM wParam)
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
					m_ViewPos[RoomedId].AsmView.right += GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.right += GapSpaceValue;
					m_ViewPos[RoomedId].RegView.left	  += GapSpaceValue;
				}
				else if(MousePt.x < m_MousePos.MousePt.x)
				{
					m_ViewPos[RoomedId].AsmView.right -= GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.right -= GapSpaceValue;
					m_ViewPos[RoomedId].RegView.left	  -= GapSpaceValue;
				}

				m_MousePos.MousePt.x = MousePt.x;
				m_MousePos.MousePt.y = MousePt.y;
				
				MoveWindow(
					m_CpuAsmView,
					m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.top,
					m_ViewPos[RoomedId].AsmView.right - m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.bottom - m_ViewPos[RoomedId].AsmView.top,			
					FALSE);

				MoveWindow(
					m_AddressView,
					m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.top,
					m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
					FALSE);

				MoveWindow(
					m_RegistryView,
					m_ViewPos[RoomedId].RegView.left,
					m_ViewPos[RoomedId].RegView.top,
					m_ViewPos[RoomedId].RegView.right - m_ViewPos[RoomedId].RegView.left,
					m_ViewPos[RoomedId].RegView.bottom - m_ViewPos[RoomedId].RegView.top,			
					FALSE);			
			}
			else if ( m_ViewPos[RoomedId].TraceViewLine == 2 )
			{			
				ULONG GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);

				if (MousePt.y > m_MousePos.MousePt.y)
				{
					if (MousePt.y > (m_ViewPos[RoomedId].AddressView.bottom - MIN_VIEW_VIDTH_HEIGHT))
					{
						MousePt.y = (m_ViewPos[RoomedId].AddressView.bottom - MIN_VIEW_VIDTH_HEIGHT);
						GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);
					}

					m_ViewPos[RoomedId].AsmView.bottom		+= GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.top		+= GapSpaceValue;
				}
				else if(MousePt.y < m_MousePos.MousePt.y)
				{
					if (MousePt.y < (m_ViewPos[RoomedId].AddressView.bottom - 8*MIN_VIEW_VIDTH_HEIGHT))
					{
						MousePt.y = (m_ViewPos[RoomedId].AddressView.bottom - 8*MIN_VIEW_VIDTH_HEIGHT);
						GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);
					}

					m_ViewPos[RoomedId].AsmView.bottom		-= GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.top		-= GapSpaceValue;
				}

				m_MousePos.MousePt.x = MousePt.x;
				m_MousePos.MousePt.y = MousePt.y;

				MoveWindow(
					m_CpuAsmView,
					m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.top,
					m_ViewPos[RoomedId].AsmView.right - m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.bottom - m_ViewPos[RoomedId].AsmView.top,			
					FALSE);

				MoveWindow(
					m_AddressView,
					m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.top,
					m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
					FALSE);
			}
			else if ( m_ViewPos[RoomedId].TraceViewLine == 3 )
			{			
				ULONG GapSpaceValue = abs(MousePt.x - m_MousePos.MousePt.x);

				if (MousePt.x > m_MousePos.MousePt.x)
				{
					m_ViewPos[RoomedId].DataView.right += GapSpaceValue;
					m_ViewPos[RoomedId].StackView.left	  += GapSpaceValue;
				}
				else if(MousePt.x < m_MousePos.MousePt.x)
				{
					m_ViewPos[RoomedId].DataView.right -= GapSpaceValue;
					m_ViewPos[RoomedId].StackView.left	  -= GapSpaceValue;
				}

				m_MousePos.MousePt.x = MousePt.x;
				m_MousePos.MousePt.y = MousePt.y;

				MoveWindow(
					m_DataView,
					m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.top,
					m_ViewPos[RoomedId].DataView.right - m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.bottom - m_ViewPos[RoomedId].DataView.top,			
					FALSE);

				MoveWindow(
					m_StackView,
					m_ViewPos[RoomedId].StackView.left,
					m_ViewPos[RoomedId].StackView.top,
					m_ViewPos[RoomedId].StackView.right - m_ViewPos[RoomedId].StackView.left,
					m_ViewPos[RoomedId].StackView.bottom - m_ViewPos[RoomedId].StackView.top,			
					FALSE);
			}
			else if ( m_ViewPos[RoomedId].TraceViewLine == 4 )
			{			
				ULONG GapSpaceValue = abs(MousePt.y - m_MousePos.MousePt.y);

				if (MousePt.y > m_MousePos.MousePt.y)
				{
					m_ViewPos[RoomedId].RegView.bottom		+= GapSpaceValue;
					m_ViewPos[RoomedId].StackView.top		+= GapSpaceValue;
					m_ViewPos[RoomedId].DataView.top		+= GapSpaceValue;

					m_ViewPos[RoomedId].AddressView.top		+= GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.bottom	+= GapSpaceValue;

					m_ViewPos[RoomedId].AsmView.bottom		+= GapSpaceValue;
				}
				else if(MousePt.y < m_MousePos.MousePt.y)
				{
					m_ViewPos[RoomedId].RegView.bottom		-= GapSpaceValue;
					m_ViewPos[RoomedId].StackView.top		-= GapSpaceValue;
					m_ViewPos[RoomedId].DataView.top		-= GapSpaceValue;

					m_ViewPos[RoomedId].AddressView.top		-= GapSpaceValue;
					m_ViewPos[RoomedId].AddressView.bottom	-= GapSpaceValue;

					m_ViewPos[RoomedId].AsmView.bottom		-= GapSpaceValue;
				}

				m_MousePos.MousePt.x = MousePt.x;
				m_MousePos.MousePt.y = MousePt.y;

				MoveWindow(
					m_CpuAsmView,
					m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.top,
					m_ViewPos[RoomedId].AsmView.right - m_ViewPos[RoomedId].AsmView.left,
					m_ViewPos[RoomedId].AsmView.bottom - m_ViewPos[RoomedId].AsmView.top,			
					FALSE);
				MoveWindow(
					m_RegistryView,
					m_ViewPos[RoomedId].RegView.left,
					m_ViewPos[RoomedId].RegView.top,
					m_ViewPos[RoomedId].RegView.right - m_ViewPos[RoomedId].RegView.left,
					m_ViewPos[RoomedId].RegView.bottom - m_ViewPos[RoomedId].RegView.top,			
					FALSE);

				MoveWindow(
					m_AddressView,
					m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.top,
					m_ViewPos[RoomedId].AddressView.right - m_ViewPos[RoomedId].AddressView.left,
					m_ViewPos[RoomedId].AddressView.bottom - m_ViewPos[RoomedId].AddressView.top,			
					FALSE);

				MoveWindow(
					m_DataView,
					m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.top,
					m_ViewPos[RoomedId].DataView.right - m_ViewPos[RoomedId].DataView.left,
					m_ViewPos[RoomedId].DataView.bottom - m_ViewPos[RoomedId].DataView.top,			
					FALSE);

				MoveWindow(
					m_StackView,
					m_ViewPos[RoomedId].StackView.left,
					m_ViewPos[RoomedId].StackView.top,
					m_ViewPos[RoomedId].StackView.right - m_ViewPos[RoomedId].StackView.left,
					m_ViewPos[RoomedId].StackView.bottom - m_ViewPos[RoomedId].StackView.top,			
					FALSE);
			}

			m_MousePos.MousePt.x = MousePt.x;
			m_MousePos.MousePt.y = MousePt.y;
			RedrawWindow(m_hWnd,0,0,RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);

			if (m_ViewPos[RoomedId].TraceViewLine == 1 || m_ViewPos[RoomedId].TraceViewLine == 3)
			{
				MouseCursor = LoadCursor(0,IDC_SIZEWE);
			}
			else
			{
				MouseCursor = LoadCursor(0,IDC_SIZENS);
			}

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

void CCpuView::CpuViewNcLButtonDown(LPARAM lParam,WPARAM wParam)
{
	
}

void CCpuView::CpuViewUserLButtonDown(LPARAM lParam,WPARAM wParam)
{
	ViewName ViewIndex = (ViewName)lParam;

	if ( ViewIndex >= kCpuAsmView &&
		ViewIndex <= kCpuStackView)
	{
		m_SelectView = ViewIndex;
		CpuViewBroadCast(ViewIndex);
		RedrawWindow(m_hWnd,0,0,RDW_INVALIDATE);
	}
}

void CCpuView::CpuViewLButtonDown(LPARAM lParam,WPARAM wParam)
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

void CCpuView::CpuViewLButtonUp(LPARAM lParam,WPARAM wParam)
{
	if ( GetCapture() == m_hWnd)
	{
		ReleaseCapture();
	}

	m_LButtonDown = FALSE;
}

void CCpuView::CpuViewUserMouseMove(LPARAM lParam,WPARAM wParam)
{
	HCURSOR MouseCursor;
	int RoomedId = IsZoomed(m_hWnd);
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(m_hWnd,&MousePoint);

	if (  MousePoint.x > m_ViewPos[RoomedId].AsmView.right &&
		MousePoint.x < m_ViewPos[RoomedId].RegView.left)
	{
		MouseCursor = LoadCursor(0,IDC_SIZEWE);
	}
	else
	{
		MouseCursor = LoadCursor(0,IDC_ARROW);
	}

	SetCursor(MouseCursor);

}
void CCpuView::CpuViewPaint( HDC hDC, RECT* prcDirty, BOOL bErase)
{

 	RECT rcClient;
 	GetClientRect(m_hWnd,&rcClient);
	int RoomedId = IsZoomed(m_hWnd);
	if (m_ViewPos[RoomedId].bInit)
	{
		RECT rcTopRight;
		rcTopRight.left		= m_ViewPos[RoomedId].AsmView.right;
		rcTopRight.right	= m_ViewPos[RoomedId].RegView.left;
		rcTopRight.top		= m_ViewPos[RoomedId].AsmView.top;
		rcTopRight.bottom	= m_ViewPos[RoomedId].RegView.bottom + VIEW_DOCK_LINEE_WIDTH;

		RECT rcAsmBottomLine;
		rcAsmBottomLine.left	= m_ViewPos[RoomedId].AsmView.left - VIEW_DOCK_LINEE_WIDTH;
		rcAsmBottomLine.right	= m_ViewPos[RoomedId].AsmView.right;
		rcAsmBottomLine.top		= m_ViewPos[RoomedId].AsmView.bottom;
		rcAsmBottomLine.bottom	= m_ViewPos[RoomedId].AddressView.top;

		RECT rcDataViewTopLine;
		rcDataViewTopLine.left		= m_ViewPos[RoomedId].AsmView.left - VIEW_DOCK_LINEE_WIDTH;
		rcDataViewTopLine.right		= m_ViewPos[RoomedId].RegView.right;
		rcDataViewTopLine.top		= m_ViewPos[RoomedId].AddressView.bottom;
		rcDataViewTopLine.bottom	= m_ViewPos[RoomedId].DataView.top;

		RECT rcDataViewRightLine;
		rcDataViewRightLine.left	= m_ViewPos[RoomedId].DataView.right;
		rcDataViewRightLine.right	= m_ViewPos[RoomedId].StackView.left;
		rcDataViewRightLine.top		= m_ViewPos[RoomedId].StackView.top;
		rcDataViewRightLine.bottom	= m_ViewPos[RoomedId].StackView.bottom;

		RECT rcClinetTopLine;
		rcClinetTopLine.left	= rcClient.left;
		rcClinetTopLine.right	= rcClient.right;
		rcClinetTopLine.top		= rcClient.top;
		rcClinetTopLine.bottom	= m_ViewPos[RoomedId].AsmView.top;

		RECT rcClinetLeftLine;
		rcClinetLeftLine.left	= rcClient.left;
		rcClinetLeftLine.right	= m_ViewPos[RoomedId].AsmView.left;
		rcClinetLeftLine.top	= rcClient.top;
		rcClinetLeftLine.bottom	= rcClient.bottom;

		RECT rcClinetBottomLine;
		rcClinetBottomLine.left		= rcClient.left;
		rcClinetBottomLine.right	= rcClient.right;
		rcClinetBottomLine.top		= m_ViewPos[RoomedId].DataView.bottom;
		rcClinetBottomLine.bottom	= rcClient.bottom;

		RECT rcClinetRightLine;
		rcClinetRightLine.left		= m_ViewPos[RoomedId].RegView.right;
		rcClinetRightLine.right		= rcClient.right;
		rcClinetRightLine.top		= rcClient.top;
		rcClinetRightLine.bottom	= rcClient.bottom;

		FillRect(hDC,&rcTopRight,m_BackLineBrush);
		FillRect(hDC,&rcAsmBottomLine,m_BackLineBrush);
		FillRect(hDC,&rcDataViewTopLine,m_BackLineBrush);
		FillRect(hDC,&rcDataViewRightLine,m_BackLineBrush);

		FillRect(hDC,&rcClinetTopLine,m_BackLineBrush);
		FillRect(hDC,&rcClinetLeftLine,m_BackLineBrush);
		FillRect(hDC,&rcClinetBottomLine,m_BackLineBrush);
		FillRect(hDC,&rcClinetRightLine,m_BackLineBrush);
	}
	else
	{
		 FillRect(hDC,&rcClient,m_BackLineBrush);		
	}

	if ( m_SelectView >= kCpuAsmView )
	{
		RECT rcTop = {0};
		RECT rcLeft = {0};
		RECT rcRight = {0};
		RECT rcBottom = {0};

		switch(m_SelectView)
		{
		case kCpuAsmView:
			{
				rcTop.left	= rcClient.left;
				rcTop.top	= rcClient.top;
				rcTop.right = m_ViewPos[RoomedId].AsmView.right;
				rcTop.bottom = rcTop.top + VIEW_DOCK_LINEE_WIDTH;

				rcLeft.left = rcTop.left;
				rcLeft.top  = rcTop.top;
				rcLeft.right = rcLeft.left + VIEW_DOCK_LINEE_WIDTH;
				rcLeft.bottom = m_ViewPos[RoomedId].AsmView.bottom + VIEW_DOCK_LINEE_WIDTH;

				rcRight.left	= m_ViewPos[RoomedId].AsmView.right;
				rcRight.top		= rcTop.top;
				rcRight.right	= rcRight.left + VIEW_DOCK_LINEE_WIDTH;
				rcRight.bottom	= m_ViewPos[RoomedId].AsmView.bottom + VIEW_DOCK_LINEE_WIDTH;

				rcBottom.left = rcLeft.left;
				rcBottom.top  = m_ViewPos[RoomedId].AsmView.bottom;
				rcBottom.right = m_ViewPos[RoomedId].AsmView.right + VIEW_DOCK_LINEE_WIDTH;
				rcBottom.bottom = m_ViewPos[RoomedId].AsmView.bottom + VIEW_DOCK_LINEE_WIDTH;
			}
			break;
		case kCpuRegView:
			{
				rcTop.left	=  m_ViewPos[RoomedId].RegView.left - VIEW_DOCK_LINEE_WIDTH;
				rcTop.top	= rcClient.top;
				rcTop.right = rcClient.right;
				rcTop.bottom = rcTop.top + VIEW_DOCK_LINEE_WIDTH;

				rcLeft.left = rcTop.left;
				rcLeft.top  = rcTop.top;
				rcLeft.right = rcLeft.left + VIEW_DOCK_LINEE_WIDTH;
				rcLeft.bottom = m_ViewPos[RoomedId].RegView.bottom + VIEW_DOCK_LINEE_WIDTH;

				rcRight.left	= m_ViewPos[RoomedId].RegView.right;
				rcRight.top		= rcTop.top;
				rcRight.right	= rcRight.left + VIEW_DOCK_LINEE_WIDTH;
				rcRight.bottom	= rcLeft.bottom;

				rcBottom.left = rcLeft.left;
				rcBottom.top  = m_ViewPos[RoomedId].RegView.bottom;
				rcBottom.right = m_ViewPos[RoomedId].RegView.right + VIEW_DOCK_LINEE_WIDTH;
				rcBottom.bottom = m_ViewPos[RoomedId].RegView.bottom + VIEW_DOCK_LINEE_WIDTH;
			}
			break;
		case kCpuAddressView:
			{
				rcTop.left	=  rcClient.left;
				rcTop.top	=  m_ViewPos[RoomedId].AddressView.top - VIEW_DOCK_LINEE_WIDTH;
				rcTop.right = m_ViewPos[RoomedId].AddressView.right;
				rcTop.bottom = m_ViewPos[RoomedId].AddressView.top;

				rcLeft.left = rcTop.left;
				rcLeft.top  = rcTop.top;
				rcLeft.right = rcLeft.left + VIEW_DOCK_LINEE_WIDTH;
				rcLeft.bottom = m_ViewPos[RoomedId].AddressView.bottom + VIEW_DOCK_LINEE_WIDTH;

				rcRight.left	= m_ViewPos[RoomedId].AddressView.right;
				rcRight.top		= rcTop.top;
				rcRight.right	= rcRight.left + VIEW_DOCK_LINEE_WIDTH;
				rcRight.bottom	= rcLeft.bottom;

				rcBottom.left = rcLeft.left;
				rcBottom.top  = m_ViewPos[RoomedId].AddressView.bottom;
				rcBottom.right = m_ViewPos[RoomedId].AddressView.right + VIEW_DOCK_LINEE_WIDTH;
				rcBottom.bottom = m_ViewPos[RoomedId].AddressView.bottom + VIEW_DOCK_LINEE_WIDTH;
			}
			break;
		case kCpuDataView:
			{
				rcTop.left	=  rcClient.left;
				rcTop.top	=  m_ViewPos[RoomedId].DataView.top - VIEW_DOCK_LINEE_WIDTH;
				rcTop.right = m_ViewPos[RoomedId].DataView.right;
				rcTop.bottom = m_ViewPos[RoomedId].DataView.top;

				rcLeft.left = rcTop.left;
				rcLeft.top  = rcTop.top;
				rcLeft.right = rcLeft.left + VIEW_DOCK_LINEE_WIDTH;
				rcLeft.bottom = m_ViewPos[RoomedId].DataView.bottom + VIEW_DOCK_LINEE_WIDTH;

				rcRight.left	= m_ViewPos[RoomedId].DataView.right;
				rcRight.top		= rcTop.top;
				rcRight.right	= rcRight.left + VIEW_DOCK_LINEE_WIDTH;
				rcRight.bottom	= rcLeft.bottom;

				rcBottom.left = rcLeft.left;
				rcBottom.top  = m_ViewPos[RoomedId].DataView.bottom;
				rcBottom.right = m_ViewPos[RoomedId].DataView.right + VIEW_DOCK_LINEE_WIDTH;
				rcBottom.bottom = m_ViewPos[RoomedId].DataView.bottom + VIEW_DOCK_LINEE_WIDTH;
			}
			break;
		case kCpuStackView:
			{
				rcTop.left	=  m_ViewPos[RoomedId].StackView.left - VIEW_DOCK_LINEE_WIDTH;
				rcTop.top	=  m_ViewPos[RoomedId].StackView.top - VIEW_DOCK_LINEE_WIDTH;
				rcTop.right = m_ViewPos[RoomedId].StackView.right;
				rcTop.bottom = m_ViewPos[RoomedId].StackView.top;

				rcLeft.left = rcTop.left;
				rcLeft.top  = rcTop.top;
				rcLeft.right = rcLeft.left + VIEW_DOCK_LINEE_WIDTH;
				rcLeft.bottom = m_ViewPos[RoomedId].StackView.bottom + VIEW_DOCK_LINEE_WIDTH;

				rcRight.left	= m_ViewPos[RoomedId].StackView.right;
				rcRight.top		= rcTop.top;
				rcRight.right	= rcRight.left + VIEW_DOCK_LINEE_WIDTH;
				rcRight.bottom	= rcLeft.bottom;

				rcBottom.left = rcLeft.left;
				rcBottom.top  = m_ViewPos[RoomedId].StackView.bottom;
				rcBottom.right = m_ViewPos[RoomedId].StackView.right + VIEW_DOCK_LINEE_WIDTH;
				rcBottom.bottom = m_ViewPos[RoomedId].StackView.bottom + VIEW_DOCK_LINEE_WIDTH;
			}
			break;
		}

		FillRect(hDC,&rcTop,m_SelectLineBrush);
		FillRect(hDC,&rcLeft,m_SelectLineBrush);
		FillRect(hDC,&rcBottom,m_SelectLineBrush);
		FillRect(hDC,&rcRight,m_SelectLineBrush);
	}
		
}

LRESULT CCpuView::CpuViewSystemCommand(LPARAM lParam,WPARAM wParam)
{
	if ( wParam == SC_MAXIMIZE )
	{
		//return 0;
	}
	else if (wParam == SC_RESTORE)
	{
		//return 0;
	}
	
	return DefMDIChildProc(m_hWnd,WM_SYSCOMMAND,wParam,lParam);
}

void CCpuView::CpuViewBroadCast( ViewName SelectView)
{
	//m_CpuAsmView.BroadCastView(SelectView);
	//m_DataView.BroadCastView(SelectView);
	//m_RegistryView.BroadCastView(SelectView);
	//m_StackView.BroadCastView(SelectView);
	//m_AddressView.BroadCastView(SelectView);
}

HCURSOR CCpuView::CpuViewTraceLoadCursor(
								POINT& MousePt,
								ULONG& TraceViewLine)
{
	HCURSOR CurrentCursor;
	
	int RoomedId = IsZoomed(m_hWnd);

	if( MousePt.x >= m_ViewPos[RoomedId].AsmView.right &&
		MousePt.x <= m_ViewPos[RoomedId].RegView.left &&
		MousePt.y > (m_ViewPos[RoomedId].RegView.top - VIEW_DOCK_LINEE_WIDTH) &&
		MousePt.y < m_ViewPos[RoomedId].RegView.bottom)
	{
		 TraceViewLine = 1;
	}
	else if( MousePt.x > ( m_ViewPos[RoomedId].AddressView.left - VIEW_DOCK_LINEE_WIDTH) &&
		MousePt.x < m_ViewPos[RoomedId].AddressView.right &&
		MousePt.y >= m_ViewPos[RoomedId].AsmView.bottom &&
		MousePt.y <= m_ViewPos[RoomedId].AddressView.top)
	{
		TraceViewLine = 2;
	}
	else if( MousePt.x >= m_ViewPos[RoomedId].DataView.right &&
		MousePt.x <= m_ViewPos[RoomedId].StackView.left &&
		MousePt.y > m_ViewPos[RoomedId].StackView.top &&
		MousePt.y <= m_ViewPos[RoomedId].StackView.bottom + VIEW_DOCK_LINEE_WIDTH)
	{
		TraceViewLine = 3;
	}
	else if( MousePt.x >= (m_ViewPos[RoomedId].DataView.left - VIEW_DOCK_LINEE_WIDTH) &&
		MousePt.x < (m_ViewPos[RoomedId].StackView.right + VIEW_DOCK_LINEE_WIDTH) &&
		MousePt.y >= m_ViewPos[RoomedId].RegView.bottom &&
		MousePt.y <= m_ViewPos[RoomedId].StackView.top)
	{
		TraceViewLine = 4;
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

int CCpuView::CpuOpenProcessDebug(OpenProcessDebug& OpenDebug)
{
	/*
	CZ0BTDisasm Disassem;
	ShowWindow(m_hWnd,SW_SHOWMAXIMIZED);
	HANDLE hFile = CreateFile(OpenDebug.ProcessPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

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
 					if (Disassem.Disassem(FileBytes,ReadLength,0,this))
 					{
 					}
				}
			}
		}
		CloseHandle(hFile);
	}
	*/
	BOOL bSuccess = FALSE;
	RequestPacket* Packet = (RequestPacket*)malloc(sizeof(RequestPacket) + sizeof(DebugCreateProcess));
	if (Packet)
	{
		ZeroMemory(
				Packet,
				sizeof(RequestPacket) + sizeof(DebugCreateProcess));
		Packet->rpDataSize = sizeof(DebugCreateProcess);
		Packet->rpFlags = kPacketDebugCreateProcess;
		DebugCreateProcess* DbgCreate = (DebugCreateProcess*)&Packet->rpData[0];
		StringCchCopy(DbgCreate->ProcessPath,2*MAX_PATH,OpenDebug.ProcessPath);
		StringCchCopy(DbgCreate->ProcessDirectory,2*MAX_PATH,OpenDebug.ProcessPath);
		StringCchCopy(DbgCreate->CreateArg,4*MAX_PATH,OpenDebug.ProcessParam);
		PathRemoveFileSpec(DbgCreate->ProcessDirectory);
		//PathStripPath(DbgCreate->ProcessPath);
		DbgCreate->CreateFlags = (CREATE_DEFAULT_ERROR_MODE | DEBUG_ONLY_THIS_PROCESS | NORMAL_PRIORITY_CLASS );
		DbgCreate->InheritHandles = FALSE;
		bSuccess = g_Debugger.StartDebugProcess(*Packet);

		ZeroMemory(
			Packet,
			sizeof(RequestPacket) + sizeof(DebugCreateProcess));

		free(Packet);
		Packet = NULL;
	}
	//Packet.rpDataSize 
	
	return bSuccess;
}


CRegisterView::CRegisterView(void)
{
	m_ShowFlags = kCpuRegView;
}

CRegisterView::~CRegisterView(void)
{

}

BOOL CRegisterView::Init(HINSTANCE hInst,HWND hWnd)
{
	if(Create(hWnd,hInst,IDC_CPU_DATA_VIEW,_T("CRegisterView")))
	{
		InsertHeader( 0 ,500,L" 寄存器(FPU)          <      <     <     <     <     <     <     <     <     <     <     <      <       <     <  <  <  <  <  <  <  <  <  < " );
		SetBodyCxSize(0);
		return TRUE;
	}

	return FALSE;
}

CDynamicDrawItem* CRegisterView::OnGetDrawData()
{
	return (CDynamicDrawItem*)1;
}
void CRegisterView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{

}

void CRegisterView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{

}

int	 CRegisterView::OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect)
{
	return 0;
}

PVOID CRegisterView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	return NULL;
}

void CRegisterView::ReleaseItemData(CDynamicDrawItem* pDraw)
{

}

void CRegisterView::OnClickItemUp()
{

}

int CRegisterView::OnMoveScroll(int Pos)
{
	return 0;
}

void CRegisterView::OnTimerSelectUp()
{

}

void CRegisterView::OnTimerSelectDown()
{

}

CDataView::CDataView(void)
	:m_HexDataLen(kHexOct16)
{
	m_ShowFlags = kCpuDataView;
}

CDataView::~CDataView(void)
{

}


BOOL CDataView::Init(HINSTANCE hInst,HWND hWnd)
{	
	return	__super::Init(hWnd,hInst);
}


CAddressView::CAddressView(void)
{
	m_ShowFlags = kCpuAddressView;
}

CAddressView::~CAddressView(void)
{
	
}

BOOL CAddressView::Init(HINSTANCE hInst,HWND hWnd)
{
	if(Create(hWnd,hInst,IDC_CPU_DATA_VIEW,_T("CAddressView")))
	{
		InsertHeader( 0 ,500,L"  地址");
		
		SetBodyCxSize(0);
		SetHeaderHeight(0);
		
		return TRUE;
	}

	return FALSE;
}

CDynamicDrawItem* CAddressView::OnGetDrawData()
{
	return (CDynamicDrawItem*)1;
}

void CAddressView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{

}

void CAddressView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{

}

int	 CAddressView::OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect)
{
	return 0;
}

PVOID CAddressView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	return NULL;
}

void CAddressView::ReleaseItemData(CDynamicDrawItem* pDraw)
{

}

void CAddressView::OnClickItemUp()
{

}

int CAddressView::OnMoveScroll(int Pos)
{
	return 0;
}

void CAddressView::OnTimerSelectUp()
{

}

void CAddressView::OnTimerSelectDown()
{

}

CStackView::CStackView(void)
{
	m_ShowFlags = kCpuStackView;
}

CStackView::~CStackView(void)
{

}


BOOL CStackView::Init(HINSTANCE hInst,HWND hWnd)
{
	if(Create(hWnd,hInst,IDC_CPU_DATA_VIEW,_T("CStackView")))
	{
		InsertHeader( 0 , 90 ,L"  地址" );
		InsertHeader( 1 , 120 ,L"  地址值" );
		InsertHeader( 2 , 200 ,L"  参数信息" );
		
		SetBodyCxSize(0);
		SetHeaderHeight(0);
		return TRUE;
	}

	return FALSE;
}

CDynamicDrawItem* CStackView::OnGetDrawData()
{
	return (CDynamicDrawItem*)1;
}

void CStackView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{

}

void CStackView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{

}

int	 CStackView::OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect)
{
	return 0;
}

PVOID CStackView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	return NULL;
}

void CStackView::ReleaseItemData(CDynamicDrawItem* pDraw)
{

}

void CStackView::OnClickItemUp()
{

}

int CStackView::OnMoveScroll(int Pos)
{
	return 0;
}

void CStackView::OnTimerSelectUp()
{

}

void CStackView::OnTimerSelectDown()
{

}