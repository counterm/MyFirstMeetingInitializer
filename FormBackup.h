#pragma once
#include "formmysqlbase.h"
class FormBackup :
	public FormMysqlBase
{
public:
	TCHAR valListBox[256];		// ѡ�е��б���ֵ
	FormBackup(void);
	~FormBackup(void);
	void connectCalling();
	HWND Create(HINSTANCE, int, HWND);
	void initListView();
	void initDbCombobox();
};

