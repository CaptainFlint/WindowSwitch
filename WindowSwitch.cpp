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

			// Find top-level owner of the current window
			HWND owner = current_wnd;
			do 
			{
				current_wnd = owner;
				owner = GetWindow(current_wnd, GW_OWNER);
			} while ((owner != NULL) && IsWindowVisible(owner));

			// Find next window in Z-stack to switch to
			do {
				current_wnd = GetNextWindow(current_wnd, GW_HWNDNEXT);
				if (current_wnd == NULL)
					break;
				owner = GetWindow(current_wnd, GW_OWNER);
				// Skip all windows that:
				// a) are invisible, or
				// b) have extended style WS_EX_TOOLWINDOW set, or
				// c) have existing and visible owner window.
			} while (!IsWindowVisible(current_wnd) ||
			         ((GetWindowLongPtr(current_wnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != 0) ||
			         ((owner != NULL) && IsWindowVisible(owner)));

			if (current_wnd != NULL)
				SetForegroundWindow(current_wnd);
        }
    }
	UnregisterHotKey(NULL, 0);

	return 0;
}
