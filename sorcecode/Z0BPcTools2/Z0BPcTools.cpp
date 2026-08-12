// Z0BPcTools2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Z0BPcTools.h"
#include "custom.h"
#include "wheel.h"
#include "Init.h"
#include "global.h"
#include "wmcommand.h"
#include "res_str.h"
#include "resource.h"
#include "util.h"
#include "attachdlg.h"
#define MAX_LOADSTRING 100



// Edit and up/dopwn controls for setting nMin and nMax, both for horizontal
// and vertical scrollbars of the custom controls:

CMdi					g_ClientMid;
CMenu					g_mainMenu;
CToolBar				g_mainToolBar;
CStatusBar				g_mainStatusBar;
CCmdBar					g_mainCmdBar;
HINSTANCE				g_hInstance = 0;
HWND					g_hwndMain	= NULL;  
HWND					hwndCustom	= 0;


static BOOL CALLBACK
	SetChildFont(HWND hwndChild, LPARAM lParam)
{
	HFONT hFont = (HFONT)lParam;
	SendMessage(hwndChild, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
	return TRUE;
}

static 
LRESULT 
CALLBACK
Z0BToolWinProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam)
{
	switch (uMsg) {
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) 
		{
			RECT rcClient;
			// Get the coordinates of the parent window's client area.
			GetClientRect(g_hwndMain, &rcClient);
			MoveWindow(
					g_mainToolBar,
					rcClient.left,
					rcClient.top,
					rcClient.right-rcClient.left,
					TOOL_BAR_HEIGHT,
					TRUE);

			MoveWindow(
					g_mainStatusBar,
					rcClient.left,
					rcClient.bottom - STATUS_BAR_HEIGHT,
					rcClient.right - rcClient.left,
					STATUS_BAR_HEIGHT,
					TRUE);

			MoveWindow(
				g_mainCmdBar,
				rcClient.left,
				rcClient.bottom - STATUS_BAR_HEIGHT - CMD_BAR_HEIGHT,
				rcClient.right - rcClient.left,
				CMD_BAR_HEIGHT,
				TRUE);

			MoveWindow(
				g_ClientMid,
				rcClient.left + 1,
				rcClient.top + TOOL_BAR_HEIGHT,
				rcClient.right - rcClient.left - 2,
				rcClient.bottom - rcClient.top - TOOL_BAR_HEIGHT - STATUS_BAR_HEIGHT - CMD_BAR_HEIGHT,
				TRUE);
						
		}
		return 0;

	case WM_COMMAND:
		// On edit control update, propagate the hrozintal/vertical range into
		// the custom control:
		{
			Z0BPcToolsComman(hwnd,wParam,lParam);
		}
		break;
	case WM_CREATE:
		{
			Init_Win_Create(g_hInstance,hwnd);
		}				
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return g_ClientMid.DefFrameProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, TCHAR* lpszCmdLine, int iCmdShow)
{
	
	INITCOMMONCONTROLSEX icce = { sizeof(INITCOMMONCONTROLSEX), ICC_UPDOWN_CLASS };
	WNDCLASS wc = { 0 };
	MSG msg;

	g_hInstance = hInst;

	InitCommonControlsEx(&icce);
	
	WheelInitialize();

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Z0BToolWinProc;
	wc.hInstance = g_hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon	= LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_Z0BPCTOOLS2));
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = _T("main_win");
	RegisterClass(&wc);

	g_mainMenu.LoadMenu(IDC_Z0BPCTOOLS2,g_hInstance);
	g_hwndMain = CreateWindow(
					_T("main_win"),
					_T("PowerDbg"),
					WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
					CW_USEDEFAULT,
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT,
					NULL, 
					g_mainMenu,
					g_hInstance,
					NULL);

	ShowWindow(g_hwndMain, iCmdShow);
	//g_mainMenu.Enable(ID_OPEN_PROCESS,MF_DISABLED);
	g_mainMenu.Enable(ID_ATTACH_PROCESS,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_RECORD,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_REFERENCE,MF_DISABLED);
	g_mainMenu.Enable(ID_MENU_DEBUG,MF_DISABLED);
	g_mainMenu.Enable(ID_MENU_OPTION,MF_DISABLED);
	g_mainMenu.Enable(ID_MENU_WINDOW,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_PATCH,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_RUN_STACK,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_BREAKPOINT,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_SOURCE_CODE,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_SOURCE_FILE,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_CPU_ASM,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_MOUDLE,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_RUN_CHECK,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_RUN_MONITOR,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_SEH,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_STACK,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_MEMORY,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_THREAD,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_WIN_HANDLE,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_HANDLS,MF_DISABLED);
	g_mainMenu.Enable(ID_VIEW_RUN_MONITOR,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_SUSPEND,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_RUN,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_RESTART,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_STOP,MF_DISABLED);
	g_mainMenu.Enable(ID_DDEBUG_STEP_IN,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_STEP_OUT,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_AUTO_STEP_IN ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_AUTO_STEP_OUT,MF_DISABLED);
	g_mainMenu.Enable(ID_AUTO_RUN_RETURN,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_RUN_UNTIL_USER_CODE ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_OPEN_OR_CLEAR_RUN,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_MONITOR_STEP_IN ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_MONITOR_STEP_OUT ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_SET_FILETR ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_SHUTDOWN_RUN ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_HARD_BP  ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_CHECK_REG ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_DLL_OUTPUT ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_FUN_PARAM ,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_SELECT_IMPORT_DLL,MF_DISABLED);
	g_mainMenu.Enable(ID_DEBUG_SELECT_SYMBOL,MF_DISABLED);
	g_mainMenu.Enable(ID_OPTION_UI_SET ,MF_DISABLED);
	g_mainMenu.Enable(ID_OPTION_DEBUG_SET,MF_DISABLED);
	g_mainMenu.Enable(ID_OPTION_REAL_SET,MF_DISABLED);
	g_mainMenu.Enable(ID_OPTION_ADD_RIGTH_EXPLORER,MF_DISABLED);
	g_mainMenu.Enable(ID_WINDOWS_SET_TOP_MOST,MF_DISABLED);
	g_mainMenu.Enable(ID_PLUGIN_ADD ,MF_DISABLED);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(g_Debugger.WaitDebugMsg())
		{
			g_Debugger.ProcessDebugEvent();
		}
	}

	WheelUninitialize();

	return (int)msg.wParam;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void Z0BPcToolsComman(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int hwndControl = (int)wParam;

	switch(hwndControl)
	{
	case ID_OPEN_PROCESS:
		{
			Z0BPcToolsMenuOpenProcess();
		}
		break;
	case ID_IDA_OPEN_STATIC:
		{
			Z0BPcToolsMenuWithIda();
		}
		break;
	case ID_ATTACH_PROCESS:
		{
			CAttchDialog AttachDlg(g_hInstance);
			AttachDlg.DoBox(hWnd,IDD_DIALOG_ATTACH_PROCESS);
		}
		break;
	case IDM_ABOUT:
		{
			DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About, (LPARAM)NULL);
		}
		break;
	case IDM_EXIT:
		{
			PostQuitMessage(WM_QUIT);
		}
		break;
	}
}

