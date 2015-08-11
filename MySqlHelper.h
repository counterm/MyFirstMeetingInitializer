#pragma once
#include <mysql_connection.h>
#include <mysql_driver.h>
#include "SqlHelperBase.h"
#include <cppconn/statement.h>
#include "ProjUtils.h"
using namespace std;
using namespace sql;


extern HWND hWndMain;

class MySqlHelper : public SqlHelperBase
{
private:
	wstring ip;
	wstring port;
	wstring name;
	wstring pwd;
	wstring scheme;
	Driver *driver;
    auto_ptr<Connection> con;
    auto_ptr<Statement> stmt;
    ResultSet *resultset;
public:
	MySqlHelper();
	~MySqlHelper();
	wstring databases[200];
	bool bIsConnect;
	void init(const TCHAR *aIp, const TCHAR *aPort, const TCHAR *aName, const TCHAR *aPwd, const TCHAR *aScheme);
	bool connect();
	void initDatabases();
	void closeConnection();
	void useSchema();
	bool connectCheck();
	void setSchema(const TCHAR* name);

	void _execute(const sql::SQLString &sql);
	ResultSet *_executeQuery(const sql::SQLString &sql);
	int _executeUpdate(const sql::SQLString &sql);

	void execute(const TCHAR *sql);
	ResultSet *executeQuery(const TCHAR *sql);
	int executeUpdate(const TCHAR *sql);

	TCHAR *getRsString(const TCHAR *fieldName);
	int getRsInt(const TCHAR *fieldName);
	bool getRsNext(void);
	int getRsCount(void);

	void foreignKeyCheck(bool yes);

	// 数据库初始化界面需要的函数
	void execRecoverStruct(const TCHAR *sql);
	void execRecoverData(const TCHAR *sql);
	void execDropDb(const TCHAR *name);
	void execClearData(const TCHAR *name);
	void dropAllTable(const TCHAR *name);
};