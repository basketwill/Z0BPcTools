#ifndef _customclassview_h
#define _customclassview_h
#include "custom.h"

#define MAX_VIEW_ELEM   20
#define custom_msg
#define BASE_ELEM_VIEW_INDEX 0

typedef enum _tagViewDirect
{
	kViewToBottom		= 1,
	kViewToTop			= 2,
	kViewToLeft			= 3,
	kViewToRight		= 4,
	kViewToFullSrceen	= 5,

}tagViewDirect;

template <typename T>
class CElemView : public CCustomView
{
public:
	CElemView();
	~CElemView();

public:
	virtual 
	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );

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

	virtual void PaintColumn(	
		HDC hDC,
		PCustomItemHead CustomItem,
		RECT rcColumn,
		int nColumn,
		BOOL bSelect);
};

template <typename T,typename T2>
class CustomClassView
{
public:
	CustomClassView();
	virtual ~CustomClassView();

	inline operator HWND()
	{
		return mhWnd;
	}

	inline operator HINSTANCE()
	{
		return mhInst;
	}

	inline void operator =(HWND hWnd )
	{
		mhWnd = hWnd;
	}

	inline void operator =(HINSTANCE hInst )
	{
		mhInst = hInst;
	}

	CElemView<T2>* AddView( CElemView<T2>* pBase,tagViewDirect ViewDirect);

	inline T* GetClass()
	{
		return (T*)GetWindowLongPtr( mhWnd,0);
	}

protected:
	HWND				mhWnd;
	HINSTANCE			mhInst;
	int					mSubPos;
	HBRUSH				m_BackLineBrush;
	CElemView<T2>*		mTView[MAX_VIEW_ELEM];
protected:
	static 
	LRESULT WINAPI DefMdiWinProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

	static
	CustomClassView<T,T2>* Init( HINSTANCE hInst,HWND hWndParent,int uID,LPTSTR szClass,LPTSTR szTitle = NULL );

protected:
	virtual custom_msg BOOL OnClose();
	virtual custom_msg BOOL OnCreate();
	virtual custom_msg BOOL OnSize();
	virtual custom_msg BOOL OnPaint( HDC hDC, RECT* prcDirty, BOOL bErase);
	virtual custom_msg BOOL OnMouseMove(LPARAM lParam,WPARAM wParam);
	
};
#endif