#include "StdAfx.h"
#include "dynamicview.h"
#include "global.h"

#define MIN_COLUMN_MOVE_POS_X 3
#define DYNAMIC_DEFAULT_VALUES 0
#define DYNAMIC_DEFAULT_PEN_SIZE 1

CDynamicView::CDynamicView(void)
{
	m_DynamicView.DyvData = NULL;
	m_ShowFlags = 0;
	SetHeaderHeight();

	m_CustomPen[kCustomBlackPen]  = CreatePen(PS_SOLID,1,kCustomBlackColor);
	m_CustomPen[kCustomRedPen]  = CreatePen(PS_SOLID,1,kCustomRedColor);
	m_CustomPen[kCustomGreenPen]  = CreatePen(PS_SOLID,1,kCustomGreenColor);
	m_CustomPen[kCustomBluePen]  = CreatePen(PS_SOLID,1,kCustomBlueColor);
	m_CustomPen[kCustomYellowPen]  = CreatePen(PS_SOLID,1,kCustomYellowColor);
	m_CustomPen[kCustomWhitePen]  = CreatePen(PS_SOLID,1,kCustomWhiteColor);
	m_CustomPen[kCustomGreyPen]  = CreatePen(PS_SOLID,1,kCustomGreyColor);
	m_CustomPen[kCustomFreGreyPen]  = CreatePen(PS_SOLID,1,kCustomFreGreyColor);
	m_CustomPen[kCustomCyanPen]  = CreatePen(PS_SOLID,1,kCustomCyanColor);
	m_CustomPen[kCustomGoldenPen]  = CreatePen(PS_SOLID,1,kCustomGoldenColor);

	m_DynamicView.DyvPaintFont		= CreateFont(
		DYNAMIC_DEFAULT_VALUES - DYNAMIC_ITEM_HEIGHT,
		DYNAMIC_DEFAULT_VALUES - DYNAMIC_ITEM_HEIGHT/2,
		DYNAMIC_DEFAULT_VALUES,
		DYNAMIC_DEFAULT_VALUES,
		400,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET, 
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 
		DEFAULT_QUALITY,
		FF_DONTCARE,
		_T("Terminal 6")
		);
	m_DynamicView.DyvOnFocus = FALSE;
	m_DynamicView.DyvBorderSize = DYNAMIC_DEFAULT_VALUES + 1;

	m_DynamicView.DyvPaintBorderBrush.CreateSolidBrush(VIEW_HEADER_LINE_COLOR);
	m_DynamicView.DyvScrollLines = DYNAMIC_DEFAULT_VALUES;
	m_DynamicView.DyvPerPagesLine = DYNAMIC_DEFAULT_VALUES;
	m_DynamicView.DyvShowScrollBar = DYNAMIC_DEFAULT_VALUES;
	m_DynamicView.DyvScrollPos = DYNAMIC_DEFAULT_VALUES;
	m_DynamicView.DyvTrackerMouseLine = kMouseTraceNone;
	m_DynamicView.DyvPaintSelectBrush.CreateSolidBrush(VIEW_SELECT_BACK_COLOR);
	m_DynamicView.DyvPaintNormalBrush.CreateSolidBrush(VIEW_NORMAL_BACK_COLOR);
	m_DynamicView.DyvPaintHeaderBrush.CreateSolidBrush(VIEW_HEADER_BACK_COLOR);
	m_DynamicView.DyvPaintHeaderLinePen.CreatePen(PS_SOLID,DYNAMIC_DEFAULT_PEN_SIZE,VIEW_HEADER_LINE_COLOR);
	m_DynamicView.DyvPaintViewWhitePen.CreatePen(PS_SOLID,DYNAMIC_DEFAULT_PEN_SIZE,VIEW_NORMAL_WHITE_LINE);
	m_DynamicView.DyvPaintViewBodyLinePen.CreatePen(PS_SOLID,DYNAMIC_DEFAULT_PEN_SIZE,VIEW_BODY_LINE_COLOR);
}

CDynamicView::~CDynamicView(void)
{

}

MAP_MSG_BEGIN(CDynamicView)
	//ON_MESSAGE(WM_NCCREATE,CDynamicView::OnNcCreate)
	ON_MESSAGE(WM_PAINT,CDynamicView::OnPaint)
	ON_MESSAGE(WM_NCPAINT,CDynamicView::OnNcPaint)
	ON_MESSAGE(WM_NCLBUTTONDOWN,CDynamicView::OnNcLButtonDown)
	ON_MESSAGE(WM_NCCALCSIZE,CDynamicView::OnNCClassSize)
	ON_MESSAGE(WM_SIZE,CDynamicView::OnSize)
	ON_MESSAGE(WM_ERASEBKGND,CDynamicView::OnEraseBkgnd)
	ON_MESSAGE(WM_MOUSEMOVE,CDynamicView::OnMouseMove)
	ON_MESSAGE(WM_LBUTTONDOWN,CDynamicView::OnLButtonDown)
	ON_MESSAGE(WM_LBUTTONDBLCLK,CDynamicView::OnLButtonDBClick)
	ON_MESSAGE(WM_LBUTTONUP,CDynamicView::OnLButtonUp)
	ON_MESSAGE(WM_RBUTTONDOWN,CDynamicView::OnRButtonDown)
	//ON_MESSAGE(WM_RBUTTONUP,CDynamicView::OnRButtonUp)
	ON_MESSAGE(WM_KEYDOWN,CDynamicView::OnKeyDown)
	ON_MESSAGE(WM_MOUSELEAVE,CDynamicView::OnMouseLeave)
	ON_MESSAGE(WM_TIMER,CDynamicView::OnTimer)
	ON_MESSAGE(WM_KILLFOCUS,CDynamicView::OnKillFocus)
	ON_MESSAGE(WM_SETFOCUS,CDynamicView::OnSetFocus)
	ON_MESSAGE(WM_HSCROLL,CDynamicView::OnHScroll)
	ON_MESSAGE(WM_VSCROLL,CDynamicView::OnVScroll)
	ON_MESSAGE(WM_MOUSEWHEEL,CDynamicView::OnMouseWheel)
	ON_MESSAGE(WM_MOUSEHWHEEL,CDynamicView::OnMouseHWheel)
