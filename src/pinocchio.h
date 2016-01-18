#include "crow/crow.h"
#include "spdlog/spdlog.h"
#include <string>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_hash_map.h>

#include <chrono>

#include "config.h"

class TUserMsg
{
public:
	TUserMsg(){ Load(); }


private:
	bool Load()
	{
		// Load user push message data from DB(or cache)
	}

	std::string m_userToken;
	tbb::concurrent_bounded_queue<std::string> m_msgQueue;
	std::chrono::system_clock::time_point m_lastTouch;
};

class TUserMap
{
public:
	TUserMap(){}

private:
    bool Load()
    {
        // Load user from DB
    }

};
class TPinocchio
{

public:
	TPinocchio(const std::string& ip, const std::string& port, const std::string& server_name);
	bool run();
private:

	bool AddRoute();
	bool AddRouteRegist();
	bool AddRouteSend();
	bool AddRouteRecv();
	bool AddRoutePublishApiKey();
	bool Initialize();

	crow::Crow<> m_app;
	const std::string& m_port;
	const std::string& m_ip;
	const std::string& m_serverName;

	tbb::concurrent_hash_map<std::string,TUserMsg> m_msgData;
};

