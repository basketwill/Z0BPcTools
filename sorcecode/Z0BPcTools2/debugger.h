#ifndef _DEBUG_EVENT_H
#define _DEBUG_EVENT_H
#include "common.h"
#include "CList.h"
#include <assert.h>

class CDbgApi;

#define LOGERROR_NOERROR    0
#define LOGERROR_UNKNOWN    1
#define LOGERROR_QUIET      2
#define LOGERROR_CP         3
#define LOGERROR_ASYNC      4

typedef unsigned char BP_UNIT;

#define BP_SIZE     sizeof(BP_UNIT)
#define BP_OPCODE   0xCC
#define DELAYED_BRANCH_SLOT_SIZE    0
#define MAX_DEBUG_ACTIVE_THREAD_NUMS 300

#define DEBUGEVENT_TIME 100

#define EXADDR(pde)    ((pde)->u.Exception.ExceptionRecord.ExceptionAddress)

#define IP_TYPE ULONG64
#define Set_PC(x, y)                (((x)->context.Eip) = (IP_TYPE) (y))
#define Set_cPC(x, y)               (((x)->Eip)         = (IP_TYPE) (y))
#define Set_STACK_POINTER(x, y)     (((x)->context.Esp) = (IP_TYPE) (y))
#define Set_FRAME_POINTER(x, y)     (((x)->context.Ebp) = (IP_TYPE) (y))

#define PcSegOfHthdx(x)     ((SEGMENT) (x->context.SegCs))
#define SsSegOfHthdx(x)     ((SEGMENT) (x->context.SegSs))

enum {
	BREAKPOINT_DEBUG_EVENT=(RIP_EVENT+1),
	CHECK_BREAKPOINT_DEBUG_EVENT,
	SEGMENT_LOAD_DEBUG_EVENT,
	DESTROY_PROCESS_DEBUG_EVENT,
	DESTROY_THREAD_DEBUG_EVENT,
	ATTACH_DEADLOCK_DEBUG_EVENT,
	ATTACH_EXITED_DEBUG_EVENT,
	ENTRYPOINT_DEBUG_EVENT,
	LOAD_COMPLETE_DEBUG_EVENT,
	INPUT_DEBUG_STRING_EVENT,
	MESSAGE_DEBUG_EVENT,
	MESSAGE_SEND_DEBUG_EVENT,
	FUNC_EXIT_EVENT,
	OLE_DEBUG_EVENT,
	FIBER_DEBUG_EVENT,
	GENERIC_DEBUG_EVENT,
	BOGUS_WIN95_SINGLESTEP_EVENT,
	MAX_EVENT_CODE
};


typedef struct _DEBUG_ACTIVE_STRUCT {
	volatile BOOL fAttach;          // tell DmPoll to act
	HANDLE        hEventApiDone;    // signal shell that API finished
	HANDLE        hEventReady;      // clear until finished loading

	BOOL          fReturn;          // API return value
	DWORD         dwError;          // GetLastError() value

	ULONG_PTR     dwProcessId;      // pid to debug
	HANDLE        hEventGo;         // signal after hitting ldr BP
	HANDLE        hProcess;         // handle for waiting on

} DEBUG_ACTIVE_STRUCT, *PDEBUG_ACTIVE_STRUCT;

typedef struct _SPAWN_STRUCT {
	BOOL                fSpawn;
	HANDLE              hEventApiDone;

	BOOL                fReturn;    // return from API
	DWORD               dwError;

	LPTSTR              szAppName;  // args to API etc
	LPTSTR              szArgs;
	LPTSTR              pszCurrentDirectory; // directory to spawn process.
	DWORD               fdwCreate;
	BOOL                fInheritHandles;
	STARTUPINFO         si;
} SPAWN_STRUCT, *PSPAWN_STRUCT;

typedef struct _EXCEPTION_DEBUG_INFO32 {
	EXCEPTION_RECORD32 ExceptionRecord;
	DWORD dwFirstChance;
} EXCEPTION_DEBUG_INFO32, *LPEXCEPTION_DEBUG_INFO32;

