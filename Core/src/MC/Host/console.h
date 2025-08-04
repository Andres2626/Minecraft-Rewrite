#pragma once
#include "common.h"

#define MC_CONSOLE_BLACK 0
#define MC_CONSOLE_BLUE 1
#define MC_CONSOLE_GREEN 2
#define MC_CONSOLE_CYAN 3
#define MC_CONSOLE_RED 4
#define MC_CONSOLE_MAG 5
#define MC_CONSOLE_YELLOW 6
#define MC_CONSOLE_INTENSITY 7

namespace MC 
{

	class MC_API Console 
	{
	public:
		static void Init();
		static void End();
		static void SetTitle(const char* title);
		static void SetDefaultFont();
		static void SetTextColor(int bG, int fG);
		static void ResetTextColor();
	};

}