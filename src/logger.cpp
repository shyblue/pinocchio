#include <iostream>
#include <stdarg.h>

#include "util/logger.h"

Logger::Logger(void)
{
}

Logger::~Logger(void)
{
}

void Logger::Create(const std::string& config_name)
{
	try
	{
		log4cxx::xml::DOMConfigurator::configureAndWatch( config_name );
		m_pLogger = log4cxx::Logger::getRootLogger();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Logger::Trace(const char* format, ...)
{
	char log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vsprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_TRACE(m_pLogger, log);
}

void Logger::Debug(const char* format, ...)
{
	char log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vsprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);
	
	LOG4CXX_TRACE(m_pLogger, log);
}

void Logger::Info(const char* format, ...)
{
	char log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vsprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_INFO(m_pLogger, log);
}

void Logger::Warn(const char* format, ...)
{
	char log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vsprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_WARN(m_pLogger, log);
}

void Logger::Error(const char* format, ...)
{
	char log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vsprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_ERROR(m_pLogger, log);
}

void Logger::Fatal(const char* format, ...)
{
	char log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vsprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);
	
	LOG4CXX_FATAL(m_pLogger, log);
}

void Logger::Trace(const wchar_t* format, ...)
{
	wchar_t log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vswprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);
	
	LOG4CXX_TRACE(m_pLogger, log);
}

void Logger::Debug(const wchar_t* format, ...)
{
	wchar_t log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vswprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_TRACE(m_pLogger, log);
}

void Logger::Info(const wchar_t* format, ...)
{
	wchar_t log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vswprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_INFO(m_pLogger, log);
}

void Logger::Warn(const wchar_t* format, ...)
{
	wchar_t log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vswprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);
	
	LOG4CXX_WARN(m_pLogger, log);
}

void Logger::Error(const wchar_t* format, ...)
{
	wchar_t log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vswprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_ERROR(m_pLogger, log);
}

void Logger::Fatal(const wchar_t* format, ...)
{
	wchar_t log[kMaxBufferLength] = { 0, };

	va_list args;
	va_start(args, format);

	vswprintf_s(log, kMaxBufferLength, format, args);

	va_end(args);

	LOG4CXX_FATAL(m_pLogger, log);
}