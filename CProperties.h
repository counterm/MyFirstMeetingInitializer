#pragma once
#include <map> 
#include <algorithm> 
#include "CProperty.h"

class CProperties  
{  
private:  
	wstring _path;
    map<wstring, void*> _map;   
public:  
    CProperty * getProperty(wstring name);  
    ~CProperties(void);  
	int loadFrom(wstring, const TCHAR* characterSet=L"ANSI");
	void setProperty(wstring, wstring);
	void saveProperty(const TCHAR* characterSet=L"ANSI");
}; 