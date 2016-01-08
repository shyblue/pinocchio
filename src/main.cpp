#include "config.h"
#include "pinocchio.h"
#include <boost/algorithm/string.hpp>
#include <tbb/tbb_thread.hpp>

using StringOpt =  boost::optional<std::string>;

int main(void)
{
	ST_CONFIG()->Initialize("./pinocchio.conf");

	TPinocchio app(ST_CONFIG()->GetConfigureData("SEVER_IP","127.0.0.1"),ST_CONFIG()->GetConfigureData("SERVER_PORT","18001"));
	
	tbb::tbb_thread appThread([&App](){ app.run(); });
	appThread.join();

	return 0;
}
