#ifndef _DBGAPI_H
#define _DBGAPI_H
#include "debugger.h"
#include "common.h"


typedef struct _HFBRX {
	// linked list
	struct _HFBRX   *next;
	LPVOID  fbrstrt;
	LPVOID  fbrcntx;

} HFBRXSTRUCT,*HFBRX;



typedef struct _EXCEPTION_DESCRIPTION {
	DWORD                    dwExceptionCode;
	EXCEPTION_FILTER_DEFAULT efd;
	EXCEPTION_CONTROL        exc;
	TCHAR                    rgchDescription[EXCEPTION_STRING_SIZE];
} EXCEPTION_DESCRIPTION;
typedef EXCEPTION_DESCRIPTION * LPEXCEPTION_DESCRIPTION;

typedef struct  _EXCEPTION_LIST {
	struct _EXCEPTION_LIST *next;
	EXCEPTION_DESCRIPTION  excp;
} EXCEPTION_LIST, *LPEXCEPTION_LIST;

typedef struct _DLLLOAD_ITEM {
	BOOL        fValidDll;         // is this entry filled?
	DWORDLONG   offBaseOfImage;    // offset for base of Image
	DWORD       cbImage;           // size of image in bytes
	LPTSTR      szDllName;         // dll name

	PIMAGE_SECTION_HEADER Sections;          // pointer to section headers
	DWORD                 NumberOfSections;  // number of section headers

#ifndef KERNEL

	BOOL        fReal;
	BOOL        fWow;
	OFFSET      offTlsIndex;    // The offset of the TLS index for the DLL
	// kentf The following comment is what I found in the sources which I
	//       hacked the OLE stuff from.
	// ptr (in debuggee's memory space) to this
	// DLL's 1-byte boolean flag indicating whether
	// OLE RPC debugging is enabled.  If this DLL
	// does not support OLE RPC, then this field
	// will be zero.
	// However, the code in dmole.c uses this as a pointer to a function which
	// takes two args, the first of which is the above described flag, and the
	// second of which is zero.
	LPVOID      lpvOleRpc;

	BOOL        fContainsOle;   // does this DLL contain any OLE RPC segments?

	PIMAGE_SECTION_HEADER sec;               //~~!!!!!

#else

	DWORD                 TimeStamp;         //
	DWORD                 CheckSum;          //
	WORD                  SegCs;             //
	WORD                  SegDs;             //
	PIMAGE_SECTION_HEADER sec;               //

#endif

} DLLLOAD_ITEM, * PDLLLOAD_ITEM;

#pragma pack(push, 1)       // {

typedef struct _OLENOT      // OLE notification (no packing!)
{
	BYTE        rgbSig[4];  // signature, "MARB"
	GUID        guid;       // GUID indicating which notification this is
	DWORD       cb;         // size of additional info which follows in memory
} OLENOT;
typedef OLENOT * POLENOT;

#pragma pack(pop)           // }

typedef enum _OLESEG
{
	olenone,  // Not a special OLE segment.
	oleorpc,  // Contains interface proxies and stubs
	olebrk,   // contains the special call made by mfcans32.dll etc
	// to tell us the destination on the server side.
	// arguments to this call are "this" and the "pmf".
	olefwd,   // Similar to olebrk except the argument to this call is
	// the address of the function we need to set the bp at.
} OLESEG;

typedef struct _OLERG           // OLE range
{
	//  PDLLLOAD_ITEM   pdllitem;   // What DLL this range is in
	UOFFSET         uoffMin;    // start addr of range of OLE code (proxies & stubs)
	UOFFSET         uoffMax;    // end addr of range (points one byte past end)
	OLESEG          segType;    // What kind of special segment is this.
} OLERG;
typedef OLERG *POLERG;



#define INTERNAL
#if defined(INTERNAL)
typedef struct _DLL_DEFER_LIST {
	struct _DLL_DEFER_LIST  * next;
	LOAD_DLL_DEBUG_INFO       LoadDll;
} DLL_DEFER_LIST, *PDLL_DEFER_LIST;
#endif

#define CWPI    14

// #ifdef STRICT
// DECLARE_HANDLE(HPID);
// DECLARE_HANDLE(HTID);
// #else
// typedef HIND    HPID;
// typedef HIND    HTID;
// #endif

