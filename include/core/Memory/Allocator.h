#pragma once

#include "common.h"

#include <stddef.h>

namespace MC
{
    namespace Memory
    {
        class MC_API Allocator
        {
        public:
            static void *Allocate(size_t size);
            static void *AllocateDebug(size_t size, const char *file, int line);
            static void Free(void *blk);
            static void FreeDebug(void *blk, const char* file, int line);
        };
    }
}