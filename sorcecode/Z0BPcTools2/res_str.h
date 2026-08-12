#ifndef _RES_STR_H
#define _RES_STR_H

#ifdef RESOURCES
#define RES_STR(a, b, c) b, c
STRINGTABLE
	BEGIN
#else

enum _RESOURCEIDS {
#define RES_STR(a, b, c) a = b,
#endif


RES_STR(DLG_Open_Filebox_Title,             3200,"Open Source File")
RES_STR(DLG_SaveAs_Filebox_Title,           3201,"Save As")
RES_STR(DLG_Merge_Filebox_Title,            3202,"Merge")
RES_STR(DLG_Browse_Filebox_Title,           3203,"Browse For File ")
RES_STR(DLG_Browse_DbugDll_Title,           3204,"Browse For DLL ")
RES_STR(DLG_Browse_For_Symbols_Title,       3205,"Open Symbol File For ")
RES_STR(DLG_Browse_LogFile_Title,           3206,"Browse For Log File")
RES_STR(DLG_Browse_Executable_Title,        3207,_T("打开可执行文件"))
RES_STR(DLG_Browse_CrashDump_Title,         3208,"Open Crash Dump")
RES_STR(TYP_File_SOURCE,                    11011,"C/C++ Src (*.c;*.cpp;*.cxx)")
RES_STR(TYP_File_INCLUDE,                   11012,"C/C++ Inc (*.h)")
RES_STR(TYP_File_ASMSRC,                    11013,"ASM   Src (*.asm;*.s)")
RES_STR(TYP_File_INC,                       11180,"User  Inc  (*.inc)")
RES_STR(TYP_File_RC,                        11100,"Resource  (*.rc)")
RES_STR(TYP_File_DLG,                       11110,"Dialog      (*.dlg)")
RES_STR(TYP_File_DEF,                       11090,"Definition (*.def)")
RES_STR(TYP_File_MAK,                       11060,"Project    (*.mak)")
RES_STR(TYP_File_All_SYMS,                  11261,"All Symbols (*.pdb;*.dbg;*.sym;*.exe;*.dll)")
RES_STR(TYP_File_Cust_SYMS,                 11262,"Symbols")
RES_STR(TYP_File_DLL,                       11220,"Dynamic Library (*.dll)")
RES_STR(TYP_File_ALL,                       11270,"All Files (*.*)")

// BUGBUG - Dead code
// RES_STR(TYP_File_C,                         11010,"C Source (*.c)")
// BUGBUG - Dead code
// RES_STR(TYP_File_CPP,                       11020,"C++ Source (*.cpp)")
// BUGBUG - Dead code
// RES_STR(TYP_File_CXX,                       11030,"C++ Source (*.cxx)")
// BUGBUG - Dead code
// RES_STR(TYP_File_H,                         11040,"C Header (*.h)")
// BUGBUG - Dead code
// RES_STR(TYP_File_NOEXT,                     11050,"No Extension (*.)")
// BUGBUG - Dead code
// RES_STR(TYP_File_OBJ,                       11080,"Object Code (*.obj)")
RES_STR(TYP_File_EXE,                       11200,"Executable (*.exe)")
// BUGBUG - Dead code
// RES_STR(TYP_File_COM,                       11210,"Command (*.com)")
RES_STR(TYP_File_ASM,                       11250,"Asm Files (*.asm)")
// BUGBUG - Dead code
// RES_STR(TYP_File_MIPSASM,                   11260,"Mips Asm Files (*.s)")
RES_STR(TYP_File_EXE_COM,                   11280,"Executable (*.exe;*.com)")
RES_STR(TYP_File_LOG,                       11281,"Log Files (*.log)")
RES_STR(TYP_File_DUMP,                      11282,"Crash Dump Files (*.dmp)")

RES_STR(DEF_Ext_C,                          11300,"*.C")
RES_STR(DEF_Ext_SOURCE,                     11301,"*.C;*.CPP;*.CXX")
RES_STR(DEF_Ext_CPP,                        11310,"*.CPP")
RES_STR(DEF_Ext_CXX,                        11320,"*.CXX")
RES_STR(DEF_Ext_INCLUDE,                    11321,"*.H;*.HPP;*.HXX")
RES_STR(DEF_Ext_H,                          11330,"*.H")
// BUGBUG - Dead code
// RES_STR(DEF_Ext_NOEXT,                      11340,"*.")
RES_STR(DEF_Ext_MAK,                        11350,"*.MAK")
// BUGBUG - Dead code
// RES_STR(DEF_Ext_OBJ,                        11370,"*.OBJ")
RES_STR(DEF_Ext_DEF,                        11380,"*.DEF")
RES_STR(DEF_Ext_RC,                         11390,"*.RC")
RES_STR(DEF_Ext_DLG,                        11400,"*.DLG")
RES_STR(DEF_Ext_INC,                        11470,"*.INC")
RES_STR(DEF_Ext_EXE,                        11490,"*.exe;*.dll")
RES_STR(DEF_Ext_COM,                        11500,"*.COM")
RES_STR(DEF_Ext_DLL,                        11510,"*.DLL")
RES_STR(DEF_Ext_ASMSRC,                     11511,"*.ASM;*.S")
RES_STR(DEF_Ext_ASM,                        11540,"*.ASM")
RES_STR(DEF_Ext_PE,                         11541,"*.exe;*.dll;*.sys")
// BUGBUG - Dead code
// RES_STR(DEF_Ext_MIPSASM,                    11550,"*.S")
RES_STR(DEF_Ext_All_SYMS,                   11551,"*.pdb;*.dbg;*.sym;*.exe;*.dll")
RES_STR(DEF_Ext_Cust_SYMS,                  11552,".pdb\0.dbg\0.sym\0")
RES_STR(DEF_Ext_ALL,                        11560,"*.*")
RES_STR(DEF_Ext_EXE_COM,                    11570,"*.exe;*.com")
RES_STR(DEF_Ext_LOG,                        11571,"*.log")
RES_STR(DEF_Ext_DUMP,                       11572,"*.dmp")


// Toolbar strings
RES_STR(TBR_FILE_OPEN,                      12000,"Open source file (Ctrl+O)")
RES_STR(TBR_FILE_SAVE_WORKSPACE,            12001,"Save workspace (Ctrl+S)")

RES_STR(TBR_EDIT_CUT,                       12002,"Cut (Ctrl+X)")
RES_STR(TBR_EDIT_COPY,                      12003,"Copy (Ctrl+C)")
RES_STR(TBR_EDIT_PASTE,                     12004,"Paste (Ctrl+V)")

RES_STR(TBR_DEBUG_GO,                       12005,"Go (F5)")
RES_STR(TBR_DEBUG_RESTART,                  12006,"Restart (Ctrl+Shift+F5)")
RES_STR(TBR_DEBUG_STOPDEBUGGING,            12007,"Stop debugging (Shift+F5)")
RES_STR(TBR_DEBUG_BREAK,                    12008,"Break (Ctrl+Break)")

RES_STR(TBR_DEBUG_STEPINTO,                 12009,"Step into (F11 or F8)")
RES_STR(TBR_DEBUG_STEPOVER,                 12010,"Step over (F10)")
RES_STR(TBR_DEBUG_RUNTOCURSOR,              12011,"Run to cursor (Ctrl+F10 or F7)")

RES_STR(TBR_EDIT_BREAKPOINTS,               12012,"Insert or remove breakpoint (F9)")
RES_STR(TBR_DEBUG_QUICKWATCH,               12013,"Quick watch (Shift+F9)")

RES_STR(TBR_VIEW_COMMAND,                   12014,"Command (Alt+1)")
RES_STR(TBR_VIEW_WATCH,                     12015,"Watch (Alt+2)")
RES_STR(TBR_VIEW_LOCALS,                    12018,"Locals (Alt+3)")
RES_STR(TBR_VIEW_REGISTERS,                 12019,"Registers (Alt+4)")
RES_STR(TBR_VIEW_MEMORY,                    12017,"Memory window (Alt+5)")
RES_STR(TBR_VIEW_CALLSTACK,                 12016,"Call stack (Alt+6)")
RES_STR(TBR_VIEW_DISASM,                    12020,"Disassembly (Alt+7)")
RES_STR(TBR_VIEW_FLOAT,                     12021,"Float (Alt+8)")

RES_STR(TBR_DEBUG_SOURCE_MODE_ON,           12022,"Source mode on")
RES_STR(TBR_DEBUG_SOURCE_MODE_OFF,          12023,"Source mode off")
#ifdef RESOURCES
END
#else
IDS_REMOVE_WARNING
};
#endif
#endif