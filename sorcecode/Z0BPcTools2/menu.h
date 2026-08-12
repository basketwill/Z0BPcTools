#ifndef _MENU_H
#define _MENU_H

struct CMenu_Data	// A structure that contains the data members for CMenu
{
	// Constructor
	CMenu_Data() : hMenu(0), IsManagedMenu(FALSE), Count(1L) {}

	HMENU hMenu;
	bool IsManagedMenu;
	long Count;
};

class CMenuT
{

public:
	//Construction
	CMenuT();
	CMenuT(UINT nID,HINSTANCE hInst);
	CMenuT(HMENU hMenu);
	CMenuT(const CMenuT& rhs);
	CMenuT& operator = (const CMenuT& rhs)
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}

	void operator = (const HMENU hImageList);
	~CMenuT();

	//Initialization
	void Attach(HMENU hMenu);
	void CreateMenu() const;
	void CreatePopupMenu() const;
	void DestroyMenu();
	HMENU Detach();

	HMENU GetHandle() const;
	BOOL LoadMenu(LPCTSTR lpszResourceName,HINSTANCE hInst) const;
	BOOL LoadMenu(UINT uIDResource,HINSTANCE hInst) const;
	BOOL LoadMenuIndirect(const LPMENUTEMPLATE lpMenuTemplate) const;

	//Menu Operations
	BOOL TrackPopupMenu(UINT uFlags, int x, int y, HWND hWnd, LPCRECT lpRect = 0) const;
	BOOL TrackPopupMenuEx(UINT uFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm) const;

	//Menu Item Operations
	BOOL AppendMenu(UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	BOOL AppendMenu(UINT uFlags, UINT_PTR uIDNewItem, HBITMAP hBitmap);
	UINT CheckMenuItem(UINT uIDCheckItem, UINT uCheck) const;
	BOOL CheckMenuRadioItem(UINT uIDFirst, UINT uIDLast, UINT uIDItem, UINT uFlags) const;
	BOOL DeleteMenu(UINT uPosition, UINT uFlags) const;
	UINT EnableMenuItem(UINT uIDEnableItem, UINT uEnable) const;
	int FindMenuItem(LPCTSTR szMenuString) const;
	UINT GetDefaultItem(UINT gmdiFlags, BOOL ByPosition = FALSE) const;
	DWORD GetMenuContextHelpId() const;

#if(WINVER >= 0x0500)	// Minimum OS required is Win2000
	BOOL GetMenuInfo(MENUINFO& mi) const;
	BOOL SetMenuInfo(const MENUINFO& mi) const;
#endif

	UINT GetMenuItemCount() const;
	UINT GetMenuItemID(int nPos) const;
	BOOL GetMenuItemInfo(UINT uItem, MENUITEMINFO& MenuItemInfo, BOOL ByPosition = FALSE) const;
	UINT GetMenuState(UINT uID, UINT uFlags) const;
	int GetMenuString(UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlags) const;
	int GetMenuString(UINT uIDItem, LPTSTR& rString, UINT uFlags) const;
	CMenuT GetSubMenu(int nPos) const;
	BOOL InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL) const;
	BOOL InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, HBITMAP hBitmap) const;
	BOOL InsertMenuItem(UINT uItem, MENUITEMINFO& MenuItemInfo, BOOL ByPosition = FALSE) const;
	BOOL InsertPopupMenu(UINT uPosition, UINT uFlags, HMENU hPopupMenu, LPCTSTR lpszNewItem) const;
	BOOL ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL) const;
	BOOL ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, HBITMAP hBitmap) const;
	BOOL RemoveMenu(UINT uPosition, UINT uFlags) const;
	BOOL SetDefaultItem(UINT uItem, BOOL ByPosition = FALSE) const;
	BOOL SetMenuContextHelpId(DWORD dwContextHelpId) const;
	BOOL SetMenuItemBitmaps(UINT uPosition, UINT uFlags, HBITMAP hbmUnchecked, HBITMAP hbmChecked) const;
	BOOL SetMenuItemInfo(UINT uItem, MENUITEMINFO& MenuItemInfo, BOOL ByPosition = FALSE) const;

	//Operators
	operator HMENU () const;

private:
	
	void Release()
	{
		if (m_pData)
		{
			delete m_pData;
			m_pData = NULL;
		}
	}

	CMenu_Data* m_pData;

};


////////////////////////////////////////
// Definitions of CMenu
//
inline CMenuT::CMenuT()
{
	m_pData = new CMenu_Data;
}

