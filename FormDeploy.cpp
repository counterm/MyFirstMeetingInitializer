#include "stdafx.h"
#include "MeetingSystemSetting.h"
#include "MySqlHelper.h"
#include "FormDeploy.h"
#include "sysmets.h"
#include <WindowsX.h>
#include <commctrl.h>

//#pragma comment(lib, "comctl32.lib")
extern HWND hWndMain;
extern FormDeploy		*dlgDeploy;
extern DBHELPER *dbHelper;

LRESULT CALLBACK DeployTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK FormDeployWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK HwndWndProc0(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HwndWndProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HwndWndProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HwndWndProc3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HwndWndProc4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HwndWndProc5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HwndWndProc6(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



FormDeploy::FormDeploy(void):curTabIndex(0)
{
	this->sm = new SettingManager();
	this->proc = FormDeployWndProc;
	for(int i=0; i < 7; i++)
		hwndChild[i] = NULL;
}


FormDeploy::~FormDeploy(void)
{
	delete this->sm;
}

HWND
FormDeploy::Create(HINSTANCE hInst, int i, HWND hWnd)
{
	return FormMysqlBase::Create(hInst, i, hWnd);
}
HWND
FormDeploy::Create(HINSTANCE hInst, HWND hWndMain)
{
	RECT rc = {};
	GetClientRect(hWndMain,&rc);

   //WNDCLASSEX wc; 
   //memset(&wc,0,sizeof(wc)); 
   //wc.hInstance = hInst; 
   //wc.lpszClassName = WC_TABCONTROL; 
   //wc.cbSize = sizeof(WNDCLASSEX); 
   //wc.style = CS_HREDRAW | CS_VREDRAW; 
   //wc.lpfnWndProc = DeployTabWndProc; 
   //wc.cbClsExtra = 0; 
   //wc.cbWndExtra = 0; 
   //wc.hIcon = NULL; 
   //wc.hCursor = LoadCursor(NULL,IDC_ARROW); 
   //wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
   //wc.lpszMenuName = NULL; 
   //wc.hIconSm = NULL;
   //RegisterClassEx(&wc);


	HWND hWnd2 = CreateWindowEx(NULL, 
		WC_TABCONTROL, L"tab1",  WS_VISIBLE | WS_CHILD, 0, 0, 
		rc.right, rc.bottom, hWndMain, NULL, hInst, NULL);

	if (hWnd2 == NULL)
	{
		logStr(L"创建失败");
		return 0;
	}



	TCHAR Name[7][9] = { L"系统", L"客户端", L"目录", L"二维码", L"铭牌", L"移动安全", L"高级" };
	
	TCITEM ti;
	memset(&ti, 0, sizeof(ti));
	ti.cchTextMax = 9;

	ti.mask = TCIF_TEXT;// | TCIF_IMAGE;
	//ti.iImage = -1;
	int t = sizeof(Name) / sizeof(Name[0]);
	HDC hdc = GetDC(hWnd2);
	for (int i = 0; i < t; i++)
	{
		ti.pszText = Name[i];
		TabCtrl_InsertItem(hWnd2, i, &ti);
		//TextOut(hdc,
		//	lpdis->rcItem.left,
		//	lpdis->rcItem.top,
		//	ti.pszText,
		//	lstrlen(ti.pszText));
		//SendMessage(hWnd2,TCM_INSERTITEM,(WPARAM)0,(LPARAM) (LPTCITEM)&ti);
	}
	DeleteDC(hdc);
	TabCtrl_SetCurSel(hWnd2, 0);
	// ShowWindow(hWnd2, SW_SHOW);
    // UpdateWindow(hWnd2);
	this->hWnd = hWnd2;
	this->initChildDialog();
	return this->hWnd;
}

//   创建所有下级对话框
void
FormDeploy::initChildDialog()
{
	//const int numbers = 7;
	//const TCHAR *idd = L"IDD_TAB_PAGE";
	//TCHAR *name = (TCHAR*)calloc(128, sizeof(TCHAR));

	HINSTANCE hInst = GetModuleHandle(NULL);
	RECT rc = {};
	GetClientRect( this->hWnd, &rc);

	//wsprintf(name, L"%s%d", idd, 0);
	hwndChild[0] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE1), hWnd, HwndWndProc0);
	hwndChild[1] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE2), hWnd, HwndWndProc1);
	hwndChild[2] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE3), hWnd, HwndWndProc2);
	hwndChild[3] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE4), hWnd, HwndWndProc3);
	hwndChild[4] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE5), hWnd, HwndWndProc4);
	hwndChild[5] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE6), hWnd, HwndWndProc5);
	hwndChild[6] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TAB_PAGE7), hWnd, HwndWndProc6);

	for (int i = 0 ; i < 7; i++){
		if (NULL == hwndChild[i]){
			logStr(L"生成子窗口失败。");
			break;
		}
	}

	showTab(0);

	//
	this->sm->init(hwndChild);
	this->sm->fileGet(dbHelper->getPath(L"fileSystem.properties"));
	//SettingItem *si = this->sm->findItem(L"backupPath");
	//si->toString();
	this->sm->dataSet(TRUE);
}

