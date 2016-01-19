#ifndef _UTIL_LOGGER_H_
#define _UTIL_LOGGER_H_

#include <boost/serialization/singleton.hpp>
#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/exception.h>

class Logger : public boost::serialization::singleton<Logger>
{
public:
	static const int kMaxBufferLength = 1024;

	void Create(const std::string& config_name);
	
	void Trace(const char* format, ...);
	void Debug(const char* format, ...);
	void Info(const char* format, ...);
	void Warn(const char* format, ...);
	void Error(const char* format, ...);
	void Fatal(const char* format, ...);

	void Trace(const wchar_t* format, ...);
	void Debug(const wchar_t* format, ...);
	void Info(const wchar_t* format, ...);
	void Warn(const wchar_t* format, ...);
	void Error(const wchar_t* format, ...);
	void Fatal(const wchar_t* format, ...);

protected:
	Logger(void);
	virtual ~Logger(void);

private:
	log4cxx::LoggerPtr m_pLogger;
};

#define ST_LOGGER Logger::get_mutable_instance()

#endif // _UTIL_LOGGER_H_