inline CMenuT::CMenuT(UINT nID,HINSTANCE hInst)
{
	m_pData = new CMenu_Data;

	HMENU menu = ::LoadMenu(hInst, MAKEINTRESOURCE(nID));
	Attach(menu);
	m_pData->IsManagedMenu = TRUE;
}

inline CMenuT::CMenuT(HMENU hMenu)
{
	m_pData = new CMenu_Data;
	Attach(hMenu);
}

inline CMenuT::CMenuT(const CMenuT& rhs)
	// Note: A copy of a CMenu is a clone of the original.
	//       Both objects manipulate the one HMENU.
{
	m_pData = rhs.m_pData;
	InterlockedIncrement(&m_pData->Count);
}

inline void CMenuT::operator = (const HMENU hMenu)
{
	Attach(hMenu);
}

inline CMenuT::~CMenuT()
{
	Release();
}

inline BOOL CMenuT::AppendMenu(UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	return ::AppendMenu(m_pData->hMenu, uFlags, uIDNewItem, lpszNewItem);
}

inline BOOL CMenuT::AppendMenu(UINT uFlags, UINT_PTR uIDNewItem, HBITMAP hBitmap)
{
	return ::AppendMenu(m_pData->hMenu, uFlags, uIDNewItem, (LPCTSTR)hBitmap);
}

inline void CMenuT::Attach(HMENU hMenu)
{
	
	if (hMenu != m_pData->hMenu)
	{
		// Release any existing menu
		if (m_pData->hMenu != 0)
		{
			Release();
			m_pData = new CMenu_Data;
		}

		if (hMenu && m_pData)
		{
			m_pData->hMenu = hMenu;			
		}
	}
}

inline UINT CMenuT::CheckMenuItem(UINT uIDCheckItem, UINT uCheck) const
	// Sets the state of the specified menu item's check-mark attribute to either selected or clear.
{
	return ::CheckMenuItem(m_pData->hMenu, uIDCheckItem, uCheck);
}

inline BOOL CMenuT::CheckMenuRadioItem(UINT uIDFirst, UINT uIDLast, UINT uIDItem, UINT uFlags) const
	// Checks a specified menu item and makes it a radio item. At the same time, the function clears
	//  all other menu items in the associated group and clears the radio-item type flag for those items.
{
	return ::CheckMenuRadioItem(m_pData->hMenu, uIDFirst, uIDLast, uIDItem, uFlags);
}

inline void CMenuT::CreateMenu() const
	// Creates an empty menu.
{
	m_pData->hMenu = ::CreateMenu();
	m_pData->IsManagedMenu = TRUE;
}

inline void CMenuT::CreatePopupMenu() const
	// Creates a drop-down menu, submenu, or shortcut menu. The menu is initially empty.
{
	m_pData->hMenu = ::CreatePopupMenu();
	m_pData->IsManagedMenu = TRUE;
}

inline BOOL CMenuT::DeleteMenu(UINT uPosition, UINT uFlags) const
	// Deletes an item from the specified menu.
{
	return ::DeleteMenu(m_pData->hMenu, uPosition, uFlags);
}

inline void CMenuT::DestroyMenu()
	// Destroys the menu and frees any memory that the menu occupies.
{
	if (::IsMenu(m_pData->hMenu))
		::DestroyMenu( Detach() );
}

inline HMENU CMenuT::Detach()
	// Detaches the HMENU from all CMenu objects.
{
	HMENU hMenu = m_pData->hMenu;
	
	m_pData->hMenu = 0;
	
	if (m_pData->Count > 0)
	{
		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			delete m_pData;
		}
	}

	m_pData = new CMenu_Data;

	return hMenu;
}

inline HMENU CMenuT::GetHandle() const
	// Returns the HMENU assigned to this CMenu
{
	return m_pData->hMenu;
}

inline UINT CMenuT::EnableMenuItem(UINT uIDEnableItem, UINT uEnable) const
{
	return ::EnableMenuItem(m_pData->hMenu, uIDEnableItem, uEnable);
}

inline int CMenuT::FindMenuItem(LPCTSTR szMenuString) const
	// Finds the position of a menu item with the specified string.
	// Returns -1 if the string is not found.
{
// 	CString str;
// 	int count = GetMenuItemCount();
// 	for (int i = 0; i < count; i++)
// 	{
// 		if (GetMenuString(i, str, MF_BYPOSITION))
// 		{
// 			if (str == szMenuString)
// 				return i;
// 		}
// 	}

	return -1;
}

inline UINT CMenuT::GetDefaultItem(UINT gmdiFlags, BOOL ByPosition /*= FALSE*/) const
	// Determines the default menu item.
	// The gmdiFlags parameter specifies how the function searches for menu items.
	// This parameter can be zero or more of the following values: GMDI_GOINTOPOPUPS; GMDI_USEDISABLED.
{
	return ::GetMenuDefaultItem(m_pData->hMenu, ByPosition, gmdiFlags);
}

