#include "stdafx.h"
#include <atlstr.h>

void WINAPI Void()
{
}

DWORD CALLBACK LoadAssembly(LPVOID hCore)
{
	typedef BOOL (*PLoadAssembly)();
	FARPROC farProc = GetProcAddress((HMODULE)hCore,"LoadAssembly");
	PLoadAssembly LoadAssembly = PLoadAssembly(farProc);

	if(LoadAssembly != NULL)  
	{   			
		LoadAssembly();		
	}
	return 0;
}


void LoadCoreModule(HMODULE hModule)
{	
	HMODULE hCore = GetModuleHandle(L"Core.dll");
	if (hCore == NULL)
	{		
		wchar_t modulePath[MAX_PATH];
		GetModuleFileName(hModule, modulePath, MAX_PATH);
		(_tcsrchr(modulePath,_T('\\')))[1] = 0;
		CString path = modulePath;		
		hCore = LoadLibrary(path + L"Core.dll"); 
	}	 

	if(hCore != NULL)  
	{	
		CreateThread(0, 0, LoadAssembly, hCore, 0, 0);
	}
}



BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:			
		LoadCoreModule(hModule);		
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

