#ifndef _bpointview_h
#define _bpointview_h

#include "customclassview.cpp"

class CBPointView : public CElemView<CBPointView>
{
public:
	CBPointView();
	~CBPointView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CBPointClassView : public CustomClassView<CBPointClassView,CBPointView>
{
public:
	CBPointClassView();
	~CBPointClassView();
public:
	static
	CBPointClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};


#endif