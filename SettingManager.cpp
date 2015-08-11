#include "stdafx.h"
#include "MeetingSystemSetting.h"
#include "SettingManager.h"
#include "FormDeploy.h"
#include "CovertString.h"
#include <vector>
#include "CProperties.h"
#include "MySqlHelper.h"
#include <fstream>

extern DBHELPER *dbHelper;

SettingItem::SettingItem(){
	this->state = 0;
	lstrcpy(this->titleName, L"");
	this->iddPage = 0;
	this->idcName = 0;
	lstrcpy(this->name, L"");
	lstrcpy(this->keyName, L"");
	lstrcpy(this->var, L"");
}

void
SettingItem::toString()
{
	wstring tmp;
	tmp.append(L"titleName:");
	tmp.append(this->titleName);
	tmp.append(L"\n");
	tmp.append(L"name:");
	tmp.append(this->name);
	tmp.append(L"\n");
	tmp.append(L"keyName:");
	tmp.append(this->keyName);
	tmp.append(L"\n");
	tmp.append(L"var:");
	tmp.append(this->var);
	tmp.append(L"\n");
	OutputDebugString(tmp.c_str());
}

SettingManager::SettingManager(void)
{
	
	for(int i = 0, j = sizeof(itemsTitle)/sizeof(itemsTitle[0]); i < j ;i++){
		lstrcpy(itemsTitle[i], 0);
	}
	for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++){
		items[i] = NULL;
	}
}


SettingManager::~SettingManager(void)
{
	for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++){
		if (items[i] != NULL){
			memset(items[i], 0, sizeof(items[i]));
			delete items[i];
		}
		items[i] = NULL;
	}
}

