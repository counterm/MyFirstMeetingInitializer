#include "stdafx.h"
#include "ProjUtils.h"
#include <fstream>

//注意：当字符串为空时，也会返回一个空字符串
void split(std::wstring& s, std::wstring& delim,std::vector< std::wstring >* ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=std::wstring::npos)
	{
		ret->push_back(s.substr(last,index-last));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (index-last>0)
	{
		ret->push_back(s.substr(last,index-last));
	}
}

//UTF-8:  EF BB BF  
//UTF-16: FF FE  
//UTF-16 Big-endian: FE FF  
//UTF-32 Big-endian: FF FE 00 00  
//UTF-32 Little-endian: 00 00 FE FF  

// ansi 文件内容读取
void getFileContentAnsii(const TCHAR *path, wstring &ret)
{
		wstring lineString;
		wifstream sqlFile;
		register TCHAR buffer[2048] = {0};
		sqlFile.imbue(std::locale("chs"));
		sqlFile.open(path);
		if (sqlFile.is_open())
		{
			while(!sqlFile.eof())
			{
				getline(sqlFile, lineString);
				ret.append(lineString);
			}
		}
		sqlFile.close();
}