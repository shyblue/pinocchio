#pragma once
#include "common.h"

class ByteOrder
{
public:
	ByteOrder(void);
	~ByteOrder(void);

	// network to host
	static uint64_t		network_to_host( uint64_t value );
	static int64_t		network_to_host( int64_t value );

	static uint32_t		network_to_host( uint32_t value );
	static int32_t		network_to_host( int32_t value );

	static uint16_t		network_to_host( uint16_t value );
	static int16_t		network_to_host( int16_t value );

	// host to network
	static uint64_t		host_to_network( uint64_t value );
	static int64_t		host_to_network( int64_t value );

	static uint32_t		host_to_network( uint32_t value );
	static int32_t		host_to_network( int32_t value );

	static uint16_t		host_to_network( uint16_t value );
	static int16_t		host_to_network( int16_t value );

};

#define BYTE_ORDER ByteOrder