typedef struct _CREATE_THREAD_DEBUG_INFO32 {
	HANDLE hThread;
	DWORD lpThreadLocalBase;
	DWORD lpStartAddress;
} CREATE_THREAD_DEBUG_INFO32, *LPCREATE_THREAD_DEBUG_INFO32;

typedef struct _EXIT_THREAD_DEBUG_INFO32 {
	DWORD dwExitCode;
} EXIT_THREAD_DEBUG_INFO32, *LPEXIT_THREAD_DEBUG_INFO32;

typedef struct _CREATE_PROCESS_DEBUG_INFO32 {
	HANDLE hFile;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD lpBaseOfImage;
	DWORD dwDebugInfoFileOffset;
	DWORD nDebugInfoSize;
	DWORD lpThreadLocalBase;
	DWORD lpStartAddress;
	DWORD lpImageName;
	WORD fUnicode;
} CREATE_PROCESS_DEBUG_INFO32, *LPCREATE_PROCESS_DEBUG_INFO32;

typedef struct _EXIT_PROCESS_DEBUG_INFO32 {
	DWORD dwExitCode;
} EXIT_PROCESS_DEBUG_INFO32, *LPEXIT_PROCESS_DEBUG_INFO32;

typedef struct _LOAD_DLL_DEBUG_INFO32 {
	HANDLE hFile;
	DWORD lpBaseOfDll;
	DWORD dwDebugInfoFileOffset;
	DWORD nDebugInfoSize;
	DWORD lpImageName;
	WORD fUnicode;
} LOAD_DLL_DEBUG_INFO32, *LPLOAD_DLL_DEBUG_INFO32;

typedef struct _UNLOAD_DLL_DEBUG_INFO32 {
	DWORD lpBaseOfDll;
} UNLOAD_DLL_DEBUG_INFO32, *LPUNLOAD_DLL_DEBUG_INFO32;

typedef struct _OUTPUT_DEBUG_STRING_INFO32 {
	DWORD lpDebugStringData;
	WORD fUnicode;
	WORD nDebugStringLength;
} OUTPUT_DEBUG_STRING_INFO32, *LPOUTPUT_DEBUG_STRING_INFO32;

typedef struct _RIP_INFO32 {
	DWORD dwError;
	DWORD dwType;
} RIP_INFO32, *LPRIP_INFO32;

typedef struct _DEBUG_EVENT32 {
	DWORD dwDebugEventCode;
	DWORD dwProcessId;
	DWORD dwThreadId;
	union {
		EXCEPTION_DEBUG_INFO32 Exception;
		CREATE_THREAD_DEBUG_INFO32 CreateThread;
		CREATE_PROCESS_DEBUG_INFO32 CreateProcessInfo;
		EXIT_THREAD_DEBUG_INFO32 ExitThread;
		EXIT_PROCESS_DEBUG_INFO32 ExitProcess;
		LOAD_DLL_DEBUG_INFO32 LoadDll;
		UNLOAD_DLL_DEBUG_INFO32 UnloadDll;
		OUTPUT_DEBUG_STRING_INFO32 DebugString;
		RIP_INFO32 RipInfo;
	} u;
} DEBUG_EVENT32, *LPDEBUG_EVENT32;


typedef struct _EXIT_PROCESS_DEBUG_INFO64 {
	DWORD dwExitCode;
} EXIT_PROCESS_DEBUG_INFO64, *LPEXIT_PROCESS_DEBUG_INFO64;

typedef struct _EXCEPTION_DEBUG_INFO64 {
	EXCEPTION_RECORD64 ExceptionRecord;
	DWORD dwFirstChance;
} EXCEPTION_DEBUG_INFO64, *LPEXCEPTION_DEBUG_INFO64;

typedef struct _CREATE_THREAD_DEBUG_INFO64 {
	HANDLE hThread;
	DWORD64 lpThreadLocalBase;
	DWORD64 lpStartAddress;
} CREATE_THREAD_DEBUG_INFO64, *LPCREATE_THREAD_DEBUG_INFO64;

