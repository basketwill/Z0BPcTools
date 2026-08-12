#ifndef _TOOLBAR_H
#define _TOOLBAR_H

#define BITMAP_ARRAY_LIST 25
#define BITMAP_PEN_LIST 2
typedef enum _tagBitmapTool
{
	kToolBarOpenFile		= 0,
	kToolBarRestart			= 1,
	kToolBarCloseDebug		= 2,
	kToolBarDebugRun		= 3,
	kToolBarRunCurThread	= 4,
	kToolBarDebugPause		= 5,
	kToolBarStepInto		= 6,
	kToolBarStepOver		= 7,
	kToolBarTraceInto		= 8,
	kToolBarTraceOver		= 9,
	kToolBarTraceLog		= 10,
	kToolBarTraceModules	= 11,
	kToolBarTraceMemMap		= 12,
	kToolBarTraceWinHandle	= 13,
	kToolBarTraceThreads	= 14,
	kToolBarTraceSysHandle	= 15,
	kToolBarTraceCpuView	= 16,
	kToolBarTraceAsmPatch	= 17,
	kToolBarTraceCpuStack	= 18,
	kToolBarTraceRefers		= 19,
	kToolBarTraceSource		= 20,
	kToolBarTraceBpoints	= 21,
	kToolBarTraceMemBreak	= 22,
	kToolBarTraceHardBreak	= 23,
	
	kToolBarTraceOptions	= 24,
	kToolBarButtonMAx		= 25,
	kToolBarWhitePen		= 0,
	kToolBarBlackPen		= 1,
}tagBitmapTool;

typedef struct _tagToolBarHitPos
{
	BOOL		LButtonDown;
	RECT		ActiveRegion;
	int			ActiveButton;
}tagToolBarHitPos,ToolBarHitPos;

class CToolBar;
typedef int (CToolBar::*pButtonEvent)(WPARAM,LPARAM);

class CToolBar
{
public:
	
	CToolBar();
	~CToolBar();
	operator HWND();
	BOOL Init( HWND hWnd,HINSTANCE hInst);

public:
	static
	LRESULT
	WINAPI
	WinProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

	void CallBackEvent( tagBitmapTool EventIndex ,WPARAM wParam,LPARAM lParam );
protected:
	void OnPaint(HDC hDC, RECT* prcDirty, BOOL bErase);
	void OnSize(WPARAM wParam,LPARAM lParam);
	void OnLButtonUp(WPARAM wParam,LPARAM lParam);
	void OnLButtonDown(WPARAM wParam,LPARAM lParam);
	void OnLButtonOpenFile(WPARAM wParam,LPARAM lParam);
	void OnLButtonRestart(WPARAM wParam,LPARAM lParam);
	void OnLButtonCloseDebug(WPARAM wParam,LPARAM lParam);
	void OnLButtonDebugRun(WPARAM wParam,LPARAM lParam);
	void OnLButtonRunCurThread(WPARAM wParam,LPARAM lParam);
	void OnLButtonDebugPause(WPARAM wParam,LPARAM lParam);
	void OnLButtonStepInto(WPARAM wParam,LPARAM lParam);

	void OnLButtonStepOver(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceInto(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceOver(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceLog(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceModules(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceMemMap(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceWinHandle(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceThreads(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceSysHandle(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceCpuView(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceAsmPatch(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceCpuStack(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceRefers(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceSource(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceBpoints(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceMemBreak(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceHardBreak(WPARAM wParam,LPARAM lParam);
	void OnLButtonTraceOptions(WPARAM wParam,LPARAM lParam);
	
private:
	HWND				m_hWnd;
	HBITMAP				m_BitmapArray[BITMAP_ARRAY_LIST];
	RECT				m_BitmapRect[BITMAP_ARRAY_LIST];

	HPEN				m_PenArray[BITMAP_PEN_LIST];

	HBRUSH				m_BrushArray[BITMAP_PEN_LIST];
	HBRUSH				m_BrushBkg;
	tagToolBarHitPos	m_ToolHitPos;

	int					m_nSelectButton;
	pButtonEvent		m_ButtonEvent[BITMAP_ARRAY_LIST];
protected:

	void DrawButton(int nIndex,HDC& PaintDc ,HDC  hdcMem,BOOL IsButtonDown = FALSE );
	void LoadButton(HINSTANCE hInst,int nIndex,int nId);
	void ResetButton(int nIndex,int TopShow,int nGap,int bmHeight,int bmWidth );
};

#endif