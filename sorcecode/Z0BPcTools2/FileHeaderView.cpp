#include "StdAfx.h"
#include "FileHeaderView.h"
#include "resource.h"

CFileHeaderView::CFileHeaderView(void)
	:m_nPointData(0)
	,m_FSelectItem(-1)
	,m_LSelectItem(-1)
	,m_pFileBuffer(0)
	,m_FileBufferSize(0)
	,m_FileName(NULL)
{
}

CFileHeaderView::~CFileHeaderView(void)
{
}

BOOL CFileHeaderView::Init( HWND hWnd , HINSTANCE hInst)
{
	if(Create(hWnd,hInst,IDC_PE_HEADER_VIEW,_T("CFileHeaderView")))
	{
		//CDosHeaderData* pData = new CDosHeaderData();
		__super::Init(reinterpret_cast<CDynamicData*>(NULL));
		InsertHeader(0,70,_T(" 偏移"));
		InsertHeader(1,100,_T(" Hex数据"));
		InsertHeader(2,150,_T(" PE文件头信息"));
		InsertHeader(3,200,_T(" 注释"));
		SetBodyCxSize(0);
		ShowVScrollBar(TRUE);		
		SetScrollRange(SB_VERT,0,MAX_DOS_HEADER_INFO_SIZE,TRUE);
		
		return TRUE;
	}

	return FALSE;
}

void CFileHeaderView::OnPaintColumn(
								CMemDC& PaintDC,
								CRect& ClientRect,
								int Item,
								int Column,
								CDynamicDrawItem* DrawItemData,
								BOOL bSelected)
{
	CDosHeaderData* pData = (CDosHeaderData*)DrawItemData;
	__super::OnPaintColumn(PaintDC,ClientRect,Item,Column,DrawItemData,bSelected);
	if (pData && pData->GetViewData(m_nPointData + Item))
	{		
		SetItemState( Item , GetItemState(Item) | kDynamicItemShowData);
		PaintColumn(PaintDC,ClientRect,pData->GetViewData(m_nPointData + Item),Column,bSelected);
	}
	
}

