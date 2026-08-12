#include "stdafx.h"
#include "customclassview.h"
#include "disasmview.h"
#include "common.h"
#include <strsafe.h>
#define VIEW_FRAME_LINEE_WIDTH 2

#ifndef _customclassview_cpp
#define _customclassview_cpp

template <typename T>
CElemView<T>::CElemView()
{

}

template <typename T>
CElemView<T>::~CElemView()
{

}

template <typename T>
BOOL CElemView<T>::Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle)
{
	if(__super::Create(hWnd,hInst,Id,szClass,0))
	{
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)1000);
		SetWindowPos(NULL,2,2,2,2,SWP_SHOWWINDOW);

		InvalidateRect();

		return TRUE;
	}

	return FALSE;
}

template <typename T>
void CElemView<T>::PaintColumn(	HDC hDC,
	PCustomItemHead CustomItem,
	RECT rcColumn,
	int nColumn,
	BOOL bSelect)
{
	if (!CustomItem)
	{
		return;
	}

	TCHAR* lpszText = NULL;

	if (nColumn == 0)
	{
		lpszText = CustomItem->lpAddress;

		SetBkMode(hDC, TRANSPARENT);

		if (bSelect)
		{
			if ( m_ParentSelectView != m_ViewNameFlags )
			{
				SetTextColor(hDC,CustomItem->NoneFocusFontColor);
			}
			else
			{
				SetTextColor(hDC,CustomItem->SelFontColor);
			}

			FillRect(hDC, &rcColumn, (HBRUSH)CustomItem->SelBgkBrush);
		}
		else
		{
			SetTextColor(hDC,CustomItem->NorFontColor);
			FillRect(hDC, &rcColumn, (HBRUSH)CustomItem->NorBgkBrush);
		}


		if (lpszText != NULL)
		{
			DrawText(
				hDC,
				lpszText, 
				-1,
				&rcColumn,
				DT_EXPANDTABS | 
				DT_VCENTER |
				DT_SINGLELINE);
		}
	}
	else if( nColumn < CUSTOM_MAX_COLUMN )
	{
		if (bSelect)
		{
			FillRect(hDC, &rcColumn, (HBRUSH)CustomItem->ColumnData[nColumn - 1].SelBgkBrush);
		}
		else
		{
			FillRect(hDC, &rcColumn, (HBRUSH)CustomItem->ColumnData[nColumn - 1].NorBgkBrush);
		}

		RECT rcIcon1;
		rcIcon1.left = rcColumn.left + 2;
		rcIcon1.top = rcColumn.top;
		rcIcon1.bottom = rcColumn.bottom;
		rcIcon1.right = rcIcon1.left;
		if (CustomItem->ColumnData[nColumn - 1].Icon && CustomItem->ColumnData[nColumn - 1].IconCount )
		{
			for ( int nIndex = 0 ;
				nIndex < CustomItem->ColumnData[nColumn - 1].IconCount ;
				nIndex++)
			{
				rcIcon1.right += m_FontWe;

				if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == FUNC_INER_ICON)
				{
					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Icon[nIndex].colorIcon);
					//SetBkMode(hDC, TRANSPARENT);

					SelectObject(hDC,m_CustomPen[kCustomBlackPen]);
					MoveToEx(hDC, rcIcon1.left + m_FontWe/2, rcIcon1.top + m_FontHe / 2, 0);
					LineTo(hDC, rcIcon1.left + m_FontWe/2, rcIcon1.top + m_FontHe / 2 + 2);
					MoveToEx(hDC, rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2, 0);
					LineTo(hDC, rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2 + 2);
				}
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == JMP_CALL_ICON )
				{
					SelectObject(hDC,m_CustomPen[kCustomBlackPen]);
					MoveToEx(hDC, rcIcon1.left + 2*m_FontWe/5, rcIcon1.top + m_FontHe / 2, 0);
					LineTo(hDC, rcIcon1.left + m_FontWe, rcIcon1.top + m_FontHe / 2);
					//MoveToEx(hDC, rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2, 0);
					//LineTo(hDC, rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2 + 2);
				}
				else if ( CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == FUNCTION_HREADER_FLAGS )
				{
					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Icon[nIndex].colorIcon);
					SetBkMode(hDC, TRANSPARENT);
					DrawText(
						hDC,
						FUCYION_HEADER_CHAR, 
						-1,
						&rcIcon1,
						DT_EXPANDTABS | 
						DT_LEFT |
						DT_SINGLELINE);
				}
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == 88)
				{
					SelectObject(hDC,m_CustomPen[kCustomBlackPen]);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2, 0);
					LineTo(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2 + 1, 0);
					LineTo(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2 + 1);

					MoveToEx(hDC, rcIcon1.left+1, rcIcon1.top + m_FontHe / 2 +1, 0);
					LineTo(hDC, rcIcon1.left+1, rcIcon1.bottom);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2 + 1, 0);
					LineTo(hDC, rcIcon1.left+2, rcIcon1.bottom);
				}
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == 77 )
				{
					SelectObject(hDC,m_CustomPen[kCustomBlackPen]);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2, 0);
					LineTo(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2 + 1, 0);
					LineTo(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2 + 1);

					MoveToEx(hDC, rcIcon1.left+1, rcIcon1.top, 0);
					LineTo(hDC, rcIcon1.left+1, rcIcon1.top + m_FontHe / 2);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top, 0);
					LineTo(hDC, rcIcon1.left+2, rcIcon1.top + m_FontHe / 2);
				}
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == 66)
				{
					SelectObject(hDC,m_CustomPen[kCustomBlackPen]);
					MoveToEx(hDC, rcIcon1.left+1, rcIcon1.top, 0);
					LineTo(hDC, rcIcon1.left+1, rcIcon1.bottom);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top, 0);
					LineTo(hDC, rcIcon1.left+2, rcIcon1.bottom);
				}

				rcIcon1.left = rcIcon1.right;
			}
		}

		rcIcon1.right += m_FontWe/2;
		if (CustomItem->ColumnData[nColumn - 1].Option )
		{
			for( int i = 0; i < CustomItem->ColumnData[nColumn - 1].OptionCount ; i++ )
			{
				lpszText = CustomItem->ColumnData[nColumn - 1].Option[i].lpText;
				int rcWidth = m_FontWe * CustomItem->ColumnData[nColumn - 1].Option[i].TextSize;

				if (CustomItem->ColumnData[nColumn - 1].OptionCount > 1)
				{
					int kkk = 0;
					kkk++;
				}


				if (!bSelect)
				{
					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Option[i].NorFontColor);
					FillRect(hDC, &rcColumn, (HBRUSH)CustomItem->ColumnData[nColumn - 1].NorBgkBrush);
				}
				else
				{
					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Option[i].SelFontColor);
					FillRect(hDC, &rcColumn, (HBRUSH)CustomItem->SelBgkBrush);
				}

				// 				if ( m_ViewNameFlags != m_ParentSelectView)
				// 				{
				// 					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Option[i].HasNonFocus);
				// 				}

				if (CustomItem->ColumnData[nColumn - 1].Option[i].FontBgColor == -1)
				{
					SetBkMode(hDC, TRANSPARENT);
				}
				else
				{
					SetBkMode(hDC,TRANSPARENT);
					SetBkColor(hDC,CustomItem->ColumnData[nColumn - 1].Option[i].FontBgColor);
				}

				if (lpszText != NULL)
				{
					RECT rcText(rcColumn);
					rcText.left = rcIcon1.right;
					rcText.right = rcText.left + rcWidth;
					rcIcon1.right = rcText.right;

					if ( i == 0)
					{
						rcIcon1.right += m_FontWe;
					}

					DrawText(
						hDC,
						lpszText, 
						-1,
						&rcText,
						DT_EXPANDTABS | 
						DT_LEFT |
						DT_SINGLELINE);
				}
			}			
		}
		else
		{
			if (bSelect)
			{
				FillRect(hDC, &rcColumn, (HBRUSH)m_hNormalBrushB);
			}
			else
			{
				FillRect(hDC, &rcColumn, (HBRUSH)m_hNormalBrushA);
			}
		}		
	}	
}

