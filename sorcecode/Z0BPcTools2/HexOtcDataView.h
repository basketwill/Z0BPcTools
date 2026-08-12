#ifndef _CHexOtcDataView_h
#define _CHexOtcDataView_h

#include "dynamicview.h"
#include "menu.h"

#define IDM_HEX_BACKUP 101
#define IDM_HEX_COPY_DATA 102
#define IDM_HEX_SHOW_ADDRESS 103
#define IDM_HEX_BINARY 104
#define IDM_HEX_SIGN_FLAGS 105

#define IDM_HEX_CREATE_BACKUP			1001
#define IDM_HEX_LOAD_BACKUP_FROM_FILE	1002
#define IDM_HEX_SAVE_BACKUP_TO_FILE		1002
typedef enum _tagHexDataType
{
	kHexOct08 = 8,
	kHexOct16 = 16,
	kHexOct32 = 32,
	kHexOct64 = 64,

	kNoteShowAnsi = 1,
	kNoteShowUinicode = 2,

};

#define HEX_VIEW_LINE_COLOR RGB(200,200,200)
#define HEX_FOCUS_TEXT_COLOR RGB(0,0,0)
#define HEX_NO_FOCUS_TEXT_COLOR RGB(128,128,128)
#define HEX_DATA_ADDRESS_COLOR RGB(128,128,128)


class CHexOctData
{
public:
	CHexOctData()
		:m_u64PointAddress(0)
		,m_HexType(kHexOct16)
	{

	}

	~CHexOctData()
	{

	}

	void SetMoudleBaseAddress(UINT64 BaseAddress,UINT64 MoudleSize)
	{
		m_u64MoudleAddress = BaseAddress;
		m_u64PointAddress  = m_u64MoudleAddress;
		m_u64MoudleSize = MoudleSize;
	}

	void SetHexDataAddress(UINT64 uAddress)
	{
		m_u64PointAddress = uAddress;
	}

	UINT64 GetPointAddress()
	{
		return m_u64PointAddress;
	}

	UINT64 GetMoudleAddress()
	{
		return m_u64MoudleAddress;
	}

	UINT64 GetMoudleSize()
	{
		return m_u64MoudleSize;
	}

	ULONG64 ReadData(
				UINT64& ReadAddress,
				PBYTE	ReadBuf,
				UINT64& DataSize)
	{
		UINT64 ReadSize = 0;

 		if (ReadAddress <  ( m_u64MoudleAddress + m_u64MoudleSize ) &&
 			ReadAddress >= m_u64MoudleAddress &&
 			ReadBuf &&
			ReadAddress)
 		{
 			if ( ( ReadAddress + DataSize ) <  ( m_u64MoudleAddress + m_u64MoudleSize ))
 			{
 				ReadSize = DataSize;
 				memcpy(ReadBuf,(PBYTE)ReadAddress,DataSize);
 			}
 			else
 			{
 				ReadSize =  ( m_u64MoudleAddress + m_u64MoudleSize -ReadAddress + 1);
 				memcpy(ReadBuf,(PBYTE)ReadAddress,ReadSize);
 			}

			return ReadSize;
 		}
		/*
		UINT64 Offset = ReadAddress;
		UINT64 CopySize = DataSize;
		if (Offset > 0 && CopySize)
		{
			if( m_SectionCount)
			{
				UINT64 uRawAddress = 0;
				UINT64 uRawSize = 0;
				UINT64 uVaOffset = 0;

				for ( int nI = 0;nI < m_SectionCount;nI++)
				{
					if (m_Section)
					{
						if ( ReadAddress >= m_Section[nI].m_uRawAddress &&
							ReadAddress < ( m_Section[nI].m_uRawAddress + 
							( m_Section[nI].m_uVirtualSize > m_Section[nI].m_uRawSize?m_Section[nI].m_uVirtualSize:m_Section[nI].m_uRawSize)))
						{
							uRawAddress = m_Section[nI].m_uRawAddress;
							uRawSize = m_Section[nI].m_uRawSize;
							uVaOffset = (ReadAddress - m_Section[nI].m_uRawAddress);
							break;
						}

					}
				}

				if (uRawAddress && uRawSize )
				{
					uRawAddress = uRawAddress + uVaOffset;
					CopySize = abs( (__int64)uRawSize - (__int64)uVaOffset);
					memcpy(ReadBuf,(const void*)uRawAddress, CopySize >= DataSize?DataSize:CopySize );

					return uRawSize;
				}
				else
				{
					if (ReadAddress <  ( m_u64MoudleAddress + m_u64MoudleSize ) &&
						ReadAddress >= m_u64MoudleAddress &&
						ReadBuf )
					{
						if ( ( ReadAddress + DataSize ) <  ( m_u64MoudleAddress + m_u64MoudleSize ))
						{
							ReadSize = DataSize;
							memcpy(ReadBuf,(PBYTE)ReadAddress,DataSize);
						}
						else
						{
							ReadSize =  ( m_u64MoudleAddress + m_u64MoudleSize -ReadAddress + 1);
							memcpy(ReadBuf,(PBYTE)ReadAddress,ReadSize);
						}
					}
				}
			}
		}
		*/
		return 0;
	}

	
	int GetHexType()
	{
		return m_HexType;
	}

private:
	int					m_HexType;
	UINT64				m_u64PointAddress;
	UINT64				m_u64MoudleAddress;
	UINT64				m_u64MoudleSize;
	
};

