#include "StdAfx.h"
#include "ReverseView.h"
#include "resource.h"
#include "common.h"
#include "global.h"

#define DEFAULT_VALUE_MULTI_MOV 300

CReverseView::CReverseView(void)
	:m_ReverseMode(kReverseIdaMode)
	,m_ReverseAmd64(FALSE)
	,m_pResolver(NULL)
	,m_Userdata(NULL)
	,m_pDrawItem(NULL)
{
	
}

CReverseView::~CReverseView(void)
{
	if (m_pDrawItem)
	{
		delete m_pDrawItem;
		m_pDrawItem = NULL;
	}
}


BOOL CReverseView::Init( HWND hWnd , HINSTANCE hInst , int ReverseMode )
{
	if(Create(hWnd,hInst,IDC_CPU_ASM_VIEW,_T("CReverseView")))
	{
		CReverseData* pData = new CReverseData();
		__super::Init(reinterpret_cast<CDynamicData*>(pData));
		InsertHeader(0,100,_T(" 地址"));
		InsertHeader(1,150,_T(" Hex数据"));
		InsertHeader(2,400,_T(" 反汇编"));
		InsertHeader(3,300,_T(" 注释"));
		ShowVScrollBar(TRUE);
		SetBodyCxSize(0);
		m_ReverseMode = ReverseMode;

		return TRUE;
	}
	
	return FALSE;
}

