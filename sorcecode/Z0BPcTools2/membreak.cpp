#include "stdafx.h"
#include "membreak.h"

CMemBreakView::CMemBreakView()
{

}

CMemBreakView::~CMemBreakView()
{

}

BOOL CMemBreakView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  地址" , 80 );
		InsertColumn( 1 ,L"  模块" , 100 );
		InsertColumn( 2 ,L"  激活" , 120 );
		InsertColumn( 3 ,L"  反汇编" , 260 );
		InsertColumn( 4 ,L"  注释" , 360 );

		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CMemBreakClassView::CMemBreakClassView()
	:CustomClassView()
{

}

CMemBreakClassView::~CMemBreakClassView()
{

}

BOOL CMemBreakClassView::OnClose()
{
	return TRUE;
}

CMemBreakClassView* CMemBreakClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CMemBreakClassView* pNewView = (CMemBreakClassView*)CustomClassView<CMemBreakClassView,CMemBreakView>::Init(hInst,hWndParent,uID,_T("CMemBreakClassView"),_T("内存断点"));
	if ( pNewView)
	{
		CElemView<CMemBreakView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}