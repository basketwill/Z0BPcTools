#include "stdafx.h"
#include "dialog.h"

CDialog::CDialog()
	:m_hWnd(NULL)
	,m_hInst(NULL)
{

}

CDialog::~CDialog()
{

}

int CDialog::Create(UINT nId,HWND hParent,HINSTANCE hInstance)
{
	
	if(m_hWnd = ::CreateDialog(hInstance, MAKEINTRESOURCE(nId), hParent, (DLGPROC)CDialog::StaticDialogProc))
	{

	}

	return TRUE;
}

LRESULT CDialog::StaticDialogProc(HWND hDlg,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = 0;

	CDialog* pDlg = (CDialog*)GetWindowLongPtr( hDlg,0);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			// Center the dialog
			//CenterWindow();
		}
		break;
	case WM_CLOSE:	
		{
			
			return 0L;
		}
		break;
	case WM_COMMAND:
		{
		
		}
		break;


	case WM_DESTROY:
		
		break;
	case WM_NOTIFY:
		{
		}

	case WM_PAINT:
		{
			break;
		}

	case WM_ERASEBKGND:
		{
			return TRUE;
		}
		break;

	} // switch(uMsg)

	return 0;
}