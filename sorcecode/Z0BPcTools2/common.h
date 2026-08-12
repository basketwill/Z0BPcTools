#ifndef _COMMON_H
#define _COMMON_H

#ifdef WIN32
#define TARGET_i386
#else 
#define TARGET_IA64
#endif

typedef enum _CUSTOM_PEN
{
	kCustomBlackPen		= 0,
	kCustomRedPen		= 1,
	kCustomGreenPen		= 2,
	kCustomBluePen		= 3,
	kCustomYellowPen	= 4,
	kCustomWhitePen		= 5,
	kCustomGreyPen		= 6,
	kCustomFreGreyPen	= 7,
	kCustomCyanPen		= 8,
	kCustomGoldenPen	= 9,

	kCustomBlackColor	= RGB(0,0,0),
	kCustomRedColor		= RGB(255,0,0),
	kCustomGreenColor	= RGB(0,255,0),
	kCustomBlueColor	= RGB(0,0,255),
	kCustomYellowColor	= RGB(0,255,255),
	kCustomWhiteColor	= RGB(255,255,255),

	kCustomGreyColor = RGB(128,128,128),
	kCustomFreGreyColor = RGB(192,192,192),
	kCustomCyanColor = RGB(0,0,128),
	kCustomGoldenColor = RGB(128,128,0),

	kCustomBkg			= RGB(255,255,245),

}CUSTOM_PEN;

/************************************************************************/
/* DBGÏà¹Ø
*/
/************************************************************************/

typedef ULONG64 PID;
typedef ULONG64 TID;
typedef ULONGLONG UOFF64;
typedef ULONG   SEGMENT;    // 32-bit compiler doesn't like "_segment"
typedef USHORT  SEG16;
typedef ULONG   SEG32;
typedef SEGMENT FAR * LPSEGMENT;
typedef ULONG   UOFF32;
typedef UOFF32 FAR * LPUOFF32;
typedef USHORT  UOFF16;
typedef UOFF16 FAR * LPUOFF16;
typedef LONG    OFF32;
typedef OFF32 FAR * LPOFF32;
typedef SHORT   OFF16;
typedef OFF16 FAR * LPOFF16;
typedef ULONGLONG UOFF64;
typedef LONGLONG OFF64;


typedef OFF64   SOFFSET;
typedef UOFF64  UOFFSET;
typedef UOFF64  OFFSET;
typedef SOFFSET FAR * LPSOFFSET;
typedef UOFFSET FAR * LPUOFFSET;
typedef OFFSET FAR * LPOFFSET;

typedef struct {
	UOFF64      off;
	SEGMENT     seg;
} address_t;

typedef struct {
	BYTE    fFlat   :1;         // true if address is flat
	BYTE    fOff32  :1;         // true if offset is 32 bits
	BYTE    fIsLI   :1;         // true if segment is linker index
	BYTE    fReal   :1;         // x86: is segment a real mode address
	BYTE    fSql    :1;         // true if execution context is SQL SP
	BYTE    fJava   :1;                     // true if Java addr
	BYTE    unused  :2;         // unused
} memmode_t;

#ifdef STRICT
DECLARE_HANDLE(HEMI);
#else
typedef HIND    HEMI;           // Executable Module Index
#endif


typedef struct ADDR {
	address_t   Addr;
	memmode_t   Mode;
	union
	{
		DWORDLONG Alignment;
		HEMI        Emi;
	};
} ADDR;                     // An address specifier
typedef ADDR *  PADDR;
typedef ADDR *  LPADDR;

#define MODE_IS_FLAT(m)     ((m).fFlat)
#define MODE_IS_OFF32(m)    ((m).fOff32)
#define MODE_IS_LI(m)       ((m).fIsLI)
#define MODE_IS_REAL(m)     ((m).fReal)

#define addrAddr(a)         ((a).Addr)
#define emiAddr(a)          ((a).Emi)
#define modeAddr(a)         ((a).Mode)

