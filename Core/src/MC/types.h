#pragma once
#include <stdint.h>
#include <string>

#ifndef NULL 
#define NULL (void*)0
#endif

typedef int8_t rd_int8_t;
typedef int16_t rd_int16_t;
typedef int32_t rd_int32_t;
typedef int64_t rd_int64_t;

typedef uint8_t rd_uint8_t;
typedef uint16_t rd_uint16_t;
typedef uint32_t rd_uint32_t;
typedef uint64_t rd_uint64_t;

typedef std::string rd_str_t;
typedef size_t rd_size_t;