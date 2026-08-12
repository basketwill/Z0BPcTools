#include "stdafx.h"
#include "threadview.h"

CThreadView::CThreadView()
{

}

CThreadView::~CThreadView()
{

}

BOOL CThreadView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  标识Id" , 120 );
		InsertColumn( 1 ,L"  入口点" , 120 );
		InsertColumn( 2 ,L"  数据块" , 120 );
		InsertColumn( 3 ,L"  最近错误" , 120 );
		InsertColumn( 4 ,L"  状态" , 120 );
		InsertColumn( 5 ,L"  优先权" , 120 );
		InsertColumn( 6 ,L"  用户时间" , 120 );
		InsertColumn( 7 ,L"  内核时间" , 120 );
		
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CThreadClassView::CThreadClassView()
	:CustomClassView()
{

}

CThreadClassView::~CThreadClassView()
{

}

BOOL CThreadClassView::OnClose()
{
	return TRUE;
}

CThreadClassView* CThreadClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CThreadClassView* pNewView = (CThreadClassView*)CustomClassView<CThreadClassView,CThreadView>::Init(hInst,hWndParent,uID,_T("ThreadClassView"),_T("线程"));
	if ( pNewView)
	{
		CElemView<CThreadView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}