#pragma once
#include "common.h"

#define RD_CONSOLE_BLACK 0
#define RD_CONSOLE_BLUE 1
#define RD_CONSOLE_GREEN 2
#define RD_CONSOLE_CYAN 3
#define RD_CONSOLE_RED 4
#define RD_CONSOLE_MAG 5
#define RD_CONSOLE_YELLOW 6
#define RD_CONSOLE_INTENSITY 7

namespace MC {

	class RD_API Console {
	public:
		static void Init();
		static void End();
		static void SetTitle(const char* title);
		static void SetDefaultFont();
		static void SetTextColor(int bG, int fG);
		static void ResetTextColor();
	};

}