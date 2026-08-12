#ifndef _winhanleview_h
#define _winhanleview_h

#include "customclassview.cpp"

class CWinHandleView : public CElemView<CWinHandleView>
{
public:
	CWinHandleView();
	~CWinHandleView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};


class CWinHandleClassView : public CustomClassView<CWinHandleClassView,CWinHandleView>
{
public:
	CWinHandleClassView();
	~CWinHandleClassView();
public:
	static
		CWinHandleClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();


};

#endif