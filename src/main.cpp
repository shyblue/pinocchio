#include "config.h"
#include "pinocchio.h"
#include <tbb/tbb_thread.h>

using StringOpt =  boost::optional<std::string>;

int main(void)
{
	ST_CONFIG()->Initialize("./pinocchio.conf");

	TPinocchio app(
			ST_CONFIG()->GetConfigureData<std::string>("SERVER_IP","0.0.0.0"),
			ST_CONFIG()->GetConfigureData<std::string>("SERVER_PORT","18080"),
			ST_CONFIG()->GetConfigureData<std::string>("SERVER_NAME","PINOCCHIO")
	);

	tbb::tbb_thread appThread([&app](){ app.run(); });
	appThread.join();

	return 0;
}
