#pragma once

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <map>

template<typename _key, typename _value>
class Factory;

class ErrorEndMaker{};

class RedisConnector
{
public:
	
	static const char RC_ERROR;
	static const char RC_INLINE;
	static const char RC_BULK;
	static const char RC_MULTIBULK;
	static const char RC_INT;

	static const char* RC_IDENTIFIER;

	typedef boost::function<bool (char*, bool&)	>														ProcessFunc;
	typedef boost::shared_ptr<std::map<const char, ProcessFunc > >	FunctionContainer;

	explicit RedisConnector(boost::asio::io_service* io_service);
	explicit RedisConnector(boost::asio::io_service* io_service, std::string host, std::string port, const timeval timeout);
	~RedisConnector(void);

private:
	virtual void EndpointSet(std::string& ipaddress, std::string& port);
	virtual bool Parser(char* buffer, const size_t buffer_size);

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

	std::string					val_error_;
	std::string					val_inline_;
	std::string					val_bulk_;
	std::vector<std::string>	val_multi_bulk_;
	int32_t						val_int_;


	FunctionConatainer			func_container_;
};


