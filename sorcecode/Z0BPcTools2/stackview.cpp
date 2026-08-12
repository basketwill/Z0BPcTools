#include "stdafx.h"
#include "stackview.h"

CStatckKView::CStatckKView()
{

}

CStatckKView::~CStatckKView()
{

}

BOOL CStatckKView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  地址" , 80 );
		InsertColumn( 1 ,L"  堆栈" , 100 );
		InsertColumn( 2 ,L"  函数过程/参数" , 260 );
		InsertColumn( 3 ,L"  调用来自" , 260 );
		InsertColumn( 4 ,L"  结构" , 360 );

		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CStackClassView::CStackClassView()
	:CustomClassView()
{

}

CStackClassView::~CStackClassView()
{

}

BOOL CStackClassView::OnClose()
{
	return TRUE;
}

CStackClassView* CStackClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CStackClassView* pNewView = (CStackClassView*)CustomClassView<CStackClassView,CStatckKView>::Init(hInst,hWndParent,uID,_T("CStackClassView"),_T("调用堆栈"));
	if ( pNewView)
	{
		CElemView<CStatckKView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}