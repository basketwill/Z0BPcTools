#ifndef _REVERSE_VIEW_H
#define _REVERSE_VIEW_H

#include "dynamicview.h"
#include "../udis86/udis86.h"
#include "../udis86/libudis86/types.h"
#include "../udis86/libudis86/extern.h"
#include "disassem.h"
#include <math.h>

#define DEFAULT_REVERSE_PAGE_SIZE 0x1000*8

#define MAX_REVERSE_DATA_NUMS	200

typedef const LPTSTR (*presolver)(struct ud*, uint64_t addr,	int64_t *offset);

typedef enum _tagEnumReverseColumn
{
	kReverseAddress		= 0,
	kReverseHexData		= 1,
	kReverseDisasm		= 2,
	kReverseNote		= 3,

	kReverseIdaMode		= 0,
	kReverseOllyDbgMode = 1,

	kReversePeHeader    = 0x1,
	kReversePeCode		= 0x2,
	kReversePeData		= 0x3,

	kReverseReadFile	= 0x1,
	kReverseReadProcess = 0x2,
	kReverseReadFromMem = 0x3,
};

#pragma pack(push,1)

class CReverseView;

class CVirtualSection
{
	friend class CReverseData;
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
	uint64_t m_uRawAddress;
	uint64_t m_uRawSize;
	uint64_t m_uVirtualAddress;
	uint64_t m_uVirtualSize;
};

class CReverseDraw;
class CReverseData
{
public:

	struct CDataItem
	{
		friend class CReverseData;
	public:
		CDataItem()
			//:m_Type(kReversePeCode)
			:m_IntsrPtr(NULL)
			//,m_InstrSize(0)
			,m_Next(0)
			,m_Pre(0)
		{

		}

		~CDataItem()
		{

		}

		CDataItem(const CDataItem& Data)
		{
			//m_Type = Data.m_Type;
			m_IntsrPtr = Data.m_IntsrPtr;
			//m_InstrSize = Data.m_InstrSize;
			m_Next = Data.m_Next;
			m_Pre  = Data.m_Pre;
		}

		CDataItem* Next()
		{
			return m_Next;
		}

		CDataItem* Pre()
		{
			return m_Pre;
		}

		uint64_t GetInstrPtr()
		{
			return m_IntsrPtr;
		}

	private:
		//BYTE			m_Type;
		uint64_t		m_IntsrPtr;
		//uint64_t		m_RealIntsr;
		//unsigned char	m_InstrSize;
		CDataItem*		m_Next;
		CDataItem*		m_Pre;
	};

	CReverseData() 
		:m_DataItem(NULL)
		,m_FirstSelect(NULL)
		,m_NextSelect(NULL)
		,m_DataCursor(NULL)
		,m_InstrPtr(NULL)
		,m_InstrSize(0)
		,m_InstrCount(0)
		,m_TrailDataItem(NULL)
		,m_SectionCount(0)
		,m_Section(NULL)
	{
		
	}

	CReverseData(BYTE* Instr,const ULONG64 IntsrSize) 
		:m_DataItem(NULL)
		,m_FirstSelect(NULL)
		,m_NextSelect(NULL)
		,m_DataCursor(NULL)
		,m_InstrPtr(NULL)
		,m_InstrSize(0)
		,m_InstrCount(0)
	{
		m_InstrPtr = Instr;
		m_InstrSize = IntsrSize;
	}

	~CReverseData()
	{
		if (m_Section)
		{
			delete[] m_Section;
			m_Section = NULL;
		}

		CDataItem*	pDataItem = m_DataItem;
		while ( pDataItem )
		{
			m_DataItem = m_DataItem->Next();

			delete pDataItem;
			pDataItem = m_DataItem;
		}
	}

public:

	CDataItem* NextData()
	{
		if (m_DataCursor)
		{
			return m_DataCursor->m_Next;
		}

		return NULL;
		
	}

	
	BOOL DecodeFromMem(
					CReverseView* pView,
					PBYTE pBuffer,
					uint64_t cBufferSize,
					float  flTotle,
					uint64_t VirtualAddress,
					BOOL	bReNew,
					BOOL	IsAmd64);

