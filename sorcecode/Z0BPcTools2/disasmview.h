#ifndef _disasmview_h
#define _disasmview_h
#include "custom.h"
#include "struct.h"

#define JMP_ICON_CODE _T('-')
#define JMP_CALL_ICON '-'
#define FUNC_INER_ICON '.'
#define VIEW_NOTE_COLOR RGB(255,0,0)
#define FUNCTION_HEADER_COLOR RGB(0,0,0)
#define FUNCTION_BODY_COLOR RGB(0,0,0)
#define FUNCTION_HREADER_FLAGS '$'
#define FUCYION_HEADER_CHAR  _T("$")

#define FUCTION_BODY_HEADER_FLAGS 'c'
#define FUCTION_RET_FLAGS      'r'
#define FUNCTION_BODY_FLAGS '|'
#define JMP_OTHER_BODY_FLAGS 'j'
#define MAX_DISASM_LINES 1000000



class CDisasmView : public CCustomView
{
public:
	CDisasmView();
	~CDisasmView();

public:
	BOOL Init(HINSTANCE hInst,HWND hWnd);
	void SetScollLines( int nMaxLine );
protected:
	virtual void OnCustomPaintBody(
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
		int		PosVert );

	virtual HRESULT CustomRButtonDown( WPARAM wp,LPARAM lp );
	virtual HRESULT CustomLDoubleClick( WPARAM wp,LPARAM lp );
	virtual void CustomVkEnter();
	void PaintAsmColumn( 
		HDC hDC,
		PCustomItemHead Custom,
		RECT rcColumn,
		int nColumn,
		BOOL bSelect);

};


#endif