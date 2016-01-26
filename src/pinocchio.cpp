#include "pinocchio.h"
#include <boost/algorithm/string.hpp>

TPinocchio::TPinocchio(const std::string& ip, const std::string& port, const std::string& server_name, const std::string& auth_key)
		: m_app(),m_ip(ip),m_port(port), m_serverName(server_name), m_authKey(auth_key), m_logHandler()
{
	Initialize();
}

bool TPinocchio::Initialize()
{
	crow::logger::setHandler(&m_logHandler);

	AddRoute();
	AddRouteRegist();
	AddRouteSend();
	AddRouteRecv();
	AddRoutePublishApiKey();

	return true;
}

bool TPinocchio::AddRoute()
{
	ROUTE(m_app,"/").name("pinocchio")
	( [] { return "This is pinocchio!"; } );

	return true;
}

bool TPinocchio::AddRouteRegist()
{
	ROUTE(m_app,"/regist/<string>")
	([&](const std::string& token)
	 {
	 	if(token.length() < 40 || token.length() > 128)
			return crow::response(400);

		 // Find user token

		 // if exist error

		 // else add user token

		return crow::response(200);
	 }
	 );

	return true;
}

bool TPinocchio::AddRouteSend()
{
	ROUTE(m_app,"/gcm/send").methods("POST"_method)
	([this](const crow::request& req)
	{
		std::string server_key = req.get_header_value("Authorization");
		std::ostringstream os;

		std::vector<std::string> fields;
		boost::split(fields, server_key, boost::is_any_of("="));

		// if(boost::algorithm::to_lower(fields[0]) != "key") return crow::response(400);
		if(!m_authKey.find(fields[1].empty())) return crow::response(400);
/*
		tbb::concurrent_hash_map<std::string,TUserMsg>::accessor a;
		if(m_userMap.find(a,key))
		{
			// fields[1] is Server Api Key
			// find server api key
			// if find
			{
				// send
			}
			fields[1].trim();
		}
*/
		return crow::response{os.str()};
	}
	);

	return true;
}

bool TPinocchio::AddRouteRecv()
{
	ROUTE(m_app,"/gcm/recv/<string>")
	([](const std::string& token)
 	 {
		 if(token.length()>128) return crow::response(400);
		 return crow::response(200);
	 }

	 // find user

	// can't find user response 400

	// get user's msg
	// response msg

	);

	return true;
}

bool TPinocchio::AddRoutePublishApiKey()
{
	ROUTE(m_app,"/gcm/publish/ServerApiKey/<string>")
	([](const std::string& base_key)
	 {
	 	if(base_key.length() < 10 || base_key.length() > 128 ) {
			return crow::response(400);
		}


		 return crow::response(200);
	 }
	);

	return true;
}
bool TPinocchio::run()
{
	m_app.ip(m_ip)
			.port(static_cast<uint16_t >(std::stoi(m_port)))
			.name(m_serverName)
			.multithreaded()
			.run();

	return true;
}
