#include "StdAfx.h"
#include "ollyasmview.h"
#include "resource.h"
#include "disassem.h"
#include "global.h"

#define IDC_OLLYDBG_ASM_VIEW  IDC_CPU_ASM_VIEW+500

COllyAsmView::COllyAsmView(void)
	:m_ShowFlags(kCpuAsmView)
{
}


COllyAsmView::~COllyAsmView(void)
{
}

BOOL COllyAsmView::Init( HINSTANCE hInst,HWND hWnd )
{
	if(Create(hWnd,hInst,IDC_OLLYDBG_ASM_VIEW,_T("COllyAsmView")))
	{
		__super::Init(0);
		InsertHeader(0,100,_T(" 地址"));
		InsertHeader(1,150,_T(" Hex数据"));
		InsertHeader(2,400,_T(" 反汇编"));
		InsertHeader(3,300,_T(" 注释"));
		ShowVScrollBar(TRUE);
		SetBodyCxSize(0);
		
		return TRUE;
	}

	return FALSE;
}

BOOL COllyAsmView::DecodeFromMem(
							PBYTE pBuffer, 
							uint64_t cBufferSize,
							float  flTotle,
							uint64_t VirtualOffset,
							BOOL	bReNew)
{
	return FALSE;
}

BOOL COllyAsmView::OnSetFocus( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	HWND hParent = GetParent(m_hWnd);

	if (hParent)
	{
		::SendMessage(hParent,WM_CPU_VIEW_LBUTTON,0,(LPARAM)m_ShowFlags);
	}

	return __super::OnSetFocus(wp,lp,bDef);
}

void COllyAsmView::OnPaintColumn(
						CMemDC& PaintDC,
						CRect& ClientRect,
						int Item, 
						int Column,
						CDynamicDrawItem* DrawItemData,
						BOOL bSelected)
{
	SetItemState(Item,kDynamicItemUnShowData);
}

CDynamicDrawItem* COllyAsmView::OnGetDrawData()
{
	return (CDynamicDrawItem*)666;
}

void COllyAsmView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{

}

void COllyAsmView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{

}

int	 COllyAsmView::OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect)
{
	return 0;
}

PVOID COllyAsmView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	return pDraw;
}

void COllyAsmView::ReleaseItemData(CDynamicDrawItem* pDraw)
{

}

void COllyAsmView::OnClickItemUp()
{

}

int COllyAsmView::OnMoveScroll(int Pos)
{
	return 0;
}

void COllyAsmView::OnTimerSelectUp()
{

}

void COllyAsmView::OnTimerSelectDown()
{

}