MAP_MSG_END()

void CDynamicView::Init(CDynamicData* pData)
{
	m_DynamicView.DyvData = pData;
}

void CDynamicView::InsertHeader( int Column ,int Width ,LPTSTR lpText )
{
	if (Column >= 0 && Column < DYNAMIC_COLUMN_MAX_SIZE && lpText)
	{
		if (!m_DynamicView.DyvHeader.DyhColumn[Column])
		{
			m_DynamicView.DyvHeader.DyhColumn[Column] = new DynamicColumn();
		}

		if(m_DynamicView.DyvHeader.DyhColumn[Column])
		{
			int TextSize = lstrlen(lpText);
			m_DynamicView.DyvHeader.DyhColumn[Column]->DycSize = Width;
			if (m_DynamicView.DyvHeader.DyhColumn[Column]->DycText)
			{
				delete[] m_DynamicView.DyvHeader.DyhColumn[Column]->DycText;
				m_DynamicView.DyvHeader.DyhColumn[Column]->DycText = NULL;
			}

			m_DynamicView.DyvHeader.DyhColumn[Column]->DycText = new TCHAR[TextSize+1];
			if (m_DynamicView.DyvHeader.DyhColumn[Column]->DycText)
			{
				StringCchCopy(m_DynamicView.DyvHeader.DyhColumn[Column]->DycText,
							  TextSize+1,
							  lpText);
			}

			InterlockedExchangeAdd(&m_DynamicView.DyvHeader.DyhColumnNums,1);
		}
	}
}

void CDynamicView::ModifyHeaderWidth( int nColumn,int Width )
{
	if (nColumn >= 0 && nColumn < DYNAMIC_COLUMN_MAX_SIZE )
	{
		if(m_DynamicView.DyvHeader.DyhColumn[nColumn])
		{
			m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycSize = Width;
		}
	}
}

void CDynamicView::ModifyHeaderText( int nColumn,LPTSTR lpText,int Width)
{
	if (nColumn >= 0 && nColumn < DYNAMIC_COLUMN_MAX_SIZE && lpText)
	{
		if (!m_DynamicView.DyvHeader.DyhColumn[nColumn])
		{
			m_DynamicView.DyvHeader.DyhColumn[nColumn] = new DynamicColumn();
		}

		if(m_DynamicView.DyvHeader.DyhColumn[nColumn])
		{
			int TextSize = lstrlen(lpText);
			
			if (m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycText)
			{
				delete[] m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycText;
				m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycText = NULL;
			}

			if (Width != -1)
			{
				m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycSize = Width;
			}

			m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycText = new TCHAR[TextSize+1];
			if (m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycText)
			{
				StringCchCopy(m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycText,
					TextSize+1,
					lpText);
			}
		}
	}
}

void CDynamicView::SetColor(COLORREF Normal,COLORREF Select)
{
	m_DynamicView.DyvNormalColor = Normal;
	m_DynamicView.DyvSelectColor = Select;
}

void CDynamicView::SetHeaderHeight( int Height )
{
	m_DynamicView.DyvHeader.DyhHeight = Height;
}

void CDynamicView::SetHeaderClick(int nColumn,BOOL Clicked)
{
	if (nColumn >=0 && nColumn < m_DynamicView.DyvHeader.DyhColumnNums)
	{		
		m_DynamicView.DyvHeader.DyhColumn[nColumn]->DycFlags |= Clicked;				
	}
}

void CDynamicView::SetHeaderColumnSize(int Column,int DycSize)
{
	if (Column >=0 && Column < m_DynamicView.DyvHeader.DyhColumnNums)
	{
		m_DynamicView.DyvHeader.DyhColumn[Column]->DycSize = DycSize;
	}
}

int CDynamicView::GetHeaderColumnSize(int Column )
{
	if (Column >=0 && Column < m_DynamicView.DyvHeader.DyhColumnNums)
	{
		return m_DynamicView.DyvHeader.DyhColumn[Column]->DycSize;
	}

	return 0;
}

LPTSTR CDynamicView::GetHeaderColumnText(int Column)
{
	if (Column >=0 && Column < m_DynamicView.DyvHeader.DyhColumnNums)
	{
		return m_DynamicView.DyvHeader.DyhColumn[Column]->DycText;
	}

	return _T(" N/A");
}

int CDynamicView::GetHeaderColumnHeight()
{
	return m_DynamicView.DyvHeader.DyhHeight;
}

int CDynamicView::GetBodyItemHeigth()
{
	return m_DynamicView.DyvBody.Height();
}

