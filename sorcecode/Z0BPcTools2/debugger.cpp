#include "stdafx.h"
#include "debugger.h"
#include "DbgApi.h"
#include <VdmDbg.h>

#define DPRINT(a,b)
#define DPRINT(a,b,c)

CDebugger			g_Debugger;

CDebugger::CDebugger()
	:mfDisconnected(FALSE)
	,mhEventRemoteQuit(NULL)
{
	ZeroMemory(&mDebugActiveStruct,sizeof(mDebugActiveStruct));
	ZeroMemory(&mSpawnStruct,sizeof(mSpawnStruct));
	ZeroMemory(&mDebugActiveEvent,sizeof(mDebugActiveEvent));
}

CDebugger::~CDebugger()
{
	
}

BOOL CDebugger::Init()
{
// 	InitializeCriticalSection(&csProcessDebugEvent);
// 	InitializeCriticalSection(&csThreadProcList);
// 	InitializeCriticalSection(&csEventList);
// 	InitializeCriticalSection(&csWalk);
// 
// 	hEventCreateProcess = CreateEvent(NULL, TRUE, FALSE, NULL);
 	mhEventRemoteQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
// 	hEventContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
// 	hEventNoDebuggee    = CreateEvent(NULL, TRUE, FALSE, NULL);
	mSpawnStruct.hEventApiDone = CreateEvent(NULL, TRUE, FALSE, NULL);
	mDebugActiveStruct.hEventApiDone = CreateEvent(NULL, TRUE, TRUE, NULL);
	mDebugActiveStruct.hEventReady   = CreateEvent(NULL, TRUE, TRUE, NULL);
	mDebugApi = new CDbgApi();

	if (mDebugApi)
	{
		mDebugApi->Init();
	}
	else
		return FALSE;
	

	return TRUE;
}

BOOL CDebugger::DbgActiveProcess(ULONG_PTR ProcessId)
{
	SetPriorityClass(0, HIGH_PRIORITY_CLASS);

	return ::DebugActiveProcess(ProcessId);
}

