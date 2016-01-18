#pragma once

#ifndef SESSION_BASE_H_
#define SESSION_BASE_H_

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include "common.h"

class SessionBase : private boost::noncopyable
{
public:
	SessionBase(boost::asio::io_service* io_service);
	virtual ~SessionBase(void);

	virtual bool Recv(char* buffer, size_t buffer_size) = 0;
	virtual bool Send(const char* buffer, const size_t buffer_size) = 0;
	virtual bool Close() = 0;
	boost::asio::ip::tcp::socket& GetSocket();
protected:
	boost::asio::ip::tcp::socket m_tcpSocket;
};
#endif