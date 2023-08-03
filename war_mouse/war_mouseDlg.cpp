
// war_mouseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "war_mouse.h"
#include "war_mouseDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <tlhelp32.h>
#include <signal.h>  // For kill() function

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwar_mouseDlg dialog

UINT set_hotkey(LPVOID pParam);
UINT set_hotkey_quit(LPVOID pParam);
UINT set_hotkey_hide(LPVOID pParam);
void ConfineCursor();
void ReleaseCursor();
bool terminateProcessByName(const char* processName);
BOOL bstart = false;
BOOL bhide = false;
BOOL benable = false;
CString Target_window_Name_eng = "Warcraft III";
HWND hWindowHandle = NULL;

Cwar_mouseDlg::Cwar_mouseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cwar_mouseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwar_mouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cwar_mouseDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cwar_mouseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Cwar_mouseDlg::OnBnClickedCancel)
//	ON_WM_ACTIVATE()
ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// Cwar_mouseDlg message handlers

BOOL Cwar_mouseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cwar_mouseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cwar_mouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cwar_mouseDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	bstart = true;
	hWindowHandle = ::FindWindow(NULL, Target_window_Name_eng);
	if (hWindowHandle == NULL)
	{
		AfxMessageBox("can't find bab po, please run again");
	}
	else
	{
		ShowWindow(SW_HIDE);
		AfxBeginThread(set_hotkey,0);
		AfxBeginThread(set_hotkey_quit,0);
		AfxBeginThread(set_hotkey_hide,0);
	}
}

UINT set_hotkey(LPVOID pParam)
{
	while (bstart)
	{
		if (RegisterHotKey(
			NULL,
			1,
			MOD_ALT | MOD_NOREPEAT,
			0x45))  //0x42 is 'e'
		{
			MSG msg = {0};
			while (GetMessage(&msg, NULL, 0, 0) != 0 && bstart)
			{
				if (msg.message == WM_HOTKEY)
				{
					benable = !benable;
					if (benable)
					{
						ConfineCursor();
					}
					else
					{
						ReleaseCursor();
					}
				}
			} 
		}
	}
	return 1;
}

UINT set_hotkey_quit(LPVOID pParam)
{
	while (bstart)
	{
		if (RegisterHotKey(
			NULL,
			1,
			MOD_ALT | MOD_NOREPEAT | MOD_CONTROL,
			0x51))  //0x42 is 'q'
		{
			MSG msg = {0};
			while (GetMessage(&msg, NULL, 0, 0) != 0 && bstart)
			{
				if (msg.message == WM_HOTKEY)
				{
					DWORD dwProcessId;
					GetWindowThreadProcessId(hWindowHandle, &dwProcessId);
					
					CString command;
					command.Format(_T("taskkill /F /PID %d"), dwProcessId); // Construct the taskkill command
					int result = system(command); // Execute the taskkill command

					ReleaseCursor();
					exit(0);
				}
			} 
		}
	}
	return 1;
}

UINT set_hotkey_hide(LPVOID pParam)
{
	while (bstart)
	{
		if (RegisterHotKey(
			NULL,
			1,
			MOD_ALT | MOD_NOREPEAT,
			0x4E))  //0x42 is 'n'
		{
			MSG msg = {0};
			while (GetMessage(&msg, NULL, 0, 0) != 0 && bstart)
			{
				if (msg.message == WM_HOTKEY)
				{
					bhide = !bhide;
					if (bhide)
					{
						::ShowWindow(hWindowHandle, SW_HIDE);
					}
					else
					{
						::ShowWindow(hWindowHandle, SW_SHOW);
					}
				}
			} 
		}
	}
	return 1;
}

void Cwar_mouseDlg::OnBnClickedCancel()
{
	OnCancel();
}

void ConfineCursor()
{
	hWindowHandle = ::FindWindow(NULL, Target_window_Name_eng);
	RECT rect;
	GetClientRect(hWindowHandle, &rect);
	MapWindowPoints(hWindowHandle, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void ReleaseCursor()
{
	ClipCursor(nullptr);
}


void Cwar_mouseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	if (LOWORD(nState) != WA_INACTIVE && bhide)
	{
		ConfineCursor();
	}
	else
	{
		// If the window has been deactivated, release the mouse cursor
		ReleaseCursor();
	}
	// TODO: Add your message handler code here
}

bool terminateProcessByName(const char* processName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
	if (Process32First(hSnap, &pe32)) {
		do {
			if (_stricmp(pe32.szExeFile, processName) == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
				if (hProcess != NULL) {
					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
					CloseHandle(hSnap);
					return true;
				}
			}
		} while (Process32Next(hSnap, &pe32));
	}

	CloseHandle(hSnap);
	return false;
}