typedef struct _CREATE_PROCESS_DEBUG_INFO64 {
	HANDLE hFile;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD64 lpBaseOfImage;
	DWORD dwDebugInfoFileOffset;
	DWORD nDebugInfoSize;
	DWORD64 lpThreadLocalBase;
	DWORD64 lpStartAddress;
	DWORD64 lpImageName;
	WORD fUnicode;
} CREATE_PROCESS_DEBUG_INFO64, *LPCREATE_PROCESS_DEBUG_INFO64;

typedef struct _EXIT_THREAD_DEBUG_INFO64 {
	DWORD dwExitCode;
} EXIT_THREAD_DEBUG_INFO64, *LPEXIT_THREAD_DEBUG_INFO64;

typedef struct _LOAD_DLL_DEBUG_INFO64 {
	HANDLE hFile;
	DWORD64 lpBaseOfDll;
	DWORD dwDebugInfoFileOffset;
	DWORD nDebugInfoSize;
	DWORD64 lpImageName;
	WORD fUnicode;
} LOAD_DLL_DEBUG_INFO64, *LPLOAD_DLL_DEBUG_INFO64;

typedef struct _UNLOAD_DLL_DEBUG_INFO64 {
	DWORD64 lpBaseOfDll;
} UNLOAD_DLL_DEBUG_INFO64, *LPUNLOAD_DLL_DEBUG_INFO64;

typedef struct _OUTPUT_DEBUG_STRING_INFO64 {
	DWORD64 lpDebugStringData;
	WORD fUnicode;
	WORD nDebugStringLength;
} OUTPUT_DEBUG_STRING_INFO64, *LPOUTPUT_DEBUG_STRING_INFO64;

typedef struct _RIP_INFO64 {
	DWORD dwError;
	DWORD dwType;
} RIP_INFO64, *LPRIP_INFO64;

typedef struct _DEBUG_EVENT64 {
	DWORD dwDebugEventCode;
	DWORD dwProcessId;
	DWORD dwThreadId;
	union {
		EXCEPTION_DEBUG_INFO64 Exception;
		CREATE_THREAD_DEBUG_INFO64 CreateThread;
		CREATE_PROCESS_DEBUG_INFO64 CreateProcessInfo;
		EXIT_THREAD_DEBUG_INFO64 ExitThread;
		EXIT_PROCESS_DEBUG_INFO64 ExitProcess;
		LOAD_DLL_DEBUG_INFO64 LoadDll;
		UNLOAD_DLL_DEBUG_INFO64 UnloadDll;
		OUTPUT_DEBUG_STRING_INFO64 DebugString;
		RIP_INFO64 RipInfo;
	} u;
} DEBUG_EVENT64, *LPDEBUG_EVENT64;

typedef struct _tagBreakPoint
{
	ULONG64				ProcessId;
	ULONG64				ThreadId;
	DWORD				bpType;
	DWORD				bpNotify;

	ADDR				BreakAddr;
	BP_UNIT				BreakInstr1;

	 BOOL				IsStep; 

#if defined(TARGET_IA64)
	 BPFLG      flags;       // IA64 mode mask
#endif
     _tagBreakPoint*	mHead;
	 _tagBreakPoint*	mTail;

}BreakPoint;