VOID CDebugger::DmFunc(
					RequestPacket& Packet)
{
	DMF     rpMsg;
	
	rpMsg = (DMF) (Packet.rpMsg & 0xffff);

	switch ( rpMsg ) 
	{
	case dmfSelLim:
		//DPRINT(5, (_T("dmfSelLim\n")));
		//DMAPILOCK();
		//ProcessSelLimCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfSetMulti:
		//DPRINT(5, (_T("dmfSetMulti\n")));
		//LpDmMsg->xosdRet = xosdNone;
		//Reply( 0, LpDmMsg, lpdbb->hpid );
		break;

	case dmfClearMulti:
		//DPRINT(5, (_T("dmfClearMulti\n")));
		//LpDmMsg->xosdRet = xosdNone;
		//Reply( 0, LpDmMsg, lpdbb->hpid );
		break;

	case dmfDebugger:
		//DPRINT(5, (_T("dmfDebugger\n")));
		//LpDmMsg->xosdRet = xosdNone;
		//Reply( 0, LpDmMsg, lpdbb->hpid );
		break;

	case dmfCreatePid:
		//DPRINT(5,(_T("dmfCreatePid\r\n")));
		//ProcessCreateProcessCmd(hprc, hthd, lpdbb);
		break;

	case dmfDestroyPid:
		//DPRINT(5, (_T("dmfDestroyPid\n")));
		//LpDmMsg->xosdRet = FreeProcess(hprc, TRUE);
		//Reply( 0, LpDmMsg, lpdbb->hpid);
		break;

	case dmfSpawnOrphan:
		//DPRINT(5, (_T("dmfSpawnOrphan\n")));
#ifdef KERNEL
		//assert(!"Spawn orphan not allowed in kernel mode");
		//LpDmMsg->xosdRet = xosdUnknown;
		//Reply( 0, LpDmMsg, lpdbb->hpid);
#else
		//ProcessSpawnOrphanCmd (hprc, hthd, lpdbb);
#endif
		break;

	case dmfProgLoad:
		//DPRINT(5, (_T("dmfProgLoad\n")));
		//ProcessProgLoadCmd(hprc, hthd, lpdbb);
		break;

	case dmfProgFree:
		//DPRINT(5, (_T("dmfProgFree\n")));


#ifndef KERNEL

		//ProcessTerminateProcessCmd(hprc, hthd, lpdbb);

#else // KERNEL
// 		if (!hprc) {
// 			LpDmMsg->xosdRet = xosdNone;
// 			Reply( 0, LpDmMsg, lpdbb->hpid);
// 			break;
// 		}
// 
// 		if (KdOptions[KDO_GOEXIT].value) {
// 			HTHDX hthdT;
// 			PBREAKPOINT bp;
// 			KdOptions[KDO_GOEXIT].value = 0;
// 			for (hthdT = hprc->hthdChild; hthdT; hthdT = hthdT->nextSibling) {
// 				if (hthdT->tstate & ts_stopped) {
// 					if (bp = AtBP(hthdT)) {
// 						if (!hthdT->fDontStepOff) {
// 							IncrementIP(hthdT);
// 						}
// 					}
// 					if (hthdT->fContextDirty) {
// 						DbgSetThreadContext( hthdT, &hthdT->context );
// 						hthdT->fContextDirty = FALSE;
// 					}
// 					KdOptions[KDO_GOEXIT].value = 1;
// 					break;
// 				}
// 			}
// 		}
// 
// 		DMAPILOCK();
// 
// 		ClearBps();
// 
// 		ProcessTerminateProcessCmd(hprc, hthd, lpdbb);
// 		DMAPIRELEASE();
// 
// 		ProcessUnloadCmd(hprc, hthd, lpdbb);
// 
// 		if (KdOptions[KDO_GOEXIT].value) {
// 			ContinueTargetSystem( DBG_CONTINUE, NULL );
// 			// Hack: don't exit from here with lock held
// 			DMAPIRELEASE();
// 		}
// 

#endif  // KERNEL

		//LpDmMsg->xosdRet = xosdNone;
		//Reply( 0, LpDmMsg, lpdbb->hpid);
		break;

	case dmfBreakpoint:
		//DPRINT(5,(_T("dmfBreakpoint\r\n")));
		//DMAPILOCK();
		//ProcessBreakpointCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfReadMem:
		// This replies in the function
		//DPRINT(5, (_T("dmfReadMem\n")));
		//DMAPILOCK();
		//ProcessReadMemoryCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfWriteMem:
		//DPRINT(5, (_T("dmfWriteMem\n")));
		//DMAPILOCK();
		//ProcessWriteMemoryCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfReadReg:
		//DPRINT(5, (_T("dmfReadReg\n")));
		//DMAPILOCK();
		//ProcessGetContextCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfWriteReg:
		//DPRINT(5, (_T("dmfWriteReg\n")));
		//DMAPILOCK();
		//ProcessSetContextCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;


#ifdef HAS_DEBUG_REGS
	case dmfReadRegEx:
		//DPRINT(5, (_T("dmfReadRegEx\n")));
		//DMAPILOCK();
#ifdef KERNEL
		//ProcessGetExtendedContextCmd(hprc, hthd, lpdbb);
#else
		//ProcessGetDRegsCmd(hprc, hthd, lpdbb);
#endif
		//DMAPIRELEASE();
		break;

	case dmfWriteRegEx:
		//DPRINT(5, (_T("dmfWriteRegEx\n")));
		//DMAPILOCK();
#ifdef KERNEL
		//ProcessSetExtendedContextCmd(hprc, hthd, lpdbb);
#else
		//ProcessSetDRegsCmd(hprc, hthd, lpdbb);
#endif
		//DMAPIRELEASE();
		break;

#else   // HAS_DEBUG_REGS
	case dmfReadRegEx:
	case dmfWriteRegEx:
		//DPRINT(5,(_T("Read/WriteRegEx\r\n")));
		//assert(dmf != dmfReadRegEx && dmf != dmfWriteRegEx);
		//LpDmMsg->xosdRet = xosdUnknown;
		//Reply( 0, LpDmMsg, lpdbb->hpid );
		break;
#endif  // HAS_DEBUG_REGS

	case dmfGo:
		//DPRINT(5, (_T("dmfGo\n")));
#if !defined(KERNEL)
		//Turn off fiber debugging
		//hprc->pFbrCntx = NULL;
#endif
		//
		// We should not have to lock this, since it should
		// be a nop if the target is running anyway.
		//
		//ProcessContinueCmd(hprc, hthd, lpdbb);
		break;

#if defined(KERNEL)
	case dmfTerm:
		//DPRINT(5, (_T("dmfTerm\n")));
		//DMAPILOCK();
		//ProcessTerminateProcessCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;
#endif

	case dmfStop:
		//DPRINT(5, (_T("dmfStop\n")));
		//ProcessAsyncStopCmd(hprc, hthd, lpdbb);
		break;

	case dmfFreeze:
		//DPRINT(5, (_T("dmfFreeze\n")));
		//ProcessFreezeThreadCmd(hprc, hthd, lpdbb);
		break;

	case dmfResume:
		//DPRINT(5, (_T("dmfResume\n")));
		//ProcessAsyncGoCmd(hprc, hthd, lpdbb);
		break;

	case dmfInit:
		//DPRINT(5, (_T("dmfInit\n")));
		//Reply( 0, &xosd, lpdbb->hpid);
		break;

	case dmfUnInit:
		//DPRINT(5, (_T("dmfUnInit\n")));
#ifdef KERNEL
		//DmPollTerminate();
#endif
		//Reply ( 1, LpDmMsg, lpdbb->hpid);
#ifndef KERNEL
		//Cleanup();
#endif
		break;

	case dmfGetDmInfo:
		//DPRINT(5,(_T("getDmInfo\r\n")));
		//ProcessGetDmInfoCmd(hprc, lpdbb, cb);
		break;

	case dmfSetupExecute:
		//DPRINT(5, (_T("dmfSetupExecute\n")));
		//DMAPILOCK();
		//ProcessSetupExecuteCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfStartExecute:
		//DPRINT(5, (_T("dmfStartExecute\n")));
		//DMAPILOCK();
		//ProcessStartExecuteCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfCleanUpExecute:
		//DPRINT(5, (_T("dmfCleanupExecute\n")));
		//DMAPILOCK();
		//ProcessCleanUpExecuteCmd(hprc, hthd, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfSystemService:
		//DPRINT(5, (_T("dmfSystemService\n")));
		//ProcessSystemServiceCmd( hprc, hthd, lpdbb );
		break;

	case dmfDebugActive:
		//DPRINT(5, (_T("dmfDebugActive\n")));
		ProcessDebugActiveCmd( Packet);
		break;

	case dmfSetPath:
		//DPRINT(5, (_T("dmfSetPath\n")));
		//ProcessSetPathCmd( hprc, hthd, lpdbb );
		break;

	case dmfQueryTlsBase:
		//DPRINT(5, (_T("dmfQueryTlsBase\n")));
		//ProcessQueryTlsBaseCmd(hprc, hthd, lpdbb );
		break;

	case dmfQuerySelector:
		//DPRINT(5, (_T("dmfQuerySelector\n")));
		//DMAPILOCK();
		//ProcessQuerySelectorCmd(hprc, hthd, lpdbb );
		//DMAPIRELEASE();
		break;

	case dmfVirtualQuery:
		//DPRINT(5,(_T("VirtualQuery\r\n")));
		//DMAPILOCK();
		//ProcessVirtualQueryCmd(hprc, lpdbb);
		//DMAPIRELEASE();
		break;

	case dmfRemoteQuit:
		//DPRINT(5,(_T("RemoteQuit\r\n")));
		//ProcessRemoteQuit();
		break;

#ifdef KERNEL
	case dmfGetSections:
		//DPRINT(5,(_T("GetGetSections\r\n")));
		//DMAPILOCK();
		//ProcessGetSectionsCmd( hprc, hthd, lpdbb );
		//DMAPIRELEASE();
		break;
#endif

	case dmfSetExceptionState:
		//DPRINT(5,(_T("SetExceptionState\r\n")));
		//ProcessSetExceptionState(hprc, hthd, lpdbb);
		break;

	case dmfGetExceptionState:
		//DPRINT(5,(_T("GetExceptionState\r\n")));
		//ProcessGetExceptionState(hprc, hthd, lpdbb);
		break;

	case dmfSingleStep:
		//DPRINT(5,(_T("SingleStep\r\n")));
#if !defined(KERNEL)
		//Turn off fiber debugging
		//hprc->pFbrCntx = NULL;
#endif
		//
		// again, this should not do anything unless the target is stopped,
		// so don't lock it.
		//
		//ProcessSingleStepCmd(hprc, hthd, lpdbb);
		break;

	case dmfRangeStep:
		//DPRINT(5,(_T("RangeStep\r\n")));
#if !defined(KERNEL)
		//Turn off fiber debugging
		//hprc->pFbrCntx = NULL;
#endif
		//ProcessRangeStepCmd(hprc, hthd, lpdbb);
		break;

	case dmfReturnStep:
		//DPRINT(5,(_T("ReturnStep\r\n")));
		//ProcessReturnStepCmd(hprc, hthd, lpdbb);
		break;

#ifndef KERNEL
	case dmfNonLocalGoto:
		//DPRINT(5,(_T("NonLocalGoto\r\n")));
		//Turn off fiber debugging
		//hprc->pFbrCntx = NULL;
		//ProcessNonLocalGoto(hprc, hthd, lpdbb);
		break;
#endif

	case dmfThreadStatus:
		//DPRINT(5,(_T("ThreadStatus\r\n")));
		//Reply( ProcessThreadStatCmd(hprc, hthd, lpdbb), LpDmMsg, lpdbb->hpid);
		break;

	case dmfProcessStatus:
		//DPRINT(5,(_T("ProcessStatus\r\n")));
// 		if (hprc) {
// 			LpDmMsg->xosdRet = xosdNone;
// 			Reply( ProcessProcStatCmd(hprc, hthd, lpdbb), LpDmMsg, lpdbb->hpid);
// 		} else {
// 			LpDmMsg->xosdRet = xosdBadProcess;
// 			Reply( 0, LpDmMsg, lpdbb->hpid);
// 		}
		break;

	case dmfGetTimeStamp:
		//DPRINT(5,(_T("ProcessGetTimeStamp\r\n")));
		//Reply (ProcessGetTimeStamp (hprc, hthd, lpdbb), LpDmMsg, lpdbb->hpid);
		break;

	case dmfNewSymbolsLoaded:
#ifdef KERNEL
		//GetKernelSymbolAddresses();
#endif
		//Reply( 0, LpDmMsg, lpdbb->hpid);
		break;

	default:
		//DPRINT(5, (_T("DM: Unknown dmf == %d\n"), dmf));
		//assert(FALSE);
		break;
	}

	return;
}

