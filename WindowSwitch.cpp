// WindowSwitch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#ifdef LOGGING
const size_t SZ = 9;
static char res[SZ];

template <class T>
void format_hex(T arg_t)
{
	size_t arg = (size_t)arg_t;
	size_t pos = SZ;
	while (pos > 0)
		res[--pos] = '0';
	pos = SZ;
	while ((arg > 0) && (pos > 0))
	{
		BYTE d = arg % 16;
		if ((d >= 0) && (d <= 9))
			res[--pos] = (d + '0');
		else if ((d >= 10) && (d <= 15))
			res[--pos] = (d - 10 + 'a');
		arg /= 16;
	}
}
#endif

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

#ifdef LOGGING
			DWORD w;
			HANDLE f = CreateFile(L"E:\\Temp\\ws.log", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (f == INVALID_HANDLE_VALUE)
				return 1;
#endif

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

#ifdef LOGGING
				BOOL v = IsWindowVisible(current_wnd);
				HWND owner = GetWindow(current_wnd, GW_OWNER);
				BOOL owner_v = (owner != NULL) && IsWindowVisible(owner);
				BOOL toolwindow = ((GetWindowLongPtr(current_wnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != 0);
				if (v) {
					WriteFile(f, "next_wnd:", 9, &w, NULL);
					format_hex(current_wnd);
					WriteFile(f, res, SZ, &w, NULL);
					WriteFile(f, ", V:", 4, &w, NULL);
					WriteFile(f, (v ? "1 " : "0 "), 2, &w, NULL);
					WriteFile(f, ", T:", 4, &w, NULL);
					WriteFile(f, (toolwindow ? "1 " : "0 "), 2, &w, NULL);
					WriteFile(f, "O:", 2, &w, NULL);
					format_hex(owner);
					WriteFile(f, res, SZ, &w, NULL);
					WriteFile(f, ", V:", 4, &w, NULL);
					WriteFile(f, (owner_v ? "1 " : "0 "), 2, &w, NULL);
					WriteFile(f, "\x0d\x0a", 2, &w, NULL);
				}
#endif

			} while (!IsWindowVisible(current_wnd) ||
			         ((GetWindowLongPtr(current_wnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != 0) ||
			         ((owner != NULL) && IsWindowVisible(owner)));

#ifdef LOGGING
			WriteFile(f, "\x0d\x0aResult:", 9, &w, NULL);
			format_hex(current_wnd);
			WriteFile(f, res, SZ, &w, NULL);
			CloseHandle(f);
#endif

			if (current_wnd != NULL)
				SetForegroundWindow(current_wnd);
		}
	}
	UnregisterHotKey(NULL, 0);

	return 0;
}
