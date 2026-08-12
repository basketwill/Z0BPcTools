/* File custom.h
 * (custom control interface)
 */

#ifndef CUSTOM_VIEW_H
#define CUSTOM_VIEW_H

#include <tchar.h>
#include <windows.h>
#include <stdint.h>
// Window class:
#define CUSTOM_WC              _T("CustomControl")
#ifndef MAX
#define MAX(a,b)    ((a) > (b) ? (a) : (b))
#endif

// Cell width and height.
#define CELL_CX          80
#define CELL_CY          12

// Row header width and column header height.
#define HEADER_CX        0
#define HEADER_CY        15

// Scroll unit, horizontal and vertical.
#define SCROLL_CX        10
#define SCROLL_CY        10

// We scroll in tens of pixels.
#define SCROLL_STEP      12

#define MAX_CUSTOM_PEN 10
// Cell colors.
#define HEADER_TEXT_COLOR		RGB(0,0,0)
#define HEADER_BACK_COLOR		RGB(192,192,192)
#define NORMAL_TEXT_COLOR		RGB(0,0,0)
//#define NORMAL_BACK_COLOR_A		RGB(255,251,240)
#define NORMAL_BACK_COLOR_A		RGB(255,255,255)
#define NORMAL_BACK_COLOR_B		RGB(192,192,192)
#define LINE_COLUMN_GAP			RGB(128,128,128)
#define LINE_COLUMN_FRAME		RGB(128,128,128)
#define NORMAL_LINE_COLOR	    RGB(0,0,0)
#define NORMAL_WHITE_COLOR	    RGB(255,255,255)
#define MOUSE_TIMER_MOVE_SELECT 0xF
#define MOUSE_TIMER				50
#define PER_SCROLL_PAGE_SIZE			10

#define DATA_ADDRESS_COLOR RGB(128,128,128)
#define DATA_ADDRESS_SELECT_COLOR RGB(0,0,0)
// Message to set column range.
// WPARAM: Min. column index
// LPARAM: Max. column index
// Returns TRUE on success.
#define XXM_SETCOLRANGE       (WM_USER + 100)

// Message to set row range.
// WPARAM: Min. row index
// LPARAM: Max. row index
// Returns TRUE on success.
#define XXM_SETROWRANGE       (WM_USER + 101)
#define COLUMN__HEADER_MAX_TEXT_LENGTH 100
#define CUSTOM_MAX_COLUMN  17

#define CUSTOM_ADDRESS_NUMS 20
#define CUSTOM_SCREEN_VIEW_SIZE 200

typedef struct _CustomHeaderData
{
	TCHAR	szText[COLUMN__HEADER_MAX_TEXT_LENGTH];
	int		nWideth;
}CustomHeaderData;

class CCustomHeader
{
public:
	CCustomHeader();
	~CCustomHeader();

	void SetHeight( int Height );
	BOOL InsertColumn( int nColumn , TCHAR* szText , int nWideth );
	void SetColumnWidth( int nColumn , int nWidth );
	ULONG GetColumnWidth( int nColumn );
	LPWSTR GetColumnText( int nColumn );
	ULONG GetColumnHeight();
	ULONG GetMaxColumn();
	
private:
	int m_nColumn;
	int m_nHeigth;
	CustomHeaderData m_HeaderData[CUSTOM_MAX_COLUMN];	
};


typedef struct _tagIcon
{
	union
	{
		COLORREF	colorIcon;
		void*		AdditionData;
	};
	
	HPEN		IconPen;
	int			nIcon;
}tagIcon,*PtagIcon;


typedef struct _tagOption
{
	LPTSTR					lpText;
	int						TextSize;
	BOOL					HasNonFocus;

	COLORREF				FontBgColor;//字体背景颜色
	COLORREF				NorFontColor;//正常显示颜色
	COLORREF				SelFontColor;//正常显示颜色

}tagOption;

typedef struct _tagCustomItemData
{
	
	tagOption*				Option;
	int						OptionCount;	

	tagIcon*				Icon;
	int						IconCount;
	BOOL					bIsClick;
	BOOL					bIsModifyed;
	LPTSTR					lpModifyText;

	HBRUSH					SelBgkBrush;//选择后显示颜色
	HBRUSH					NorBgkBrush;// 背景颜色

	_tagCustomItemData*		Next;

}CustomItemData,*PCustomItemData;

class CustomItemHead
{
public:
	CustomItemHead();	

	~CustomItemHead();
	
	CustomItemHead* Next();

	CustomItemHead* Pre();
	
	uint64_t GetPtrValue();

	uint64_t GetScaler();
public:

	void InsertAddressColumn(int IconNums,LPTSTR lpText,HBRUSH hBgkBrush,HBRUSH hBgkSelect );
	void SetAddressColumnIcon(int IconNums,int nIconType,int nIconType2,int nIconType3 );
	int  GetetAddressColumnIcon(int IconNums);

	void SetColumnItem(int nColumn,int IconNums,tagOption* Option,int nSize,HBRUSH hBgkBrush,HBRUSH hBgkSelect );
	void SetItemText(int nColumn, LPTSTR szText );
	void SetInstructionType( uint8_t InsType);
	uint8_t GetInstructionType();
public:

	int						ColumnCount;
	CustomItemData			ColumnData[CUSTOM_MAX_COLUMN];

	uint8_t					InstructionType;
	uint64_t				ptrAddress;
	uint64_t				Scaler;//计数
	COLORREF				NoneFocusFontColor;//非聚焦显示颜色

	COLORREF				NorFontColor;//正常显示颜色
	COLORREF				SelFontColor;//正常显示颜色

	HBRUSH					SelBgkBrush;//选择后显示颜色
	HBRUSH					NorBgkBrush;// 背景颜色

	LPTSTR					lpAddress;
	CustomItemHead*			ModifyItemHead;
	CustomItemHead*			tagNext;
	CustomItemHead*			tagPre;
	CustomItemHead*			tagJmpTo;

};

typedef CustomItemHead* PCustomItemHead;

class CSrceenView
{
public:
	CSrceenView();
	~CSrceenView();

public:
	inline PCustomItemHead* GetItemHeader()
	{
		return mItemHead;
	}

	inline PCustomItemHead GetItem( int iElem)
	{
		return mItemHead[iElem];
	}

	inline void SetItem( int iElem,PCustomItemHead pItem )
	{
		mItemHead[iElem] = pItem;
	}

	inline void SetSingleSelItem( int Elem)
	{
		mSelectItemMin = mSelectItemMax = mItemHead[Elem];
	}

	inline void SetMultiSelItem( int Elem)
	{
		mSelectItemMax = mItemHead[Elem];
	}

	inline BOOL IsFirstItem()
	{
		return (mSelectItemMin == m_ptrFirstElem);
	}

	inline PCustomItemHead GetSelItem()
	{
		return mSelectItemMin;
	}

	inline void SelToNextItem()
	{
		if (mSelectItemMin && mSelectItemMin->tagNext)
		{
			mSelectItemMin = mSelectItemMax = mSelectItemMin->tagNext;
		}
	}

	inline void SelToPreItem()
	{
		if (mSelectItemMin && mSelectItemMin->tagPre)
		{
			mSelectItemMin = mSelectItemMax = mSelectItemMin->tagPre;
		}
	}

	inline BOOL GetSelectItem(PCustomItemHead pItem )
	{
		if (pItem)
		{
			if ( pItem == mSelectItemMin || pItem == mSelectItemMax)
			{
				return TRUE;
			}
			else
			{
				if ( pItem )
				{
					if ( mSelectItemMin && mSelectItemMax)
					{
						if (mSelectItemMin->ptrAddress <= mSelectItemMax->ptrAddress)
						{
							if ( pItem->ptrAddress >= mSelectItemMin->ptrAddress &&
								pItem->ptrAddress <= mSelectItemMax->ptrAddress )
							{
								return TRUE;
							}
						}
						else
						{
							if ( pItem->ptrAddress <= mSelectItemMin->ptrAddress &&
								pItem->ptrAddress >= mSelectItemMax->ptrAddress )
							{
								return TRUE;
							}
						}
					}
				}
			}
		}

		return FALSE;
	}

	inline BOOL isSingleSel()
	{
		return m_IsSingleSel;
	}

	inline void SetSingleSel()
	{
		m_IsSingleSel = TRUE;
	}

	inline void SetNultiSel()
	{
		m_IsSingleSel = FALSE;
	}

	PCustomItemHead InsertItem( uint64_t u64ptr,LPTSTR lpText,HBRUSH hBgkBrush,HBRUSH hBgkSelect);
	void InsertAddressColumn(PCustomItemHead pItem,int IconNums,LPTSTR lpText,HBRUSH hBgkBrush,HBRUSH hBgkSelect);
	void SetAddressColumnIcon(PCustomItemHead pItem,int IconNums,int nIconType,int nIconType2 = 0,int nIconType3 = 0);
	void SetColumnItem(PCustomItemHead pItem,int nColumn,int IconNums,tagOption* Option,int	nSize,HBRUSH hBgkBrush,HBRUSH hBgkSelect);
	void SetItemText( PCustomItemHead pItem, int nColumn, LPTSTR szText);
	void SetItemHeight(int nHeight);
	void MoveItem(int MoveNums,int PageSize,BOOL IsUp);
	int	 GetItemHeigth();
	int	 GetItemCount();
private:
	int						m_ViewItemHeight;
	int						mViewItemCapacity;
	int						mViewItemSize;
	PCustomItemHead*		mItemHead;
	