int CReverseView::OnMoveScroll(int Pos)
{
	int IMoveItem = 0;
	CReverseData::CDataItem* pCursor = m_ReverseData.FirstCursor();
	if (Pos >= 0)
	{
		for ( IMoveItem = 0; IMoveItem < Pos;IMoveItem++)
		{
			if (pCursor)
			{
				if (pCursor->Next())
				{
					pCursor = pCursor->Next();
					m_ReverseData.FirstCursor() = pCursor;
				}
				else
				{
					break;
				}
				
			}
			else
			{
				break;
			}
		}

		return IMoveItem;
	}
	else
	{
		for ( IMoveItem = 0; IMoveItem < abs(Pos);IMoveItem++)
		{
			if (pCursor)
			{
				if (pCursor->Pre())
				{
					pCursor = pCursor->Pre();
					m_ReverseData.FirstCursor() = pCursor;

				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		return (0-IMoveItem);
	}
	
	return 0;
}

int CReverseView::OnMouseMovMulti(
							int nItem,
							int nColumn,
							CPoint& pt,
							int& nDirect)
{
	CReverseData::CDataItem* pSelected = m_ReverseData.FirstCursor();
	CReverseData::CDataItem* pFSelect = m_ReverseData.FSlectItem();
	CReverseData::CDataItem* pLSelect = m_ReverseData.LSlectItem();
	int nI = 0;

	while ( nI < nItem)
	{
		if (pSelected->Next())
		{
			pSelected = pSelected->Next();
		}
		else
		{
			pSelected = NULL;
			break;
		}

		nI++;
	}

	long kkkk = nDirect;

	if (pSelected && pFSelect && pLSelect)
	{
		m_ReverseData.LSlectItem() = pSelected;
	}

	return 0;
}

void CReverseView::OnTimerSelectUp()
{
	CReverseData::CDataItem* pSelected = m_ReverseData.FirstCursor();
	if (pSelected)
	{
		m_ReverseData.LSlectItem() = pSelected;
	}
	
}

void CReverseView::OnTimerSelectDown()
{
	CReverseData::CDataItem* pSelected = m_ReverseData.FirstCursor();
	ULONG ColumnPagsSize = m_DynamicView.DyvBody.Size();

	int nI = 0;

	while ( nI < ColumnPagsSize)
	{
		if (pSelected->Next())
		{
			pSelected = pSelected->Next();
		}
		else
		{
			break;
		}
		nI++;
	}

	if (pSelected)
	{
		m_ReverseData.LSlectItem() = pSelected;
	}

}

void CReverseView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{
	USHORT ClickType = Click.Type();
	ULONG  OnClickItem = Click.Item();
	ULONG ClickColumn = Click.Column();
	CReverseData::CDataItem* pFirst = m_ReverseData.FirstCursor();

	if (ClickType == kDynamicClickedDown )
	{
		 int nItem = 0;

		while ( nItem < OnClickItem)
		{
			if (pFirst->Next())
			{
				pFirst = pFirst->Next();
			}
			else
			{
				pFirst = NULL;
				break;
			}

			nItem++;
		}

		if (pFirst)
		{
			m_ReverseData.FSlectItem() = m_ReverseData.LSlectItem() = pFirst;
		}
	}
	else if ( ClickType == kDynamicDBClickedDown )
	{

	}
	else if ( ClickType == kDynamicRClickedDown)
	{

	}
}

void CReverseView::OnPaintColumn(
						CMemDC& PaintDC,
						CRect& ClientRect,
						int Item, 
						int Column,
						CDynamicDrawItem* DrawItemData,
						BOOL bSelected )
{
	
	//__super::OnPaintColumn(PaintDC,ClientRect,Item,Column,DrawItemData);
	CReverseData::CDataItem* pFirstCursor = m_ReverseData.FirstCursor();
	
	CReverseDraw* pReverseDraw = (CReverseDraw*)DrawItemData;
	
	if (pReverseDraw && pReverseDraw->m_DataCursor)
	{
		//ud* ud_obj = &pReverseDraw->m_ReverseDisasm.ud_obj;
		CZ0BTDisasm* pDisam = &pReverseDraw->m_ReverseDisasm.Z0bDisam;

		if (pDisam->GetView())
		{
			PaintAsmColumn(
						PaintDC,
						ClientRect,
						pDisam->GetView(),
						Column,
						bSelected);			
		}
/*
		switch (Column)
		{
		case kReverseAddress:
			{
			TCHAR szAsmBuf[64] = {0};

			StringCchPrintf(szAsmBuf,64,_T(" %08X"),ud_insn_off(ud_obj));
			SetItemState( Item , GetItemState(Item) | kDynamicItemShowData);
			PaintDC.DrawText(
				szAsmBuf, 
				-1,
				ClientRect,
				DT_EXPANDTABS |
				DT_TABSTOP |
				DT_LEFT |
				DT_SINGLELINE);
			}
			break;
		case kReverseHexData:
			{
				TCHAR szAsmBuf[64] = {0};
				StringCchPrintf( szAsmBuf,64,_T(" %-16.16s"),ud_get_hex(ud_obj));
				PaintDC.DrawText(
					szAsmBuf, 
					-1,
					ClientRect,
					DT_EXPANDTABS |
					DT_TABSTOP |
					DT_LEFT |
					DT_SINGLELINE);
			}
			
			break;
		case kReverseDisasm:
			{
				TCHAR szAsmBuf[64] = {0};
				StringCchPrintf(szAsmBuf, 64,_T("  %s"),ud_insn_asm(ud_obj));
				PaintDC.DrawText(
					szAsmBuf, 
					-1,
					ClientRect,
					DT_EXPANDTABS |
					DT_TABSTOP |
					DT_LEFT |
					DT_SINGLELINE);
			}
			break;
		case kReverseNote:
			break;
		}

		if ( bLastColumn )
		{
			pFirstCursor = pFirstCursor->Next();
		}	
		*/	
	}
	else
	{
		SetItemState(Item,kDynamicItemUnShowData);
	}
	
}

void CReverseView::PaintAsmColumn(	
						CMemDC& PaintDC,
						CRect& rcColumn,
						CViewData* CustomItem,
						int nColumn,
						BOOL bSelect)
{
	if (!CustomItem)
	{
		return;
	}

	int m_FontWe = m_DynamicView.DyvBody.Height()/2;
	int m_FontHe = m_DynamicView.DyvBody.Height();
	TCHAR* lpszText = NULL;

	if (nColumn == 0)
	{
		lpszText = CustomItem->GetHeadText();
		
		PaintDC.SetBkMode(TRANSPARENT);

		if (bSelect)
		{
			if ( !GetFocusState() )
			{
				PaintDC.SetTextColor(CustomItem->GetHeadNonFontColor());
			}
			else
			{
				PaintDC.SetTextColor(CustomItem->GetHeadSelFontColor());
			}

			PaintDC.FillRect(rcColumn, (HBRUSH)m_DynamicView.DyvPaintSelectBrush);
		}
		else
		{
			PaintDC.SetTextColor(CustomItem->GetHeadNorFontColor());
			PaintDC.FillRect(rcColumn, (HBRUSH)m_DynamicView.DyvPaintNormalBrush);
		}

		if (lpszText != NULL)
		{
			PaintDC.DrawText(
						lpszText, 
						-1,
						rcColumn,
						DT_EXPANDTABS | 
						DT_VCENTER |
						DT_SINGLELINE);
		}
	}
	else if( nColumn < CUSTOM_MAX_COLUMN )
	{
		if (bSelect)
		{
			PaintDC.FillRect(rcColumn, (HBRUSH)m_DynamicView.DyvPaintSelectBrush);
		}
		else
		{
			PaintDC.FillRect(rcColumn, (HBRUSH)m_DynamicView.DyvPaintNormalBrush);
		}

		RECT rcIcon1;
		rcIcon1.left = rcColumn.left + 2;
		rcIcon1.top = rcColumn.top;
		rcIcon1.bottom = rcColumn.bottom;
		rcIcon1.right = rcIcon1.left;
		tagIcon* pIcon = CustomItem->GetColumnIcon(nColumn - 1);
		int IconCount = CustomItem->GetColumnIconCount(nColumn - 1);

		if (pIcon && IconCount)
		{
			
			for ( int nIndex = 0 ;
				nIndex <  IconCount;
				nIndex++)
			{
				rcIcon1.right += m_FontWe;

				if (pIcon[nIndex].nIcon == FUNC_INER_ICON)
				{
					PaintDC.SetTextColor(pIcon[nIndex].colorIcon);
					//SetBkMode(hDC, TRANSPARENT);

					PaintDC.SelectObject(m_CustomPen[kCustomBlackPen]);
					PaintDC.MoveTo(rcIcon1.left + m_FontWe/2, rcIcon1.top + m_FontHe / 2);
					PaintDC.LineTo(rcIcon1.left + m_FontWe/2, rcIcon1.top + m_FontHe / 2 + 2);
					PaintDC.MoveTo(rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2);
					PaintDC.LineTo(rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2 + 2);
				}
				else if (pIcon[nIndex].nIcon == JMP_CALL_ICON )
				{
					PaintDC.SelectObject(m_CustomPen[kCustomBlackPen]);
					PaintDC.MoveTo(rcIcon1.left + 2*m_FontWe/5, rcIcon1.top + m_FontHe / 2);
					PaintDC.LineTo(rcIcon1.left + m_FontWe, rcIcon1.top + m_FontHe / 2);
					//MoveToEx(hDC, rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2, 0);
					//LineTo(hDC, rcIcon1.left + m_FontWe/2 + 1, rcIcon1.top + m_FontHe / 2 + 2);
				}
				else if ( pIcon[nIndex].nIcon == FUNCTION_HREADER_FLAGS )
				{
					PaintDC.SetTextColor(pIcon[nIndex].colorIcon);
					PaintDC.SetBkMode(TRANSPARENT);
					PaintDC.DrawText(
						FUCYION_HEADER_CHAR, 
						-1,
						rcIcon1,
						DT_EXPANDTABS | 
						DT_LEFT |
						DT_SINGLELINE);
				}
				else if (pIcon[nIndex].nIcon == FUCTION_BODY_HEADER_FLAGS)
				{
					PaintDC.SelectObject(m_CustomPen[kCustomBlackPen]);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2);
					PaintDC.LineTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2 + 1);
					PaintDC.LineTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2 + 1);

					PaintDC.MoveTo(rcIcon1.left+1, rcIcon1.top + m_FontHe / 2 +1);
					PaintDC.LineTo(rcIcon1.left+1, rcIcon1.bottom);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2 + 1);
					PaintDC.LineTo(rcIcon1.left+2, rcIcon1.bottom);
				}
				else if (pIcon[nIndex].nIcon == FUCTION_RET_FLAGS )
				{
					PaintDC.SelectObject(m_CustomPen[kCustomBlackPen]);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2);
					PaintDC.LineTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top + m_FontHe / 2 + 1);
					PaintDC.LineTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2 + 1);

					PaintDC.MoveTo(rcIcon1.left+1, rcIcon1.top);
					PaintDC.LineTo(rcIcon1.left+1, rcIcon1.top + m_FontHe / 2);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top);
					PaintDC.LineTo(rcIcon1.left+2, rcIcon1.top + m_FontHe / 2);
				}
				else if (pIcon[nIndex].nIcon == FUNCTION_BODY_FLAGS)
				{
					PaintDC.SelectObject(m_CustomPen[kCustomBlackPen]);
					PaintDC.MoveTo(rcIcon1.left+1, rcIcon1.top);
					PaintDC.LineTo(rcIcon1.left+1, rcIcon1.bottom);
					PaintDC.MoveTo(rcIcon1.left + 2, rcIcon1.top);
					PaintDC.LineTo(rcIcon1.left+2, rcIcon1.bottom);
				}
				else if (pIcon[nIndex].nIcon == JMP_OTHER_BODY_FLAGS)
				{
					/*
					if (m_ScreenView.GetSelItem() == CustomItem && CustomItem->tagJmpTo )
					{
						if (CustomItem->GetInstructionType() == kInstructionJmp)
						{
							PaintDC.SelectObject(m_CustomPen[kCustomRedPen]);
							PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2);
							PaintDC.LineTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2);

							if (CustomItem->tagJmpTo->GetPtrValue() < CustomItem->GetPtrValue())
							{
								PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2);
								PaintDC.LineTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top - 1);
							}
							else
							{
								PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2);
								PaintDC.LineTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
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
								PaintDC.SelectObject(m_CustomPen[kCustomRedPen]);
								PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2);
								PaintDC.LineTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2);
								PaintDC.LineTo(rcIcon1.right - 4, rcIcon1.top + m_FontHe / 2 - 4);
								PaintDC.MoveTo(rcIcon1.right, rcIcon1.top + m_FontHe / 2);
								PaintDC.LineTo(rcIcon1.right - 4, rcIcon1.top + m_FontHe / 2 + 4);

								if (pSelItem->tagJmpTo->GetPtrValue() < pSelItem->GetPtrValue())
								{
									PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2);
									PaintDC.LineTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
								}
								else
								{
									PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top + m_FontHe / 2);
									PaintDC.LineTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top - 1);
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
										PaintDC.SelectObject(m_CustomPen[kCustomRedPen]);
										PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top);
										PaintDC.LineTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
									}

								}
								else if (pSelItem->tagJmpTo->GetPtrValue() < pSelItem->GetPtrValue())
								{
									if ( CustomItem->GetPtrValue() > pSelItem->tagJmpTo->GetPtrValue() &&
										CustomItem->GetPtrValue() < pSelItem->GetPtrValue())
									{
										PaintDC.SelectObject(m_CustomPen[kCustomRedPen]);
										PaintDC.MoveTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.top);
										PaintDC.LineTo(rcIcon1.left + m_FontWe*1 / 6, rcIcon1.bottom);
									}
								}
							}
						}
					}
					*/
				}

				rcIcon1.left = rcIcon1.right;
			}
		}

		rcIcon1.right += m_FontWe/2;
		tagOption* pOption = CustomItem->GetColumnOption(nColumn-1);
		int OptionCount = CustomItem->GetColumnOptionCount(nColumn - 1);
		if (pOption )
		{
			for( int i = 0; i < OptionCount; i++ )
			{
				lpszText = pOption[i].lpText;
				int rcWidth = m_FontWe * pOption[i].TextSize;

				if (OptionCount > 1)
				{
					int kkk = 0;
					kkk++;
				}

				if (!bSelect)
				{
					PaintDC.SetTextColor(pOption[i].NorFontColor);
				}
				else
				{
					PaintDC.SetTextColor(pOption[i].SelFontColor);
				}

				if ( !GetFocusState())
				{
					PaintDC.SetTextColor(pOption[i].HasNonFocus);
				}

				if (pOption[i].FontBgColor == -1)
				{
					PaintDC.SetBkMode(TRANSPARENT);
				}
				else
				{
					PaintDC.SetBkMode(OPAQUE);
					PaintDC.SetBkColor(pOption[i].FontBgColor);
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

					PaintDC.DrawText(
						lpszText, 
						-1,
						rcText,
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
				PaintDC.FillRect(rcColumn, (HBRUSH)m_DynamicView.DyvPaintSelectBrush);
			}
			else
			{
				PaintDC.FillRect(rcColumn, (HBRUSH)m_DynamicView.DyvPaintNormalBrush);
			}
		}		
	}	
}

