#pragma once
#include "common.h"

namespace MC {

	class RD_API Host {
	public:
		static void Init(int argc, char** argv);
		static void End();
	};

}