#include "stdafx.h"
#include <stdlib.h>
#include "wmcommand.h"
#include "global.h"
#include "res_str.h"
#include "resource.h"
#include "debugger.h"
#include "util.h"
#include <strsafe.h>

int  BoxCount = 0;
WORD curHelpId = 0;
static BOOL FAddToSearchPath = FALSE;
static BOOL FAddToRootMap = FALSE;

#define MAX_MSG_TXT 1024
BOOL
StartFileDlg(
    HWND hwnd,
    int titleId,
    int defExtId,
    int helpId,
    int templateId,
    LPTSTR fileName,
    DWORD* pFlags,
    LPOFNHOOKPROC lpfnHook
    )
{
#define filtersMaxSize 350

    OPENFILENAME        OpenFileName;
	TCHAR                title[MAX_MSG_TXT] = {0};
	TCHAR                defExt[MAX_MSG_TXT] = {0};
    BOOL                result;
	TCHAR                filters[filtersMaxSize] = {0};
    LPOFNHOOKPROC       lpDlgHook = NULL;
    HCURSOR             hSaveCursor;
	TCHAR                files[_MAX_PATH + 8] = {0};
	TCHAR                szExt[_MAX_EXT + 8] = {0};
	TCHAR                szBase[_MAX_PATH + 8] = {0};
    int                 indx;
    TCHAR                ch;
	TCHAR                fname[_MAX_FNAME] = {0};
	TCHAR                ext[_MAX_EXT] = {0};
    static              TCHAR *  szInitialDir = NULL;

    *pFlags |= (OFN_EXPLORER | OFN_NOCHANGEDIR);

    if(szInitialDir == NULL)
    {
        DWORD retval = GetCurrentDirectory(NULL, NULL);
        szInitialDir = (TCHAR *)malloc(retval*sizeof(TCHAR));
        GetCurrentDirectory(retval, szInitialDir);
    }

    if (DLG_Browse_Filebox_Title == titleId) {
        _wsplitpath( fileName, NULL, NULL, fname, ext );
        _wmakepath( files, NULL, NULL, fname, ext );
    } else {
        wcscpy(files, fileName);
    }

    hSaveCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

    InitFilterString((WORD)titleId, (LPTSTR)filters, (int)filtersMaxSize);
    LoadString(g_hInstance, titleId, (LPTSTR)title, MAX_MSG_TXT);
    LoadString(g_hInstance, defExtId, (LPTSTR)defExt, MAX_MSG_TXT);
    if (templateId) 
	{

        *pFlags |= OFN_ENABLETEMPLATE;
        OpenFileName.lpTemplateName = MAKEINTRESOURCE(templateId);
    } else 
	{
        *pFlags |= OFN_EXPLORER;
    }

    if (lpfnHook) 
	{

        lpDlgHook = lpfnHook;

        *pFlags |= OFN_ENABLEHOOK;
    }

    curHelpId = (WORD) helpId;
    OpenFileName.lStructSize = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner = hwnd;
    OpenFileName.hInstance = g_hInstance;
    OpenFileName.lpstrFilter = (LPTSTR)filters;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter = 0;
    OpenFileName.nFilterIndex = 1;
    OpenFileName.lpstrFile = files;
    OpenFileName.nMaxFile = _MAX_PATH;
    OpenFileName.lpstrFileTitle = NULL;
    OpenFileName.lpstrInitialDir = szInitialDir;
    OpenFileName.lpstrTitle = (LPTSTR)title;
    OpenFileName.Flags = *pFlags;
    OpenFileName.lpstrDefExt = (LPTSTR)NULL;
    OpenFileName.lCustData = 0L;
    OpenFileName.lpfnHook = lpDlgHook;

    DlgEnsureTitleBar();
    BoxCount++;

    switch (titleId) {

// BUGBUG - dead code - kcarlos
#if 0
    case DLG_SaveAs_Filebox_Title:
        //
        // Here we have the second half of a *kludge* (repeat, KLUDGE!!!) that
        // began back up in the SaveAsFile() routine.  If the 'fileName' parameter
        // is a null string, then we know (magic!) that we are Save As'ing a new file
        // that has no name as yet.  But the user's window shows a title of "UNTITLED n",
        // and that has been stuck in 'fileName' after the initial null.  So we find
        // it and then add that to the dialog box's title, eh?
        //
        if (!*fileName)
        {
            sprintf (title+strlen(title), "   %s", fileName+1);
        }

        result = GetSaveFileName((LPOPENFILENAME)&OpenFileName) ;
        break ;
#endif

    case DLG_Open_Filebox_Title:
        wcscat(OpenFileName.lpstrFile, defExt);
        // fall thru
    case DLG_Browse_Executable_Title:
    case DLG_Browse_CrashDump_Title:
        result = GetOpenFileName((LPOPENFILENAME)&OpenFileName) ;
        break ;

    case DLG_Browse_LogFile_Title:
        if (fileName) {
            wcscpy(OpenFileName.lpstrFile, fileName);
        } else {
            *OpenFileName.lpstrFile = 0;
        }
        result = GetOpenFileName((LPOPENFILENAME)&OpenFileName) ;
        break;

    case DLG_Browse_DbugDll_Title:
    case DLG_Merge_Filebox_Title:
        *(OpenFileName.lpstrFile) = '\0';
        result = GetOpenFileName((LPOPENFILENAME)&OpenFileName) ;
        break ;

    case DLG_Browse_Filebox_Title:
        _wsplitpath (files, NULL, NULL, szBase, szExt);
        indx = matchExt (szExt, filters);

        if (indx != -1) {
            OpenFileName.nFilterIndex = indx;
        }

        wcscat(title, szBase);
        if (*szExt) {
            wcscat(title, szExt);
        }

        FAddToSearchPath = FALSE;
        FAddToRootMap = FALSE;

        result = GetOpenFileName((LPOPENFILENAME)&OpenFileName) ;

        if (FAddToSearchPath) {
            AddToSearchPath(OpenFileName.lpstrFile);
        } else if (FAddToRootMap) {
            RootSetMapped(fileName, OpenFileName.lpstrFile);
        }
        break ;

    default:
        return FALSE;
        break;
    }


    BoxCount--;

    if (result)
    {
        wcscpy(fileName, OpenFileName.lpstrFile);

        if (result) {
            ch = fileName[OpenFileName.nFileOffset-1];
            fileName[OpenFileName.nFileOffset-1] = 0;
            if ((szInitialDir == NULL) || (wcscmp(szInitialDir, fileName) != 0)) {
                if (szInitialDir != NULL) {
                    free(szInitialDir);
                }
                szInitialDir = _wcsdup(fileName);
            }
            fileName[OpenFileName.nFileOffset-1] = ch;
        }

       *pFlags = OpenFileName.Flags ;

    }

    SetCursor(hSaveCursor);

    return result;
}

