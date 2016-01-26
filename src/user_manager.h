#pragma once

#include "database_manager.h"
#include "logger.h"

class UserManager
{
	using DbMgrPtr=std::shared_ptr<TDatabaseManager>;
public:
	explicit  UserManager(DbMgrPtr db) : m_spDbManager(db) { Initialize(); }
	~UserManager() {};

private:
	int Initialize() { return Load(); };
	int Load();

	DbMgrPtr m_spDbManager;
};