#ifndef _GLOBAL_H
#define _GLOBAL_H
#include "custom.h"
#include "mdi.h"
#include "menu.h"
#include "toolbar.h"
#include "statusbar.h"
#include "cmdbar.h"
#include "common.h"
#include "debugger.h"
#include <CommDlg.h>

#define WM_USER_BASE 0x01
#define WM_OPEN_DEBUG		( WM_USER + WM_USER_BASE + 1)
#define WM_CPU_VIEW_LBUTTON ( WM_USER + WM_USER_BASE + 2 )

#define MAINWIN_PADDING      5
#define TOOL_BAR_HEIGHT		25
#define STATUS_BAR_HEIGHT	22
#define CMD_BAR_HEIGHT		25


class CMdi;
extern CMdi					g_ClientMid;
extern CMenu				g_mainMenu;
extern CToolBar				g_mainToolBar;
extern CStatusBar			g_mainStatusBar;
extern CCmdBar				g_mainCmdBar;
extern HINSTANCE			g_hInstance;
extern HWND					g_hwndMain;       // Main window.
extern HWND					hwndCustom;     // Custom control window.
extern CDebugger			g_Debugger;
#endif