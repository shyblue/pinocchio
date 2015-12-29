#include "crow/crow.h"
#include "spdlog/spdlog.h"
#include <string>
#include <tbb/concurrent_queue.hpp>
#include <chrono>

class TUserMsg
{
public:
	TUser(){ Load(); }

	std::string&

private:
	bool Load()
	{
		// Load user data from DB(or cache)
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
	std::string m_port;
	std::string m_ip;

	tbb::concurrent_hash_map<std::string,TUserMsg> m_msgData;
};

