#ifndef _IDA_VIEW_H
#define _IDA_VIEW_H

#include "disasmview.h"
#include "symbol.h"
#include "ReverseView.h"
#include <stdint.h>
#include "cstack.h"
#include "HexOtcDataView.h"
#include "FileHeaderView.h"

typedef struct _tagSkipJmpTable
{
	PCustomItemHead SelItem;
	uint64_t		ptrJmpAddress;
	BOOL			JmpType;
	_tagSkipJmpTable* _mNext;
	_tagSkipJmpTable* _mPrev;
}tagSkipJmpTable;


class CFileInfo
{
public:
	CFileInfo()
	{
		m_pFileBuffer = NULL;
		m_FileBufferSize = 0;
	}

	~CFileInfo()
	{
		if (m_pFileBuffer)
		{
			delete[] m_pFileBuffer;
			m_pFileBuffer = NULL;
		}
	}

	BOOL ReadFile( LPTSTR lpFileName )
	{
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
			FileSize.LowPart = ::GetFileSize(hFile,(LPDWORD)&FileSize.HighPart);

			if (FileSize.LowPart)
			{
				::SetFilePointer(hFile,0,0,FILE_BEGIN);
				PBYTE FileBytes = new BYTE[FileSize.LowPart];
				if (FileBytes)
				{
					ULONG ReadLength = 0;
					if (::ReadFile(hFile,FileBytes,FileSize.LowPart,&ReadLength,NULL))
					{
						m_pFileBuffer = FileBytes;
						m_FileBufferSize = FileSize.QuadPart;
						::CloseHandle(hFile);

						return TRUE;									
					}

					delete[] FileBytes;
					FileBytes = NULL;
				}
			}

			::CloseHandle(hFile);
		}

		return FALSE;
	}

	void GetFileData(LPBYTE& lpBase,uint64_t& lpFileSize)
	{
		lpBase = m_pFileBuffer;
		lpFileSize = m_FileBufferSize;
	}

	void GetDosHeader()
	{
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PBYTE pBase = m_pFileBuffer ;
		int FileSize = m_FileBufferSize;
		
		if ( FileSize > ( sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS))&& pBase )
		{
			BOOL IsAmd64 = FALSE;
			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBase;
			if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE )
			{
				pNtHeader = (PIMAGE_NT_HEADERS)&((const unsigned char *)(pBase))[pDosHeader->e_lfanew];
				if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
				{
					ULONG_PTR nAddress = 0;
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
				}
			}
		}
	}

	BOOL IsAmdX64()
	{
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PBYTE pBase = m_pFileBuffer ;
		int FileSize = m_FileBufferSize;
		BOOL IsAmd64 = FALSE;
		if ( FileSize > ( sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS))&& pBase )
		{			
			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBase;
			if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE )
			{
				pNtHeader = (PIMAGE_NT_HEADERS)&((const unsigned char *)(pBase))[pDosHeader->e_lfanew];
				if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
				{
					ULONG_PTR nAddress = 0;
					PIMAGE_FILE_HEADER pFileHeader = &pNtHeader->FileHeader;
					
					if( pFileHeader &&(pFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64 ||
						pFileHeader->Machine == IMAGE_FILE_MACHINE_IA64))
					{
						IsAmd64 = TRUE;
					}
				}
			}
		}

		return IsAmd64;
	}

	BOOL IsPeFile()
	{
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PBYTE pBase = m_pFileBuffer ;
		int FileSize = m_FileBufferSize;
		BOOL IsAmd64 = FALSE;
		if ( FileSize > ( sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS))&& pBase )
		{			
			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBase;
			if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE )
			{
				pNtHeader = (PIMAGE_NT_HEADERS)&((const unsigned char *)(pBase))[pDosHeader->e_lfanew];
				if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
				{
					return TRUE;
				}
			}
		}

		return FALSE;
	}

private:
	PBYTE		m_pFileBuffer;
	uint64_t	m_FileBufferSize;
};

class CIdaView : public CClassViewBase
{
public:

	void operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	operator LPTSTR()
	{
		return m_FilePath;
	}

	void operator =(HINSTANCE hWnd)
	{
		m_hInst = hWnd;
	}

	CIdaView();
	~CIdaView();