typedef enum {
	dmfRemoteDied = -1,             /* debugger quit */
	dmfCommError  = -2,             /* transport layer error */

	dmfNull = 0,

	dmfInit,
	dmfUnInit,
	dmfRemoteQuit,
	dmfSelect,
	dmfConnect,

	dmfCreatePid,
	dmfDestroyPid,

	dmfSetPath,
	dmfSpawnOrphan,
	dmfProgLoad,
	dmfProgFree,
	dmfDebugActive,
	dmfTerm,

	dmfGo,
	dmfStop,
	dmfFreeze,
	dmfResume,

	dmfSingleStep,
	dmfReturnStep,
	dmfNonLocalGoto,
	dmfRangeStep,
	dmfGoToReturn,

	dmfBreakpoint,
	dmfGetExceptionState,
	dmfSetExceptionState,

	dmfReadMem,
	dmfReadReg,
	dmfReadFrameReg,
	dmfWriteMem,
	dmfWriteReg,
	dmfWriteFrameReg,
	dmfGetFP,
	dmfSetFP,
	dmfThreadStatus,
	dmfProcessStatus,
	dmfQueryTlsBase,
	dmfQuerySelector,
	dmfVirtualQuery,
	dmfReadRegEx,
	dmfWriteRegEx,
	dmfGetSections,

	dmfInit32SegValues,
	dmfSelLim,

	dmfSetMulti,
	dmfClearMulti,
	dmfDebugger,
	dmfSync,
	dmfSystemService,
	dmfGetPrompt,
	dmfSendChar,
	dmfGetDmInfo,

	dmfSetupExecute,
	dmfStartExecute,
	dmfCleanUpExecute,

	dmfRemoteUtility,       // for the mfile utility
	dmfGetTimeStamp,

	dmfNewSymbolsLoaded,    // Informs the DM that new symbols have been reloaded

	dmfLast

} _DMF;



typedef LONG DMF;

typedef HANDLE HDEP;
typedef HANDLE HIND;

#ifdef STRICT
DECLARE_HANDLE(HPID);
DECLARE_HANDLE(HTID);
#else
typedef HIND    HPID;
typedef HIND    HTID;
#endif

typedef HPID FAR *LPHPID;
typedef HTID FAR *LPHTID;

#pragma   pack(push, 4)
//  it is important that the rgbVar fields be aligned on a DWORDLONG boundary
#ifndef TARGJAVA
typedef struct _DBB {
	DMF  dmf;
	union {
		HPID hpid;
		DWORDLONG Alignment1;
	};
	union {
		HTID htid;
		//DWORDLONG Alignment2;
	};
	union {
		BYTE rgbVar[1];
		DWORDLONG Alignment;
	};
} DBB;
#else
typedef struct _DBB {
	DMF  dmf;
	WORD dummy;
	HPID hpid;
	HTID htid;
	BYTE rgbVar[];
} DBB;
#endif

typedef DBB *LPDBB;

#pragma pack(pop)

#define CheckPacketFlags( a,b) ((a.rpFlags & b) == b)

typedef enum _tagPacketType
{
	kPacketDebugCreateProcess = 0x1,
	kPacketDebugActiveProcess = 0x2,
}tagPacketType;

typedef struct _tagRequestPacket
{
	LONG		rpMsg;
	void*		hpid;
	void*		htid;
	ULONG		rpMsgError;
	ULONG		rpFlags;
	ULONG		rpDataSize;
	BYTE		rpData[0];
}RequestPacket;

typedef struct _tagDebugCreateProcess
{
	TCHAR	ProcessPath[2*MAX_PATH];  //进程path
	TCHAR	CreateArg[4*MAX_PATH];  //参数
	TCHAR	ProcessDirectory[2*MAX_PATH];//目录
	BOOL	InheritHandles;
	ULONG	CreateFlags;

}DebugCreateProcess;

typedef struct _tagDebugActiveThread DebugActiveThread;
typedef struct _tagDebugActiveProcess 
{
	DMPSTATE					State;         // DM state model
	CList<DebugActiveThread>	ChildThread;
	PVOID						FbrCntx;       // Pointer to a fiber context to display
}DebugProcess;

typedef struct _tagDebugActiveThread
{
	TID						ThreadId;
	PID						ProcessId;
	HANDLE					ThreadHandle;
	DebugProcess*			ActiveProcess;
	CONTEXT					ThreadContext;
	TSTATEX					State;
	ULONG64					StartAddress;// Thread start address
	BOOL					InitThreadContext;
	ULONG64					StackRA;
	ULONG64					StackBase;
	int						FuncEval;
	ULONG					ExitCode;
	ULONG64					OffTeb;
	EXCEPTION_RECORD64		ExceptionRecord;

	_tagDebugActiveThread*	_mNext;
	_tagDebugActiveThread*	_mPrev;
}DebugActiveThread;

