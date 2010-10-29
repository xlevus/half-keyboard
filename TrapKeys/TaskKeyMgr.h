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
#pragma once
#include "TaskKeyHook.h"

//////////////////
// Use this class to disable task keys, task manager and/or the taskbar.
// Call Disable with flags for items you want to disable; for example:
//
// CTaskMgrKeys::Disable(CTaskMgrKeys::ALL);
// 
class CTaskKeyMgr {
public:
	enum {
		TASKMGR  = 0x01,	// disable task manager (Ctrl+Alt+Del)
		TASKKEYS = 0x02,	// disable task keys (Alt-TAB, etc)
		TASKBAR  = 0x04,	// disable task bar
		ALL=0xFFFF			// disable everything :(
	};
	static void Disable(DWORD dwItem,BOOL bDisable,BOOL bBeep=FALSE);

	static BOOL IsTaskMgrDisabled();
	static BOOL IsTaskBarDisabled();
	static BOOL AreTaskKeysDisabled() {
		return ::AreTaskKeysDisabled(); // call DLL 
	}
};
