/* File custom.c
 * (custom control implementation)
 */
#include "stdafx.h"
#include "custom.h"
#include "wheel.h"
#include "global.h"
#include <strsafe.h>

// Data of the custom control.
typedef struct CustomData {
	//CCustomView 
    HWND hwnd;
    HFONT hFont;
    int ColMin;
    int ColMax;
    int RowMin;
    int RowMax;
} CustomData;



CCustomHeader::CCustomHeader()
{
	m_nColumn = 0;
	m_nHeigth = HEADER_CY;
	ZeroMemory( 
			m_HeaderData,
			CUSTOM_MAX_COLUMN * sizeof(CustomHeaderData));
}

CCustomHeader::~CCustomHeader()
{
	
}

BOOL CCustomHeader::InsertColumn( 
							int nColumn,
							TCHAR* szText,
							int nWideth )
{
	if ( szText && nColumn >= 0 && nColumn < CUSTOM_MAX_COLUMN )
	{
		int Length = _tcslen(szText);

		//if (Length <= COLUMN__HEADER_MAX_TEXT_LENGTH)
		{
			m_nColumn++;
			_tcsncpy_s(
					m_HeaderData[nColumn].szText,
					szText,
					COLUMN__HEADER_MAX_TEXT_LENGTH - sizeof(TCHAR));
		}

		m_HeaderData[nColumn].nWideth = nWideth;

		return TRUE;
	}

	return FALSE;
}

ULONG CCustomHeader::GetColumnWidth( int nColumn )
{
	if ( nColumn >= 0 && nColumn < m_nColumn )
	{
		return m_HeaderData[nColumn].nWideth;
	}

	return 0;
}

LPWSTR CCustomHeader::GetColumnText( int nColumn )
{
	if ( nColumn >= 0 && nColumn < m_nColumn )
	{
		return m_HeaderData[nColumn].szText;
	}

	return _T("");
}

ULONG CCustomHeader::GetColumnHeight()
{
	return m_nHeigth;
}

ULONG CCustomHeader::GetMaxColumn()
{
	return m_nColumn;
}

void CCustomHeader::SetHeight( int Height )
{
	m_nHeigth = Height;
}

void CCustomHeader::SetColumnWidth( int nColumn , int nWidth )
{
	if ( nColumn >= 0 && nColumn < m_nColumn )
	{
		m_HeaderData[nColumn].nWideth = nWidth;
	}
}

CustomItemHead::CustomItemHead()
	:ModifyItemHead(NULL)
    ,tagNext(NULL)
	,tagPre(NULL)
	,tagJmpTo(NULL)
	,lpAddress(NULL)
	,InstructionType(0)
	,ptrAddress(0)
	,ColumnCount(0)
	,Scaler(0)
{
	ZeroMemory(this,sizeof(*this));
}

CustomItemHead::~CustomItemHead()
{
	if (lpAddress)
	{
		delete[] lpAddress;
		lpAddress = NULL;
	}
}

CustomItemHead* CustomItemHead::Next()
{
	return tagNext;
}

CustomItemHead* CustomItemHead::Pre()
{
	return tagPre;
}

uint64_t CustomItemHead::GetPtrValue()
{
	return ptrAddress;
}

uint64_t CustomItemHead::GetScaler()
{
	return Scaler;
}


void CustomItemHead::InsertAddressColumn(
	int IconNums,
	LPTSTR lpText,
	HBRUSH hBgkBrush,
	HBRUSH hBgkSelect )
{

	ColumnData[0].SelBgkBrush = hBgkSelect;
	ColumnData[0].NorBgkBrush = hBgkBrush;

	ColumnData[0].Icon = new tagIcon[IconNums];
	if (ColumnData[0].Icon)
	{
		memset(ColumnData[0].Icon,0,IconNums*sizeof(tagIcon));
		ColumnData[0].IconCount = IconNums;
		for (int i = 0;i < IconNums ;i++)
		{
			ColumnData[0].Icon[i].nIcon = 0;
			ColumnData[0].Icon[i].colorIcon = NORMAL_TEXT_COLOR;

		}
	}

	ColumnData[0].Option = new tagOption[1];
	if (ColumnData[0].Option)
	{
		memset(ColumnData[0].Option,0,sizeof(tagOption));
		ColumnData[0].OptionCount = 1;
		ColumnData[0].Option->NorFontColor = DATA_ADDRESS_SELECT_COLOR;
		ColumnData[0].Option->SelFontColor = DATA_ADDRESS_SELECT_COLOR;
		ColumnData[0].Option->HasNonFocus  = DATA_ADDRESS_COLOR;
		ColumnData[0].Option->FontBgColor  = -1;

		if (lpText)
		{
			int Length = lstrlen(lpText);

			if (Length)
			{
				ColumnData[0].Option->TextSize = Length;
				ColumnData[0].Option->lpText = new TCHAR[Length + 1];
				if (ColumnData[0].Option->lpText)
				{
					StringCchCopy( 
						ColumnData[0].Option->lpText,
						Length + 1,
						lpText);
				}
			}
		}
	}	
}

void    CustomItemHead::SetAddressColumnIcon(
	int IconNums,
	int nIconType,
	int nIconType2,
	int nIconType3 )
{
	if( this )
	{
		if (this && IconNums < ColumnData[0].IconCount )
		{
// 			for ( int i = 0;
// 				i < ColumnData[0].IconCount ;
// 				i++)
			{
				ColumnData[0].Icon[IconNums].nIcon = nIconType;
				ColumnData[0].Icon[IconNums].colorIcon = nIconType2;
			}
		}
	}
}

int  CustomItemHead::GetetAddressColumnIcon(int IconNums)
{
	if (this && IconNums < ColumnData[0].IconCount )
	{
		return ColumnData[0].Icon[IconNums].nIcon;
	}

	return 0;
}

void CustomItemHead::SetColumnItem(
						int			nColumn,
						int			IconNums,
						tagOption* Option,
						int			nSize,
						HBRUSH hBgkBrush,
						HBRUSH hBgkSelect )
{
	if( this && nColumn < (CUSTOM_MAX_COLUMN - 2))
	{
		if (this)
		{
			ColumnData[nColumn-1].SelBgkBrush = hBgkSelect;
			ColumnData[nColumn-1].NorBgkBrush = hBgkBrush;

			if (IconNums)
			{
				ColumnData[nColumn-1].Icon = new tagIcon[IconNums];
				if (ColumnData[nColumn-1].Icon)
				{
					memset(ColumnData[nColumn-1].Icon,0,IconNums*sizeof(tagIcon));
					ColumnData[nColumn-1].IconCount = IconNums;
					for (int i = 0;i < IconNums ;i++)
					{
						ColumnData[nColumn-1].Icon[i].nIcon = 0;
						ColumnData[nColumn-1].Icon[i].colorIcon = NORMAL_TEXT_COLOR;

					}
				}
			}			


			if (nSize)
			{
				ColumnData[nColumn-1].Option = Option;
				ColumnData[nColumn-1].OptionCount = nSize;
				// 				if (m_ItemHead[nItem]->ColumnData[nColumn-1].Option)
				// 				{
				// 					memset(m_ItemHead[nItem]->ColumnData[nColumn-1].Option,0,nSize*sizeof(tagOption));
				// 
				// 					for (int k = 0; k < nSize;k++)
				// 					{
				// 						m_ItemHead[nItem]->ColumnData[nColumn-1].Option->NorFontColor = Option[k].NorFontColor;
				// 						m_ItemHead[nItem]->ColumnData[nColumn-1].Option->SelFontColor = Option[k].SelFontColor;
				// 						m_ItemHead[nItem]->ColumnData[nColumn-1].Option->HasNonFocus  = Option[k].HasNonFocus;
				// 						if (Option[k].lpText)
				// 						{
				// 							int Length = lstrlen(Option[k].lpText);
				// 
				// 							if (Length)
				// 							{
				// 								m_ItemHead[nItem]->ColumnData[nColumn-1].Option->lpText = new TCHAR[Length + 1];
				// 								if (m_ItemHead[nItem]->ColumnData[nColumn-1].Option->lpText)
				// 								{
				// 									StringCchCopy( 
				// 										m_ItemHead[nItem]->ColumnData[nColumn-1].Option->lpText,
				// 										Length + 1,
				// 										Option[k].lpText);
				// 								}
				// 							}
				// 						}
				// 					}		

				//}
			}
		}
	}
}

void CustomItemHead::SetInstructionType( uint8_t InsType)
{
	InstructionType = InsType;
}

uint8_t CustomItemHead::GetInstructionType()
{
	return InstructionType;
}

void CustomItemHead::SetItemText( int nColumn, LPTSTR szText )
{
	if( this && 
		nColumn < CUSTOM_MAX_COLUMN )
	{
		if ( this)
		{
			if (nColumn < ColumnCount)
			{
			}			
		}		
	}
}

CCustomItem::CCustomItem()
{
	m_nMaxItem = 0;
	m_nItem = 0;
	m_nHeight = CELL_CY;
			
	m_ItemHead = NULL;
}

CCustomItem::~CCustomItem()
{
	Destroy();
}

void CCustomItem::InsertItem( 
						int nItem,
						uint64_t u64ptr,
						LPTSTR lpText,
						HBRUSH hBgkBrush,
						HBRUSH hBgkSelect )
{
	if( m_ItemHead &&
		m_nMaxItem && 
		m_nItem >= nItem &&
		m_nItem < m_nMaxItem )
	{
		if ( m_ItemHead[nItem])
		{
			delete m_ItemHead[nItem];
			m_ItemHead[nItem] = NULL;
		}

		m_ItemHead[nItem] = new CustomItemHead();
		if (m_ItemHead[nItem])
		{
			memset(m_ItemHead[nItem],0,sizeof(CustomItemHead));
			
			if (lpText)
			{
				int Length = lstrlen(lpText);
				m_ItemHead[nItem]->lpAddress = new TCHAR[Length + 1];
				if (m_ItemHead[nItem]->lpAddress)
				{
					m_ItemHead[nItem]->NorFontColor = DATA_ADDRESS_COLOR;
					m_ItemHead[nItem]->NoneFocusFontColor = DATA_ADDRESS_COLOR;
					m_ItemHead[nItem]->SelFontColor = DATA_ADDRESS_SELECT_COLOR;
					m_ItemHead[nItem]->SelBgkBrush = hBgkSelect;
					m_ItemHead[nItem]->NorBgkBrush  = hBgkBrush;
					m_ItemHead[nItem]->ptrAddress = u64ptr;
					StringCchCopy(
							m_ItemHead[nItem]->lpAddress,
							Length + 1,
							lpText
								);
				}
			}

			m_nItem++;
		}
	}
}