BOOL CDynamicView::OnNcCreate( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	return TRUE;
}

void CDynamicView::DrawBorder(CMemDC& PaintDC,CRect& ClientRect)
{
	if (!m_DynamicView.DyvBorderSize)
	{
		return;
	}

	CRect BorderRect(	
			ClientRect.left,
			ClientRect.top,
			ClientRect.right,
			ClientRect.bottom);

	PaintDC.FillRect(BorderRect,m_DynamicView.DyvPaintBorderBrush);
}

void CDynamicView::DrawHeader(CMemDC& PaintDC,CRect& ClientRect)
{
	CRect HeaderRect(	
			ClientRect.left,
			ClientRect.top,
			ClientRect.right,
			ClientRect.bottom);

	PaintDC.FillRect(HeaderRect,m_DynamicView.DyvPaintHeaderBrush);
	PaintDC.SelectObject(m_DynamicView.DyvPaintViewWhitePen);
	PaintDC.MoveTo(HeaderRect.left, HeaderRect.bottom);
	PaintDC.LineTo(HeaderRect.left, HeaderRect.top);
	PaintDC.LineTo(HeaderRect.right, HeaderRect.top);

	CRect ColumnRect(
			HeaderRect.left,
			HeaderRect.top,
			HeaderRect.right,
			HeaderRect.bottom);
	int ColumnSize = 0;

	PaintDC.SelectObject(m_DynamicView.DyvPaintHeaderLinePen);
	PaintDC.MoveTo(HeaderRect.left, HeaderRect.bottom - 1);
	PaintDC.LineTo(HeaderRect.right,HeaderRect.bottom - 1);

	for (	int Col = 0;
			Col < m_DynamicView.DyvHeader.DyhColumnNums;
			Col++)
	{

		ColumnRect.left = ColumnSize + ColumnRect.left;
		ColumnSize = GetHeaderColumnSize(Col);
		ColumnRect.right = ColumnRect.left + ColumnSize;

		if( Col )
		{
			PaintDC.SelectObject(m_DynamicView.DyvPaintHeaderLinePen);
			PaintDC.MoveTo(ColumnRect.left, ColumnRect.top);
			PaintDC.LineTo(ColumnRect.left, ColumnRect.bottom - 1);
		}		
				
		PaintDC.SelectObject(m_DynamicView.DyvPaintViewWhitePen);
		PaintDC.MoveTo(ColumnRect.left + 1, ColumnRect.bottom - 1);
		PaintDC.LineTo(ColumnRect.left + 1, ColumnRect.top);

		if ((m_DynamicView.DyvHeader.DyhColumn[Col]->DycFlags & kButtonDown) == kButtonDown)
		{
			PaintDC.SelectObject(m_DynamicView.DyvPaintHeaderLinePen);
			PaintDC.MoveTo(ColumnRect.left + 1, ColumnRect.bottom - 1);
			PaintDC.LineTo(ColumnRect.left + 1, ColumnRect.top);
			PaintDC.LineTo(ColumnRect.right,ColumnRect.top);

			ColumnRect.left += 2;
			ColumnRect.top += 2;
			PaintDC.DrawText(
				GetHeaderColumnText(Col), 
				-1,
				ColumnRect,
				DT_EXPANDTABS | 
				DT_VCENTER |
				DT_SINGLELINE);

			ColumnRect.left -= 2;
			ColumnRect.top -= 2;
		}
		else
		{
			PaintDC.DrawText(
				GetHeaderColumnText(Col), 
				-1,
				ColumnRect,
				DT_EXPANDTABS | 
				DT_VCENTER |
				DT_SINGLELINE);
		}
		
	}

	
}

void CDynamicView::OnPaintColumn(
							CMemDC& PaintDC,
							CRect& ClientRect,
							int Item,
							int Column,
							CDynamicDrawItem* DrawItemData,
							BOOL bSelected)
{
	SetItemState( Item, kDynamicItemShowData);
}

int CDynamicView::OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect)
{
	return 0;
}

void CDynamicView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{

}

void CDynamicView::OnClickHeadDwon(int nColumn)
{
	//SetHeaderClick(nColumn,kButtonDown);
}

void CDynamicView::OnClickItemUp()
{

}

void CDynamicView::OnClickHeadUp(int nColumn)
{

}

