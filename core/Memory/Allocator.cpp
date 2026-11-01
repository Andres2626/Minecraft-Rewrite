#include "mc.h"

#include "Memory/Allocator.h"

#include "Log/Log.h"

namespace MC
{
    namespace Memory
    {
        void *Allocator::Allocate(size_t size)
        {
            if (size == 0) {
                size = 1;
            }

            void* ptr = std::malloc(size);
            if (!ptr)
                throw std::bad_alloc();

            return ptr;
        }

        void *Allocator::AllocateDebug(size_t size, const char* file, int line)
        {
            mc_warn("Allocate in {}:{}", file, line);
            return Allocate(size);
        }

        void Allocator::Free(void *blk)
        {
            if (!blk)
                return;

            std::free(blk);
        }

        void Allocator::FreeDebug(void *blk, const char* file, int line)
        {
            mc_warn("Free in {}:{}", file, line);
            Free(blk);
        }
    }
}