void CCustomItem::InsertAddressColumn( 
								int nItem,
								int IconNums,
								LPTSTR lpText,
								HBRUSH hBgkBrush,
								HBRUSH hBgkSelect )
{
	if( m_ItemHead &&
		m_nMaxItem && 
		m_nItem >= nItem &&
		m_nItem < m_nMaxItem )
	{
		if (m_ItemHead[nItem])
		{
			m_ItemHead[nItem]->ColumnData[0].SelBgkBrush = hBgkSelect;
			m_ItemHead[nItem]->ColumnData[0].NorBgkBrush = hBgkBrush;

			m_ItemHead[nItem]->ColumnData[0].Icon = new tagIcon[IconNums];
			if (m_ItemHead[nItem]->ColumnData[0].Icon)
			{
				memset(m_ItemHead[nItem]->ColumnData[0].Icon,0,IconNums*sizeof(tagIcon));
				m_ItemHead[nItem]->ColumnData[0].IconCount = IconNums;
				for (int i = 0;i < IconNums ;i++)
				{
					m_ItemHead[nItem]->ColumnData[0].Icon[i].nIcon = 0;
					m_ItemHead[nItem]->ColumnData[0].Icon[i].colorIcon = NORMAL_TEXT_COLOR;
					
				}
			}

			m_ItemHead[nItem]->ColumnData[0].Option = new tagOption[1];
			if (m_ItemHead[nItem]->ColumnData[0].Option)
			{
				memset(m_ItemHead[nItem]->ColumnData[0].Option,0,sizeof(tagOption));
				m_ItemHead[nItem]->ColumnData[0].OptionCount = 1;
				m_ItemHead[nItem]->ColumnData[0].Option->NorFontColor = DATA_ADDRESS_SELECT_COLOR;
				m_ItemHead[nItem]->ColumnData[0].Option->SelFontColor = DATA_ADDRESS_SELECT_COLOR;
				m_ItemHead[nItem]->ColumnData[0].Option->HasNonFocus  = DATA_ADDRESS_COLOR;
				m_ItemHead[nItem]->ColumnData[0].Option->FontBgColor  = -1;
				
				if (lpText)
				{
					int Length = lstrlen(lpText);

					if (Length)
					{
						m_ItemHead[nItem]->ColumnData[0].Option->TextSize = Length;
						m_ItemHead[nItem]->ColumnData[0].Option->lpText = new TCHAR[Length + 1];
						if (m_ItemHead[nItem]->ColumnData[0].Option->lpText)
						{
							StringCchCopy( 
									m_ItemHead[nItem]->ColumnData[0].Option->lpText,
									Length + 1,
									lpText);
						}
					}
				}

			}
			
		}
	}
}

void    CCustomItem::SetAddressColumnIcon(int nItem,int IconNums,int nIconType,int nIconType2,int nIconType3 )
{
	if( m_ItemHead &&
		m_nMaxItem && 
		m_nItem >= nItem &&
		m_nItem < m_nMaxItem )
	{
		if (m_ItemHead[nItem] && IconNums < m_ItemHead[nItem]->ColumnData[0].IconCount )
		{
			for ( int i = 0;
				i < m_ItemHead[nItem]->ColumnData[0].IconCount ;
				i++)
			{
				m_ItemHead[nItem]->ColumnData[0].Icon[IconNums].nIcon = nIconType;
				m_ItemHead[nItem]->ColumnData[0].Icon[IconNums].colorIcon = nIconType2;
			}
		}
	}
}

void  CCustomItem::SetColumnItem(
							int nItem,
							int nColumn,
							int IconNums,
							tagOption* Option,
							int nSize,
							HBRUSH hBgkBrush,
							HBRUSH hBgkSelect )
{
	if( m_ItemHead &&
		m_nMaxItem && 
		m_nItem >= nItem &&
		m_nItem < m_nMaxItem &&
		nColumn < (CUSTOM_MAX_COLUMN - 2))
	{
		if (m_ItemHead[nItem])
		{
			m_ItemHead[nItem]->ColumnData[nColumn-1].SelBgkBrush = hBgkSelect;
			m_ItemHead[nItem]->ColumnData[nColumn-1].NorBgkBrush = hBgkBrush;

			if (IconNums)
			{
				m_ItemHead[nItem]->ColumnData[nColumn-1].Icon = new tagIcon[IconNums];
				if (m_ItemHead[nItem]->ColumnData[nColumn-1].Icon)
				{
					memset(m_ItemHead[nItem]->ColumnData[nColumn-1].Icon,0,IconNums*sizeof(tagIcon));
					m_ItemHead[nItem]->ColumnData[nColumn-1].IconCount = IconNums;
					for (int i = 0;i < IconNums ;i++)
					{
						m_ItemHead[nItem]->ColumnData[nColumn-1].Icon[i].nIcon = 0;
						m_ItemHead[nItem]->ColumnData[nColumn-1].Icon[i].colorIcon = NORMAL_TEXT_COLOR;
						
					}
				}
			}			


			if (nSize)
			{
				m_ItemHead[nItem]->ColumnData[nColumn-1].Option = Option;
				m_ItemHead[nItem]->ColumnData[nColumn-1].OptionCount = nSize;
// 				if (m_ItemHead[nItem]->ColumnData[nColumn-1].Option)
// 				{
// 					memset(m_ItemHead[nItem]->ColumnData[nColumn-1].Option,0,nSize*sizeof(tagOption));
// 
// 					for (int k = 0; k < nSize;k++)
// 					{
// 						m_ItemHead[nItem]->ColumnData[nColumn-1].Option->NorFontColor = Option[k].NorFontColor;
// 						m_ItemHead[nItem]->ColumnData[nColumn-1].Option->SelFontColor = Option[k].SelFontColor;
// 						m_ItemHead[nItem]->ColumnData[nColumn-1].Option->HasNonFocus  = Option[k].HasNonFocus;
// 						if (Option[k].lpText)
// 						{
// 							int Length = lstrlen(Option[k].lpText);
// 
// 							if (Length)
// 							{
// 								m_ItemHead[nItem]->ColumnData[nColumn-1].Option->lpText = new TCHAR[Length + 1];
// 								if (m_ItemHead[nItem]->ColumnData[nColumn-1].Option->lpText)
// 								{
// 									StringCchCopy( 
// 										m_ItemHead[nItem]->ColumnData[nColumn-1].Option->lpText,
// 										Length + 1,
// 										Option[k].lpText);
// 								}
// 							}
// 						}
// 					}		

				//}
			}
		}
	}
}

void CCustomItem::SetItemText( int nItem , int nColumn, LPWSTR szText )
{
	if( m_ItemHead &&
		m_nMaxItem && 
		m_nItem <= m_nMaxItem && 
		nColumn < CUSTOM_MAX_COLUMN )
	{
		if ( m_ItemHead[nItem])
		{
			if (nColumn < m_ItemHead[nItem]->ColumnCount)
			{
			}			
		}		
	}
}

void CCustomItem::SetItemHeight( int nHeight )
{
	m_nHeight = nHeight;
}

int	CCustomItem::GetItemHeigth()
{
	return m_nHeight;
}

int	CCustomItem::GetItemCount()
{
	return m_nItem;
}

LPWSTR  CCustomItem::GetItemText( int nItem , int nColumn)
{
	if ( nItem < m_nItem && nColumn < CUSTOM_MAX_COLUMN)
	{
		//return m_ItemData[nColumn][nItem]->lpText;
	}

	return _T("");
}

void CCustomItem::Destroy()
{
	for ( int i = 0; i < m_nItem;i++)
	{
		DetroyItemHeader(m_ItemHead[i]);
		delete m_ItemHead[i];
		m_ItemHead[i] = NULL;
	}

	if (m_nItem)
	{
		if (m_ItemHead)
		{
			delete[] m_ItemHead;
			m_ItemHead = NULL;
		}
	}
	
}

void CCustomItem::DetroyItemData(CustomItemData* ItemData)
{
	for ( int i = 0;i < CUSTOM_MAX_COLUMN;i++)
	{
		if(ItemData[i].Option && ItemData[i].OptionCount)
		{
			for(int j = 0; j < ItemData[i].OptionCount;j++)
			{
				if (ItemData[i].Option[j].lpText)
				{
					delete[] ItemData[i].Option[j].lpText;
					ItemData[i].Option[j].lpText = NULL;
				}
			}

			delete[] ItemData[i].Option;			
		}

		if (ItemData[i].Icon)
		{
			delete[] ItemData[i].Icon;
		}

		if (ItemData[i].lpModifyText)
		{
			delete[] ItemData[i].lpModifyText;
		}
	}
}

void CCustomItem::DetroyItemHeader(PCustomItemHead ItemHeader)
{
	if (ItemHeader)
	{
		if (ItemHeader->lpAddress)
		{
			delete[] ItemHeader->lpAddress;
			ItemHeader->lpAddress = NULL;
		}

		DetroyItemData(ItemHeader->ColumnData);

		if (ItemHeader->ModifyItemHead)
		{
			DetroyItemHeader(ItemHeader->ModifyItemHead);
			delete ItemHeader->ModifyItemHead;
			ItemHeader->ModifyItemHead = NULL;
		}
	}
}

void CCustomItem::SetMaxItemNums( int nMaxItem)
{
	m_nMaxItem = nMaxItem;	
	m_ItemHead = new PCustomItemHead[m_nMaxItem];
	memset( 
			m_ItemHead, 
			0, 
			m_nMaxItem *sizeof(PCustomItemHead));
}

CSrceenView::CSrceenView()
	:mViewItemCapacity(0)
	,mSelectItemMin(NULL)
	,mSelectItemMax(NULL)
	,m_ptrFirstElem(NULL)
	,m_ptrLastElem(NULL)
	,mItemHead(NULL)
	,m_ptrJmpElem(NULL)
	,m_cbElemSize(0)
	,mViewItemSize(0)
	,m_ViewItemHeight(CELL_CY)
	,m_IsSingleSel(FALSE)
	