BOOL CDebugger::StartDebugProcess(RequestPacket& Packet)
{
	Packet.rpMsg = (dmfDebugActive & 0xFFFF);

	DmFunc( Packet);
	return FALSE;
}

void CDebugger::ProcessDebugActiveCmd(RequestPacket& Packet)
{
	if (CheckPacketFlags(Packet, kPacketDebugCreateProcess))
	{
		//CREATE_DEFAULT_ERROR_MODE
		PROCESS_INFORMATION pi = {0};
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		if (Packet.rpDataSize)
		{
			DebugCreateProcess* DebugProcess = (DebugCreateProcess*)&Packet.rpData[0];
			SetDebugPrivilege();
			mSpawnStruct.fReturn = 
				CreateProcess(
						NULL,
						DebugProcess->ProcessPath,
						//DebugProcess->CreateArg,
						NULL,
						NULL,
						DebugProcess->InheritHandles,
						DebugProcess->CreateFlags,
						NULL,
						DebugProcess->ProcessDirectory,
						&mSpawnStruct.si,
						&pi
						);

			if (mSpawnStruct.fReturn == 0) {
				mSpawnStruct.dwError = GetLastError();
			} else {
				mDebugActiveStruct.dwProcessId = pi.dwProcessId;
				mSpawnStruct.dwError = 0;
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}

		}
		
		SetEvent(mSpawnStruct.hEventApiDone);

	}

	if ( CheckPacketFlags(Packet,kPacketDebugActiveProcess))
	{
		mDebugActiveStruct.fAttach = FALSE;
		SetDebugPrivilege ();
		mDebugActiveStruct.fReturn = DbgActiveProcess(mDebugActiveStruct.dwProcessId);

		if (mDebugActiveStruct.fReturn == 0) {
			mDebugActiveStruct.dwError = GetLastError();
		} else {
			mDebugActiveStruct.dwError = 0;
			mDebugActiveEvent.ActiveProcessId = mDebugActiveStruct.dwProcessId;
		}

		 SetEvent(mDebugActiveStruct.hEventApiDone);
	}
}