typedef struct  _HPRCX {
	// linked lists
	struct _HPRCX   *next;
	struct _HTHDX   *hthdChild;
	struct _HFBRX   *FbrLst;
	PID             pid;            // OS provided process ID
	HANDLE          rwHand;         // OS provided Process handle
	BOOL            CloseProcessHandle; // If we have a private
	// handle to this process, close it.
	// Otherwise, it belongs to smss.
	DWORD           dwExitCode;     // Process exit status

	HPID            hpid;           // binding to EM object

	DMPSTATE        pstate;         // DM state model
	BOOL            f16bit;         // CreateProcess EXE was 16 bit
	EXCEPTION_LIST *exceptionList;  // list of exceptions to silently
	// continue unhandled
	int             cLdrBPWait;     // timeout counter while waiting for ldr BP
	HANDLE          hExitEvent;     // synchronization object for
	// process termination
	PDLLLOAD_ITEM   rgDllList;      // module list
	int             cDllList;       // item count for module list

	HANDLE          hEventCreateThread;  // Sync object for thread creation

#ifndef KERNEL
	BOOL            fUseFbrs;       // Use fiber context or thread context
	PVOID           pFbrCntx;       // Pointer to a fiber context to display
	// NULL = use thread context
	DWORD_PTR        dwKernel32Base; // lpBaseOfDll for kernel32.

	DWORD           colerg;         // number of OLE ranges in *rgolerg
	POLERG          rgolerg;        // array of OLERGs: sorted list of all
	// addresses in this process (including
	// its DLLs) which are special OLE
	// segments.  May be
	// NULL if colerg is zero.

	ORPC_DEBUGGING_STATE    OrpcDebugging;  // orpc debugging state (see above)
	ORPCKEYSTATE            orpcKeyState;

	DWORD_PTR        rgwpoff[CWPI];  // addrs of Wndproc-invoking functions

	HLLI            llnlg;          // non-local goto

	UOFFSET         PebAddress;     // NT Process Environment Block

#else

	BOOL            fRomImage;      // rom image
	BOOL            fRelocatable;   // relocatable code

#endif

#if defined(TARGET_i386)
	SEGMENT         segCode;
#endif

#if defined(INTERNAL)
	// during process startup, dll name resolution may be
	// deferred until the loader BP.  Once the process is
	// fully initialized, this deferral is no longer allowed.
	BOOL            fNameRequired;
	PDLL_DEFER_LIST pDllDeferList;
#endif

} HPRCXSTRUCT, *HPRCX;

class CHPrcX
{
public:
	CHPrcX()
		:mInner(0)
		,mOpenHand(INVALID_HANDLE_VALUE)
		,mPId(-1)
	{

	}

	~CHPrcX()
	{

	}

	BOOL IsInner()
	{
		return mInner;
	}

public:
	HANDLE          mOpenHand;
	ULONG64			mPId;
	BOOL			mInner;
};

#define hprcxNull       ((HPRCX) 0)


typedef struct  _WTNODE {
	struct _WTNODE      *caller;      // caller's wtnode
	struct _WTNODE      *callee;      // current function called by this function
	DWORDLONG           offset;       // address of this function
	DWORDLONG           sp;           // SP for this frame
	int                 icnt;         // number of instructions executed
	int                 scnt;         // subordinate count
	int                 lex;          // lexical level of this function
	LPSTR               fname;        // function name
} WTNODE, *LPWTNODE;




typedef struct _CRASH_THREAD 
{
	DWORD ThreadId;
	DWORD SuspendCount;
	DWORD Teb;
	DWORD PriorityClass;
	DWORD Priority;
}CRASH_THREAD, *PCRASH_THREAD;




#define hthdxNull ((HTHDX) NULL)


typedef struct _OLERG *POLERG;
typedef enum _ORPCKEYSTATE ORPCKEYSTATE;

typedef struct _OLERET
{
	struct _OLERET *poleretNext;
	UOFFSET         uoffRet;
	UOFFSET         dwEsp;
} OLERET;


typedef struct _OLERET *POLERET;

#ifdef HAS_DEBUG_REGS
typedef struct DEBUGREG {
	DWORDLONG   DataAddr;       //  Data Address
	DWORD       DataSize;       //  Data Size
	BPTP        BpType;         //  read, write, execute, etc
	BOOL        InUse;          //  In use
	DWORD       ReferenceCount;
} DEBUGREG;

typedef struct _CALLSTRUCT CALLSTRUCT, *PCALLSTRUCT;
typedef DEBUGREG *PDEBUGREG;
#endif

