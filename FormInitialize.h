#pragma once
#include "formmysqlbase.h"
class FormInitialize :
	public FormMysqlBase
{
public:
	FormInitialize(void);
	~FormInitialize(void);
	void connectCalling();
};