#define ADDR_IS_FLAT(a)     (MODE_IS_FLAT(modeAddr(a)))
#define ADDR_IS_OFF32(a)    (MODE_IS_OFF32(modeAddr(a)))
#define ADDR_IS_LI(a)       (MODE_IS_LI(modeAddr(a)))
#define ADDR_IS_REAL(a)     (MODE_IS_REAL(modeAddr(a)))

#define ADDRSEG16(a)        {ADDR_IS_FLAT(a) = FALSE; ADDR_IS_OFF32(a) = FALSE;}
#define ADDRSEG32(a)        {ADDR_IS_FLAT(a) = FALSE; ADDR_IS_OFF32(a) = TRUE;}
#define ADDRLIN32(a)        {ADDR_IS_FLAT(a) = TRUE;  ADDR_IS_OFF32(a) = TRUE;}

#define GetAddrSeg(a)       ((a).Addr.seg)
#define GetAddrOff(a)       ((a).Addr.off)
#define SetAddrSeg(a,s)     ((a)->Addr.seg=s)

#if defined(TARGET_IA64)
//
// these are used in the flags field of a BREAKPOINT (IA64-specific)
//
typedef enum {
	BREAKPOINT_IA64_MASK = 0x000f0000,
	BREAKPOINT_IA64_MODE = 0x00010000,   // IA64 EM mode
	BREAKPOINT_IA64_MOVL = 0x00020000,   // displaced instruction is MOVL
} BPFLG;
#endif



typedef enum {
	ps_root       = 0x0001,     /* This is the root process, do not send a */
	/* dbcDeleteProc when this is continued */
	/* after a dbcProcTerm. */
	ps_preStart   = 0x0002,     /* Process is expecting loader BP */
	ps_preEntry   = 0x0004,     /* Process is expecting Entry BP */
	ps_dead       = 0x0010,     /* This process is dead. */
	ps_deadThread = 0x0020,     /* This process owns dead threads */
	ps_exited     = 0x0040,     /* We have notified the debugger that this */
	/* process has exited. */
	ps_destroyed  = 0x0080,     /* This process has been destroyed (deleted) */
	ps_killed     = 0x0100,     /* This process is being killed */
	ps_connect    = 0x0200
} DMPSTATE;


typedef enum _EXCEPTION_FILTER_DEFAULT {
	efdIgnore,
	efdNotify,
	efdCommand,
	efdStop
} EXCEPTION_FILTER_DEFAULT;
typedef EXCEPTION_FILTER_DEFAULT * LPEXCEPTION_FILTER_DEFAULT;

typedef enum _EXCEPTION_CONTROL {
	exfFirst,
	exfNext,
	exfSpecified,
	exfDefault
} EXCEPTION_CONTROL;
typedef EXCEPTION_CONTROL * LPEXCEPTION_CONTROL;

#define EXCEPTION_STRING_SIZE 60

typedef enum _ORPC_DEBUGGING_STATE
{
	ORPC_NOT_DEBUGGING      = 0,
	ORPC_START_DEBUGGING,
	ORPC_DEBUGGING,
	ORPC_STOP_DEBUGGING
} ORPC_DEBUGGING_STATE;

// OLE Rpc debugging
typedef enum _ORPCKEYSTATE {

	orpcKeyNotChecked,      // the DebugObjectRPCEnabled key has not been looked at.
	//
	orpcKeyEnabled,         // the DebugObjectRPCEnabled key is enabled.
	//
	orpcKeyFailed,          // We tried to modify the key but failed, so don't try again.
	//
}   ORPCKEYSTATE;

typedef HANDLE HDEP;
typedef HANDLE HIND;
typedef HDEP    HLLI;       // A handle to a linked list
typedef HDEP    HLLE;       // A handle to a linked list entry

