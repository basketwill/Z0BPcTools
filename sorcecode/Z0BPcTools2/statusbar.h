#ifndef _STATUS_BAR_H
#define _STATUS_BAR_H
#include "winbase.h"
#define STATUS_PANNEL_COLOR RGB(200,200,200)

class CStatusBar
{
public:
	CStatusBar();
	~CStatusBar();

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

protected:
	void OnPaint(HDC hDC, RECT* prcDirty, BOOL bErase);
private:
	HWND		m_hWnd;
	CBrush		m_PannelBrush;
	CBrush		m_ScrollBrush;
	CPen		m_PannelPen;
	CFont		m_hPaintFont;
	float		m_nPercentProcess;
	COLORREF	m_colorProcess;
	LPTSTR	m_pszText[MAX_PATH];

public:
	void ShowStatusInfo(LPTSTR pszText,float Percent);
	void ShowStatusInfo(LPTSTR pszText);
	void AddProcess(float Percent);
};
#endif