void
	AddToSearchPath(
	LPTSTR lpstrFile
	)
{
// 	PCTSTR  psz = g_contPaths_WkSp.m_pszSourceCodeSearchPath;
// 	int     i;
// 	char *  pch;
// 	char    ch;
// 
// 	pch = lpstrFile + strlen(lpstrFile);
// 	while (*pch != '\\') {
// 		pch--;
// 		DAssert(pch >= lpstrFile);
// 	}
// 
// 	i = (int) (pch - lpstrFile);
// 	pch = (PTSTR) calloc( i + 2 + (psz?strlen(psz):0), sizeof(TCHAR) );
// 	ch = lpstrFile[i];
// 	lpstrFile[i] = 0;
// 	strcpy(pch, lpstrFile);
// 	lpstrFile[i] = ch;
// 	if (psz) {
// 		*(pch + i) = ';';
// 		*(pch + i + 1) = 0;
// 		strcat(pch, psz );
// 	}
// 
// 	FREE_STR(g_contPaths_WkSp.m_pszSourceCodeSearchPath);
// 	g_contPaths_WkSp.m_pszSourceCodeSearchPath = pch;
}

BOOL
	RootSetMapped(
	LPTSTR lpszSrcRoot,
	LPTSTR lpszTargetRoot
	)
{
// 	UINT     i;
// 	LPSTR    p, q;
// 	CHAR     chpSaved, chqSaved;
// 
// 	if (lpszSrcRoot == NULL || lpszTargetRoot == NULL ||
// 		*lpszSrcRoot == '\0' || *lpszTargetRoot == '\0') {
// 			return FALSE;  // strlen is zero
// 	}
// 
// 	if (!((lpszSrcRoot[1] == ':' ||
// 		(lpszSrcRoot[0] == '\\' && lpszSrcRoot[1] == '\\')) &&
// 		(lpszTargetRoot[1] == ':' ||
// 		(lpszTargetRoot[0] == '\\' && lpszTargetRoot[1] == '\\'))
// 		)
// 		) {
// 			return FALSE;  // ignore non fullpath for now
// 	}
// 
// 	//  Make both strings lower case so we don't have some problems
// 	_strlwr(lpszSrcRoot);
// 	_strlwr(lpszTargetRoot);
// 
// 	p = lpszSrcRoot + strlen(lpszSrcRoot) - 1;
// 	q = lpszTargetRoot + strlen(lpszTargetRoot) - 1;
// 
// 	while (p >= lpszSrcRoot && q >= lpszTargetRoot) {
// 		if (*(p--) != *(q--)) {
// 			// Move to last matched character
// 			p += 1;
// 			q += 1;
// 
// 			//  If last matched char is a ':' then skip 1 character
// 			//  If last matched is ":\" then skip 2 characters
// 			if (*p == ':') {
// 				p++;
// 				q++;
// 				if (*p == '\\') {
// 					p++;
// 					q++;
// 				}
// 			}
// 			//  If last matched char is a "\" then skip 1 character
// 			else if (*p == '\\') {
// 				p++;
// 				q++;
// 			}
// 			//  If last matched is not a "\" then
// 			//  look for a slash and skip it
// 			//
// 			else {
// 				while (*p != '\\') {
// 					p++;
// 					q++;
// 				}
// 				p++;
// 				q++;
// 			}
// 
// 			chpSaved = *p;
// 			chqSaved = *q;
// 			*p = *q = '\0';
// 			for (i=0; i<CMappedRoots; i++) {
// 				if (_stricmp(RgMappedRoots[i].lpszSrcRoot, lpszSrcRoot) == 0 &&
// 					_stricmp(RgMappedRoots[i].lpszTargetRoot, lpszTargetRoot) == 0)
// 				{
// 					*p = chpSaved;
// 					*q = chqSaved;
// 					return TRUE; // already there
// 				}
// 			}
// 			if (CMappedRoots >= MAX_MAPPED_ROOTS) {
// 				memcpy(RgMappedRoots,
// 					&(RgMappedRoots[1]),
// 					sizeof(struct MRootPair)*--CMappedRoots);
// 			}
// 
// 			if ((RgMappedRoots[CMappedRoots].lpszSrcRoot = _strdup(lpszSrcRoot)) == NULL) {
// 				*p = chpSaved;
// 				*q = chqSaved;
// 				return FALSE;
// 			}
// 			if ((RgMappedRoots[CMappedRoots].lpszTargetRoot = _strdup(lpszTargetRoot)) == NULL) {
// 				free(RgMappedRoots[CMappedRoots].lpszSrcRoot);
// 				*p = chpSaved;
// 				*q = chqSaved;
// 				return FALSE;  // error - out of memory? - skip the mapping
// 			}
// 
// 			RgMappedRoots[CMappedRoots++].dwSrcLen = strlen(lpszSrcRoot);
// 			*p = chpSaved;
// 			*q = chqSaved;
// 			return (TRUE);
// 		}
// 
// 	}
	return FALSE;
}  /* RootSetMapped() */

