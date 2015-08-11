


#pragma once
class FormMysqlBase
{
public:
	HWND hWnd;	
	DLGPROC proc;
	FormMysqlBase(void);
	~FormMysqlBase(void);
	HWND Create(HINSTANCE, int, HWND);
	void show();
	void connectCalling();
	void hide();
	void setComboSel(int, int);
	void initCombobox(int idc, wstring[], int);
	void setComboSel(int, const TCHAR*);
	void getComboSelString(int, TCHAR*);
	DWORD processWaitUntilDone(TCHAR* strcmdline, LPWSTR strcmdline2);
};