void CFileHeaderView::PaintColumn(	
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

CDynamicDrawItem* CFileHeaderView::OnGetDrawData()
{
	CDosHeaderData* pDosHead = new CDosHeaderData(m_pFileBuffer,m_FileBufferSize);
	if (pDosHead)
	{
		pDosHead->GetWinExtFileInfo(m_FileName);
	}

	return (CDynamicDrawItem*)(pDosHead);
}

void CFileHeaderView::ReleaseDrawData(CDynamicDrawItem* pDraw)
{
	CDosHeaderData* pDosHead = (CDosHeaderData*)pDraw;
	if (pDosHead)
	{
		delete pDosHead;
		pDosHead = NULL;
	}
}

void CFileHeaderView::OnClickItem(CRect& ClientRect,CDynamicClickItem& Click )
{
	USHORT ClickType = Click.Type();
	ULONG  ClickItem = Click.Item();
	ULONG ClickColumn = Click.Column();
	CDosHeaderData* pDosHead = (CDosHeaderData*)Click.ExtData();
	if (ClickType == kDynamicClickedDown )
	{
		FirstSelect() = LastSelect() = (m_nPointData + ClickItem);
		//m_nPointData;

	}
	else if ( ClickType == kDynamicDBClickedDown )
	{

	}
	else if ( ClickType == kDynamicRClickedDown)
	{

	}
}

PVOID CFileHeaderView::OnGetItemData(int nItem ,CDynamicDrawItem* pDraw)
{
	CDosHeaderData* pDosHead = (CDosHeaderData*)pDraw;
	UINT64 uDefaultSelect = -1;
	if (pDosHead && FirstSelect() != uDefaultSelect &&
		LastSelect() != uDefaultSelect)
	{
		if (FirstSelect() <= (m_nPointData + nItem) &&
			LastSelect() >= (m_nPointData + nItem))
		{
			SetItemState(nItem,GetItemState(nItem) | kDynamicItemSelect);
		}
	}

	return pDosHead;
}

void CFileHeaderView::ReleaseItemData(CDynamicDrawItem* pDraw)
{

}

void CFileHeaderView::OnTimerSelectUp()
{

}

void CFileHeaderView::OnTimerSelectDown()
{

}

int CFileHeaderView::OnMoveScroll(int Pos)
{
	m_nPointData += Pos;
	if (m_nPointData < 0)
	{
		m_nPointData = 0;
	}

	return Pos;
}


BOOL CDosHeaderData::GetWinExtFileInfo(LPTSTR	FileName)
{
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	PBYTE pBase = m_pFileBuffer ;
	int FileSize = m_FileBufferSize;

	if ( FileSize > ( sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS))&& 
		pBase &&
		m_WinExtFile )
	{
		BOOL IsAmd64 = FALSE;
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBase;
		if ( pDosHeader && pDosHeader->e_magic == IMAGE_DOS_SIGNATURE )
		{
			pNtHeader = (PIMAGE_NT_HEADERS)&((const unsigned char *)(pBase))[pDosHeader->e_lfanew];
			if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
			{
				uint64_t nAddress = 0;
				PIMAGE_FILE_HEADER pFileHeader = &pNtHeader->FileHeader;
				PIMAGE_OPTIONAL_HEADER32	pOption = (PIMAGE_OPTIONAL_HEADER32)&pNtHeader->OptionalHeader;
				PIMAGE_OPTIONAL_HEADER64    pOption64 = (PIMAGE_OPTIONAL_HEADER64)&pNtHeader->OptionalHeader;

				PIMAGE_DATA_DIRECTORY	pDataDirectory = (PIMAGE_DATA_DIRECTORY)&pNtHeader->OptionalHeader.DataDirectory;
				PIMAGE_SECTION_HEADER	pSection = (PIMAGE_SECTION_HEADER)( (PBYTE)pNtHeader + sizeof(IMAGE_NT_HEADERS));

				TCHAR szAddress[64] = {0};				
				if(pFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64 ||
					pFileHeader->Machine == IMAGE_FILE_MACHINE_IA64)
				{
					IsAmd64 = TRUE;
				}

				uint64_t ImageBase = pOption->ImageBase;
				uint64_t ImageSize = pOption->SizeOfImage;
				uint64_t ImageImportRva = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;			
				int		 ImageImportSize = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
				uint64_t ImageRelocationRva = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
				int      ImageRelocationSize = pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

				if (IsAmd64)
				{
					ImageBase = pOption64->ImageBase;
					ImageSize = pOption64->SizeOfImage;
					ImageImportRva			= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
					ImageImportSize			= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

					ImageRelocationRva	= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
					ImageRelocationSize		= pOption64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
					pSection = (PIMAGE_SECTION_HEADER)( (PBYTE)pNtHeader + sizeof(IMAGE_NT_HEADERS64));;
				}

				int nI = 0;
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_magic),pDosHeader->e_magic,_T("e_magic"),JL_FONT_INS_COLOR,_T("PE头标志"),FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cblp),pDosHeader->e_cblp,_T("e_cblp"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cp),pDosHeader->e_cp,_T("e_cp"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_crlc),pDosHeader->e_crlc,_T("e_crlc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cparhdr),pDosHeader->e_cparhdr,_T("e_cparhdr"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_minalloc),pDosHeader->e_minalloc,_T("e_minalloc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_maxalloc),pDosHeader->e_maxalloc,_T("e_maxalloc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_ss),pDosHeader->e_ss,_T("e_ss"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_sp),pDosHeader->e_sp,_T("e_sp"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_csum),pDosHeader->e_csum,_T("e_csum"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_ip),pDosHeader->e_ip,_T("e_ip"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_cs),pDosHeader->e_cs,_T("e_cs"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_lfarlc),pDosHeader->e_lfarlc,_T("e_lfarlc"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_ovno),pDosHeader->e_ovno,_T("e_ovno"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[0]),pDosHeader->e_res[0],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[1]),pDosHeader->e_res[1],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[2]),pDosHeader->e_res[2],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res[3]),pDosHeader->e_res[3],_T("e_res"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_oemid),pDosHeader->e_oemid,_T("e_oemid"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_oeminfo),pDosHeader->e_oeminfo,_T("e_oeminfo"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);	
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[0]),pDosHeader->e_res2[0],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[1]),pDosHeader->e_res2[1],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[2]),pDosHeader->e_res2[2],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[3]),pDosHeader->e_res2[3],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[4]),pDosHeader->e_res2[4],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[5]),pDosHeader->e_res2[5],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[6]),pDosHeader->e_res2[6],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[7]),pDosHeader->e_res2[7],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[8]),pDosHeader->e_res2[8],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_res2[9]),pDosHeader->e_res2[9],_T("e_res2"),OP_MEM_FONT_DATA_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".DosHeader"),sizeof(pDosHeader->e_lfanew),pDosHeader->e_lfanew,_T("e_lfanew"),JL_FONT_INS_COLOR,_T("NT头偏移"),FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
				nAddress = pDosHeader->e_lfanew;

				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".NtHeader"),sizeof(pNtHeader->Signature),pNtHeader->Signature,_T("Signature"),JL_FONT_INS_COLOR,_T("NT头标志"),0,0,FUNCTION_HREADER_FLAGS,IsAmd64);

				TCHAR szCpuInfo[30] = {0};
				switch (pFileHeader->Machine)
				{
				case IMAGE_FILE_MACHINE_I386:
					StringCchCopy(szCpuInfo,30,_T("Intel 386"));
					break;
				case IMAGE_FILE_MACHINE_R3000:
					StringCchCopy(szCpuInfo,30,_T("MIPS little"));
					break;
				case IMAGE_FILE_MACHINE_R4000:
					StringCchCopy(szCpuInfo,30,_T(" MIPS little"));
					break;
				case IMAGE_FILE_MACHINE_R10000:
					StringCchCopy(szCpuInfo,30,_T("MIPS little"));
					break;
				case IMAGE_FILE_MACHINE_WCEMIPSV2:
					StringCchCopy(szCpuInfo,30,_T("MIPS little V2"));
					break;
				case IMAGE_FILE_MACHINE_ALPHA:
					StringCchCopy(szCpuInfo,30,_T("Alpha_AXP"));
					break;
				case IMAGE_FILE_MACHINE_SH3:
					StringCchCopy(szCpuInfo,30,_T("SH3 little"));
					break;
				case IMAGE_FILE_MACHINE_ARM:
					StringCchCopy(szCpuInfo,30,_T("ARM Little"));
					break;
				case IMAGE_FILE_MACHINE_IA64:
					StringCchCopy(szCpuInfo,30,_T("Intel 64"));
					IsAmd64 = TRUE;
					break;
				case IMAGE_FILE_MACHINE_ALPHA64:
					StringCchCopy(szCpuInfo,30,_T("ALPHA64"));
					break;
				case IMAGE_FILE_MACHINE_AMD64:
					StringCchCopy(szCpuInfo,30,_T("AMD64 (K8)"));
					IsAmd64 = TRUE;
					break;
				default:
					StringCchCopy(szCpuInfo,30,_T("未知"));
				}

				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->Machine),pFileHeader->Machine,_T("Machine"),JL_FONT_INS_COLOR,szCpuInfo,FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->NumberOfSections),pFileHeader->NumberOfSections,_T("NumberOfSections"),FILE_HEADER_SHOW_COLOR,_T("节段数量"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->TimeDateStamp),pFileHeader->TimeDateStamp,_T("TimeDateStamp"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->PointerToSymbolTable),pFileHeader->PointerToSymbolTable,_T("PointerToSymbolTable"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->NumberOfSymbols),pFileHeader->NumberOfSymbols,_T("NumberOfSymbols"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->SizeOfOptionalHeader),pFileHeader->SizeOfOptionalHeader,_T("SizeOfOptionalHeader"),FILE_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
				m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".FileHeader"),sizeof(pFileHeader->Characteristics),pFileHeader->Characteristics,_T("Characteristics"),FILE_HEADER_SHOW_COLOR,NULL,FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);

				if (IsAmd64)
				{
#ifndef _WIN64
					TCHAR szAddressEntry[64] = {0};
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->Magic),pOption64->Magic,_T("Magic"),JL_FONT_INS_COLOR,_T("pe可选头标志"),FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorLinkerVersion),pOption64->MajorLinkerVersion,_T("MajorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MinorLinkerVersion),pOption64->MinorLinkerVersion,_T("MinorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfCode),pOption64->SizeOfCode,_T("SizeOfCode"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfInitializedData),pOption64->SizeOfInitializedData,_T("SizeOfInitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfUninitializedData),pOption64->SizeOfUninitializedData,_T("SizeOfUninitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("%s.%016llX"),FileName,pOption64->AddressOfEntryPoint + pOption64->ImageBase);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->AddressOfEntryPoint),pOption64->AddressOfEntryPoint,szAddressEntry,JL_FONT_INS_COLOR,_T("入口点地址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->BaseOfCode),pOption64->BaseOfCode,_T("BaseOfCode"),JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					//ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->BaseOfData),pOption64->BaseOfData,_T("BaseOfData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS);
					StringCchPrintf(szAddressEntry,64,_T("%s.%016llX"),FileName,pOption64->ImageBase);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->ImageBase),pOption64->ImageBase,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件基址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SectionAlignment),pOption64->SectionAlignment,_T("SectionAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->FileAlignment),pOption64->FileAlignment,_T("FileAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorOperatingSystemVersion),pOption64->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorOperatingSystemVersion),pOption64->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorImageVersion),pOption64->MajorImageVersion,_T("MajorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MinorImageVersion),pOption64->MinorImageVersion,_T("MinorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MajorSubsystemVersion),pOption64->MajorSubsystemVersion,_T("MajorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->MinorSubsystemVersion),pOption64->MinorSubsystemVersion,_T("MinorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->Win32VersionValue),pOption64->Win32VersionValue,_T("Win32VersionValue"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("0x%016llX"),pOption64->SizeOfImage);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfImage),pOption64->SizeOfImage,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件大小"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfHeaders),pOption64->SizeOfHeaders,_T("SizeOfHeaders"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->CheckSum),pOption64->CheckSum,_T("CheckSum"),OPTION_HEADER_SHOW_COLOR,_T("镜像文件校验和"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->Subsystem),pOption64->Subsystem,_T("Subsystem"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->DllCharacteristics),pOption64->DllCharacteristics,_T("DllCharacteristics"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfStackReserve),pOption64->SizeOfStackReserve,_T("SizeOfStackReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfStackCommit),pOption64->SizeOfStackCommit,_T("SizeOfStackCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfHeapReserve),pOption64->SizeOfHeapReserve,_T("SizeOfHeapReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->SizeOfHeapCommit),pOption64->SizeOfHeapCommit,_T("SizeOfHeapCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->LoaderFlags),pOption64->LoaderFlags,_T("LoaderFlags"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption64->NumberOfRvaAndSizes),pOption64->NumberOfRvaAndSizes,_T("NumberOfRvaAndSizes"),OPTION_HEADER_SHOW_COLOR,NULL,FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
#endif

				}
				else
				{
					TCHAR szAddressEntry[64] = {0};

					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->Magic),pOption->Magic,_T("Magic"),JL_FONT_INS_COLOR,_T("pe可选头标志"),FUCTION_BODY_HEADER_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorLinkerVersion),pOption->MajorLinkerVersion,_T("MajorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MinorLinkerVersion),pOption->MinorLinkerVersion,_T("MinorLinkerVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfCode),pOption->SizeOfCode,_T("SizeOfCode"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfInitializedData),pOption->SizeOfInitializedData,_T("SizeOfInitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfUninitializedData),pOption->SizeOfUninitializedData,_T("SizeOfUninitializedData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("%s.%X"),FileName,pOption->AddressOfEntryPoint + pOption->ImageBase);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->AddressOfEntryPoint),pOption->AddressOfEntryPoint,szAddressEntry,JL_FONT_INS_COLOR,_T("入口点地址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->BaseOfCode),pOption->BaseOfCode,_T("BaseOfCode"),JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->BaseOfData),pOption->BaseOfData,_T("BaseOfData"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("%s.%X"),FileName,pOption->ImageBase);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->ImageBase),pOption->ImageBase,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件基址"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SectionAlignment),pOption->SectionAlignment,_T("SectionAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->FileAlignment),pOption->FileAlignment,_T("FileAlignment"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorOperatingSystemVersion),pOption->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorOperatingSystemVersion),pOption->MajorOperatingSystemVersion,_T("MajorOperatingSystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorImageVersion),pOption->MajorImageVersion,_T("MajorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MinorImageVersion),pOption->MinorImageVersion,_T("MinorImageVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MajorSubsystemVersion),pOption->MajorSubsystemVersion,_T("MajorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->MinorSubsystemVersion),pOption->MinorSubsystemVersion,_T("MinorSubsystemVersion"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->Win32VersionValue),pOption->Win32VersionValue,_T("Win32VersionValue"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					StringCchPrintf(szAddressEntry,64,_T("0x%X"),pOption->SizeOfImage);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfImage),pOption->SizeOfImage,szAddressEntry,JL_FONT_INS_COLOR,_T("镜像文件大小"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfHeaders),pOption->SizeOfHeaders,_T("SizeOfHeaders"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->CheckSum),pOption->CheckSum,_T("CheckSum"),OPTION_HEADER_SHOW_COLOR,_T("镜像文件校验和"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->Subsystem),pOption->Subsystem,_T("Subsystem"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->DllCharacteristics),pOption->DllCharacteristics,_T("DllCharacteristics"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfStackReserve),pOption->SizeOfStackReserve,_T("SizeOfStackReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfStackCommit),pOption->SizeOfStackCommit,_T("SizeOfStackCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfHeapReserve),pOption->SizeOfHeapReserve,_T("SizeOfHeapReserve"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->SizeOfHeapCommit),pOption->SizeOfHeapCommit,_T("SizeOfHeapCommit"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->LoaderFlags),pOption->LoaderFlags,_T("LoaderFlags"),OPTION_HEADER_SHOW_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);
					m_WinExtFile[nI++].ShowDosHeaderInfo(nAddress,_T(".OptionHeader"),sizeof(pOption->NumberOfRvaAndSizes),pOption->NumberOfRvaAndSizes,_T("NumberOfRvaAndSizes"),OPTION_HEADER_SHOW_COLOR,NULL,FUCTION_RET_FLAGS,0,FUNCTION_HREADER_FLAGS,IsAmd64);

				}

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
					_T("Export Directory RVA"),
					JL_FONT_INS_COLOR,
					_T("导出表RVA"),
					FUCTION_BODY_HEADER_FLAGS,
					0,
					FUNCTION_HREADER_FLAGS,
					IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size,
					_T("Export Directory Size"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
					_T("Import Directory RVA"),
					JL_FONT_INS_COLOR,
					_T("导入表RVA"),FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size,
					_T("Import Directory Size"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress,
					_T("Resource Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("资源表RVA"),FUNCTION_BODY_FLAGS,
					0,0,
					IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size,
					_T("Resource Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress,
					_T("Exception Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("异常链表RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size,
					_T("Exception Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress,
					_T("Security Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("安全属性节RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size,
					_T("Security Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,
					_T("Relocation Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,
					_T("重定向表RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size,
					_T("Relocation Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress,
					_T("Debug Directory RVA"),
					PUSH_POP_FONT_INS_COLOR
					,_T("调试符号表RVA"),FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size,
					_T("Debug Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress,
					_T("Architecture Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size,
					_T("Architecture Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress,
					_T("Reserved"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size,
					_T("Reserved"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress,
					_T("TLS Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size,
					_T("TLS Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress,
					_T("Configuration Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size,
					_T("Configuration Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress,
					_T("Bound Import Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size,
					_T("Bound Import Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress,
					_T("Import Address Table Directory RVA"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size,
					_T("Import Address Table Directory Size"),
					JL_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress,
					_T("Delay Import Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size,
					_T("Delay Import Directory Size"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress,
					_T(".Net MetaData Directory RVA"),
					PUSH_POP_FONT_INS_COLOR,NULL,FUNCTION_BODY_FLAGS,0,0,IsAmd64);

				m_WinExtFile[nI++].ShowDosHeaderInfo(
					nAddress,
					_T(".Directory"),
					sizeof(pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size),
					pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size,
					_T(".Net MetaData Directory Size"),
					PUSH_POP_FONT_INS_COLOR,
					NULL,
					FUCTION_RET_FLAGS,
					0,
					FUNCTION_HREADER_FLAGS,IsAmd64);

				nAddress = (ULONG_PTR)((UCHAR*)pSection - (UCHAR*)pBase);
				for ( int i = 0 ; i < pNtHeader->FileHeader.NumberOfSections;i++)
				{
					TCHAR szNote[64] = {0};
					StringCchPrintf(
						szNote,
						64,
						_T("RA:%08X RS:%08X %s:%08X"),
						pSection[i].PointerToRawData,
						pSection[i].SizeOfRawData,
						_T("属性"),
						pSection[i].Characteristics);
					int nType1 = 0;
					int nType2 = 0;
					int nType3 = 0;

					if ( i == 0)
					{
						nType1 = FUCTION_BODY_HEADER_FLAGS;
						nType3 = FUNCTION_HREADER_FLAGS;
					}
					else if ( i == (pNtHeader->FileHeader.NumberOfSections - 1))
					{
						nType1 = FUCTION_RET_FLAGS;
						nType3 = FUNCTION_HREADER_FLAGS;
					}
					else
					{
						nType1 = FUNCTION_BODY_FLAGS;
					}

					if ( pSection[i].VirtualAddress <= ImageImportRva &&
						ImageImportRva <= pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
					{
						ImageImportRva = ( ImageImportRva - pSection[i].VirtualAddress ) + pSection[i].PointerToRawData;
					}

					if ( pSection[i].VirtualAddress <= ImageRelocationRva &&
						ImageRelocationRva <= pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
					{
						ImageRelocationRva = ( ImageRelocationRva - pSection[i].VirtualAddress ) + pSection[i].PointerToRawData;
					}


					m_WinExtFile[nI++].ShowSectionInfo(
						nAddress,
						_T(".Section"),
						pSection[i].VirtualAddress,
						pSection[i].Misc.VirtualSize,
						(const char*)pSection[i].Name,
						SECTION_NAME_SHOW_COLOR,
						szNote,
						nType1,
						nType2,
						nType3,
						IsAmd64);
				}


			}
		}
	}

	return FALSE;
}