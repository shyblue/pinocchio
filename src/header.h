#pragma once

#ifndef _HEADERBASE_H_
#define  _HEADERBASE_H_

#include "common.h"
class Header
{
public:
	Header(void) : m_sizeOfHeader(0) {}; 
	virtual ~Header(void) {};
	virtual size_t GetHeaderSize() = 0;
	virtual size_t GetBodySize() = 0;
	virtual size_t GetEnderSize() = 0;
	virtual size_t GetTotalSize(void) = 0;
	virtual size_t Serialize(char* buffer, size_t buffersize) = 0;
	virtual size_t Deserialize(char* buffer, size_t buffersize) = 0;
	virtual bool Validate() = 0;
	virtual bool CheckEndmarker(char* buffer, size_t length) = 0;
	virtual void SetHeaderSize(size_t size) = 0;
	virtual void SetDataSize(size_t size) = 0;

	virtual void HostToNetwork()  {};
	virtual void NetworkToHost()  {};

	virtual uint32_t GetNetworkEndMarker();
	virtual uint16_t GetProtocolNo() = 0;

	size_t			m_sizeOfHeader;
	size_t			m_sizeOfEnder;
};

#endif // _HEADER_H_