void
SettingManager::init(HWND hWnd[])
{
	int len = sizeof(itemsTitle[0]);
	int i = 0, j = 0, k = 0;


	int iddPage[10] = {0};
	int idcPage[10][10] = {{0},{0}};
	TCHAR name[10][10][SETTING_LENGTH] = {{0},{0},{0}};
	TCHAR pageName[50][SETTING_LENGTH] = {{0},{0}};
	TCHAR keyNames[10][10][SETTING_LENGTH] = {{0},{0},{0}};

	// PAGE 1
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE1;
		lstrcpy(pageName[i], L"ϵͳ����");
		int IDC_PAGE0[] = {IDC_EDIT_SET1_1, IDC_EDIT_SET1_2, IDC_EDIT_SET1_3, IDC_EDIT_SET1_4, IDC_EDIT_SET1_5};

		lstrcpy(keyNames[i][j], L"syncService");j++;
		lstrcpy(keyNames[i][j], L"autoMeetingStart");j++;
		lstrcpy(keyNames[i][j], L"autoBackup");j++;
		lstrcpy(keyNames[i][j], L"autoBackupDateTime");j++;
		lstrcpy(keyNames[i][j], L"autoTempDelete");j++;

		j = 0;
		lstrcpy(name[i][j], L"�ͻ��˷����ַ");j++;
		lstrcpy(name[i][j], L"��̨��ʱ�Զ���ʼ����");j++;
		lstrcpy(name[i][j], L"�Զ��������ݿ�");j++;
		lstrcpy(name[i][j], L"�Զ�����ʱ������");j++;
		lstrcpy(name[i][j], L"�Զ������ʱ�ĵ�");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));
		i++;
	}

	// PAGE 2 �ͻ���
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE2;
		lstrcpy(pageName[i], L"�ͻ�������");
		int IDC_PAGE0[] = {0, IDC_EDIT_SET2_1, IDC_EDIT_SET2_2, IDC_EDIT_SET2_3};
		
		lstrcpy(keyNames[i][j], L"functionVersion");j++;
		lstrcpy(keyNames[i][j], L"androidFunctionVersion");j++;
		lstrcpy(keyNames[i][j], L"iosFunctionVersion");j++;
		lstrcpy(keyNames[i][j], L"iosid");j++;

		j = 0;
		lstrcpy(name[i][j], L"��������");j++;
		lstrcpy(name[i][j], L"android��������");j++;
		lstrcpy(name[i][j], L"iOSӦ�ñ��");j++;
		lstrcpy(name[i][j], L"iOS��������");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));
		i++;
	}

	// PAGE 3 Ŀ¼
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE3;
		lstrcpy(pageName[i], L"Ŀ¼����");
		int IDC_PAGE0[] = {IDC_EDIT_SET3_1, IDC_EDIT_SET3_2, IDC_EDIT_SET3_3,
			IDC_EDIT_SET3_4, IDC_EDIT_SET3_5, IDC_EDIT_SET3_6,IDC_EDIT_SET3_7, 0};


		lstrcpy(keyNames[i][j],L"backupPath");j++;
		lstrcpy(keyNames[i][j],L"uploadPath");j++;
		lstrcpy(keyNames[i][j],L"uploadAlbum");j++;
		lstrcpy(keyNames[i][j],L"videoPath");j++;
		lstrcpy(keyNames[i][j],L"meetingPath");j++;
		lstrcpy(keyNames[i][j],L"tempPath");j++;
		lstrcpy(keyNames[i][j],L"nameplateStylePath");j++;
		lstrcpy(keyNames[i][j],L"uploadSpaceLimit");j++;

		j = 0;
		lstrcpy(name[i][j],L"����Ŀ¼");j++;
		lstrcpy(name[i][j],L"��������Ŀ¼");j++;
		lstrcpy(name[i][j],L"���Ŀ¼");j++;
		lstrcpy(name[i][j],L"��ƵĿ¼");j++;
		lstrcpy(name[i][j],L"������������");j++;
		lstrcpy(name[i][j],L"��ʱ·��");j++;
		lstrcpy(name[i][j],L"������ʽͼƬ");j++;
		lstrcpy(name[i][j],L"���������С");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));
		i++;
	}

	// PAGE 4 ��ά��
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE4;
		lstrcpy(pageName[i], L"��ά������");
		int IDC_PAGE0[] = {IDC_EDIT_SET4_1, IDC_EDIT_SET4_2, IDC_EDIT_SET4_3};


		lstrcpy(keyNames[i][j], L"qrcodeExpire");j++;
		lstrcpy(keyNames[i][j], L"loginAutoClassAExpire");j++;
		lstrcpy(keyNames[i][j], L"qrcodeImageSize");j++;
		

		j = 0;
		lstrcpy(name[i][j], L"�����ά�볬ʱ(��)");j++;
		lstrcpy(name[i][j], L"�����Ա��¼��ʱ(��)");j++;
		lstrcpy(name[i][j], L"��ά���С");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));

		i++;
	}

	// PAGE 5 ����
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE5;
		lstrcpy(pageName[i], L"��������");
		int IDC_PAGE0[] = {IDC_EDIT_SET5_1, IDC_EDIT_SET5_2, IDC_EDIT_SET5_3,
			IDC_EDIT_SET5_4, IDC_EDIT_SET5_5, IDC_EDIT_SET5_6};

		lstrcpy(keyNames[i][j],L"deviceStatusExpire");j++;
		lstrcpy(keyNames[i][j],L"fontPath");j++;
		lstrcpy(keyNames[i][j],L"fontName");j++;
		lstrcpy(keyNames[i][j],L"fontFile");j++;
		lstrcpy(keyNames[i][j],L"screenStyle0");j++;
		lstrcpy(keyNames[i][j],L"screenStyle1");j++;

		j = 0;
		lstrcpy(name[i][j],L"���ӳ�ʱ(��)");j++;
		lstrcpy(name[i][j],L"����Ŀ¼");j++;
		lstrcpy(name[i][j],L"������ʾ��(���Ÿ���)");j++;
		lstrcpy(name[i][j],L"�����ļ���(���Ÿ���)");j++;
		lstrcpy(name[i][j],L"������ɫ");j++;
		lstrcpy(name[i][j],L"������ɫ");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));

		i++;
	}

	// PAGE 6 �ƶ���ȫ
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE6;
		lstrcpy(pageName[i], L"��������");
		int IDC_PAGE0[] = {IDC_EDIT_SET6_1, IDC_EDIT_SET6_2};

		lstrcpy(keyNames[i][j],L"gatewayPath");j++;
		lstrcpy(keyNames[i][j],L"appID");j++;

		j = 0;
		lstrcpy(name[i][j],L"���ص�ַ");j++;
		lstrcpy(name[i][j],L"Ӧ��ϵͳ���");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));
		i++;
	}

	// PAGE 7 �߼�
	{
		j = 0;
		iddPage[i] = IDD_TAB_PAGE7;
		lstrcpy(pageName[i], L"�߼�����");
		int IDC_PAGE0[] = {IDC_EDIT_SET7_1, IDC_EDIT_SET7_2, IDC_EDIT_SET7_3,
			IDC_EDIT_SET7_4, IDC_EDIT_SET7_5, IDC_EDIT_SET7_6};

		lstrcpy(keyNames[i][j],L"outputImageSize");j++;
		lstrcpy(keyNames[i][j],L"outputImageQuality");j++;
		lstrcpy(keyNames[i][j],L"md5Password");j++;
		lstrcpy(keyNames[i][j],L"remoteAOPSwitch");j++;
		lstrcpy(keyNames[i][j],L"devicePermission");j++;
		lstrcpy(keyNames[i][j],L"login");j++;

		j = 0;
		lstrcpy(name[i][j],L"��������ͼƬ���");j++;
		lstrcpy(name[i][j],L"��������ͼƬ����(0-100)");j++;
		lstrcpy(name[i][j],L"ʹ�����ı�������(0/1)");j++;
		lstrcpy(name[i][j],L"ʹ�ýӿ���֤(0/1)");j++;
		lstrcpy(name[i][j],L"ʹ������Ȩ��(true/false)");j++;
		lstrcpy(name[i][j],L"ʹ�õ�¼��֤(true/false)");j++;

		memcpy(idcPage[i], IDC_PAGE0, sizeof(IDC_PAGE0));
		i++;
	}

	memcpy(itemsTitle, pageName, 10*SETTING_LENGTH*sizeof(TCHAR));

	//int iddPage[10] = {0};
	//int idcPage[10][10] = {{0},{0}};
	//TCHAR name[10][10][128] = {{0},{0},{0}};
	//TCHAR pageName[10][128] = {{0},{0}};

	TCHAR tmpStr[128] = {0};
	i = 0;
	//SettingItem *tmp;
	while(iddPage[i] != 0){
		
		j = 0;
		while (keyNames[i][j][0] != 0){
			
			items[k] = new SettingItem();//{hWnd, iddPage[i], idcPage[i][j], {0}, {0}, 0};
			//tmp = items[k];
			items[k]->titleName[0] = 0;
			if (j==0){
				lstrcpy(items[k]->titleName, pageName[i]);
			}
			items[k]->hwndParnet = hWnd[i];
			items[k]->iddPage = iddPage[i];
			items[k]->idcName = idcPage[i][j];    
			items[k]->type = 0;
			items[k]->var[0] = 0;
			lstrcpy(items[k]->keyName, keyNames[i][j]);
			//lstrcpy(tmp.name, name[i][j]);	// ����������
			lstrcpy(items[k]->name, name[i][j]);
			// OutputDebugString(items[k]->name);
			//items[k] = tmp;	// д������
			k++;
			j++;
		}
		i++;
	}
	items[10]->toString();
	//logStr(items[10]->iddPage);
	SettingItem* si = this->findItem(L"uploadSpaceLimit");
	si->state = 2;
	si = this->findItem(L"screenStyle0");
	si->state = 12;
	si = this->findItem(L"screenStyle1");
	si->state = 12;
	si = this->findItem(L"functionVersion");
	si->state = 2;
}

