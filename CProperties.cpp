#include "stdafx.h"
#include "CProperty.h"
#include "CProperties.h"
#include "CovertString.h"
#include <fstream>

CProperty * CProperties::getProperty(wstring name)
{
    void * pa = NULL;  
	map<wstring, void*>::iterator iter;
    //if(_map.Lookup(name.c_str(), pa))  
	iter = _map.find(name);
	if (iter != _map.end())
		pa = iter->second;
        return (CProperty *)pa;  
 
    return NULL;  
}  

CProperties::~CProperties(void)  
{  
    // POSITION pos;  
	map<wstring, void*>::reverse_iterator iter;
    wstring key;
	for(iter = _map.rbegin(); iter != _map.rend(); iter++)
	{
		delete iter->second;
	}
    //for (pos = _map.GetStartPosition(); pos != NULL;)  
    //{  
    //   _map.GetNextAssoc(pos, key, pa);  
    //   delete pa;  
    //}  
}  
 
int CProperties::loadFrom(wstring path, const TCHAR* characterSet)
{
	_path = path;
	ifstream in(path);
    int lines = 0;  
	char tmp[512] = {0};
    while(!in.eof())  
    {
        //读取其中的一行
		char line[512];  
        in.getline(line, 512);
		string s;
		if (characterSet[0] == 'U'){
			if (line[0] == 0xFFFFFFEF){
				sscanf(line, "%*3c%498c\n", tmp);
				strncpy(line, tmp, 512);
			}
			s = ws2s(utf8_2_ws(line));
		}else{
			s = line;
		}
 
        //空白行，跳过  
        if(s.empty())  
            continue;  
 
        //#为注释标记，跳过  
        if(s[0] == '#')
            continue;
 
        //不包含=，跳过
        int i = s.find("=");
        if(i < 0)
            continue;
 
        //拆分成key=value
        wstring key = s2ws(s.substr(0, i));
        wstring value = s2ws(s.substr(i + 1));
		_map.insert(pair<wstring, void*>(key, new CProperty(value)));
        //_map.SetAt(wstring(key.c_str()), new CProperty(wstring(value.c_str())));  
        lines++;
    }  
	in.close();
    return lines;  
}

void CProperties::setProperty(wstring key, wstring value)
{
	CProperty *cp = getProperty(key);
	if (cp == NULL)
		return;
	cp->setValue(value);
}

// 保存相应属性的值
void CProperties::saveProperty(const TCHAR* characterSet)
{
	ofstream out(_path);
	map<wstring, void*>::iterator iter;
	wstring tmp;
	CProperty *cp;
	for(iter = _map.begin(); iter != _map.end(); iter++)
	{
		cp = (CProperty*)iter->second;
		tmp.clear();
		tmp.append(iter->first);
		tmp.append(L"=");
		tmp.append(cp->getString());
		if (characterSet[0] == 'U'){
			out << ws2utf8(tmp);
		}else{
			out << ws2s(tmp);
		}
		out << '\n';
	}
	out.close();
}