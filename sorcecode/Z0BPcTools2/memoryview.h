#ifndef _memoryview_h
#define _memoryview_h


#include "customclassview.cpp"

class CMemoryView : public CElemView<CMemoryView>
{
public:
	CMemoryView();
	~CMemoryView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CMemoryClassView : public CustomClassView<CMemoryClassView,CMemoryView>
{
public:
	CMemoryClassView();
	~CMemoryClassView();
public:
	static
		CMemoryClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};

#endif