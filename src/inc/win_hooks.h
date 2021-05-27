#pragma once
#include <Windows.h>
#include "quicktype.h"


extern QuickTypeBL quickType;
HWINEVENTHOOK SetWindowsEventHook_ForegroundWindow(WINEVENTPROC proc)
{
	HWINEVENTHOOK hook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND,
		EVENT_SYSTEM_FOREGROUND, nullptr, proc, 0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	return hook;
}

void CALLBACK WindowsEventHook_CallBack(HWINEVENTHOOK hWinEventHook,
	DWORD eventType, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread,
	DWORD dwmsEventTime)
{
	if(eventType == EVENT_SYSTEM_FOREGROUND)
	{
		quickType.handleWindowsEventHookCallback(hWinEventHook,
			eventType,
			hwnd,
			idObject,
			idChild,
			dwEventThread,
			dwmsEventTime);
	}
}



