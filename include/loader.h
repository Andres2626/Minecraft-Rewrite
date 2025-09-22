#ifndef _LOADER_H
#define _LOADER_H

#include "errors.h"

void mc_set_module_path(const char *path);
const char* mc_get_module_path();
int mc_open_module(struct dl_mod *dl, struct mc_mod **mod, const char *name);
const char *mc_get_module_error(int error);
int mc_load_module(struct mc_mod *mod, void* api);
int mc_unload_module(struct dl_mod *dl, struct mc_mod *mod);

#endif /* !_LOADER_H */