TCHAR szOpenExeArgs[_MAX_PATH] = {0};
void Z0BPcToolsMenuOpenProcess()
{
	TCHAR szExeName[MAX_PATH] = { 0 };
	DWORD dw = OFN_EXPLORER | OFN_HIDEREADONLY
		| OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (0 == *szExeName) {
		GetCurrentDirectory(sizeof(szExeName), szExeName);
		if ('\\' != szExeName[wcslen(szExeName)-1]) {
			wcscat(szExeName, _T("\\"));
		}
		wcscat(szExeName, _T("*.exe"));
	}

	if (StartFileDlg(
					g_hwndMain,                    
					DLG_Browse_Executable_Title, 
					DEF_Ext_EXE, 
					ID_OPEN_PROCESS, 
					IDD_DLG_FILEOPEN_EXE,
					szExeName,
					&dw,
					(LPOFNHOOKPROC)OpenExeWithArgsHookProc)) 
	{		
			if (*szExeName) 
			{				
				LPTSTR lpszExeName = szExeName, lpszOpenExeArgs = szOpenExeArgs;

				lpszExeName = CPSkipWhitespace(lpszExeName);
				lpszOpenExeArgs = CPSkipWhitespace(lpszOpenExeArgs);

				LPTSTR LpszCommandLine = _wcsdup(lpszOpenExeArgs);
								//if (0 == strlen(lpszOpenExeArgs)) {
				// empty string. Just pass a NULL to LogStartWithArgs.
				//lpszOpenExeArgs = NULL;
				//}

				LogStartWithArgs(lpszExeName, lpszOpenExeArgs);
			}
	}
}


void Z0BPcToolsMenuWithIda()
{
	TCHAR szExeName[MAX_PATH] = { 0 };
	DWORD dw = OFN_EXPLORER | OFN_HIDEREADONLY
		| OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (0 == *szExeName) {
		GetCurrentDirectory(sizeof(szExeName), szExeName);
		if ('\\' != szExeName[wcslen(szExeName)-1]) {
			wcscat(szExeName, _T("\\"));
		}
		wcscat(szExeName, _T("*.exe;*.dll;*.sys"));
	}

	if (StartFileDlg(
		g_hwndMain,                    
		DLG_Browse_Executable_Title, 
		DEF_Ext_PE, 
		ID_IDA_OPEN_STATIC, 
		0,
		szExeName,
		&dw,
		(LPOFNHOOKPROC)NULL)) 
	{
		if (*szExeName) {


			LPTSTR lpszExeName = szExeName, lpszOpenExeArgs = szOpenExeArgs;

			lpszExeName = CPSkipWhitespace(lpszExeName);
			
			//if (0 == strlen(lpszOpenExeArgs)) {
			// empty string. Just pass a NULL to LogStartWithArgs.
			//lpszOpenExeArgs = NULL;
			//}

			g_ClientMid.OpenWithIda(lpszExeName);
		}
	}
}