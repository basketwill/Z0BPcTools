#ifndef _referenceview_h
#define _referenceview_h
#include "customclassview.cpp"

class CReferenceView : public CElemView<CReferenceView>
{
public:
	CReferenceView();
	~CReferenceView();

	virtual BOOL Init(HINSTANCE hInst,HWND hWnd,UINT Id,LPTSTR szClass,LPTSTR szTitle = NULL );
};

class CReferenceClassView : public CustomClassView<CReferenceClassView,CReferenceView>
{
public:
	CReferenceClassView();
	~CReferenceClassView();
public:
	static
	CReferenceClassView* Init( HINSTANCE hInst,HWND hWndParent,int uID );
protected:
	virtual BOOL OnClose();
};


#endif