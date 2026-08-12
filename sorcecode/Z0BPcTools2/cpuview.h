#ifndef _CPUVIEW_H
#define _CPUVIEW_H


#include "custom.h"
#include "struct.h"
#include "disasmview.h"
#include "struct.h"
#include "ollyasmview.h"
#include "HexOtcDataView.h"
#define  MAX_ROOM 2

class CRegisterView : public CDynamicView
{
public:
	CRegisterView();
	~CRegisterView();
	BOOL Init(HINSTANCE hInst,HWND hWnd);

protected:
	virtual CDynamicDrawItem* OnGetDrawData();
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw);
	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual int	 OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect);
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);
	virtual void OnClickItemUp();
	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();
};

class CAddressView : public CDynamicView
{
public:
	CAddressView();
	~CAddressView();
	BOOL Init(HINSTANCE hInst,HWND hWnd);

protected:
	virtual CDynamicDrawItem* OnGetDrawData();
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw);
	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual int	 OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect);
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);
	virtual void OnClickItemUp();
	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();
};

class CStackView : public CDynamicView
{
public:
	CStackView();
	~CStackView();
	BOOL Init(HINSTANCE hInst,HWND hWnd);
protected:
	virtual CDynamicDrawItem* OnGetDrawData();
	virtual void ReleaseDrawData(CDynamicDrawItem* pDraw);
	virtual void OnClickItem(CRect& ClientRect,CDynamicClickItem& Click );
	virtual int	 OnMouseMovMulti(int nItem,int nColumn,CPoint& pt,int& nDirect);
	virtual PVOID OnGetItemData(int nItem ,CDynamicDrawItem* pDraw);
	virtual void ReleaseItemData(CDynamicDrawItem* pDraw);
	virtual void OnClickItemUp();
	virtual int OnMoveScroll(int Pos);
	virtual	void OnTimerSelectUp();
	virtual	void OnTimerSelectDown();
};

class CDataView : public CHexOtcDataView
{
public:
	CDataView();
	~CDataView();
	BOOL Init(HINSTANCE hInst,HWND hWnd);

protected:
	int					m_HexDataLen;
};

typedef struct tagViewPos
{
	BOOL bInit;
	ULONG TraceViewLine;
	RECT ViewClient;	
	
	RECT AddressView;
	RECT DataView;
	RECT AsmView;
	RECT RegView;
	RECT StackView;

}ViewPos;

typedef struct tagMousePos
{
	POINT	MousePt;
}MousePos;

class CCpuView : public CClassViewBase
{
public:
	CCpuView();
	~CCpuView();

	void operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	void operator =(HINSTANCE hWnd)
	{
		m_hInst = hWnd;
	}

	operator HWND()
	{
		return m_hWnd;
	}

 	virtual CDynamicView* getAsmView()
 	{
 		return &m_CpuAsmView;
 	}

	static CCpuView* Init(HINSTANCE hInst,HWND hWnd,LPTSTR szClass,LPTSTR szTitle,BOOL bClose = FALSE);
	void CpuViewSize(LPARAM lParam,WPARAM wParam);
	void CpuViewMouseMove(LPARAM lParam,WPARAM wParam);
	void CpuViewMouseLeave(LPARAM lParam,WPARAM wParam);
	void CpuViewLButtonDown(LPARAM lParam,WPARAM wParam);
	void CpuViewNcLButtonDown(LPARAM lParam,WPARAM wParam);
	void CpuViewUserLButtonDown(LPARAM lParam,WPARAM wParam);
	void CpuViewLButtonUp(LPARAM lParam,WPARAM wParam);
	void CpuViewUserMouseMove(LPARAM lParam,WPARAM wParam);
	LRESULT CpuViewSystemCommand(LPARAM lParam,WPARAM wParam);
	int CpuOpenProcessDebug(OpenProcessDebug& OpenDebug);
	BOOL OnClose()
	{
		return m_bIsClose;
	}

	void OnClose(BOOL bClose)
	{
		m_bIsClose = bClose;
	}
protected:
	BOOL CpuViewActive( HWND hWnd );
	void CpuViewPaint( HDC hDC, RECT* prcDirty, BOOL bErase);

	HCURSOR CpuViewTraceLoadCursor(POINT& MousePt,ULONG& TraceViewLine);

	void CpuViewBroadCast( ViewName SelectView);

	
public:
	static 
	LRESULT WINAPI DefMdiWinProc(
									HWND hWnd,
									UINT Msg,
									WPARAM wParam,
									LPARAM lParam);
private:
	HWND				m_hWnd;
	HINSTANCE			m_hInst;
	
	CRegisterView		m_RegistryView;
	COllyAsmView		m_CpuAsmView;
	CAddressView		m_AddressView;
	CDataView			m_DataView;
	CStackView			m_StackView;
	ViewPos				m_ViewPos[MAX_ROOM];
	MousePos			m_MousePos;
	HBRUSH				m_BackLineBrush;
	HBRUSH				m_SelectLineBrush;
	int					m_SelectView;
	BOOL				m_LButtonDown;
	BOOL				m_bIsClose;
};
#endif