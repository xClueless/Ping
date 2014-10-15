#include "PingUtil.hpp"

uint32_t read_png_uint32(const char* firstByte)
{
	return ntohl(*((uint32_t*) firstByte));
}
