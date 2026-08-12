#include "stdafx.h"
#include "mdi.h"
#include "resource.h"
#include "disassem.h"


CMdi::CMdi()
	:m_hInstance(NULL)
	,m_hWnd(NULL)
	,m_CpuView(NULL)
	,m_ThreadView(NULL)
    ,m_WinHandleView(NULL)
	,m_SysHandleView(NULL)
	,m_MemoryView(NULL)
	,m_ModuleView(NULL)
	,m_LogView(NULL)
	,m_ReferenceView(NULL)
	,m_BPointView(NULL)
	,m_StackInfoView(NULL)
	,m_MemBreakView(NULL)
	,m_HardBreakView(NULL)
{
	ZeroMemory(
			&m_ClientStruct,
			sizeof(CLIENTCREATESTRUCT));

	m_CpuView = NULL;

	ZeroMemory(&m_IdaViewList,sizeof(tagIDA_VIEW));
}

CMdi::~CMdi()
{
	if (m_CpuView)
	{
		
	}
}

CMdi::operator HWND()
{
	return m_hWnd;
}

BOOL CMdi::Init(HINSTANCE hIns,HWND hParent)
{
	m_ClientStruct.hWindowMenu	= NULL;
	m_ClientStruct.idFirstChild	= IDM_FIRSTCHILD;
	m_hInstance = hIns;
	RECT rcClient;
	GetClientRect(hParent,&rcClient);

	m_hWnd = CreateWindow(
					_T("MDICLIENT"),
					NULL,
					WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
					0,
					0, 
					0,
					0,
					hParent,
					NULL,
					hIns,
					(void *) &m_ClientStruct);

	if (m_hWnd)
	{
	 	MoveWindow(m_hWnd, 
		 				0, 
		 				0, 
		 				rcClient.right, //width
		 				rcClient.bottom, //height
		 				true);

		m_StackInfoView		= CStackClassView::Init(hIns,m_hWnd,IDI_ICON_STACKINFO);
		m_ReferenceView		= CReferenceClassView::Init(hIns,m_hWnd,IDI_ICON_REFERENCE);
		m_LogView			= CLogClassView::Init(hIns,m_hWnd,IDI_ICON_LOG);
		m_ModuleView		= CMoudleClassView::Init(hIns,m_hWnd,IDI_ICON_MOUDLES);
		m_MemoryView		= CMemoryClassView::Init(hIns,m_hWnd,IDI_ICON_MEMORY);
		m_SysHandleView		= CSysHandleClassView::Init(hIns,m_hWnd,IDI_ICON_HANDLES);
		m_WinHandleView		= CWinHandleClassView::Init(hIns,m_hWnd,IDI_ICON_WINHANDLES);
		m_ThreadView		= CThreadClassView::Init(hIns,m_hWnd,IDI_ICON_THREAD);
		m_BPointView		= CBPointClassView::Init(hIns,m_hWnd,IDI_ICON_BREAKPOINT);
		m_MemBreakView		= CMemBreakClassView::Init(hIns,m_hWnd,IDI_ICON_MEMBREAK);
		m_HardBreakView		= CHardBreakClassView::Init(hIns,m_hWnd,IDI_ICON_HARD_BREAK);
		m_CpuView			= CCpuView::Init(hIns,m_hWnd,_T("CPUASM"),_T("CPU"));
		
	}

	return FALSE;
}

HRESULT CMdi::DefFrameProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (hwnd)
	{
		return ::DefFrameProc(hwnd,m_hWnd, uMsg, wParam, lParam);
	}

	return 0;
	
}


int CMdi::StartDebugProcess(OpenProcessDebug& OpenDebug)
{
	if (m_CpuView)
	{
		return m_CpuView->CpuOpenProcessDebug(OpenDebug);
	}

	return 0;
}

int CMdi::OpenWithIda(LPTSTR PathFile )
{
	if ( m_IdaViewList._Ida_Count < MAX_IDA_VIEW_LIST_SIZE )
	{
		CIdaView* pView = NULL;
		if( pView = m_IdaViewList._Ida[m_IdaViewList._Ida_Count] = CIdaView::Init(
																	m_hInstance,
																	m_hWnd,
																	_T("IDAVIEW"),
																	PathFile,
																	m_IdaViewList._Ida_Count))
		{
			m_IdaViewList._Ida_Count++;
			pView->ShowIdaView(PathFile);
			
		}
		
	}

	return 0;
}