ULONG CDebugger::DmPoll(LPVOID lpv)
{
	DEBUG_EVENT64 de;
	PROCESS_INFORMATION pi;
	int         nprocs = 0;
	UINT        ErrMode;

	 SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	 if (mSpawnStruct.fSpawn) {
		 mSpawnStruct.fSpawn = FALSE;
		 ErrMode = SetErrorMode( 0 );

		 mSpawnStruct.fReturn =
			 CreateProcess( mSpawnStruct.szAppName,
			 mSpawnStruct.szArgs,
			 NULL,
			 NULL,
			 mSpawnStruct.fInheritHandles,
			 mSpawnStruct.fdwCreate,
			 NULL,
			 mSpawnStruct.pszCurrentDirectory,
			 &mSpawnStruct.si,
			 &pi
			 );

		 SetErrorMode( ErrMode );
		 if (mSpawnStruct.fReturn == 0) {
			 mSpawnStruct.dwError = GetLastError();
		 } else {
			 mSpawnStruct.dwError = 0;
			 CloseHandle(pi.hThread);
			 CloseHandle(pi.hProcess);
			// Close3Handles(rgh);
		 }

		 SetEvent(mSpawnStruct.hEventApiDone);
	 }

	 if (mDebugActiveStruct.fAttach) {
		 if (mfDisconnected && mDebugActiveStruct.dwProcessId == 0) {
			 SetEvent( mhEventRemoteQuit );
			 SetEvent( mDebugActiveStruct.hEventReady );
			 mDebugActiveStruct.fReturn = 1;
			 mDebugActiveStruct.dwError = 0;
		 } else {
			 mDebugActiveStruct.fAttach = FALSE;
			 SetDebugPrivilege ();
			 mDebugActiveStruct.fReturn = DebugActiveProcess(mDebugActiveStruct.dwProcessId);
			 if (mDebugActiveStruct.fReturn == 0) {
				 mDebugActiveStruct.dwError = GetLastError();
			 } else {
				 mDebugActiveStruct.dwError = 0;
			 }
		 }
		 SetEvent(mDebugActiveStruct.hEventApiDone);
	 }

	return ErrMode;
}

