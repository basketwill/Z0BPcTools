#ifndef _logview_h
#define _logview_h
#include "customclassview.cpp"

class CLogView : public CElemView<CLogView>
{
public:
	CLogView();
	~CLogView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CLogClassView : public CustomClassView<CLogClassView,CLogView>
{
public:
	CLogClassView();
	~CLogClassView();
public:
	static
	CLogClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};

#endif