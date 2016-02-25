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
				ST_LOGGER.Debug() << message;
				break;
			case crow::LogLevel::INFO:
				ST_LOGGER.Info() << message;
				break;
			case crow::LogLevel::WARNING:
				ST_LOGGER.Warn() << message;
				break;
			case crow::LogLevel::ERROR:
				ST_LOGGER.Error() << message;
				break;
			case crow::LogLevel::CRITICAL:
				ST_LOGGER.Critical() << message;
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

	crow::Crow<> app_;
	const std::string ip_;
	const std::string port_;
	const std::string server_name_;
	const std::string auth_key_;
	PinocchioLogHandler log_handler_;
	DbMgrPtr sp_db_mgr_;
	UserManager user_mgr_;

};

