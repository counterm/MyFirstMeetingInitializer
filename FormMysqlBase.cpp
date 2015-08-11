#include "stdafx.h"
#include <Shellapi.h>
#include "CovertString.h"
#include "MeetingSystemSetting.h"
#include "FormMysqlBase.h"
#include "MySqlHelper.h"

extern DBHELPER *dbHelper;
extern HWND hWndMain;
extern HINSTANCE hInst;
INT_PTR CALLBACK FormLoadingWndProc(HWND, UINT, WPARAM, LPARAM);

FormMysqlBase::FormMysqlBase(void)
{
}


FormMysqlBase::~FormMysqlBase(void)
{
}

HWND
FormMysqlBase::Create(HINSTANCE hInst, int idd, HWND hWnd)
{
	this->hWnd = CreateDialog(hInst, MAKEINTRESOURCE(idd), hWnd, this->proc);
	this->hide();
	return this->hWnd;
}


void
FormMysqlBase::show()
{
	ShowWindow(this->hWnd, SW_SHOWNA);
}

void
FormMysqlBase::hide()
{
	ShowWindow(this->hWnd, SW_HIDE);
}


void FormMysqlBase::initCombobox(int idc, wstring arr[], int arrLength)
{
	HWND hwndComboDb = GetDlgItem(this->hWnd, idc);

	SendMessage(hwndComboDb, CB_RESETCONTENT, 0, 0);
	
	for(int i = 0; i < arrLength; i++)
	{
		if (arr[i].c_str()[0] == '\0')
		{
			break;
		}
		SendMessage(hwndComboDb, CB_ADDSTRING, 0, (LPARAM)(arr[i].c_str()));
	}
}

void
FormMysqlBase::setComboSel(int idc, int index)
{
	int iComCount = 0;
	TCHAR cbText[256] = {0};
	HWND hwndCombo = GetDlgItem(this->hWnd,idc);
	iComCount = SendMessage(hwndCombo, CB_GETCOUNT, 0, 0);
	if (index < 0 && index >= iComCount){
		index = 0;
	}
	SendMessage(hwndCombo, CB_SETCURSEL, index, 0);
}

void
FormMysqlBase::setComboSel(int idc, const TCHAR* selName)
{
	if (selName[0] == 0)
	{
		return;
	}

	HWND hwndCombo = GetDlgItem(this->hWnd,idc);

	int iComCount = 0;
	TCHAR cbText[256] = {0};
	iComCount = SendMessage(hwndCombo, CB_GETCOUNT, 0, 0);

	for (int i = 0; i < iComCount; i++)
	{
		SendMessage(hwndCombo, CB_GETLBTEXT, i, (LPARAM)cbText);
		if (lstrcmp(cbText,selName) == 0)
		{
			SendMessage(hwndCombo, CB_SETCURSEL, i, 0);
			break;
		}
	}
}


void
FormMysqlBase::getComboSelString(int idc, TCHAR *ret)
{
	HWND hwndCombo = GetDlgItem(this->hWnd,idc);
	//TCHAR ret[256] = {0};
	int iSel = CB_ERR;
	iSel = SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);

	if (iSel != CB_ERR)
	{
		SendMessage(hwndCombo, CB_GETLBTEXT, iSel , (LPARAM)ret);
	}else{
		ret[0] = 0;
	}
}

DWORD
FormMysqlBase::processWaitUntilDone(TCHAR* strcmdline, LPWSTR strcmdline2)
{
	dbHelper->foreignKeyCheck(FALSE);
	system(ws2s(strcmdline2).c_str());//"e://can//vc//win32//win32project1//release//mysqldump.exe -u root -p123456 a12>e://can//VC//win32//Win32Project1//Release//2015-7-17_9-35-33.187.sql");
	dbHelper->foreignKeyCheck(TRUE);
	return 1;
	TCHAR tmp[128] = {0};
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	// set up members of startupinfo structure.
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.lpReserved = NULL;
	siStartInfo.lpReserved2 = NULL;
	siStartInfo.cbReserved2 = 0;
	siStartInfo.lpDesktop = NULL;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW;
	siStartInfo.wShowWindow = SW_SHOW;

	// create the child process.
	if (0==CreateProcess(
			NULL, //strcmdline,
			 strcmdline2,//(LPWSTR)strcmdline,
			//L"mysqldump.exe",
			//L"",
			NULL, // process security attributes
			NULL, // primary thread security attributes
			0, // handles are inherited
			0, // creation flags
			NULL, // use parent\'s environment
			NULL, // use parent\'s current directory
			&siStartInfo, // startupinfo pointer
			&piProcInfo) // receives process_information
			// wait for the processs to finish
		){
		//MessageBox(hWndMain, L"²Ù×÷Ê§°Ü", L"", MB_OK);
		//DWORD err = GetLastError();
		//wsprintf(tmp, L"%d", err);
		//MessageBox(hWndMain, tmp, L"", MB_OK);
		return 0;
	}else{
	}
	
	DWORD rc = WaitForSingleObject(
		piProcInfo.hProcess, // process handle
		INFINITE);
	return 1;
}