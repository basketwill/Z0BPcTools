#ifndef _CDynamicView_h
#define _CDynamicView_h
#include "winbase.h"
#include "common.h"
#include <strsafe.h>

typedef ULONGLONG POS;
#define MAX_CUSTOM_PEN 10
#define DYNAMIC_DEFAULT_ITEM_COUNT 1000
#define VIEW_ITEM_SELECT_COLOR	RGB(192,192,192)
#define VIEW_SELECT_BACK_COLOR	RGB(192,192,192)
#define VIEW_NORMAL_BACK_COLOR	RGB(255,255,245)

#define VIEW_NORMAL_WHITE_LINE	RGB(255,255,255)

#define VIEW_HEADER_BACK_COLOR RGB(192,192,192)
#define VIEW_HEADER_LINE_COLOR  RGB(0,0,0)
#define VIEW_TEXT_DEFAULT_COLOR RGB(0,0,0)

#define VIEW_BODY_LINE_COLOR	RGB(128,128,128)
#define DYNAMIC_HEADER_HEIGHT 15 //头高度
#define DYNAMIC_ITEM_HEIGHT 12
#define DYNAMIC_BORDER_CX	  0 //边框宽度

#define DYNAMIC_COLUMN_MAX_SIZE 17

#define WM_TIMEER_MOV_SELECT_UP 1
#define WM_TIMEER_MOV_SELECT_DOWN 2
#define TIMER_SELECT 50

typedef enum __tagTraceFlags
{
	kMouseTraceNone = -1,
	kMouseTraceButtonDown = 0,

	kButtonDown = 0x1,
	kButtonUp	= 0x2,
}tagTraceFlags;

class CDynamicData
{
	friend class CDynamicView;
public:
	CDynamicData()
		:m_DydData(NULL)
		,m_DydSize(0)
	{

	}

	~CDynamicData()
	{

	}

	BYTE*& Data()
	{
		return m_DydData;
	}

	ULONG_PTR& Size()
	{
		return m_DydSize;
	}

protected:
	BYTE*			m_DydData;
	ULONG_PTR		m_DydSize;

protected:
	virtual void GetDynamicData() = 0;

};

typedef void (__cdecl *pGetDynamicData)(const CDynamicData& DynamicData);

typedef enum _tagDynamicItemState
{
	kDynamicItemInit		= 0,
	kDynamicItemUnShowData	= 0,
	kDynamicItemShowData	= 1,
	kDynamicItemSelect		= 1<<1,
	kDynamicItemClicked		= 1<<2,

	kDynamicClickedDown		= 0,
	kDynamicDBClickedDown	= 1,
	kDynamicRClickedDown	= 2,
};

#pragma pack(push,1)

class CDynamicArrayItem
{
	friend class CDynamicView;
public:
	CDynamicArrayItem()
		:m_State(kDynamicItemInit)
	{

	}

	~CDynamicArrayItem()
	{

	}

protected:
	BOOLEAN m_State;
};

#pragma pack(pop)

#ifndef __tagDynamicColumn
#define __tagDynamicColumn

struct DynamicColumn
{
	DynamicColumn()
	{
		ZeroMemory(this,sizeof(*this));
	}

	TCHAR*		DycText;
	ULONG		DycSize;
	ULONG		DycFlags;
};

#endif

#ifndef _DynamicHeader
#define _DynamicHeader

struct DynamicHeader
{
	DynamicHeader()
	{
		ZeroMemory(this,sizeof(*this));
	}

	ULONG& Height()
	{
		return DyhHeight;
	}

	ULONG			DyhHeight;
	ULONG			DyhColumnNums;
	DynamicColumn*	DyhColumn[DYNAMIC_COLUMN_MAX_SIZE];

};

#endif

class CDynamicText
{
public:
	CDynamicText()
		:m_Next(NULL)
		,m_szText(NULL)
		,m_clrBkGround(-1)
		,m_clrText(VIEW_TEXT_DEFAULT_COLOR)
	{

	}

