#ifndef _CFileHeaderView_H
#define _CFileHeaderView_H

#include "dynamicview.h"
#include "disassem.h"
#include "util.h"
#define MAX_DOS_HEADER_INFO_SIZE 200

class CWinExecFile
{
public:
	CWinExecFile()
	{

	}

	~CWinExecFile()
	{

	}

	void ShowSectionInfo(
		uint64_t& Address,
		LPTSTR HeaderName,
		ULONG Values,
		ULONG VaSize,
		const char* lpMagic,
		COLORREF ColorShow,
		LPTSTR lpNote,
		int nIconType1,
		int nIconType2,
		int nIconType3,
		BOOL bIsX64)
	{

		TCHAR szAddress[64] = {0};

		if (bIsX64)
		{
			StringCchPrintf(
				szAddress,
				64,
				_T("%016llX"),
				//HeaderName,
				Address);		
		}
		else
		{
			StringCchPrintf(
				szAddress,
				64,
				_T("%08X"),
				//HeaderName,
				Address);	
		}

		LPVOID pCurItem = m_ViewData.Insert(Address,szAddress);

		if (!pCurItem)
		{
			return;
		}	

		if (bIsX64)
		{
			StringCchPrintf( szAddress,64,_T(" RVA(%016X):RVS(%08X)"),Values,VaSize);
		}
		else
		{
			StringCchPrintf( szAddress,64,_T(" RVA(%08X):RVS(%08X)"),Values,VaSize);
		}


		m_ViewData.InsertHex(3,szAddress);
		m_ViewData.SetHexIcon(0,nIconType1,FUNCTION_HEADER_COLOR);
		m_ViewData.SetHexIcon(1,nIconType2,FUNCTION_HEADER_COLOR);
		m_ViewData.SetHexIcon(2,nIconType3,FUNCTION_HEADER_COLOR);

		Address += sizeof(IMAGE_SECTION_HEADER);

		tagOption * Option = new tagOption[1];
		if (!Option)
		{
			return;
		}

		ZeroMemory(Option,sizeof(tagOption));
		for ( int i = 0 ; i < 1 ; i++ )
		{
			Option[i].FontBgColor = -1;				
			Option[i].NorFontColor = ColorShow;
			Option[i].SelFontColor = ColorShow;


			Option[i].lpText = new TCHAR[strlen(lpMagic) + 1];
			if ( Option[i].lpText )
			{
				Option[i].TextSize = strlen(lpMagic);
				int j = 0;
				for (  ; j < Option[i].TextSize;j++)
				{
					Option[i].lpText[j] = lpMagic[j];
				}

				Option[i].lpText[j] = 0;
			}

			Option[i].HasNonFocus = FALSE;

		}

		m_ViewData.SetViewColumn(1,0,Option,1);		

		if (lpNote)
		{
			tagOption* pNoteOption = new tagOption[2];
			if (pNoteOption)
			{
				ZeroMemory(pNoteOption,sizeof(tagOption)*2);

				for ( int i = 0 ; i < 1 ; i++ )
				{
					pNoteOption[i].FontBgColor = -1;				
					pNoteOption[i].NorFontColor = VIEW_NOTE_COLOR;
					pNoteOption[i].SelFontColor = VIEW_NOTE_COLOR;

					pNoteOption[i].lpText = new TCHAR[lstrlen(lpNote) + 1];
					if ( pNoteOption[i].lpText )
					{
						StringCchCopy( pNoteOption[i].lpText,lstrlen(lpNote) + 1,lpNote );
						pNoteOption[i].TextSize = GetTextSize(lpNote);
					}

					pNoteOption[i].HasNonFocus = FALSE;

				}

				m_ViewData.SetViewColumn(2,0,pNoteOption,2);

			}
		}

	}


