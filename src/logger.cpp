#include "logger.h"

Logger::Logger(void)
{
}

Logger::~Logger(void)
{
}

void Logger::Create()
{
	try
	{
		sinks_.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("server","log",23,59));
		sinks_.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());

		logger_ = std::make_shared<spdlog::logger>("pinocchio", sinks_.begin(), sinks_.end()) ;

		spdlog::register_logger(logger_);

		logger_->set_level(spdlog::level::trace);
	}
	catch (std::exception& e)
	{
		std::cout << "Log create failed" << e.what() << std::endl;
	}

}