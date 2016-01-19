#include "config.h"
#include "pinocchio.h"
#include "logger.h"

#include <tbb/tbb_thread.h>

using StringOpt =  boost::optional<std::string>;

int main(void)
{
	ST_LOGGER.Create("log4cxx.xml");
	ST_LOGGER.Info("Pinocchio Server Start!!!");

	ST_CONFIG()->Initialize("./pinocchio.conf");

	TPinocchio app(
			ST_CONFIG()->GetConfigureData<std::string>("SERVER_IP","0.0.0.0"),
			ST_CONFIG()->GetConfigureData<std::string>("SERVER_PORT","18080"),
			ST_CONFIG()->GetConfigureData<std::string>("SERVER_NAME","PINOCCHIO"),
			ST_CONFIG()->GetConfigureData<std::string>("AUTH_KEY","AIzaSyCaV3Ymy5IPqnxXVkrYmwG0IpB5mNj1ZZ8")
	);

	tbb::tbb_thread appThread([&app](){ app.run(); });
	appThread.join();

	return 0;
}