BOOL
CDebugger::SetDebugPrivilege(
    void
    )
{
    HANDLE  hToken;
    LUID    DebugValue;
    TOKEN_PRIVILEGES tkp;
    BOOL    rVal = TRUE;

    if (!OpenProcessToken(GetCurrentProcess(),
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return FALSE;
    }

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &DebugValue)) {

        rVal = FALSE;

    } else {

        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Luid = DebugValue;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        AdjustTokenPrivileges(hToken,
            FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);

        if (GetLastError() != ERROR_SUCCESS) {
            rVal = FALSE;
        }
    }

    CloseHandle(hToken);

    return rVal;
}

DebugProcess* CDebugger::HPRCFromPID(
	PID pid
	)
{
	if (pid == mDebugActiveEvent.ActiveProcessId)
	{
		return &mDebugActiveEvent.ActiveProcess;
	}

	return NULL;
	
}

DebugActiveThread*
CDebugger::HTHDXFromPIDTID(
	PID pid,
	TID tid
	)
{
	DebugActiveThread* ActiveThread = mDebugActiveEvent.ActiveThreadList.Head();

	for (  ; ActiveThread > 0 ;ActiveThread = mDebugActiveEvent.ActiveThreadList.Next(ActiveThread))
	{
		if ( ActiveThread->ThreadId == tid && ActiveThread->ProcessId == pid)
		{			
			return ActiveThread;			
		}
	}

	return NULL;
}

void CDebugger::ProcessAttachProcess()
{

}

BOOL CDebugger::AttachProcessId(ULONG_PTR ProcessId)
{
	RequestPacket Packet;
	Packet.hpid = (void*)ProcessId;
	Packet.rpFlags = kPacketDebugActiveProcess;
	Packet.rpDataSize = 0;
	Packet.rpMsgError = 0;

	mDebugActiveStruct.dwProcessId = ProcessId;

	return StartDebugProcess(Packet);
}

void
CDebugger::UpdateThreadContext(
	DebugActiveThread*   hthd
	)
{
	hthd->ThreadContext.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT;
	DbgGetThreadContext( hthd, &hthd->ThreadContext );


}

BOOL
CDebugger::TranslateAddress(
	DebugProcess*		hprc,
	DebugActiveThread*  hthd,
	LPADDR lpaddr,
	BOOL   f16ToFlat
	)
{
	if (hthd == 0) {
		for (hthd = hprc->ChildThread.Head(); hthd != NULL; hthd = hprc->ChildThread.Next(hthd)) {
			if (hthd->State & ts_stopped) {
				break;
			}
		}

		if (hthd == 0) {
			hthd = hprc->ChildThread.Head();
		}
	}

	ADDR_IS_FLAT(*lpaddr) = TRUE;
	ADDR_IS_REAL(*lpaddr) = FALSE;
	return TRUE;
}

