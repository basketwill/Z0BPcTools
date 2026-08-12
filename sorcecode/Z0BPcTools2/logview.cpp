#include "stdafx.h"
#include "logview.h"

CLogView::CLogView()
{

}

CLogView::~CLogView()
{

}

BOOL CLogView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  地址" , 100 );
		InsertColumn( 1 ,L"  消息" , 500 );		

		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CLogClassView::CLogClassView()
	:CustomClassView()
{

}

CLogClassView::~CLogClassView()
{

}

BOOL CLogClassView::OnClose()
{
	return TRUE;
}

CLogClassView* CLogClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CLogClassView* pNewView = (CLogClassView*)CustomClassView<CLogClassView,CLogView>::Init(hInst,hWndParent,uID,_T("CLogClassView"),_T("日志数据"));
	if ( pNewView)
	{
		CElemView<CLogView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}