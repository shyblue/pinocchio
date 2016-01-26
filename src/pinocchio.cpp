#include "pinocchio.h"

TPinocchio::TPinocchio(const std::string& ip, const std::string& port, const std::string& server_name, const std::string& auth_key, DbMgrPtr db_mgr_ptr)
		: m_app(),m_ip(ip),m_port(port), m_serverName(server_name), m_authKey(auth_key), m_logHandler(), m_spDbMgr(db_mgr_ptr), m_userMgr(m_spDbMgr)
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

		 if(!m_spDbMgr->AddMember(token))
		 {
			 ST_LOGGER.Error("Could not Add userToken [%s]",token.c_str());
			 return crow::response(404);
		 }
		return crow::response(200);
	 }
	 );

	return true;
}

bool TPinocchio::AddRouteSend()
{
	ROUTE(m_app,"/gcm/send").methods("POST"_method)
	([&](const crow::request& req)
	{
		std::string server_key = req.get_header_value("Authorization");

		ST_LOGGER.Trace("[Authorization : %s]",server_key.c_str());
		std::vector<std::string> fields;
		boost::split(fields, server_key, boost::is_any_of("="));

		// if(boost::algorithm::to_lower(fields[0]) != "key") return crow::response(400);
		if(!m_authKey.find(fields[1])) return crow::response(405);

		auto x = crow::json::load(req.body);
		if(!x)
		{
			ST_LOGGER.Trace("[POSTED JSON]");
			ST_LOGGER.Trace("%s",req.body.c_str());
			ST_LOGGER.Trace("[POSTED JSON END]");
			return crow::response(406);
		}

		std::stringstream to,data;
		to << x["to"];
		ST_LOGGER.Trace("%s",to.str().c_str());
		std::string token( to.str() );
		if(token.front()=='"')
		{
			token.erase(0,1);
			token.erase(token.size()-1);
		}

		if(!m_spDbMgr->IsMember(token) )
		{
			ST_LOGGER.Trace("Token not found in userToken [%s]",token.c_str());
			return crow::response(407);
		}

		data << x["data"];
		ST_LOGGER.Trace("%s",data.str().c_str());
		std::string msg( data.str() );
		if(!m_spDbMgr->AddMsg(token,msg))
		{
			ST_LOGGER.Trace("Could not push message[%s] to user[%s]",msg.c_str(),token.c_str());
			return crow::response(408);
		}

		return crow::response(200);
	}
	);

	return true;
}

bool TPinocchio::AddRouteRecv()
{
	ROUTE(m_app,"/gcm/recv/<string>")
	([&](const std::string& token) {
		if (token.length() > 128) return crow::response(400);

		if (!m_spDbMgr->IsMember(token))
		{
			ST_LOGGER.Trace("[Could not find userToken [%s]", token.c_str());
			return crow::response(407);
		}

		// get user's msg
		std::string msg;
		if(!m_spDbMgr->GetMsg(token,msg))
		{
			ST_LOGGER.Trace("[Get Message DB Error [%s]",token.c_str());
			return crow::response(408);
		}
		// response msg
		crow::json::wvalue x;
		x["data"]=msg;

		return crow::response(x);
	}
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
