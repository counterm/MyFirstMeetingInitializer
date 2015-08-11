// --------------------------------------------------------------------
// ��ʼ�����Ի����ʱ�򣬻��������ݿ�
// ������԰�ť��Ҳ�������������ݿ�
// ������水ť���������������ݿ�
// --------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include "FormConnection.h"
#include "MySqlHelper.h"
#include "CovertString.h"
#include "MeetingSystemSetting.h"
#include "CProperties.h"
#include <fstream>

extern TCHAR strLog[1000];
#define MYSQLROWSIZE 100

extern HINSTANCE hInst;
extern DBHELPER *dbHelper;
extern FormConnection *dlgConnection;

extern void connectCalling();
INT_PTR CALLBACK FormConnectionWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

FormConnection::FormConnection()
{
	this->data = new TCHAR*[5];
	this->data[0] = (TCHAR*)malloc(sizeof(TCHAR)*MYSQLROWSIZE);
	this->data[1] = (TCHAR*)malloc(sizeof(TCHAR)*MYSQLROWSIZE);
	this->data[2] = (TCHAR*)malloc(sizeof(TCHAR)*MYSQLROWSIZE);
	this->data[3] = (TCHAR*)malloc(sizeof(TCHAR)*MYSQLROWSIZE);
	this->data[4] = (TCHAR*)malloc(sizeof(TCHAR)*MYSQLROWSIZE);

	/*hwndComboDb = GetDlgItem(this->hWnd,IDC_COMBO_CONDB);*/

	this->proc = FormConnectionWndProc;
	//cfgFile = L"config.ini";
	cfgFile = L"hibernate.properties";
	cfgPath = (TCHAR*)calloc(256, sizeof(TCHAR));
	getConfigData(); 
	setAllData();
}


// ���������ļ� ·�����ļ���
const TCHAR*
FormConnection::getConfigPath()
{
	if (cfgPath[0] != 0){
		return cfgPath;
	}

	wstring strFind;
	strFind.append(dbHelper->getPath());
	strFind.append(L"\\");
	strFind.append(cfgFile);

	TCHAR* strRet = (TCHAR*)calloc(256, sizeof(TCHAR));
	lstrcpy(strRet, strFind.c_str());

	return strRet;
};


// ��ȡ�����ļ��е�ֵ
// ������ TCHAR data[5][100]
void
FormConnection::getConfigData()
{
	TCHAR ip[MYSQLROWSIZE] = {0},
		port[MYSQLROWSIZE] = {0};
	size_t index = 0;

	CProperties ps;
	ps.loadFrom(dbHelper->getPath(cfgFile));
 
	wstring name = ps.getProperty(L"dataSource.username")->getString();
	wstring pwd = ps.getProperty(L"dataSource.password")->getString();
	wstring ipPort = ps.getProperty(L"dataSource.serverName")->getString();
	wstring scheme = ps.getProperty(L"dataSource.databaseName")->getString();
 
	index = ipPort.find(L":");

	if ((int)index > -1)
	{
		lstrcpy(ip, ipPort.substr(0, index).c_str());
		lstrcpy(port, ipPort.substr(index+1).c_str());
	}else{
		lstrcpy(ip, ipPort.c_str());
		lstrcpy(port, L"3306");
	}

	lstrcpy(this->data[0], ip);
	lstrcpy(this->data[1], port);
	lstrcpy(this->data[2], name.c_str());
	lstrcpy(this->data[3], pwd.c_str());
	lstrcpy(this->data[4], scheme.c_str());

	//TCHAR ip[MYSQLROWSIZE] = {0},
	//	port[MYSQLROWSIZE] = {0},
	//	ipPort[MYSQLROWSIZE] = {0},
	//	pwd[MYSQLROWSIZE] = {0},
	//	name[MYSQLROWSIZE] = {0},
	//	scheme[MYSQLROWSIZE] = {0};
	//const TCHAR* lpAllPath = getConfigPath();
	//strIpPort = wstring(ipPort);

	//GetPrivateProfileString(NULL, L"dataSource.serverName", NULL, 
	//	ipPort, MYSQLROWSIZE, lpAllPath);
	//GetPrivateProfileString(NULL, L"dataSource.password", NULL, 
	//	pwd, MYSQLROWSIZE, lpAllPath);
	////GetPrivateProfileString(L"mysql", L"port", NULL, 
	////	port, MYSQLROWSIZE, lpAllPath);
	//GetPrivateProfileString(NULL, L"dataSource.username", NULL, 
	//	name, MYSQLROWSIZE, lpAllPath);
	//GetPrivateProfileString(NULL, L"dataSource.databaseName", NULL, 
	//	scheme, MYSQLROWSIZE, lpAllPath);

	//finding = _tcschr(ipPort, ':');

	//index = strIpPort.find(';');
	//if (index > -1)
	//{
	//	lstrcpy(ip, strIpPort.substr(0, index).c_str());
	//	lstrcpy(port, strIpPort.substr(index+1).c_str());
	//}else{
	//	lstrcpy(ip, strIpPort.c_str());
	//	lstrcpy(port, L"3306");
	//}

	//lstrcpy(this->data[0], ip);
	//lstrcpy(this->data[1], port);
	//lstrcpy(this->data[2], name);
	//lstrcpy(this->data[3], pwd);
	//lstrcpy(this->data[4], scheme);



}

