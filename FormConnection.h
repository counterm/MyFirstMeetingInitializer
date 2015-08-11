#pragma once
#include "FormMysqlBase.h"
#include "MySqlHelper.h"

extern HWND hWndMain;
extern MySqlHelper *dbHelper;							// 数据库操作对象

class FormConnection : public FormMysqlBase
{
private:
	TCHAR *cfgFile;
	TCHAR *cfgPath;
	//TCHAR data[5][100];
	const TCHAR *getConfigPath();
public:
	TCHAR **data;
	HWND hwndComboDb;
	FormConnection();
	INT_PTR CALLBACK wndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void getConfigData();
	TCHAR* getAllData();
	void setAllData();
	bool saveSetting();
	const TCHAR* getData();
	const TCHAR* getIp();
	const TCHAR* getPort();
	const TCHAR* getName();
	const TCHAR* getPwd();
	const TCHAR* getScheme();
	const TCHAR* getCbScheme();
	//void initCombobox(MySqlHelper *);
	void connectCalling();
};