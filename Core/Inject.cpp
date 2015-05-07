#include "stdafx.h"
#include <detours.h>
#include <windows.h>

BOOL WINAPI CreateProcessWithDll (LPSTR fileName, LPSTR argument, LPSTR dllName)  
{      
	STARTUPINFO si;     
	PROCESS_INFORMATION pi;  
	ZeroMemory(&si, sizeof(STARTUPINFO));     
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));     
	si.cb = sizeof(STARTUPINFO);  
	BOOL state = DetourCreateProcessWithDll(fileName,argument,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&si, &pi,dllName,NULL);	
	return state;	
}  


typedef DWORD (WINAPI *PFNTCREATETHREADEX)  
	(   
	PHANDLE                 ThreadHandle,     
	ACCESS_MASK             DesiredAccess,    
	LPVOID                  ObjectAttributes,     
	HANDLE                  ProcessHandle,    
	LPTHREAD_START_ROUTINE  lpStartAddress,   
	LPVOID                  lpParameter,      
	BOOL                    CreateSuspended,      
	DWORD                   dwStackSize,      
	DWORD                   dw1,   
	DWORD                   dw2,   
	LPVOID                  Unknown   
	);   


BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)   
{  
	TOKEN_PRIVILEGES tp;  
	HANDLE hToken;  
	LUID luid;  
	if( !OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken) )  
	{  		  
		return FALSE;  
	}  

	if( !LookupPrivilegeValue(NULL, lpszPrivilege,&luid) )  
	{  		  
		return FALSE;   
	}  

	tp.PrivilegeCount = 1;  
	tp.Privileges[0].Luid = luid;  
	if( bEnablePrivilege )  
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
	else  
		tp.Privileges[0].Attributes = 0;  
	if( !AdjustTokenPrivileges(hToken, FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL,(PDWORD) NULL) )  
	{   		  
		return FALSE;   
	}   
	if( GetLastError() == ERROR_NOT_ALL_ASSIGNED )  
	{  		 
		return FALSE;  
	}   
	return TRUE;  
} 

BOOL IsVistaOrLater()  
{  
	OSVERSIONINFO osvi;  
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));  
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
	GetVersionEx(&osvi);  
	if( osvi.dwMajorVersion >= 6 )  
		return TRUE;  
	return FALSE;  
}  

BOOL CreateRemoteThreadAdapter(HANDLE hProcess, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pRemoteBuf)  
{  
	HANDLE      hThread = NULL;  
	FARPROC     pFunc = NULL;  

	if(IsVistaOrLater() == TRUE )    
	{  
		pFunc =(FARPROC)DetourFindFunction("ntdll.dll","NtCreateThreadEx");
		if( pFunc == NULL )  
		{  
			return FALSE;  
		}  

		((PFNTCREATETHREADEX)pFunc)(&hThread,0x1FFFFF,NULL,hProcess,pThreadProc, pRemoteBuf,FALSE, NULL, NULL, NULL,NULL);  
		if( hThread == NULL )  
		{  			 
			return FALSE;  
		}  
	}  
	else                   
	{  
		hThread = CreateRemoteThread(hProcess,NULL,0,pThreadProc,pRemoteBuf,0,NULL); 
		if( hThread == NULL )  
		{  			 
			return FALSE;  
		}  
	}  

	if( WAIT_FAILED == WaitForSingleObject(hThread, INFINITE) )  
	{  		
		return FALSE;  
	}  
	return TRUE;  
}  

BOOL CreateRemoteThreadAdapterWithDll(DWORD dwPID, char *szDllName)  
{  
	HANDLE hProcess = NULL;  
	LPVOID pRemoteBuf = NULL;  
	FARPROC pThreadProc = NULL;  
	DWORD dwBufSize = strlen(szDllName) + 1;  

	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )  
	{  		
		return FALSE;  
	}  

	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize,MEM_COMMIT, PAGE_READWRITE);  
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName,dwBufSize, NULL);  
	pThreadProc =(FARPROC)DetourFindFunction("kernel32.dll","LoadLibraryA"); 

	if(!CreateRemoteThreadAdapter(hProcess, (LPTHREAD_START_ROUTINE)pThreadProc, pRemoteBuf) )  
	{  		
		return FALSE;  
	}  
	VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);  
	CloseHandle(hProcess);  
	return TRUE;  
}  

BOOL WINAPI CreateRemoteThreadWithDll(DWORD dwPID, char *szDllName)  
{  
	SetPrivilege(SE_DEBUG_NAME, TRUE);    
	return CreateRemoteThreadAdapterWithDll(dwPID,szDllName);    
}  