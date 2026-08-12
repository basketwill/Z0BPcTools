#include "stdafx.h"
#include "hardbreak.h"

CHardBreakView::CHardBreakView()
{

}

CHardBreakView::~CHardBreakView()
{

}

BOOL CHardBreakView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
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

CHardBreakClassView::CHardBreakClassView()
	:CustomClassView()
{

}

CHardBreakClassView::~CHardBreakClassView()
{

}

BOOL CHardBreakClassView::OnClose()
{
	return TRUE;
}

CHardBreakClassView* CHardBreakClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CHardBreakClassView* pNewView = (CHardBreakClassView*)CustomClassView<CHardBreakClassView,CHardBreakView>::Init(hInst,hWndParent,uID,_T("CHardBreakClassView"),_T("硬件断点"));
	if ( pNewView)
	{
		CElemView<CHardBreakView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}