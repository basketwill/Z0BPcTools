#include "stdafx.h"
#include "toolbar.h"
#include "resource.h"
#include "global.h"

#define BUTTON_BITMAP_GAP 2

CToolBar::CToolBar()
	:m_hWnd(NULL)
	,m_nSelectButton(-1)
{
	ZeroMemory(m_BitmapArray,BITMAP_ARRAY_LIST*sizeof(HBITMAP));
	ZeroMemory(m_BitmapRect,BITMAP_ARRAY_LIST*sizeof(RECT));
	ZeroMemory(&m_ToolHitPos,sizeof(tagToolBarHitPos));

	m_PenArray[kToolBarWhitePen] = CreatePen(PS_SOLID,5,kCustomWhiteColor);
	m_PenArray[kToolBarBlackPen] = CreatePen(PS_SOLID,5,kCustomBlackColor);

	m_BrushArray[kToolBarWhitePen] = CreateSolidBrush(kCustomWhiteColor);
	m_BrushArray[kToolBarBlackPen] = CreateSolidBrush(kCustomBlackColor);

	m_BrushBkg = CreateSolidBrush(kCustomBkg);

	ZeroMemory(m_ButtonEvent,sizeof(pButtonEvent*)*BITMAP_ARRAY_LIST);
	m_ButtonEvent[kToolBarOpenFile] = (pButtonEvent)&CToolBar::OnLButtonOpenFile;
	m_ButtonEvent[kToolBarRestart]	= (pButtonEvent)&CToolBar::OnLButtonRestart;
	m_ButtonEvent[kToolBarCloseDebug]	=  (pButtonEvent)&CToolBar::OnLButtonCloseDebug;
	m_ButtonEvent[kToolBarDebugRun]		=  (pButtonEvent)&CToolBar::OnLButtonDebugRun;
	m_ButtonEvent[kToolBarRunCurThread]	=  (pButtonEvent)&CToolBar::OnLButtonRunCurThread;
	m_ButtonEvent[kToolBarDebugPause] =  (pButtonEvent)&CToolBar::OnLButtonDebugPause;
	m_ButtonEvent[kToolBarStepInto]		=  (pButtonEvent)&CToolBar::OnLButtonStepInto;
	m_ButtonEvent[kToolBarStepOver]		=  (pButtonEvent)&CToolBar::OnLButtonStepOver;
	m_ButtonEvent[kToolBarTraceInto]		=  (pButtonEvent)&CToolBar::OnLButtonTraceInto;
	m_ButtonEvent[kToolBarTraceOver]		=  (pButtonEvent)&CToolBar::OnLButtonTraceOver;
	m_ButtonEvent[kToolBarTraceLog]		=  (pButtonEvent)&CToolBar::OnLButtonTraceLog;
	m_ButtonEvent[kToolBarTraceModules]		=  (pButtonEvent)&CToolBar::OnLButtonTraceModules;
	m_ButtonEvent[kToolBarTraceMemMap]		=  (pButtonEvent)&CToolBar::OnLButtonTraceMemMap;
	m_ButtonEvent[kToolBarTraceWinHandle]		=  (pButtonEvent)&CToolBar::OnLButtonTraceWinHandle;
	m_ButtonEvent[kToolBarTraceThreads]		=  (pButtonEvent)&CToolBar::OnLButtonTraceThreads;
	m_ButtonEvent[kToolBarTraceSysHandle]		=  (pButtonEvent)&CToolBar::OnLButtonTraceSysHandle;
	m_ButtonEvent[kToolBarTraceCpuView]		=  (pButtonEvent)&CToolBar::OnLButtonTraceCpuView;
	m_ButtonEvent[kToolBarTraceAsmPatch]		=  (pButtonEvent)&CToolBar::OnLButtonTraceAsmPatch;
	m_ButtonEvent[kToolBarTraceCpuStack]		=  (pButtonEvent)&CToolBar::OnLButtonTraceCpuStack;
	m_ButtonEvent[kToolBarTraceRefers]		=  (pButtonEvent)&CToolBar::OnLButtonTraceRefers;
	m_ButtonEvent[kToolBarTraceSource]		=  (pButtonEvent)&CToolBar::OnLButtonTraceSource;
	m_ButtonEvent[kToolBarTraceBpoints]		=  (pButtonEvent)&CToolBar::OnLButtonTraceBpoints;

	m_ButtonEvent[kToolBarTraceMemBreak]		=  (pButtonEvent)&CToolBar::OnLButtonTraceMemBreak;
	m_ButtonEvent[kToolBarTraceHardBreak]		=  (pButtonEvent)&CToolBar::OnLButtonTraceHardBreak;
	m_ButtonEvent[kToolBarTraceOptions]		=  (pButtonEvent)&CToolBar::OnLButtonTraceOptions;
}