typedef struct _tagDebugActiveEvent
{
	PID								ActiveProcessId;
	DWORD64							ActiveThreadNums;
	CList<DebugActiveThread>		ActiveThreadList;
	DebugProcess					ActiveProcess;
#ifdef WIN64
	DEBUG_EVENT64			ActiveDebugEvent;
#else
	DEBUG_EVENT				ActiveDebugEvent;
#endif
}DebugActiveEvent;

//
// Conversion Macros
//
#define COPYSE(p64, p32, f)                 \
	p64->f = (ULONG64)(LONG64)(LONG)p32->f
#define SE32To64(x32) ((ULONG64)(LONG64)(LONG)(x32))
#define SEPtrTo64(x32) ((ULONG64)(LONG64)(LONG_PTR)(x32))
#define DBGKD_VERS_FLAG_DATA							0x0002
#define DBGKD_VERS_FLAG_MP                              0x0001
__inline

__inline
	VOID
	RipInfo32To64(
	LPRIP_INFO32 r32,
	LPRIP_INFO64 r64
	)
{
	r64->dwError = r32->dwType;
	r64->dwError = r32->dwError;
}

__inline
	VOID
	RipInfo64To32(
	LPRIP_INFO64 r64,
	LPRIP_INFO32 r32
	)
{
	r32->dwError = r64->dwType;
	r32->dwError = r64->dwError;
}

__inline
	VOID
	OutputDebugStringInfo32To64(
	LPOUTPUT_DEBUG_STRING_INFO32 o32,
	LPOUTPUT_DEBUG_STRING_INFO64 o64
	)
{
	o64->lpDebugStringData = SE32To64( o32->lpDebugStringData );
	o64->fUnicode = o32->fUnicode;
	o64->nDebugStringLength = o32->nDebugStringLength;
}

__inline
	VOID
	OutputDebugStringInfo64To32(
	LPOUTPUT_DEBUG_STRING_INFO64 o64,
	LPOUTPUT_DEBUG_STRING_INFO32 o32
	)
{
	o32->lpDebugStringData = (DWORD)o64->lpDebugStringData;
	o32->fUnicode = o64->fUnicode;
	o32->nDebugStringLength = o64->nDebugStringLength;
}


__inline
	VOID
	UnloadDllDebugInfo32To64(
	LPUNLOAD_DLL_DEBUG_INFO32 u32,
	LPUNLOAD_DLL_DEBUG_INFO64 u64
	)
{
	u64->lpBaseOfDll = SE32To64(u32->lpBaseOfDll);
}

__inline
	VOID
	UnloadDllDebugInfo64To32(
	LPUNLOAD_DLL_DEBUG_INFO64 u64,
	LPUNLOAD_DLL_DEBUG_INFO32 u32
	)
{
	u32->lpBaseOfDll = (DWORD)u64->lpBaseOfDll;
}

__inline
	VOID
	LoadDllDebugInfo32To64(
	LPLOAD_DLL_DEBUG_INFO32 l32,
	LPLOAD_DLL_DEBUG_INFO64 l64
	)
{
	l64->hFile = l32->hFile;
	l64->lpBaseOfDll = SE32To64( l32->lpBaseOfDll );
	l64->dwDebugInfoFileOffset = l32->dwDebugInfoFileOffset;
	l64->nDebugInfoSize = l32->nDebugInfoSize;
	l64->lpImageName = SE32To64(l32->lpImageName );
	l64->fUnicode = l32->fUnicode;
}

__inline
	VOID
	LoadDllDebugInfo64To32(
	LPLOAD_DLL_DEBUG_INFO64 l64,
	LPLOAD_DLL_DEBUG_INFO32 l32
	)
{
	l32->hFile = l64->hFile;
	l32->lpBaseOfDll = (DWORD)l64->lpBaseOfDll;
	l32->dwDebugInfoFileOffset = l64->dwDebugInfoFileOffset;
	l32->nDebugInfoSize = l64->nDebugInfoSize;
	l32->lpImageName = (DWORD)l64->lpImageName;
	l32->fUnicode = l64->fUnicode;
}

