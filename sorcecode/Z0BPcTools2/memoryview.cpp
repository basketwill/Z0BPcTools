#include "stdafx.h"
#include "memoryview.h"

CMemoryView::CMemoryView()
{

}

CMemoryView::~CMemoryView()
{

}

BOOL CMemoryView::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if (__super::Init(hInst,hWnd,Id,szClass,szTitle))
	{
		InsertColumn( 0 ,L"  地址" , 100 );
		InsertColumn( 1 ,L"  大小" , 100 );
		InsertColumn( 2 ,L"  节名" , 100 );
		InsertColumn( 3 ,L"  区段" , 100 );
		InsertColumn( 4 ,L"  包含" , 100 );
		InsertColumn( 5 ,L"  类型" , 100 );
		InsertColumn( 6 ,L"  访问" , 100 );
		InsertColumn( 7 ,L"  初始访问" , 100 );
		InsertColumn( 8 ,L"  映射名称" , 300 );

		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);

		return TRUE;
	}

	return FALSE;
}

CMemoryClassView::CMemoryClassView()
	:CustomClassView()
{

}

CMemoryClassView::~CMemoryClassView()
{

}

BOOL CMemoryClassView::OnClose()
{
	return TRUE;
}

CMemoryClassView* CMemoryClassView::Init( HINSTANCE hInst,HWND hWndParent,int uID )
{
	CMemoryClassView* pNewView = (CMemoryClassView*)CustomClassView<CMemoryClassView,CMemoryView>::Init(hInst,hWndParent,uID,_T("CMemoryClassView"),_T("内存映射"));
	if ( pNewView)
	{
		CElemView<CMemoryView>* pEl = pNewView->AddView(NULL,kViewToFullSrceen);
		if (pEl)
		{
			return pNewView;
		}
	}

	return pNewView;
}