	virtual ~CDynamicText()
	{
		if (m_szText)
		{
			free(m_szText);
			m_szText = NULL;
		}
	}

	CDynamicText*& Next()
	{
		return m_Next;
	}

	LPTSTR& Text()
	{
		return m_szText;
	}

	COLORREF& BkColor()
	{
		return m_clrBkGround;
	}

	COLORREF& TextColor()
	{
		return m_clrText;
	}

	CRect& Rect()
	{
		return m_rcClient;
	}

private:
	LPTSTR			m_szText;
	CRect			m_rcClient;
	COLORREF		m_clrBkGround;
	COLORREF		m_clrText;
	CDynamicText*   m_Next;
};

class CDynamicPaint
{
public:
	CDynamicPaint()
	{

	}

	virtual ~CDynamicPaint()
	{

	}
private:
	CBrush m_hbSelect;
	CBrush m_hnNormal;
};

class CDynamicBody
{
	friend class CDynamicView;
public:
	CDynamicBody()
		:m_DybHeight(DYNAMIC_ITEM_HEIGHT)
		,m_DybPagesSize(0)
		,m_DybIsMultiSelect(TRUE)
		,m_DybMoveDirect(-1)
	{
		m_DybMouseClickPos.x = 0;
		m_DybMouseClickPos.y = 0;
		m_DybSelectTimer[0] = 0;
		m_DybSelectTimer[1] = 0;
	}
	virtual ~CDynamicBody()
	{

	}
protected:
	ULONG				m_DybHeight;
	ULONG				m_DybPagesSize;
	BOOL				m_DybIsMultiSelect;
	int					m_DybMoveDirect;
	UINT_PTR			m_DybSelectTimer[2];
	CPoint				m_DybMouseClickPos;
	CDynamicArrayItem	m_DybItem[DYNAMIC_DEFAULT_ITEM_COUNT];
public:
	ULONG& Height()
	{
		return m_DybHeight;
	}

	ULONG& Size()
	{
		return m_DybPagesSize;
	}
};

typedef CPoint MOUSEPOS;

typedef struct __tagDynamicView
{
	ULONG64			DyvScrollLines;
	ULONG			DyvPerPagesLine;
	POS				DyvScrollPos;
	int				DyvShowScrollBar;
	MOUSEPOS		DyvMouseTacePos;
	COLORREF		DyvNormalColor;
	COLORREF		DyvSelectColor;
	HFONT			DyvPaintFont;
	CBrush			DyvPaintSelectBrush;
	CBrush			DyvPaintNormalBrush;
	CBrush			DyvPaintHeaderBrush;
	CBrush			DyvPaintBorderBrush;
	CPen			DyvPaintHeaderLinePen;
	CPen			DyvPaintViewWhitePen;
	CPen			DyvPaintViewBodyLinePen;
	ULONG			DyvBorderSize;

	BOOL			DyvTrackerMouseLine;
	BOOL			DyvOnFocus;
	DynamicHeader	DyvHeader;
	CDynamicBody	DyvBody;
	CDynamicData*	DyvData;
	
}DynamicView;

class CDynamicDrawItem
{
public:
	CDynamicDrawItem()
	{
		ZeroMemory(this,sizeof(*this));
	}

	virtual ~CDynamicDrawItem()
	{
		if (m_DataSize)
		{
			delete[] (BYTE*)m_Data;
			m_Data = NULL;
			m_DataSize = 0;
		}
	}

	int& Item()
	{
		return m_Item;
	}

	int& Column()
	{
		return m_Column;
	}

	PVOID& ExtData()
	{
		return m_Data;
	}

	BOOL& EndDraw()
	{
		return m_EndDraw;
	}

protected:
	int		m_Item;
	int		m_Column;
	BOOL	m_EndDraw;
	ULONG	m_DataSize;
	PVOID	m_Data;

};

