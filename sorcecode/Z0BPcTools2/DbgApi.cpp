#include "stdafx.h"
#include "DbgApi.h"

ULONG ValidPteMask           = X86_VALID;
ULONG TransitionMask         = X86_TRANSITION_MASK;
ULONG TransitionCheck        = X86_TRANSITION_CHECK;
ULONG ValidPfnMask           = X86_VALID_PFN_MASK;
ULONG ValidPfnShift          = X86_VALID_PFN_SHIFT;
ULONG TransitionPfnMask      = X86_TRANS_PFN_MASK;
ULONG TransitionPfnShift     = X86_TRANS_PFN_SHIFT;
ULONG PdeShift               = X86_PDE_SHIFT;
ULONG PteShift               = X86_PTE_SHIFT;
ULONG PteMask                = X86_PTE_MASK;
ULONG PhysicalAddressMask    = X86_PHYSICAL_MASK;
ULONG PhysicalAddressStart   = X86_PHYSICAL_START;
ULONG PhysicalAddressEnd     = X86_PHYSICAL_END;
ULONG PageSize               = X86_PAGESIZE;
ULONG PageShift              = X86_PAGESHIFT;
ULONG LargePageMask          = X86_LARGE_PAGE_MASK;
ULONG LargePageSize          = X86_LARGE_PAGE_SIZE;


CDbgApi::CDbgApi()
{
	ZeroMemory(&mSystemInfo,sizeof(mSystemInfo));
	GetSystemInfo(&mSystemInfo);
}

CDbgApi::~CDbgApi()
{

}

BOOL CDbgApi::Init()
{
	return TRUE;
}

PVOID
VaToLocation (
    IN PVOID VirtualAddress
    )

{
    ULONG   PdeOffset;
    ULONG   PteOffset;
    PULONG  PtePage;
    PVOID   VaPage;
    ULONG   Pfn;
    ULONG   i;
    ULONG   Offset;
	BOOL    UserModeDump = TRUE;

    if (UserModeDump) {

		/*
        Offset = 0;
        for (i=0; i<DumpHeaderUser->MemoryRegionCount; i++) {
            if ((ULONG)VirtualAddress >= (ULONG)MemoryInfo[i].BaseAddress &&
                (ULONG)VirtualAddress < (ULONG)MemoryInfo[i].BaseAddress+MemoryInfo[i].RegionSize) {
                return (PVOID)((ULONG)MemoryData+Offset+((ULONG)VirtualAddress-(ULONG)MemoryInfo[i].BaseAddress));
            }
            Offset += MemoryInfo[i].RegionSize;
        }
		*/
        return NULL;
    }

	/*
    if (((ULONG)VirtualAddress >= PhysicalAddressStart) &&
        ((ULONG)VirtualAddress <  PhysicalAddressEnd)) {

        VaPage = PageToLocation (GetPhysicalPage (VirtualAddress));

        return (PVOID)((PCHAR)VaPage + ((ULONG)VirtualAddress & (PageSize - 1)));

    }

    PdeOffset = (ULONG)VirtualAddress >> PdeShift;
    PteOffset = ((ULONG)VirtualAddress >> PteShift) & PteMask;

    if (DmpPdePage[PdeOffset] & ValidPteMask) {

        //
        // PDE is valid.  Check for Large Pages.
        //

        if (DmpPdePage[PdeOffset] & LargePageMask) {

            Pfn = ((DmpPdePage[PdeOffset] & ~(LargePageSize - 1)) |
                    ((ULONG)VirtualAddress & (LargePageSize - 1))) >> PageShift;

        } else {

            //
            // Lookup page via PTE.
            //

            PtePage = (PULONG)PageToLocation(PteToPfn(DmpPdePage[PdeOffset]));
            if (PtePage == NULL) {
                return NULL;
            }

            Pfn = PteToPfn( PtePage[PteOffset] );
            if (Pfn == 0xFFFFFFFF) {
                return NULL;
            }
        }

        VaPage = PageToLocation( Pfn );

        if (VaPage == NULL) {
            return NULL;
        }
        return (PVOID)((PCHAR)VaPage + ((ULONG)VirtualAddress &
                                                            (PageSize - 1)));
    }
	*/
    return NULL;
}


BOOL
CDbgApi::DbgWriteMemory(
    HPRCX       hprc,
    ULONG64     qwOffset,
    LPVOID      lpb,
    DWORD       cb,
    SIZE_T *     pcbWritten
    )
{
    BOOL        fRet;
    LPVOID lpOffset = (LPVOID)qwOffset;

    //assert(hprc->rwHand != (HANDLE)-1);

    // For now we will not modify fibers
    /*if ((hprc->pFbrCntx) && (hprc->fUseFbrs)) {
        return FALSE;
    }*/

    if (hprc->rwHand == (HANDLE)-1) {
        return FALSE;
    }

    fRet = ::WriteProcessMemory(hprc->rwHand, lpOffset, lpb, cb, pcbWritten);

#if  defined(JLS_RWBP) && DBG
    {
        DWORD   cbT;
        LPBYTE  lpbT = malloc(cb);

        //assert( fRet );
       // assert( *pcbWritten == cb );
        fRet = ReadProcessMemory(hprc->rwHand, lpOffset, lpbT, cb, &cbT);
       // assert(fRet);
        //assert( cb == cbT);
        //assert(memcmp(lpbT, lpb) == 0);
        free lpbT;
    }
#endif

    return fRet;
}

