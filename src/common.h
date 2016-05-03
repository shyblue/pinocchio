#pragma once

#ifdef _WIN32
#include <cstdint>
#include <WinSock2.h>
#define atoll(str) _atoi64(str)
#define SNPRINTF	sprintf_s
#endif // _WIN32

#ifdef _UNICODE
#define CHARSIZE sizeof(wchar_t)
#else
#define CHARSIZE sizeof(char)
#endif //_UNICODE

#define NULLPTR nullptr

#define BYTEUNIT 0
#define KBYTEUNIT 10
#define MBYTEUNIT 20
#define GBYTEUNIT 30

template< typename T >
class at_block_exit_finalizer_holder
{
	at_block_exit_finalizer_holder( const T& fn ) : m_finalizer( &fn ) {}
	at_block_exit_finalizer_holder( const at_block_exit_finalizer_holder& a ) : m_finalizer( a.m_finalizer ) { a.m_finalizer = nullptr; }
	~at_block_exit_finalizer_holder() { if ( m_finalizer ) (*m_finalizer)(); }
private:
	void operator = ( const at_block_exit_finalizer_holder& );
	mutable const T* m_finalizer;
};

template< typename T > static inline at_block_exit_finalizer_holder<T> at_block_exit( const T& fn )
{
	return at_block_exit_finalizer_holder<T>(fn);
}

#define AFC_FINALLY( ... ) auto _at_block_exit_##__FUNCTION__##__LINE__ = at_block_exit( [&]() __VA_ARGS__ )
