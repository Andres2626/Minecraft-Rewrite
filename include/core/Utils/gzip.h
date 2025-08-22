#pragma once
#include "common.h"

namespace MC 
{
	namespace Utils 
	{
		MC_API unsigned char* decompress(const void* in, size_t in_size, size_t* out);
		MC_API unsigned char* compress(const void* input, size_t in_size, size_t* out);
	}
}