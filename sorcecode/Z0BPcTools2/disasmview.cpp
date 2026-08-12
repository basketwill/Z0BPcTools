#include "stdafx.h"
#include "disasmview.h"
#include "disassem.h"
#include "resource.h"
#include "global.h"

BOOL CDisasmView::Init(HINSTANCE hInst,HWND hWnd)
{
	if(Create(hWnd,hInst,IDC_CPU_ASM_VIEW,_T("CDisasmView"),kCpuAsmView))
	{
		InsertColumn( 0 ,L"  地址" , 70 );
		InsertColumn( 1 ,L"  Hex 数据" , 110 );
		InsertColumn( 2 ,L"  反汇编" , 350 );
		InsertColumn( 3 ,L"  注释" , 150 );
		
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)MAX_DISASM_LINES+50);


		RECT rc = { 0, 0, 0, 0 };
		GetClientRect(hWnd,&rc);

		SetWindowPos(
			NULL, 
			0,
			0,
			1,
			1,
			SWP_NOZORDER|SWP_SHOWWINDOW);

		return TRUE;
	}
	return FALSE;
}

void CDisasmView::SetScollLines( int nMaxLine )
{
	SendMessage(
		XXM_SETROWRANGE,
		(WPARAM)0,
		(LPARAM)nMaxLine);
}