typedef struct  _HTHDX {

	//
	// list of all threads
	//
	struct  _HTHDX    *nextGlobalThreadThisProbablyIsntTheOneYouWantedToUse;

	//
	// list of threads in one process
	//
	struct  _HTHDX    *nextSibling;

	HPRCX             hprc;         // OSDebug handle for process
	HTID              htid;         // OSDebug handle for thread
	TID               tid;          // OS thread ID
	HANDLE            rwHand;       // OS handle to thread
	ULONG64           lpStartAddress;// Thread start address
	CONTEXT           context;      // Thread context
	LPVOID            atBP;
	TSTATEX           tstate;
	BOOL              fExceptionHandled;
	DWORDLONG         stackRA;
	DWORDLONG         stackBase;
	int               cFuncEval;
	DWORD             dwExitCode;
	DWORDLONG         offTeb;

	BOOL              fContextDirty;// has the context changed?
	BOOL              fContextStale;// does the context need to be refreshed?

	BOOL              fAddrIsFlat;  // Is this address segmented?
	BOOL              fAddrIsReal;  // Is this address in real mode?
	BOOL              fAddrOff32;   // Is the offset of this addres 32 bits?
	BOOL              fDontStepOff; //

	BOOL              fWowEvent;    // Was the last event WOW?

	ADDR              addrIsCall;
	int               iInstrIsCall;

	EXCEPTION_RECORD64 ExceptionRecord;

	BOOL              fIsCallDone;
	BOOL              fDisplayReturnValues;
	BOOL              fStopOnNLG;
	BOOL              fReturning;

	ADDR              addrFrom;
	ADDR              addrStack;

	WTNODE            wthead;       // root of the call tree for a wt command
	LPWTNODE          wtcurr;       // current wtnode
	DWORD             wtmode;       // wt command executing?

	LIST_ENTRY        WalkList;         //  Walks associated with thread

#ifdef HAS_DEBUG_REGS
	DEBUGREG          DebugRegs[NUMBER_OF_DEBUG_REGISTERS];
#endif



#ifndef KERNEL
	CRASH_THREAD      CrashThread;      // State info from crashdump
	PCALLSTRUCT       pcs;              // used for DM initiated function calling
	POLERET           poleret;
#endif // !KERNEL

} HTHDXSTRUCT, *HTHDX;

typedef struct _BREAKPOINT {
	struct _BREAKPOINT *next;
	HPRCX      hprc;        // The process the BP belongs to
	HTHDX      hthd;        // The thread the BP belongs to
	BPTP       bpType;      // OSDebug BP type
	BPNS       bpNotify;    // OSDebug notify type

	ADDR       addr;        // The address of the Breakpoint
	BP_UNIT    instr1;      // The displaced instruction
	HANDLE     hWalk;       // walk list handle if it is a watchpoint

	BYTE       instances;   // The # of instances that exist
	HPID       id;          // Id supplied by the EM
	BOOL       isStep;      // Single step flag
	DWORD      hBreakPoint; // kernel debugger breakpoint handle
#if defined(TARGET_IA64)
	BPFLG      flags;       // IA64 mode mask
#endif
} BREAKPOINT;
typedef BREAKPOINT *PBREAKPOINT;

typedef void (*ACVECTOR)(DEBUG_EVENT64*, HTHDX, DWORDLONG, DWORDLONG);
typedef void (*DDVECTOR)(DEBUG_EVENT64*, HTHDX);

typedef struct _CALLSTRUCT {
	PBREAKPOINT pbp;
	LPVOID      atBP;
	CONTEXT     context;
	ACVECTOR    Action;
	LPARAM      lparam;
	BOOL        HasReturnValue;
} CALLSTRUCT, *PCALLSTRUCT;

class CDbgApi
{
public:
	CDbgApi();
	~CDbgApi();
	
public: 

	BOOL Init();
	BOOL
	DbgWriteMemory(
		HPRCX       hprc,
		ULONG64     qwOffset,
		LPVOID      lpb,
		DWORD       cb,
		SIZE_T *     pcbWritten
		);

	BOOL DbgReadMemory(
		CHPrcX&		HPrcX,
		ULONG64		qwOffset,
		LPVOID		lpb,
		DWORD		cb,
		SIZE_T *    lpRead
		);

	DWORD
	DmpReadMemory (
		IN PVOID BaseAddress,
		IN PVOID Buffer,
		IN ULONG Size
		);

	DWORD
	DmpWriteMemory (
		IN PVOID BaseAddress,
		IN PVOID Buffer,
		IN ULONG Size
		);

private:
	SYSTEM_INFO          mSystemInfo;
};

#endif