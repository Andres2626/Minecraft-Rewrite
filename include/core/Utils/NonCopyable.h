#pragma once
#include "common.h"

namespace MC 
{
    class MC_API NonCopyable
    {
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}