	void ShowDosHeaderInfo(
		uint64_t& Address,
		LPTSTR HeaderName,
		int Bit,
		UINT64 Values,
		LPTSTR lpMagic,
		COLORREF ColorShow,						
		LPTSTR lpNote,
		int nIconType1,
		int nIconType2,
		int nIconType3,
		BOOL bIsX64)
	{
		TCHAR szAddress[64] = {0};

		if (bIsX64)
		{
			StringCchPrintf(
				szAddress,
				64,
				_T("%016llX"),
				Address);	
		}
		else
		{
			StringCchPrintf(
				szAddress,
				64,
				_T("%08X"),
				Address);	
		}


		LPVOID pCurItem = m_ViewData.Insert(Address,szAddress);

		if (!pCurItem)
		{
			return;
		}

		if (Bit == 1)
		{
			StringCchPrintf( szAddress,64,_T(" %02X"),Values & 0xFF);
		}
		else if (Bit == 2 )
		{
			StringCchPrintf( szAddress,64,_T(" %04X"),Values & 0xFFFF);
		}
		else
		{
			if (bIsX64)
			{
				if (Bit == 4)
				{
					StringCchPrintf( szAddress,64,_T(" %08X"),Values & 0xFFFFFFFF);
				}
				else
				{
					StringCchPrintf( szAddress,64,_T(" %016llX"),Values);
				}
			}
			else
			{
				StringCchPrintf( szAddress,64,_T(" %08X"),Values);
			}

		}

		m_ViewData.InsertHex(3,szAddress);
		m_ViewData.SetHexIcon(0,nIconType1,FUNCTION_HEADER_COLOR);
		m_ViewData.SetHexIcon(1,nIconType2,FUNCTION_HEADER_COLOR);
		m_ViewData.SetHexIcon(2,nIconType3,FUNCTION_HEADER_COLOR);

		Address += Bit;

		tagOption * Option = new tagOption[1];
		if (!Option)
		{
			return;
		}

		ZeroMemory(Option,sizeof(tagOption));
		for ( int i = 0 ; i < 1 ; i++ )
		{
			Option[i].FontBgColor = -1;				
			Option[i].NorFontColor = ColorShow;
			Option[i].SelFontColor = ColorShow;


			Option[i].lpText = new TCHAR[lstrlen(lpMagic) + 1];
			if ( Option[i].lpText )
			{
				StringCchCopy( Option[i].lpText,lstrlen(lpMagic) + 1,lpMagic );
				Option[i].TextSize = GetTextSize(lpMagic);
			}

			Option[i].HasNonFocus = FALSE;

		}

		m_ViewData.SetViewColumn(1,0,Option,1);

		if (lpNote)
		{
			tagOption* pNoteOption = new tagOption[2];
			if (pNoteOption)
			{
				ZeroMemory(pNoteOption,sizeof(tagOption)*2);

				for ( int i = 0 ; i < 1 ; i++ )
				{
					pNoteOption[i].FontBgColor = -1;				
					pNoteOption[i].NorFontColor = VIEW_NOTE_COLOR;
					pNoteOption[i].SelFontColor = VIEW_NOTE_COLOR;

					pNoteOption[i].lpText = new TCHAR[lstrlen(lpNote) + 1];
					if ( pNoteOption[i].lpText )
					{
						StringCchCopy( pNoteOption[i].lpText,lstrlen(lpNote) + 1,lpNote );
						pNoteOption[i].TextSize = GetTextSize(lpNote);
					}

					pNoteOption[i].HasNonFocus = FALSE;

				}

				m_ViewData.SetViewColumn(2,0,pNoteOption,2);
			}		
		}

	}

	CViewData* GetViewData()
	{
		return &m_ViewData;
	}

private:
	CViewData	m_ViewData;
};
class CDosHeaderData
{
public:
	CDosHeaderData()
		:m_WinExtFile(NULL)
		,m_pFileBuffer(NULL)
		,m_FileBufferSize(NULL)
	{
		m_WinExtFile = new CWinExecFile[MAX_DOS_HEADER_INFO_SIZE];
	}

	CDosHeaderData(
		PBYTE			pFileBuffer,
		uint64_t		FileBufferSize)
	{
		m_WinExtFile = new CWinExecFile[MAX_DOS_HEADER_INFO_SIZE];
		m_pFileBuffer = pFileBuffer;
		m_FileBufferSize = FileBufferSize;
	}

	~CDosHeaderData()
	{
		if (m_WinExtFile)
		{
			delete[] m_WinExtFile;
			m_WinExtFile = NULL;
		}
	}

	BOOL GetWinExtFileInfo(LPTSTR m_FileName);

	CViewData* GetViewData(int nI)
	{
		if (m_WinExtFile)
		{
			if (nI < MAX_DOS_HEADER_INFO_SIZE)
			{
				return m_WinExtFile[nI].GetViewData();
			}			
		}

		return NULL;
	}

private:
	CWinExecFile*	m_WinExtFile;

	PBYTE			m_pFileBuffer;
	uint64_t		m_FileBufferSize;

};

class CFileHeaderView : public CDynamicView
{
public:
	CFileHeaderView(void);
	virtual ~CFileHeaderView(void);

	BOOL Init( HWND hWnd , HINSTANCE hInst);
protected:
	virtual void OnPaintColumn(
		CMemDC& PaintDC,
		CRect& ClientRect,
		int Item,
		int Column,
		CDynamicDrawItem* DrawItemData,
		BOOL bSelected = FALSE);
	void PaintColumn(	
		CMemDC& PaintDC,
		CRect& rcColumn,
		CViewData* CustomItem,
		int nColumn,
		BOOL bSelect);

	virtual CDynamicDrawItem* OnGetDrawData();
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw);

	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);

	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();
public:
	inline void SetWinExtFileData(
		PBYTE			pFileBuffer,
		uint64_t		FileBufferSize,
		LPTSTR			FileName)
	{
		m_FileBufferSize = FileBufferSize;
		m_pFileBuffer = pFileBuffer;
		if (FileName)
		{
			int Length = (lstrlen(FileName) + 1);
			m_FileName = new TCHAR[Length];
			if (m_FileName)
			{
				StringCchCopy(m_FileName,Length+1,FileName);
			}
		}

		InvalidateRect();
	}

	UINT64& FirstSelect()
	{
		return m_FSelectItem;
	}

	UINT64& LastSelect()
	{
		return m_LSelectItem;
	}

private:
	PBYTE			m_pFileBuffer;
	uint64_t		m_FileBufferSize;
	LPTSTR			m_FileName;
	__int64			m_nPointData;

	UINT64			m_FSelectItem;
	UINT64			m_LSelectItem;


};

#endif