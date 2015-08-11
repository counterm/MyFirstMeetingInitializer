// MeetingSystemSetting.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MeetingSystemSetting.h"
#include "CovertString.h"
#include <CommCtrl.h>
#include <WindowsX.h>

#include "MySqlHelper.h"
#include "FormConnection.h"
#include "FormInitialize.h"
#include "FormBackup.h"
#include "FormDeploy.h"
#include "SD_Scroll.h"

#define MAX_LOADSTRING 100
#define SET_WIN_WIDTH 600
#define SET_WIN_HEIGHT 515

//#pragma comment(linker, \
//  "\"/manifestdependency:type='Win32' "\
//  "name='Microsoft.Windows.Common-Controls' "\
//  "version='6.0.0.0' "\
//  "processorArchitecture='*' "\
//  "publicKeyToken='6595b64144ccf1df' "\
//  "language='*'\"")
//#pragma comment(lib, "ComCtl32.lib")


// ȫ�ֱ���:
HWND hWndMain;
HWND hwndCurrent;
HINSTANCE hInst;								// ��ǰʵ��
DBHELPER *dbHelper;		// ���ݿ��������
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������


TCHAR strLog[1000];		// debug string
HWND hWndDlgMain;								// ��ǰʹ�õĴ���

// ȫ�ֺ���
void connectCalling();
DWORD WINAPI initDbHelper(LPVOID lpparentet);

// ��Э����
FormConnection	*dlgConnection;
FormInitialize	*dlgInitialize;
FormBackup		*dlgBackup;
FormDeploy		*dlgDeploy;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	dbHelper = new DBHELPER;		// ���ݿ��������
	dlgConnection	= new FormConnection;
	dlgInitialize	= new FormInitialize;
	dlgBackup		= new FormBackup;
	dlgDeploy		= new FormDeploy;

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MEETINGSYSTEMSETTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	dlgDeploy->Create(hInst, hWndMain);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEETINGSYSTEMSETTING));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEETINGSYSTEMSETTING));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MEETINGSYSTEMSETTING);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   int style = (WS_OVERLAPPEDWINDOW) & ~WS_THICKFRAME;
   hWnd = CreateWindow(szWindowClass, szTitle, style,
      CW_USEDEFAULT, CW_USEDEFAULT, SET_WIN_WIDTH, SET_WIN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   hWndMain = hWnd;

   {
	   // ����
	   RECT rc = {};
	   int dlgWidth,dlgHeight;
		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);  

		GetWindowRect(hWnd, &rc);
		
		dlgWidth=rc.right-rc.left;
		dlgHeight=rc.bottom-rc.top;
   
		SetWindowPos(hWnd, NULL, (nScreenWidth - dlgWidth) / 2, (nScreenHeight - dlgHeight) / 2, 
			0, 0, SWP_NOSIZE);
   }
   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int		cxClient, cyClient;
	SCROLLINFO		si;
	int				iHorzPos, iVertPos;//ˮƽ������λ��	 ��ֱ������λ��
	RECT rc = {};

	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
    //HANDLE_MSG(hWnd, WM_INITDIALOG, SD_OnInitDialog);
    //HANDLE_MSG(hWnd, WM_SIZE, SD_OnSize);
    //HANDLE_MSG(hWnd, WM_HSCROLL, SD_OnHScroll);
    //HANDLE_MSG(hWnd, WM_VSCROLL, SD_OnVScroll);
	case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				{
					HWND hwndTab = dlgDeploy->getTabHwnd();
					//TCHAR tmp[128] = {0};
					//wsprintf(tmp, L"%x", &hwndTab);
					//logStr(tmp);
					int iPage = TabCtrl_GetCurSel(dlgDeploy->hWnd);
					dlgDeploy->showTab(iPage);
				}
			}
		break;
	case WM_COMMAND:
		int wmId, wmEvent;
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_32775:
			dlgDeploy->show();
			dlgBackup->hide();
			dlgInitialize->hide();
			dlgConnection->hide();
			hwndCurrent = dlgDeploy->hWnd;
			SendMessage(hWnd, WM_SIZE, MAKELONG(SET_WIN_WIDTH, SET_WIN_HEIGHT), 0);
			break;
		case ID_32772:
			dlgDeploy->hide();
			dlgBackup->hide();
			dlgInitialize->hide();
			dlgConnection->show();
			hwndCurrent = dlgConnection->hWnd;
			SendMessage(hWnd, WM_SIZE, MAKELONG(SET_WIN_WIDTH, SET_WIN_HEIGHT), 0);
			break;
		case ID_32773:
			dlgDeploy->hide();
			dlgBackup->hide();
			dlgConnection->hide();
			dlgInitialize->show();
			hwndCurrent = dlgInitialize->hWnd;
			SendMessage(hWnd, WM_SIZE, MAKELONG(SET_WIN_WIDTH, SET_WIN_HEIGHT), 0);
			//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, SET_WIN_WIDTH, SET_WIN_HEIGHT, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case ID_32774:
			dlgDeploy->hide();
			dlgInitialize->hide();
			dlgConnection->hide();
			dlgBackup->show();
			hwndCurrent = dlgBackup->hWnd;
			SendMessage(hWnd, WM_SIZE, MAKELONG(SET_WIN_WIDTH, SET_WIN_HEIGHT), 0);
			// SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, SET_WIN_WIDTH, SET_WIN_HEIGHT, SWP_NOMOVE);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		

		// ��ʼ���������ݹ�����
		//dlgDeploy->Create(hInst, IDD_FORMVIEW1, hWnd);
		dlgConnection->Create(hInst, IDD_FORMVIEW, hWnd);
		dlgConnection->setAllData();
		dlgInitialize->Create(hInst, IDD_FORMINITIALIZE, hWnd);
		dlgBackup->Create(hInst, IDD_FORMBACKUP, hWnd);

		dbHelper->setConnectCalling(connectCalling);

		// dlgDeploy->Create(hInst, hWndMain);


		//dlgDeploy->show();

		dlgConnection->getAllData();

		CreateThread(NULL,0,initDbHelper,NULL,0,NULL);  
