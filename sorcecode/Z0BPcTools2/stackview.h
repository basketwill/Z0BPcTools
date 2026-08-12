#ifndef _stackview_hh
#define _stackview_hh

#include "customclassview.cpp"

class CStatckKView : public CElemView<CStatckKView>
{
public:
	CStatckKView();
	~CStatckKView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CStackClassView : public CustomClassView<CStackClassView,CStatckKView>
{
public:
	CStackClassView();
	~CStackClassView();
public:
	static
	CStackClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};


#endif