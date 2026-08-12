#ifndef _COllyAsmView_H
#define _COllyAsmView_H

#include "dynamicview.h"
#include "../udis86/udis86.h"
#include "../udis86/libudis86/types.h"
#include "../udis86/libudis86/extern.h"



class COllyAsmView : public CDynamicView
{
public:
	COllyAsmView(void);
	virtual ~COllyAsmView(void);

public:
	BOOL Init(HINSTANCE hInst,HWND hWnd);

protected:
	BOOL DecodeFromMem(PBYTE pBuffer, uint64_t cBufferSize,float  flTotle,uint64_t VirtualOffset,BOOL	bReNew);

	virtual void OnPaintColumn(
		CMemDC& PaintDC,
		CRect& ClientRect,
		int Item, 
		int Column,
		CDynamicDrawItem* DrawItemData,
		BOOL bSelected = FALSE);

	virtual CDynamicDrawItem* OnGetDrawData();
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw);
	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual int	 OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect);
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);
	virtual void OnClickItemUp();
	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();

protected:
	afx_msg BOOL OnSetFocus( WPARAM wp,LPARAM lp,BOOL& bDef);

private:
	int m_ShowFlags;
};

#endif