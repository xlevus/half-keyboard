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
#include "stdafx.h"
#include "resource.h"
//#include "StatLink.h"
#include "TaskKeyMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// Main dialog
//
class CMyDialog : public CDialog {
public:
	CMyDialog(CWnd* pParent = NULL) : CDialog(IDD_MYDIALOG, pParent) { }

protected:
	HICON m_hIcon;
	//CStaticLink	m_wndLink1;
	//CStaticLink	m_wndLink2;
	//CStaticLink	m_wndLink3;

	virtual BOOL OnInitDialog();

	// command/UI update handlers
	afx_msg void OnDisableTaskMgr();
	afx_msg void OnDisableTaskKeys();
	afx_msg void OnDisableTaskbar();
	afx_msg void OnUpdateDisableTaskMgr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableTaskKeys(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableTaskbar(CCmdUI* pCmdUI);
	afx_msg LRESULT OnKickIdle(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()
};

//////////////////
// Standard MFC dialog app class in 14 lines + comments
//
class CMyApp : public CWinApp {
public:
	virtual BOOL InitInstance() {
		// init app: run dialog
		CMyDialog dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
		return FALSE;
	}
	virtual int ExitInstance() {
		// For safety, re-enable everything on exit
		CTaskKeyMgr::Disable(CTaskKeyMgr::ALL, FALSE);
		return 0;
	}
} theApp;

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_COMMAND(IDC_DISABLE_TASKKEYS,OnDisableTaskKeys)
	ON_COMMAND(IDC_DISABLE_TASKBAR, OnDisableTaskbar)
	ON_COMMAND(IDC_DISABLE_TASKMGR, OnDisableTaskMgr)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKKEYS, OnUpdateDisableTaskKeys)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKBAR, OnUpdateDisableTaskbar)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKMGR, OnUpdateDisableTaskMgr)
	ON_MESSAGE(WM_KICKIDLE,OnKickIdle)
END_MESSAGE_MAP()

//////////////////
// Initialize dialog: subclass hyperlinks and load icon
//
BOOL CMyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// init my hyperlinks
	//m_wndLink1.SubclassDlgItem(IDC_PDURL,this);
	//m_wndLink2.SubclassDlgItem(IDC_MSDNURL,this);
	//m_wndLink3.SubclassDlgItem(IDC_MSDNLINK,this);

	// Set the icon for this dialog. MFC doesn't do it for dialog apps.
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;
}

////////////////////////////////////////////////////////////////
// Command/UI update handlers. You can write this n your sleep--Zzzz.

void CMyDialog::OnDisableTaskKeys()
{
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKKEYS,
		!CTaskKeyMgr::AreTaskKeysDisabled(), TRUE); // beep
}

void CMyDialog::OnUpdateDisableTaskKeys(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::AreTaskKeysDisabled());
}

void CMyDialog::OnDisableTaskbar()
{
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKBAR,
		!CTaskKeyMgr::IsTaskBarDisabled());
}

void CMyDialog::OnUpdateDisableTaskbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::IsTaskBarDisabled());
}

void CMyDialog::OnDisableTaskMgr()
{
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKMGR,
		!CTaskKeyMgr::IsTaskMgrDisabled());
}

void CMyDialog::OnUpdateDisableTaskMgr(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::IsTaskMgrDisabled());
}

//////////////////
// This is required to make ON_UPDATE_COMMAND_UI work properly.
// Cf. my column in the July 1997 Microsoft Systems Journal.
// 
LRESULT CMyDialog::OnKickIdle(WPARAM wp, LPARAM lCount)
{
	UpdateDialogControls(this, TRUE);
	return 0;
}