inline DWORD CMenuT::GetMenuContextHelpId() const
	// Retrieves the Help context identifier associated with the menu.
{
	return ::GetMenuContextHelpId(m_pData->hMenu);
}

#if(WINVER >= 0x0500)
// minimum OS required : Win2000

inline BOOL CMenuT::GetMenuInfo(MENUINFO& mi) const
	// Retrieves the menu information.
{
	return ::GetMenuInfo(m_pData->hMenu, &mi);
}

inline BOOL CMenuT::SetMenuInfo(const MENUINFO& mi) const
	// Sets the menu information from the specified MENUINFO structure.
{

	return ::SetMenuInfo(m_pData->hMenu, &mi);
}

#endif

inline UINT CMenuT::GetMenuItemCount() const
	// Retrieves the number of menu items.
{
	return ::GetMenuItemCount(m_pData->hMenu);
}

inline UINT CMenuT::GetMenuItemID(int nPos) const
{
	return ::GetMenuItemID(m_pData->hMenu, nPos);
}

inline int GetWinVersion()
{
	DWORD dwVersion = GetVersion();
	int Platform = (dwVersion < 0x80000000)? 2:1;
	int MajorVer = LOBYTE(LOWORD(dwVersion));
	int MinorVer = HIBYTE(LOWORD(dwVersion));

	int nVersion =  1000*Platform + 100*MajorVer + MinorVer;

	// Return values and window versions:
	//  1400     Windows 95
	//  1410     Windows 98
	//  1490     Windows ME
	//  2400     Windows NT
	//  2500     Windows 2000
	//  2501     Windows XP
	//  2502     Windows Server 2003
	//  2600     Windows Vista and Windows Server 2008
	//  2601     Windows 7 and Windows Server 2008 r2
	//  2602     Windows 8 and Windows Server 2012
	//  2603     Windows 8.1 and Windows Server 2012 r2

	// Note: For windows 8.1 and above, the value returned is also affected by the embedded manifest
	//       Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS (2602). 
	return nVersion;
}


inline UINT GetSizeofMenuItemInfo()
{
	// For Win95 and NT, cbSize needs to be 44
	if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
		return CCSIZEOF_STRUCT(MENUITEMINFO, cch);

	return sizeof(MENUITEMINFO);
}

inline BOOL CMenuT::GetMenuItemInfo(UINT uItem, MENUITEMINFO& MenuItemInfo, BOOL ByPosition /*= FALSE*/) const
{
	MenuItemInfo.cbSize = GetSizeofMenuItemInfo();
	return ::GetMenuItemInfo(m_pData->hMenu, uItem, ByPosition, &MenuItemInfo);
}

inline UINT CMenuT::GetMenuState(UINT uID, UINT uFlags) const
{
	return ::GetMenuState(m_pData->hMenu, uID, uFlags);
}

inline int CMenuT::GetMenuString(UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlags) const
	// Copies the text string of the specified menu item into the specified buffer.
{
	return ::GetMenuString(m_pData->hMenu, uIDItem, lpString, nMaxCount, uFlags);
}

inline int CMenuT::GetMenuString(UINT uIDItem, LPTSTR& rString, UINT uFlags) const
{
	int n = ::GetMenuString(m_pData->hMenu, uIDItem, rString, 80, uFlags);
	
	return n;
}

inline CMenuT CMenuT::GetSubMenu(int nPos) const
	// Retrieves the CMenu object of a pop-up menu.
{
	CMenuT* pMenu = new CMenuT;
	pMenu->m_pData->hMenu = ::GetSubMenu(m_pData->hMenu, nPos);
	pMenu->m_pData->IsManagedMenu = FALSE;

	return *pMenu;
}

inline BOOL CMenuT::InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/) const
	// Inserts a new menu item into a menu, moving other items down the menu.
{
	return ::InsertMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, lpszNewItem);
}

inline BOOL CMenuT::InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, HBITMAP hBitmap) const
	// Inserts a new menu item into a menu, moving other items down the menu.
{
	return ::InsertMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, (LPCTSTR)hBitmap);
}

inline BOOL CMenuT::InsertMenuItem(UINT uItem, MENUITEMINFO& MenuItemInfo, BOOL ByPosition /*= FALSE*/) const
{
	MenuItemInfo.cbSize = GetSizeofMenuItemInfo();
	return ::InsertMenuItem(m_pData->hMenu, uItem, ByPosition, &MenuItemInfo);
}

