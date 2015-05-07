#include "stdafx.h"
#include <atlstr.h>

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;


#pragma data_seg ("SHAREDDATA")
char ASSEMBLY_TO_LOAD[MAX_PATH] = {0};
#pragma data_seg ()

HANDLE ASSEMBLY_LOAD_EVENT = NULL;
LPCSTR ASSEMBLY_LOAD_EVENT_NAME = "ASSEMBLY_LOAD_EVENT";


MethodInfo^ TryGetAssemblyMain(String^ assemblyName)
{
	try
	{
		Assembly^ assembly = Assembly::LoadFrom(assemblyName);
		array<Type^>^ types= assembly->GetTypes();
		for each (Type^ type in types)
		{
			MethodInfo^ main = type->GetMethod("Main");
			if( main!= nullptr && main->IsStatic && main->Name == "Main")
			{
				return main;
			}
		}
		return nullptr;
	}
	catch (Exception^)
	{
		return nullptr;
	}
}

void WINAPI SetAssembly(LPCSTR assmelby)
{	
	if (ASSEMBLY_LOAD_EVENT == NULL )
	{
		ASSEMBLY_LOAD_EVENT = CreateEvent(NULL,TRUE,FALSE,ASSEMBLY_LOAD_EVENT_NAME);		 
	} 
	else
	{
		ResetEvent(ASSEMBLY_LOAD_EVENT);
	}	
	strcpy_s(ASSEMBLY_TO_LOAD, MAX_PATH, assmelby);
}

void WINAPI WaitForLoadAssembly(DWORD dwTimeOut)
{
	if (ASSEMBLY_LOAD_EVENT == NULL )
	{
		return;
	}	
	WaitForSingleObject(ASSEMBLY_LOAD_EVENT,dwTimeOut);
}


String^ GetAssembly()
{
	String^ assembly = gcnew String(ASSEMBLY_TO_LOAD);
	if (String::IsNullOrEmpty(assembly) == false)
	{
		return assembly;
	}

	String^ ini = Path::GetFullPath("Inject.ini");
	if (File::Exists(ini ))
	{
		char* pIni =(char*)Marshal::StringToHGlobalAnsi(ini).ToPointer(); 
		if (GetPrivateProfileString("Assembly","Path","", ASSEMBLY_TO_LOAD,MAX_PATH, pIni) >0)
		{
			assembly = Path::GetFullPath(gcnew String(ASSEMBLY_TO_LOAD));
		}
	}
	return assembly;
}

BOOL WINAPI LoadAssembly()
{		
	String^ assembly = GetAssembly();

	if (ASSEMBLY_LOAD_EVENT == NULL )
	{
		ASSEMBLY_LOAD_EVENT = CreateEvent(NULL,TRUE,FALSE,ASSEMBLY_LOAD_EVENT_NAME);			
	}	
	if (ASSEMBLY_LOAD_EVENT != NULL )
	{
		SetEvent(ASSEMBLY_LOAD_EVENT);
	}

	if (File::Exists(assembly) == false)
	{				
		return FALSE;
	}

	MethodInfo^ main = TryGetAssemblyMain(assembly);
	if (main == nullptr)
	{		
		return FALSE;
	}

	try
	{
		array<Object^>^ parameters = { nullptr };
		main->Invoke(nullptr,parameters);
		return TRUE;
	}
	catch(Exception^)
	{
		return FALSE;
	}
}


