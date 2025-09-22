#ifndef _ERRORS_H
#define _ERRORS_H

#define MC_LOADER_OK               1 /* no error */

#define MC_LOADER_ERROR            0 /* generic error */

/* module loader errors */
#define MC_LOADER_NOFILE           2 /* module not found in filesystem */
#define MC_LOADER_INV_POINTER      3 /* invalid path pointer */
#define MC_LOADER_NO_HEADER        4 /* module header not found */
#define MC_LOADER_INV_MAGIC        5 /* invalid magic number */
#define MC_LOADER_INV_STR          6 /* invalid structure */
#define MC_LOADER_INV_VER          7 /* invalid version */
#define MC_LOADER_START            8 /* module must be loaded before start */

#endif /* !_ERRORS_H */