	BOOL GetRawData(  void*& lpData,int& lpSize )
	{
		lpData = 0;
		lpSize = 0;
		if (m_InstrPtr && m_InstrSize)
		{
			lpData = m_InstrPtr;
			lpSize = m_InstrSize;
		}

		return TRUE;
	}

	BOOL ReadData( void* lpData,uint64_t FileSize,int ReadType)
	{
		if (lpData && ReadType)
		{
			switch(ReadType)
			{
			case kReverseReadFile:
				{
					LPTSTR lpFileName = (LPTSTR)lpData;
					HANDLE hFile = CreateFile(
										lpFileName,
										GENERIC_READ, 
										FILE_SHARE_READ,
										NULL, 
										OPEN_EXISTING, 
										NULL, 
										NULL);

					if (hFile != INVALID_HANDLE_VALUE)
					{
						LARGE_INTEGER FileSize;
						FileSize.LowPart = GetFileSize(hFile,(LPDWORD)&FileSize.HighPart);

						if (FileSize.LowPart)
						{
							SetFilePointer(hFile,0,0,FILE_BEGIN);
							PBYTE FileBytes = new BYTE[FileSize.LowPart];
							if (FileBytes)
							{
								ULONG ReadLength = 0;
								if (ReadFile(hFile,FileBytes,FileSize.LowPart,&ReadLength,NULL))
								{
									m_InstrPtr = FileBytes;
									m_InstrSize = FileSize.QuadPart;
									CloseHandle(hFile);

									return TRUE;									
								}

							}
						}
						CloseHandle(hFile);
					}
				}
				break;
			case kReverseReadProcess:
				break;
			case kReverseReadFromMem:
				{
					if (lpData && FileSize)
					{
						m_InstrPtr = (PBYTE)lpData;
						m_InstrSize = FileSize;
					}
				}
				break;
			}
		}
		return FALSE;
	}


	ULONG64 GetInstrCount()
	{
		return m_InstrCount;
	}

	CDataItem*& FirstCursor()
	{
		return m_DataCursor;
	}

	CDataItem*& FSlectItem()
	{
		return m_FirstSelect;
	}

	CDataItem*& LSlectItem()
	{
		return m_NextSelect;
	}

	int ReadProcessData( 
				uint64_t InstrOffset ,
				BYTE* pInstr,
				uint64_t ReadSize = 30)
	{
		int64_t Offset = (InstrOffset - m_DataBaseValues);
		uint64_t CopySize = ReadSize;
		if (Offset > 0 && pInstr)
		{
			if( m_SectionCount)
			{
				uint64_t uRawAddress = 0;
				uint64_t uRawSize = 0;
				uint64_t uVaOffset = 0;

				for ( int nI = 0;nI < m_SectionCount;nI++)
				{
					if (m_Section)
					{
						if ( InstrOffset >= m_Section[nI].m_uVirtualAddress &&
							InstrOffset < ( m_Section[nI].m_uVirtualAddress + 
							( m_Section[nI].m_uVirtualSize > m_Section[nI].m_uRawSize?m_Section[nI].m_uVirtualSize:m_Section[nI].m_uRawSize)))
						{
							uRawAddress = m_Section[nI].m_uRawAddress;
							uRawSize = m_Section[nI].m_uRawSize;
							uVaOffset = (InstrOffset - m_Section[nI].m_uVirtualAddress);
							break;
						}

					}
				}

				if (uRawAddress && uRawSize /*&& uVaOffset < uRawSize*/)
				{
					uRawAddress = uRawAddress + uVaOffset;
					CopySize = abs( (__int64)uRawSize - (__int64)uVaOffset);
					memcpy(pInstr,(const void*)uRawAddress, CopySize >= 30?30:CopySize );

					return uRawSize;
				}
			}
		}
		
		return 0;
	}

