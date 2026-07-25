#pragma once

#include "common.h"

namespace MC 
{
	namespace File 
	{
		class MC_API FileSystem
		{
		public:
			static u64t FileSize(const mc_str &path);
			static bool Exists(const mc_str&path);
			static void *ReadBinary(const mc_str& path, u64t &size);
			static mc_str ReadText(const mc_str &path);
			static bool WriteBinary(const mc_str& path, const void *data, u64t size);
			static bool WriteText(const mc_str& path, const mc_str &text);
		};
	}
}
