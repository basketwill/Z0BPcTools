#ifndef _threadview_h
#define _threadview_h

#include "customclassview.cpp"

class CThreadView : public CElemView<CThreadView>
{
public:
	CThreadView();
	~CThreadView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};


class CThreadClassView : public CustomClassView<CThreadClassView,CThreadView>
{
public:
	CThreadClassView();
	~CThreadClassView();
public:
	static
	CThreadClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
	
	
};


#endif