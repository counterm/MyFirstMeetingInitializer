#include "stdafx.h"
#include <fstream>
#include "MySqlHelper.h"
#include "CovertString.h"
#include "MeetingSystemSetting.h"
#include "FormInitialize.h"
#include "FormConnection.h"

//#include <boost/archive/detail/utf8_codecvt_facet.hpp>

#define SQLSTRUCT L"struct.sql"
#define SQLDATA L"data.sql"

extern TCHAR strLog[1000];
extern DBHELPER *dbHelper;
extern FormInitialize *dlgInitialize;
extern FormConnection *dlgConnection;
extern void getFileContentAnsii(const TCHAR *, wstring &);

INT_PTR CALLBACK FormInitializeWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

FormInitialize::FormInitialize(void)
{
	this->proc = FormInitializeWndProc;
}


FormInitialize::~FormInitialize(void)
{
}

void FormInitialize::connectCalling()
{
	this->initCombobox(IDC_COMBO_INITDB, dbHelper->databases, 
		sizeof(dbHelper->databases)/sizeof(dbHelper->databases[0]));

	if (dlgConnection->getScheme()[0] == 0){
		this->setComboSel(IDC_COMBO_INITDB, 0);
	}else{
		this->setComboSel(IDC_COMBO_INITDB, dlgConnection->getScheme());
	}
}


INT_PTR CALLBACK FormInitializeWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// SendDlgItemMessage(dlgInitialize->hWnd, IDC_BUTTON_INITRECOVERDATA, BM_SETSTATE, 0, BN_DISABLE);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		register wstring sqlFileContent;
		char* temp = (char*)calloc(512, sizeof(char));
		const TCHAR* pathStruct = dbHelper->getPath(SQLSTRUCT);
		const TCHAR* pathData = dbHelper->getPath(SQLDATA);

		switch (wmId)
		{
		// ������б�
		case IDC_BUTTON_INITCLEARSTRUCT:
			if (MessageBox(dlgInitialize->hWnd, L"ȷ��������", L"��ʾ", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			{
				TCHAR name[128] = {0};
				dlgInitialize->getComboSelString(IDC_COMBO_INITDB, name);
				dbHelper->dropAllTable(name);
				MessageBox(dlgInitialize->hWnd, L"�������", L"��ʾ", MB_OK);
			}
			break;
		// �ָ��ṹ
		case IDC_BUTTON_INITRECOVERSTRUCT:
			if (MessageBox(dlgInitialize->hWnd, L"ȷ��������", L"��ʾ", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			/*createLoadingDialog();
			destroyLoadingDialog();*/
			//break;
			{
				TCHAR name[128] = {0};
				dlgInitialize->getComboSelString(IDC_COMBO_INITDB, name);
				if (name == NULL)
				{
					MessageBox(dlgInitialize->hWnd, L"��û��ѡ�����ݿ�", L"����", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}
				dbHelper->setSchema(name);
			}
			sqlFileContent.clear();
			getFileContentAnsii(pathStruct, sqlFileContent);
			try{
				// createLoadingDialog();
				dbHelper->execRecoverStruct(sqlFileContent.c_str());
			}catch(exception &e){
				destroyLoadingDialog();
				MessageBox(dlgInitialize->hWnd, s2ws(e.what()).c_str(), L"���ݿ����", MB_OK | MB_ICONERROR);
				return (INT_PTR)FALSE;
			}
			// destroyLoadingDialog();
			MessageBox(dlgInitialize->hWnd, L"�ָ��ṹ�ɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			break;
		// �ָ�����
		case IDC_BUTTON_INITRECOVERDATA:
			if (MessageBox(dlgInitialize->hWnd, L"ȷ��������", L"��ʾ", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			{
				TCHAR name[128] = {0};
				dlgInitialize->getComboSelString(IDC_COMBO_INITDB, name);
				if (name == NULL)
				{
					MessageBox(dlgInitialize->hWnd, L"��û��ѡ�����ݿ�", L"����", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}
				dbHelper->setSchema(name);
			}
			sqlFileContent.clear();
			getFileContentAnsii(pathData, sqlFileContent);

			// SendMessage(dlgInitialize->hWnd, BN_DISABLE, 0, 0);

			try{
				dbHelper->execRecoverData(sqlFileContent.c_str());
			}catch(exception &e){
				MessageBox(dlgInitialize->hWnd, s2ws(e.what()).c_str(), L"���ݿ����", MB_OK | MB_ICONERROR);
				return (INT_PTR)FALSE;
			}
			MessageBox(dlgInitialize->hWnd, L"�ָ����ݳɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			break;
		// ɾ�����ݿ�
		case IDC_BUTTON_INITDROPDB:
			if (MessageBox(dlgInitialize->hWnd, L"ȷ��������", L"��ʾ", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			{
				TCHAR name[128] = {0};
				dlgInitialize->getComboSelString(IDC_COMBO_INITDB, name);
				if (name == NULL)
				{
					MessageBox(dlgInitialize->hWnd, L"��û��ѡ�����ݿ�", L"����", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}
				dbHelper->execDropDb(name);
				MessageBox(dlgInitialize->hWnd, L"�������", L"", MB_OK | MB_ICONINFORMATION);
			}
			break;
		// �������
		case IDC_BUTTON_INITCLEARDATA:
			if (MessageBox(dlgInitialize->hWnd, L"ȷ��������", L"��ʾ", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				break;
			}
			{	
				TCHAR name[128] = {0};
				dlgInitialize->getComboSelString(IDC_COMBO_INITDB, name);
				if (name == NULL)
				{
					MessageBox(dlgInitialize->hWnd, L"��û��ѡ�����ݿ�", L"����", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}
				try{
					dbHelper->execClearData(name);
				}catch(exception &e){
					MessageBox(dlgInitialize->hWnd, s2ws(e.what()).c_str(), L"���ݿ����", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}
				MessageBox(dlgInitialize->hWnd, L"������ݳɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			}
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