	PCustomItemHead			mSelectItemMin;
	PCustomItemHead			mSelectItemMax;

	PCustomItemHead			m_ptrFirstElem;
	PCustomItemHead			m_ptrLastElem;
	PCustomItemHead			m_ptrJmpElem;
	uint64_t				m_cbElemSize;
	BOOL					m_IsSingleSel;
};

class CCustomItem
{
public:
	CCustomItem();
	~CCustomItem();
	void InsertItem( int nItem , uint64_t u64ptr, LPTSTR lpText,HBRUSH hBgkBrush,HBRUSH hBgkSelect );

	void InsertAddressColumn( 
						int nItem,
						int IconNums,
						LPTSTR lpText,
						HBRUSH hBgkBrush,
						HBRUSH hBgkSelect );

	void    SetAddressColumnIcon(int nItem,int IconNums,int nIconType,int nIconType2 = 0,int nIconType3 = 0 );

	void    SetColumnItem(
					int			nItem,
					int			nColumn,
					int			IconNums,
					tagOption* Option,
					int			nSize,
					HBRUSH hBgkBrush,
					HBRUSH hBgkSelect );

	void SetItemText( int nItem , int nColumn, LPTSTR szText );
	void SetItemHeight(  int nHeight );
	int	 GetItemHeigth();
	void SetMaxItemNums( int nMaxItem );
	int	 GetItemCount();

	LPWSTR GetItemText( int nItem , int nColumn);

	void Destroy();
	void DetroyItemData(CustomItemData* ItemData);
	void DetroyItemHeader(PCustomItemHead ItemHeader);
public:
	int						m_nHeight;
	int						m_nItem;
	int						m_nMaxItem;

	PCustomItemHead*		m_ItemHead;
};

typedef struct tagCustomViewPos
{
	BOOL vInit;
	int vTop;
	int vLeft;
	int vRight;
	int vBottom;

}CustomViewPos;

typedef struct _tagCustomViewPage
{
	int		TotlePagesNums;
	int		PerPagesSize;
	int		HorScrollPos; //水平滚动条
	int		VerScrollPos; //垂直
	RECT	PageRect; //页区域大小

	int     CurShowFisrtItem;//当前显示的第一行Item
	int     CurShowLastItem;//当前显示最后一行Item

	int		SelectMinPageItem;
	int		SelectMaxPageItem;
	int		SelectPreMaxItem;
}CustomViewPage;

class CCustomView
{
public:
	CCustomView(void);
	~CCustomView();

	operator HWND()
	{
		return m_hWnd;
	}

	void operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

public:
	BOOL Register(HINSTANCE hInstance,LPTSTR lpClassName);
	void Unregister(void);

	static 
	LRESULT 
	CALLBACK
	CustomProc(
			HWND hwnd,
			UINT uMsg,
			WPARAM wParam, 
			LPARAM lParam);

protected:
	void CustomSetScrollbarRange(
						int scrollbarId,
						int indexMin,
						int indexMax);

	BOOL CustomSetColRange(
				int ColMin,
				int colMax);

	void
		CustomSize(
		UINT uWidth, 
		UINT uHeight);

	BOOL CustomSetRowRange(
						int rowMin,
						int rowMax);

	void OnCustomPaint( HDC hDC, RECT* prcDirty, BOOL bErase);

	void OnCustomNcClassSize( WPARAM wParam,LPARAM lParam);
	void OnCustomNcPaint(WPARAM wParam,LPARAM lParam);

	void OnCustomPaintHeader(
						HDC PaintDC,
						RECT* prcDirty,
						RECT* rcDirtyBody,						
						int ColMin,
						int colMax,
						int rowMin,
						int rowMax,
						int nPosHorz,
						int nPosVert );

	virtual void OnCustomPaintBody(
		HDC		PaintDC,
		RECT*	prcDirty,
		RECT*	rcDirtyBody,
		int		HeaderHeight,
		int		ItemHeight,
		int		ColMin,
		int		ColMax,
		int		RowMin,
		int		RowMax,
		int		PosHorz,
		int		PosVert );