typedef enum {
	//ts_preStart =0x1000,        /* Before the starting point of the thread */
	/* from this state a registers and trace   */
	/* are dealt with specially                */
	ts_running  =     1,        /* Execution is proceeding on the thead    */
	ts_stopped  =     2,        /* An event has stopped execution          */
	ts_frozen   = 0x010,        /* Debugger froze thread.                  */
	ts_dead     = 0x020,        /* Thread is dead.                         */
	ts_destroyed =0x040,        /* Thread is destroyed (deleted)           */
	ts_first    = 0x100,        /* Thread is at first chance exception     */
	ts_second   = 0x200,        /* Thread is at second chance exception    */
	ts_rip      = 0x400,        /* Thread is in RIP state                  */
	ts_stepping = 0x800,        /*                                         */
	ts_funceval = 0x40000000    /* Thread is being used for function call  */
} TSTATEX;

enum {
	bptpExec,
	bptpDataC,
	bptpDataW,
	bptpDataR,
	bptpDataExec,
	bptpRegC,
	bptpRegW,
	bptpRegR,
	bptpMessage,
	bptpMClass,
	bptpInt,
	bptpRange
};
typedef DWORD BPTP;

enum {
	bpnsStop,
	bpnsContinue,
	bpnsCheck,
	bpnsMax             // donc for mac dm
};
typedef DWORD BPNS;


#if defined(TARGET_i386)

typedef BYTE BP_UNIT;
#define BP_OPCODE   0xCC
#define DELAYED_BRANCH_SLOT_SIZE    0

#define MAX_INSTRUCTION_SIZE    20

#define HAS_DEBUG_REGS
// #undef NO_TRACE_FLAG

#define NUMBER_OF_DEBUG_REGISTERS   4
#define DEBUG_REG_DATA_SIZES        { 1, 2, 4 }
#define MAX_DEBUG_REG_DATA_SIZE     4
#define DEBUG_REG_LENGTH_MASKS      {   \
	0xffffffff,          \
	0,          \
	1,          \
	0Xffffffff, \
	3           \
}


#define TF_BIT_MASK 0x00000100  /* This is the right bit map for */
/* the 286, make sure its correct */
/* for the 386. */

#elif defined (TARGET_ALPHA) || defined (TARGET_AXP64)

typedef DWORD BP_UNIT;
#define BP_OPCODE   0x80L
#define DELAYED_BRANCH_SLOT_SIZE    0

#define MAX_INSTRUCTION_SIZE    4

// #undef HAS_DEBUG_REGS
#define NO_TRACE_FLAG

#elif defined(TARGET_IA64)

typedef ULONGLONG BP_UNIT;
#define KDP_BREAKPOINT_ALIGN 0x3
#define BP_OPCODE  (BREAK_INSTR | (ULONGLONG)(DEBUG_STOP_BREAKPOINT << 6))
#define KERNEL_BREAKIN_OPCODE  (BREAK_INSTR | (ULONGLONG)(BREAKIN_BREAKPOINT << 6))
#define DELAYED_BRANCH_SLOT_SIZE    0
#define MAX_INSTRUCTION_SIZE    4  //v-vadimp ?

// HAS_DEBUG_REGS must be defined to enable ExtendedContext support
#define HAS_DEBUG_REGS
//#define NO_TRACE_FLAG  //v-vadimp - there's some code for setting the SS flag (PSR.ss), rem'-ing this out should enable it

// IA64 has 8 DBR's and 8 IBR's that can emulate up to 4 full feature BR's
#define NUMBER_OF_DEBUG_REGISTERS   4
#define DEBUG_REG_DATA_SIZES        { 1, 2, 4, 8 }
#define MAX_DEBUG_REG_DATA_SIZE     8
#define DEBUG_REG_LENGTH_MASKS      {       \
	(DWORDLONG)0,                \
	(DWORDLONG)0xff,             \
	(DWORDLONG)0xffff,           \
	(DWORDLONG)0,                \
	(DWORDLONG)0xffffffff,       \
	(DWORDLONG)0,                \
	(DWORDLONG)0,                \
	(DWORDLONG)0,                \
	(DWORDLONG)0xffffffffffffff, \
}

#else

#error "Unknown target CPU"