CToolBar::~CToolBar()
{

}

CToolBar::operator HWND()
{
	return m_hWnd;
}

BOOL CToolBar::Init( HWND hWnd,HINSTANCE hInst)
{
	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass,sizeof(WNDCLASSEX));
	wndclass.cbSize		= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= CToolBar::WinProc;
	wndclass.cbClsExtra	= 0;
	wndclass.cbWndExtra	= sizeof(CToolBar*);
	wndclass.hInstance		= hInst;
	wndclass.hIcon			= 0;
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH) CreateSolidBrush(RGB(255,255,255));
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= _T("Z0BToolBar");
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
				0,
				_T("Z0BToolBar"),
				NULL,
				WS_VISIBLE |
				WS_CHILD |
				WS_TABSTOP /*|WS_BORDER*/,
				0,
				0,
				0,
				0,
				hWnd,
				(HMENU)IDC_Z0B_TOOLBAR,
				hInst,
				0);

	if (m_hWnd)
	{
		BITMAP Bm;
		LoadButton(hInst,kToolBarOpenFile,IDB_BITMAP_OPEN_FILE);
		LoadButton(hInst,kToolBarRestart,IDB_BITMAP_RESTART);
		LoadButton(hInst,kToolBarCloseDebug,IDB_BITMAP_CLOSE);
		LoadButton(hInst,kToolBarDebugRun,IDB_BITMAP_RUN);
		LoadButton(hInst,kToolBarRunCurThread,IDB_BITMAP_RUN_THREAD);
		LoadButton(hInst,kToolBarDebugPause,IDB_BITMAP_PAUSE);
		LoadButton(hInst,kToolBarStepInto,IDB_BITMAP_STEPIN);
		LoadButton(hInst,kToolBarStepOver,IDB_BITMAP_STEP_OVER);
		LoadButton(hInst,kToolBarTraceInto,IDB_BITMAP_TRACE_IN);
		LoadButton(hInst,kToolBarTraceOver,IDB_BITMAP_TRACE_OVER);
		LoadButton(hInst,kToolBarTraceLog,IDB_BITMAP_TRACE_LOG);
		LoadButton(hInst,kToolBarTraceModules,IDB_BITMAP_MODULES);
		LoadButton(hInst,kToolBarTraceMemMap,IDB_BITMAP_TRACE_MEMORY);
		LoadButton(hInst,kToolBarTraceWinHandle,IDB_BITMAP_WIN_HANDLES);
		LoadButton(hInst,kToolBarTraceThreads,IDB_BITMAP_THREAD_INFO);
		LoadButton(hInst,kToolBarTraceSysHandle,IDB_BITMAP_SYSTEM_HANDLES);
		LoadButton(hInst,kToolBarTraceCpuView,IDB_BITMAP_CPU_DISASM);
		LoadButton(hInst,kToolBarTraceAsmPatch,IDB_BITMAP_PATCH);
		LoadButton(hInst,kToolBarTraceCpuStack,IDB_BITMAP_STACKS);		
		LoadButton(hInst,kToolBarTraceRefers,IDB_BITMAP_REFERS);
		LoadButton(hInst,kToolBarTraceSource,IDB_BITMAP_SOURCECODE);
		LoadButton(hInst,kToolBarTraceBpoints,IDB_BITMAP_BREAKS);
		LoadButton(hInst,kToolBarTraceMemBreak,IDB_BITMAP_BP_MEM);
		LoadButton(hInst,kToolBarTraceHardBreak,IDB_BITMAP_BP_HARD);
		LoadButton(hInst,kToolBarTraceOptions,IDB_BITMAP_OPTIONS);
		SetWindowLongPtr(m_hWnd,0,(LONG_PTR)this);
		ShowWindow(m_hWnd,SW_SHOW);
	}

	return TRUE;
}

void CToolBar::LoadButton(HINSTANCE hInst,int nIndex,int nId)
{
	BITMAP Bm;
	m_BitmapArray[nIndex]		= LoadBitmap(hInst,  MAKEINTRESOURCE(nId));		
	GetObject(m_BitmapArray[nIndex], sizeof(BITMAP), &Bm);
	m_BitmapRect[nIndex].right	= Bm.bmWidth + 1;
	m_BitmapRect[nIndex].bottom	= Bm.bmHeight + 1;
}