void CDynamicView::DrawBody(CMemDC& PaintDC,CRect& ClientRect)
{
	CRect BodyRect(	
		ClientRect.left,
		ClientRect.top,
		ClientRect.right,
		ClientRect.bottom);
	

	PaintDC.FillRect(BodyRect,m_DynamicView.DyvPaintNormalBrush);

	ULONG ColumnHeight = m_DynamicView.DyvBody.Height();
	ULONG ColumnPagsSize = m_DynamicView.DyvBody.Size();

	CDynamicDrawItem* DrawItemData = (CDynamicDrawItem*)OnGetDrawData();

	if (DrawItemData)
	{
		for( int Item = 0 ;
			Item < ColumnPagsSize;
			Item++)
		{
			int ColumnSize = 0;
			BOOL bSelected = FALSE;
			CRect ColumnRect;
			ColumnRect.left = BodyRect.left;
			ColumnRect.top	= ClientRect.top + Item * ColumnHeight;
			ColumnRect.bottom = ColumnRect.top + ColumnHeight;
			ColumnRect.right = 	BodyRect.right;

			OnGetItemData(Item,DrawItemData);

#ifdef _DEBUG
			TCHAR szError[100];
			StringCchPrintf(szError,100,_T("Item:%d---Status:%d"),Item,GetItemState(Item));
			OutputDebugString(szError);
#endif

			if ( (GetItemState(Item) & kDynamicItemSelect) == kDynamicItemSelect )
			{
				bSelected = TRUE;
				PaintDC.FillRect(ColumnRect,m_DynamicView.DyvPaintSelectBrush);

#ifdef _DEBUG
				TCHAR szError[100];
				StringCchPrintf(szError,100,_T("Item:%d---kDynamicItemSelect"),Item);
				OutputDebugString(szError);
#endif
			}			

			for (	int Col = 0;
				Col < m_DynamicView.DyvHeader.DyhColumnNums;
				Col++)
			{
				ColumnRect.top  = ClientRect.top + Item * ColumnHeight;
				ColumnRect.bottom = ColumnRect.top + ColumnHeight;
				ColumnRect.left = ColumnSize + ColumnRect.left;
				ColumnSize = GetHeaderColumnSize(Col);
				ColumnRect.right = ColumnRect.left + ColumnSize;

				//*DrawItemData.Column() = Col;
				OnPaintColumn(
					PaintDC,
					ColumnRect,
					Item,
					Col,
					DrawItemData,
					bSelected
					);

				if (Col)
				{
					PaintDC.SelectObject(m_DynamicView.DyvPaintViewBodyLinePen);
					PaintDC.MoveTo(ColumnRect.left, ColumnRect.top);
					PaintDC.LineTo(ColumnRect.left, ColumnRect.bottom);
				}
				
			}
			
			ReleaseItemData(DrawItemData);
		}

		ReleaseDrawData(DrawItemData);
	}

	
}

BOOL CDynamicView::OnNCClassSize( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	if ( (BOOL) wp && m_DynamicView.DyvBorderSize)
	{
		NCCALCSIZE_PARAMS FAR* lpNcsp = (NCCALCSIZE_PARAMS FAR*)lp;
		if (lpNcsp)
		{
			InflateRect(
				&((NCCALCSIZE_PARAMS* )lp)->rgrc[0],
				-m_DynamicView.DyvBorderSize,
				-m_DynamicView.DyvBorderSize);
		}
	}

	return 0;
}

BOOL CDynamicView::OnPaint( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	CPaintDC PaintDC(m_hWnd);
	CMemDC MemDC(PaintDC);	
	CClientRect ClientRect(m_hWnd);

	CBitmap OldBitmap = MemDC.CreateCompatibleBitmap(PaintDC, ClientRect.Width(),ClientRect.Height());	
	CFont OldFont = MemDC.SelectObject(m_DynamicView.DyvPaintFont);
	MemDC.SetBkMode(TRANSPARENT);
	
// 	if (m_DynamicView.DyvBorderSize)
// 	{
// 		CRect BorderRect(	
// 			ClientRect.Top(),
// 			ClientRect.Left(),
// 			ClientRect.Right(),
// 			ClientRect.Bottom());
// 		DrawBorder(MemDC,BorderRect);
// 	}

	int HeaderHeight = GetHeaderColumnHeight();	

	if (m_DynamicView.DyvHeader.DyhColumnNums && HeaderHeight )
	{
		CRect HeaderRect(	
			0,
			0,
			ClientRect.Right() - 0,
			HeaderHeight + 0);

		DrawHeader(MemDC,HeaderRect);	
	}

	CRect BodyRect(
				0,
				0 + HeaderHeight,
				ClientRect.Right() - 0,
				ClientRect.Bottom() - 0);

	DrawBody(MemDC,BodyRect);
	
	PaintDC.BitBlt(
				0,
				0,
				ClientRect.Width(),
				ClientRect.Height(),
				MemDC, 
				0,
				0,
				SRCCOPY);

	MemDC.SelectObject(OldFont);
	MemDC.SelectObject(OldBitmap);
	MemDC.Destroy();
	bDef = TRUE;	
	return 0;
}

BOOL CDynamicView::OnNcLButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	SetFocus(m_hWnd);
	return 0;
}

BOOL CDynamicView::OnNcPaint( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	if (m_DynamicView.DyvBorderSize)
	{
		CWindowDC PaintDC(m_hWnd);
		CClientRect ClientRect(m_hWnd);
		CRect rcClient = ClientRect;
		CPoint Pt(rcClient.left,rcClient.top);
		ClientToScreen(m_hWnd,Pt);

		int VScrollWidth = 0;
		
		if ((m_DynamicView.DyvShowScrollBar & SB_VERT) == SB_VERT)
		{
			VScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
		}		

		CRect rcWindows;	
		GetWindowRect(m_hWnd,&rcWindows);

		rcClient.OffsetRect(Pt.x - rcWindows.left, Pt.y - rcWindows.top);

		PaintDC.ExcludeClipRect(rcClient.left,rcClient.top,rcClient.right + VScrollWidth,rcClient.bottom);
		//rcWindows.InflateRect(1,1);
		rcWindows.OffsetRect(-rcWindows.left, -rcWindows.top);
		PaintDC.FillRect(rcWindows,m_DynamicView.DyvPaintBorderBrush);

	}
	
	return 0;
}

