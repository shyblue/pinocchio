#include "pinocchio.h"
#include "config.h"

using StringOpt =  boost::optional<std::string>;

TPinocchio::TPinocchio(const std::string& ip, const std::string& port) : m_app(),m_ip(ip),m_port(port)
{
	Initialize();
}

bool TPinocchio::Initialize()
{
	return AddRouter();
}

bool TPinocchio::AddRouter()
{
	CROW_ROUTE(m_app,"/").name("hello")
	( [] { return "Hello World!"; } );

	CROW_ROUTE(m_app,"/send_msg").methods("POST"_method)
	([](const crow::request& req)
	 {
	 	auto x = crow::json::load(req.body);

		if(!x) return crow::response(400);

		std::ostringstream os;
		os << "send_msg";
		return crow::response{os.str()};
	 }
	 );

	return true;
}
bool TPinocchio::run()
{
	m_app.port(std::stoi(m_port))
		.multithreaded()
		.run();

	return true;
}
int main(void)
{
	Config conf("./pinocchio.conf","SERVER_CONFIG");
	
	std::string IP;
	std::string PORT;

	StringOpt ip_opt = conf.GetElementStr("IP");
	if(ip_opt)
	{
		IP = ip_opt.get();
	}
	

	StringOpt port_opt = conf.GetElementStr("PORT");
	if(port_opt)
	{
		PORT = port_opt.get();
	}

	TPinocchio App(IP,PORT);
	App.run();

	return 0;
}
