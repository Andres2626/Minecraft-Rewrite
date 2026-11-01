#pragma once

#include "Allocator.h"

#if defined (MC_USE_VS_COMPILER)
#pragma warning(disable:4595)
#endif /* MC_COMPILER_VS */

inline void *operator new(size_t size)
{
	return MC::Memory::Allocator::Allocate(size);
}

inline void *operator new(size_t size, const char *file, int line)
{
	return MC::Memory::Allocator::AllocateDebug(size, file, line);
}

inline void *operator new[](size_t size)
{
	return MC::Memory::Allocator::Allocate(size);
}

inline void *operator new[](size_t size, const char* file, int line)
{
	return MC::Memory::Allocator::AllocateDebug(size, file, line);
}

inline void operator delete(void *blk)
{
	return MC::Memory::Allocator::Free(blk);
}

inline void operator delete(void *blk, const char* file, int line)
{
	return MC::Memory::Allocator::FreeDebug(blk, file, line);
}

inline void operator delete[](void *blk)
{
	return MC::Memory::Allocator::Free(blk);
}

inline void operator delete[](void *blk, const char* file, int line)
{
	return MC::Memory::Allocator::FreeDebug(blk, file, line);
}

#if defined (MC_USE_VS_COMPILER)
#pragma warning(default:4595)
#endif

#if defined (_DEBUG)
#define mcnew new(__FILE__, __LINE__)
#define mcdel delete
#else
#define mcnew new
#define mcdel delete
#endif
