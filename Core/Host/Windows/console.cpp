#include "Host/console.h"

#include <Windows.h>

namespace MC 
{

	void Console::Init() 
	{
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
		SetDefaultFont();
	}

	void Console::End()
	{
		/* not implemented */
	}

	void Console::SetTitle(const char* title)
	{
		SetConsoleTitle((LPCSTR)title);
	}

	void Console::SetTextColor(int bG, int fG)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (bG << 4) | fG);
	}

	void Console::ResetTextColor()
	{
		SetTextColor(MC_CONSOLE_BLACK, MC_CONSOLE_INTENSITY);
	}

	void Console::SetDefaultFont()
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

}