void
FormDeploy::show()
{
	ShowWindow(this->hWnd, SW_SHOW);
}

void 
FormDeploy::showTab(int tabIndex)
{
	ShowWindow(hwndChild[curTabIndex], SW_HIDE);
	ShowWindow(hwndChild[tabIndex], SW_SHOW);
	curTabIndex = tabIndex;
	SetWindowPos(hwndChild[tabIndex], HWND_TOP, 1, 25, 0, 0, SWP_NOSIZE);
}

HWND
FormDeploy::getTabHwnd()
{
	return FindWindow(WC_TABCONTROL,  L"tab1");
}

void 
FormDeploy::saveConfigFile()
{
	this->sm->dataSet(FALSE);
	this->sm->fileSet();
	MessageBox(NULL, L"保存完成。", L"提示", MB_OK | MB_ICONINFORMATION);
}

INT_PTR CALLBACK FormDeployWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	//HANDLE_MSG(hDlg, WM_INITDIALOG, SD_OnInitDialog);
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		{
			//TCHAR path[512] = {0};
			//lstrcpy(path, dbHelper->getPath(L""));
			//dlgDeploy->sm->fileGet(path);
			//dlgDeploy->sm->init(dlgDeploy->hwndChild);

			//HWND hWnd = GetDlgItem(hDlg, IDC_TAB_DEPLOY);
			//if (hWnd == NULL)
			//	MessageBox(hDlg, L"创建Tab失败", L"", MB_OK);
			//WCHAR Name[7][9] = { L"系统", L"客户端", L"目录", L"二维码", L"铭牌", L"移动安全", L"高级" };
			//TCITEM ti;
			//memset(&ti, 0, sizeof(ti));
			//ti.cchTextMax = 9;
			//ti.mask = TCIF_TEXT;// | TCIF_IMAGE;
			////ti.iImage = -1;
			//int t = sizeof(Name) / sizeof(Name[0]);
			//for (int i = 0; i < t; i++)
			//{
			//	ti.pszText = Name[i];
			//	TabCtrl_InsertItem(hWnd, i, &ti);
			//	//SendMessage(hWnd2,TCM_INSERTITEM,(WPARAM)0,(LPARAM) (LPTCITEM)&ti);
			//}
		}
		return TRUE;
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		//switch (wmId)
		//{
		//default:
		//	break;
		//}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK DeployTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		//switch (wmId)
		//{
		//default:
		//	break;
		//}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK HwndWndProc0(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE1:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK HwndWndProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE2:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK HwndWndProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE3:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK HwndWndProc3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE4:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK HwndWndProc4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE5:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK HwndWndProc5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE6:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK HwndWndProc6(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_NCCREATE:
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_DE7:
			dlgDeploy->saveConfigFile();
			break;
		default:
			break;
		}
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

//
//INT_PTR CALLBACK FormDeployTabWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_CREATE:
//		break;
//	case WM_COMMAND:
//		int wmId, wmEvent;
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//
//		//switch (wmId)
//		//{
//		//default:
//		//	break;
//		//}
//	default:
//		break;
//	}
//	return (INT_PTR)FALSE;
//}