BOOL
CDebugger::AddrReadMemory(
	DebugProcess*			hprc,
	DebugActiveThread*      hthd,
	LPADDR					paddr,
	LPVOID					lpv,
	DWORD					cb,
	LPDWORD					lpRead
	)
{
	BOOL        fRet;
	ADDR        addr;

	if (ADDR_IS_LI(*paddr)) {
		return FALSE;
	}

	addr = *paddr;
	if (!ADDR_IS_FLAT(addr)) {
		fRet = TranslateAddress(hprc, hthd, &addr, TRUE);
		//assert(fRet);
		if (!fRet) {
			return fRet;
		}
	}


// 	if (!DbgReadMemory(hprc, GetAddrOff(addr), lpv, cb, lpRead)) {
// 		return FALSE;
// 	}


	return TRUE;
}

void 
CDebugger::AddrFromHthdx(PVOID* paddr,DebugActiveThread* hthd)
{
// 	AddrInit(
// 		paddr,
// 		0,
// 		PcSegOfHthdx(hthd),
// 		PC(hthd),
// 		(BOOL)hthd->fAddrIsFlat,
// 		(BOOL)hthd->fAddrOff32,
// 		FALSE,                  
// 		(BOOL)hthd->fAddrIsReal);
}

BOOL
CDebugger::DecodeSingleStepEvent(
	DebugActiveThread* hthd,
	DEBUG_EVENT64 *de,
	PDWORD eventCode,
	PDWORD_PTR subClass
	)
{
	return TRUE;
}