class CHexOtcDataView : public CDynamicView
{
public:
	class CVirtualSection
	{
		friend class CHexOtcDataView;
	public:
		CVirtualSection()
		{
			m_uRawAddress = 0;
			m_uRawSize = 0;
			m_uVirtualAddress = 0;
			m_uVirtualSize = 0;
		}

		~CVirtualSection()
		{

		}

	private:
		UINT64 m_uRawAddress;
		UINT64 m_uRawSize;
		UINT64 m_uVirtualAddress;
		UINT64 m_uVirtualSize;
	};

	CHexOtcDataView(void);
	virtual ~CHexOtcDataView(void);
	BOOL Init( HWND hWnd , HINSTANCE hInst);

	void SetHexDataAddress(UINT64 uAddress)
	{
		if (m_HexOctData)
		{
			m_HexOctData->SetHexDataAddress(uAddress);
		}
	}

	void SetMoudleBaseAddress(UINT64 BaseAddress,UINT64 MoudleSize)
	{
		if (m_HexOctData)
		{
			m_HexOctData->SetMoudleBaseAddress(BaseAddress,MoudleSize);
		}

		if (MoudleSize)
		{
			SetScrollRange(SB_VERT,0,MoudleSize/16 + 20,TRUE);
		}

		InvalidateRect();
	}

	void SetSectionCount(int nCount)
	{
		if (nCount)
		{
			m_Section = new CVirtualSection[nCount];
			if (m_Section)
			{
				m_SectionCount = nCount;
			}
		}
	}

	void SetSectionInfo(
		int nIndex,
		UINT64 uRa,
		UINT64 uRaSize,
		UINT64 uVa,
		UINT64 uVaSize)
	{
		if (nIndex < m_SectionCount)
		{
			m_Section[nIndex].m_uRawAddress = uRa;
			m_Section[nIndex].m_uRawSize = uRaSize;
			m_Section[nIndex].m_uVirtualAddress = uVa;
			m_Section[nIndex].m_uVirtualSize = uVaSize;
		}
	}

	void SetMode(BOOL bShowX64)
	{
		m_bShowX64 = bShowX64;
	}

	UINT64 GetVirtualOffset(UINT64 uAddress)
	{
		UINT64 uVirtualAress = uAddress;

		if (uAddress)
		{
			if( m_SectionCount)
			{
				UINT64 uRawAddress = 0;
				UINT64 uVaOffset = 0;

				for ( int nI = 0;nI < m_SectionCount;nI++)
				{
					if (m_Section)
					{
						if ( uAddress >= m_Section[nI].m_uRawAddress &&
							uAddress < ( m_Section[nI].m_uRawAddress + 
							( m_Section[nI].m_uVirtualSize > m_Section[nI].m_uRawSize?m_Section[nI].m_uVirtualSize:m_Section[nI].m_uRawSize)))
						{
							uRawAddress = m_Section[nI].m_uVirtualAddress;
							uVaOffset = (uAddress - m_Section[nI].m_uRawAddress);
							break;
						}

					}
				}

				if (uRawAddress )
				{
					uVirtualAress = (uRawAddress + uVaOffset);
				}
			}
		}

		return uVirtualAress;
	}
protected:
	virtual void OnPaintColumn(
						CMemDC& PaintDC,
						CRect& ClientRect,
						int Item,
						int Column,
						CDynamicDrawItem* DrawItemData,
						BOOL bSelected = FALSE);

	virtual CDynamicDrawItem* OnGetDrawData();
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw);
	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual void OnClickHeadDwon(int nColumn);
	virtual void OnClickHeadUp(int nColumn);
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);

	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();
	
private:
	CHexOctData*		m_HexOctData;
	CPen				m_HexLinePen;
	BOOL				m_bShowX64;
	int					m_SectionCount;
	CVirtualSection*	m_Section;
	CMenuT				m_Menu;
	CMenuT				m_BackMenu;
	BOOL				m_bShowAnsi;
	int					m_HexDataLen;
};

#endif