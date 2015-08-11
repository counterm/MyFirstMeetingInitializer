#include "stdafx.h"
#include "CProperty.h"
#include "CovertString.h"
CProperty::CProperty(wstring value)  
{  
    _value = value;  
}

void CProperty::setValue(wstring value)
{
	_value = value;
}
 
wstring CProperty::getString(void)  
{  
    return _value;  
}  
 
int CProperty::getInteger(void)  
{  
    return atoi(ws2s(_value).c_str());
}  
 
bool CProperty::getBoolean(void)  
{  
	// TCHAR tmp[256] = {0};
	TCHAR *tmp = (TCHAR*)calloc(256, sizeof(TCHAR));
	lstrcpy(tmp, _value.c_str());
	lstrcpy(tmp, (LPCWSTR)_tcslwr_s(tmp, 256));
	return wstring(L"true").compare(tmp) == 0;  
} 