BOOL CReverseView::ReadData( void* lpData,uint64_t lpSize,int ReadType)
{
	return m_ReverseData.ReadData(lpData,lpSize,ReadType);
}

BOOL CReverseView::GetRawData(  void*& lpData,int& lpSize )
{
	return m_ReverseData.GetRawData(lpData,lpSize);
}

BOOL CReverseData::DecodeFromMem(
	CReverseView* pView,
	PBYTE pBuffer,
	uint64_t cBufferSize,
	float  flTotle,
	uint64_t VirtualAddress,
	BOOL	bReNew,
	BOOL	IsAmd64)
{
	ud ud_obj;
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj,IsAmd64==TRUE?64:32);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
	ud_set_pc(&ud_obj,VirtualAddress);
	ud_set_input_buffer(&ud_obj,pBuffer,cBufferSize);
	CDataItem* pNext = m_DataItem;

	if (!bReNew)
	{
		pNext = m_TrailDataItem;		
	}

	float flAddProcess = flTotle/(float)cBufferSize;

	while(ud_decode(&ud_obj))
	{
		TCHAR szShowInfo[MAX_PATH] = {0};
		StringCchPrintf((STRSAFE_LPWSTR)szShowInfo,MAX_PATH,_T(" 解析地址:%X"),ud_insn_off(&ud_obj));
		g_mainStatusBar.ShowStatusInfo(szShowInfo);
		g_mainStatusBar.AddProcess(flAddProcess);
		CDataItem* pTem = new CDataItem();
		if (pTem)
		{
			m_InstrCount++;

			if (m_InstrCount == 100)
			{
				pView->InvalidateRect();
			}
			pTem->m_IntsrPtr = ud_insn_off(&ud_obj);
			//pTem->m_InstrSize = ud_insn_len(&ud_obj);
			//pTem->m_RealIntsr = (uint64_t)ud_obj.inp_buf;
			if (pNext)
			{
				pNext->m_Next = pTem;
				pTem->m_Pre = pNext;
				m_TrailDataItem = pTem;
			}
			else
			{
				m_DataItem = pTem;
				m_DataCursor = m_DataItem;
				if (!bReNew)
				{
					m_TrailDataItem = pTem;
				}

			}

			pNext = pTem;
		}
	}

	return TRUE;
}

