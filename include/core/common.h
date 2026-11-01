
#ifndef _COMMON_H_
#define _COMMON_H_

#define SET_BIT(x, bit) ((x) |= (bit))
#define BIT_CHK(x, bit) (((x) & (bit)) != 0)
#define BIT_L(x) (1 << (x))
#define BIT_R(x) (1 >> (x))

#if defined(_DEBUG) || defined(DEBUG)
# define MC_USE_DEBUG
#else
# define MC_USE_RELEASE
#endif /* DEBUG */

#if defined(__GNUC__)
# define MC_USE_GNU_COMPILER
#elif defined (_MSC_VER)
# define MC_USE_VS_COMPILER
#else /* !__GNUC__ / !_MSC_VER */

/* TODO: Compilation is not guaranteed in other compilers. */
#define MC_USE_UNKNOWN_COMPILER

#endif /* __GNUC__ */

#if defined(__CYGWIN__)
# define MC_PLATFORM_WINDOWS
# define MC_USE_CYGWIN
#elif defined(_WIN32)
# define MC_PLATFORM_WINDOWS
# if defined(_WIN64)
#  define MC_PLATFORM_WIN64
# else /* !_WIN64 */
#  define MC_PLATFORM_WIN32
# endif /* _WIN64 */
#elif defined (__linux__)
# define MC_PLATFORM_LINUX
#elif defined (__EMSCRIPTEN__)
# define MC_PLATFORM_WEB
#else /* !__CYGWIN__ / !_WIN32 / !__linux__ / !__EMSCRIPTEN__*/
# define MC_PLATFORM_UNSUPPORTED
# error "platform is not supported"
#endif /* __CYGWIN__ / _WIN32 / __linux__ / __EMSCRIPTEN__*/

#if defined(MC_PLATFORM_WINDOWS)
# if defined(MC_EXPORT_DLL)
#  define MC_API __declspec(dllexport)
# elif defined(MC_IMPORT_DLL)
#  define MC_API __declspec(dllimport)
# else
#  define MC_API
# endif /* MC_EXPORT_DLL */
#elif defined(MC_PLATFORM_LINUX)
# if defined(MC_EXPORT_DLL) || defined(MC_IMPORT_DLL)
#  if defined(MC_USE_GNU_COMPILER)
#   define MC_API __attribute__((visibility("default")))
#  else /* !MC_USE_GNU_COMPILER */
#   define MC_API
#  endif /* MC_USE_GNU_COMPILER */
# else /* !MC_EXPORT_DLL / !MC_IMPORT_DLL */
#  define MC_API
# endif /* MC_EXPORT_DLL / MC_IMPORT_DLL */
#else /* !MC_PLATFORM_WINDOWS / !MC_PLATFORM_LINUX */
# define MC_API
#endif /* MC_PLATFORM_WINDOWS */

/* debug break macro */
#if defined(MC_PLATFORM_WINDOWS)
# define MC_INTERNAL_BREAK_POINT __debugbreak()
#elif defined (MC_PLATFORM_LINUX)
# include <signal.h>
# define MC_INTERNAL_BREAK_POINT raise(SIGTRAP)
#else
# define MC_INTERNAL_BREAK_POINT
#endif /* MC_PLATFORM_WINDOWS */

/* semver is used for versioning. */
#define MC_VERSION_MAJOR     0
#define MC_VERSION_MINOR     8
#define MC_VERSION_PATCH     0
#define MC_VERSION_NUMBER    ((MC_VERSION_MAJOR * 10000) + (MC_VERSION_MINOR * 100) + MC_VERSION_PATCH)
#define MC_VERSION_STRING    "v0.8.0"

#include "types.h"

#ifdef MC_USE_VS_COMPILER
#pragma warning(disable:4251)
#pragma warning(disable:4996) /* _CRT_SECURE_NO_WARNINGS */
#endif

#endif /* _COMMON_H_ */
