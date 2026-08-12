#ifndef _attachdlg_h
#define _attachdlg_h
#include "custom.h"

#define kAttachViewSel 1

class CProcessView : public CCustomView
{
public:
	CProcessView();
	~CProcessView();
	BOOL Init(HINSTANCE hInst,HWND hWnd);

	ULONG_PTR GetSelProcessId();
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

	void PaintColumn(	HDC hDC,
		PCustomItemHead CustomItem,
		RECT rcColumn,
		int nColumn,
		BOOL bSelect);
};


class CAttchDialog
{
public:
	CAttchDialog(HINSTANCE hInst);
	void operator =(HWND hWnd)
	{
		mhWnd = hWnd;
	}

	INT_PTR DoBox( HWND hWndParent, UINT nID);
	BOOL OnInit();
	BOOL OnSize();

	BOOL OnAttach();
	
	~CAttchDialog();
protected:
	static INT_PTR WINAPI AttachDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HINSTANCE		mhInst;
	HWND			mhWnd;
	CProcessView	mProcessView;

};

#endif