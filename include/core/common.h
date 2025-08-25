
#ifndef _COMMON_H_
#define _COMMON_H_

#define BIT_L(x) 1 << x
#define BIT_R(x) 1 >> x

#if defined (DEBUG)
#define MC_USE_DEBUG
#else
#define MC_USE_RELEASE
#endif

#if defined (__GNUC__)
#define MC_USE_GNU_COMPILER
#elif defined (_MSC_VER)
#define MC_USE_VS_COMPILER
#else
#define MC_USE_UNKNOUN_COMPILER
#endif 

#if defined (__CYGWIN__)
#define MC_PLATFORM_WINDOWS
#define MC_USE_CYGWIN
#elif defined(_WIN32) || defined(MC_USE_WINDOWS32)
#define MC_PLATFORM_WINDOWS
#if defined(_WIN64) || defined (MC_MACHINE64)
#define MC_PLATFORM_WIN32
#else
#define MC_PLATFORM_WIN64
#endif
#elif defined (__linux__)
#define MC_PLATFORM_LINUX
#else
#define MC_PLATFORM_UNSUPPORTED
#error "Platform is not supported"
#endif

#if defined(MC_PLATFORM_WINDOWS)
#if defined(MC_EXPORT_DLL)
#define MC_API __declspec(dllexport)
#elif defined(MC_IMPORT_DLL)
#define MC_API __declspec(dllimport)
#else
#define MC_API
#endif
#elif defined(MC_PLATFORM_LINUX)
#if defined(MC_EXPORT_DLL) || defined(MC_IMPORT_DLL)
#if defined(MC_USE_GNU_COMPILER)
#define MC_API __attribute__((visibility("default")))
#else
#define MC_API
#endif
#else
#define MC_API
#endif
#else
#define MC_API
#endif

/* debug break macro */
#if defined (MC_PLATFORM_WINDOWS)
#define MC_INTERNAL_BREAK_POINT __debugbreak()
#elif defined (MC_PLATFORM_LINUX)
#include <signal.h>
#define MC_INTERNAL_BREAK_POINT raise(SIGTRAP)
#endif

#define MC_VERSION_MAJOR     0
#define MC_VERSION_MINOR     0
#define MC_VERSION_PATCH     2
#define MC_VERSION_NUMBER    ((MC_VERSION_MAJOR * 10000) + (MC_VERSION_MINOR * 100) + MC_VERSION_PATCH)
#define MC_VERSION_STRING    "pa.0.0.2"
#define _USE_MATH_DEFINES 1

#include <fstream>
#include <iostream>
#include <locale>
#include <math.h>
#include <memory>
#include <map>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types.h"

#endif
