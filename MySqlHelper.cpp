#include "stdafx.h"
#include "CovertString.h"
#include "MySqlHelper.h"
#include "FormConnection.h"

#define MYSQLDELIM L";"
#define DEFAULTTABLE "information_schema"
extern void getFileContentAnsii(wstring &path, wstring &ret);

MySqlHelper::MySqlHelper()
{
	bIsConnect = FALSE;
	//this->ip = (TCHAR*)calloc(256, sizeof(TCHAR));
	//this->port = (TCHAR*)calloc(256, sizeof(TCHAR));
	//this->name = (TCHAR*)calloc(256, sizeof(TCHAR));
	//this->pwd = (TCHAR*)calloc(256, sizeof(TCHAR));
	//this->scheme = (TCHAR*)calloc(256, sizeof(TCHAR));; 
}
MySqlHelper::~MySqlHelper()
{
	this->closeConnection();
}

void 
MySqlHelper::init(const TCHAR *aIp, const TCHAR *aPort, 
					   const TCHAR *aName, const TCHAR *aPwd, const TCHAR *aScheme)
{
	this->ip.clear();
	this->ip.append(aIp);
	this->port.clear();
	this->port.append(aPort);
	this->name.clear();
	this->name.append(aName);
	this->pwd.clear();
	this->pwd.append(aPwd);
	this->scheme.clear();
	this->scheme.append(aScheme);

	//this->ip = aIp;
	//this->port = aPort;
	//this->name = aName;
	//this->pwd = aPwd;
	//this->scheme = aScheme;
}

// 使用数据库
void 
MySqlHelper::useSchema()
{
	this->con->setSchema(ws2s(scheme));
}

bool
MySqlHelper::connect()
{
	if (this->bIsConnect)
	{
		this->closeConnection();
	}
	TCHAR connectString[256] = {0};
	wsprintf(connectString, L"tcp://%s:%s", ip.c_str(), port.c_str());
	driver = get_driver_instance();//mysql::get_mysql_driver_instance();
	try{
		wstring valDatabase;
		auto_ptr<Connection> con(driver->connect(ws2s(connectString), 
			ws2s(name.c_str()), ws2s(pwd.c_str())));
		bIsConnect = TRUE;
		this->con = con;

		this->initDatabases();
		
		if (scheme[0] == '\0')
		{
			scheme = this->databases[0];
		}

		this->con->setSchema(ws2s(scheme));
		auto_ptr<Statement> stmt2(this->con->createStatement());
		this->stmt = stmt2;
		//{
		//	this->resultset.reset(this->executeQuery(L"show databases;"));
		//	if (this->resultset->next()){
		//		MessageBox(hWndMain, s2ws(this->resultset->getString("database").c_str()).c_str(), L"", MB_OK);
		//	}else{
		//		MessageBox(hWndMain, L"没有", L"", MB_OK);
		//	}
		//}
	}catch(exception &e){
		wstring wException(L"尝试连接数据库发生错误：\r\n");
		wException.append(s2ws(e.what()));
		MessageBox(hWndMain, 
			wException.c_str(), L"错误", 
			MB_OK | MB_ICONERROR);
		return FALSE;
	}
	this->callConnectCalling();
	return TRUE;
}



void
MySqlHelper::initDatabases()
{
		// get databases
	this->con->setSchema(DEFAULTTABLE);
	auto_ptr<Statement> stmt(this->con->createStatement());
	ResultSet *rs = stmt->executeQuery("show databases");
	int stringIndex = 0;
	int arrLength = sizeof(this->databases) / sizeof(this->databases[0]);
	while(rs->next())
	{
		this->databases[stringIndex] = s2ws(rs->getString("database"));
		stringIndex++;
	}
	// zero
	while(stringIndex<arrLength)
	{
		this->databases[stringIndex] = L"\0";
		stringIndex++;
	}
}

void
MySqlHelper::closeConnection()
{
	if (!this->bIsConnect){
		return;
	}
	if (this->resultset && !this->resultset->isClosed())
	{
		this->resultset->close();
	}
	if (!this->con->isClosed())
	{
		this->con->close();
	}
	this->bIsConnect = FALSE;
}

bool
MySqlHelper::connectCheck()
{
	if (this->bIsConnect)
	{
		return TRUE;
	}else{
		MessageBox(hWndMain, L"请先连接数据库", L"错误", MB_OK | MB_ICONERROR);
		return FALSE;
	}
}

void
MySqlHelper::setSchema(const TCHAR* name)
{
	this->con->setSchema(ws2utf8(name));
}

void
MySqlHelper::_execute(const sql::SQLString &sql)
{
	// MessageBox(hWndMain, utf8_2_ws(sql.c_str()).c_str(), L"", MB_OK);
	this->stmt->execute(sql);
}

ResultSet*
MySqlHelper::_executeQuery(const sql::SQLString &sql)
{
	this->stmt->execute(sql);
	this->resultset = this->stmt->getResultSet();
	return this->resultset;
}

int
MySqlHelper::_executeUpdate(const sql::SQLString &sql)
{
	return stmt->executeUpdate(sql);
}


void
MySqlHelper::execute(const TCHAR *sql)
{
	if (!this->connectCheck())
	{
		return;
	}
	wstring sqlLoop;
	vector<wstring> vtSql;
	split(wstring(sql), wstring(MYSQLDELIM), &vtSql);
	if (vtSql.size() > 1)
	{
		for(size_t i = 0; i < vtSql.size() - 1 ; i++)
		{
			sqlLoop = vtSql.at(i);
			this->_execute(ws2utf8(sqlLoop));
		}
	}else{
		//this->_execute(ws2utf8(sql));
		SQLString sql2 = ws2utf8(sql);
		this->_execute(sql2);
	}
}

