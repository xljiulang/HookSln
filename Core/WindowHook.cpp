#include "stdafx.h"
#include <Windows.h>
#include <tlhelp32.h>

typedef BOOL (CALLBACK* HookDelegate)(int nCode, WPARAM wParam, LPARAM lParam);

typedef struct HookItem
{
	HookDelegate HookDelegate;
	HHOOK HHOOK;	
};

HookItem HookItemList[15];



DWORD GetMainThreadId(DWORD processId = 0)
{
	if(processId == 0)
	{
		processId = GetCurrentProcessId();
	}

	THREADENTRY32 te32 = {sizeof(te32)};
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if( Thread32First(hThreadSnap, &te32) )
	{
		do{
			if( processId == te32.th32OwnerProcessID )
			{
				return te32.th32ThreadID;
			}
		}while( Thread32Next(hThreadSnap, &te32) );
	}
	return 0;
}

LRESULT CALLBACK HookProc(int idHook, int nCode, WPARAM wParam, LPARAM lParam)
{	
	HookItem hookItem = HookItemList[idHook + 1];

	if (nCode < 0)
	{
		return CallNextHookEx(hookItem.HHOOK,nCode,wParam,lParam);
	}

	if(hookItem.HookDelegate != NULL && hookItem.HookDelegate(nCode,wParam,lParam) )
	{ 
		return 1 ;
	}
	return CallNextHookEx(hookItem.HHOOK,nCode,wParam,lParam);
}


LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_CALLWNDPROC, nCode,wParam,lParam);
}

LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_CALLWNDPROCRET, nCode,wParam,lParam);
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_CBT, nCode,wParam,lParam);
}
LRESULT CALLBACK DebugProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_DEBUG, nCode,wParam,lParam);
}

LRESULT CALLBACK ForegroundIdleProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_FOREGROUNDIDLE, nCode,wParam,lParam);
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_GETMESSAGE, nCode,wParam,lParam);
}
LRESULT CALLBACK JournalPlaybackProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_JOURNALPLAYBACK, nCode,wParam,lParam);
}

LRESULT CALLBACK JournalRecordProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_JOURNALRECORD, nCode,wParam,lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_KEYBOARD, nCode,wParam,lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_KEYBOARD_LL, nCode,wParam,lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_MOUSE, nCode,wParam,lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_MOUSE_LL, nCode,wParam,lParam);
}

LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_MSGFILTER, nCode,wParam,lParam);
}

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam,LPARAM lParam)
{
	return HookProc(WH_SHELL, nCode,wParam,lParam);
}

LRESULT CALLBACK SysMsgProc(int nCode, WPARAM wParam,LPARAM lParam)
{	
	return HookProc(WH_SYSMSGFILTER, nCode,wParam,lParam);
}


HOOKPROC GetHookProc(int idHook)
{
	switch (idHook)
	{
	case WH_CALLWNDPROC:
		return CallWndProc;		
	case WH_CALLWNDPROCRET:
		return CallWndRetProc;		
	case WH_CBT:
		return CBTProc;		
	case WH_DEBUG:
		return DebugProc;		
	case WH_FOREGROUNDIDLE :
		return ForegroundIdleProc;		
	case WH_GETMESSAGE:
		return GetMsgProc;		
	case WH_JOURNALPLAYBACK:
		return JournalPlaybackProc;		
	case WH_JOURNALRECORD:
		return JournalRecordProc ;		
	case WH_KEYBOARD:
		return KeyboardProc ;		
	case WH_KEYBOARD_LL:
		return LowLevelKeyboardProc;		
	case WH_MOUSE:
		return MouseProc;		
	case WH_MOUSE_LL:
		return LowLevelMouseProc;		
	case WH_MSGFILTER:
		return MessageProc ;		
	case WH_SHELL:
		return ShellProc;		
	case WH_SYSMSGFILTER:
		return SysMsgProc ;		
	default:
		return NULL;
	}
}

void WINAPI UnHookWindow(int idHook)
{
	HHOOK hHook = HookItemList[idHook +1].HHOOK;
	if (hHook > 0)
	{
		UnhookWindowsHookEx(hHook);
		HookItemList[idHook +1].HHOOK = NULL;
		HookItemList[idHook +1].HookDelegate = NULL;
	}	 
}

BOOL WINAPI HookWindow(int idHook, HookDelegate hookDelegate)
{ 
	HOOKPROC hookProc = GetHookProc(idHook);
	if (hookProc == NULL )
	{
		return NULL;
	}

	UnHookWindow(idHook);

	DWORD mainThreadId = GetMainThreadId();
	HHOOK hHook = SetWindowsHookEx(idHook,hookProc,NULL,mainThreadId);	
	if(hHook >0)
	{
		int index = idHook +1;
		HookItemList[index].HHOOK = hHook;
		HookItemList[index].HookDelegate = hookDelegate;			
	}
	return hHook > 0;
}


