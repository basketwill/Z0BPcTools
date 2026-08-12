#ifndef _CMD_BAR_H
#define _CMD_BAR_H
#include "winbase.h"

class CCmdBar
{
public:
	CCmdBar();
	~CCmdBar();

	operator HWND();
	BOOL Init( HWND hWnd,HINSTANCE hInst);
	void OnSize(WPARAM wParam,LPARAM lParam);
	void OnPaint(HDC hDC, RECT* prcDirty, BOOL bErase);
public:
	static
		LRESULT
		WINAPI
		WinProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);
private:
	HWND		m_hWnd;
	HINSTANCE	m_hInst;

	HWND		m_StaticName;
	HWND		m_ComBoxCmd;
	HWND		m_StaticStatus;
	CBrush		m_BrushBkg;
	CRect		m_rcTextBar;
	CFont		m_hPaintFont;
	
};
#endif