BOOL CReverseView::DecodeFromMem(PBYTE pBuffer, uint64_t cBufferSize,float  flTotle,uint64_t VirtualOffset,BOOL bReNew)
{
	if (pBuffer && cBufferSize)
	{
		BOOL bSuccess = m_ReverseData.DecodeFromMem(this,pBuffer,cBufferSize,flTotle,VirtualOffset,bReNew,m_ReverseAmd64);
		if (bSuccess)
		{
			SetScrollRange(SB_VERT,0,m_ReverseData.GetInstrCount(),TRUE);

			InvalidateRect();
		}

		return bSuccess;
	}
}

CDynamicDrawItem* CReverseView::OnGetDrawData()
{
	if (!m_pDrawItem)
	{
		m_pDrawItem = new CReverseDraw();
	}

	CReverseDraw* pDrawItem = m_pDrawItem;
	if (pDrawItem)
	{
		pDrawItem->m_DataCursor = m_ReverseData.FirstCursor();
	}

	return (CDynamicDrawItem*)pDrawItem;
}

void CReverseView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{
// 	if ( pDraw)
// 	{
// 		delete ((CReverseDraw*)pDraw);
// 	}
}


PVOID CReverseView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	CReverseDraw* pReverse = (CReverseDraw*)pDraw;

	if (!pReverse)
	{
		return 0;
	}

	if (pReverse->GetInstrOffset() >= 0 )
	{
		//uint64_t RealOffset = pReverse->GetInstrOffset() - m_ReverseData.m_DataBaseValues;
		BYTE Instr[30] = {0};
		//ud* ud_obj = &pReverse->m_ReverseDisasm.ud_obj;
		CZ0BTDisasm* pDisam = &pReverse->m_ReverseDisasm.Z0bDisam;
		if(m_ReverseData.ReadProcessData(pReverse->GetInstrOffset(),Instr) &&
			pDisam )
		{
			//ud_init(ud_obj);
			//ud_set_pc(ud_obj,pReverse->GetInstrOffset());
			//m_ReverseData.Disasm32(ud_obj,Instr,30);
			pDisam->SetResolverSymbol(m_pResolver,m_Userdata);
			pDisam->SetMode(m_ReverseAmd64==TRUE?64:32);
			pDisam->DisasmBuffer(Instr,30,pReverse->GetInstrOffset(),m_ReverseAmd64);

		}

		CReverseData::CDataItem* pFSelectItem = m_ReverseData.FSlectItem();
		CReverseData::CDataItem* pLSelectItem = m_ReverseData.LSlectItem();
		SetItemState( nItem, kDynamicItemShowData);
		if (pFSelectItem && pLSelectItem)
		{
#ifdef _DEBUG
			TCHAR szError[100];
			
			wsprintf(
					szError,
					L"pFSelectItem:%X",
					pFSelectItem->GetInstrPtr());

			OutputDebugString(szError);

			wsprintf(
				szError,
				L"pLSelectItem:%X",
				pLSelectItem->GetInstrPtr());

			OutputDebugString(szError);
#endif
			if (( pReverse->GetInstrOffset() >= pFSelectItem->GetInstrPtr() &&
				pReverse->GetInstrOffset() <= pLSelectItem->GetInstrPtr()) ||
				pReverse->GetInstrOffset() <= pFSelectItem->GetInstrPtr() &&
				pReverse->GetInstrOffset() >= pLSelectItem->GetInstrPtr())
			{
				SetItemState(nItem,kDynamicItemShowData | kDynamicItemSelect);
			}
		}
	}

	return 0;
}

void CReverseView::OnClickItemUp()
{
	if ( m_ReverseData.FSlectItem() && m_ReverseData.LSlectItem())
	{
		if (m_ReverseData.FSlectItem()->GetInstrPtr() > m_ReverseData.LSlectItem()->GetInstrPtr())
		{
			CReverseData::CDataItem* pFSelect = m_ReverseData.FSlectItem();
			m_ReverseData.FSlectItem() = m_ReverseData.LSlectItem();
			m_ReverseData.LSlectItem() = pFSelect;
		}
	}
}

void CReverseView::ReleaseItemData(CDynamicDrawItem* pDraw)
{
	CReverseDraw* pReverse = (CReverseDraw*)pDraw;

	if (pReverse)
	{
		CZ0BTDisasm* pDisam = &pReverse->m_ReverseDisasm.Z0bDisam;
		if (pDisam)
		{
			pDisam->ReleaseViewData();
		}
	}

	if (pReverse && pReverse->m_DataCursor)
	{
		pReverse->m_DataCursor = pReverse->m_DataCursor->Next();
	}
}