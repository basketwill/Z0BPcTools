#ifndef _WM_COMMAND_H
#define _WM_COMMAND_H

#include <CommDlg.h>
void
	InitFilterString(
	WORD titleId,
	LPTSTR filter,
	int maxLen
	);
UINT_PTR
	APIENTRY
	OpenExeWithArgsHookProc(
	HWND    hDlg,
	UINT    msg,
	WPARAM  wParam,
	LPARAM  lParam
	);

int	LogStartWithArgs(
	LPTSTR   lpsz,
	LPTSTR   lpszArgs
	);
BOOL
StartFileDlg(
	HWND hwnd,
	int titleId,
	int defExtId,
	int helpId,
	int templateId,
	LPTSTR fileName,
	DWORD* pFlags,
	LPOFNHOOKPROC lpfnHook
	);

void

	DlgEnsureTitleBar(
	void);

int matchExt (TCHAR* queryExtension, TCHAR* sourceList);
void AddToSearchPath(
	LPTSTR lpstrFile);

BOOL RootSetMapped(LPTSTR, LPTSTR);
UINT_PTR
	APIENTRY
	DlgFile(
	HWND hDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);

#endif