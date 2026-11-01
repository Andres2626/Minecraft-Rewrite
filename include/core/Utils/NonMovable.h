#pragma once
#include "common.h"

namespace MC 
{
    class MC_API NonMovable
    {
    protected:
        NonMovable() = default;
        ~NonMovable() = default;
        NonMovable(NonMovable&&) = delete;
        NonMovable& operator=(NonMovable&&) = delete;
    };
}