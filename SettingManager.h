#pragma once

/*
 * 单个控件属性保存
 * type描述：
 *	是描述其控件类型的
 *  0 代表文本框
 *  1 代表下拉框
*/
class SettingItem
{
public:
	SettingItem();
	void toString();
	HWND hwndParnet;
	TCHAR titleName[128];
	int iddPage;
	int idcName;
	TCHAR name[128];
	TCHAR keyName[128];
	TCHAR var[128];
	int type;
	// state == 2，控件的时候则需要特别设置， 
	// state == 4，则在读取的时候需要特别设置
	// state == 8, 则在保存的时候需要特别设置
	unsigned char state;	
};



//typedef struct{
//	HWND hwndParnet;
//	int iddPage;
//	int idcName;
//	TCHAR name[128];
//	TCHAR keyName[128];
//	TCHAR var[128];
//	int type;
//} SettingItem;

#define SETTING_LENGTH 128
#define SETTING_FILE L"fileSystem.properties"

class SettingManager
{
private:
	TCHAR filePath[256];
	TCHAR itemsTitle[50][SETTING_LENGTH];	// 标题
	SettingItem* items[50];		// 控件相关量
public:
	SettingManager(void);
	~SettingManager(void);
	void init(HWND[]);
	SettingItem* SettingManager::findItem(const TCHAR*);
	void dataSet(bool toControl);
	void fileSet();
	void fileGet(const TCHAR* path);
};