BOOL CDynamicView::OnSize( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	int ColumnHeight = m_DynamicView.DyvBody.Height();
	int HeaderHeight = m_DynamicView.DyvHeader.DyhHeight;
	CClientRect ClientRect(m_hWnd);

	CRect BodyRect(
		0,
		0 + HeaderHeight,
		ClientRect.Right() - 0,
		ClientRect.Bottom() - 0);

	if (ColumnHeight)
	{
		m_DynamicView.DyvBody.Size() = BodyRect.Height()/ColumnHeight + 1;
	}	

	return TRUE;
}

BOOL CDynamicView::OnEraseBkgnd( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	return 0;
}

BOOL CDynamicView::OnMouseMove( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	BOOL IsRoomed = IsZoomed(m_hWnd);
	BOOL MouseTraceLine = m_DynamicView.DyvTrackerMouseLine;
	CClientRect ClientRect(m_hWnd);
	CPoint MousePt = GetCursorPos();
	ScreenToClient(m_hWnd,MousePt);

	TRACKMOUSEEVENT Tme;     
	Tme.cbSize = sizeof(TRACKMOUSEEVENT); //¼à¿ØÊó±êÀë¿ª     
	Tme.dwFlags = TME_LEAVE/*|TME_HOVER*/;     
	Tme.hwndTrack = m_hWnd;  
	Tme.dwHoverTime = 1;
	TrackMouseEvent(&Tme);
	HCURSOR CurrentCursor = LoadCursor(m_hInst,IDC_ARROW);

	if ( MouseTraceLine > kMouseTraceButtonDown )
	{
		LONG PreMousePosX	= m_DynamicView.DyvMouseTacePos.x;
		LONG MoveToPosX		= MousePt.x;
		CRect FlushRect(
					m_DynamicView.DyvMouseTacePos.x,
					ClientRect.Top(),
					ClientRect.Right(),
					ClientRect.Bottom()
			);
		CurrentCursor = LoadCursor(m_hInst,IDC_SIZEWE);

		ULONG MoveDistanceX = abs(PreMousePosX - MoveToPosX);
		if ( MoveDistanceX > 0 )
		{
			int ColumnIndex = MouseTraceLine - 1;
			int LowPosX = (ClientRect.Left() + MIN_COLUMN_MOVE_POS_X);
			for (int Col = 0;Col < ColumnIndex;Col++)
			{
				LowPosX += GetHeaderColumnSize(Col);
			}

			int ColumnSize = GetHeaderColumnSize(ColumnIndex);

			if(ColumnSize)
			{
				if (PreMousePosX > MoveToPosX)
				{
					if (PreMousePosX < LowPosX )
					{
						goto __finally_;
					}

					if ( MousePt.x >= LowPosX)
					{
						ColumnSize = ColumnSize - MoveDistanceX;
					}
					else
					{
						ColumnSize = MIN_COLUMN_MOVE_POS_X;
					}
				}
				else
				{
					ColumnSize = ColumnSize + MoveDistanceX;
				}

				SetHeaderColumnSize(ColumnIndex,ColumnSize);
				CurrentCursor = LoadCursor(m_hInst,IDC_SIZEWE);
				m_DynamicView.DyvMouseTacePos.x = MousePt.x;
				m_DynamicView.DyvMouseTacePos.y = MousePt.y;
				InvalidateRect();
			}
		}
	}
	else if ( MouseTraceLine == kMouseTraceButtonDown )
	{
		//here is LbuttonDown
		if ( m_DynamicView.DyvBody.m_DybIsMultiSelect)
		{
			int HeaderHeight = GetHeaderColumnHeight();

			if (MousePt.y > HeaderHeight && MousePt.y < ClientRect.Bottom())
			{
				// 			int MoveSize = abs(m_DynamicView.DyvBody.m_DybMouseClickPos.y -
				// 								MousePt.y);
				KillTimer(m_hWnd,WM_TIMEER_MOV_SELECT_DOWN);
				KillTimer(m_hWnd,WM_TIMEER_MOV_SELECT_UP);

				m_DynamicView.DyvBody.m_DybSelectTimer[0] = 0;
				m_DynamicView.DyvBody.m_DybSelectTimer[1] = 0;

				if (GetBodyItemHeigth())
				{
					int nItem = (MousePt.y - HeaderHeight)/GetBodyItemHeigth();
					int nColumnSize = 0;
					for (	int Col = 0;
						Col < m_DynamicView.DyvHeader.DyhColumnNums;
						Col++)
					{
						if ( MousePt.x > nColumnSize && MousePt.x < (nColumnSize + GetHeaderColumnSize(Col)))
						{
							OnMouseMovMulti(nItem,Col,MousePt,m_DynamicView.DyvBody.m_DybMoveDirect);
							InvalidateRect();
							break;
						}

						nColumnSize += GetHeaderColumnSize(Col);

					}

				}
			}
			else if ( MousePt.y <= HeaderHeight )
			{
				if (!m_DynamicView.DyvBody.m_DybSelectTimer[0])
				{
					m_DynamicView.DyvBody.m_DybSelectTimer[0] = 
															SetTimer(
																	m_hWnd,
																	WM_TIMEER_MOV_SELECT_UP,
																	TIMER_SELECT,
																	NULL);
				}
				
			}
			else if ( MousePt.y >= ClientRect.Bottom() )
			{
				if (!m_DynamicView.DyvBody.m_DybSelectTimer[1])
				{
					m_DynamicView.DyvBody.m_DybSelectTimer[1] = 
										SetTimer(
											m_hWnd,
											WM_TIMEER_MOV_SELECT_DOWN,
											TIMER_SELECT,
											NULL);
				}
			}


		}
	}

__finally_:

	if ( m_DynamicView.DyvTrackerMouseLine != kMouseTraceButtonDown )
	{
		if(TRUE)
		{
			CRect ColumnRect(
				ClientRect.Left() + 0,
				ClientRect.Top(),
				ClientRect.Right() - 0,
				ClientRect.Bottom());

			int ColumnSize = 0;
			for (	int Col = 0;
				Col < m_DynamicView.DyvHeader.DyhColumnNums;
				Col++)
			{
				ColumnRect.left = ColumnSize + ColumnRect.left;
				ColumnSize = GetHeaderColumnSize(Col);
				ColumnRect.right = ColumnRect.left + ColumnSize;

				if ( (ColumnRect.left == MousePt.x ||
					(ColumnRect.left + 1) == MousePt.x) && Col)
				{
					CurrentCursor = LoadCursor(m_hInst,IDC_SIZEWE);
					break;
				}

			}
		}
	}
		
	SetCursor(CurrentCursor);

	return 0;
}

