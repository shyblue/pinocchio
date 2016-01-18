#include <boost/asio/detail/socket_ops.hpp>

#include "byte_order.h"

ByteOrder::ByteOrder(void)
{
}


ByteOrder::~ByteOrder(void)
{
}
// network to host
uint64_t ByteOrder::network_to_host( uint64_t value )
{
#ifdef _ENABLE
	return ( (static_cast<uint64_t>(ntohl(static_cast<uint32_t>(value))) << 32 ) + ntohl(value >> 32) );
#else
	return value;
#endif
}

int64_t ByteOrder::network_to_host( int64_t value )
{
#ifdef _ENABLE
	return ( (static_cast<uint64_t>(ntohl(static_cast<uint32_t>(value))) << 32 ) + ntohl(value >> 32) );
#else
	return value;
#endif
}

uint32_t ByteOrder::network_to_host( uint32_t value )
{
#ifdef _ENABLE
	return ntohl(value);
#else
	return value;
#endif
}

int32_t ByteOrder::network_to_host( int32_t value)
{
#ifdef _ENABLE
	return ntohl(value);
#else
	return value;
#endif
}

uint16_t ByteOrder::network_to_host( uint16_t value )
{
#ifdef _ENABLE
	return ntohs(value);
#else
	return value;
#endif
}

int16_t ByteOrder::network_to_host( int16_t value)
{
#ifdef _ENABLE
	return ntohs(value);
#else
	return value;
#endif
}

// host to network
uint64_t ByteOrder::host_to_network( uint64_t value )
{
#ifdef _ENABLE
	return ( (static_cast<uint64_t>(htonl(static_cast<uint32_t>(value))) << 32 ) + htonl(value >> 32) );
#else
	return value;
#endif
}

int64_t ByteOrder::host_to_network( int64_t value )
{
#ifdef _ENABLE
	return ( (static_cast<uint64_t>(htonl(static_cast<uint32_t>(value))) << 32 ) + htonl(value >> 32) );
#else
	return value;
#endif
}


uint32_t ByteOrder::host_to_network( uint32_t value )
{
#ifdef _ENABLE
	return htonl(value);
#else
	return value;
#endif
}

int32_t ByteOrder::host_to_network( int32_t value )
{
#ifdef _ENABLE
	return htonl(value);
#else
	return value;
#endif
}

uint16_t ByteOrder::host_to_network( uint16_t value )
{
#ifdef _ENABLE
	return htons(value);
#else
	return value;
#endif
}

int16_t ByteOrder::host_to_network(int16_t value)
{
#ifdef _ENABLE
	return htons(value);
#else
	return value;
#endif
}


