#include <stdint.h>
#include <stddef.h>

#include "byte_order.h"
#include "header.h"

size_t Header::GetEnderSize()
{
	return 0;
}

uint32_t Header::GetNetworkEndMarker()
{
	return 0;
}
