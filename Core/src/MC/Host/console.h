#pragma once
#include "common.h"

/* Initialize console subsystem */
void RD_API mc_console_init();

/* Finalize console subsystem */
void RD_API mc_console_fini();

void RD_API mc_console_set_title(const char* title);