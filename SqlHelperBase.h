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
	void setConnectCalling(void(*)());	// 添加连接时的调用函数
	void callConnectCalling();	// 调用连接时的调用函数
	const TCHAR* getPath();
	const TCHAR* getPath(const TCHAR* fileName);
};

