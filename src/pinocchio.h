#include "crow/crow.h"
#include "logger.h"

#include <string>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_hash_map.h>

#include <chrono>

#include "config.h"
#include "database_manager.h"
#include "user_manager.h"

class PinocchioLogHandler : public crow::ILogHandler {
public:
	void log(std::string message, crow::LogLevel level) override {

		switch (level)
		{
			case crow::LogLevel::DEBUG:
				ST_LOGGER.Debug(message.c_str());
				break;
			case crow::LogLevel::INFO:
				ST_LOGGER.Info(message.c_str());
				break;
			case crow::LogLevel::WARNING:
				ST_LOGGER.Warn(message.c_str());
				break;
			case crow::LogLevel::ERROR:
				ST_LOGGER.Error(message.c_str());
				break;
			case crow::LogLevel::CRITICAL:
				ST_LOGGER.Fatal(message.c_str());
				break;
		}
	}
};

class TPinocchio
{

public:
	using DbMgrPtr=std::shared_ptr<TDatabaseManager>;

	TPinocchio(const std::string& ip, const std::string& port, const std::string& server_name, const std::string& auth_key, DbMgrPtr db_mgr_ptr);
	bool run();
private:

	bool AddRoute();
	bool AddRouteRegist();
	bool AddRouteSend();
	bool AddRouteRecv();
	bool AddRoutePublishApiKey();
	bool Initialize();

	crow::Crow<> m_app;
	const std::string& m_ip;
	const std::string& m_port;
	const std::string& m_serverName;
	const std::string& m_authKey;
	PinocchioLogHandler m_logHandler;
	DbMgrPtr m_spDbMgr;
	UserManager m_userMgr;

};