{
	mItemHead = new PCustomItemHead[CUSTOM_SCREEN_VIEW_SIZE];
	if (mItemHead)
	{
		ZeroMemory(mItemHead,CUSTOM_SCREEN_VIEW_SIZE*sizeof(PCustomItemHead));
		mViewItemCapacity = CUSTOM_SCREEN_VIEW_SIZE;
	}
}

CSrceenView::~CSrceenView()
{

}

PCustomItemHead CSrceenView::InsertItem( uint64_t u64ptr, LPTSTR lpText,HBRUSH hBgkBrush,HBRUSH hBgkSelect )
{
	if ( !m_ptrLastElem)
	{
		m_ptrLastElem = new CustomItemHead();

		if ( m_ptrFirstElem == NULL )
		{
			m_ptrFirstElem = m_ptrLastElem;
		}
	}
	else
	{
		m_ptrLastElem->tagNext = new CustomItemHead();

		if ( m_ptrLastElem->tagNext )
		{
			PCustomItemHead pTemp = m_ptrLastElem->tagNext;

			pTemp->tagPre = m_ptrLastElem;
			m_ptrLastElem = pTemp;
		}
	}

	if (m_ptrLastElem)
	{
		if (lpText)
		{
			int Length = lstrlen(lpText);
			m_ptrLastElem->lpAddress				= new TCHAR[Length + 1];
			if (m_ptrLastElem->lpAddress)
			{
				m_ptrLastElem->NorFontColor			= DATA_ADDRESS_COLOR;
				m_ptrLastElem->NoneFocusFontColor	= DATA_ADDRESS_COLOR;
				m_ptrLastElem->SelFontColor			= DATA_ADDRESS_SELECT_COLOR;
				m_ptrLastElem->SelBgkBrush			= hBgkSelect;
				m_ptrLastElem->NorBgkBrush			= hBgkBrush;
				m_ptrLastElem->ptrAddress			= u64ptr;
				m_ptrLastElem->Scaler				= m_cbElemSize;
				StringCchCopy(
					m_ptrLastElem->lpAddress,
					Length + 1,
					lpText
					);
			}
		}

		if (mViewItemSize < mViewItemCapacity)
		{
			mItemHead[mViewItemSize++] = m_ptrLastElem;
		}

		m_cbElemSize++;

		return m_ptrLastElem;
	}

	return NULL;
}

void CSrceenView::InsertAddressColumn( 
						PCustomItemHead pItem,
						int IconNums,
						LPTSTR lpText,
						HBRUSH hBgkBrush,
						HBRUSH hBgkSelect )
{
	if (pItem)
	{
		pItem->InsertAddressColumn(IconNums,lpText,hBgkBrush,hBgkSelect);
	}
}

void    CSrceenView::SetAddressColumnIcon(
									PCustomItemHead pItem,
									int IconNums,
									int nIconType,
									int nIconType2,
									int nIconType3 )
{
	if( pItem )
	{
		pItem->SetAddressColumnIcon(IconNums,nIconType,nIconType2,nIconType3);
	}
}

void    CSrceenView::SetColumnItem(
						PCustomItemHead pItem,
						int			nColumn,
						int			IconNums,
						tagOption* Option,
						int			nSize,
						HBRUSH hBgkBrush,
						HBRUSH hBgkSelect )
{
	if( pItem && nColumn < (CUSTOM_MAX_COLUMN - 2))
	{
		pItem->SetColumnItem(nColumn,IconNums,Option,nSize,hBgkBrush,hBgkSelect);
	}
}

void CSrceenView::SetItemText( PCustomItemHead pItem, int nColumn, LPTSTR szText )
{
	if( pItem && 
		nColumn < CUSTOM_MAX_COLUMN )
	{
		pItem->SetItemText(nColumn,szText);
	}
}

void CSrceenView::SetItemHeight( int nHeight )
{
	m_ViewItemHeight = nHeight;
}

void CSrceenView::MoveItem( int MoveNums,int PageSize,BOOL IsUp)
{
	if ( MoveNums)
	{
		PCustomItemHead pOldLastItem =  0;
		PCustomItemHead pOldLastItem_2 =  0;
		if (IsUp)
		{
			pOldLastItem = mItemHead[0];
			
			for ( int i = 0;i < MoveNums ; i++)
			{
				if (pOldLastItem)
				{
					pOldLastItem = pOldLastItem->tagPre;
				}
				else
				{
					pOldLastItem = m_ptrFirstElem;
					break;
				}
			}

			for ( int i = 0 ; i <= PageSize; i++ )
			{
				mItemHead[i] = pOldLastItem;

				if (pOldLastItem)
				{
					pOldLastItem = pOldLastItem->tagNext;
				}

			}
		}
		else
		{
			pOldLastItem = mItemHead[0];
 			for ( int i = 0;i < MoveNums ; i++ )
 			{
 				if (pOldLastItem && pOldLastItem->tagNext)
 				{
					pOldLastItem = pOldLastItem->tagNext;
 				}
				else
				{
					break;
				}
 			}
 			
			if (pOldLastItem)
			{
				for ( int i = 0 ; i<= (PageSize + 1) ;i++)
				{		
					if (pOldLastItem)
					{
						mItemHead[i] = pOldLastItem;
						pOldLastItem = pOldLastItem->tagNext;
					}
					else
					{
						mItemHead[i] = 0;
					}
					
				}
			}
			else
			{
				for ( int i = 0 ; i<= PageSize ;i++)
				{					
					mItemHead[i] = 0;
				}
			}			
		}
	}
}

int	CSrceenView::GetItemHeigth()
{
	return m_ViewItemHeight;
}

int	 CSrceenView::GetItemCount()
{
	return m_cbElemSize;
}

