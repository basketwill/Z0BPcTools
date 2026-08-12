#include "stdafx.h"
#include "syshandlesview.h"

CSysHandleView::CSysHandleView()
{

}

CSysHandleView::~CSysHandleView()
{

}

BOOL CSysHandleView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  句柄" , 80 );
		InsertColumn( 1 ,L"  类型" , 120 );
		InsertColumn( 2 ,L"  参考" , 80 );
		InsertColumn( 3 ,L"  访问" , 80 );
		InsertColumn( 4 ,L"  T" , 80 );
		InsertColumn( 5 ,L"  信息" , 80 );
		InsertColumn( 6 ,L"  名称" , 260 );
		
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CSysHandleClassView::CSysHandleClassView()
	:CustomClassView()
{

}

CSysHandleClassView::~CSysHandleClassView()
{

}

BOOL CSysHandleClassView::OnClose()
{
	return TRUE;
}

CSysHandleClassView* CSysHandleClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CSysHandleClassView* pNewView = (CSysHandleClassView*)CustomClassView<CSysHandleClassView,CSysHandleView>::Init(hInst,hWndParent,uID,_T("CSysHandleClassView"),_T("系统句柄"));
	if ( pNewView)
	{
		CElemView<CSysHandleView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}