	void SetImageBase(
				uint64_t ImageBase)
	{
		m_DataBaseValues = ImageBase;
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
		uint64_t uRa,
		uint64_t uRaSize,
		uint64_t uVa,
		uint64_t uVaSize)
	{
		if (nIndex < m_SectionCount)
		{
			m_Section[nIndex].m_uRawAddress = uRa;
			m_Section[nIndex].m_uRawSize = uRaSize;
			m_Section[nIndex].m_uVirtualAddress = uVa;
			m_Section[nIndex].m_uVirtualSize = uVaSize;
		}
	}

	
private:
	BYTE*				m_InstrPtr;	
	uint64_t			m_InstrSize;
	uint64_t			m_InstrCount;
	uint64_t			m_DataBaseValues;

	CDataItem*			m_DataItem;
	CDataItem*			m_TrailDataItem;

	CDataItem*			m_DataCursor;
	CDataItem*			m_FirstSelect;
	CDataItem*			m_NextSelect;

	int					m_SectionCount;
	CVirtualSection*	m_Section;
};

typedef struct _tagReverseDisasm
{
	CZ0BTDisasm	Z0bDisam;
}ReverseDisasm;

class CReverseDraw
{
	friend class CReverseDraw;
	friend class CReverseView;
public:
	CReverseDraw()
		:m_DataCursor(NULL)
	{
		//ud_init(&m_ReverseDisasm.ud_obj);
	}

	~CReverseDraw()
	{

	}

	uint64_t GetInstrOffset()
	{
		if (m_DataCursor)
		{
			return m_DataCursor->GetInstrPtr();
		}

		return -1;

	}

private:
	CReverseData::CDataItem*	m_DataCursor;
	ReverseDisasm				m_ReverseDisasm;
};

#pragma pack(pop)

class CReverseView : public CDynamicView
{
public:
	CReverseView(void);
	virtual ~CReverseView(void);
public:
	BOOL Init( HWND hWnd , HINSTANCE hInst , int ReverseMode = kReverseIdaMode);
	BOOL ReadData( void* lpData,uint64_t lpSize,int ReadType);
	BOOL GetRawData(  void*& lpData,int& lpSize );
	BOOL Decode(BOOL bIsAmd64 = FALSE);

	BOOL DecodeFromMem(PBYTE pBuffer, uint64_t cBufferSize,float  flTotle,uint64_t VirtualOffset,BOOL	bReNew);

	void SetMode(int bit )
	{
		if (bit == 64)
		{
			m_ReverseAmd64 = TRUE;
		}
		else
		{
			m_ReverseAmd64 = FALSE;
		}
	}

	void SetResolverSymbol(const LPTSTR (*resolver)(struct ud*, 
		uint64_t addr,
		int64_t *offset),void* userdata)
	{
		m_pResolver = resolver;
		m_Userdata = userdata;
	}

	void SetImageBase(
		uint64_t ImageBase)
	{
		m_ReverseData.SetImageBase(ImageBase);
	}

	void SetSectionCount(int nCount)
	{
		m_ReverseData.SetSectionCount(nCount);
	}

	void SetSectionInfo(
					int nIndex,
					uint64_t uRa,
					uint64_t uRaSize,
					uint64_t uVa,
					uint64_t uVaSize)
	{
		m_ReverseData.SetSectionInfo(nIndex,uRa,uRaSize,uVa,uVaSize);
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
	virtual int	 OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect);
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);
	virtual void OnClickItemUp();
	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();
	void PaintAsmColumn(	
				CMemDC& PaintDC,
				CRect& rcColumn,
				CViewData* CustomItem,
				int nColumn,
				BOOL bSelect);
private:
	int				 m_ReverseMode;
	CReverseData	 m_ReverseData;
	CReverseDraw*	 m_pDrawItem;
	BOOL			 m_ReverseAmd64;
		
	presolver		m_pResolver;
	void*			m_Userdata;

};

#endif