#include "crow/crow.h"
#include "spdlog/spdlog.h"
#include <string>

class TPinocchio
{

public:
	TPinocchio(const std::string& ip, const std::string& port);
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

};