CCustomView::CCustomView(void)
	:m_hWnd(0)
	,m_ColMin(0)
	,m_ColMax(CUSTOM_MAX_COLUMN)
	,m_rowMin(0)
	,m_RowMax(0)
	,m_bFocus(FALSE)
	,m_bTrackLine(-1)
	,m_hPaintFont(0)
	,m_bLButtonDown(FALSE)
	,m_PerPageScroll(PER_SCROLL_PAGE_SIZE)
	,m_PreScrollPos(0)
	,m_ViewNameFlags(-1)
	,m_ParentSelectView(-1)
	,m_FontWe(0)
	,m_FontHe(0)
	,m_bShowBoder(FALSE)
{
	m_ptTrackPos.x = 0;
	m_ptTrackPos.y = 0;
	m_hWhitePen    = NULL;;
	m_hLinePen = NULL;
	
	m_hWhiteHeaderBrush	= NULL;
	m_hHeaderBrush		= NULL;

	m_hWhitePen = CreatePen(PS_SOLID,1,NORMAL_WHITE_COLOR);
	m_hLinePen = CreatePen(PS_SOLID,1,NORMAL_LINE_COLOR);

	m_hWhiteHeaderBrush	= CreateSolidBrush(NORMAL_WHITE_COLOR);
	m_hHeaderBrush		= CreateSolidBrush(HEADER_BACK_COLOR);

	m_hNormalBrushA		= CreateSolidBrush(NORMAL_BACK_COLOR_A);
	m_hNormalBrushB		= CreateSolidBrush(NORMAL_BACK_COLOR_B);

	m_hNewPen			= CreatePen(PS_SOLID,1,LINE_COLUMN_GAP);
	m_hLineBrush		= CreateSolidBrush(NORMAL_LINE_COLOR);
	m_hLineFrameBrush   = CreateSolidBrush(LINE_COLUMN_FRAME);
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

	m_hPaintFont		= CreateFont(
										0 - SCROLL_STEP,
										0 - SCROLL_STEP/2,
										0,
										0,
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

	
// 	LOGFONT logFont;
// 	GetObject(m_hPaintFont,sizeof(logFont),&logFont);
	m_FontWe = SCROLL_STEP/2;
	m_FontHe = SCROLL_STEP;
	ZeroMemory(&m_ViewPos,sizeof(CustomViewPos));
	ZeroMemory(&m_ViewPags,sizeof(CustomViewPage));
}

CCustomView::~CCustomView()
{	
	//m_CustomItem.Destroy();	
}

void
CCustomView::CustomSetScrollbarRange(
							 int scrollbarId,
							 int indexMin, 
							 int indexMax)
{
	SCROLLINFO si = {0};

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE;
	si.nMin = 0;    
    si.nMax = ( indexMax + m_ViewPags.PerPagesSize) * m_CustomItem.GetItemHeigth() + m_CustomHeader.GetColumnHeight();
    SetScrollInfo(m_hWnd, scrollbarId, &si, TRUE);
}

// Handle XXM_SETCOLRANGE:
BOOL
CCustomView::CustomSetColRange(
					int ColMin,
					int ColMax)
{
    if (ColMin >= ColMax)
        return FALSE;

    m_ColMin = ColMin;
    m_ColMax = ColMax;

    CustomSetScrollbarRange(SB_HORZ, ColMin, ColMax);
    ::InvalidateRect(m_hWnd, NULL, TRUE);

    return TRUE;
}

// Handle XXM_SETROWRANGE:
BOOL CCustomView::CustomSetRowRange(int RowMin, int RowMax)
{
    if (RowMin >= RowMax)
        return FALSE;

    m_rowMin = RowMin;
    m_RowMax = RowMax;

    CustomSetScrollbarRange(
						SB_VERT,
						RowMin,
						RowMax);

    ::InvalidateRect(m_hWnd, NULL, TRUE);

    return TRUE;
}

// Handle WM_SIZE:
void
CCustomView::CustomSize(
				UINT uWidth, 
				UINT uHeight)
{
  //InvalidateRect(m_hWnd,NULL,TRUE);
  m_ViewPags.PageRect.left = 0;
  m_ViewPags.PageRect.right = uWidth;
  m_ViewPags.PageRect.top = m_CustomHeader.GetColumnHeight();
  m_ViewPags.PageRect.bottom = uHeight - m_ViewPags.PageRect.top;

  m_ViewPags.PerPagesSize = ((m_ViewPags.PageRect.bottom - \
								m_ViewPags.PageRect.top)/m_CustomItem.GetItemHeigth() +\
								1);
  if (m_ViewPags.PerPagesSize)
  {
	  m_ViewPags.TotlePagesNums = m_CustomItem.GetItemCount()/m_ViewPags.PerPagesSize + 1;
  }

  PCustomItemHead pFirstItem = m_ScreenView.GetItem(0);
  for ( int i = 0 ; i <= m_ViewPags.PerPagesSize;i++)
  {
	  m_ScreenView.SetItem(i,pFirstItem);

	  if (pFirstItem)
	  {
		  pFirstItem = pFirstItem->tagNext;
	  }
  }
  
}

void
OnCustomPaintCell(HDC hDC, const TCHAR* lpszText, RECT* lpRect, HBRUSH hBackBrush)
{
    FillRect(hDC, lpRect, hBackBrush);

    if (lpszText != NULL)
        DrawText(
				hDC,
				lpszText, 
				-1,
				lpRect,
				DT_EXPANDTABS | 
				DT_VCENTER |
				DT_SINGLELINE);
}

static void
CustomSetClip(HDC hDC, RECT* prcClip)
{
    HRGN hrgnClip;
    
    hrgnClip = CreateRectRgn(
						prcClip->left,
						prcClip->top,
						prcClip->right,
						prcClip->bottom);

    SelectClipRgn(hDC, hrgnClip);
    DeleteObject(hrgnClip);
}

void CCustomView::OnCustomPaintRowHeader()
{
	//     if (prcDirty->left < CELL_CX) {
	//         RECT rcClip = { 0, HEADER_CY, HEADER_CX, rcDirtyBody.bottom };
	//         CustomSetClip(hDC, &rcClip);
	// 
	//         for (row = RowMin; row <= RowMax; row++) {
	//             RECT rc;
	//             TCHAR pszBuffer[32];
	// 
	//             rc.left = 0;
	//             rc.top = HEADER_CY + row * CELL_CY - PosVert;
	//             rc.right = HEADER_CX;
	//             rc.bottom = rc.top + CELL_CY;
	//             _sntprintf_s(pszBuffer, 32, _TRUNCATE, _T("Row %d"), row);
	//             //OnCustomPaintCell(hDC, pszBuffer, &rc, hHeaderBrush);
	// 			OnCustomPaintCell(hDC, NULL, &rc, hHeaderBrush);
	//         }
	//     }

}

void CCustomView::OnCustomPaintHeader(
							HDC PaintDC,
							RECT* prcDirty,
							RECT* rcDirtyBody,
							int ColMin,
							int ColMax,
							int RowMin,
							int RowMax,
							int PosHorz,
							int PosVert
							)
{
	
	int Col;
	
	if (prcDirty->top <= m_CustomHeader.GetColumnHeight()) 
	{
		RECT rcClip = { HEADER_CX, 0, rcDirtyBody->right, m_CustomHeader.GetColumnHeight() };
		CustomSetClip(PaintDC, &rcClip);

		RECT rcColumn;
		CopyMemory(
				&rcColumn,
				&rcClip,
				sizeof(RECT));

		int PrePosX = 0;

		for (Col = ColMin; Col < ColMax ; Col++)
		{
			RECT rcViewHeader;
			TCHAR pszBuffer[32];

			rcViewHeader.left = HEADER_CX + PrePosX - PosHorz;
			rcViewHeader.top = 0;
			rcViewHeader.bottom = m_CustomHeader.GetColumnHeight();
			if ( Col == ColMax - 1 )
			{
				rcViewHeader.right = rcDirtyBody->right;
			}
			else
			{
				rcViewHeader.right = rcViewHeader.left + m_CustomHeader.GetColumnWidth(Col);
			}			
				
				//_sntprintf_s(pszBuffer, 32, _TRUNCATE, _T("Column %d"), col);
			OnCustomPaintCell(
								PaintDC,
								m_CustomHeader.GetColumnText(Col),
								&rcViewHeader,
								m_hHeaderBrush);						
			
			if (Col)
			{
				SelectObject(PaintDC,m_hLinePen);
				::MoveToEx(PaintDC,rcViewHeader.left, rcViewHeader.top, 0);
				::LineTo(PaintDC, rcViewHeader.left, rcViewHeader.bottom);
			}

			SelectObject(PaintDC,m_hWhitePen);
			::MoveToEx(PaintDC,rcViewHeader.left+1, rcViewHeader.bottom - 1, 0);
			::LineTo(PaintDC, rcViewHeader.left + 1, rcViewHeader.top);
			::LineTo(PaintDC, rcViewHeader.right, rcViewHeader.top);

			PrePosX += (rcViewHeader.right - rcViewHeader.left);

		}

		SelectObject(PaintDC,m_hLinePen);
// 		if(IsShowBorder())
// 		{
// 			::MoveToEx(PaintDC,rcColumn.left, rcColumn.top, 0);
// 			::LineTo(PaintDC, rcColumn.right, rcColumn.top);
// 		}

		::MoveToEx(PaintDC,rcColumn.left, rcColumn.bottom - 1, 0);
		::LineTo(PaintDC, rcColumn.right, rcColumn.bottom - 1);
		
		//::MoveToEx(PaintDC,rcColumn.right - 1, rcColumn.top, 0);
		//::LineTo(PaintDC, rcColumn.right - 1, rcColumn.bottom);
	}

}

void CCustomView::OnCustomPaintBody(
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
	SetTextColor(PaintDC, NORMAL_TEXT_COLOR);
	SetBkMode(PaintDC, TRANSPARENT);

	for (int Row = RowMin; Row <= RowMax; Row++) 
	{
		RECT rcBody;

		rcBody.top = HeaderHeight + Row * ItemHeight - PosVert;
		rcBody.bottom = rcBody.top + ItemHeight;

		int PrePosX = 0;

		for (int Col = ColMin; Col < ColMax; Col++) {
			
			rcBody.left = HEADER_CX + PrePosX - PosHorz;

			if (rcBody.left > prcDirty->right)
			{
				break;
			}

			rcBody.right = rcBody.left + m_CustomHeader.GetColumnWidth(Col); 

			if (rcBody.right > prcDirty->right || Col == ColMax - 1)
			{
				rcBody.right = prcDirty->right;
			}

			if ( Row < m_RowMax )
			{
				if (m_ViewPags.SelectMinPageItem == Row )
				{
					OnCustomPaintCell(
						PaintDC, 
						m_CustomItem.GetItemText(Row,Col),
						&rcBody, 
						m_hNormalBrushB);
				}
				else
				{
					if ( m_ViewPags.SelectMaxPageItem >= 0 &&
						((m_ViewPags.SelectMaxPageItem > m_ViewPags.SelectMinPageItem && 
						Row > m_ViewPags.SelectMinPageItem && Row <= m_ViewPags.SelectMaxPageItem) ||
						( m_ViewPags.SelectMaxPageItem < m_ViewPags.SelectMinPageItem &&
						Row >= m_ViewPags.SelectMaxPageItem && Row < m_ViewPags.SelectMinPageItem)))
					{

						OnCustomPaintCell(
							PaintDC, 
							m_CustomItem.GetItemText(Row,Col),
							&rcBody, 
							m_hNormalBrushB);
					}
					else
					{
						OnCustomPaintCell(
							PaintDC, 
							m_CustomItem.GetItemText(Row,Col),
							&rcBody, 
							m_hNormalBrushA);
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

// 			if (IsShowBorder())
// 			{
// 				if (Col >= 0)
// 				{
// 					SelectObject(PaintDC, m_hNewPen);
// 					::MoveToEx(	PaintDC,rcBody.left, rcBody.top, 0);
// 					::LineTo(	PaintDC, rcBody.left,  rcBody.bottom);
// 				}
// 
// 				if (Col == (ColMax - 1))
// 				{
// 					//SelectObject(PaintDC, m_hNewPen);
// 					::MoveToEx(	PaintDC,rcBody.right -1, prcDirty->top, 0);
// 					::LineTo(	PaintDC, rcBody.right -1,  rcBody.bottom);
// 				}
// 
// 				if ( Row == RowMax)
// 				{
// 					::MoveToEx(	PaintDC,rcBody.left, rcDirtyBody->bottom - 1, 0);
// 					::LineTo(	PaintDC, rcDirtyBody->right,  rcDirtyBody->bottom - 1);
// 				}
// 			}
// 			else
			{
				if (Col)
				{
					SelectObject(PaintDC, m_hNewPen);
					::MoveToEx(	PaintDC,rcBody.left, rcBody.top, 0);
					::LineTo(	PaintDC, rcBody.left,  rcBody.bottom);
				}	
			}
			
			PrePosX += (rcBody.right - rcBody.left);            
		}
	}
}

void CCustomView::OnCustomNcPaint(WPARAM wParam,LPARAM lParam)
{
	HBRUSH hBrush;
	RECT rw;
	RECT rc;
	POINT pt;
	HDC hdc = GetWindowDC(m_hWnd);
	GetWindowRect(m_hWnd, &rw);
	GetClientRect(m_hWnd, &rc);
	pt.x = rc.left;
	pt.y = rc.top;
	ClientToScreen(m_hWnd, &pt);
	OffsetRect(&rc, pt.x - rw.left, pt.y - rw.top);
	ExcludeClipRect(hdc, rc.left, rc.top, rc.right, rc.bottom);  // 扣除客户区

	OffsetRect(&rw, -rw.left, -rw.top);
	
	FillRect(hdc, &rw, m_hLineFrameBrush);
	ReleaseDC(m_hWnd,hdc);
}

void CCustomView::OnCustomNcClassSize( WPARAM wParam,LPARAM lParam)
{
	if ( (BOOL) wParam)
	{
		NCCALCSIZE_PARAMS FAR* lpNcsp = (NCCALCSIZE_PARAMS FAR*)lParam;
		if (lpNcsp)
		{
			InflateRect(&((NCCALCSIZE_PARAMS* )lParam)->rgrc[0],-0.5,-0.5);
  			//lpNcsp->rgrc[0].top += 1;   
  			//lpNcsp->rgrc[0].bottom -= 1;   
  			//lpNcsp->rgrc[0].left += 1;   
  			//lpNcsp->rgrc[0].right -= 1;
						
		}
	}
}

// Handle WM_PAINT message.
void CCustomView::OnCustomPaint(HDC hDC, RECT* prcDirty, BOOL bErase)
{
    int PosHorz;
    int PosVert;
	int ItemHeight;
	int HeaderHeight;
    RECT rcDirtyBody;
    int col, ColMin, ColMax;
    int RowMin = 0, RowMax = 0;

	HBITMAP hCompatibleBitMap,hOldBitmap;
	HDC	hCompatibleDC;

	hCompatibleBitMap = NULL;
	hOldBitmap = NULL;
  
    PosHorz				= m_ViewPags.HorScrollPos;
    PosVert				= m_ViewPags.VerScrollPos;
	RowMin				= 0;

	m_RowMax			= m_CustomItem.GetItemCount();
	
	HeaderHeight		= m_CustomHeader.GetColumnHeight();
	ItemHeight			= m_ScreenView.GetItemHeigth();
	   
    rcDirtyBody.left	= MAX(prcDirty->left, HEADER_CX);
    rcDirtyBody.top		= MAX(prcDirty->top, HeaderHeight);
    rcDirtyBody.right	= prcDirty->right;
    rcDirtyBody.bottom	= prcDirty->bottom;

    ColMin				= 0;
    ColMax				= m_CustomHeader.GetMaxColumn();

    if (ColMax > m_ColMax)
        ColMax			= m_ColMax;

	RowMax				= RowMin + m_ViewPags.PerPagesSize + 1;
		
	hCompatibleDC		= CreateCompatibleDC(hDC);

	if (hCompatibleDC)
	{
		hCompatibleBitMap = CreateCompatibleBitmap(
													hDC,
													prcDirty->right,
													prcDirty->bottom);
	}
	else
	{
		hCompatibleDC = hDC;
	}

	if (hCompatibleBitMap)
	{
		hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC,hCompatibleBitMap);
	}

    if (m_hPaintFont != NULL)
        SelectObject(hDC, m_hPaintFont);

    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, HEADER_TEXT_COLOR);

	OnCustomPaintHeader(
					hDC,
					prcDirty,
					&rcDirtyBody,
					ColMin,
					ColMax,
					RowMin,
					RowMax,
					PosHorz,
					PosVert);

	OnCustomPaintRowHeader();

    // Paint row headers:
	if (m_hPaintFont != NULL)
		SelectObject(hCompatibleDC, m_hPaintFont);

    CustomSetClip(hDC, &rcDirtyBody);

	if(ItemHeight)
	{		
		OnCustomPaintBody(
				hCompatibleDC,
				prcDirty,
				&rcDirtyBody,
				HeaderHeight,
				ItemHeight,
				ColMin,
				ColMax,
				RowMin,
				RowMax,
				PosHorz,
				PosVert);

		if (hCompatibleBitMap)
		{
			BitBlt(	hDC,
				rcDirtyBody.left,
				rcDirtyBody.top,
				rcDirtyBody.right - rcDirtyBody.left,
				rcDirtyBody.bottom - rcDirtyBody.top,
				hCompatibleDC,
				HEADER_CX,
				HeaderHeight,
				SRCCOPY);
		}

	}

	if (hCompatibleBitMap)
	{
		SelectObject(hCompatibleDC,hOldBitmap);
		DeleteObject(hCompatibleBitMap);
		DeleteDC(hCompatibleDC);
	}
}

// Lets distinguish "real nPage" and logical "nPage". 
// The former is the value describing the state of scrollbar and
// stored internally. We can get it with GetScrollInfo().
//
// The latter is how much we really want to scroll when scrolling
// up or down for a page.
int CCustomView::CustomLogicalPage(int nPage)
{
	int nScrollStep = m_CustomItem.GetItemHeigth();
	int nHeaderHeight = m_CustomHeader.GetColumnHeight();

	if ( nPage == 0)
	{
		return m_ViewPags.PerPagesSize  * nScrollStep;
	}

    if ( nPage <= ( nHeaderHeight + nScrollStep))
    {
		return 0;
    }

	int MovePages = nPage - nHeaderHeight;

	if ( MovePages % nScrollStep != 0)
	{
		MovePages = ( MovePages/nScrollStep + 1)*nScrollStep;
	}

    // In other cases, lets respect the real value.
    return MovePages;
}

// Perform the scrolling to the new location.
// (Ensures the newPos is in the allowed range between nMin and nMax so all 
// callers can be simplified in this regard.)
void CCustomView::CustomSetScrollPos(int nPos, BOOL isVertical)
{
    int		scrollbarId = isVertical ? SB_VERT : SB_HORZ;
    int		nOldPos;
    int		dx, dy;
    RECT	rcBody;

    nOldPos = GetScrollPos(m_hWnd, scrollbarId);

    // Setup and refresh the toolbar:
    SetScrollPos(m_hWnd, scrollbarId, nPos, TRUE);

    // Ensure the new pos is in the allowed range (between nin and nMax):
    nPos = GetScrollPos(m_hWnd, scrollbarId);
	m_PreScrollPos = nPos;

    // Refresh client area:
    GetClientRect(m_hWnd, &rcBody);
    if (isVertical) {
        dx = 0;
        dy = nOldPos - nPos;
        rcBody.top = m_CustomHeader.GetColumnHeight();

		m_ViewPags.VerScrollPos = nPos;

#ifdef OLD_ITEM_BODY
#else
		int MoveItemNums = 0;
		int ItemHeight = m_ScreenView.GetItemHeigth();
		if (ItemHeight)
		{
			MoveItemNums = abs(dy)/ItemHeight;
		}

		m_ScreenView.MoveItem(MoveItemNums,m_ViewPags.PerPagesSize,nOldPos>nPos?TRUE:FALSE);
		
#endif
    } else {
        dx = nOldPos - nPos;
        dy = 0;
        rcBody.left = HEADER_CX;

		m_ViewPags.HorScrollPos = nPos;
    }

	::InvalidateRect(m_hWnd,&rcBody,TRUE);
//     ScrollWindowEx(
// 				m_hWnd,
// 				dx,
// 				dy,
// 				&rcBody,
// 				&rcBody,
// 				NULL,
// 				NULL,
// 				SW_ERASE | SW_INVALIDATE);
}

// Handle VM_VSCROLL and VM_HSCROLL:
void CCustomView::CustomScroll(int iWhat, BOOL isVertical)
{
	if ( m_ParentSelectView != -1 && m_ParentSelectView != m_ViewNameFlags )
	{
		HWND hParent = GetParent(m_hWnd);
		::SendMessage(hParent,WM_CPU_VIEW_LBUTTON,0x999,m_ViewNameFlags);
	}

#ifdef OLD_ITEM_BODY
	if (m_CustomItem.GetItemCount())
#else
	if (m_ScreenView.GetItemCount())
#endif
	{
		int scrollbarId = isVertical ? SB_VERT : SB_HORZ;
		int nPos = 0;;
		int nScrollStep;
		SCROLLINFO si;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
		GetScrollInfo(m_hWnd, scrollbarId, &si);
#ifdef OLD_ITEM_BODY
		nScrollStep = m_CustomItem.GetItemHeigth();
#else
		nScrollStep = m_ScreenView.GetItemHeigth();
#endif
		switch (iWhat) {
		case SB_TOP:            nPos = si.nMin; break;
		case SB_BOTTOM:         nPos = si.nMax; break;
		case SB_LINEUP:         nPos = si.nPos - nScrollStep; break;
		case SB_LINEDOWN:       nPos = si.nPos + nScrollStep; break;
		case SB_PAGEUP:         nPos = si.nPos - CustomLogicalPage(si.nPage); break;
		case SB_PAGEDOWN:       nPos = si.nPos + CustomLogicalPage(si.nPage); break;
		case SB_THUMBTRACK:
			{
				if (si.nTrackPos)
				{
					nPos = CustomLogicalPage(si.nTrackPos);
				}
				else
				{
					nPos = 0;
				}
							
			}
			break;
		default:
		case SB_THUMBPOSITION:  nPos = si.nPos; break;
		}

		CustomSetScrollPos(nPos, isVertical);
	}
   
}

// Handle WM_MOUSEWHEEL and WM_MOUSEHWHEEL:
void CCustomView::CustomMouseWheel( int iDelta, BOOL isVertical)
{

	if ( m_ParentSelectView != -1 && m_ParentSelectView != m_ViewNameFlags )
	{
		HWND hParent = GetParent(m_hWnd);
		::SendMessage(hParent,WM_CPU_VIEW_LBUTTON,0x999,m_ViewNameFlags);
	}
#ifdef 	OLD_ITEM_BODY
	if (m_CustomItem.GetItemCount())
#else
if (m_ScreenView.GetItemCount())
#endif
	{
		int scrollbarId = isVertical ? SB_VERT : SB_HORZ;
		SCROLLINFO si;
		int iLines;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_POS;
		GetScrollInfo(m_hWnd, scrollbarId, &si);

		// Compute how many lines to scroll.
		//     iLines = WheelScrollLines(m_hWnd, iDelta * m_CustomItem.GetItemHeigth(),
		//         CustomLogicalPage(si.nPage), isVertical);
		if (iDelta > 0)
		{
#ifdef 	OLD_ITEM_BODY
			iLines = 0 - m_CustomItem.GetItemHeigth()*2;
#else
			iLines = 0 - m_ScreenView.GetItemHeigth()*2;
#endif
		}
		else if ( iDelta < 0 )
		{
#ifdef 	OLD_ITEM_BODY
			iLines = m_CustomItem.GetItemHeigth()*2;
#else
			iLines = m_ScreenView.GetItemHeigth()*2;
#endif
		}


		// Scroll to the desired location.
		CustomSetScrollPos( si.nPos + iLines, isVertical);
	}
    
}

void CCustomView::CustomKeyUp(UINT vkCode)
{
   if (vkCode == VK_RETURN)
   {
	   CustomVkEnter();
   }
}
// Handle WM_KEYDOWN message.
void CCustomView::CustomKeyDown(UINT vkCode)
{
    BOOL isShiftPressed = (GetKeyState(VK_SHIFT) & 0x8000);

	if ( vkCode == VK_DOWN ||
		vkCode == VK_UP)
	{
		//
		//
#ifdef OLD_ITEM_BODY
		if (m_ViewPags.SelectMinPageItem >= 0)
#else
		if (m_ScreenView.GetSelItem() > 0)
#endif
		
		{
			int RowMin = 0;
			int HeaderHeight = 0;
			int ItemHeight = 0;
			int RowMax = 0;
			int PosHorz,PosVert;
			RECT Rect,rcFlush;
			ULONG uOldSelect = -1;
			int uNextSelectItem = -1;
			GetClientRect(m_hWnd,&Rect);
			rcFlush.left = Rect.left;
			rcFlush.right = Rect.right;

#ifdef OLD_ITEM_BODY
			uOldSelect = m_ViewPags.SelectMinPageItem;
			uNextSelectItem = m_ViewPags.SelectMinPageItem;
			PosHorz = GetScrollPos(m_hWnd, SB_HORZ);
			PosVert = GetScrollPos(m_hWnd, SB_VERT);
			HeaderHeight = m_CustomHeader.GetColumnHeight();
			ItemHeight   = m_CustomItem.GetItemHeigth();
			RowMin = PosVert;
			RowMin /= ItemHeight;

			RowMax = (Rect.bottom - HeaderHeight + PosVert);
			RowMax /= ItemHeight;
#else
			PosHorz = 0;
			PosVert = 0;
			HeaderHeight = m_CustomHeader.GetColumnHeight();
			ItemHeight   = m_ScreenView.GetItemHeigth();
			RowMin = PosVert;
			
			RowMax = m_ViewPags.PerPagesSize;

			for ( int i = 0;i < RowMax;i++)
			{
				if ( m_ScreenView.GetSelItem() == m_ScreenView.GetItem(i))
				{
					uOldSelect = i;
					uNextSelectItem = uOldSelect;
					m_ScreenView.SetSingleSelItem(uOldSelect);
					break;
				}
			}

			if (uOldSelect == -1 )
			{
				PCustomItemHead pSelItem = m_ScreenView.GetSelItem();
				
				for ( int i = 0 ;i < RowMax ; i++ )
				{
					m_ScreenView.SetItem(i,pSelItem);

					if (pSelItem)
					{
						pSelItem = pSelItem->tagNext;
					}
				}

				uOldSelect = 0;
				uNextSelectItem = uOldSelect;
				m_ScreenView.SetSingleSelItem(0);				
			}

			::InvalidateRect(m_hWnd,&Rect,TRUE);
			
#endif	

			//RowMin = Rect.top - nHeaderHeight + PosVert - (nItemHeight - 1);
			if ( VK_UP == vkCode )
			{
				uNextSelectItem--;
			}
			else
			{
				uNextSelectItem++;
			}


#ifdef OLD_ITEM_BODY
			if ( uNextSelectItem >= 0 &&
				uNextSelectItem  < m_RowMax )
#else
			if( uNextSelectItem >= -1 &&
				uNextSelectItem  <= m_ViewPags.PerPagesSize)
#endif
			{
#ifdef OLD_ITEM_BODY
				m_ViewPags.SelectMinPageItem = uNextSelectItem;
#else
				if ( VK_UP == vkCode )
				{
					if (m_ScreenView.IsFirstItem())
					{
						return;
					}

					m_ScreenView.SelToPreItem();
				}
				else
				{
					m_ScreenView.SelToNextItem();
				}
#endif
				rcFlush.top = HeaderHeight + uOldSelect * ItemHeight - PosVert;
				rcFlush.bottom = rcFlush.top + ItemHeight;

				rcFlush.left = MAX(Rect.left, HEADER_CX);
				rcFlush.top = MAX(Rect.top, HeaderHeight);
				rcFlush.right = Rect.right;
				rcFlush.bottom = Rect.bottom;

				::InvalidateRect(m_hWnd,&rcFlush,TRUE);
				
				if ( uNextSelectItem < RowMin )
				{
					CustomScroll( SB_LINEUP, !isShiftPressed);
				}
				else if ( uNextSelectItem > ( RowMax - 1) )
				{
					 CustomScroll( SB_LINEDOWN, !isShiftPressed);
				}

				rcFlush.top = HeaderHeight + m_ViewPags.SelectMinPageItem * ItemHeight - PosVert;
				rcFlush.bottom = rcFlush.top + ItemHeight;

				::InvalidateRect(m_hWnd,&rcFlush,TRUE);
			}

		}
	}

    switch (vkCode) {
    case VK_HOME:   CustomScroll( SB_TOP, !isShiftPressed); break;
    case VK_END:    CustomScroll( SB_BOTTOM, !isShiftPressed); break;
    case VK_PRIOR:  CustomScroll( SB_PAGEUP, !isShiftPressed); break;
    case VK_NEXT:   CustomScroll( SB_PAGEDOWN, !isShiftPressed); break;
  //  case VK_UP:    	CustomScroll( SB_LINEUP, TRUE); break;
  //  case VK_DOWN:   CustomScroll( SB_LINEDOWN, TRUE); break;
    case VK_LEFT:   CustomScroll( SB_LINELEFT, FALSE); break;
    case VK_RIGHT:  CustomScroll( SB_LINERIGHT, FALSE); break;
	case VK_RETURN: CustomVkEnter();break; 

    }	
}

void CCustomView::CustomVkEnter()
{

}

HRESULT CCustomView::CustomMouseLeave( WPARAM wp,LPARAM lp )
{
	m_bTrackLine = -1;	

	return 0;
}

HRESULT CCustomView::CustomTimer( WPARAM wp,LPARAM lp )
{
	if ( m_bLButtonDown == TRUE && !m_ScreenView.isSingleSel() )
	{
		RECT Rect;
		POINT MousePt;
		BOOL bIsRoomed;
		int PosHorz;
		int ColMax;
		int ColCur;
		int PrePosX;
		int PreColumnWidth;
		int PosMulX;
		int HeaderHeight = 0;
		int ItemHeight = 0;
		RECT rcDirtyBody;
		GetCursorPos(&MousePt);
		ScreenToClient(m_hWnd, &MousePt);
		GetClientRect(m_hWnd, &Rect);
		HeaderHeight = m_CustomHeader.GetColumnHeight();

#ifdef OLD_ITEM_BODY
		ItemHeight   = m_CustomItem.GetItemHeigth();
#else
		ItemHeight   = m_ScreenView.GetItemHeigth();
#endif

		rcDirtyBody.left = MAX(Rect.left, HEADER_CX);
		rcDirtyBody.top = MAX(Rect.top, HeaderHeight);
		rcDirtyBody.right = Rect.right;
		rcDirtyBody.bottom = Rect.bottom;

		if ( (rcDirtyBody.top < MousePt.y && 
			rcDirtyBody.bottom >	MousePt.y &&
			rcDirtyBody.left <	MousePt.x && 
			rcDirtyBody.right >	MousePt.x) )
		{
			
		}

		if(MousePt.y > rcDirtyBody.bottom || rcDirtyBody.top > MousePt.y)
		{			
			int RowMin = 0;
			int RowMax = 0;
			int PosHorz,PosVert;		
			int PosCursor = MousePt.y;
			
			if (MousePt.y > rcDirtyBody.bottom)
			{
				CustomSetScrollPos(m_ViewPags.VerScrollPos + ItemHeight,TRUE);
			}
			else
			{
				CustomSetScrollPos(m_ViewPags.VerScrollPos - ItemHeight,TRUE);
			}

#ifdef OLD_ITEM_BODY
			PosVert = m_ViewPags.VerScrollPos;
			RowMin = PosVert;
			RowMin /= ItemHeight;
#else
			PosVert = 0;
			RowMin = PosVert;
#endif
			RowMax = RowMin + m_ViewPags.PerPagesSize;

			if (MousePt.y > rcDirtyBody.bottom)
			{
#ifdef OLD_ITEM_BODY
				m_ViewPags.SelectMaxPageItem = RowMax;
#else
				if (m_ScreenView.GetItem(RowMax))
				{
					m_ScreenView.SetMultiSelItem(RowMax);
				}
				
#endif
			}
			else
			{
#ifdef OLD_ITEM_BODY
				m_ViewPags.SelectMaxPageItem = RowMin;
#else
				m_ScreenView.SetMultiSelItem(RowMin);
#endif
			}
						
			::InvalidateRect(
							m_hWnd,
							&m_ViewPags.PageRect,
							TRUE);			
		}
	}

	return 0;
}

HRESULT CCustomView::CustomMouseHove( WPARAM wp,LPARAM lp )
{
	if( m_bLButtonDown == TRUE)
	{
		if (wp == MOUSE_TIMER_MOVE_SELECT)
		{
			RECT Rect;
			POINT MousePt;
			BOOL bIsRoomed;
			int PosHorz;
			int ColMax;
			int ColCur;
			int PrePosX;
			int PreColumnWidth;
			int PosMulX;
			GetCursorPos(&MousePt);
			ScreenToClient(m_hWnd, &MousePt);
			GetClientRect(m_hWnd, &Rect);

			if ( Rect.top < MousePt.y && 
				Rect.bottom >	MousePt.y &&
				Rect.left <	MousePt.x && 
				Rect.right >	MousePt.x )
			{

			}
			else
			{
				PostMessage(m_hWnd,WM_MOUSEWHEEL,0xff000000,0);
			}
		}
		
	}

	return 0;
}

HRESULT CCustomView::CustomMouseMove( WPARAM wp,LPARAM lp )
{
	RECT Rect;
	POINT MousePt;
	BOOL bIsRoomed;
	int PosHorz;
	int ColMax;
	int ColCur;
	int PrePosX;
	int PreColumnWidth;
	int PosMulX;
	GetCursorPos(&MousePt);
	ScreenToClient(m_hWnd, &MousePt);
	GetClientRect(m_hWnd, &Rect);

	PrePosX = 0;
	PreColumnWidth = 0;
	bIsRoomed = IsZoomed(m_hWnd);
	PosHorz = GetScrollPos(m_hWnd, SB_HORZ);
	ColMax = m_CustomHeader.GetMaxColumn();
	ColCur = m_bTrackLine;

	TRACKMOUSEEVENT tme;     
	tme.cbSize = sizeof(TRACKMOUSEEVENT); //监控鼠标离开     
	tme.dwFlags = TME_LEAVE/*|TME_HOVER*/;     
	tme.hwndTrack = m_hWnd;  
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
	
	HCURSOR hCur = LoadCursor(0,IDC_ARROW);

	if ( ColCur > 0 )
	{
		RECT rcFlush;
		PosMulX = m_CustomHeader.GetColumnWidth(ColCur - 1);
		PrePosX = abs(m_ptTrackPos.x - MousePt.x);
		rcFlush.left = m_ptTrackPos.x;
		rcFlush.top = Rect.top;
		rcFlush.bottom = Rect.bottom;
		rcFlush.right = Rect.right;

		if ( PosMulX >= 0 && MousePt.x > 1 )
		{
			if ( MousePt.x > m_ptTrackPos.x)
			{
				PosMulX = PosMulX + PrePosX;
			}
			else
			{
				PosMulX = PosMulX - PrePosX;
				rcFlush.left -= PrePosX;

				if ( m_CustomHeader.GetColumnWidth(ColCur - 1) < 2 )
				{
					goto __NEXT_MOVE;
				}
			}			

			m_CustomHeader.SetColumnWidth( ColCur - 1 , PosMulX);

			hCur = LoadCursor(0,IDC_SIZEWE);

			::InvalidateRect(m_hWnd,NULL,TRUE);
			m_ptTrackPos.x = MousePt.x;
		}
	}
	else if( m_bLButtonDown == TRUE && !m_ScreenView.isSingleSel())
	{
		if ( Rect.top < MousePt.y && 
			Rect.bottom >	MousePt.y &&
			Rect.left <	MousePt.x && 
			Rect.right >	MousePt.x )
		{
			RECT rcDirtyBody;
			int HeaderHeight = 0;
			int ItemHeight = 0;

			HeaderHeight = m_CustomHeader.GetColumnHeight();

#ifdef OLD_ITEM_BODY

			ItemHeight   = m_CustomItem.GetItemHeigth();
#else
			ItemHeight   = m_ScreenView.GetItemHeigth();
#endif

			rcDirtyBody.left = MAX(Rect.left, HEADER_CX);
			rcDirtyBody.top = MAX(Rect.top, HeaderHeight);
			rcDirtyBody.right = Rect.right;
			rcDirtyBody.bottom = Rect.bottom;

			if ( rcDirtyBody.top < MousePt.y && 
				rcDirtyBody.bottom >	MousePt.y &&
				rcDirtyBody.left <	MousePt.x && 
				rcDirtyBody.right >	MousePt.x )
			{
				int RowMin = 0;
				
				int RowMax = 0;
				int PosHorz,PosVert;
				int PosCursor = MousePt.y;

#ifdef OLD_ITEM_BODY
				ULONG OldSelectItem = m_ViewPags.SelectMinPageItem;
				PosVert = GetScrollPos(m_hWnd, SB_VERT);
				RowMin = PosVert;
				RowMin /= ItemHeight;

				RowMax = (Rect.bottom - HeaderHeight + PosVert);
				RowMax /= ItemHeight;
#else
				PosVert = 0;
				RowMin = PosVert;				
				RowMax = m_ViewPags.PerPagesSize;
#endif

				if ( RowMax >= RowMin && PosCursor > HeaderHeight )
				{
					RECT rcFlush;
					PosCursor -= HeaderHeight;
					PosCursor /= ItemHeight;

					PosCursor += RowMin;

				
#ifdef OLD_ITEM_BODY
					m_ViewPags.SelectMaxPageItem = PosCursor;

#else
					if (m_ScreenView.GetItem(PosCursor))
					{
						m_ScreenView.SetMultiSelItem(PosCursor);
					}
					
#endif
					::InvalidateRect(
						m_hWnd,
						&rcDirtyBody,
						TRUE);
					
				}
			}
			else
			{
				SetTimer(
					m_hWnd,
					MOUSE_TIMER_MOVE_SELECT,
					MOUSE_TIMER,
					NULL);
			}
			
			
		}
		else
		{
			SetTimer(
					m_hWnd,
					MOUSE_TIMER_MOVE_SELECT,
					MOUSE_TIMER,
					NULL);
		}
	}

__NEXT_MOVE:

	if ( Rect.top < MousePt.y && MousePt.y < Rect.bottom &&
		 Rect.left < MousePt.x && Rect.right > MousePt.x)
	{
		for (int Col = 0; Col < ColMax; Col++)
		{
			RECT rc = {0};
			rc.left = HEADER_CX + PrePosX - PosHorz;
			rc.right = rc.left + m_CustomHeader.GetColumnWidth(Col);

			PrePosX += m_CustomHeader.GetColumnWidth(Col);

			if ( rc.left == MousePt.x || ( rc.left - 1) == MousePt.x)
			{	
				if (m_bLButtonDown == FALSE)
				{
					hCur = LoadCursor(0,IDC_SIZEWE);
				}											
				
				break;
			}
		}
	}

	SetCursor(hCur);

	return 0;
}

HRESULT CCustomView::CustomMouseUp( WPARAM wp,LPARAM lp)
{
	if ( GetCapture() == m_hWnd)
	{
		ReleaseCapture();
	}

	m_bTrackLine = -1;
	m_bLButtonDown = FALSE;

	KillTimer( 
		m_hWnd,
		MOUSE_TIMER_MOVE_SELECT);

	return 0;
}

HRESULT CCustomView::CustomNcLMouseDown( WPARAM wp,LPARAM lp )
{
	if ( m_ParentSelectView != -1 && m_ParentSelectView != m_ViewNameFlags )
	{
		HWND hParent = GetParent(m_hWnd);
		::SendMessage(hParent,WM_CPU_VIEW_LBUTTON,0x999,m_ViewNameFlags);
	}

	return 0;
}

HRESULT CCustomView::CustomRButtonDown( WPARAM wp,LPARAM lp )
{
	return 0;
}

HRESULT CCustomView::CustomLDoubleClick( WPARAM wp,LPARAM lp )
{
	return 0;
}

HRESULT CCustomView::CustomLButtonDown( WPARAM wp,LPARAM lp)
{
	RECT	Rect;
	POINT	MousePt;
	BOOL	bIsRoomed;
	int		PosHorz;
	int		ColMax;
	int		PrePosX;
	ULONG   MouseClickItem;

	
	GetCursorPos(&MousePt);
	ScreenToClient(m_hWnd, &MousePt);
	GetClientRect(m_hWnd, &Rect);
	SetCapture(m_hWnd);
	//KillTimer( m_hWnd,MOUSE_TIMER_MOVE_SELECT);
	MouseClickItem = 0;
	PrePosX = 0;
	bIsRoomed = IsZoomed(m_hWnd);
	PosHorz = GetScrollPos(m_hWnd, SB_HORZ);
	ColMax = m_CustomHeader.GetMaxColumn();

	if ( m_ParentSelectView != -1 && m_ParentSelectView != m_ViewNameFlags )
	{
		HWND hParent = GetParent(m_hWnd);
		::SendMessage(hParent,WM_CPU_VIEW_LBUTTON,0x999,m_ViewNameFlags);
	}
	
	if ( Rect.top <= MousePt.y && 
		Rect.bottom >=	MousePt.y &&
		Rect.left <=	MousePt.x && 
		Rect.right >=	MousePt.x )
	{
		
		for (int col = 0; col < ColMax; col++)
		{
			RECT rc = {0};
			rc.left = HEADER_CX + PrePosX - PosHorz;
			rc.right = rc.left + m_CustomHeader.GetColumnWidth(col);

			PrePosX += m_CustomHeader.GetColumnWidth(col);

			if (rc.left == MousePt.x ||  ( rc.left - 1) == MousePt.x )
			{
				m_bTrackLine = col;
				m_ptTrackPos.x = MousePt.x;
				m_ptTrackPos.y = MousePt.y;

				HCURSOR hCur = LoadCursor(0,IDC_SIZEWE);
				SetCursor(hCur);
				
				break;
			}
		}

		if ( m_bTrackLine < 0 )
		{
			int RowMin = 0;
			int HeaderHeight = 0;
			int ItemHeight = 0;
			int RowMax = 0;
			int PosHorz,PosVert;
			int PosCursor = MousePt.y;

#ifdef OLD_ITEM_BODY
			ULONG OldSelectItem = m_ViewPags.SelectMinPageItem;

			PosHorz = GetScrollPos(m_hWnd, SB_HORZ);
			PosVert = GetScrollPos(m_hWnd, SB_VERT);
#else
			PosHorz = 0;
			PosVert = 0;
#endif
			HeaderHeight = m_CustomHeader.GetColumnHeight();
			ItemHeight   = m_CustomItem.GetItemHeigth();

			if (ItemHeight)
			{
				//RowMin = Rect.top - nHeaderHeight + PosVert - (nItemHeight - 1);				

#ifdef OLD_ITEM_BODY
				RowMin = PosVert;
				RowMin /= ItemHeight;


				RowMax = (Rect.bottom - HeaderHeight + PosVert);
				RowMax /= ItemHeight;
#else
				RowMin = 0;
				RowMax = m_ViewPags.PerPagesSize;
#endif
				if ( RowMax >= RowMin && PosCursor > HeaderHeight )
				{
					RECT rcFlush;
					PosCursor -= HeaderHeight;
					PosCursor /= ItemHeight;

					PosCursor += RowMin;

					rcFlush.left = HEADER_CX;
					rcFlush.right = Rect.right - HEADER_CX;
#ifdef OLD_ITEM_BODY
					if (PosCursor >= 0 && PosCursor < m_RowMax)
#else
					if(PosCursor >= 0)
#endif
					{
						
						m_bLButtonDown = TRUE;
#ifdef OLD_ITEM_BODY
						m_ViewPags.SelectMinPageItem = PosCursor;
						m_ViewPags.SelectMaxPageItem = -1;
						if (OldSelectItem >= 0)
#else
						if(m_ScreenView.GetItem(PosCursor))
#endif
						{
#ifdef OLD_ITEM_BODY
#else
							m_ScreenView.SetSingleSelItem(PosCursor);
#endif
							rcFlush.left = MAX(Rect.left, HEADER_CX);
							rcFlush.top = MAX(Rect.top, HeaderHeight);
							rcFlush.right = Rect.right;
							rcFlush.bottom = Rect.bottom;

							::InvalidateRect(m_hWnd,&rcFlush,TRUE);
						}
#ifdef OLD_ITEM_BODY
						rcFlush.top = HeaderHeight + PosCursor * ItemHeight - PosVert;
						rcFlush.bottom = rcFlush.top + ItemHeight;

						::InvalidateRect(m_hWnd,&rcFlush,TRUE);
#endif
					}
				}
			}			
		}
	}

	return 0;
}

// Control window procedure.
LRESULT 
CCustomView::CustomProc(
				HWND hwnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam)
{
    CCustomView* pData = (CCustomView*) GetWindowLongPtr(hwnd, 0);

	if (pData)
	{
		switch(uMsg) {
		case XXM_SETCOLRANGE:
			return pData->CustomSetColRange( wParam, lParam);

		case XXM_SETROWRANGE:
			return pData->CustomSetRowRange( wParam, lParam);

		case WM_ERASEBKGND:
			return TRUE;  // Defer erasing into WM_PAINT

		//case WM_NCPAINT:
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				RECT rcClient;
				GetClientRect(hwnd,&rcClient);
				pData->OnCustomPaint(ps.hdc, &rcClient, ps.fErase);
				EndPaint(hwnd, &ps);				
			}
			return 0;
			
		case WM_PRINTCLIENT:
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				pData->OnCustomPaint((HDC) wParam, &rc, TRUE);
				return 0;
			}
		case WM_NCCALCSIZE:
			{
				if ( pData->IsShowBorder())
				{
					pData->OnCustomNcClassSize(wParam,lParam);

					//return 0;
				}
			}
			break;
		case WM_NCACTIVATE:
				{
					PostMessage(hwnd,WM_NCPAINT,1,0);
				}
				break;
		case WM_NCPAINT:
			{
				if (pData->IsShowBorder())
				{
					pData->OnCustomNcPaint(wParam,lParam);
					//return 0;
				}
				break;
			}
			break;
		case WM_KEYDOWN:
			pData->CustomKeyDown(wParam);
			return 0;
		case WM_SYSKEYDOWN:
			pData->CustomKeyUp(wParam);
			return 0;
		case WM_KEYUP:
			//pData->CustomKeyUp(wParam);
			break;
		case WM_GETDLGCODE:
			return DLGC_WANTARROWS;

		case WM_VSCROLL:
		case WM_HSCROLL:
			pData->CustomScroll(LOWORD(wParam), (uMsg == WM_VSCROLL));
			return 0;

		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			{
				pData->CustomMouseWheel((SHORT)HIWORD(wParam), (uMsg == WM_MOUSEWHEEL));
			}
			return 0;

		case WM_LBUTTONDOWN:
			{
				SetFocus(hwnd);
				pData->CustomLButtonDown(wParam,lParam);
				return 0;
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				pData->CustomLDoubleClick(wParam,lParam);
			}
			return 0;
		case WM_RBUTTONDOWN:
			{
				pData->CustomRButtonDown(wParam,lParam);
			}
			return 0;

		case WM_LBUTTONUP:
			{
				pData->CustomMouseUp(wParam,lParam);
				return 0;
			}
			break;
		
		case WM_MOUSEMOVE:
			{
				pData->CustomMouseMove(wParam,lParam);
			}
			return 0;

		case WM_MOUSEHOVER:
			{
				pData->CustomMouseHove(wParam,lParam);
				return 0;
			}
			break;
		case WM_MOUSELEAVE:
			{
				pData->CustomMouseLeave(wParam,lParam);
				return 0;
			}
			break;
		case WM_SIZE:
			pData->CustomSize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_TIMER:
			{
				pData->CustomTimer(wParam,lParam);
				return 0;
			}
			break;
		case WM_NCCREATE:
			{
				*pData = hwnd;
			}
			return TRUE;
// 		case WM_NCDESTROY:
// 			if(pData != NULL)
// 				delete pData;
// 			return 0;
		}
	}
	
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL
CCustomView::Register(
					HINSTANCE hInstance,
					LPTSTR lpClassName)
{
    WNDCLASS wc = { 0 };

    wc.hInstance = hInstance;
    wc.lpfnWndProc = CustomProc;
	wc.style	   = 0xB;
    wc.cbWndExtra = sizeof(CCustomView*);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = lpClassName;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	ULONG dwError = RegisterClass(&wc);

	if (!dwError)
	{
		if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
		{
			return TRUE;
		}
	}

    return dwError;
}

void
CCustomView::Unregister(void)
{
    UnregisterClass(CUSTOM_WC, NULL);
}

BOOL CCustomView::InsertColumn( int nColumn , TCHAR* szText , int nWideth )
{
	return m_CustomHeader.InsertColumn(nColumn,szText,nWideth);
}

void	CCustomView::SetColumnWidth( int nColumn , int nWideth )
{
	m_CustomHeader.SetColumnWidth(nColumn,nWideth);
}

PCustomItemHead	CCustomView::InsertItem( uint64_t u64ptr, LPTSTR lpText )
{
	PCustomItemHead pItem = m_ScreenView.InsertItem(u64ptr,lpText,m_hNormalBrushA,m_hNormalBrushB);
	if (pItem)
	{
		SendMessage(
			XXM_SETROWRANGE,
			(WPARAM)0,
			(LPARAM)m_ScreenView.GetItemCount());
	}

	return pItem;
}

void CCustomView::InsertItem( int nItem , uint64_t u64ptr, LPTSTR lpText )
{
	return m_CustomItem.InsertItem(nItem,u64ptr,lpText,m_hNormalBrushA,m_hNormalBrushB);
}

void   CCustomView::InsertAddressColumn(int nItem,int IconNums,LPTSTR lpText )
{
	return m_CustomItem.InsertAddressColumn(nItem,IconNums,lpText,m_hNormalBrushA,m_hNormalBrushB);
}

void CCustomView::InsertAddressColumn(PCustomItemHead pItem,int IconNums,LPTSTR lpText )
{
	return m_ScreenView.InsertAddressColumn(pItem,IconNums,lpText,m_hNormalBrushA,m_hNormalBrushB);
}

void    CCustomView::SetAddressColumnIcon(int nItem,int IconNums,int nIconType,int nIconType2,int nIconType3 )
{
	return m_CustomItem.SetAddressColumnIcon(nItem,IconNums,nIconType,nIconType2,nIconType3);
}

void    CCustomView::SetAddressColumnIcon(PCustomItemHead pItem,int IconNums,int nIconType,int nIconType2,int nIconType3 )
{
	return m_ScreenView.SetAddressColumnIcon(pItem,IconNums,nIconType,nIconType2,nIconType3);
}

void    CCustomView::SetColumnItem(
							int nItem,
							int nColumn,
							int IconNums,
							tagOption* Option,
							int nSize )
{
	return m_CustomItem.SetColumnItem(nItem,nColumn,IconNums,Option,nSize,m_hNormalBrushA,m_hNormalBrushB);
}

void    CCustomView::SetColumnItem(
						PCustomItemHead pItem,
						int nColumn,
						int IconNums,
						tagOption* Option,
						int nSize )
{
	return m_ScreenView.SetColumnItem(pItem,nColumn,IconNums,Option,nSize,m_hNormalBrushA,m_hNormalBrushB);
}

void	CCustomView::SetItemText( int nItem , int nColumn, LPWSTR szText )
{
	return m_CustomItem.SetItemText(nItem,nColumn,szText);
}

void	CCustomView::SetItemText( PCustomItemHead pItem, int nColumn, LPWSTR szText )
{
	return m_ScreenView.SetItemText(pItem,nColumn,szText);
}

void CCustomView::SetItemHeight( int nHeight )
{
	m_CustomItem.SetItemHeight(nHeight);
}

void CCustomView::SetMaxItemNums( int nMaxItem)
{
	m_CustomItem.SetMaxItemNums(nMaxItem);
}

ULONG CCustomView::GetColumnWidth( int nColumn )
{
	return m_CustomHeader.GetColumnWidth(nColumn);
}

void CCustomView::SetHeaderHeight( int Height)
{
	m_CustomHeader.SetHeight(Height);
}

ULONG CCustomView::GetColumnHeight()
{
	return m_CustomHeader.GetColumnHeight();
}

ULONG	CCustomView::GetItemCount()
{
	return m_CustomItem.GetItemCount();
}

void CCustomView::BroadCastView( int SelectedView)
{
	m_ParentSelectView = SelectedView;
}

void CCustomView::SetShowBorder()
{
	m_bShowBoder = TRUE;
}

void CCustomView::SetDisableBorder()
{
	m_bShowBoder = FALSE;
}


BOOL CCustomView::Create(
					HWND hWnd,
					HINSTANCE hInstance,
					int nId,
					LPTSTR lpClassName,
					int ViewFlags,
					BOOL IsShowBorder)
{
	LONG Error = Register(hInstance,lpClassName);
	
	if(Error)
	{
		DWORD dwStyle = WS_VISIBLE |
						WS_CHILD |
						WS_TABSTOP;
		if (IsShowBorder)
		{
			dwStyle |= WS_BORDER;
		}

		m_hWnd = CreateWindow(
					lpClassName,
					NULL, 
					dwStyle,
					0,
					0,
					0,
					0,
					hWnd,
					(HMENU)nId,
					hInstance,
					0);

		if (m_hWnd)
		{
			m_ViewNameFlags = ViewFlags;
			SetWindowLongPtr(m_hWnd,0, (LONG_PTR)this);
			return TRUE;
		}
	}
	

	return FALSE;
}

HRESULT CCustomView::SendMessage(UINT uMsg , WPARAM wParam,LPARAM lParam )
{
	return ::SendMessage(
					m_hWnd,
					uMsg,
					wParam,
					lParam); 
}

BOOL CCustomView::SetWindowPos(
						HWND hWndInsertAfter,
						int X,
						int Y,
						int cx,
						int cy,
						UINT uFlags)
{
	return ::SetWindowPos( 
						m_hWnd,
						hWndInsertAfter,
						X,
						Y,
						cx,
						cy,
						uFlags);
}