#include <loader.h>

#include "mod.h"
#include "dl.h"

#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 512

/* init and fini typedefs */
typedef void (*init)(struct mc_mod* mod, void* api);
typedef void (*fini)();

/* 
 * 'mod_path' is concatenated before the mod name so that the loader can find the module 
 * in the file system, otherwise the modules would be loaded in the same folder as the 
 * executable. 
 */
static const char *mod_path;

/* error messages */
const char *error_list[] = {
	[MC_LOADER_ERROR] = "generic error",
	[MC_LOADER_OK] = 0,
	[MC_LOADER_NOFILE] = "module not found in filesystem",
	[MC_LOADER_INV_POINTER] = "invalid path pointer",
	[MC_LOADER_NO_HEADER] = "header not found",
	[MC_LOADER_INV_MAGIC] = "invalid magic number",
	[MC_LOADER_INV_STR] = "invalid structure",
	[MC_LOADER_INV_VER] = "invalid version",
	[MC_LOADER_START] = "module must be loaded before start",
};

static char *name_to_path(const char *name, const char *path, char *ret, size_t rsize)
{
	/* check valid pointers */
	if (!name || !path)
		return NULL;

	/* leave the module path as path/module.mod */
	snprintf(ret, rsize, "%s/%s.mod", path, name);

	/* check path separators */
	for (int i = 0; ret[i] != '\0'; i++) {
#if defined(_WIN32) && !defined(__CYGWIN__) 
		/* in Windows systems '/' is not allowed */
		if (ret[i] == '/')
			ret[i] = '\\';
#else
		/* in POSIX systems '\' is not allowed */
		if (ret[i] == '\\')
			ret[i] = '/';
#endif
	}
	
	return ret;
}

static char *name_to_sym(const char *name, const char *suffix, char *sym, size_t ssize)
{
	/* check valid pointers */
	if (!name || !sym)
		return NULL;

	/* obtain symbol name */
	snprintf(sym, ssize, "mc_mod_%s_%s", name, suffix);
	return sym;
}

static int open_module(struct dl_mod *dl, const char* file)
{
	if (dl_open_module(dl, file, 0) != DL_OK)
		return MC_LOADER_ERROR; /* error loading module */

	return MC_LOADER_OK;
}

static int check_module(const char *name, struct dl_mod *dl, struct mc_mod **mod)
{
	/* check valid pointer */
	if (!name) {
		dl_close_module(dl);
		return MC_LOADER_INV_POINTER;
	}

	char buff[BUFF_SIZE];
	memset(buff, 0, BUFF_SIZE);
	name_to_sym(name, "header", buff, BUFF_SIZE);

	struct mc_mod *header = (struct mc_mod*)dl_sym_module(dl, buff);

	if (!header) {
		dl_close_module(dl);
		return MC_LOADER_NO_HEADER;
	}

	/* check magic number */
	if (header->magic != MC_MODULE_MAG) {
		dl_close_module(dl);
		return MC_LOADER_INV_MAGIC;
	}

	/* check module structure (init and fini) functions */
	name_to_sym(name, "init", buff, 512);
	init minit = (init)dl_sym_module(dl, buff);
	if (!minit) {
		dl_close_module(dl);
		return MC_LOADER_INV_STR;
	}

	name_to_sym(name, "fini", buff, 512);
	fini mfini = (fini)dl_sym_module(dl, buff);
	if (!mfini) {
		dl_close_module(dl);
		return MC_LOADER_INV_STR; 
	}

	/* check if module version is valid */
	if (header->version > 1) {
		dl_close_module(dl);
		return MC_LOADER_INV_VER;
	}

	header->init = minit;
	header->fini = mfini;
	header->mver = 1;
	*mod = header;

	return MC_LOADER_OK;
}

void mc_set_module_path(const char *path)
{
	/* check valid pointer */
	if (!path)
		return; 

	mod_path = path;
}

const char *mc_get_module_path()
{
	return mod_path;
}

int mc_open_module(struct dl_mod *dl, struct mc_mod **mod, const char *name)
{
	if (!mod_path)
		return MC_LOADER_INV_POINTER;

	char buff[BUFF_SIZE];
	memset(buff, 0, BUFF_SIZE);

	name_to_path(name, mod_path, buff, BUFF_SIZE);

	if (open_module(dl, buff) != MC_LOADER_OK)
		return MC_LOADER_NOFILE;

	return check_module(name, dl, mod);
}

const char* mc_get_module_error(int error)
{
	return error_list[error];
}

int mc_load_module(struct mc_mod *mod, void *api)
{
	/* check if the module is loaded */
	if (!mod)
		return MC_LOADER_START;

	mod->init(mod, api);
	return MC_LOADER_OK;
}

int mc_unload_module(struct dl_mod *dl, struct mc_mod *mod)
{
	/* check if the module is loaded */
	if (!mod)
		return MC_LOADER_START;

	/* call to fini function */
	mod->fini();

	/* close module */
	dl_close_module(dl);
	return MC_LOADER_OK;
}
