#include "stdafx.h"
#include "CNtdll.h"
#include <strsafe.h>

BOOL CNtDll::QuerySystemProces(CList<PROCESS_ATTACH_INFO>& ProcessInfo )
{
	BOOL bSuccess = FALSE;
	HMODULE hNtDll = LoadLibrary(L"ntdll.dll");

	if(!hNtDll)
		return FALSE;

	ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)
		GetProcAddress(
		hNtDll,
		"NtQuerySystemInformation");
	ULONG cbBuffer = 0x10000;
	LPVOID pBuffer = NULL;
	NTSTATUS Status;

	do
	{
		pBuffer = malloc(cbBuffer);
		if(pBuffer == NULL)
			return FALSE;

		memset(
			pBuffer,
			0,
			cbBuffer);

		Status = ZwQuerySystemInformation(
							SystemProcessesAndThreadsInformation,
							pBuffer,
							cbBuffer,
							NULL);

		if(Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			free(pBuffer);
			cbBuffer = cbBuffer * 2;
		}

	}while(Status == STATUS_INFO_LENGTH_MISMATCH);

	PSYSTEM_PROCESSES pInfo = (PSYSTEM_PROCESSES)pBuffer;

	while(TRUE)
	{

		if (pInfo->ProcessName.Buffer)
		{
			PROCESS_ATTACH_INFO* Process = new PROCESS_ATTACH_INFO();

			if (Process)
			{
				ZeroMemory(Process,sizeof(PROCESS_ATTACH_INFO));
				Process->ProcessId = pInfo->ProcessId;
				Process->InheritedFromProcessId = pInfo->InheritedFromProcessId;
				StringCchCopy(Process->ProcessName,2*MAX_PATH,(TCHAR*)pInfo->ProcessName.Buffer);
				ProcessInfo.AddHead(Process);
				bSuccess = TRUE;
			}			
		}
		
		if (pInfo->NextEntryDelta == 0)
		{
			break;
			//如果没有下一个就结束
		}

		pInfo = (PSYSTEM_PROCESSES)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);
	}
	ULONG OldPID = 0;
	
	free(pBuffer);
	FreeLibrary(hNtDll);

	return bSuccess;
}