BOOL CDynamicView::OnMouseLeave( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	m_DynamicView.DyvTrackerMouseLine = kMouseTraceNone;
	return TRUE;
}

BOOL CDynamicView::OnTimer( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	int nTimerEvent = (int)wp;

	if (nTimerEvent == WM_TIMEER_MOV_SELECT_DOWN)
	{
		OnViewScrollMove(SB_LINEDOWN,2);
		OnTimerSelectDown();
	}
	else if ( nTimerEvent == WM_TIMEER_MOV_SELECT_UP )
	{
		OnViewScrollMove(SB_LINEUP,2);
		OnTimerSelectUp();
	}
	return 0;
}

BOOL CDynamicView::OnKillFocus( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	m_DynamicView.DyvOnFocus = FALSE;
	InvalidateRect();
	return 0;
}

BOOL CDynamicView::OnSetFocus( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	HWND hParent = GetParent(m_hWnd);

	if (hParent)
	{
		::SendMessage(hParent,WM_CPU_VIEW_LBUTTON,0,(LPARAM)m_ShowFlags);
	}

	m_DynamicView.DyvOnFocus = TRUE;
	InvalidateRect();
	return 0;
}

BOOL CDynamicView::OnLButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	CClientRect ClientRect(m_hWnd);
	CPoint MousePt = GetCursorPos();
	ScreenToClient(m_hWnd,MousePt);	
	SetCapture(m_hWnd);
	SetFocus(m_hWnd);
	if ( ClientRect.Top() <		MousePt.y && 
		ClientRect.Bottom() >	MousePt.y &&
		ClientRect.Left() <		MousePt.x && 
		ClientRect.Right() >	MousePt.x )
	{
		CRect ColumnRect(
			ClientRect.Left() + 0,
			ClientRect.Top(),
			ClientRect.Right() - 0,
			ClientRect.Bottom());

		int ColumnSize = 0;
		int Item = kMouseTraceNone;
		ULONG HeaderHeight = m_DynamicView.DyvHeader.Height();
		ULONG ColumnHeight = m_DynamicView.DyvBody.Height();
		ULONG ColumnPagsSize = m_DynamicView.DyvBody.Size();

		m_DynamicView.DyvTrackerMouseLine = kMouseTraceButtonDown;
		CRect HeaderRect = ColumnRect;
		HeaderRect.top = ClientRect.Top();
		HeaderRect.bottom = HeaderRect.top + HeaderHeight;

		for (	int Col = 0;
				Col < m_DynamicView.DyvHeader.DyhColumnNums;
				Col++)
		{
			ColumnRect.left = ColumnSize + ColumnRect.left;
			ColumnSize = GetHeaderColumnSize(Col);
			ColumnRect.right = ColumnRect.left + ColumnSize;

			if ( ColumnRect.left == MousePt.x ||
				 ( ColumnRect.left + 1 ) == MousePt.x)
			{
				SetCursor(LoadCursor(m_hInst,IDC_SIZEWE));
				m_DynamicView.DyvTrackerMouseLine = ( kMouseTraceButtonDown + Col );
				m_DynamicView.DyvMouseTacePos = MousePt;
				
				return TRUE;
			}

			if ( HeaderHeight )
			{
				HeaderRect.left = ColumnRect.left + 2;
				HeaderRect.right = ColumnRect.right - 2;
				if ( HeaderRect.PtInRect(MousePt))
				{
					OnClickHeadDwon(Col);
					InvalidateRect();
					return 0;
				}
			}
		}

		//for ( int PageItem = 0;PageItem < ColumnPagsSize;PageItem++)
		//{
			//SetItemState(PageItem,GetItemState(PageItem) & ~kDynamicItemClicked);
			//SetItemState(PageItem,GetItemState(PageItem) & ~kDynamicItemSelect);
		//}

		if ( ColumnPagsSize && ColumnHeight && MousePt.y > HeaderHeight)
		{
			int MouseTrace = (MousePt.y - HeaderHeight - 0);
			if (MouseTrace > kMouseTraceNone)
			{
				Item = MouseTrace / ColumnHeight;
				if (GetItemState(Item) & kDynamicItemShowData )
				{
					//SetItemState(Item ,GetItemState(Item) | kDynamicItemSelect);
					ColumnRect.top  = ClientRect.Top() +
						0 +
						HeaderHeight +
						Item * ColumnHeight;
					ColumnRect.bottom = ColumnRect.top + ColumnHeight;
					ColumnRect.left = ClientRect.Left() + 0;
					ColumnRect.right = ClientRect.Right() - 0;
					ColumnSize = 0;

					for (	int Col = 0;
						Col < m_DynamicView.DyvHeader.DyhColumnNums;
						Col++)
					{					
						ColumnRect.left = ColumnSize + ColumnRect.left;
						ColumnSize = GetHeaderColumnSize(Col);
						ColumnRect.right = ColumnRect.left + ColumnSize;

						if ( ColumnRect.PtInRect(MousePt))
						{
							CDynamicClickItem Click;
							Click.Item() = Item;
							Click.Column() = Col;
							Click.Type() = kDynamicClickedDown;
							Click.m_ClickPos = MousePt;
							m_DynamicView.DyvBody.m_DybMouseClickPos = MousePt;
							m_DynamicView.DyvBody.m_DybMoveDirect = 0;
							OnClickItem(ColumnRect,Click);
							break;
						}
					}
				}			

				InvalidateRect();
			}
		}
	}

	
	return 0;
}

