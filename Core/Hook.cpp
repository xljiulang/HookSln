#include "stdafx.h"
#include <detours.h>
#include <windows.h>

#pragma comment(lib, "detours.lib") 


PVOID WINAPI FindFunction(LPSTR moduleName, LPSTR funcName)
{
	return DetourFindFunction(moduleName,funcName);
} 


BOOL WINAPI HookFunction(PVOID& targetFuncPointer , PVOID proxyFuncPointer) 
{	  	 
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());	
	LONG state = DetourAttach(&targetFuncPointer,proxyFuncPointer);
	DetourTransactionCommit();
	return state == 0;
}

BOOL WINAPI UnHookFunction(PVOID& targetFuncPointer , PVOID proxyFuncPointer) 
{	
	DetourTransactionBegin();	
	DetourUpdateThread(GetCurrentThread());
	LONG state = DetourDetach(&targetFuncPointer,proxyFuncPointer);
	DetourTransactionCommit();
	return state == 0;
}
