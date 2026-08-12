#include "stdafx.h"
#include "menu.h"

CMenu::CMenu()
{

}

CMenu::~CMenu()
{

}

CMenu::operator HMENU()
{
	return m_cMenu;
}

BOOL CMenu::Init(UINT nId,HINSTANCE hInst)
{
	return LoadMenu(nId,hInst);
}

BOOL CMenu::LoadMenu(UINT nId,HINSTANCE hInst)
{
	
	m_cMenu.LoadMenu(nId,hInst);
	if (m_cMenu)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CMenu::Enable(UINT uIDEnableItem, UINT uEnable)
{
	return m_cMenu.EnableMenuItem(uIDEnableItem,uEnable);
}

BOOL CMenu::Pop(UINT uFlags, int x, int y, HWND hWnd)
{
	return m_cMenu.TrackPopupMenu(uFlags,x,y,hWnd);
}