#include "stdafx.h"
#include "winhanleview.h"

CWinHandleView::CWinHandleView()
{

}

CWinHandleView::~CWinHandleView()
{

}

BOOL CWinHandleView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  句柄" , 80 );
		InsertColumn( 1 ,L"  标题" , 120 );
		InsertColumn( 2 ,L"  数据块" , 80 );
		InsertColumn( 3 ,L"  父窗口" , 80 );
		InsertColumn( 4 ,L"  WinProc" , 80 );
		InsertColumn( 5 ,L"  ID" , 70 );
		InsertColumn( 6 ,L"  风格" , 80 );
		InsertColumn( 7 ,L"  扩展风格" , 80 );
		InsertColumn( 8 ,L"  线程" , 80 );
		InsertColumn( 9 ,L"  ClsProc" , 80 );
		InsertColumn( 10 ,L"  类" , 80 );

		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CWinHandleClassView::CWinHandleClassView()
	:CustomClassView()
{

}

CWinHandleClassView::~CWinHandleClassView()
{

}

BOOL CWinHandleClassView::OnClose()
{
	return TRUE;
}

CWinHandleClassView* CWinHandleClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CWinHandleClassView* pNewView = (CWinHandleClassView*)CustomClassView<CWinHandleClassView,CWinHandleView>::Init(hInst,hWndParent,uID,_T("CWinHandleClassView"),_T("窗口"));
	if ( pNewView)
	{
		CElemView<CWinHandleView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}