SettingItem* SettingManager::findItem(const TCHAR* keyName)
{
	for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++)
	{
		if (items[i] == NULL){
			break;
		}
		if (lstrcmp(items[i]->keyName, keyName) == 0)
		{
			return items[i];
		}
	}
	return NULL;
}

// ����ֵ��ؼ�֮��Ĵ��ݣ�toControl=TRUE������ֵ���ؼ�
void SettingManager::dataSet(bool toControl)
{

	if (toControl == TRUE){
		for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++)
		{
			if (items[i] == NULL){
				break;
			}
			if (items[i]->type == 0 && (items[i]->state & 2) == 0 && items[i]->idcName != 0){
				SetDlgItemText(items[i]->hwndParnet, items[i]->idcName, items[i]->var);
			}
		}
	}else{
		TCHAR *tmp = (TCHAR*)calloc(SETTING_LENGTH, sizeof(TCHAR));
		for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++)
		{
			if (items[i] == NULL){
				break;
			}
			if (items[i]->type == 0 && (items[i]->state & 2) == 0 && items[i]->idcName != 0){
				GetDlgItemText(items[i]->hwndParnet, items[i]->idcName, tmp, 128);
				lstrcpy(items[i]->var, tmp);
			}
		}
	}
}

// ���������ļ�
void SettingManager::fileSet()
{
	ofstream outStream;
	outStream.open(this->filePath);
	wstring tmp;
	for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++)
	{
		if (items[i] == NULL){
			break;
		}
		tmp.clear();
		if (items[i]->titleName[0] != 0){
			tmp.append(L"\n\n#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n# ");
			tmp.append(items[i]->titleName);
			tmp.append(L"\n#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
		}
		if ((items[i]->state & 8) == 0){
			tmp.append(L"# ");
			tmp.append(items[i]->name);
			tmp.append(L"\n");
			tmp.append(items[i]->keyName);
			tmp.append(L"=");
			tmp.append(items[i]->var);	
		}else{
			if (lstrcmp(items[i]->keyName, L"screenStyle0") == 0){
				{
					SettingItem *itm = this->findItem(L"screenStyle1");
					tmp.append(L"# ");
					tmp.append(items[i]->name);
					tmp.append(L", ");
					tmp.append(itm->name);
					tmp.append(L"\n");
					tmp.append(L"screenStyle=");
					tmp.append(items[i]->var);
					tmp.append(L",");
					tmp.append(itm->var);
				}
			}
		}
		outStream << ws2utf8(tmp);
		outStream << '\n';
	}
	outStream.close();
}