__inline
	VOID
	ExitProcessDebugInfo32To64(
	LPEXIT_PROCESS_DEBUG_INFO32 e32,
	LPEXIT_PROCESS_DEBUG_INFO64 e64
	)
{
	e64->dwExitCode = e32->dwExitCode;
}

__inline
	VOID
	ExitProcessDebugInfo64To32(
	LPEXIT_PROCESS_DEBUG_INFO64 e64,
	LPEXIT_PROCESS_DEBUG_INFO32 e32
	)
{
	e32->dwExitCode = e64->dwExitCode;
}

__inline
	VOID
	ExitThreadDebugInfo32To64(
	LPEXIT_THREAD_DEBUG_INFO32 e32,
	LPEXIT_THREAD_DEBUG_INFO64 e64
	)
{
	e64->dwExitCode = e32->dwExitCode;
}

__inline
	VOID
	ExitThreadDebugInfo64To32(
	LPEXIT_THREAD_DEBUG_INFO64 e64,
	LPEXIT_THREAD_DEBUG_INFO32 e32
	)
{
	e32->dwExitCode = e64->dwExitCode;
}

__inline
	VOID
	CreateProcessDebugInfo32To64(
	LPCREATE_PROCESS_DEBUG_INFO32 c32,
	LPCREATE_PROCESS_DEBUG_INFO64 c64
	)
{
	c64->hFile = c32->hFile;
	c64->hProcess = c32->hProcess;
	c64->hThread = c32->hThread;
	c64->lpBaseOfImage = SE32To64( c32->lpBaseOfImage );
	c64->dwDebugInfoFileOffset = c32->dwDebugInfoFileOffset;
	c64->nDebugInfoSize = c32->nDebugInfoSize;
	c64->lpThreadLocalBase = SE32To64( c32->lpThreadLocalBase );
	c64->lpStartAddress = SE32To64( c32->lpStartAddress );
	c64->lpImageName = SE32To64( c32->lpImageName );
	c64->fUnicode = c32->fUnicode;
}

__inline
	VOID
	CreateProcessDebugInfo64To32(
	LPCREATE_PROCESS_DEBUG_INFO64 c64,
	LPCREATE_PROCESS_DEBUG_INFO32 c32
	)
{
	c32->hFile = c64->hFile;
	c32->hProcess = c64->hProcess;
	c32->hThread = c64->hThread;
	c32->lpBaseOfImage = (DWORD)c64->lpBaseOfImage;
	c32->dwDebugInfoFileOffset = c64->dwDebugInfoFileOffset;
	c32->nDebugInfoSize = c64->nDebugInfoSize;
	c32->lpThreadLocalBase = (DWORD)c64->lpThreadLocalBase;
	c32->lpStartAddress = (DWORD)c64->lpStartAddress;
	c32->lpImageName = (DWORD)c64->lpImageName;
	c32->fUnicode = c64->fUnicode;
}


__inline
	VOID
	CreateThreadDebugInfo32To64(
	LPCREATE_THREAD_DEBUG_INFO32 c32,
	LPCREATE_THREAD_DEBUG_INFO64 c64
	)
{
	c64->hThread = c32->hThread;
	c64->lpThreadLocalBase = SE32To64( c32->lpThreadLocalBase );
	c64->lpStartAddress = SE32To64( c32->lpStartAddress );
}

__inline
	VOID
	CreateThreadDebugInfo64To32(
	LPCREATE_THREAD_DEBUG_INFO64 c64,
	LPCREATE_THREAD_DEBUG_INFO32 c32
	)
{
	c32->hThread = c64->hThread;
	c32->lpThreadLocalBase = (DWORD)c64->lpThreadLocalBase;
	c32->lpStartAddress = (DWORD)c64->lpStartAddress;
}


