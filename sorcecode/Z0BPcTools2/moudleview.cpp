#include "stdafx.h"
#include "moudleview.h"

CMoudleView::CMoudleView()
{

}

CMoudleView::~CMoudleView()
{

}

BOOL CMoudleView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  基址" , 100 );
		InsertColumn( 1 ,L"  大小" , 100 );
		InsertColumn( 2 ,L"  入口点" , 100 );
		InsertColumn( 3 ,L"  名称" , 100 );
		InsertColumn( 4 ,L"  文件版本" , 100 );
		InsertColumn( 5 ,L"  路径" , 300 );
		
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CMoudleClassView::CMoudleClassView()
	:CustomClassView()
{

}

CMoudleClassView::~CMoudleClassView()
{

}

BOOL CMoudleClassView::OnClose()
{
	return TRUE;
}

CMoudleClassView* CMoudleClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CMoudleClassView* pNewView = (CMoudleClassView*)CustomClassView<CMoudleClassView,CMoudleView>::Init(hInst,hWndParent,uID,_T("CMoudleClassView"),_T("执行模块"));
	if ( pNewView)
	{
		CElemView<CMoudleView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}