#include "pinocchio.h"

TPinocchio::TPinocchio(const std::string& ip, const std::string& port) : m_app(),m_ip(ip),m_port(port)
{
	Initialize();
}

bool TPinocchio::Initialize()
{
	AddRoute();
	AddRouteRegist();
	AddRouteSend();
	AddRouteRecv();
	AddRoutePublishApiKey();

	return true;
}

bool TPinocchio::AddRoute()
{
	CROW_ROUTE(m_app,"/").name("pinocchio")
	( [] { return "This is pinocchio!"; } );

	return true;
}

bool TPinocchio::AddRouteRegist()
{
	CROW_ROUTE(m_app,"/regist/<string>")
	([&](const std::string& token)
	 {
	 	if(token.length() < 40 || token.length() > 128)
			return crow::response(200);
			
		std::ostringstream os;
		os << token;

		return crow::response{os.str()};
	 }
	 );

	return true;
}

bool TPinocchio::AddRouteSend()
{
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

	return true;
}

bool TPinocchio::AddRouteRecv()
{
	CROW_ROUTE(m_app,"/gcm/recv/<string>")
	([](const std::string& token)
 	 {
		 if(token.length()>128) return crow::response(400);
		 return crow::response(200);
	 }
	);

	return true;
}

bool TPinocchio::AddRoutePublishApiKey()
{
	CROW_ROUTE(m_app,"/gcm/publish/ServerApiKey/<string>")
	([](const std::string& base_key)
	 {
	 	if(base_key.length() < 10 || base_key.length() > 128 )
		return crow::response(400);
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
