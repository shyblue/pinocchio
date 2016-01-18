#pragma once

#ifndef UTILITY_CONNECTOR_REDISCONNECTOR_H_
#define UTILITY_CONNECTOR_REDISCONNECTOR_H_

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/is_placeholder.hpp>

#include "logger.h"

#include "container.h"
#include "sync_session_impl.h"

template<typename _key, typename _value>
class Factory;
class ErrorEndMaker{};

class RedisConnector : public SyncSessionImpl
{
public:
	
	static const char RC_ERROR;
	static const char RC_INLINE;
	static const char RC_BULK;
	static const char RC_MULTIBULK;
	static const char RC_INT;

	static const char* RC_IDENTIFIER;

	typedef boost::function<bool (char*, bool&)	>	ProcessFunc;
	typedef boost::shared_ptr<MapContainer<const char, ProcessFunc > >	FunctionConatainer;

	explicit RedisConnector(boost::asio::io_service* io_service);
	explicit RedisConnector(boost::asio::io_service* io_service, std::string host, std::string port, const timeval timeout);
	~RedisConnector(void);

private:
	virtual void EndpointSet(std::string& ipaddress, std::string& port);
	virtual bool Parser(char *buffer, const size_t buffer_size);

	void Initialize();

	void ProcessFuncRegister();

	bool ErrorProcessing(char* buffer, bool& retry);
	bool InlineProcessing(char* buffer, bool& retry);
	bool BulkProcessing(char* buffer, bool& retry);
	bool MultiBulkProcessing(char* buffer, bool& retry);
	bool IntProcessing(char* buffer, bool& retry);

	char* IdentifierParseToString(char* buffer, std::string& value);
	char* IdentifierParseToInt(char* buffer, int& value);
	char* Bulk(char* buffer, bool& retry, std::string& value);

	std::string					m_error;
	std::string					m_inline;
	std::string					m_bulk;
	std::vector<std::string>	m_multiBulk;
	int32_t						m_int;
	FunctionConatainer			m_functionContainer;
};

#endif


