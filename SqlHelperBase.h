#pragma once
class SqlHelperBase
{
private:
	/*wstring path;
	int flagPath;*/
	void(*fn)();
public:
	SqlHelperBase();
	~SqlHelperBase();
	void setConnectCalling(void(*)());	// �������ʱ�ĵ��ú���
	void callConnectCalling();	// ��������ʱ�ĵ��ú���
	const TCHAR* getPath();
	const TCHAR* getPath(const TCHAR* fileName);
};