// ��ȡ�����ļ�
void SettingManager::fileGet(const TCHAR* path)
{
	lstrcpy(this->filePath, path);
	CProperties *cps = new CProperties();
	CProperty *cp = NULL;
	wstring tmp;
	cps->loadFrom(dbHelper->getPath(SETTING_FILE), L"UTF8");
	for(int i = 0, j = sizeof(items)/sizeof(items[0]); i < j ;i++){
		if (items[i] == NULL){
			break;
		}
		if ((items[i]->state & 4) == 0){
			cp = cps->getProperty(this->items[i]->keyName);
			//if (cp == NULL){
			//	logStr(this->items[i]->keyName);
			//}
			tmp.clear();
			tmp = cp->getString();
			lstrcpy(this->items[i]->var,tmp.c_str());
		}else{
			// ��Ļ�ϲ���ȡ
			if (lstrcmp(items[i]->keyName, L"screenStyle0") == 0){
				cp = cps->getProperty(L"screenStyle");
				tmp.clear();
				tmp.append(cp->getString());
				{
					int index = tmp.find(L",");
					lstrcpy(this->items[i]->var, tmp.substr(0, index).c_str());
					SettingItem *itm = this->findItem(L"screenStyle1");
					if (itm != NULL){
						lstrcpy(itm->var,tmp.substr(index+1).c_str());
					}
				}
			}
		}
		if (cp != NULL){
			memset(cp, 0, sizeof(cp));
			cp = NULL;
		}
	}
	cp = NULL;
	delete cps;

	//SettingItem* si = this->findItem(L"functionVersion");
	//si->toString();
}
