#include "stdafx.h"
#include "bpointview.h"

CBPointView::CBPointView()
{

}

CBPointView::~CBPointView()
{

}

BOOL CBPointView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
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

CBPointClassView::CBPointClassView()
	:CustomClassView()
{

}

CBPointClassView::~CBPointClassView()
{

}

BOOL CBPointClassView::OnClose()
{
	return TRUE;
}

CBPointClassView* CBPointClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CBPointClassView* pNewView = (CBPointClassView*)CustomClassView<CBPointClassView,CBPointView>::Init(hInst,hWndParent,uID,_T("CBPointClassView"),_T("软件断点"));
	if ( pNewView)
	{
		CElemView<CBPointView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}