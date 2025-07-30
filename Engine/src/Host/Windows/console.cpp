#include "..\console.h"


#include <Windows.h>

static void set_default_font() 
{
	CONSOLE_FONT_INFOEX cfi;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 12;
	cfi.dwFontSize.X = 7;
	cfi.dwFontSize.Y = 12;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(h, FALSE, &cfi);
}

void mc_console_init() 
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	set_default_font();
}

void mc_console_fini() 
{
	/* not implemented */
}

void mc_console_set_title(const char *title) 
{
	SetConsoleTitle((LPCSTR)title);
}