VOID CDebugger::ProcessDebugEvent()
{
	
	ULONG64 ThreadId = mDebugActiveEvent.ActiveDebugEvent.dwThreadId;
	DebugProcess* ActiveProcess = HPRCFromPID(mDebugActiveEvent.ActiveDebugEvent.dwProcessId);
	ULONG   EventCode = mDebugActiveEvent.ActiveDebugEvent.dwDebugEventCode;
	ULONG64 SubClass = 0;
	DebugActiveThread* ActiveThread = NULL;
	ADDR                addr;
	if (ActiveProcess)
	{

		if (ActiveProcess->State & ps_killed)
		{
			if (EventCode == EXCEPTION_DEBUG_EVENT)
			{
				ContinueDebugEvent(
					mDebugActiveEvent.ActiveDebugEvent.dwProcessId,
					mDebugActiveEvent.ActiveDebugEvent.dwThreadId,
					DBG_EXCEPTION_NOT_HANDLED);

				return;
			}
			else if (EventCode != EXIT_THREAD_DEBUG_EVENT
				&& EventCode != EXIT_PROCESS_DEBUG_EVENT )
			{
				ContinueDebugEvent(
					mDebugActiveEvent.ActiveDebugEvent.dwProcessId,
					mDebugActiveEvent.ActiveDebugEvent.dwThreadId,
					DBG_EXCEPTION_NOT_HANDLED);

				return;
			}
		}

		if (EventCode == CREATE_THREAD_DEBUG_EVENT)
		{

// 			DPRINT(3, (_T("*** NEW TID = (PID,TID)(%08lx, %08lx)\n"),
// 				de->dwProcessId, de->dwThreadId));

		}
		else
		{
			ActiveThread = HTHDXFromPIDTID(
								(PID)mDebugActiveEvent.ActiveDebugEvent.dwProcessId,
								(TID)mDebugActiveEvent.ActiveDebugEvent.dwThreadId);

			if (ActiveThread && EventCode == EXCEPTION_DEBUG_EVENT)
			{
				memcpy(&ActiveThread->ExceptionRecord,
						&mDebugActiveEvent.ActiveDebugEvent.u.Exception.ExceptionRecord,
						sizeof(EXCEPTION_RECORD64));
			}

			if (ActiveThread && ActiveProcess)
			{
 				DebugActiveThread*   hthdT;
 			
 				ActiveProcess->FbrCntx = NULL;
// 
// 
 				for (hthdT = ActiveProcess->ChildThread.Head(); hthdT; hthdT = ActiveProcess->ChildThread.Next(hthdT)) {
 					UpdateThreadContext (hthdT);
 				}
			} 
			else if (ActiveProcess && (ActiveProcess->State & ps_killed)) 
			{
				if (EventCode == EXIT_PROCESS_DEBUG_EVENT) {
					/* Process exited on a thread we didn't pick up */
					//ProcessExitProcessEvent(de, NULL);

				} else {
					/* this is an exit thread for a thread we never picked up */
					
					ContinueDebugEvent(
						mDebugActiveEvent.ActiveDebugEvent.dwProcessId,
						mDebugActiveEvent.ActiveDebugEvent.dwThreadId,
						DBG_CONTINUE);
				}

				//goto done;
			} 
			else if(EventCode!=CREATE_PROCESS_DEBUG_EVENT) 
			{
				ContinueDebugEvent(
					mDebugActiveEvent.ActiveDebugEvent.dwProcessId,
					mDebugActiveEvent.ActiveDebugEvent.dwThreadId,
					DBG_CONTINUE);

				//goto done;
			}
		}

		if (ActiveThread) {
			//hthd->tstate &= ~ts_running;
			//hthd->tstate |= ts_stopped;
			ActiveThread->State = (TSTATEX)((ULONG)ActiveThread->State & (~ts_running));
			ActiveThread->State = (TSTATEX)((ULONG)ActiveThread->State | (ts_stopped));
		}

		if (EventCode == EXCEPTION_DEBUG_EVENT) 
		{

		}

		//ActiveThread = DbgGetThreadContext(ThreadId);

		switch (mDebugActiveEvent.ActiveDebugEvent.dwDebugEventCode)
		{
		case EXCEPTION_DEBUG_EVENT:
			{
				SubClass = mDebugActiveEvent.ActiveDebugEvent.u.Exception.ExceptionRecord.ExceptionCode;

				switch (SubClass)
				{
				case STATUS_SEGMENT_NOTIFICATION:
					//EventCode = de->dwDebugEventCode = SEGMENT_LOAD_DEBUG_EVENT;
					break;

				case EXCEPTION_SINGLE_STEP:
					{
						AddrFromHthdx((PVOID*)&addr, ActiveThread);

						//
						// This may be a single step or a hardware breakpoint.
						// If it is a single step, leave it at that.  If it is a
						// hardware breakpoint, convert it to a BREAKPOINT_DEBUG_EVENT.
						//

						DecodeSingleStepEvent( 
											ActiveThread,
											(DEBUG_EVENT64*)&mDebugActiveEvent.ActiveDebugEvent,
											&EventCode, 
											(PDWORD_PTR)&SubClass );
					}
					break;
				case EXCEPTION_BREAKPOINT:
					{
						AddrFromHthdx((PVOID*)&addr, ActiveThread);
/*
						if ((AddrReadMemory(hprc, ActiveThread, &addr, &instr, BP_SIZE, &len) == 0)
							|| (len != BP_SIZE)) {
								//DPRINT(0, ("Memory read failed @ %I64x!!!\n",GetAddrOff(addr)));
								//assert(FALSE);
								instr = 0;
						}

						fInstrIsBp = FALSE;
						if (instr == BP_OPCODE) {
							fInstrIsBp = TRUE;
						} else if (instr == 0x3) { // 0xcd?
							--addr.addr.off;
							if (AddrReadMemory(hprc,
								ActiveThread,
								&addr,
								&instr,
								1,
								&len)
								&& (len == 1)
								&& (instr == 0xcd)) {

									fInstrIsBp = TRUE;
							} else {
								++addr.addr.off;
							}
						}

						Set_PC(ActiveThread, addr.addr.off);
						ActiveThread->fContextDirty = TRUE;
*/
						//DPRINT(3, (_T("Looking for BP@%I64x (instr=%I64x)\n"), addr.addr.off,
						//	(ULONG64)instr));
					}
					break;
				}
				DbgProcessExceptionEvent(ActiveThread);
			}
			break;
		case CREATE_THREAD_DEBUG_EVENT:
			{

			}
			break;
		case CREATE_PROCESS_DEBUG_EVENT:
			{

			}
			break;
		case EXIT_THREAD_DEBUG_EVENT:
			{

			}
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			{

			}
			break;
		case LOAD_DLL_DEBUG_EVENT:
			{

			}
			break;
		case UNLOAD_DLL_DEBUG_EVENT:
			{

			}
			break;
		case OUTPUT_DEBUG_STRING_EVENT:
			{

			}
			break;
		case RIP_EVENT:
			{

			}
			break;
		default:
			{

			}
			break;
		}
		DWORD dwDbgContinue = DBG_CONTINUE;
		if (mDebugActiveEvent.ActiveDebugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
		{
			dwDbgContinue = DBG_EXCEPTION_NOT_HANDLED;
		}

		ContinueDebugEvent(
			mDebugActiveEvent.ActiveDebugEvent.dwProcessId,
			mDebugActiveEvent.ActiveDebugEvent.dwThreadId,
			dwDbgContinue);
	}	
}

BOOL CDebugger::DbgGetThreadContext(DebugActiveThread* DbgThread ,LPCONTEXT Context)
{
	if (DbgThread && Context)
	{
		return GetThreadContext(DbgThread->ThreadHandle,Context);
	}
	return FALSE;
}

DebugActiveThread* CDebugger::DbgGetThreadContext( ULONG64 ThreadId )
{
	DebugActiveThread* ActiveThread = mDebugActiveEvent.ActiveThreadList.Head();

	for (  ; ActiveThread > 0 ;ActiveThread = mDebugActiveEvent.ActiveThreadList.Next(ActiveThread))
	{
		if ( ActiveThread->ThreadId == ThreadId)
		{
			ActiveThread->InitThreadContext = FALSE;
			ZeroMemory(
				&ActiveThread->ThreadContext,
				sizeof(CONTEXT));

			ActiveThread->ThreadContext.ContextFlags = VDMCONTEXT_FULL;

			if ( GetThreadContext(
						ActiveThread->ThreadHandle,
						&ActiveThread->ThreadContext))
			{
					return ActiveThread;
			}
		}
	}


	ActiveThread = (DebugActiveThread*)malloc(sizeof(DebugActiveThread));
	if (ActiveThread)
	{
		ActiveThread->InitThreadContext = TRUE;
		ActiveThread->ThreadId = ThreadId;
		
		return ActiveThread;
	}

	return NULL;
}

BOOL CDebugger::WaitDebugMsg()
{
	return DbgWaitDebugEvent(&mDebugActiveEvent.ActiveDebugEvent,0);
}

BOOL
#ifdef WIN64
	DbgWaitDebugEvent(LPDEBUG_EVENT64 DgbEvent,DWORD TimeOut)
#else
	CDebugger::DbgWaitDebugEvent(LPDEBUG_EVENT DgbEvent,DWORD TimeOut)
#endif
{

	BOOL Rval = FALSE;
#ifdef WIN64
	Rval = WaitForDebugEvent((LPDEBUG_EVENT)DgbEvent, TimeOut);
#else

	Rval = WaitForDebugEvent(DgbEvent, TimeOut);

	if (Rval) {
		//DebugEvent32To64((LPDEBUG_EVENT32)&DgbEvent, de64);
	}

#endif

	return Rval;

}

void CDebugger::DbgProcessExceptionEvent(DebugActiveThread* ActiveThread)
{
#ifdef WIN64
	EXCEPTION_DEBUG_INFO64 * Exception = &mDebugActiveEvent.ActiveDebugEvent.u.Exception;
#else
	EXCEPTION_DEBUG_INFO* Exception = &mDebugActiveEvent.ActiveDebugEvent.u.Exception;
#endif
	

	if (ActiveThread)
	{
	}
	else
	{

	}

	if ( Exception->ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT )
	{

	}
	else if ( Exception->ExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP )
	{
	}
}

BOOL CDebugger::DbgReadMemory(
			void* pBuffer,
			void* pBaseAdress,
			DWORD ReadSize,
			DWORD ReadType)
{
	if (!pBuffer || !pBaseAdress || !ReadSize)
	{
		return FALSE;
	}

	ADDR ExceptAddress;
	ExceptAddress.Addr.off = (UOFF64)pBaseAdress;

	CHPrcX		HPrcX;
	ULONG64		qwOffset;
	LPVOID		lpb;
	DWORD		cb;
	SIZE_T		lpRead;

	lpb = pBuffer;
	cb = ReadSize;
	lpRead = 0;
	qwOffset = (ULONG64)pBaseAdress;

	if (!mDebugApi->DbgReadMemory(HPrcX,qwOffset,lpb,cb,&lpRead))
	{
		return FALSE;
	}

	
	for ( int i = 0 ; i < mBreakPointList.Count() ; )
	{

	}

	return TRUE;
}