template <typename T>
void CElemView<T>::OnCustomPaintBody(
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
	int		PosVert )
{
	for (int Row = RowMin; Row <= RowMax; Row++) 
	{
#ifdef OLD_ITEM_BODY
		if (m_CustomItem.m_ItemHead )
#else
		if (m_ScreenView.GetItemHeader())
#endif
		{			
			RECT rcBody;
#ifdef OLD_ITEM_BODY
			rcBody.top = HeaderHeight + Row * ItemHeight - PosVert;
#else
			rcBody.top = HeaderHeight + Row * ItemHeight;
#endif
			rcBody.bottom = rcBody.top + ItemHeight;

			int PrePosX = 0;

			for (int Col = ColMin; Col < ColMax; Col++) {
				TCHAR pszBuffer[32] = {0};

				rcBody.left = PrePosX - PosHorz;

				if (rcBody.left > prcDirty->right)
				{
					break;
				}

				rcBody.right = rcBody.left + m_CustomHeader.GetColumnWidth(Col); 

				if (rcBody.right > prcDirty->right || Col == ColMax - 1)
				{
					rcBody.right = prcDirty->right;
				}
#ifdef OLD_ITEM_BODY
				if ( Row < m_RowMax && m_CustomItem.m_ItemHead[Row] )
#else
				if ( m_ScreenView.GetItem(Row) )
#endif
				{

#ifdef OLD_ITEM_BODY
					if (m_ViewPags.SelectMinPageItem == Row )
#else
					if (m_ScreenView.GetSelectItem( m_ScreenView.GetItem(Row)) )
#endif					
					{

#ifdef OLD_ITEM_BODY
						PaintColumn(PaintDC,m_CustomItem.m_ItemHead[Row],rcBody,Col,TRUE );
#else
						PaintColumn(PaintDC,m_ScreenView.GetItem(Row),rcBody,Col,TRUE );
#endif

					}
					else
					{
#ifdef OLD_ITEM_BODY
						if ( m_ViewPags.SelectMaxPageItem >= 0 &&
							((m_ViewPags.SelectMaxPageItem > m_ViewPags.SelectMinPageItem && 
							Row > m_ViewPags.SelectMinPageItem && Row <= m_ViewPags.SelectMaxPageItem) ||
							( m_ViewPags.SelectMaxPageItem < m_ViewPags.SelectMinPageItem &&
							Row >= m_ViewPags.SelectMaxPageItem && Row < m_ViewPags.SelectMinPageItem)))
						{

							PaintColumn(PaintDC,m_CustomItem.m_ItemHead[Row],rcBody,Col,TRUE );


						}
						else
#endif
						{
#ifdef OLD_ITEM_BODY
							PaintColumn(PaintDC,m_CustomItem.m_ItemHead[Row],rcBody,Col,FALSE );
#else
							PaintColumn(PaintDC,m_ScreenView.GetItem(Row),rcBody,Col,FALSE );
#endif

						}

					}
				}
				else
				{
					FillRect(
						PaintDC,
						&rcBody, 
						m_hNormalBrushA);
				}			

				if (Col)
				{
					SelectObject(PaintDC, m_hNewPen);
					::MoveToEx(	PaintDC,rcBody.left, rcBody.top, 0);
					::LineTo(	PaintDC, rcBody.left,  rcBody.bottom);
				}			

				PrePosX += (rcBody.right - rcBody.left);            
			}
		}
	}
}


