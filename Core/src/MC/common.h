
#ifndef _COMMON_H_
#define _COMMON_H_

#define BIT_L(x) 1 << x
#define BIT_R(x) 1 >> x

#if defined(_WIN32) || defined(RD_USE_WINDOWS32)
#if defined(__CYGWIN__)
#define RD_USE_CYGWIN
#endif
#define RD_PLATFORM_WINDOWS
#if defined(_WIN64) || defined (RD_MACHINE64)
#define RD_PLATFORM_WIN32
#else
#define RD_PLATFORM_WIN64
#endif
#else
#define RD_PLATFORM_UNUSED
#error "Platform is not supported"
#endif

#if defined (RD_PLATFORM_WINDOWS)
#if defined (RD_EXPORT_DLL)
#define RD_API __declspec(dllexport)
#else
#if defined (RD_EXPORT_LIB)
#define RD_API
#else 
#define RD_API __declspec(dllimport)
#endif
#endif
#endif

#define RD_VERSION_MAJOR     0
#define RD_VERSION_MINOR     0
#define RD_VERSION_PATH      1
#define RD_VERSION_NUMBER    ((RD_VERSION_MAJOR * 10000) + (RD_VERSION_MINOR * 100) + RD_VERSION_PATH)
#define RD_VERSION_STRING    "pa.0.0.1"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types.h"

#endif