void CDisasmView::PaintAsmColumn(	HDC hDC,
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
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == FUCTION_BODY_HEADER_FLAGS)
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
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == FUCTION_RET_FLAGS )
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
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == FUNCTION_BODY_FLAGS)
				{
					SelectObject(hDC,m_CustomPen[kCustomBlackPen]);
					MoveToEx(hDC, rcIcon1.left+1, rcIcon1.top, 0);
					LineTo(hDC, rcIcon1.left+1, rcIcon1.bottom);
					MoveToEx(hDC, rcIcon1.left + 2, rcIcon1.top, 0);
					LineTo(hDC, rcIcon1.left+2, rcIcon1.bottom);
				}
				else if (CustomItem->ColumnData[nColumn - 1].Icon[nIndex].nIcon == JMP_OTHER_BODY_FLAGS)
				{
					if (m_ScreenView.GetSelItem() == CustomItem && CustomItem->tagJmpTo )
					{
						if (CustomItem->GetInstructionType() == kInstructionJmp)
						{
							SelectObject(hDC,m_CustomPen[kCustomRedPen]);
							MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2, 0);
							LineTo(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2);

							if (CustomItem->tagJmpTo->GetPtrValue() < CustomItem->GetPtrValue())
							{
								MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2, 0);
								LineTo(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top - 1);
							}
							else
							{
								MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2, 0);
								LineTo(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
							}
						}
						
					}
					else
					{
						PCustomItemHead pSelItem = m_ScreenView.GetSelItem();
						if (pSelItem && 
							pSelItem->tagJmpTo &&
							pSelItem->tagJmpTo->GetPtrValue() == CustomItem->GetPtrValue())
						{
							if (pSelItem->GetInstructionType() == kInstructionJmp)
							{
								SelectObject(hDC,m_CustomPen[kCustomRedPen]);
								MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2, 0);
								LineTo(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2);
								LineTo(hDC, rcIcon1.right - 4, rcIcon1.top + m_FontHe / 2 - 4);
								MoveToEx(hDC, rcIcon1.right, rcIcon1.top + m_FontHe / 2,0);
								LineTo(hDC, rcIcon1.right - 4, rcIcon1.top + m_FontHe / 2 + 4);

								if (pSelItem->tagJmpTo->GetPtrValue() < pSelItem->GetPtrValue())
								{
									MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2, 0);
									LineTo(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
								}
								else
								{
									MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2, 0);
									LineTo(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top - 1);
								}
							}							
						}
						else if (pSelItem && pSelItem->tagJmpTo )
						{
							if (pSelItem->GetInstructionType() == kInstructionJmp)
							{
							if (pSelItem->tagJmpTo->GetPtrValue() > pSelItem->GetPtrValue())
							{
								if ( CustomItem->GetPtrValue() < pSelItem->tagJmpTo->GetPtrValue() &&
									CustomItem->GetPtrValue() > pSelItem->GetPtrValue())
								{
									SelectObject(hDC,m_CustomPen[kCustomRedPen]);
									MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top, 0);
									LineTo(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
								}
								
							}
							else if (pSelItem->tagJmpTo->GetPtrValue() < pSelItem->GetPtrValue())
							{
								if ( CustomItem->GetPtrValue() > pSelItem->tagJmpTo->GetPtrValue() &&
									CustomItem->GetPtrValue() < pSelItem->GetPtrValue())
								{
									SelectObject(hDC,m_CustomPen[kCustomRedPen]);
									MoveToEx(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top, 0);
									LineTo(hDC, rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
								}
							}
							}
						}
					}
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
				}
				else
				{
					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Option[i].SelFontColor);
				}

				if ( m_ViewNameFlags != m_ParentSelectView)
				{
					SetTextColor(hDC,CustomItem->ColumnData[nColumn - 1].Option[i].HasNonFocus);
				}

				if (CustomItem->ColumnData[nColumn - 1].Option[i].FontBgColor == -1)
				{
					SetBkMode(hDC, TRANSPARENT);
				}
				else
				{
					SetBkMode(hDC,OPAQUE);
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

HRESULT CDisasmView::CustomRButtonDown( WPARAM wp,LPARAM lp )
{
	return 0;
}

void CDisasmView::CustomVkEnter()
{
	if ( m_ScreenView.GetSelItem() && m_ScreenView.GetSelItem()->tagJmpTo )
	{
		BOOL bHasUpdate = FALSE;
		int SelItemIndex = 0;
		for ( int i = 0; i < m_ViewPags.PerPagesSize;i++)
		{
			if (m_ScreenView.GetSelItem()->tagJmpTo == m_ScreenView.GetItem(i))
			{
				m_ScreenView.SetSingleSelItem(i);
				InvalidateRect();
				bHasUpdate = TRUE;
				break;
			}
			else if ( m_ScreenView.GetItem(i) == m_ScreenView.GetSelItem())
			{
				SelItemIndex = i;
			}
		}

		if (bHasUpdate == FALSE)
		{
			uint64_t ScalerPos = -1;
			PCustomItemHead pNextItem = m_ScreenView.GetSelItem()->tagJmpTo;
		
			for ( int i = 0 ; i < SelItemIndex ; i++)
			{
				if (pNextItem)
				{
					pNextItem = pNextItem->tagPre;
				}				
			}

			for ( int i = 0; i <= m_ViewPags.PerPagesSize;i++)
			{
				if (pNextItem)
				{
					m_ScreenView.SetItem(i,pNextItem);
					pNextItem = pNextItem->tagNext;

					if ( m_ScreenView.GetItem(i) == m_ScreenView.GetSelItem()->tagJmpTo )
					{
						m_ScreenView.SetSingleSelItem(i);
						ScalerPos = m_ScreenView.GetItem(i)->GetScaler();
					}
				}
				else
				{
					m_ScreenView.SetItem(i,0);
				}
						
			}

			if (ScalerPos!= -1)
			{
				SetScrollPos(m_hWnd,SB_VERT,ScalerPos*m_ScreenView.GetItemHeigth(),TRUE);
			}
			InvalidateRect();
		}
	}
}

HRESULT CDisasmView::CustomLDoubleClick( WPARAM wp,LPARAM lp )
{
	return 0;
}


void CDisasmView::OnCustomPaintBody(
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
						PaintAsmColumn(PaintDC,m_CustomItem.m_ItemHead[Row],rcBody,Col,TRUE );
#else
						PaintAsmColumn(PaintDC,m_ScreenView.GetItem(Row),rcBody,Col,TRUE );
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

							PaintAsmColumn(PaintDC,m_CustomItem.m_ItemHead[Row],rcBody,Col,TRUE );


						}
						else
#endif
						{
#ifdef OLD_ITEM_BODY
							PaintAsmColumn(PaintDC,m_CustomItem.m_ItemHead[Row],rcBody,Col,FALSE );
#else
							PaintAsmColumn(PaintDC,m_ScreenView.GetItem(Row),rcBody,Col,FALSE );
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


CDisasmView::CDisasmView()
{

}

CDisasmView::~CDisasmView()
{

}