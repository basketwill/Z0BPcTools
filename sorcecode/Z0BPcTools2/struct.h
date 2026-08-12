#ifndef _STRUCT_H
#define _STRUCT_H

typedef enum tagViewName
{
	kCpuAsmView		= 1,
	kCpuRegView		= 2,
	kCpuAddressView = 3,
	kCpuDataView	= 4,
	kCpuStackView	= 5,

}ViewName;

typedef struct tagOpenProcessDebug
{
	TCHAR ProcessPath[MAX_PATH];
	TCHAR ProcessParam[4*MAX_PATH];

}OpenProcessDebug;

class CDisasmView;
class COllyAsmView;
class CDynamicView;

class CClassViewBase 
{
public:
	CClassViewBase()
	{

	}

	virtual ~CClassViewBase()
	{

	}
public:
	virtual CDynamicView* getAsmView()
	{
		return NULL;
	}
};

#endif