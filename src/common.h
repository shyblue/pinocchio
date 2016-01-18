#pragma once

#ifndef __COMMON_H_
#define __COMMON_H_

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

#endif // __COMMON_H_