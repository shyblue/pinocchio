#include "pinocchio.h"
#include "config.h"

#include <boost/algorithm/string.hpp>


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
	CROW_ROUTE(m_app,"/").name("pinocchio")
	( [] { return "This is pinocchio!"; } );

	CROW_ROUTE(m_app,"/gcm/send").methods("POST"_method)
	([](const crow::request& req)
		{
			std::string server_key = req.get_header_value("Authorization");

			std::ostringstream os;

			std::vector<std::string> fields;
			boost::split(fields, server_key, boost::is_any_of("="));
			for (const auto& field : fields)
				os << "[" << field << "]";

			return crow::response{os.str()};
		}
	);

	CROW_ROUTE(m_app,"/regist/<string>")
	([](const crow::request& req,const std::string& token)
	 {
	 	auto x = crow::json::load(req.body);

		if(!x) return crow::response(400);

		std::ostringstream os;
		os << "send_msg";
		return crow::response{os.str()};
	 }
	 );

	CROW_ROUTE(m_app,"/gcm/recv/<string>")
	([](const std::string& token)
 	 {
		 if(token.length()!=10) return crow::response(400);
		 return crow::response(200);
	 }
	);

	return true;
}
bool TPinocchio::run()
{
	m_app.port(static_cast<uint16_t >(std::stoi(m_port)))
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
