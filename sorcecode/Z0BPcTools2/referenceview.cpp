#include "stdafx.h"
#include "referenceview.h"

CReferenceView::CReferenceView()
{

}

CReferenceView::~CReferenceView()
{

}

BOOL CReferenceView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  地址" , 100 );
		InsertColumn( 1 ,L"  反汇编" , 150 );
		InsertColumn( 2 ,L"  注释" , 500 );

		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CReferenceClassView::CReferenceClassView()
	:CustomClassView()
{

}

CReferenceClassView::~CReferenceClassView()
{

}

BOOL CReferenceClassView::OnClose()
{
	return TRUE;
}

CReferenceClassView* CReferenceClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CReferenceClassView* pNewView = (CReferenceClassView*)CustomClassView<CReferenceClassView,CReferenceView>::Init(hInst,hWndParent,uID,_T("CReferenceView"),_T("参考引用数据"));
	if ( pNewView)
	{
		CElemView<CReferenceView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}