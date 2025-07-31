#pragma once
#include "common.h"

/* Initialize console subsystem */
void mc_console_init();

/* Finalize console subsystem */
void mc_console_fini();

void mc_console_set_title(const char* title);