#pragma once

#include "crow/crow.h"
#include "crow/json.h"
#include "spdlog/spdlog.h"
#include <string>

class TPinocchio
{

public:
	TPinocchio(const std::string& ip, const std::string& port);
	bool run();
private:
	bool AddRouter();
	bool Initialize();

	crow::SimpleApp m_app;
	std::string m_port;
	std::string m_ip;

};