;
		// dlgConnection->initCombobox(dbHelper);
		//{

		//	HWND h = CreateDialog(hInst, MAKEINTRESOURCE(IDD_FORMVIEW1), hWnd, About);
		//	if (h==NULL)
		//	{
		//		logStr(L"NO");
		//	}
		//	ShowWindow(h, SW_SHOWNA);
		//}
		break;
	//case WM_DRAWITEM:
	//	{
	//		LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lParam; // item drawing information
	//		HWND hTabCtrl = dlgDeploy->hWnd;
	//		HBRUSH hbr;
	//		TCITEM tci;
	//		TCHAR *szTabText = (TCHAR*)calloc(128, sizeof(TCHAR));
	//		if (hTabCtrl == lpdis->hwndItem)   // is this the tab control?
	//		{
	//			//memset(szTabText, '\0', sizeof(szTabText));
	//			tci.mask = TCIF_TEXT;
	//			tci.pszText = szTabText;
	//			tci.cchTextMax = sizeof(szTabText)-1;

	//			TabCtrl_GetItem(hTabCtrl, lpdis->itemID, &tci);

	//			FillRect(lpdis->hDC, &lpdis->rcItem, (HBRUSH)(COLOR_WINDOW+1));
	//			SetBkColor(lpdis->hDC, COLOR_WINDOW+1);

	//			TextOut(lpdis->hDC,
	//				lpdis->rcItem.left,
	//				lpdis->rcItem.top,
	//				tci.pszText,
	//				lstrlen(tci.pszText));
	//		}
	//	}
	//	break;
	case WM_DESTROY: 
		//delete dbHelper;
		dbHelper->closeConnection();
		delete dlgConnection;
		delete dlgInitialize;
		delete dlgBackup;
		delete dlgDeploy;
		PostQuitMessage(0);
		break;
	//case WM_SIZE:
		//cxClient = LOWORD(lParam);
		//cyClient = HIWORD(lParam);
		//if (hwndCurrent == dlgDeploy->hWnd)
		//{
		//	GetClientRect(hwndCurrent,&rc);
		//}else{
		//	rc.left = 0;
		//	rc.top = 0;
		//	rc.right = 10;
		//	rc.bottom = 10;
		//}
		////�趨��ֱ��������Χ��ҳ���С
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_RANGE | SIF_PAGE;
		//si.nMin		= 0;
		//si.nMax		= rc.bottom;
		//si.nPage	= cyClient;
		//SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		////�趨ˮƽ��������Χ��ҳ���С
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_RANGE | SIF_PAGE;
		//si.nMin		= 0;
		//si.nMax		= rc.right;
		//si.nPage	= cxClient;
		//SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		//return 0;
