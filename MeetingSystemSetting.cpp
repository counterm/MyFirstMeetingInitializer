// MeetingSystemSetting.cpp : 定义应用程序的入口点。
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


// 全局变量:
HWND hWndMain;
HWND hwndCurrent;
HINSTANCE hInst;								// 当前实例
DBHELPER *dbHelper;		// 数据库操作对象
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名


TCHAR strLog[1000];		// debug string
HWND hWndDlgMain;								// 当前使用的窗口

// 全局函数
void connectCalling();
DWORD WINAPI initDbHelper(LPVOID lpparentet);

// 表单协助类
FormConnection	*dlgConnection;
FormInitialize	*dlgInitialize;
FormBackup		*dlgBackup;
FormDeploy		*dlgDeploy;

// 此代码模块中包含的函数的前向声明:
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

	dbHelper = new DBHELPER;		// 数据库操作对象
	dlgConnection	= new FormConnection;
	dlgInitialize	= new FormInitialize;
	dlgBackup		= new FormBackup;
	dlgDeploy		= new FormDeploy;

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MEETINGSYSTEMSETTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	dlgDeploy->Create(hInst, hWndMain);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEETINGSYSTEMSETTING));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

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
	   // 居中
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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int		cxClient, cyClient;
	SCROLLINFO		si;
	int				iHorzPos, iVertPos;//水平滚动条位置	 垂直滚动条位置
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
		// 分析菜单选择:
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
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		

		// 初始化窗口数据管理类
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
		////设定垂直滚动条范围和页面大小
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_RANGE | SIF_PAGE;
		//si.nMin		= 0;
		//si.nMax		= rc.bottom;
		//si.nPage	= cyClient;
		//SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		////设定水平滚动条范围和页面大小
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_RANGE | SIF_PAGE;
		//si.nMin		= 0;
		//si.nMax		= rc.right;
		//si.nPage	= cxClient;
		//SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		//return 0;
////处理垂直滚动条消息
	//case WM_VSCROLL:
		//si.cbSize	= sizeof(si);
		//si.fMask	= SIF_ALL;
		//GetScrollInfo(hWnd, SB_VERT, &si);

		//iVertPos = si.nPos;

		//switch(LOWORD(wParam))
		//{
  //        case SB_TOP:	//置顶(先按下Shift键不放，然后点击滚动条方块上侧区域就能置顶)
  //             si.nPos = si.nMin ;
  //             break ;
  //             
  //        case SB_BOTTOM://置底(同置顶)
  //             si.nPos = si.nMax ;
  //             break ;

  //        case SB_LINEUP:
  //             si.nPos -= 10 ;//每一行滚动10个像素
  //             break ;
  //   
  //        case SB_LINEDOWN:
  //             si.nPos += 10 ;
  //             break ;
  //   
  //        case SB_PAGEUP://翻页就是一个客户区大小
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
		//	//这里InvalidateRect、ScrollWindow，效果相同
		//	//InvalidateRect(hWnd, NULL, FALSE);
		//	ScrollWindow(hWnd, 0, iVertPos - si.nPos, NULL, NULL);
		//}
		//return 0;

	////处理水平滚动条(和垂直的类似)
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

// “关于”框的消息处理程序。
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

// 连接时要调用的其它函数
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