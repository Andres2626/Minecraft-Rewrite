#pragma once
#include <stdint.h>
#include <string>

#ifndef NULL 
#define NULL (void*)0
#endif

typedef int mc_int;
typedef int8_t mc_i8;
typedef int16_t mc_i16;
typedef int32_t mc_i32;
typedef int64_t mc_i64;

typedef unsigned int mc_u8;
typedef uint8_t mc_uc8;
typedef uint16_t mc_u16;
typedef uint32_t mc_u32;
typedef uint64_t mc_u64;

typedef std::string mc_str;
typedef size_t mc_size;