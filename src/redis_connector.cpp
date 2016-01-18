#include "redis_connector.h"

const char RedisConnector::RC_ERROR		= '-';
const char RedisConnector::RC_INLINE	= '+';
const char RedisConnector::RC_BULK		= '$';
const char RedisConnector::RC_MULTIBULK = '*';
const char RedisConnector::RC_INT		= ':';

const char * RedisConnector::RC_IDENTIFIER = "\r\n";

RedisConnector::RedisConnector(boost::asio::io_service* io_service) :
	SyncSessionImpl(io_service, 0)
	,m_int(0)
//	, m_functorContainer(MapContainer<const char, boost::function<bool (char*, bool&)> >() )
{
	Initialize();
}

RedisConnector::RedisConnector(boost::asio::io_service* io_service, std::string host, std::string port, const timeval timeout) :
	SyncSessionImpl(io_service, 0)
	,m_int(0)
//	, m_functorContainer(MapContainer<const char, boost::function<bool (char*, bool&)> >() )
{
	Initialize();
	// timeout value set and connect set
	SetSendTimeout(timeout);
	SetRecvTimeout(timeout);
	Connect(host, port);
}

RedisConnector::~RedisConnector(void)
{
}

void RedisConnector::Initialize()
{
	ProcessFuncRegister();
}

void RedisConnector::ProcessFuncRegister()
{
	m_functionContainer->Insert(RC_ERROR,	boost::bind(&RedisConnector::ErrorProcessing, this, _1, _2));
	m_functionContainer->Insert(RC_INLINE,	boost::bind(&RedisConnector::InlineProcessing, this, _1, _2));
	m_functionContainer->Insert(RC_BULK,	boost::bind(&RedisConnector::BulkProcessing, this, _1, _2));
	m_functionContainer->Insert(RC_MULTIBULK,boost::bind(&RedisConnector::MultiBulkProcessing, this, _1, _2));
	m_functionContainer->Insert(RC_INT,		boost::bind(&RedisConnector::IntProcessing, this, _1, _2));
}

void RedisConnector::EndpointSet(std::string& ipaddress, std::string& port)
{
	if(ipaddress == "0")
	{
		// defalut address : roof back
		ipaddress.assign("127.0.0.1");
	}
	if(port == "0")
	{
		// redis defalut port : 6379
		port.assign("6379");
	}
}

bool RedisConnector::Parser(char *buffer, const size_t buffer_size)
{
	size_t availablebytes_transferred = 0;
	size_t recv_size = 0;
	size_t index = 1;

	bool retry = false;

	do
	{
		availablebytes_transferred = recv(GetSocket().native(), buffer + availablebytes_transferred , static_cast<int>(buffer_size - availablebytes_transferred), 0);
	
		recv_size += availablebytes_transferred;

		if(availablebytes_transferred > 0)
		{
			char* pass_buffer = buffer + index;

			ProcessFunc* processing = m_functionContainer->GetValue(buffer[0]);

			if(processing)
			{
				processing->operator()(pass_buffer, retry);
			}
		}
		else
		{
			ST_LOGGER.Error("[Redis Error] time out");
			return false;
		}
	} while(retry);

	return true;
}

bool RedisConnector::ErrorProcessing(char* buffer, bool& retry)
{
	if (!IdentifierParseToString(buffer, m_error))
	{
		ST_LOGGER.Error("[RedisConnector] ErrorProcessing error");
		return true;
	}

	return true;
}

bool RedisConnector::InlineProcessing(char* buffer, bool& retry)
{

	if (!IdentifierParseToString(buffer, m_inline))
	{
		ST_LOGGER.Error("[RedisConnector] InlineProcessing error");
		return true;
	}

	return true;
}

bool RedisConnector::BulkProcessing(char* buffer, bool& retry)
{

	if (!Bulk(buffer, retry, m_bulk))
	{
		ST_LOGGER.Error("[RedisConnector] BulkProcessing EndMaker error");
	}
	return true;	
}

bool RedisConnector::MultiBulkProcessing(char* buffer, bool& retry)
{
	char * p = 0;
	int bulk_count = 0;

    p = IdentifierParseToInt(buffer, bulk_count);
    if(p)
	{
		p += strlen(RC_IDENTIFIER);

		m_multiBulk.clear();

		for(int i = 0; i < bulk_count; ++i)
		{
			if(p[0] == RC_BULK)
			{
				p++;
				std::string bulk;
				if (!(p = Bulk(p, retry, bulk)))
				{
					ST_LOGGER.Error("[RedisConnector] MultiBulkProcessing error");
					return false;
				}
				m_multiBulk.push_back(bulk);		
			}
		}
	}

	return true;
}

bool RedisConnector::IntProcessing(char* buffer, bool& retry)
{
	if(!IdentifierParseToInt(buffer, m_int))
	{
		ST_LOGGER.Error("[RedisConnector] InlineProcessing error");
		return false;
	}

	return true;
}

char* RedisConnector::IdentifierParseToString(char* buffer, std::string& value)
{
	char * p = nullptr;
	p = strstr(buffer, RC_IDENTIFIER);

	if (p != nullptr)
	{
		value.assign(buffer, p); 
		return p;
	}

	return p;
}
char* RedisConnector::IdentifierParseToInt(char* buffer, int& value)
{
	char * p = nullptr;
	p = strstr(buffer, RC_IDENTIFIER);

	if (p != nullptr)
	{
		std::string num(buffer, p);
		value = std::stoi(num);
		return p;
	}

	return p;
}

char* RedisConnector::Bulk(char* buffer, bool& retry, std::string& value)
{
	char * p = nullptr;
	int body_length = 0;

	p = IdentifierParseToInt(buffer, body_length);
	if(p != nullptr)
	{	
		p += strlen(RC_IDENTIFIER);

		if(strlen(p) < body_length + strlen(RC_IDENTIFIER))
		{
			retry = true;
			return 0;
		}

		value.assign(p, body_length);
		p += body_length + strlen(RC_IDENTIFIER); 
	}
	else
	{
		ST_LOGGER.Error("[RedisConnector] BulkProcessing EndMaker error");
	}

	return p;
}