////����ֱ��������Ϣ
	//case WM_VSCROLL:
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_ALL;
		//GetScrollInfo(hWnd, SB_VERT, &si);

		//iVertPos = si.nPos;

		//switch(LOWORD(wParam))
		//{
  //        case SB_TOP:	//�ö�(�Ȱ���Shift�����ţ�Ȼ���������������ϲ���������ö�)
  //             si.nPos = si.nMin ;
  //             break ;
  //             
  //        case SB_BOTTOM://�õ�(ͬ�ö�)
  //             si.nPos = si.nMax ;
  //             break ;

  //        case SB_LINEUP:
  //             si.nPos -= 10 ;//ÿһ�й���10������
  //             break ;
  //   
  //        case SB_LINEDOWN:
  //             si.nPos += 10 ;
  //             break ;
  //   
  //        case SB_PAGEUP://��ҳ����һ���ͻ�����С
  //             si.nPos -= cyClient ;
  //             break ;
  //   
  //        case SB_PAGEDOWN:
  //             si.nPos += cyClient ;
  //             break ;
  //   
  //        case SB_THUMBTRACK:
  //             si.nPos = HIWORD (wParam) ;
  //             break ;
  //   
  //        default :
  //             break ;
		//}

		//si.fMask	= SIF_POS;
		//SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		//GetScrollInfo(hWnd, SB_VERT, &si);

		//if(iVertPos != si.nPos)
		//{
		//	//����InvalidateRect��ScrollWindow��Ч����ͬ
		//	//InvalidateRect(hWnd, NULL, FALSE);
		//	ScrollWindow(hWnd, 0, iVertPos - si.nPos, NULL, NULL);
		//}
		//return 0;

	////����ˮƽ������(�ʹ�ֱ������)
	//case WM_HSCROLL:
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_ALL;
		//GetScrollInfo(hWnd, SB_HORZ, &si);

		//iHorzPos = si.nPos;

		//switch(LOWORD(wParam))
		//{
  //        case SB_LINELEFT:
  //             si.nPos -= 10 ;
  //             break ;
  //   
  //        case SB_LINERIGHT:
  //             si.nPos += 10 ;
  //             break ;
  //   
  //        case SB_PAGELEFT:
  //             si.nPos -= cxClient ;
  //             break ;
  //   
  //        case SB_PAGERIGHT:
  //             si.nPos += cxClient ;
  //             break ;
  //   
  //        case SB_THUMBTRACK:
  //             si.nPos = HIWORD (wParam) ;
  //             break ;
  //   
  //        default :
  //             break ;
		//}

		//si.fMask	= SIF_POS;
		//SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		//GetScrollInfo(hWnd, SB_HORZ, &si);

		//if(iHorzPos != si.nPos)
		//{
		//	//InvalidateRect(hWnd, NULL, FALSE);
		//	ScrollWindow(hWnd, iHorzPos - si.nPos, 0, NULL, NULL);
		//}

		//return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// ����ʱҪ���õ���������
void connectCalling()
{
	dbHelper->initDatabases();
	dlgConnection->connectCalling();
	dlgBackup->connectCalling();
	dlgInitialize->connectCalling();
}





INT_PTR CALLBACK FormLoadingWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		break;
	}
	return (INT_PTR)FALSE;
}


void createLoadingDialog()
{
	HWND dlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_LOADING), hWndMain, FormLoadingWndProc);
	ShowWindow(dlg, SW_SHOWNA);
}

void destroyLoadingDialog()
{
	SendDlgItemMessage(hWndDlgMain, IDD_LOADING, WM_DESTROY, 0 , 0);
	//SendMessage(dlgLoading, WM_CLOSE, 0, 0);
}


void logStr(const TCHAR* content)
{
	MessageBox(hWndMain, content, L"", MB_OK);
}
void logStr(int i)
{
	TCHAR tmp[128] = {0};
	wsprintf(tmp, L"%d", i);
	logStr(tmp);
}

DWORD WINAPI  initDbHelper(LPVOID lpparentet)
{
	dbHelper->init(
		dlgConnection->getIp(),
		dlgConnection->getPort(),
		dlgConnection->getName(),
		dlgConnection->getPwd(),
		dlgConnection->getScheme()
	);
	dbHelper->connect();
	return 0;
}