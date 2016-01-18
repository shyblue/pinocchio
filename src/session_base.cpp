#include "session_base.h"

SessionBase::SessionBase(boost::asio::io_service* io_service):
	m_tcpSocket(*io_service)
{

}
SessionBase::~SessionBase(void)
{
}

boost::asio::ip::tcp::socket& SessionBase::GetSocket()
{
	return m_tcpSocket;
}