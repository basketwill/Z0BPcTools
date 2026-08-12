#ifndef _MDI_H
#define _MDI_H
#include "cpuview.h"
#include "idaview.h"
#include "threadview.h"
#include "winhanleview.h"
#include "syshandlesview.h"
#include "memoryview.h"
#include "moudleview.h"
#include "logview.h"
#include "referenceview.h"
#include "bpointview.h"
#include "stackview.h"
#include "membreak.h"
#include "hardbreak.h"

#define MAX_IDA_VIEW_LIST_SIZE 100
#define IDI_BASE_VIEW IDI_CPU_VIEW

typedef struct _tagIDA_VIEW
{
	CIdaView* _Ida[MAX_IDA_VIEW_LIST_SIZE];
	int		  _Ida_Count;

}tagIDA_VIEW,IDA_VIEW;

class CThreadClassView;

class CMdi
{
public:
	CMdi();
	~CMdi();

	BOOL Init(HINSTANCE hIns,HWND hParent);
	BOOL AddChildView(LPWSTR lpClassName,LPWSTR lpTitleName);
	HRESULT DefFrameProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	operator HWND();
	int StartDebugProcess(OpenProcessDebug& OpenDebug);
	int OpenWithIda(LPTSTR PathFile );
private:
	CLIENTCREATESTRUCT		m_ClientStruct;
	HWND					m_hWnd;
	HINSTANCE				m_hInstance;
	CCpuView*				m_CpuView;
	CThreadClassView*		m_ThreadView;
	CWinHandleClassView*	m_WinHandleView;
	CSysHandleClassView*	m_SysHandleView;
	CMemoryClassView*		m_MemoryView;
	CMoudleClassView*		m_ModuleView;
	CLogClassView*			m_LogView;
	CReferenceClassView*	m_ReferenceView;
	CBPointClassView*		m_BPointView;
	CStackClassView*		m_StackInfoView;
	CMemBreakClassView*		m_MemBreakView;
	CHardBreakClassView*	m_HardBreakView;
	tagIDA_VIEW				m_IdaViewList;
};

#endif