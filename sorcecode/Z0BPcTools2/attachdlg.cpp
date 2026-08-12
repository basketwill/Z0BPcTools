#include "stdafx.h"
#include "attachdlg.h"
#include "resource.h"
#include "global.h"
#include "CNtdll.h"
#include <strsafe.h>

#define SHOW_VIEW_TOP 5
#define SHOW_VIEW_BOTTOM 50

CProcessView::CProcessView()
{

}

CProcessView::~CProcessView()
{

}

BOOL CProcessView::Init(HINSTANCE hInst,HWND hWnd)
{
	if(Create(hWnd,hInst,IDC_ATTACH_PROCESS_VIEW,_T("CAttachView"),kAttachViewSel,TRUE))
	{
		m_ScreenView.SetSingleSel();
		InsertColumn( 0 ,L"  进程" , 70 );
		InsertColumn( 1 ,L"  名称" , 120 );
		InsertColumn( 2 ,L"  窗口" , 150 );
		InsertColumn( 3 ,L"  路径" , 250 );
		//SetMaxItemNums(10050);
		SetShowBorder();
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)30);
		
		CList<PROCESS_ATTACH_INFO> ProcessInfo;
		if(CNtDll::QuerySystemProces(ProcessInfo) && !ProcessInfo.IsEmpty())
		{
			PROCESS_ATTACH_INFO* AttachItem = NULL;
			AttachItem = ProcessInfo.Head();
			int nI = 0;
			do
			{
				if (AttachItem)
				{
					TCHAR szBuffer[64] = {0};
					StringCchPrintf(szBuffer,64,_T("  %08X"),AttachItem->ProcessId);
					PCustomItemHead pItem = InsertItem(AttachItem->ProcessId,szBuffer);

					if (pItem)
					{
						//pItem->SetInstructionType(kInstructionJmp);
						StringCchPrintf(szBuffer,64,_T("%s"),AttachItem->ProcessName);
						pItem->InsertAddressColumn(0,szBuffer,0,0);
					}					
				}

				nI++;
				AttachItem = ProcessInfo.Next(AttachItem);
			}while(AttachItem);

			InvalidateRect();
		}
		
		return TRUE;
	}

	return FALSE;
}

ULONG_PTR CProcessView::GetSelProcessId()
{
	PCustomItemHead SelItem = m_ScreenView.GetSelItem();

	if (SelItem)
	{
		return SelItem->GetPtrValue();
	}

	return 0;
}

void CProcessView::PaintColumn(	HDC hDC,
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

void CProcessView::OnCustomPaintBody(
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


CAttchDialog::CAttchDialog(HINSTANCE hInst)
{
	mhInst = hInst;
}

CAttchDialog::~CAttchDialog()
{

}

INT_PTR CAttchDialog::AttachDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			if ( wParam == IDOK )
			{
				CAttchDialog* pAttach = (CAttchDialog*) GetWindowLongPtr(hwnd,GWLP_USERDATA);
				if (pAttach)
				{
					pAttach->OnAttach();
				}

				EndDialog(hwnd,0);
			}
			else if( wParam == IDCANCEL)
			{
				EndDialog(hwnd,-1);
			}
		}
		break;
	case WM_INITDIALOG:
		{
			CAttchDialog* pAttach = (CAttchDialog*)lParam;
			if (pAttach)
			{
				*pAttach = hwnd;
				SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)pAttach);
				pAttach->OnInit();
			}
			return 0;
		}
		break;
	case WM_SIZE:
		{
			CAttchDialog* pAttach = (CAttchDialog*) GetWindowLongPtr(hwnd,GWLP_USERDATA);
			if (pAttach)
			{
				pAttach->OnSize();
			}
		}
		break;
	case WM_SYSCOMMAND:
		{
			if ( (wParam & 0xFFF0)  == SC_CLOSE)
			{
				EndDialog(hwnd,-1);
			}
		}
		break;
	}
	return 0;
}

INT_PTR CAttchDialog::DoBox( HWND hWndParent, UINT nID)
{
	return DialogBoxParam(mhInst, MAKEINTRESOURCE(nID), hWndParent, AttachDlgProc, (LPARAM)this);
}

BOOL CAttchDialog::OnInit()
{
	SetWindowText(mhWnd,_T("选择要附加的进程"));

	if(mProcessView.Init(mhInst,mhWnd))
	{
		mProcessView.SetWindowPos(
								NULL,
								SHOW_VIEW_TOP,
								SHOW_VIEW_TOP,
								SHOW_VIEW_TOP,
								SHOW_VIEW_TOP,
								SWP_SHOWWINDOW);
		//RedrawWindow(mhWnd,0,0,RDW_INVALIDATE);
	}

	OnSize();

	return TRUE;
}

BOOL CAttchDialog::OnSize()
{
	RECT rcClient;
	RECT rcOkClient,rcCancelClient;
	GetClientRect(mhWnd,&rcClient);

	GetClientRect(GetDlgItem(mhWnd,IDOK),&rcOkClient);
	GetClientRect(GetDlgItem(mhWnd,IDCANCEL),&rcCancelClient);

	MoveWindow(
		GetDlgItem(mhWnd,IDOK),
		rcClient.right - 2*SHOW_VIEW_TOP - 2*(rcOkClient.right - rcOkClient.left) ,
		rcClient.bottom - (rcOkClient.bottom - rcOkClient.top) - SHOW_VIEW_TOP,
		rcOkClient.right - rcOkClient.left,
		rcOkClient.bottom - rcOkClient.top,
		FALSE);

	MoveWindow(
		GetDlgItem(mhWnd,IDCANCEL),
		rcClient.right - SHOW_VIEW_TOP - (rcCancelClient.right - rcCancelClient.left) ,
		rcClient.bottom - (rcCancelClient.bottom - rcCancelClient.top) - SHOW_VIEW_TOP,
		rcCancelClient.right - rcCancelClient.left,
		rcCancelClient.bottom - rcCancelClient.top,
		FALSE);

	MoveWindow(
		mProcessView,
		rcClient.left + SHOW_VIEW_TOP,
		rcClient.top + SHOW_VIEW_TOP,
		(rcClient.right-rcClient.left) - 2*SHOW_VIEW_TOP,
		rcClient.bottom- rcClient.top - 3*SHOW_VIEW_TOP - (rcOkClient.bottom - rcOkClient.top),
		FALSE);

	RedrawWindow(mhWnd,0,0,RDW_ERASE | RDW_ALLCHILDREN | RDW_INVALIDATE);
	
	return TRUE;
}


BOOL CAttchDialog::OnAttach()
{

	ULONG_PTR AttachProcessId = mProcessView.GetSelProcessId();

	if (AttachProcessId)
	{
		if(g_Debugger.AttachProcessId(AttachProcessId))
		{

		}
	}

	return TRUE;
}