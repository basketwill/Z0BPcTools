#ifndef _disassem_h
#define _disassem_h
#include "cpuview.h"

#include "../udis86/udis86.h"
#include "../udis86/libudis86/types.h"
#include "../udis86/libudis86/extern.h"

#define MAX_LPTSTR_LIST_SIZE 30

#define RET_CALL_BK_INS_COLOR RGB(0,255,255)
#define JL_BK_INS_COLOR RGB(255,255,0)
#define JL_FONT_INS_COLOR RGB(255,0,0)
#define PUSH_POP_FONT_INS_COLOR RGB(0,0,255)
#define OP_IMM_FONT_COLOR  RGB(128,128,0)
#define OP_MEM_BK_COLOR RGB(0,255,255)
#define OP_MEM_FONT_DATA_COLOR RGB(0,0,128)
#define SECTION_NAME_SHOW_COLOR RGB(0,68,0)
#define FILE_HEADER_SHOW_COLOR RGB(89,89,0)
#define OPTION_HEADER_SHOW_COLOR RGB(0,0,145)
#define CALL_IMPORT_ADDRESS_COLOR RGB(255,0,0)

typedef enum _tagInstructionType
{
	kInstructionUnkonw	= 0x0,
	kInstructionCall	= 0x1,
	kInstructionJmp		= 0x2,
	kInstructionMov		= 0x3,
	kInstructionRet		= 0x4,
	kInstructionLea		= 0x5,
	kInstructionPush	= 0x6,
	kInstructionPop		= 0x7,
	kInstructionAdd		= 0x8,
	kInstructionDec		= 0x9,
	kInstructionInt1	= 0xA,
	kInstructionInt2	= 0xB,
	kInstructionInt3	= 0xC,
	kInstructionInt		= 0xD,
	kInstructionNop		= 0xE,
	kInstruction0000	= 0xF,
	kInstructionInvalid = 0x10,
};

typedef enum _JMP_TYPE
{
	kJmpOther = 0,
	kCallFar  = 0x10,
	kRetType  = 0x20,
}JMP_TYPE;

typedef struct _tagDisBufferInfo
{
	COLORREF				FontBgColor;//字体背景颜色
	COLORREF				NorFontColor;//正常显示颜色
	COLORREF				SelFontColor;//正常显示颜色

	LPTSTR					lpText;
}tagDisBufferInfo;

typedef void* (*relove_skip_jmp)( ud_t& ud_obj , void* cur_node,uint64_t skip_jmp,int JmpType);


class CViewHead
{
public:
	CViewHead()
	{
		ZeroMemory(this,sizeof(*this));
	}

	~CViewHead()
	{
		if (m_szText)
		{
			delete[] m_szText;
			m_szText = NULL;
		}
	}

	LPVOID Insert(uint64_t uOffset,LPTSTR szText);
	void	SetInstructionType( uint8_t InsType);

	LPTSTR GetText()
	{
		return m_szText;
	}

	COLORREF GetNonFontColor()
	{
		return m_NoneFocusFontColor;
	}

	COLORREF GetSelFontColor()
	{
		return m_SelFontColor;
	}

	COLORREF GetNorFontColor()
	{
		return m_NorFontColor;
	}

private:
	uint8_t					m_uInstrType;
	uint64_t				m_u64Address;
	COLORREF				m_NoneFocusFontColor;//非聚焦显示颜色
	COLORREF				m_NorFontColor;//正常显示颜色
	COLORREF				m_SelFontColor;//正常显示颜色

	HBRUSH					m_SelBgkBrush;//选择后显示颜色
	HBRUSH					m_NorBgkBrush;// 背景颜色
	LPTSTR					m_szText;

};

class CViewColumn
{
public:
	CViewColumn()
	{
		ZeroMemory(this,sizeof(*this));
	}

	~CViewColumn()
	{
		if (m_Option)
		{
			for ( int nI = 0; nI < m_OptionCount;nI++)
			{
				if (m_Option[nI].lpText)
				{
					delete[] m_Option[nI].lpText;
					m_Option[nI].lpText = NULL;
				}
			}

			delete[] m_Option;
			m_Option = NULL;
		}

		if ( m_Icon)
		{
// 			for ( int nI = 0; nI < m_IconCount;nI++)
// 			{
// 				if (m_Icon[nI].AdditionData)
// 				{
// 					delete[] m_Icon[nI].AdditionData;
// 					m_Icon[nI].AdditionData = NULL;
// 				}
// 			}

			delete[] m_Icon;
			m_Icon = NULL;
		}

		ZeroMemory(this,sizeof(*this));
	}