BOOL CDynamicView::OnLButtonDBClick( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	CClientRect ClientRect(m_hWnd);
	CPoint MousePt = GetCursorPos();
	ScreenToClient(m_hWnd,MousePt);	
	SetFocus(m_hWnd);
	ULONG HeaderHeight = m_DynamicView.DyvHeader.Height();
	ULONG ColumnHeight = m_DynamicView.DyvBody.Height();
	ULONG ColumnPagsSize = m_DynamicView.DyvBody.Size();
	int ColumnSize = 0;
	int Item = 0;

	if ( ClientRect.Top() <		MousePt.y && 
		ClientRect.Bottom() >	MousePt.y &&
		ClientRect.Left() <		MousePt.x && 
		ClientRect.Right() >	MousePt.x )
	{
		CRect ColumnRect(
			ClientRect.Left() + 0,
			ClientRect.Top(),
			ClientRect.Right() - 0,
			ClientRect.Bottom());

		if ( ColumnPagsSize && ColumnHeight)
		{
			int MouseTrace = (MousePt.y - HeaderHeight - 0);
			if (MouseTrace > kMouseTraceNone)
			{
				Item = MouseTrace / ColumnHeight;
				//SetItemState(Item ,GetItemState(Item) | kDynamicItemClicked);

				ColumnRect.top  = ClientRect.Top() +
					0 +
					HeaderHeight +
					Item * ColumnHeight;
				ColumnRect.bottom = ColumnRect.top + ColumnHeight;
				ColumnRect.left = ClientRect.Left() + 0;
				ColumnRect.right = ClientRect.Right() - 0;
				ColumnSize = 0;

				for (	int Col = 0;
					Col < m_DynamicView.DyvHeader.DyhColumnNums;
					Col++)
				{					
					ColumnRect.left = ColumnSize + ColumnRect.left;
					ColumnSize = GetHeaderColumnSize(Col);
					ColumnRect.right = ColumnRect.left + ColumnSize;

					if ( ColumnRect.PtInRect(MousePt))
					{
						CDynamicClickItem Click;
						Click.Item() = Item;
						Click.Column() = Col;
						Click.Type() = kDynamicDBClickedDown;
						Click.m_ClickPos = MousePt;
						OnClickItem(ColumnRect,Click);
						break;
					}

				}
			}
		}
	}

	return 0;
}

BOOL CDynamicView::OnLButtonUp( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;

	KillTimer(m_hWnd,WM_TIMEER_MOV_SELECT_DOWN);
	KillTimer(m_hWnd,WM_TIMEER_MOV_SELECT_UP);

	if ( GetCapture() == m_hWnd)
	{
		ReleaseCapture();

		for(int nI = 0; nI < m_DynamicView.DyvHeader.DyhColumnNums;nI ++)
		{
			if ((m_DynamicView.DyvHeader.DyhColumn[nI]->DycFlags & kButtonDown) == kButtonDown)
			{
				m_DynamicView.DyvHeader.DyhColumn[nI]->DycFlags &= ~kButtonDown;
				OnClickHeadUp(nI);
				InvalidateRect();

				goto _____finally;
			}
		}

		OnClickItemUp();

	}

_____finally:
	m_DynamicView.DyvMouseTacePos.x = 0;
	m_DynamicView.DyvMouseTacePos.y = 0;
	m_DynamicView.DyvTrackerMouseLine = kMouseTraceNone;

	return 0;
}

