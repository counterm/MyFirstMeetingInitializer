#include "stdafx.h"
#include "SqlHelperBase.h"


SqlHelperBase::SqlHelperBase()
{
}


SqlHelperBase::~SqlHelperBase()
{
	
}

// 添加连接时的调用函数
void SqlHelperBase::setConnectCalling(void(*fn)())
{
	this->fn = fn;
}

// 调用连接时的调用函数
void SqlHelperBase::callConnectCalling()
{
	this->fn();
}



// 返回配置文件 路径及文件名
const TCHAR* SqlHelperBase::getPath()
{
	return getPath(NULL);
	//TCHAR tPath[MAX_PATH] = {0};
	//GetModuleFileName( NULL, tPath, MAX_PATH );

	//// 去掉文件夹名
	//const TCHAR antiSlash ='\\';
	//wstring strArr(tPath);
	//int index = strArr.rfind(antiSlash);
	//wstring strFind = strArr.substr(0, index);

	//TCHAR* strRet = (TCHAR*)calloc(256, sizeof(TCHAR));
	//lstrcpy(strRet, strFind.c_str());

	//return strRet;
}

// 函数为需要追加的文件地址
const TCHAR* SqlHelperBase::getPath(const TCHAR* fileName)
{
	TCHAR tPath[MAX_PATH] = {0};
	GetModuleFileName( NULL, tPath, MAX_PATH );

	// 去掉文件夹名
	const TCHAR antiSlash ='\\';
	wstring strArr(tPath);
	int index = strArr.rfind(antiSlash);
	wstring strFind = strArr.substr(0, index);
	
	if (fileName != NULL)
	{
		strFind.append(L"\\");
		strFind.append(fileName);
	}

	TCHAR* strRet = (TCHAR*)calloc(256, sizeof(TCHAR));
	lstrcpy(strRet, strFind.c_str());

	return strRet;
}