FORCEINLINE
	VOID
	ExceptionRecord32To64(IN PEXCEPTION_RECORD32 Ex32,
	OUT PEXCEPTION_RECORD64 Ex64)
{
	ULONG i;

	Ex64->ExceptionCode = Ex32->ExceptionCode;
	Ex64->ExceptionFlags = Ex32->ExceptionFlags;
	Ex64->ExceptionRecord = Ex32->ExceptionRecord;
	COPYSE(Ex64,Ex32,ExceptionAddress);
	Ex64->NumberParameters = Ex32->NumberParameters;

	for (i = 0; i < EXCEPTION_MAXIMUM_PARAMETERS; i++)
	{
		COPYSE(Ex64,Ex32,ExceptionInformation[i]);
	}
}


__inline
	VOID
	ExceptionDebugInfo32To64(
	LPEXCEPTION_DEBUG_INFO32 e32,
	LPEXCEPTION_DEBUG_INFO64 e64
	)
{
	ExceptionRecord32To64(&e32->ExceptionRecord, &e64->ExceptionRecord);
	e64->dwFirstChance = e32->dwFirstChance;
}

__inline
	VOID
	ExceptionDebugInfo64To32(
	LPEXCEPTION_DEBUG_INFO64 e64,
	LPEXCEPTION_DEBUG_INFO32 e32
	)
{
	//__asm int 3;
	//ExceptionRecord64To32(&e64->ExceptionRecord, &e32->ExceptionRecord);
	e32->dwFirstChance = e64->dwFirstChance;
}

__inline
	VOID
	DebugEvent32To64(
	LPDEBUG_EVENT32 d32,
	LPDEBUG_EVENT64 d64
	)
{
	d64->dwDebugEventCode = d32->dwDebugEventCode;
	d64->dwProcessId = d32->dwProcessId;
	d64->dwThreadId = d32->dwThreadId;

	switch (d32->dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		ExceptionDebugInfo32To64(&d32->u.Exception, &d64->u.Exception);
		break;

	case CREATE_THREAD_DEBUG_EVENT:
		CreateThreadDebugInfo32To64(&d32->u.CreateThread, &d64->u.CreateThread);
		break;

	case CREATE_PROCESS_DEBUG_EVENT:
		CreateProcessDebugInfo32To64(&d32->u.CreateProcessInfo, &d64->u.CreateProcessInfo);
		break;

	case EXIT_THREAD_DEBUG_EVENT:
		ExitThreadDebugInfo32To64(&d32->u.ExitThread, &d64->u.ExitThread);
		break;

	case EXIT_PROCESS_DEBUG_EVENT:
		ExitProcessDebugInfo32To64(&d32->u.ExitProcess, &d64->u.ExitProcess);
		break;

	case LOAD_DLL_DEBUG_EVENT:
		LoadDllDebugInfo32To64(&d32->u.LoadDll, &d64->u.LoadDll);
		break;

	case UNLOAD_DLL_DEBUG_EVENT:
		UnloadDllDebugInfo32To64(&d32->u.UnloadDll, &d64->u.UnloadDll);
		break;

	case OUTPUT_DEBUG_STRING_EVENT:
		OutputDebugStringInfo32To64(&d32->u.DebugString, &d64->u.DebugString);
		break;

	case RIP_EVENT:
		RipInfo32To64(&d32->u.RipInfo, &d64->u.RipInfo);
		break;

#if DBG
	default:
		DebugBreak();
		//ASSERT(0);
		break;
#endif
	}
}