LRESULT CToolBar::WinProc(
					HWND hWnd,
					UINT uMsg,
					WPARAM wParam,
					LPARAM lParam)
{
	CToolBar* pData = (CToolBar*)GetWindowLongPtr(hWnd,0);

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
		case WM_ERASEBKGND:
			{
				return 0;
			}
		case WM_SIZE:
			{
				pData->OnSize(wParam,lParam);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				pData->OnLButtonDown(wParam,lParam);
				return 0;
			}
			break;
		case WM_LBUTTONUP:
			{
				pData->OnLButtonUp(wParam,lParam);
				return 0;
			}
			break;
		}

		if ( ( uMsg - WM_USER ) > 0  && ( uMsg - WM_USER ) < kToolBarButtonMAx )
		{
			pData->CallBackEvent( (tagBitmapTool)((uMsg - WM_USER) & 0xFF),wParam,lParam );
		}
	}	

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void CToolBar::CallBackEvent( tagBitmapTool EventIndex,WPARAM wParam,LPARAM lParam )
{
	if ( EventIndex && EventIndex < kToolBarButtonMAx )
	{
		if ( m_ButtonEvent[EventIndex])
		{
			(this->*(this->m_ButtonEvent[EventIndex]))(wParam,lParam);
		}
	}
}

void CToolBar::ResetButton(int nIndex,int TopShow,int nGap ,int bmHeight,int bmWidth)
{
	m_BitmapRect[nIndex].top = TopShow;

	if (nIndex > 0)
	{
		m_BitmapRect[nIndex].left = m_BitmapRect[nIndex-1].right + BUTTON_BITMAP_GAP*nGap;
	}
	else
	{
		m_BitmapRect[nIndex].left = BUTTON_BITMAP_GAP*nGap;
	}
	
	m_BitmapRect[nIndex].bottom = m_BitmapRect[nIndex].top + bmHeight;
	m_BitmapRect[nIndex].right = m_BitmapRect[nIndex].left + bmWidth;
}

void CToolBar::OnSize(WPARAM wParam,LPARAM lParam)
{
	RECT rcClient;
	int bmHeight,bmWidth;
	GetClientRect(m_hWnd,&rcClient);

	int rcHeight = rcClient.bottom - rcClient.top;
	int TopShow = (rcHeight - (m_BitmapRect[kToolBarOpenFile].bottom - m_BitmapRect[kToolBarOpenFile].top))/2;

	bmHeight = m_BitmapRect[kToolBarOpenFile].bottom - m_BitmapRect[kToolBarOpenFile].top;
	bmWidth  = m_BitmapRect[kToolBarOpenFile].right - m_BitmapRect[kToolBarOpenFile].left;

	ResetButton(kToolBarOpenFile,TopShow,3,bmHeight,bmWidth);
	ResetButton(kToolBarRestart,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarCloseDebug,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarDebugRun,TopShow,3,bmHeight,bmWidth);
	ResetButton(kToolBarRunCurThread,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarDebugPause,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarStepInto,TopShow,3,bmHeight,bmWidth);
	ResetButton(kToolBarStepOver,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceInto,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceOver,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceLog,TopShow,3,bmHeight,bmWidth);
	ResetButton(kToolBarTraceModules,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceMemMap,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceWinHandle,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceThreads,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceSysHandle,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceCpuView,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceAsmPatch,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceCpuStack,TopShow,1,bmHeight,bmWidth);	
	ResetButton(kToolBarTraceRefers,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceSource,TopShow,1,bmHeight,bmWidth);

	ResetButton(kToolBarTraceBpoints,TopShow,3,bmHeight,bmWidth);
	ResetButton(kToolBarTraceMemBreak,TopShow,1,bmHeight,bmWidth);
	ResetButton(kToolBarTraceHardBreak,TopShow,1,bmHeight,bmWidth);

	ResetButton(kToolBarTraceOptions,TopShow,3,bmHeight,bmWidth);
}

