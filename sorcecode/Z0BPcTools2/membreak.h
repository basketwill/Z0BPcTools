#ifndef _membreak_h
#define _membreak_h

#include "customclassview.cpp"

class CMemBreakView : public CElemView<CMemBreakView>
{
public:
	CMemBreakView();
	~CMemBreakView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CMemBreakClassView : public CustomClassView<CMemBreakClassView,CMemBreakView>
{
public:
	CMemBreakClassView();
	~CMemBreakClassView();
public:
	static
		CMemBreakClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};


#endif