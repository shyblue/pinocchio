#pragma once

#ifndef _NETWORK_ASYNCSESSION_IMPL_H_
#define _NETWORK_ASYNCSESSION_IMPL_H_

#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>
#include <atomic>

#include "common.h"
#include "session_base.h"

class Header;
class SyncSessionImpl : public SessionBase
{
public:

	const static int32_t kTimewaitsec = 1;

	typedef struct timeval				timeval;
	typedef boost::shared_ptr<Header>	HeaderPtr;

	explicit SyncSessionImpl(boost::asio::io_service* io_service, Header* pheader);
	virtual ~SyncSessionImpl(void);

	bool IsConnection();
	
	void SetSendTimeout(const timeval& settime);
	void SetRecvTimeout(const timeval& settime);

	void ClearSendTimeout();
	void ClearRecvTimeout();

	bool Connect(std::string& ipaddress, std::string& port);

	virtual bool Send(const char* buffer, const size_t buffer_size);

	virtual bool Recv(char* buffer, size_t buffer_size);
	virtual bool Close();

	const int32_t GetSendPacketNo();
protected :
	virtual void	EndpointSet(std::string& ipaddress, std::string& port);
	virtual bool	Parser(char *buffer, const size_t buffer_size);
	size_t			HeaderParser(char *buffer, const size_t buffer_size);
	bool			BodyParser(char *buffer, size_t &total_length);
private:
	void SetTimeWait();

	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::deadline_timer m_sendTimer;
	timeval m_sendTimeInterval;
	timeval m_recvTimeInterval;

	std::atomic<int32_t>	m_sendPacketNo;

	HeaderPtr m_pHeader;
};
#endif // _NETWORK_ASYNCSESSION_IMPL_H_