// ��ȡ�ļ����е�ֵ
TCHAR*
FormConnection::getAllData()
{
	GetDlgItemText(this->hWnd, IDC_EDIT_CONIP, this->data[0], MYSQLROWSIZE);
	GetDlgItemText(this->hWnd, IDC_EDIT_CONPORT, this->data[1], MYSQLROWSIZE);
	GetDlgItemText(this->hWnd, IDC_EDIT_CONNAME, this->data[2], MYSQLROWSIZE);
	GetDlgItemText(this->hWnd, IDC_EDIT_CONPWD, this->data[3], MYSQLROWSIZE);
	this->getCbScheme();
	return (this->data)[0];
}

// �����ڴ���data��ֵ���ı���
void
FormConnection::setAllData()
{
	SetDlgItemText(this->hWnd, IDC_EDIT_CONIP,		this->data[0]);
	SetDlgItemText(this->hWnd, IDC_EDIT_CONPORT,	this->data[1]);
	SetDlgItemText(this->hWnd, IDC_EDIT_CONNAME,	this->data[2]);
	SetDlgItemText(this->hWnd, IDC_EDIT_CONPWD,	this->data[3]);
	//this->setComboDbSel(this->data[4]);
	//SetDlgItemText(this->hWnd, IDC_COMBO_CONDB, this->data[4]);
	
}

