#ifndef _syshandlesview_h
#define _syshandlesview_h

#include "customclassview.cpp"

class CSysHandleView : public CElemView<CSysHandleView>
{
public:
	CSysHandleView();
	~CSysHandleView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};


class CSysHandleClassView : public CustomClassView<CSysHandleClassView,CSysHandleView>
{
public:
	CSysHandleClassView();
	~CSysHandleClassView();
public:
	static
		CSysHandleClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};

#endif