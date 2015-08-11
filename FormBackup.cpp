#include "stdafx.h"
#include <windows.h>
#include "FormBackup.h"
#include "FormConnection.h"
#include "MeetingSystemSetting.h"
#include <ctime>


extern TCHAR strLog[1000];
extern DBHELPER *dbHelper;
extern FormConnection *dlgConnection;
extern FormBackup *dlgBackup;
INT_PTR CALLBACK FormBackupWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

FormBackup::FormBackup(void)
{
	this->proc = FormBackupWndProc;
	memset(this->valListBox, 0, sizeof(this->valListBox));	//?
}


FormBackup::~FormBackup(void)
{
}

HWND
FormBackup::Create(HINSTANCE hInst, int idd, HWND hWnd)
{
	FormMysqlBase::Create(hInst, idd, hWnd);
	this->initListView();
	return this->hWnd;
}


void
FormBackup::initListView()
{
	HWND listView = GetDlgItem(this->hWnd, IDC_LIST_BACKUP);
	WIN32_FIND_DATA data;
	HANDLE hSearch;
	const TCHAR* path = dbHelper->getPath(L"*.sql");

	SendMessage(listView, LB_RESETCONTENT, 0, 0);

	hSearch = FindFirstFile(path, &data);
	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do{
			//MessageBox(this->hWnd, data.cFileName, L"", MB_OK);
			SendMessage(listView, LB_ADDSTRING, 0, (LPARAM)(data.cFileName));
		}while(FindNextFile(hSearch, &data));
	}
	
	FindClose(hSearch);
}

// 数据库连接后的一些控件更新操作
void 
FormBackup::connectCalling()
{
	this->initDbCombobox();
	//if (dlgConnection->getScheme()[0] == 0){
	//	this->setComboSel(IDC_COMBO_BACKUPDB, 0);
	//}else{
	//	this->setComboSel(IDC_COMBO_BACKUPDB, dlgConnection->getScheme());
	//}
}

void
FormBackup::initDbCombobox()
{
	this->initCombobox(IDC_COMBO_BACKUPDB, dbHelper->databases, 
		sizeof(dbHelper->databases)/sizeof(dbHelper->databases[0]));
}



INT_PTR CALLBACK
FormBackupWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDC_BUTTON_BACKUPRECOVER:
			if (MessageBox(dlgBackup->hWnd, L"确定操作吗？", L"提示", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			if (dlgBackup->valListBox[0] == 0)
			{
				MessageBox(hWndMain, L"请选择需要恢复的脚本", L"错误", MB_OK);
				return (INT_PTR)FALSE;
			}else{
				{
					TCHAR dumpString[512] = {0};
					TCHAR *dbSelected = (TCHAR*)calloc(256, sizeof(TCHAR));
					const TCHAR *filePath = dbHelper->getPath(dlgBackup->valListBox);
					dlgBackup->getComboSelString(IDC_COMBO_BACKUPDB, dbSelected);
					if (dbSelected[0] == 0)
					{
						MessageBox(hWndMain, L"请选择需要恢复的数据库", L"错误", MB_OK | MB_ICONERROR);
						return (INT_PTR)FALSE;
					}
					wsprintf(dumpString, L"mysql -h %s -P %s -u %s -p%s %s < %s",
						dlgConnection->getIp(),
						dlgConnection->getPort(),
						dlgConnection->getName(),
						dlgConnection->getPwd(),
						dbSelected,
						filePath
						);
					if (dlgBackup->processWaitUntilDone(NULL, dumpString))
					{
						MessageBox(hWndMain, L"操作完成", L"提示", MB_OK);
					}
				}
			}
			break;
		case IDC_LIST_BACKUP:
			switch(wmEvent)
			{
			case LBN_SELCHANGE:
				{
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST_BACKUP);
					TCHAR *selText = (TCHAR*)calloc(128, sizeof(TCHAR));
					int sel = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
					SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)selText);
					lstrcpy(dlgBackup->valListBox, selText);
				}
			}
			break;
		case IDC_BUTTON_BACKUPBACKUP:
			if (MessageBox(dlgBackup->hWnd, L"确定操作吗？", L"提示", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			//ShellExecute(
			{
				const TCHAR* path = dbHelper->getPath(NULL);
				TCHAR *arg1 = (TCHAR*)calloc(512, sizeof(TCHAR));
				TCHAR *arg2 = (TCHAR*)calloc(512, sizeof(TCHAR));
				TCHAR *dbName = (TCHAR*)calloc(128, sizeof(TCHAR));

				SYSTEMTIME sysTime;
				ZeroMemory(&sysTime, sizeof(sysTime));
				GetLocalTime(&sysTime);

				wsprintf(arg1, L"%s\\mysqldump.exe", path);
				dlgBackup->getComboSelString(IDC_COMBO_BACKUPDB, dbName);
				if (dbName[0]==0)
				{
					MessageBox(dlgBackup->hWnd, L"请选择数据库。", L"提示", MB_OK);
					return (INT_PTR)FALSE;
				}
				wsprintf(arg2, L"mysqldump -u %s -p%s %s > %s\\%s__%d-%d-%d_%d-%d-%d.%d.sql",
					//path,
					dlgConnection->getName(),
					dlgConnection->getPwd(),
					dbName,
					path,
					dbName,
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond,
					sysTime.wMilliseconds
					);
				// lstrcpy(arg2, L"e://can//vc//win32//win32project1//release//mysqldump.exe -u root -p123456 a12>e://can//VC//win32//Win32Project1//Release//2015-7-17_9-35-33.187.sql");
				/*MessageBox(hWndMain, arg1, L"", MB_OK);
				MessageBox(hWndMain, arg2, L"", MB_OK);*/
				if (0 != dlgBackup->processWaitUntilDone(arg1, arg2) )
				{
					MessageBox(hWndMain, L"操作成功", L"", MB_OK);
					dlgBackup->initListView();
				}
			}
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}