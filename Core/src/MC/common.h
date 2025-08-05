
#ifndef _COMMON_H_
#define _COMMON_H_

#define BIT_L(x) 1 << x
#define BIT_R(x) 1 >> x

#if defined (DEBUG)
#define MC_USE_DEBUG
#else
#define MC_USE_RELEASE
#endif

#if defined(_WIN32) || defined(MC_USE_WINDOWS32)
#if defined(__CYGWIN__)
#define MC_USE_CYGWIN
#endif
#define MC_PLATFORM_WINDOWS
#if defined(_WIN64) || defined (MC_MACHINE64)
#define MC_PLATFORM_WIN32
#else
#define MC_PLATFORM_WIN64
#endif
#else
#define MC_PLATFORM_UNSUPPORTED
#error "Platform is not supported"
#endif

#if defined (MC_PLATFORM_WINDOWS)
#if defined (MC_EXPORT_DLL)
#define MC_API __declspec(dllexport)
#else
#if defined (MC_EXPORT_LIB)
#define MC_API
#else 
#define MC_API __declspec(dllimport)
#endif
#endif
#endif

#define MC_VERSION_MAJOR     0
#define MC_VERSION_MINOR     0
#define MC_VERSION_PATH      1
#define MC_VERSION_NUMBER    ((MC_VERSION_MAJOR * 10000) + (MC_VERSION_MINOR * 100) + MC_VERSION_PATH)
#define MC_VERSION_STRING    "pa.0.0.1"
#define _USE_MATH_DEFINES 1

#include <fstream>
#include <iostream>
#include <locale>
#include <math.h>
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