DWORD
CDbgApi::DmpReadMemory (
    IN PVOID BaseAddress,
    IN PVOID Buffer,
    IN ULONG Size
    )
{
    PCHAR Location;
    PCHAR OutBuffer;
    ULONG BytesCopied = 0;
    PCHAR VirtualAddress;

	
    __try {
        VirtualAddress = (PCHAR)BaseAddress;
        OutBuffer = (PCHAR)Buffer;

        while ((Location = (PCHAR)VaToLocation(VirtualAddress)) && (Size > 0)) {
            if (OutBuffer) {
                *OutBuffer++ = *Location;
            }
            Size -= 1;
            BytesCopied += 1;
            VirtualAddress += 1;
        }

    } __except(EXCEPTION_EXECUTE_HANDLER) {
        BytesCopied = 0;
    }

    return BytesCopied;
}


DWORD
CDbgApi::DmpWriteMemory (
    IN PVOID BaseAddress,
    IN PVOID Buffer,
    IN ULONG Size
    )
{
    PCHAR Location;
    PCHAR OutBuffer;
    ULONG BytesCopied = 0;
    PCHAR VirtualAddress;
    ULONG Protect;


    __try {
        VirtualAddress = (PCHAR)BaseAddress;
        OutBuffer = (PCHAR)Buffer;

        while (Size) {
           // Location = (char *)VaToLocation( VirtualAddress );
            if (!Location) {
                break;
            }
            VirtualProtect( Location, mSystemInfo.dwPageSize, PAGE_WRITECOPY, &Protect );
            *Location = *OutBuffer++;
            Size -= 1;
            BytesCopied += 1;
            VirtualAddress += 1;
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        BytesCopied = 0;
    }

    return BytesCopied;
}



BOOL
CDbgApi::DbgReadMemory(
    CHPrcX&		HPrcX,
    ULONG64		qwOffset,
    LPVOID		lpb,
    DWORD		cb,
    SIZE_T *    lpRead
    )
{
    DWORD cbr;
    LPVOID lpOffset = (LPVOID)qwOffset;

    if (HPrcX.IsInner()) 
	{

        cbr = DmpReadMemory( (LPVOID)qwOffset, lpb, cb );
        if (lpRead) 
		{
            *lpRead = cbr;
        }

        return (cbr > 0) || (cbr == cb);
    }

    if (HPrcX.mOpenHand == (HANDLE)-1)
	{
        return FALSE;
    }

    if (ReadProcessMemory(HPrcX.mOpenHand, lpOffset, lpb, cb, lpRead))
	{
        return TRUE;
    } 
	else
	{

#if DBG
        int e = GetLastError();
#endif
        //
        //  Reads across page boundaries will not work if the
        //  second page is not accessible.
        //
//#define PAGE_SIZE (SystemInfo.dwPageSize)
#define PAGE_MASK (~(DWORD_PTR)(PAGE_SIZE-1))

        DWORD firstsize;
        SIZE_T  dwRead;

        firstsize = (DWORD)((((DWORD_PTR)lpOffset + PAGE_SIZE) & PAGE_MASK) - (DWORD_PTR)lpOffset);
        if (cb < firstsize) 
		{
            firstsize = cb;
        }

        //DPRINT(0,("ReadMemory @%p %i bytes failed. Error:%i Now trying reading %i bytes\n",lpOffset,cb, e, firstsize));
        //
        // read from the first page.  If the first read fails,
        // fail the whole thing.
        //

        if (!ReadProcessMemory(HPrcX.mOpenHand,
                               lpOffset,
                               lpb,
                               firstsize,
                               lpRead)) 
		{
           //DPRINT(0,("ReadMemory @%p %i bytes failed. Error:%i \n",lpOffset, firstsize, GetLastError()));
            return FALSE;
        }

        //
        // read intermediate complete pages.
        // if any of these reads fail, succeed with a short read.
        //

        //assert(*lpRead == firstsize);
        cb -= firstsize;
        lpb = (LPVOID)((LPBYTE)lpb + firstsize);
        lpOffset = (LPVOID)((LPBYTE)lpOffset + firstsize);

        while (cb >= PAGE_SIZE) 
		{

            if (!ReadProcessMemory(HPrcX.mOpenHand, lpOffset, lpb, PAGE_SIZE,
                                                                 &dwRead)) 
			{
                return TRUE;
            }
			else
			{
               // assert(dwRead == PAGE_SIZE);
                lpb = (LPVOID)((LPBYTE)lpb + PAGE_SIZE);
                lpOffset = (LPVOID)((LPBYTE)lpOffset + PAGE_SIZE);
                *lpRead += dwRead;
                cb -= PAGE_SIZE;
            }
        }

        if (cb > 0)
		{
            if (ReadProcessMemory(HPrcX.mOpenHand, lpOffset, lpb, cb, &dwRead)) 
			{
                //assert(dwRead == cb);
                *lpRead += dwRead;
            }
        }

        return TRUE;
    }

	return FALSE;
}