#endif

__inline
BOOL
Is64PtrSE(
    ULONG64 p64
    )
/*++
Description:
    Is the pointer sign extended? It is an error if the pointer 
    is in the range of 2GB and 4GB
--*/
{
    return ((p64 < 0x0000000080000000) || (0x100000000 <= p64));
}

#define PassingExceptionForThisThread(h) \
	(((h)->tstate & (ts_first | ts_second)) && !(h)->fExceptionHandled)

#define X86_VALID               0x1
#define X86_TRANSITION_MASK     0xC00
#define X86_TRANSITION_CHECK    0x800
#define X86_VALID_PFN_MASK      0xFFFFF000
#define X86_VALID_PFN_SHIFT     12
#define X86_TRANS_PFN_MASK      0xFFFFF000
#define X86_TRANS_PFN_SHIFT     12
#define X86_PDE_SHIFT           22
#define X86_PTE_SHIFT           12
#define X86_PTE_MASK            0x3ff
#define X86_PHYSICAL_MASK       0x0
#define X86_PHYSICAL_START      0x1
#define X86_PHYSICAL_END        0x0
#define X86_PAGESIZE            4096
#define X86_PAGESHIFT           12
#define X86_LARGE_PAGE_MASK     0x80
#define X86_LARGE_PAGE_SIZE     (4*1024*1024)

#define MIPS_VALID              0x2
#define MIPS_TRANSITION_MASK    0x104
#define MIPS_TRANSITION_CHECK   0x100
#define MIPS_VALID_PFN_MASK     0x3FFFFFC0
#define MIPS_VALID_PFN_SHIFT    6
#define MIPS_TRANS_PFN_MASK     0xFFFFFE00
#define MIPS_TRANS_PFN_SHIFT    9
#define MIPS_PDE_SHIFT          22
#define MIPS_PTE_SHIFT          12
#define MIPS_PTE_MASK           0x3ff
#define MIPS_PHYSICAL_MASK      0x1FFFFFFF
#define MIPS_PHYSICAL_START     0x80000000
#define MIPS_PHYSICAL_END       0xBFFFFFFF
#define MIPS_PAGESIZE           4096
#define MIPS_PAGESHIFT          12

#define PPC_VALID               0x4
#define PPC_TRANSITION_MASK     0x3
#define PPC_TRANSITION_CHECK    0x2
#define PPC_VALID_PFN_MASK      0xFFFFF000
#define PPC_VALID_PFN_SHIFT     12
#define PPC_TRANS_PFN_MASK      0xFFFFF000
#define PPC_TRANS_PFN_SHIFT     12
#define PPC_PDE_SHIFT           22
#define PPC_PTE_SHIFT           12
#define PPC_PTE_MASK            0x3ff
#define PPC_PHYSICAL_MASK       0x1FFFFFFF
#define PPC_PHYSICAL_START      0x80000000
#define PPC_PHYSICAL_END        0x807FFFFF
#define PPC_PAGESIZE            4096
#define PPC_PAGESHIFT           12

#define ALPHA_VALID             0x1
#define ALPHA_TRANSITION_MASK   0x6
#define ALPHA_TRANSITION_CHECK  0x4
#define ALPHA_VALID_PFN_MASK    0xFFFFFE00
#define ALPHA_VALID_PFN_SHIFT   9
#define ALPHA_TRANS_PFN_MASK    0xFFFFFE00
#define ALPHA_TRANS_PFN_SHIFT   9
#define ALPHA_PDE_SHIFT         24
#define ALPHA_PTE_SHIFT         13
#define ALPHA_PTE_MASK          0x7ff
#define ALPHA_PHYSICAL_MASK     0x3FFFFFFF
#define ALPHA_PHYSICAL_START    0x80000000
#define ALPHA_PHYSICAL_END      0xBFFFFFFF
#define ALPHA_PAGESIZE          8192
#define ALPHA_PAGESHIFT         13

#define PAGE_SIZE 0x2000

#endif