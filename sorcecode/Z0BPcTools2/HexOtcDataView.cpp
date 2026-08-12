#include "StdAfx.h"
#include "HexOtcDataView.h"
#include "resource.h"

CHexOtcDataView::CHexOtcDataView(void)
	:m_HexOctData(NULL)
	,m_bShowX64(FALSE)
	,m_Section(NULL)
	,m_SectionCount(0)
	,m_bShowAnsi(TRUE)
	,m_HexDataLen(kHexOct16)
{
	m_HexLinePen.CreatePen(PS_SOLID,1,HEX_VIEW_LINE_COLOR);
}


CHexOtcDataView::~CHexOtcDataView(void)
{
	if (m_Section)
	{
		delete[] m_Section;
		m_Section = NULL;
	}

	if (m_HexOctData)
	{
		delete m_HexOctData;
		m_HexOctData = NULL;
	}
}

BOOL CHexOtcDataView::Init( HWND hWnd , HINSTANCE hInst)
{
	if(Create(hWnd,hInst,IDC_CPU_DATA_VIEW,_T("CHexOtcDataView")))
	{
		//CHexOctData* pData = new CHexOctData();
		__super::Init(reinterpret_cast<CDynamicData*>(NULL));

		int nTextWidth = m_DynamicView.DyvBody.Height()/2;

		InsertHeader(0,nTextWidth*12,_T(" 地址"));
		InsertHeader(1,nTextWidth*(3*m_HexDataLen + 2),_T(" &HEX数据"));
		InsertHeader(2,nTextWidth*(m_HexDataLen +4),_T(" &ASCII"));
		InsertHeader(3,200,_T("  "));
		SetBodyCxSize(0);
		ShowVScrollBar(TRUE);
		m_Menu.CreateMenu();
		m_Menu.CreatePopupMenu();
		m_BackMenu.CreateMenu();
		 
		m_BackMenu.CreatePopupMenu();
		m_BackMenu.AppendMenu(MF_STRING,IDM_HEX_CREATE_BACKUP,_T("创建备份"));
		m_BackMenu.AppendMenu(MF_STRING,IDM_HEX_LOAD_BACKUP_FROM_FILE,_T("从文件载入备份"));
		m_BackMenu.AppendMenu(MF_STRING,IDM_HEX_SAVE_BACKUP_TO_FILE,_T("保存数据到文件"));
				
		m_Menu.AppendMenu(MF_STRING|MF_POPUP,(ULONG_PTR)((HMENU)m_BackMenu)/*IDM_HEX_BACKUP*/,_T("备份")); 
		m_Menu.AppendMenu(MF_STRING,IDM_HEX_COPY_DATA,_T("复制")); 
		m_Menu.AppendMenu(MF_STRING,IDM_HEX_SHOW_ADDRESS,_T("地址"));
		m_Menu.AppendMenu(MF_STRING,IDM_HEX_BINARY,_T("二进制"));
		m_Menu.AppendMenu(MF_STRING,IDM_HEX_SIGN_FLAGS,_T("标签                  :"));
		
		SetMenu(hWnd,m_Menu);

		return TRUE;
	}

	return FALSE;
}

