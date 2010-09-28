// WindowSwitch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

int WinMainCRTStartup(void)
{
	if (!RegisterHotKey(NULL, 0, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 'Z'))
		return 1;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_HOTKEY)
        {
			HWND current_wnd = GetForegroundWindow();
			if (current_wnd == NULL)
				continue;

			DWORD pid, next_pid;
			GetWindowThreadProcessId(current_wnd, &pid);

			HWND next_wnd = current_wnd;
			do {
				next_wnd = GetNextWindow(next_wnd, GW_HWNDNEXT);
				if (next_wnd == NULL)
					break;
				GetWindowThreadProcessId(next_wnd, &next_pid);
			} while(!IsWindowVisible(next_wnd) || (next_pid == pid));

			if (next_wnd != NULL)
				SetForegroundWindow(next_wnd);
        }
    }
	UnregisterHotKey(NULL, 0);

	return 0;
}