 	virtual CDynamicView* getAsmView()
 	{
 		return (CDynamicView*)&m_ReverseView;
 	}

	static CIdaView* Init(HINSTANCE hInst,HWND hWnd,LPTSTR szClass,LPTSTR szTitle,UINT Id );
	static LRESULT WINAPI DefMdiWinProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
	static const LPTSTR ResolverSymbol(struct ud*,	uint64_t addr,int64_t *offset);

	static void* ReloveSkipJmp( struct ud& ud_obj, void* cur_node,uint64_t skip_jmp, int JmpType = 0);
public:
	void ShowIdaView(LPTSTR PathFile);
	const LPTSTR GetSymbol(	uint64_t addr,int64_t *offset);
	void* ReloveSkipJmp( void* cur_node,uint64_t skip_jmp,int JmpType);
	//void SetSymbol
protected:
	HWND							m_hWnd;
	HINSTANCE						m_hInst;
	//CDisasmView					m_pDisAsmView;
	CReverseView					m_ReverseView;
	CFileHeaderView					m_FileHeaderView;
	CHexOtcDataView					m_HexOtcDataView;

	CFileInfo						m_FileInfo;
	HBRUSH							m_BackLineBrush;
	HBRUSH							m_SelectLineBrush;

	CSymbol							m_Symbol;
	CStack<tagSkipJmpTable>			m_SkipJmpTable;
	TCHAR							m_FilePath[MAX_PATH];

	ViewPos							m_ViewPos[MAX_ROOM];
	MousePos						m_MousePos;
	
	BOOL							m_LButtonDown;
	BOOL							m_bIsClose;
protected:
	BOOL OnViewActive( HWND hWnd );
	void OnViewPaint( HDC hDC, RECT* prcDirty, BOOL bErase);
	void OnViewSize(LPARAM lParam,WPARAM wParam);
	void OnViewMouseMove(LPARAM lParam,WPARAM wParam);
	void OnViewMouseLeave(LPARAM lParam,WPARAM wParam);
	void OnViewLButtonUp(LPARAM lParam,WPARAM wParam);
	void OnViewLButtonDown(LPARAM lParam,WPARAM wParam);
	static		
	UINT WINAPI ShowPeFileIdaInfo(void* pIdaView);
protected:
	BOOL ShowFileIdaInfo(LPTSTR FileName);
	void ShowDosHeaderInfo(
					ULONG_PTR& Address,
					LPTSTR HeaderName,
					int Bit,
					ULONG Values,
					LPTSTR lpMagic,
					COLORREF ColorShow,					
					LPTSTR lpNote = NULL,
					int nIconType1 = 0,
					int nIconType2 = 0,
					int nIconType3 = 0,
					BOOL bIsX64 = FALSE);
	void ShowSectionInfo(
				ULONG_PTR& Address,
				LPTSTR HeaderName,
				ULONG Values,
				ULONG VaSize,
				const char* lpMagic,
				COLORREF ColorShow,
				LPTSTR lpNote = NULL,
				int nIconType1 = 0,
				int nIconType2 = 0,
				int nIconType3 = 0,
				BOOL bIsX64 = FALSE);
	void SetSymbol( uint64_t u64Ptr,LPTSTR SymbolName);
	void AddSymbolMoudle(uint64_t u64ptr,uint64_t uSize , LPTSTR lpMoudleName);

	void GetImportTable(PBYTE pBase,uint64_t ImageBase,uint64_t ImportTableRva,PIMAGE_SECTION_HEADER pSection,int SectionNums,BOOL bIsX64 = FALSE);
	void GetRelocationTable(PBYTE pBase,uint64_t ImageBase,uint64_t RelocationRva,PIMAGE_SECTION_HEADER pSection,int SectionNums,BOOL bIsX64 = FALSE);
	uint64_t GetRwaDataOffset( PIMAGE_SECTION_HEADER pSection , int SectionNums, uint64_t VirtualAddr );
	uint64_t GetVirtualOffset( PIMAGE_SECTION_HEADER pSection, int SectionNums, uint64_t RawAddr );

	HCURSOR CpuViewTraceLoadCursor(POINT& MousePt,ULONG& TraceViewLine);
};
#endif