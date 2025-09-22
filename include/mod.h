/* MC module system boot protocol */

#ifndef _MOD_H
#define _MOD_H

#include <stdint.h>

/* integer types */
typedef signed char mod_i8;
typedef short mod_i16;
typedef int mod_i32;
typedef long long mod_i64;

/* unsigned types */
typedef unsigned char mod_u8;
typedef unsigned short mod_u16;
typedef unsigned int mod_u32;
typedef unsigned long long mod_u64;

/* misc types */
typedef int mod_ver; /* version type */

/*
 * The MC module loader detects a magic number in the module to be able
 * to start it, in this case it is 'MMOD' or 0x444F4D4D
 */
#define MC_MODULE_MAG 0x444F4D4D /* 0x4D, 0x4d, 0x4f, 0x44 */

/* MAGIC values */
#define MC_MODULE_MAG_SIZE 4   /* magic number size */
#define MC_MODULE_MAGO 0x44    /* 'M' */
#define MC_MODULE_MAG1 0x4d    /* 'M' */
#define MC_MODULE_MAG2 0x4f    /* 'O' */
#define MC_MODULE_MAG3 0x44    /* 'D' */

struct mc_mod {
	mod_u32 magic;           /* must be 0x444F4D4D */
	char* name;              /* module name */
	char* author;            /* module author */
	mod_ver version;         /* MC engine version */
	mod_ver mver;            /* module version */

	/* module required functions */
	void (*init)(struct mc_mod* mod, void* api);
	void (*fini)();
};

#define MC_MOD_HEADER_SIZE sizeof(struct mc_mod)

/*
 * The '_PUBLIC' definition is compiler-dependent, so the goal is to leave functions
 * compiled using C++ with the same naming convention as in C, to avoid errors when
 * loading symbols by the DL library.
 */
#ifdef __cplusplus
#define _PUBLIC extern "C"
#else
#define _PUBLIC
#endif

/*
 * '_EXPORT_DL' definition depends on the host compiler. On Windows compilers, __declspec(dllexport)
 * is used to export a function, whereas on POSIX systems, __attribute__((visibility("default")))
 * is used. However, this is not universal across all platforms, so check carefully before exporting
 * a function, as the library should, and only should, detect that the function was exported for
 * DL; otherwise, the function will not be called and the library will generate an error.
 */
#if defined(_WIN32) && !defined(__CYGWIN__)
/*
 * CYGWIN system are considered POSIX environment, for this reason the usual definition for
 * exporting symbols as is the case with pure _WIN32 systems (mingw32, VS, ...) is not used.
 */
#define _EXPORT_DL __declspec(dllexport)
#else
#define _EXPORT_DL __attribute__((visibility("default"))) /* FIXME: Not guaranteed on all compilers */
#endif

#define _FUNCTION _PUBLIC _EXPORT_DL
#define _VARIABLE _PUBLIC _EXPORT_DL

/*
 * 'INIT' and 'FINI' macros are used to define the entry point of the module and what
 * the loader does is call these functions to handle the module.
 */
#ifdef __cplusplus
#define MC_MOD_INIT(name) _FUNCTION void mc_mod_##name##_init(struct mc_mod *mod, void *api)
#define MC_MOD_FINI(name) _FUNCTION void mc_mod_##name##_fini(void)
#else
#define MC_MOD_INIT(name) _FUNCTION void mc_mod_##name##_init(struct mc_mod *mod, void *api)
#define MC_MOD_FINI(name) _FUNCTION void mc_mod_##name##_fini(void)
#endif


   /* Module loader detect this to know if you are working with a valid module.  */
#define MC_MOD_REGISTER(n, v, a) \
_VARIABLE struct mc_mod mc_mod_##n##_header = { \
	.magic = MC_MODULE_MAG, \
	.name = #n, \
	.author = a, \
	.version = v  \
}

#endif /* !_MOD_H */