inline BOOL CMenuT::InsertPopupMenu(UINT uPosition, UINT uFlags, HMENU hPopupMenu, LPCTSTR lpszNewItem) const
	// Inserts a popup menu item at the specified position in the menu.
{
	// Ensure the correct flags are set
	uFlags &= ~MF_BITMAP;
	uFlags &= ~MF_OWNERDRAW;
	uFlags |= MF_POPUP;

	return ::InsertMenu(m_pData->hMenu, uPosition, uFlags, (UINT_PTR)hPopupMenu, lpszNewItem);
}

inline BOOL CMenuT::LoadMenu(LPCTSTR lpszResourceName,HINSTANCE hInst) const
	// Loads the menu from the specified windows resource.
{
	m_pData->hMenu = ::LoadMenu(hInst, lpszResourceName);

	return NULL != m_pData->hMenu;
}

inline BOOL CMenuT::LoadMenu(UINT uIDResource,HINSTANCE hInst) const
	// Loads the menu from the specified windows resource.
{
	m_pData->hMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(uIDResource));
	
	return NULL != m_pData->hMenu;
}

inline BOOL CMenuT::LoadMenuIndirect(const LPMENUTEMPLATE lpMenuTemplate) const
{
	
	m_pData->hMenu = ::LoadMenuIndirect(lpMenuTemplate);
	
	return NULL != m_pData->hMenu;
}

inline BOOL CMenuT::ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/) const
	// Changes an existing menu item. This function is used to specify the content, appearance, and behavior of the menu item.
{
	
	return ::ModifyMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, lpszNewItem);
}

inline BOOL CMenuT::ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, HBITMAP hBitmap) const
	// Changes an existing menu item. This function is used to specify the content, appearance, and behavior of the menu item.
{
	
	return ::ModifyMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, (LPCTSTR)hBitmap);
}

inline BOOL CMenuT::RemoveMenu(UINT uPosition, UINT uFlags) const
	// Deletes a menu item or detaches a submenu from the menu.
{
	
	return ::RemoveMenu(m_pData->hMenu, uPosition, uFlags);
}

inline BOOL CMenuT::SetDefaultItem(UINT uItem, BOOL ByPosition /*= FALSE*/) const
	//  sets the default menu item for the menu.
{
	
	return ::SetMenuDefaultItem(m_pData->hMenu, uItem, ByPosition);
}

inline BOOL CMenuT::SetMenuContextHelpId(DWORD dwContextHelpId) const
	// Associates a Help context identifier with the menu.
{
	
	return ::SetMenuContextHelpId(m_pData->hMenu, dwContextHelpId);
}

inline BOOL CMenuT::SetMenuItemBitmaps(UINT uPosition, UINT uFlags, HBITMAP hbmUnchecked, HBITMAP hbmChecked) const
	// Associates the specified bitmap with a menu item.
{
	return ::SetMenuItemBitmaps(m_pData->hMenu, uPosition, uFlags, hbmUnchecked, hbmChecked);
}

inline BOOL CMenuT::SetMenuItemInfo(UINT uItem, MENUITEMINFO& MenuItemInfo, BOOL ByPosition /*= FALSE*/) const
	// Changes information about a menu item.
{
	
	MenuItemInfo.cbSize = GetSizeofMenuItemInfo();
	return ::SetMenuItemInfo(m_pData->hMenu, uItem, ByPosition, &MenuItemInfo);
}

inline BOOL CMenuT::TrackPopupMenu(UINT uFlags, int x, int y, HWND hWnd, LPCRECT lpRect /*= 0*/) const
	// Displays a shortcut menu at the specified location and tracks the selection of items on the menu.
{
	return ::TrackPopupMenu(m_pData->hMenu, uFlags, x, y, 0, hWnd, lpRect);
}

inline BOOL CMenuT::TrackPopupMenuEx(UINT uFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm) const
	// Displays a shortcut menu at the specified location and tracks the selection of items on the shortcut menu.
{
	return ::TrackPopupMenuEx(m_pData->hMenu, uFlags, x, y, hWnd, lptpm);
}

inline CMenuT::operator HMENU () const
	// Retrieves the menu's handle.
{	
	return m_pData->hMenu;
}


class CMenu
{
public:
	CMenu();
	~CMenu();

	operator HMENU();

	BOOL Init(UINT nId,HINSTANCE hInst);
	BOOL LoadMenu(UINT nId,HINSTANCE hInst);
	BOOL Enable(UINT uIDEnableItem, UINT uEnable);
	BOOL Pop(UINT uFlags, int x, int y, HWND hWnd);
private:
	CMenuT	m_cMenu;
};
#endif