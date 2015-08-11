#pragma once
#include "formmysqlbase.h"
#include "SettingManager.h"

class FormDeploy:
	public FormMysqlBase
{
public:
	SettingManager *sm;
	HWND hwndChild[7];

private:
	int curTabIndex;
	
public:
	
	FormDeploy(void);
	~FormDeploy(void);
	HWND Create(HINSTANCE, int, HWND);
	HWND Create(HINSTANCE, HWND);
	void initChildDialog();	//   创建所有下级对话框
	void show();
	void showTab(int);
	HWND getTabHwnd();
	void saveConfigFile();
};