void CToolBar::OnPaint(HDC hDC, RECT* prcDirty, BOOL bErase)
{
	HDC  hdcMem;
	RECT rcPaint;
	
	hdcMem = CreateCompatibleDC(hDC);
	GetClientRect(m_hWnd,&rcPaint);
	
	FillRect(hDC,&rcPaint,m_BrushBkg);

	DrawButton(kToolBarOpenFile,hDC,hdcMem); 
	DrawButton(kToolBarRestart,hDC,hdcMem); 
	DrawButton(kToolBarCloseDebug,hDC,hdcMem); 
	DrawButton(kToolBarDebugRun,hDC,hdcMem); 
	DrawButton(kToolBarRunCurThread,hDC,hdcMem); 
	DrawButton(kToolBarDebugPause,hDC,hdcMem);
	DrawButton(kToolBarStepInto,hDC,hdcMem);	
	DrawButton(kToolBarStepOver,hDC,hdcMem);
	DrawButton(kToolBarTraceInto,hDC,hdcMem);
	DrawButton(kToolBarTraceOver,hDC,hdcMem);
	DrawButton(kToolBarTraceLog,hDC,hdcMem);
	DrawButton(kToolBarTraceModules,hDC,hdcMem);
	DrawButton(kToolBarTraceMemMap,hDC,hdcMem);
	DrawButton(kToolBarTraceWinHandle,hDC,hdcMem);
	DrawButton(kToolBarTraceThreads,hDC,hdcMem);
	DrawButton(kToolBarTraceSysHandle,hDC,hdcMem);
	DrawButton(kToolBarTraceCpuView,hDC,hdcMem);
	DrawButton(kToolBarTraceAsmPatch,hDC,hdcMem);
	DrawButton(kToolBarTraceCpuStack,hDC,hdcMem);	
	DrawButton(kToolBarTraceRefers,hDC,hdcMem);
	DrawButton(kToolBarTraceSource,hDC,hdcMem);
	DrawButton(kToolBarTraceBpoints,hDC,hdcMem);
	DrawButton(kToolBarTraceMemBreak,hDC,hdcMem);
	DrawButton(kToolBarTraceHardBreak,hDC,hdcMem);
	DrawButton(kToolBarTraceOptions,hDC,hdcMem);

	if (m_nSelectButton >= 0 && m_nSelectButton < kToolBarButtonMAx)
	{
		DrawButton(m_nSelectButton,hDC,hdcMem,TRUE);
		
	}

 	DeleteDC(hdcMem);
	SelectObject(hDC,NULL);
}

void CToolBar::DrawButton(int nIndex,HDC& PaintDc,HDC  hdcMem ,BOOL IsButtonDown )
{
	RECT rcClient;
	rcClient.top = m_BitmapRect[nIndex].top;
	rcClient.left = m_BitmapRect[nIndex].left;
	rcClient.right = m_BitmapRect[nIndex].right;
	rcClient.bottom = m_BitmapRect[nIndex].bottom;

	FillRect(PaintDc,&rcClient,m_BrushArray[kToolBarBlackPen]);
	SelectObject(hdcMem, m_BitmapArray[nIndex]);

	if (IsButtonDown)
	{
		BitBlt(
			PaintDc, 
			m_BitmapRect[nIndex].left + 1,
			m_BitmapRect[nIndex].top + 1, 
			m_BitmapRect[nIndex].right - m_BitmapRect[nIndex].left,
			m_BitmapRect[nIndex].bottom - m_BitmapRect[nIndex].top,
			hdcMem,
			0,
			0,
			SRCCOPY);
	}
	else
	{
		BitBlt(
			PaintDc, 
			m_BitmapRect[nIndex].left ,
			m_BitmapRect[nIndex].top, 
			m_BitmapRect[nIndex].right - m_BitmapRect[nIndex].left,
			m_BitmapRect[nIndex].bottom - m_BitmapRect[nIndex].top,
			hdcMem,
			0,
			0,
			SRCCOPY);
	}	
}

void CToolBar::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	if ( GetCapture() == m_hWnd)
	{
		ReleaseCapture();
	}

	m_nSelectButton = -1;

	RedrawWindow(m_hWnd,0,0,RDW_ALLCHILDREN | RDW_INVALIDATE);
}

void CToolBar::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	RECT	Rect;
	POINT	MousePt;
	SetFocus(m_hWnd);
	GetCursorPos(&MousePt);
	ScreenToClient(m_hWnd, &MousePt);
	GetClientRect(m_hWnd, &Rect);
	SetCapture(m_hWnd);


	for ( int i = 0; i < kToolBarButtonMAx ; i++ )
	{
		if (PtInRect(&m_BitmapRect[i],MousePt))
		{
			m_nSelectButton = i;
			PostMessage(m_hWnd,WM_USER + i,0,0 );
		}
	}

	RedrawWindow(m_hWnd,0,0,RDW_ALLCHILDREN | RDW_INVALIDATE);

}

void CToolBar::OnLButtonOpenFile(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonRestart(WPARAM wParam,LPARAM lParam)
{

}
void CToolBar::OnLButtonCloseDebug(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonDebugRun(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonRunCurThread(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonDebugPause(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonStepInto(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonStepOver(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceInto(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceOver(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceLog(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceModules(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceMemMap(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceWinHandle(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceThreads(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceSysHandle(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceCpuView(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceAsmPatch(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceCpuStack(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceRefers(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceSource(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceBpoints(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceMemBreak(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceHardBreak(WPARAM wParam,LPARAM lParam)
{

}

void CToolBar::OnLButtonTraceOptions(WPARAM wParam,LPARAM lParam)
{

}
