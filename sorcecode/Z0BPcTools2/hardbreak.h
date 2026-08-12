#ifndef _hardbreak_h
#define _hardbreak_h

#include "customclassview.cpp"

class CHardBreakView : public CElemView<CHardBreakView>
{
public:
	CHardBreakView();
	~CHardBreakView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CHardBreakClassView : public CustomClassView<CHardBreakClassView,CHardBreakView>
{
public:
	CHardBreakClassView();
	~CHardBreakClassView();
public:
	static
		CHardBreakClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};


#endif