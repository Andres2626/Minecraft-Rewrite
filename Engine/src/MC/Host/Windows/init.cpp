#include "..\init.h"
#include "..\console.h"

#include <string.h>
#include <locale>

void mc_host_init(int argc, char **argv) 
{
	mc_console_init();

	setlocale(LC_ALL, "");
}

void mc_host_fini() 
{
	mc_console_fini();
}
