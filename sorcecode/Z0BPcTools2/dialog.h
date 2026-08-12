#ifndef _DIALOG_H
#define _DIALOG_H


class CDialog
{
public:
	CDialog();
	~CDialog();

	int Create(UINT nId,HWND hParent,HINSTANCE hInstance);
	static LRESULT StaticDialogProc(HWND hDlg,UINT uMsg, WPARAM wParam, LPARAM lParam);
	
private:
	UINT		mResourceId;
	HWND		m_hWnd;
	HINSTANCE	m_hInst;
};
#endif