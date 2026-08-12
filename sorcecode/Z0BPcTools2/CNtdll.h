#ifndef _CNTDLL_H
#define _CNTDLL_h
#include "CList.h"
#include <Windows.h>
#include <vector>
#include <string>
#ifdef __cplusplus
extern "C"{
#endif
typedef 
	DWORD 
	( WINAPI *ZWQUERYSYSTEMINFORMATION)(
									DWORD,
									PVOID,
									DWORD,
									PDWORD);

typedef LONG KPRIORITY;

typedef struct _VM_COUNTERS {
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} VM_COUNTERS;
typedef VM_COUNTERS *PVM_COUNTERS;


typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_HANDLE_INFORMATION 
{
	ULONG ProcessId;
	UCHAR ObjectTypeNumber;
	UCHAR Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
}SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX 
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_INFORMATION Information[1];
}SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

/************************************************************************/
/* ½ø³Ì                                                                     */
/************************************************************************/
typedef struct _SYSTEM_PROCESSES
{
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	IO_COUNTERS IoCounters;
} _SYSTEM_PROCESSES,*PSYSTEM_PROCESSES;

#define SystemProcessesAndThreadsInformation 5
#define SystemHandleInformation 0x10 //16

#ifdef __cplusplus
};

typedef LONG NTSTATUS;
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

typedef struct _PROCESS_ATTACH_INFO
{
	ULONG					ProcessId;
	ULONG					InheritedFromProcessId;
	TCHAR					ProcessName[2*MAX_PATH];
	_PROCESS_ATTACH_INFO*	_mNext;
	_PROCESS_ATTACH_INFO*	_mPrev;

}PROCESS_ATTACH_INFO;

class CNtDll
{
public:
	CNtDll(void)
	{

	}

	~CNtDll(void)
	{

	}
public:
	static BOOL QuerySystemProces(CList<PROCESS_ATTACH_INFO>& ProcessInfo);
};
#endif

#endif