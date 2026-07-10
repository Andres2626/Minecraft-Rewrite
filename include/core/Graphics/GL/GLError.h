#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{
		namespace GL 
		{
			class Error 
			{
			public:
				static u32t GetError();
				static mc_str GetErrorStr(u32t num);
			};
		}
	}
}