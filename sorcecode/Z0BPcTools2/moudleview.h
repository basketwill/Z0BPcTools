#ifndef _moudleview_h
#define _moudleview_h

#include "customclassview.cpp"

class CMoudleView : public CElemView<CMoudleView>
{
public:
	CMoudleView();
	~CMoudleView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};


class CMoudleClassView : public CustomClassView<CMoudleClassView,CMoudleView>
{
public:
	CMoudleClassView();
	~CMoudleClassView();
public:
	static
		CMoudleClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};

#endif