#pragma once
#include "common.h"

namespace MC {

	class MC_API Host {
	public:
		static void Init(int* argc, char*** argv);
		static void End();
	};

}