	virtual HRESULT CustomRButtonDown( WPARAM wp,LPARAM lp );
	virtual HRESULT CustomLDoubleClick( WPARAM wp,LPARAM lp );
	virtual void CustomVkEnter();
	int CustomLogicalPage(int nPage);
	void CustomSetScrollPos(int nPos, BOOL isVertical);
	void CustomScroll(int iWhat, BOOL isVertical);
	void CustomMouseWheel( int iDelta, BOOL isVertical);
	void CustomKeyDown( UINT vkCode);
	void CustomKeyUp(UINT vkCode);
	void OnCustomPaintRowHeader();
	HRESULT CustomLButtonDown( WPARAM wp,LPARAM lp );
	HRESULT CustomNcLMouseDown( WPARAM wp,LPARAM lp );
	HRESULT CustomMouseUp( WPARAM wp,LPARAM lp );
	HRESULT CustomMouseMove( WPARAM wp,LPARAM lp );
	HRESULT CustomMouseHove( WPARAM wp,LPARAM lp );
	HRESULT CustomMouseLeave( WPARAM wp,LPARAM lp );
	HRESULT CustomTimer( WPARAM wp,LPARAM lp );
public:
	BOOL	InsertColumn( int nColumn , TCHAR* szText , int nWideth );
	void	SetColumnWidth( int nColumn , int nWideth );
	void	InsertItem( int nItem , uint64_t u64ptr, LPTSTR lpText );
	PCustomItemHead	InsertItem( uint64_t u64ptr, LPTSTR lpText );
	void    InsertAddressColumn(int nItem,int IconNums,LPTSTR lpText );
	void    InsertAddressColumn(PCustomItemHead pItem,int IconNums,LPTSTR lpText );
	void    SetAddressColumnIcon(int nItem,int IconNums,int nIconType,int nIconType2 = 0,int nIconType3 = 0 );
	void    SetAddressColumnIcon(PCustomItemHead pItem,int IconNums,int nIconType,int nIconType2 = 0,int nIconType3 = 0 );
	void    SetColumnItem(
					int nItem,
					int nColumn,
					int IconNums,
					tagOption* Option,
					int nSize );
	void    SetColumnItem(
					PCustomItemHead pItem,
					int nColumn,
					int IconNums,
					tagOption* Option,
					int nSize );
	void	SetItemText( int nItem , int nColumn, LPWSTR szText );
	void	SetItemText( PCustomItemHead pItem, int nColumn, LPWSTR szText );
	void	SetHeaderHeight( int Height);
	void	SetItemHeight( int nHeight );	
	void	SetMaxItemNums( int nMaxItem);
	ULONG	GetColumnWidth( int nColumn );
	ULONG	GetColumnHeight();
	ULONG	GetItemCount();
	void	BroadCastView( int SelectedView);
	void    SetShowBorder();
	void    SetDisableBorder();
	BOOL	Create(
				HWND hWnd,
				HINSTANCE hInstance,
				int nId,
				LPTSTR lpClassName,
				int ViewFlags,
				BOOL IsShowBorder = FALSE);
	HRESULT	SendMessage(UINT uMsg , WPARAM wParam,LPARAM lParam );
	BOOL	SetWindowPos(
				HWND hWndInsertAfter,
				int X,
				int Y,
				int cx,
				int cy,
				UINT uFlags);

	inline void InvalidateRect()
	{
		::InvalidateRect( m_hWnd,NULL,FALSE);
	}

	inline BOOL IsShowBorder()
	{
		return m_bShowBoder;
	}

protected:
	 HWND			m_hWnd;
	 int			m_ColMin;
	 int			m_ColMax;
	 int			m_rowMin;
	 int			m_RowMax;
	 BOOL			m_bFocus;
	 BOOL			m_bShowBoder;
	 BOOL			m_bLButtonDown;
	 int			m_bTrackLine;
	 POINT			m_ptTrackPos;

	// int			m_SelectItem;
	// int			m_TrackSelectItem;

	 int			m_PerPageScroll;
	 int			m_PreScrollPos;

	 int			m_ViewNameFlags;
	 int			m_ParentSelectView;

	 

	 //画刷画笔
	 HPEN			m_hWhitePen;
	 HPEN			m_hLinePen;
	 HPEN			m_hNewPen;
	 HBRUSH			m_hWhiteHeaderBrush;
	 HBRUSH			m_hHeaderBrush;

	 HBRUSH			m_hNormalBrushA;
	 HBRUSH			m_hNormalBrushB;
	 HBRUSH			m_hLineBrush;
	 HBRUSH			m_hLineFrameBrush;
	 HPEN			m_CustomPen[MAX_CUSTOM_PEN];
	 HFONT			m_hPaintFont;
	 int			m_FontWe;
	 int			m_FontHe;

	 CCustomHeader	m_CustomHeader;

	 CCustomItem	m_CustomItem;

	 CustomViewPos	m_ViewPos;
	 CustomViewPage m_ViewPags;
	 CSrceenView	m_ScreenView;
};
// Register/unregister the window class:



#endif  /* CUSTOM_H */
