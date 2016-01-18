#include <exception>

#include "spdlog/spdlog.h"

#include "header.h"
#include "sync_session_impl.h"

SyncSessionImpl::SyncSessionImpl(boost::asio::io_service* io_service, Header* pheader) :
	SessionBase(io_service),
	m_resolver(*io_service),
	m_sendTimer(*io_service),
	m_sendPacketNo(1),
	m_pHeader(pheader)
{
	ClearSendTimeout();
	ClearRecvTimeout();	
}


SyncSessionImpl::~SyncSessionImpl(void)
{
	Close();
}

void SyncSessionImpl::SetSendTimeout(const timeval& settime)
{
	m_sendTimeInterval = settime;
}

void SyncSessionImpl::SetRecvTimeout(const timeval& settime)
{
	m_recvTimeInterval = settime;
}

void SyncSessionImpl::ClearSendTimeout()
{
	timerclear(&m_sendTimeInterval);
}
void SyncSessionImpl::ClearRecvTimeout()
{
	timerclear(&m_recvTimeInterval);
}

void SyncSessionImpl::SetTimeWait()
{
	int sendret = -1;
	int recvret = -1;

	if(m_sendTimeInterval.tv_sec != 0 || m_sendTimeInterval.tv_usec != 0)
		sendret = setsockopt(GetSocket().native(), SOL_SOCKET, SO_SNDTIMEO, (const char*)&m_sendTimeInterval, sizeof(timeval) );
	if(m_recvTimeInterval.tv_sec != 0 || m_recvTimeInterval.tv_usec != 0)
		recvret = setsockopt(GetSocket().native(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&m_recvTimeInterval, sizeof(timeval) );

	ST_LOGGER.Trace(L"sendret(%d) tv_sec(%d) tv_usec(%d)",sendret, m_sendTimeInterval.tv_sec, m_sendTimeInterval.tv_usec);
	ST_LOGGER.Trace(L"recvret(%d) tv_sec(%d) tv_usec(%d)",recvret, m_recvTimeInterval.tv_sec, m_recvTimeInterval.tv_usec);
}

bool SyncSessionImpl::IsConnection()
{
	return GetSocket().is_open();
}

void SyncSessionImpl::EndpointSet(std::string& ipaddress, std::string& port)
{
	return;
}

bool SyncSessionImpl::Connect(std::string& ipaddress, std::string& port)
{
	if (IsConnection())
	{
		ST_LOGGER.Error(L"IsConnection");
		Close();
	}

	EndpointSet(ipaddress, port);

	boost::asio::ip::tcp::resolver::query query(ipaddress.c_str(), port.c_str());
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = m_resolver.resolve(query);
	try
	{
		boost::system::error_code error = boost::asio::error::host_not_found;
		GetSocket().connect(*endpoint_iterator, error);	

		if (error)
			throw boost::system::system_error(error);

	}
	catch (boost::system::system_error& error)
	{
		ST_LOGGER.Error(L"[Error] boost system error [%d]", error.code().value() );
		return false;
	}
	catch (std::exception& excetion)
	{
		ST_LOGGER.Error(L"exception what[%s]", excetion.what());
		return false;
	}

	SetTimeWait();

	return true;
}

bool SyncSessionImpl::Send(const char* buffer, const size_t buffer_size)
{
	size_t total_transferred = 0;
	const int32_t try_count = 5;

	if (!IsConnection())
	{
		ST_LOGGER.Error(L"!IsConnection");
		return false;
	}

	if (buffer_size <= 0 )
	{
		ST_LOGGER.Error(L"buffer_size[%d]", buffer_size);
		return false;
	}

	try
	{
		for (int i = 0;i < try_count;++i)
		{
			int bytes_transferred = send(GetSocket().native(), (char*)(buffer + total_transferred), static_cast<int>(buffer_size - total_transferred), 0);		

			total_transferred += bytes_transferred;
			if(bytes_transferred < 0)
			{
				ST_LOGGER.Error(L"ret(%d)", bytes_transferred);
				return false;
			}

			if (total_transferred < buffer_size )
			{
				continue;
			}
			break;
		}
	}
	catch (boost::system::system_error& error)
	{
		ST_LOGGER.Error(L"error boost system error [%d]", error.code().value() );
		return false;
	}
	catch (std::exception& exception)
	{
		ST_LOGGER.Error(L"exception what[%s]", exception.what());
		return false;
	}

	if (total_transferred < buffer_size )
	{
		ST_LOGGER.Error(L"total_transferred[%d] < buffer_size[%d]", total_transferred, buffer_size);
		return false;
	}

	return true;
}


bool SyncSessionImpl::Paser(char* buffer, const size_t buffer_size)
{
	size_t total_length = HeaderPaser(buffer, buffer_size);
	if (total_length < 0)
	{
		ST_LOGGER.Error(L"!HeaderPaser");
		return false;
	}

	if (!BodyPaser(buffer, total_length))
	{
		ST_LOGGER.Error(L"!BodyPaser");
		return false;
	}

	return true;
}

size_t SyncSessionImpl::HeaderPaser(char* buffer, const size_t buffer_size)
{
	int availablebytes_transferred = recv(GetSocket().native(), buffer , static_cast<int>(m_pHeader->GetHeaderSize()), MSG_PEEK);
	if (availablebytes_transferred != m_pHeader->GetHeaderSize()) 
	{
		ST_LOGGER.Error(L"recv error");
		return 0;
	}

	size_t index = m_pHeader->Deserialize(buffer, m_pHeader->GetHeaderSize());

	if ( !m_pHeader->Validate() )
	{
		ST_LOGGER.Error(L"not Invalidheader");
	    return 0;
	}

	size_t total_length = m_pHeader->GetTotalSize();

	if ( total_length < 0 )
	{
		ST_LOGGER.Error(L"invalid total_length[%d]", total_length);
		return 0;
	}

	//std::stringstream hexCode;
	//for (size_t idx = 0; idx < m_pHeader->GetTotalSize(); ++idx)
	//{
	//	hexCode << std::hex << static_cast<int16_t>(buffer[idx]);
	//}
	//ST_LOGGER.Trace("[SyncSessionImpl] [HEX CODE : %s] [%d:%d]", hexCode.str().c_str(),m_pHeader->GetTotalSize(),m_pHeader->GetBodySize());

	return total_length;
}

bool SyncSessionImpl::BodyPaser(char* buffer, size_t& total_length)
{
	int bytes_transferred = 0;

	while ( total_length > bytes_transferred )
	{
		int transferred = recv(GetSocket().native(), (char*)(buffer + bytes_transferred), (static_cast<int>(total_length) - bytes_transferred), 0);

		if(transferred <= 0)
		{
				ST_LOGGER.Error(L"data transferred(%d)", transferred);
			    return false;	
		}

		bytes_transferred += transferred;
	}

	if (!m_pHeader->CheckEndmarker(buffer, bytes_transferred))
	{
		ST_LOGGER.Error(L"endmarker not");
		return false;	
	}

	return true;
}

bool SyncSessionImpl::Recv(char* buffer, size_t buffer_size)
{
	try
	{	
		if (!IsConnection())
		{
			ST_LOGGER.Error(L"!IsConnection");
			return false;
		}

		Paser(buffer, buffer_size);
	}
	catch(boost::system::system_error error)
	{
		ST_LOGGER.Error(L"error boost system error [%d]", error.code().value() );
		return false;
	}
	catch(std::exception& exception)
	{
		ST_LOGGER.Error(L"exception what[%s]", exception.what());
		return false;	
	}

	return true;
}

bool SyncSessionImpl::Close()
{
	try
	{
		boost::system::error_code close_error;

		if (IsConnection())
		{
			GetSocket().close(close_error);
		}
		else
			ST_LOGGER.Error("[SyncSessionImpl] Has not connection.");


		if (close_error)
			throw close_error;
	}
	catch (boost::system::system_error& error)
	{
		boost::system::error_code ec = error.code();
		ST_LOGGER.Error("[SyncSessionImpl] Boost System Error [%s : %d] %s",ec.category().name(),ec.value(),ec.message().c_str() );
		return false;
	}
	catch (std::exception& exception)
	{
		ST_LOGGER.Error("[SyncSessionImpl] Exception what [%s]", exception.what());
		return false;
	}
	return true;
}

const int32_t SyncSessionImpl::GetSendPacketNo()
{ 
	m_sendPacketNo.fetch_add(1);
	return m_sendPacketNo.load();
}