ResultSet*
MySqlHelper::executeQuery(const TCHAR *sql)
{
	if (!this->connectCheck())
	{
		return NULL;
	}

	wstring sqlLoop;
	vector<wstring> vtSql;
	split(wstring(sql), wstring(MYSQLDELIM), &vtSql);
	if (vtSql.size() > 1)
	{
		for(size_t i = 0; i < vtSql.size() - 1 ; i++)
		{
			sqlLoop = vtSql.at(i);
			this->_execute(ws2utf8(sqlLoop));
		}
	}else{
		this->_execute(ws2utf8(sql));
	}

	this->resultset = this->stmt->getResultSet();
	return this->resultset;
}

int
MySqlHelper::executeUpdate(const TCHAR *sql)
{
	if (!this->connectCheck())
	{
		return 0;
	}
	
	wstring sqlLoop;
	vector<wstring> vtSql;
	split(wstring(sql), wstring(MYSQLDELIM), &vtSql);
	if (vtSql.size() > 1)
	{
		for(size_t i = 0; i < vtSql.size() - 1 ; i++)
		{
			sqlLoop = vtSql.at(i);
			this->_executeUpdate(ws2utf8(sqlLoop));
		}
	}else{
		this->_executeUpdate(ws2utf8(sql));
	}
}



// 返回相应字段字符串值
TCHAR*
MySqlHelper::getRsString(const TCHAR *fieldName)
{
	wstring fieldString = s2ws(this->resultset->getString(ws2s(fieldName)).asStdString());
	return (TCHAR*)(fieldString.c_str());
}

// 返回相应字段整形值
int
MySqlHelper::getRsInt(const TCHAR *fieldName)
{
	return this->resultset->getInt(ws2s(fieldName));
}

// 返回下一条获取判断
bool
MySqlHelper::getRsNext(void)
{
	return this->resultset->next();
}

// 返回数据量
int
MySqlHelper::getRsCount(void)
{
	return this->resultset->rowsCount();
}


void
MySqlHelper::foreignKeyCheck(bool yes)
{
	if (yes)
	{
		this->_execute("SET foreign_key_checks = 1");
	}else{
		this->_execute("SET foreign_key_checks = 0");
	}
}

// 数据库初始化界面需要的函数
void
MySqlHelper::execRecoverStruct(const TCHAR *sql)
{
	this->foreignKeyCheck(FALSE);
	this->execute(sql);
	this->foreignKeyCheck(TRUE);
}
void
MySqlHelper::execRecoverData(const TCHAR *sql)
{
	this->foreignKeyCheck(FALSE);
	this->execute(sql);
	this->foreignKeyCheck(TRUE);
}
void
MySqlHelper::execDropDb(const TCHAR *name)
{
	TCHAR sql[64] = L"drop database ";
	lstrcat(sql, name);
	this->setSchema(s2ws(DEFAULTTABLE).c_str());
	this->execute(sql);
}
void
MySqlHelper::dropAllTable(const TCHAR *name)
{
	this->setSchema(name);
	this->executeQuery(L"show table status where engine is not null;");
	int count = this->resultset->rowsCount();
	register TCHAR tableNames[500][200] = {{0},{0}};
	const TCHAR* sqlHeader = L"drop table ";
	const TCHAR* sqlHeader2 = L"drop view ";
	TCHAR sql[256] = {0};
	int i = 0, j = 0;
	// 获取表格名称
	while(this->resultset->next())
	{
		lstrcpy(tableNames[i], utf8_2_ws(this->resultset->getString("name").c_str()).c_str());
		++i;
	}
	this->foreignKeyCheck(FALSE);
	// 执行
	while(j < i)
	{
		memset(sql, 0, sizeof(sql));
		lstrcpy(sql, sqlHeader);
		lstrcat(sql, tableNames[j]);
		this->_execute(ws2utf8(sql));
		memset(tableNames[j], 0, sizeof(tableNames[j]));
		++j;
	}

	
	// drop view
	wsprintf(sql, L"select table_name from information_schema.Views where table_schema = '%s'",
		name);
	this->executeQuery(sql);
	count = this->resultset->rowsCount();
	i = 0;
	j = 0;
	// 获取表格名称
	while(this->resultset->next())
	{
		lstrcpy(tableNames[i], utf8_2_ws(this->resultset->getString("table_name").c_str()).c_str());
		++i;
	}
	// 执行
	while(j < i)
	{
		memset(sql, 0, sizeof(sql));
		lstrcpy(sql, sqlHeader2);
		lstrcat(sql, tableNames[j]);
		this->_execute(ws2utf8(sql));
		memset(tableNames[j], 0, sizeof(tableNames[j]));
		++j;
	}

	this->foreignKeyCheck(TRUE);
}
void
MySqlHelper::execClearData(const TCHAR *name)
{
	this->con->setSchema(ws2s(name));
	this->executeQuery(L"show table status where engine is not null;");
	int count = this->resultset->rowsCount();
	register TCHAR tableNames[500][200] = {{0},{0}};
	const TCHAR* sqlHeader = L"truncate table ";
	TCHAR sql[128] = {0};
	int i = 0, j = 0;
	// 获取表格名称
	while(this->resultset->next())
	{
		lstrcpy(tableNames[i], utf8_2_ws(this->resultset->getString("name").c_str()).c_str());
		++i;
	}
	this->foreignKeyCheck(FALSE);
	while(j < i)
	{
		memset(sql, 0, sizeof(sql));
		lstrcpy(sql, sqlHeader);
		lstrcat(sql, tableNames[j]);
		// MessageBox(hWndMain, sql, L"", MB_OK);
		this->_execute(ws2utf8(sql));
		++j;
	}
	this->foreignKeyCheck(TRUE);
	
	// 清除
	//sql.append("");
}