BOOL CDynamicView::OnRButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	CClientRect ClientRect(m_hWnd);
	CPoint MousePt = GetCursorPos();
	ScreenToClient(m_hWnd,MousePt);	
	SetFocus(m_hWnd);
	ULONG HeaderHeight = m_DynamicView.DyvHeader.Height();
	ULONG ColumnHeight = m_DynamicView.DyvBody.Height();
	ULONG ColumnPagsSize = m_DynamicView.DyvBody.Size();
	int ColumnSize = 0;
	int Item = 0;

	if ( ClientRect.Top() <		MousePt.y && 
		ClientRect.Bottom() >	MousePt.y &&
		ClientRect.Left() <		MousePt.x && 
		ClientRect.Right() >	MousePt.x )
	{
		CRect ColumnRect(
			ClientRect.Left() + 0,
			ClientRect.Top(),
			ClientRect.Right() - 0,
			ClientRect.Bottom());

		if ( ColumnPagsSize && ColumnHeight)
		{
			int MouseTrace = (MousePt.y - HeaderHeight - 0);
			if (MouseTrace > kMouseTraceNone)
			{
				Item = MouseTrace / ColumnHeight;
				//SetItemState(Item ,GetItemState(Item) | kDynamicItemClicked);

				ColumnRect.top  = ClientRect.Top() +
					0 +
					HeaderHeight +
					Item * ColumnHeight;
				ColumnRect.bottom = ColumnRect.top + ColumnHeight;
				ColumnRect.left = ClientRect.Left() + 0;
				ColumnRect.right = ClientRect.Right() - 0;
				ColumnSize = 0;

				for (	int Col = 0;
					Col < m_DynamicView.DyvHeader.DyhColumnNums;
					Col++)
				{		
					ColumnRect.left = ColumnSize + ColumnRect.left;
					ColumnSize = GetHeaderColumnSize(Col);
					ColumnRect.right = ColumnRect.left + ColumnSize;

					if ( ColumnRect.PtInRect(MousePt))
					{
						CDynamicClickItem Click;
						Click.Item() = Item;
						Click.Column() = Col;
						Click.Type() = kDynamicRClickedDown;
						Click.m_ClickPos = MousePt;
						OnClickItem(ColumnRect,Click);
						break;
					}

				}
			}
		}
	}
	return 0;
}

BOOL CDynamicView::OnKeyDown( WPARAM wp,LPARAM lp,BOOL& bDef)
{

	bDef = TRUE;
	return 0;
}

BOOL CDynamicView::OnVScroll( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	int iWhat = LOWORD(wp);
	OnViewScrollMove(iWhat);
	return 0;
}

BOOL CDynamicView::OnHScroll( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	//bDef = TRUE;
	//InvalidateRect();
	return 0;
}

BOOL CDynamicView::OnMouseWheel( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	bDef = TRUE;
	int i120 = -120;
	int iWhat = (int)((short)(wp >> 16));
	if (iWhat > 0 )
	{
		OnViewScrollMove(SB_LINEUP,2);
	}
	else
	{
		OnViewScrollMove(SB_LINEDOWN,2);
	}
	
	return 0;
}

BOOL CDynamicView::OnMouseHWheel( WPARAM wp,LPARAM lp,BOOL& bDef)
{
	//bDef = TRUE;
	//InvalidateRect();
	return 0;
}

void CDynamicView::SetItemState( int Item,int State)
{
	if(Item < DYNAMIC_DEFAULT_ITEM_COUNT)
		m_DynamicView.DyvBody.m_DybItem[Item].m_State = State;
}

int  CDynamicView::GetItemState( int Item)
{
	if(Item < DYNAMIC_DEFAULT_ITEM_COUNT)
		return m_DynamicView.DyvBody.m_DybItem[Item].m_State;

	return kDynamicItemInit;
}

void CDynamicView::ShowHScrollBar(BOOL bShow )
{
	if(ShowScrollBar(SB_HORZ,bShow))
	{
		if (bShow)
		{
			m_DynamicView.DyvShowScrollBar |= SB_HORZ;
		}
		else
		{
			m_DynamicView.DyvShowScrollBar &= ~SB_HORZ;
		}
		
	}
}

void CDynamicView::ShowVScrollBar(BOOL bShow )
{
	if(ShowScrollBar(SB_VERT,bShow))
	{
		if (bShow)
		{
			m_DynamicView.DyvShowScrollBar |= SB_VERT;
			SetScrollRange(SB_VERT,0,0x4000,TRUE);
		}
		else
		{
			m_DynamicView.DyvShowScrollBar &= ~SB_VERT;
		}
	}
}


void CDynamicView::OnViewScrollMove(int iWhat,int MoveStep)
{
	int ScrollStep = 0;
	SCROLLINFO si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(m_hWnd, SB_VERT, &si);
	int nPos = 0;

	switch (iWhat) {
	case SB_TOP:            nPos = si.nMin; break;
	case SB_BOTTOM:         nPos = si.nMax; break;
	case SB_LINEUP:         ScrollStep = -MoveStep; break;
	case SB_LINEDOWN:       ScrollStep = MoveStep; break;
	case SB_PAGEUP:         ScrollStep = -m_DynamicView.DyvBody.Size(); break;
	case SB_PAGEDOWN:       ScrollStep = m_DynamicView.DyvBody.Size(); break;
	case SB_THUMBTRACK:
		{
			//if (si.nTrackPos)
			{
				ScrollStep = ( si.nTrackPos - m_DynamicView.DyvScrollPos);
 			}
// 			else
// 			{
// 				ScrollStep = 0;
// 			}

		}
		break;
	case SB_THUMBPOSITION:  
		{
			if (si.nTrackPos )
			{
				//ScrollStep = (m_DynamicView.DyvScrollPos - si.nTrackPos);
			}
			
		}
		break;
	}

	ScrollStep = OnMoveScroll(ScrollStep);
	nPos = m_DynamicView.DyvScrollPos;//GetScrollPos(m_hWnd, SB_VERT);
	if( ScrollStep != 0)
	{
		nPos += ScrollStep;

	}

	m_DynamicView.DyvScrollPos = nPos;
	SetScrollPos(m_hWnd, SB_VERT, nPos, TRUE);
	InvalidateRect();
}