__inline
	VOID
	DebugEvent64To32(
	LPDEBUG_EVENT64 d64,
	LPDEBUG_EVENT32 d32
	)
{
	d32->dwDebugEventCode = d64->dwDebugEventCode;
	d32->dwProcessId = d64->dwProcessId;
	d32->dwThreadId = d64->dwThreadId;

	switch (d64->dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		ExceptionDebugInfo64To32(&d64->u.Exception, &d32->u.Exception);
		break;

	case CREATE_THREAD_DEBUG_EVENT:
		CreateThreadDebugInfo64To32(&d64->u.CreateThread, &d32->u.CreateThread);
		break;

	case CREATE_PROCESS_DEBUG_EVENT:
		CreateProcessDebugInfo64To32(&d64->u.CreateProcessInfo, &d32->u.CreateProcessInfo);
		break;

	case EXIT_THREAD_DEBUG_EVENT:
		ExitThreadDebugInfo64To32(&d64->u.ExitThread, &d32->u.ExitThread);
		break;

	case EXIT_PROCESS_DEBUG_EVENT:
		ExitProcessDebugInfo64To32(&d64->u.ExitProcess, &d32->u.ExitProcess);
		break;

	case LOAD_DLL_DEBUG_EVENT:
		LoadDllDebugInfo64To32(&d64->u.LoadDll, &d32->u.LoadDll);
		break;

	case UNLOAD_DLL_DEBUG_EVENT:
		UnloadDllDebugInfo64To32(&d64->u.UnloadDll, &d32->u.UnloadDll);
		break;

	case OUTPUT_DEBUG_STRING_EVENT:
		OutputDebugStringInfo64To32(&d64->u.DebugString, &d32->u.DebugString);
		break;

	case RIP_EVENT:
		RipInfo64To32(&d64->u.RipInfo, &d32->u.RipInfo);
		break;

#if DBG
	default:
		DebugBreak();
		//ASSERT(0);
		break;
#endif
	}
}

class CDebugger
{
public:
	CDebugger(void);
	~CDebugger();
	
	BOOL DbgActiveProcess(ULONG_PTR ProcessId);
	BOOL Init();

public:
	
	void ProcessAttachProcess();
	VOID DmFunc(RequestPacket& Packet);
	VOID ProcessDebugEvent();
	BOOL StartDebugProcess(RequestPacket& Packet);
	BOOL AttachProcessId(ULONG_PTR ProcessId);
	DebugProcess* HPRCFromPID(
								PID pid
								);

	DebugActiveThread*
		HTHDXFromPIDTID(
		PID pid,
		TID tid
		);

	void AddrFromHthdx(PVOID* paddr,DebugActiveThread* hthd);
	BOOL
		AddrReadMemory(
		DebugProcess*			hprc,
		DebugActiveThread*      hthd,
		LPADDR					paddr,
		LPVOID					lpv,
		DWORD					cb,
		LPDWORD					lpRead
		);
	BOOL
		TranslateAddress(
		DebugProcess*		hprc,
		DebugActiveThread*  hthd,
		LPADDR lpaddr,
		BOOL   f16ToFlat
		);

	BOOL
		DecodeSingleStepEvent(
		DebugActiveThread* hthd,
		DEBUG_EVENT64 *de,
		PDWORD eventCode,
		PDWORD_PTR subClass
		);

	void
		UpdateThreadContext(
		DebugActiveThread*   hthd
		);
	BOOL WaitDebugMsg();
private:
	ULONG DmPoll(LPVOID lpv);
	BOOL SetDebugPrivilege(void);
	void ProcessDebugActiveCmd(RequestPacket& Packet);
	DebugActiveThread* DbgGetThreadContext(ULONG64	ThreadId);
	BOOL DbgGetThreadContext(DebugActiveThread* DbgThread ,LPCONTEXT Context);
	BOOL DbgReadMemory( void* pBuffer,void* pBaseAdress,DWORD ReadSize,DWORD ReadType);
	void DbgProcessExceptionEvent(DebugActiveThread* ActiveThread);
 
#ifdef WIN64
	BOOL	DbgWaitDebugEvent(LPDEBUG_EVENT64 DgbEvent,DWORD TimeOut);
#else
	BOOL	DbgWaitDebugEvent(LPDEBUG_EVENT DgbEvent,DWORD TimeOut);
#endif
private:
	
	SPAWN_STRUCT			mSpawnStruct;          // packet for causing CreateProcess()
	DEBUG_ACTIVE_STRUCT		mDebugActiveStruct;    // ... for DebugActiveProcess()
	BOOL					mfDisconnected;
	HANDLE					mhEventRemoteQuit;
private:
	DebugActiveEvent		mDebugActiveEvent;
	CList<BreakPoint>		mBreakPointList;



private:
	/************************************************************************/
	/* API                                                                     */
	/************************************************************************/
	CDbgApi*				mDebugApi;

};
#endif