	void    InsertHex( int IconNums,LPTSTR lpText );
	void	SetHexIcon(int IconNums,int nIconType,int nIconType2,int nIconType3 );
	void    SetViewColumn(
					int IconNums,
					tagOption* Option,
					int nSize );

	LPTSTR GetOptionText( int nI)
	{
		if (nI < m_OptionCount)
		{
			if (m_Option)
			{
				return m_Option[nI].lpText;
			}
		}
		return NULL;
	}

	tagIcon* GetIcon()
	{
		return m_Icon;
	}

	int GetIconCount()
	{
		return m_IconCount;
	}

	tagOption* GetOption()
	{
		return m_Option;
	}

	int GetOptionCount()
	{
		return m_OptionCount;
	}

private:

	tagOption*				m_Option;
	int						m_OptionCount;	

	tagIcon*				m_Icon;
	int						m_IconCount;
	
	HBRUSH					m_SelBgkBrush;//选择后显示颜色
	HBRUSH					m_NorBgkBrush;// 背景颜色

};

class CViewData
{
public:
	CViewData();
	~CViewData();
public:
	LPVOID		Insert(uint64_t uOffset,LPTSTR szText);
	void		SetInstructionType( uint8_t InsType);

	void		InsertHex( int IconNums,LPTSTR lpText );
	void		SetHexIcon(int IconNums,int nIconType,int nIconType2,int nIconType3 = 0 );
	void    SetViewColumn(
					int nColumn,
					int IconNums,
					tagOption* Option,
					int nSize );

	LPTSTR GetHeadText()
	{
		return m_ViewHead.GetText();
	}

	COLORREF GetHeadNonFontColor()
	{
		return m_ViewHead.GetNonFontColor();
	}

	COLORREF GetHeadSelFontColor()
	{
		return m_ViewHead.GetSelFontColor();
	}

	COLORREF GetHeadNorFontColor()
	{
		return m_ViewHead.GetNorFontColor();
	}

	tagIcon* GetColumnIcon( int nColumn );
	int		 GetColumnIconCount(int nColumn);

	tagOption* GetColumnOption( int nColumn );
	int		 GetColumnOptionCount(int nColumn);
private:
	CViewHead	m_ViewHead;
	CViewColumn	m_ViewColumn[CUSTOM_MAX_COLUMN];
	
};

class CZ0BTDisasm
{
public:
	CZ0BTDisasm();
	~CZ0BTDisasm();
	void SetResolverSymbol(const LPTSTR (*resolver)(struct ud*, 
								uint64_t addr,
							int64_t *offset),void* userdata);

	void SetResolverSkipjmp(void* (*relove_skip_jmp)( struct ud& ud_obj , void* cur_node,uint64_t skip_jmp, int JmpType),void* userdata);
	void SetMode(int mode);
	void SetMoudleInfo( uint64_t moudle_base,uint64_t moudle_size);
	BOOL Disassem( LPBYTE Start ,uint64_t DisSize,uint64_t Offset,CClassViewBase* pView,LPTSTR lpSectionName = NULL);
	BOOL DisassemIda( LPBYTE Start ,ULONG_PTR DisSize,CClassViewBase* pView);

	BOOL DisasmBuffer( 
				LPBYTE pBuffer,
				uint64_t BufSize,
				uint64_t Offset,
				BOOL bIsX64,
				LPTSTR lpSectionName = NULL);
	BOOL ParseAsm(ud_t& ud,tagOption*& Option,int& nSize);

	BOOL DisassenCopy(LPTSTR DstBuf,const char* AsmBuf);

	int  GetAsmBufCount(const char* AsmBuf,LPTSTR*& DstBuf);

	int  GetAsmBufCount(ud_t& ud,tagDisBufferInfo*& DstBuf);
	CViewData* GetView()
	{
		return ud_pView;
	}

	void ReleaseViewData()
	{
		if (ud_pView)
		{
			delete ud_pView;
			ud_pView = NULL;
		}
	}

private:
	ud_t		    ud_obj;
	uint64_t	    ud_moudle_base;
	uint64_t        ud_moudle_size;
	CViewData*		ud_pView;
	relove_skip_jmp ud_relove_skip_jmp;
};
#endif