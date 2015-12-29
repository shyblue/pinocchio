
#include "pinocchio.h"
#include "config.h"

#include <boost/algorithm/string.hpp>
#include <tbb/tbb_thread.hpp>

using StringOpt =  boost::optional<std::string>;


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
	
	tbb::tbb_thread AppThread([&App](){ App.run(); });

	return 0;
}
