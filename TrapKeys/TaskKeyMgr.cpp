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
#include "StdAfx.h"
#include "TaskKeyMgr.h"

#define HKCU HKEY_CURRENT_USER

// Magic registry key/value for "Remove Task Manager" policy.
//
LPCTSTR KEY_DisableTaskMgr =
	"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
LPCTSTR VAL_DisableTaskMgr = "DisableTaskMgr";

//////////////////
// Disable task-key related stuff.
// 
// dwFlags  = what to disable
// bDisable = disable (TRUE) or enable (FALSE)
// bBeep    = whether to beep for illegal keys (TASKKEYS only)
//
void CTaskKeyMgr::Disable(DWORD dwFlags, BOOL bDisable, BOOL bBeep)
{
	// task manager (Ctrl+Alt+Del)
	if (dwFlags & TASKMGR) {
		HKEY hk;
		if (RegOpenKey(HKCU, KEY_DisableTaskMgr,&hk)!=ERROR_SUCCESS)
			RegCreateKey(HKCU, KEY_DisableTaskMgr, &hk);

		if (bDisable) { // disable TM: set policy = 1
			DWORD val=1;
			RegSetValueEx(hk, VAL_DisableTaskMgr, NULL,
				REG_DWORD, (BYTE*)&val, sizeof(val));

		} else { // enable TM: remove policy 
			RegDeleteValue(hk,VAL_DisableTaskMgr);
		}
	}

	// task keys (Alt-TAB etc)
	if (dwFlags & TASKKEYS)
		::DisableTaskKeys(bDisable,bBeep); // install keyboard hook

	// task bar
	if (dwFlags & TASKBAR) {
		HWND hwnd = FindWindow("Shell_traywnd", NULL);
		EnableWindow(hwnd, !bDisable);
	}
}

BOOL CTaskKeyMgr::IsTaskBarDisabled()
{
	HWND hwnd = FindWindow("Shell_traywnd", NULL);
	return IsWindow(hwnd) ? !IsWindowEnabled(hwnd) : TRUE;
}

BOOL CTaskKeyMgr::IsTaskMgrDisabled()
{
	HKEY hk;
	if (RegOpenKey(HKCU, KEY_DisableTaskMgr, &hk)!=ERROR_SUCCESS)
		return FALSE; // no key ==> not disabled

	DWORD val=0;
	DWORD len=4;
	return RegQueryValueEx(hk, VAL_DisableTaskMgr,
		NULL, NULL, (BYTE*)&val, &len)==ERROR_SUCCESS && val==1;
}