void CHexOtcDataView::OnPaintColumn(
								CMemDC& PaintDC,
								CRect& ClientRect,
								int Item,
								int Column,
								CDynamicDrawItem* DrawItemData,
								BOOL bSelected)
{
	CHexOctData* pHexData = (CHexOctData*)DrawItemData;

	if (!pHexData)
	{
		return;
	}

	
	BYTE ReadBuf[64] = {0};
	UINT64 ReadSize = kHexOct16;
	if (m_HexDataLen == kHexOct16)
	{
		ReadSize = 16;
	}
	else if (m_HexDataLen == kHexOct08)
	{
		ReadSize = 8;
	}

	UINT64 uAddress = pHexData->GetPointAddress() + Item*ReadSize;
	ReadSize = pHexData->ReadData(uAddress,ReadBuf,ReadSize);
	{
		if ( Column == 0 )
		{
			if (ReadSize)
			{
				TCHAR szAsmBuf[64] = {0};
				uAddress = GetVirtualOffset(uAddress);
				if (m_bShowX64)
				{
					StringCchPrintf( szAsmBuf,64,_T(" %016llX"),uAddress);
				}
				else
				{
					StringCchPrintf(szAsmBuf,64,_T(" %08X"),uAddress);
				}				
				
				SetItemState( Item , GetItemState(Item) | kDynamicItemShowData);
				PaintDC.SetTextColor(HEX_DATA_ADDRESS_COLOR);
				PaintDC.DrawText(
					szAsmBuf, 
					-1,
					ClientRect,
					DT_EXPANDTABS |
					DT_TABSTOP |
					DT_LEFT |
					DT_SINGLELINE);
			}			
		}
		else if ( Column == 1)
		{
			BOOL bShowSize = 16;
			int m_FontWe = m_DynamicView.DyvBody.Height()/2;
			int m_FontHe = m_DynamicView.DyvBody.Height();

			if (m_HexDataLen == kHexOct16)
			{
				bShowSize = 16;
			}
			else if (m_HexDataLen == kHexOct08)
			{
				bShowSize = 8;
			}				
			
			CRect rcText;
			rcText = ClientRect;

			for ( int nI = 0; nI < bShowSize;nI++)
			{
				BOOL bDrawLine = FALSE;
				if (rcText.left > ClientRect.right)
				{
					rcText.left = ClientRect.right;
				}

				rcText.right = rcText.left + 3*m_FontWe;

				if (rcText.right > ClientRect.right)
				{
					rcText.right = ClientRect.right;
				}

				if (nI < ReadSize)
				{
					TCHAR szAsmBuf[64] = {0x20};
					StringCchPrintf(&szAsmBuf[1],64,_T("%02X"),ReadBuf[nI]);

					PaintDC.SetTextColor(HEX_FOCUS_TEXT_COLOR);

					if (!GetFocusState())
					{
						PaintDC.SetTextColor(HEX_NO_FOCUS_TEXT_COLOR);
					}

					PaintDC.DrawText(
						szAsmBuf,
						-1,
						rcText,
						DT_EXPANDTABS |
						DT_TABSTOP |
						DT_LEFT |
						DT_SINGLELINE);
					
				}

				rcText.left = rcText.right;
				
				if (m_bShowX64 == FALSE)
				{
					if ( nI == 3 || nI == 7 || nI == 11 || nI == 15)
					{
						bDrawLine = TRUE;
					}
					
				}
				else
				{
					if ( nI == 7 || nI == 15)
					{
						bDrawLine = TRUE;
					}
				}

				if(bDrawLine)
				{
					if ( ( rcText.right + m_FontWe/2 - 1 ) <= ClientRect.right)
					{
						CPoint TopLine,BottomLine;
						TopLine.x = (rcText.right + m_FontWe/2 - 1);
						TopLine.y = rcText.top;

						BottomLine.x = TopLine.x;
						BottomLine.y = rcText.bottom;

						PaintDC.SelectObject(m_HexLinePen);
						PaintDC.MoveTo(TopLine);
						PaintDC.LineTo(BottomLine);
					}					
				}
								
			}
		
		}
		else if( Column == 2)
		{
			TCHAR szAsmBuf[64] = {0};
			int nTextWidth = m_DynamicView.DyvBody.Height()/2;
			for ( int nI = 0; nI < ReadSize;)
			{
				if (m_bShowAnsi)
				{
					szAsmBuf[nI] = ReadBuf[nI];
					if (ReadBuf[nI] == 0)
					{
						szAsmBuf[nI] = '.';
					}

					
				}
				else
				{
					szAsmBuf[nI/2] = ((TCHAR*)ReadBuf)[nI];
					if (((TCHAR*)ReadBuf)[nI] == 0)
					{
						szAsmBuf[nI/2] = '.';
					}

					nI ++;					
				}
				nI++;
			}
			
			//StringCchPrintf( szAsmBuf,64," %s",ReadBuf);
			PaintDC.SetTextColor(HEX_FOCUS_TEXT_COLOR);
			CRect rcText = ClientRect;
			rcText.left += nTextWidth;
			PaintDC.DrawText(
				szAsmBuf, 
				-1,
				rcText,
				DT_EXPANDTABS |
				DT_TABSTOP |
				DT_LEFT |
				DT_SINGLELINE);
		}
	}

	
}

