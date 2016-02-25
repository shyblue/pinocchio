#include "pinocchio.h"
#include <chrono>


TPinocchio::TPinocchio(const std::string& ip, const std::string& port, const std::string& server_name, const std::string& auth_key, DbMgrPtr db_mgr_ptr)
		: app_(), ip_(ip), port_(port), server_name_(server_name), auth_key_(auth_key), log_handler_(), sp_db_mgr_(db_mgr_ptr), user_mgr_(
		sp_db_mgr_)
{
	Initialize();
}

bool TPinocchio::Initialize()
{
	crow::logger::setHandler(&log_handler_);

	AddRoute();
	AddRouteRegist();
	AddRouteSend();
	AddRouteRecv();
	AddRoutePublishApiKey();

	return true;
}

bool TPinocchio::AddRoute()
{
	ROUTE(app_, "/").name("pinocchio")
	( [] { return "This is pinocchio!"; } );

	return true;
}

bool TPinocchio::AddRouteRegist()
{
	ROUTE(app_, "/regist/<string>")
	([&](const std::string& token)
	 {
	 	if(token.length() < 40 || token.length() > 128)
			return crow::response(400);

		 /*
		if(sp_db_mgr_->IsMember(token) )
		{
			ST_LOGGER.Trace("Already regist in userToken [%s]",token.c_str());
			return crow::response(403);
		}
		*/

		 if(!sp_db_mgr_->AddMember(token))
		 {
			 ST_LOGGER.Error() << "Could not Add userToken [" << token << "]" ;
			 return crow::response(404);
		 }
		return crow::response(200);
	 }
	 );

	return true;
}

bool TPinocchio::AddRouteSend()
{
	ROUTE(app_, "/gcm/send").methods("POST"_method)
	([&](const crow::request &req) {
		Stopwatch<> sw;
		std::string server_key = req.get_header_value("Authorization");

		std::vector<std::string> fields;
		boost::split(fields, server_key, boost::is_any_of("="));
		boost::trim(fields[1]);
		if(!boost::equal(auth_key_, fields[1]))
		{
			ST_LOGGER.Error() << "AUTH KEY :" << auth_key_;
			ST_LOGGER.Error() << "FIELDS[1]:" << fields[1];

			return crow::response(405);
		}

		auto x = crow::json::load(req.body);
		if (!x) {
			ST_LOGGER.Error() << "[POSTED JSON PARSE ERROR]";
			ST_LOGGER.Error() << req.body ;
			ST_LOGGER.Error() << "[POSTED JSON END]";
			return crow::response(406);
		}

		std::string token = x["to"].s();
		if (token.front() == '"') {
			token.erase(0, 1);
			token.erase(token.size() - 1);
		}

		/*
       if(!sp_db_mgr_->IsMember(token) )
       {
           ST_LOGGER.Trace("Token not found in userToken [%s]",token.c_str());
           return crow::response(407);
       }
       */

		std::string msg = std::move(crow::json::dump(x["data"]));
		if (!sp_db_mgr_->AddMsg(token, msg)) {
			ST_LOGGER.Trace() << "Could not push message[" << msg << "] to user[" << token << "]";
			return crow::response(408);
		}

		ST_LOGGER.Trace() << "AddRouteSend" << "[" << msg << "] " << sw.stop() << " micro senconds";
		return crow::response(200);
	}
	);
	return true;
}

bool TPinocchio::AddRouteRecv()
{
	ROUTE(app_, "/gcm/recv/<string>")
	([&](const std::string& token)
	 {
		Stopwatch<> sw;
		if (token.length() > 128) return crow::response(400);
/*
		if (!sp_db_mgr_->IsMember(token))
		{
			ST_LOGGER.Trace("[Could not find userToken [%s]", token.c_str());
			return crow::response(407);
		}
*/
		// get user's msg
		std::vector<RedisValue> arr;
		if(!sp_db_mgr_->GetMsg(token, arr))
		{
			ST_LOGGER.Trace() << "[Get Message DB Error : " << token << "]" ;
			return crow::response(408);
		}

		// response msg
		std::stringstream oss;
		oss << "[";
		if(arr.size() > 1)	std::reverse(arr.begin(),arr.end());

		for(const auto & v : arr)
		{
			oss << "{data:" << v.toString() << "},";
		}
		oss << "{data:\"\"}]";
		ST_LOGGER.Trace() << "AddRouteRecv User " << token << " got  msg [" << oss.str() <<"] elapsed  " << sw.stop() << " micro seconds";
		return crow::response(oss.str());
	 }
	);

	return true;
}

bool TPinocchio::AddRoutePublishApiKey()
{
	ROUTE(app_, "/gcm/publish/ServerApiKey/<string>")
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
	bool isSsl = static_cast<bool>(ST_CONFIG.GetConfigureData<int>("IS_SSL_SERVER",0));
	if(isSsl)
	{
		app_.ssl_file(
				ST_CONFIG.GetConfigureData<std::string>("CERT_FILE_NAME","shyblue.sarang.net.crt"),
				ST_CONFIG.GetConfigureData<std::string>("KEY_FILE_NAME","shyblue.sarang.net.key")
		);
	}

	app_.ip(ip_)
			.port(static_cast<uint16_t >(std::stoi(port_)))
			.name(server_name_)
			.run();

	return true;
}