template <typename T,typename T2>
CustomClassView<T,T2>::CustomClassView()
	:mhWnd(NULL)
	,mhInst(NULL)
	,mSubPos(0)
{
	ZeroMemory( &mTView,sizeof(T*)*MAX_VIEW_ELEM);
	m_BackLineBrush		= (HBRUSH)GetStockObject(BLACK_BRUSH);
}

template <typename T,typename T2>
CustomClassView<T,T2>::~CustomClassView()
{

}

template <typename T,typename T2>
CustomClassView<T,T2>* CustomClassView<T,T2>::Init( HINSTANCE hInst,HWND hWndParent,int uID,LPTSTR szClass,LPTSTR szTitle)
{
	HWND MdiChildHandle = NULL;
	CustomClassView<T,T2>* pCustomView = NULL;
	
	WNDCLASSEX wndclassMainFrame;
	wndclassMainFrame.cbSize		= sizeof(WNDCLASSEX);
	wndclassMainFrame.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassMainFrame.lpfnWndProc	= CustomClassView<T,T2>::DefMdiWinProc;
	wndclassMainFrame.cbClsExtra	= 0;
	wndclassMainFrame.cbWndExtra	= sizeof(CustomClassView<T,T2>*);
	wndclassMainFrame.hInstance		= hInst;
	wndclassMainFrame.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(uID));
	wndclassMainFrame.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclassMainFrame.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclassMainFrame.lpszMenuName	= NULL;
	wndclassMainFrame.lpszClassName	= szClass;
	wndclassMainFrame.hIconSm		= NULL;

	if(!RegisterClassEx(&wndclassMainFrame))
	{
		// return if RegisterClassEx fails
		DWORD dw_LastError = GetLastError();
		if(ERROR_CLASS_ALREADY_EXISTS != dw_LastError)
		{
			// return if the error is other than "error class already exists" 
			return 0;
		}
	}

	MDICREATESTRUCT    ChildStruct;
	ChildStruct.szClass = szClass;
	ChildStruct.szTitle = szTitle;
	ChildStruct.hOwner	 = hInst;
	ChildStruct.x		 = CW_USEDEFAULT;
	ChildStruct.y		 = CW_USEDEFAULT;
	ChildStruct.cx		 = CW_USEDEFAULT;
	ChildStruct.cy		 = CW_USEDEFAULT;
	ChildStruct.style	 = 0;
	ChildStruct.lParam	 = 0;

	MdiChildHandle = (HWND)SendMessage(
		hWndParent,
		WM_MDICREATE,
		(WPARAM)0,
		(LPARAM)&ChildStruct);

	pCustomView = (CustomClassView<T,T2>*)GetWindowLongPtr( MdiChildHandle,0);

	if (pCustomView)
	{
		*pCustomView = hInst;
		ShowWindow(*pCustomView,SW_SHOWMAXIMIZED);
	}
	else
	{
		return pCustomView;
	}

	return pCustomView;
}