CDynamicDrawItem* CHexOtcDataView::OnGetDrawData()
{
	if (!m_HexOctData)
	{
		m_HexOctData = new CHexOctData();
	}

	return (CDynamicDrawItem*)m_HexOctData;
}

void CHexOtcDataView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{

}

void CHexOtcDataView::OnClickHeadDwon(int nColumn)
{
	
	int nTextWidth = m_DynamicView.DyvBody.Height()/2;

	if ( nColumn == 1 )
	{
		__super::OnClickHeadDwon(nColumn);
		SetHeaderClick(nColumn,kButtonDown);
// 		if ( m_HexDataLen == kHexOct16 )
// 		{
// 			m_HexDataLen = kHexOct08;
// 		}
// 		else
// 		{
// 			m_HexDataLen = kHexOct16;
// 		}
// 
// 		ModifyHeaderWidth(nColumn,nTextWidth*(3*m_HexDataLen + 2));
	}
	else if ( nColumn == 2)
	{
		__super::OnClickHeadDwon(nColumn);
		SetHeaderClick(nColumn,kButtonDown);
// 		if (m_bShowAnsi)
// 		{
// 			m_bShowAnsi = FALSE;
// 			ModifyHeaderText(nColumn,_T(" &UNICODE"),nTextWidth*(m_HexDataLen/2 +4));
// 		}
// 		else
// 		{
// 			m_bShowAnsi = TRUE;
// 			ModifyHeaderText(nColumn,_T(" &ASCII"),nTextWidth*(m_HexDataLen +4));
// 		}
	}
}

void CHexOtcDataView::OnClickHeadUp(int nColumn)
{
	int nTextWidth = m_DynamicView.DyvBody.Height()/2;

	if ( nColumn == 1 )
	{

		if ( m_HexDataLen == kHexOct16 )
		{
			m_HexDataLen = kHexOct08;
		}
		else
		{
			m_HexDataLen = kHexOct16;
		}

		ModifyHeaderWidth(nColumn,nTextWidth*(3*m_HexDataLen + 2));
	}
	else if ( nColumn == 2)
	{
		if (m_bShowAnsi)
		{
			m_bShowAnsi = FALSE;
			ModifyHeaderText(nColumn,_T(" &UNICODE"),nTextWidth*(m_HexDataLen/2 +4));
		}
		else
		{
			m_bShowAnsi = TRUE;
			ModifyHeaderText(nColumn,_T(" &ASCII"),nTextWidth*(m_HexDataLen +4));
		}
	}
}

void CHexOtcDataView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{
	USHORT ClickType = Click.Type();
	ULONG  ClickItem = Click.Item();
	ULONG ClickColumn = Click.Column();

	if (ClickType == kDynamicRClickedDown)
	{
		CPoint MousePoint;
		GetCursorPos(&MousePoint);//获取光标位置
		m_Menu.TrackPopupMenu(TPM_LEFTALIGN,MousePoint.x,MousePoint.y,m_hWnd);//在指定位置显示快捷菜单，并跟踪菜单选项
	}
	
}

PVOID CHexOtcDataView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	return NULL;
}

void CHexOtcDataView::ReleaseItemData(CDynamicDrawItem* pDraw)
{

}

void CHexOtcDataView::OnTimerSelectUp()
{

}

void CHexOtcDataView::OnTimerSelectDown()
{

}

int CHexOtcDataView::OnMoveScroll(int Pos)
{
	CHexOctData* pHexData = (CHexOctData*)m_HexOctData;

	if (pHexData)
	{
		UINT64 PointAddress = pHexData->GetPointAddress();
		
// 		if (pHexData->GetMoudleAddress() <= PointAddress &&
// 			(pHexData->GetMoudleAddress() + pHexData->GetMoudleSize()) >= PointAddress)
		{
			PointAddress += (16*Pos);

			if (PointAddress <= pHexData->GetMoudleAddress())
			{
				PointAddress = pHexData->GetMoudleAddress();
			}
			else if( (pHexData->GetMoudleAddress() + pHexData->GetMoudleSize()) < PointAddress )
			{
				PointAddress = ( pHexData->GetMoudleAddress() + pHexData->GetMoudleSize() + 16);
			}

			pHexData->SetHexDataAddress(PointAddress);
		}	

		return Pos;
	}
	return Pos;
}