#pragma pack(push,1)

class CDynamicClickItem : public CDynamicDrawItem
{
	friend class CDynamicView;
public:
	CDynamicClickItem()
		:m_ClickType(0)
	{

	}

	virtual ~CDynamicClickItem()
	{

	}

	BOOLEAN& Type()
	{
		return m_ClickType;
	}

protected:
	BOOLEAN		m_ClickType;
	MOUSEPOS	m_ClickPos;
};

#pragma pack(pop)

class CDynamicView : public CWinBase<CDynamicView>
{
	DECLARE_MSG_MAP()
public:
	CDynamicView(void);
	virtual ~CDynamicView(void);

	void SetColor(
			COLORREF Normal = 0,
			COLORREF Select = VIEW_ITEM_SELECT_COLOR);

	void SetHeaderHeight( int Height = DYNAMIC_HEADER_HEIGHT);

	void InsertHeader( int Column ,int Width ,LPTSTR lpText );
	void ModifyHeaderText( int nColumn,LPTSTR lpText,int Width = -1);
	void ModifyHeaderWidth( int nColumn,int Width );
	void Init( CDynamicData* pData);

protected:
	DynamicView		m_DynamicView;
	HPEN			m_CustomPen[MAX_CUSTOM_PEN];
	int				m_ShowFlags;
protected:
	virtual void DrawBorder(CMemDC& PaintDC,CRect& ClientRect);
	virtual void DrawHeader(CMemDC& PaintDC,CRect& ClientRect);
	virtual void DrawBody(CMemDC& PaintDC,CRect& ClientRect);
	
	virtual void OnPaintColumn(
							CMemDC& PaintDC,
							CRect& ClientRect,
							int Item,
							int Column,
							CDynamicDrawItem* DrawItemData,
							BOOL bSelected = FALSE );
	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual void OnClickHeadDwon(int nColumn);
	virtual void OnClickHeadUp(int nColumn);

	virtual void OnClickItemUp();
	virtual int OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect);
	void SetItemState( int Item,int State);
	int  GetItemState( int Item);
	void ShowHScrollBar(BOOL bShow );
	void ShowVScrollBar(BOOL bShow );
	void SetHeaderClick(int nColumn,int Clicked);
	BOOL GetFocusState()
	{
		return m_DynamicView.DyvOnFocus;
	}

protected:
	virtual CDynamicDrawItem* OnGetDrawData() = 0;
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw) = 0;

	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw) = 0;
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw) = 0;
	virtual int OnMoveScroll(int Pos) = 0;
	virtual	void OnTimerSelectUp() = 0;
	virtual	void OnTimerSelectDown() = 0;
protected:
	void SetBodyCxSize(int nCx)
	{
		m_DynamicView.DyvBorderSize = nCx;
	}
private:
	int GetHeaderColumnSize(int Column );
	void SetHeaderColumnSize(int Column,int DycSize);
	int GetHeaderColumnHeight();
	int GetBodyItemHeigth();
	LPTSTR GetHeaderColumnText(int Column);

	void OnViewScrollMove(int iWhat,int MoveStep = 1);
protected:
	afx_msg BOOL OnNcCreate( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnPaint( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnNCClassSize( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnNcPaint( WPARAM wp,LPARAM lp,BOOL& bDef); 
	afx_msg BOOL OnNcLButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnSize( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnEraseBkgnd( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnMouseMove( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnMouseLeave( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnTimer( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnKillFocus( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnSetFocus( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnLButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnLButtonDBClick( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnLButtonUp( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnRButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnKeyDown( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnVScroll( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnHScroll( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnMouseWheel( WPARAM wp,LPARAM lp,BOOL& bDef);
	afx_msg BOOL OnMouseHWheel( WPARAM wp,LPARAM lp,BOOL& bDef);

};

#endif