template <typename T,typename T2>
LRESULT CustomClassView<T,T2>::DefMdiWinProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	CustomClassView* pData = (CustomClassView*) GetWindowLongPtr(hWnd, 0);

	switch(uMsg)
	{
	case WM_CREATE:
		{
			if(pData)
			{
			   pData->OnCreate();
			}
		}
		break;
	case WM_NCCREATE:
		{
			if (!pData)
			{
				pData = new T();
				*pData = hWnd;
				SetWindowLongPtr(hWnd,0,(LONG_PTR)pData);
			}
		}
		break;
	case WM_SIZE:
		{
			if (pData)
			{
				pData->OnSize();
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (pData)
			{
				if (pData->OnMouseMove(lParam,wParam))
				{
					return 0;
				}
			}
		}
		break;
	case WM_ERASEBKGND:
		{
			return 0;
		}
		break;
	case WM_PAINT:
		{
			if (pData)
			{
				PAINTSTRUCT ps;
				BeginPaint(hWnd, &ps);			
				pData->OnPaint(ps.hdc, &ps.rcPaint, ps.fErase);
				EndPaint(hWnd, &ps);
			}
		}
		break;
	case WM_DESTROY:
		{

			if (pData->OnClose())
			{
				SetWindowLongPtr( hWnd,0,0);

				if (pData)
				{
					delete pData;
					pData = NULL;
				}
			}
			else
			{
				return 0;
			}

		}
		break;
	default:
		break;
	}

	return DefMDIChildProc(hWnd, uMsg, wParam, lParam); 
}

template <typename T,typename T2>
BOOL CustomClassView<T,T2>::OnCreate()
{
	return TRUE;
}

template <typename T,typename T2>
BOOL CustomClassView<T,T2>::OnClose()
{
	return TRUE;
}

template <typename T,typename T2>
BOOL CustomClassView<T,T2>::OnSize()
{
	RECT rcClient;
	GetClientRect(mhWnd,&rcClient);
	InflateRect(&rcClient,0 - VIEW_FRAME_LINEE_WIDTH,0 - VIEW_FRAME_LINEE_WIDTH);
	if ( mSubPos == 1)
	{
		if (mTView[0])
		{
			MoveWindow(
					*mTView[0],
					rcClient.left,
					rcClient.top,
					rcClient.right - rcClient.left,
					rcClient.bottom - rcClient.top,			
					FALSE
					);

			RedrawWindow(mhWnd,0,0,RDW_ERASE | RDW_ALLCHILDREN | RDW_INVALIDATE);
		}
	}
	
	return TRUE;
}

template <typename T,typename T2>
BOOL CustomClassView<T,T2>::OnMouseMove(LPARAM lParam,WPARAM wParam)
{
	return FALSE;
}

template <typename T,typename T2>
BOOL CustomClassView<T,T2>::OnPaint( HDC hDC, RECT* prcDirty, BOOL bErase)
{
	RECT rcClient;
	GetClientRect(mhWnd,&rcClient);
	RECT rcLeft(rcClient);
	RECT rcTop(rcClient);
	RECT rcBottom(rcClient);
	RECT rcRight(rcClient);

	rcLeft.right = rcLeft.left + VIEW_FRAME_LINEE_WIDTH;
	rcTop.bottom = rcTop.top + VIEW_FRAME_LINEE_WIDTH;
	rcRight.left = rcRight.right - VIEW_FRAME_LINEE_WIDTH;
	rcBottom.top = rcBottom.bottom - VIEW_FRAME_LINEE_WIDTH;

	FillRect(hDC,&rcLeft,m_BackLineBrush);
	FillRect(hDC,&rcTop,m_BackLineBrush);
	FillRect(hDC,&rcRight,m_BackLineBrush);
	FillRect(hDC,&rcBottom,m_BackLineBrush);

	return TRUE;
}

template <typename T,typename T2>
CElemView<T2>* CustomClassView<T,T2>::AddView(CElemView<T2>* pBase,tagViewDirect ViewDirect)
{
	if (pBase)
	{

	}
	else
	{
		if (ViewDirect == kViewToFullSrceen)
		{
			if (  0 <=  mSubPos && mSubPos < MAX_VIEW_ELEM )
			{
				T2* pT2 = new T2();
				if (pT2)
				{
					if (pT2->Init(mhInst,mhWnd,mSubPos,_T("CElemView<T2>")))
					{
						mTView[mSubPos++] = pT2;
						
						return pT2;
					}
					else
					{
						delete pT2;
						pT2 = NULL;
					}
				}
			}
			
		}
	}

	SendMessage( mhWnd,WM_SIZE,0,0);

	return NULL;
}

#endif