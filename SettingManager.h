#pragma once

/*
 * �����ؼ����Ա���
 * type������
 *	��������ؼ����͵�
 *  0 �����ı���
 *  1 ����������
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
	// state == 2���ؼ���ʱ������Ҫ�ر����ã� 
	// state == 4�����ڶ�ȡ��ʱ����Ҫ�ر�����
	// state == 8, ���ڱ����ʱ����Ҫ�ر�����
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
	TCHAR itemsTitle[50][SETTING_LENGTH];	// ����
	SettingItem* items[50];		// �ؼ������
public:
	SettingManager(void);
	~SettingManager(void);
	void init(HWND[]);
	SettingItem* SettingManager::findItem(const TCHAR*);
	void dataSet(bool toControl);
	void fileSet();
	void fileGet(const TCHAR* path);
};

