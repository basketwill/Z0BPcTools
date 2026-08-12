#include "stdafx.h"
#include "Init.h"
#include "global.h"

void Init_Win_Create( HINSTANCE hIns,HWND hWnd )
{
	int scrWidth,scrHeight;
	RECT rcClient;
	
	scrWidth  = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	
	GetWindowRect(hWnd,&rcClient);
	
	g_mainToolBar.Init(hWnd,hIns);
	g_mainStatusBar.Init(hWnd,hIns);
	g_ClientMid.Init(hIns,hWnd);
	g_mainCmdBar.Init(hWnd,hIns);

	MoveWindow(g_mainToolBar,0,0,rcClient.right-rcClient.left,0,TRUE);	
	MoveWindow(g_ClientMid,0,0,rcClient.right-rcClient.left,0,TRUE);
	MoveWindow(g_mainStatusBar,0,0,rcClient.right-rcClient.left,0,TRUE);
	MoveWindow(g_mainCmdBar,0,0,rcClient.right-rcClient.left,0,TRUE);

	g_Debugger.Init();
}
