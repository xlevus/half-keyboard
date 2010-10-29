//   Copyright 2010 Chris Targett
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.


// Originally based on: 
// MSDN Magazine -- September 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 and Visual Studio .NET on Windows XP.
//
// This file implements the low-level keyboard hook that traps the task keys.

#define _WIN32_WINNT 0x0500 // for KBDLLHOOKSTRUCT
#include <afxwin.h>         // MFC core and standard components

#define DLLEXPORT __declspec(dllexport)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// App (DLL) object
//
class CTaskKeyHookDll : public CWinApp {
public:
	CTaskKeyHookDll()  { }
	~CTaskKeyHookDll() { }
} MyDll;

////////////////
// The section is SHARED among all instances of this DLL.
// A low-level keyboard hook is always a system-wide hook.
// 
#pragma data_seg (".mydata")
HHOOK g_hHookKbdLL = NULL; // hook handle
BOOL  g_bBeep = FALSE;		// beep on illegal key
#pragma data_seg ()
#pragma comment(linker, "/SECTION:.mydata,RWS") // tell linker: make it shared

/////////////////
// Low-level keyboard hook:
// Trap task-switching keys by returning without passing along.
//
LRESULT CALLBACK MyTaskKeyHookLL(int nCode, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;

	if (nCode==HC_ACTION) {
		BOOL bCtrlKeyDown =
			GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);

		if ((pkh->vkCode==VK_ESCAPE && bCtrlKeyDown) ||				    // Ctrl+Esc
			 (pkh->vkCode==VK_TAB && pkh->flags & LLKHF_ALTDOWN) ||   // Alt+TAB
			 (pkh->vkCode==VK_ESCAPE && pkh->flags & LLKHF_ALTDOWN)|| // Alt+Esc
			 (pkh->vkCode==VK_LWIN || pkh->vkCode==VK_RWIN)) { // Start Menu
			if (g_bBeep && (wp==WM_SYSKEYDOWN||wp==WM_KEYDOWN))
				MessageBeep(0); // only beep on downstroke if requested
			return 1; // gobble it: go directly to jail, do not pass go
		}
	}
	return CallNextHookEx(g_hHookKbdLL, nCode, wp, lp);
}

//////////////////
// Are task keys disabled--ie, is hook installed?
// Note: This assumes there's no other hook that does the same thing!
//
extern "C" DLLEXPORT BOOL AreTaskKeysDisabled()
{
	return g_hHookKbdLL != NULL;
}

//////////////////
// Disable task keys: install low-level kbd hook.
// Return whether currently disabled or not.
//
extern "C" DLLEXPORT BOOL DisableTaskKeys(BOOL bDisable, BOOL bBeep)
{
	if (bDisable) {
		if (!g_hHookKbdLL) {
			g_hHookKbdLL = SetWindowsHookEx(WH_KEYBOARD_LL,
				MyTaskKeyHookLL, MyDll.m_hInstance, 0);
		}

	} else if (g_hHookKbdLL != NULL) {
		UnhookWindowsHookEx(g_hHookKbdLL);
		g_hHookKbdLL = NULL;
	}
	g_bBeep = bBeep;

	TRACE("LOL");

	return AreTaskKeysDisabled();
}