// ��������
bool
FormConnection::saveSetting()
{
	this->getAllData();
	const TCHAR* path = this->getConfigPath();
	if (!!data[0][0] && !!data[1][0] && !!data[2][0]/* && !!data[3][0]*/ && !!data[4][0])
	{
		TCHAR ipPort[128] = {0};
		wsprintf(ipPort, L"%s:%s", getIp(), getPort());

		CProperties ps;
		ps.loadFrom(dbHelper->getPath(cfgFile));
		ps.setProperty(L"dataSource.serverName", ipPort);
		ps.setProperty(L"dataSource.username", getName());
		ps.setProperty(L"dataSource.password", getPwd());
		ps.setProperty(L"dataSource.databaseName", getCbScheme());
		ps.saveProperty();

		//WritePrivateProfileString(NULL,L"dataSource.serverName",	ipPort,	path);
		////WritePrivateProfileString(NULL,L"port",	getPort(),	path);
		//WritePrivateProfileString(NULL,L"dataSource.username",	getName(),	path);
		//WritePrivateProfileString(NULL,L"dataSource.password",	getPwd(),	path);
		//WritePrivateProfileString(NULL,L"dataSource.databaseName",getCbScheme(), path);
		MessageBox(this->hWnd, L"����ɹ�", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return TRUE;
	}else{
		MessageBox(this->hWnd, L"���ϲ�����", L"��ʾ", MB_OK | MB_ICONERROR);
		return	FALSE;
	}
}

const TCHAR*
FormConnection::getData()
{
	return data[0];
}

const TCHAR*
FormConnection::getIp()
{
	return data[0];
}

const TCHAR*
FormConnection::getPort()
{
	return data[1];
}

const TCHAR*
FormConnection::getName()
{
	return data[2];
}

const TCHAR*
FormConnection::getPwd()
{
	return data[3];
}

const TCHAR*
FormConnection::getScheme()
{
	return data[4];
}

const TCHAR*
FormConnection::getCbScheme()
{
	hwndComboDb = GetDlgItem(this->hWnd, IDC_COMBO_CONDB);
	// ����Ǳ� getAllData() ���õĻ�
	// ������û�б���ʼ��������Ҫ����
	// ��û�����������ʱ�򣬲���Ҫ����ִ��
	int iComCount = SendMessage(hwndComboDb, CB_GETCOUNT, 0, 0);
	if (iComCount == 0)
	{
		return L"";
	}
	int cbIndex = SendMessage(hwndComboDb, CB_GETCURSEL, 0, 0);
	//TCHAR tmp[256] = {0};
	SendMessage(hwndComboDb, CB_GETLBTEXT, cbIndex , (LPARAM)this->data[4]);
	//wsprintf(strLog, L"%d", cbIndex);
	//lstrcpy(this->data[4], tmp);
	return this->data[4];
}

// ���ݿ����Ӻ��һЩ�ؼ����²���
void
FormConnection::connectCalling()
{
	this->initCombobox(IDC_COMBO_CONDB, dbHelper->databases, 
		sizeof(dbHelper->databases)/sizeof(dbHelper->databases[0]));
	if (this->data[4][0] == 0){
		this->setComboSel(IDC_COMBO_CONDB, 0);
	}else{
		this->setComboSel(IDC_COMBO_CONDB, this->data[4]);
	}
}

INT_PTR CALLBACK
PwdWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CLOSE:
		// MessageBox(hDlg, L"123", L"", MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		// break;
	case WM_INITDIALOG:
		{
			const TCHAR *pwd = dlgConnection->getPwd();
			SetDlgItemText(hDlg, IDC_EDIT_PWD_1,pwd);
		}
		return (INT_PTR)TRUE;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{
		case IDC_BUTTON_PWD:
			{
				TCHAR *execStr = (TCHAR*)calloc(256, sizeof(TCHAR));
				TCHAR *pwd1 = (TCHAR*)calloc(256, sizeof(TCHAR));
				TCHAR *pwd2 = (TCHAR*)calloc(256, sizeof(TCHAR));
				TCHAR *pwd3 = (TCHAR*)calloc(256, sizeof(TCHAR));
				//TCHAR pwd[3][256] = {{0},{0},{0}};
				// ԭʼ����
				GetDlgItemText(hDlg, IDC_EDIT_PWD_1, pwd1, 256);
				// ������
				GetDlgItemText(hDlg, IDC_EDIT_PWD_2, pwd2, 256);
				// �ظ�������
				GetDlgItemText(hDlg, IDC_EDIT_PWD_3, pwd3, 256);
				if (pwd2[0] == 0 || pwd3[0] == 0)
				{
					MessageBox(hDlg, L"����������", L"����", MB_OK | MB_ICONWARNING);
					break;
				}
				if (0 != lstrcmp(pwd2, pwd3))
				{
					MessageBox(hDlg, L"�����������벻һ��", L"����", MB_OK | MB_ICONWARNING);
					break;
				}

				//������ӳɹ����
				SetDlgItemText(dlgConnection->hWnd, IDC_EDIT_CONPWD, pwd1);
				lstrcpy(dlgConnection->data[3], pwd1);
				dbHelper->init(
					dlgConnection->getIp(),
					dlgConnection->getPort(),
					dlgConnection->getName(),
					dlgConnection->getPwd(),
					dlgConnection->getScheme()
					);
				//if (!dbHelper->connect())
				//{
				//	MessageBox(hDlg, L"ԭ����������޸ġ�", L"��ʾ", MB_OK | MB_ICONERROR);
				//	break;
				//}
				
				// �޸�����
				wsprintf(execStr, L"mysqladmin --host=%s --port=%s -u %s -p%s password %s", 
						dlgConnection->getIp(),
						dlgConnection->getPort(),
						dlgConnection->getName(),
						pwd1,
						pwd2
					);
				dlgConnection->processWaitUntilDone(NULL, execStr);

				// ����
				SetDlgItemText(dlgConnection->hWnd, IDC_EDIT_CONPWD, pwd2);
				lstrcpy(dlgConnection->data[3], pwd2);
				dbHelper->init(
					dlgConnection->getIp(),
					dlgConnection->getPort(),
					dlgConnection->getName(),
					dlgConnection->getPwd(),
					dlgConnection->getScheme()
					);
				if (!dbHelper->connect())
				{
					MessageBox(hDlg, L"�޸�����ʧ��", L"��ʾ", MB_OK | MB_ICONERROR);
					break;
				}

				// ����ֵ
				SetDlgItemText(dlgConnection->hWnd, IDC_EDIT_CONPWD, pwd2);
				MessageBox(hDlg, L"�޸�����ɹ�", L"��ʾ", MB_OK | MB_ICONINFORMATION);
				EndDialog(hDlg, LOWORD(wParam));
			}
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK
FormConnectionWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		int wmId, wmEvent;
		bool saveSuccess;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{
		case IDC_COMBO_CONDB:
			if (wmEvent == CBN_SELCHANGE)
			{
				dlgConnection->getCbScheme();
			}
			break;
		// ��������
		case IDC_BUTTON_CONTEST:
			SendDlgItemMessage(dlgConnection->hWnd, IDC_COMBO_CONDB, CB_RESETCONTENT, 0, 0);
			memset(dlgConnection->data[4], 0, sizeof(dlgConnection->data[4]));
			dlgConnection->getAllData();
			dbHelper->init(
				dlgConnection->getIp(),
				dlgConnection->getPort(),
				dlgConnection->getName(),
				dlgConnection->getPwd(),
				dlgConnection->getScheme()
				);
			if (dbHelper->connect())
			{
				MessageBox(hDlg, L"���ӳɹ�", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			}else{
				// MessageBox(hDlg, L"����ʧ��", L"��ʾ", MB_OK | MB_ICONERROR);
			}
			break;
		// �������Ӳ���
		case IDC_BUTTON_CONSAVE:
			saveSuccess = dlgConnection->saveSetting();
			if (saveSuccess)
			{
				dbHelper->init(
					dlgConnection->getIp(),
					dlgConnection->getPort(),
					dlgConnection->getName(),
					dlgConnection->getPwd(),
					dlgConnection->getScheme()
					);
				dbHelper->connect();
			}
			break;
		// �������ݿ�
		case IDC_BUTTON_CONCREATE:
			{
				bool hasError = FALSE;
				TCHAR *dbName = new TCHAR[256];
				GetDlgItemText(hDlg, IDC_EDIT_CONCREATE, dbName, 256);
				if (dbName[0] == 0)
				{
					MessageBox(dlgConnection->hWnd, L"���������ݿ�����", L"����", MB_OK);
					return (INT_PTR)FALSE;
				}
				TCHAR sql[256] = {0};
				wsprintf(sql, L"create database %s", dbName);
				try{
					dbHelper->execute(sql);
				}catch(exception& e){
					hasError = TRUE;
					UNREFERENCED_PARAMETER(e);
					wsprintf(strLog, L"�������ݿ�ʧ��:\n%s", s2ws(e.what()));
					MessageBox(hDlg, strLog, L"����", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}
				if (!hasError)
				{
					MessageBox(hDlg, L"�������ݿ�ɹ�", L"��ʾ", MB_OK | MB_ICONINFORMATION);
					connectCalling();
				}
			}
			break;
		// �޸�����Ի���
		case IDC_BUTTON_CONPWD:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_MODIFYPASSWORD), hWndMain, PwdWndProc);
			}
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}