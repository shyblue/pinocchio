#pragma once

#include <boost/serialization/singleton.hpp>
#include <spdlog/spdlog.h>

class Logger : public boost::serialization::singleton<Logger>
{
public:
	void Create();
	auto Trace() { return logger_->trace(); }
	auto Debug(){ return logger_->debug(); }
	auto Info()	{ return logger_->info();	}
	auto Warn() { return logger_->warn(); }
	auto Error() { return logger_->error(); }
	auto Critical() { return logger_->critical(); }
protected:
	Logger(void);
	virtual ~Logger(void);

private:
	std::vector<spdlog::sink_ptr> sinks_;
	std::shared_ptr<spdlog::logger> logger_;
};

#define ST_LOGGER Logger::get_mutable_instance()