UINT_PTR
APIENTRY
OpenExeWithArgsHookProc(
    HWND    hDlg,
    UINT    msg,
    WPARAM  wParam,
    LPARAM  lParam
    )
{

    extern TCHAR szOpenExeArgs[_MAX_PATH];

    switch( msg ) {
    case  WM_INITDIALOG:
        return TRUE;

    case WM_NOTIFY:
        {
            LPOFNOTIFY lpon = (LPOFNOTIFY) lParam;

            switch(lpon->hdr.code) {
            case CDN_FILEOK:
                GetDlgItemText(hDlg, IDC_EDIT_ARGS, szOpenExeArgs, sizeof(szOpenExeArgs));
                return 0;
            }
        }
    }

    return DlgFile(hDlg, msg, wParam, lParam);
}                              

UINT_PTR
	APIENTRY
	DlgFile(
	HWND hDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)

{
	switch (uMsg) {

	case WM_NOTIFY:
		{
			LPOFNOTIFY lpon = (LPOFNOTIFY) lParam;

			switch (lpon->hdr.code) {
			case CDN_HELP:
				// Help button pushed
				//Dbg(WinHelp(hDlg,szHelpFileName, HELP_CONTEXT, curHelpId));
				break;
			}
		}
		break;
	}
	return FALSE;
}                                       /* DlgFile() */


void

DlgEnsureTitleBar(
	void)
{
	if (BoxCount == 0) {
		//UpdateTitleBar(TBM_UNKNOWN, TRUE);
	}
}

#define TMP_STRING_SIZE     512

void
	AppendFilter(
	WORD filterTextId,
	int filterExtId,
	LPTSTR filterString,
	int *len,
	int maxLen
	)
{
	int size;

	//Append filter text
	TCHAR szTmp[TMP_STRING_SIZE] = {0};
	LoadString(g_hInstance, filterTextId, (LPTSTR)szTmp, MAX_MSG_TXT);
	size = wcslen(szTmp) + 1;
	
	memmove(filterString + (*len)*sizeof(TCHAR), szTmp, size*sizeof(TCHAR));
	*len += size;
	LoadString(g_hInstance, filterExtId, (LPTSTR)szTmp, MAX_MSG_TXT);
	size = wcslen(szTmp) + 1;
	//Assert(*len + size < maxLen);
	memmove(filterString + (*len)*sizeof(TCHAR), szTmp, size*sizeof(TCHAR));
	*len += size;
}                                       /* AppendFilter() */

