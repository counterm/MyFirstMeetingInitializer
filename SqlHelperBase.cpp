#include "stdafx.h"
#include "SqlHelperBase.h"


SqlHelperBase::SqlHelperBase()
{
}


SqlHelperBase::~SqlHelperBase()
{
	
}

// �������ʱ�ĵ��ú���
void SqlHelperBase::setConnectCalling(void(*fn)())
{
	this->fn = fn;
}

// ��������ʱ�ĵ��ú���
void SqlHelperBase::callConnectCalling()
{
	this->fn();
}



// ���������ļ� ·�����ļ���
const TCHAR* SqlHelperBase::getPath()
{
	return getPath(NULL);
	//TCHAR tPath[MAX_PATH] = {0};
	//GetModuleFileName( NULL, tPath, MAX_PATH );

	//// ȥ���ļ�����
	//const TCHAR antiSlash ='\\';
	//wstring strArr(tPath);
	//int index = strArr.rfind(antiSlash);
	//wstring strFind = strArr.substr(0, index);

	//TCHAR* strRet = (TCHAR*)calloc(256, sizeof(TCHAR));
	//lstrcpy(strRet, strFind.c_str());

	//return strRet;
}

// ����Ϊ��Ҫ׷�ӵ��ļ���ַ
const TCHAR* SqlHelperBase::getPath(const TCHAR* fileName)
{
	TCHAR tPath[MAX_PATH] = {0};
	GetModuleFileName( NULL, tPath, MAX_PATH );

	// ȥ���ļ�����
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