void
	InitFilterString(
	WORD titleId,
	LPTSTR filter,
	int maxLen
	)
{
	int len = 0;

	switch (titleId) {
	case DLG_Browse_CrashDump_Title:
		AppendFilter(TYP_File_DUMP, DEF_Ext_DUMP, filter, &len, maxLen);
		break;

	case DLG_Browse_Executable_Title:
		AppendFilter(TYP_File_EXE, DEF_Ext_EXE, filter, &len, maxLen);
		break;

	case DLG_Browse_LogFile_Title:
		AppendFilter(TYP_File_LOG, DEF_Ext_LOG, filter, &len, maxLen);
		break;

	case DLG_SaveAs_Filebox_Title:
	case DLG_Open_Filebox_Title:
	case DLG_Merge_Filebox_Title:
	case DLG_Browse_Filebox_Title:
		AppendFilter(TYP_File_SOURCE, DEF_Ext_SOURCE, filter, &len, maxLen);
		AppendFilter(TYP_File_INCLUDE, DEF_Ext_INCLUDE, filter, &len, maxLen);
		AppendFilter(TYP_File_ASMSRC, DEF_Ext_ASMSRC, filter, &len, maxLen);
		AppendFilter(TYP_File_INC, DEF_Ext_INC, filter, &len, maxLen);
		AppendFilter(TYP_File_RC, DEF_Ext_RC, filter, &len, maxLen);
		AppendFilter(TYP_File_DLG, DEF_Ext_DLG, filter, &len, maxLen);
		AppendFilter(TYP_File_DEF, DEF_Ext_DEF, filter, &len, maxLen);
		AppendFilter(TYP_File_MAK, DEF_Ext_MAK, filter, &len, maxLen);
		break ;

	case DLG_Browse_DbugDll_Title:
		AppendFilter(TYP_File_DLL, DEF_Ext_DLL, filter, &len, maxLen);
		break;

	default:
		//DAssert(FALSE);
		break;
	}

	AppendFilter(TYP_File_ALL, DEF_Ext_ALL, filter, &len, maxLen);
	filter[len] = '\0';
}                                       /* InitFilterString() */

int matchExt (TCHAR* queryExtension, TCHAR* sourceList)
{
	int   answer;
	int   idxPair    = 1;        // a 1-based index!
	TCHAR* tokenMatch = 0;

	TCHAR  delims[]   = _T("*,; ") ;  // Given a typical string: "*.c;*.cpp;*.cxx",
	// strtok() would produce three tokens:
	// ".c", ".cpp", and ".cxx".

	while (*sourceList != 0  &&  tokenMatch == 0)
	{
		while (*sourceList != '\0')
		{ sourceList++; }          // skip first string of pair
		sourceList++;                 // and increment beyond NULL

		if (*sourceList != '\0')
		{
			TCHAR* work = _wcsdup (sourceList);  // copy to poke holes in

			tokenMatch = wcstok (work, delims);

			while (tokenMatch  &&  _wcsicmp (tokenMatch, queryExtension))
			{
				tokenMatch = wcstok (0, delims);
			}

			free (work);
		}

		if (tokenMatch == 0)             // no match:  need to move to next pair
		{
			while (*sourceList != '\0')
			{ sourceList++; }          // skip second string of pair
			sourceList++;                 // and increment beyond NULL

			idxPair++;
		}
	}

	answer = (tokenMatch != 0) ? idxPair : (-1);

	return (answer);
}

int	LogStartWithArgs(
	LPTSTR   lpsz,
	LPTSTR   lpszArgs
	)
{
	lpsz = CPSkipWhitespace(lpsz);
	if (*lpsz == 0) {
		return LOGERROR_UNKNOWN;
	}

	OpenProcessDebug* OpenDebug = (OpenProcessDebug*)malloc(sizeof(OpenProcessDebug));
	if (OpenDebug)
	{
		memset(OpenDebug,0,sizeof(OpenDebug));
		StringCchCopy( OpenDebug->ProcessPath,MAX_PATH,lpsz);
		StringCchCopy( OpenDebug->ProcessParam,4*MAX_PATH,lpszArgs);
		// 	if (!RestartDebuggee(lpsz, lpszArgs)) {
		// 		CmdLogVar(ERR_Start_Failed);
		// 		return LOGERROR_QUIET;
		// 	}
		int nResult = g_ClientMid.StartDebugProcess(*OpenDebug);

		free(OpenDebug);
		OpenDebug = NULL;
	}
	
	return LOGERROR_NOERROR;
}