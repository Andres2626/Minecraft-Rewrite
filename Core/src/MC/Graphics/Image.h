#pragma once
#include "common.h"

namespace MC 
{
	namespace Graphics 
	{

		struct MC_API Image 
		{
		public:
			const char* path;
			unsigned char* pixels;
			int x;
			int y;
			int nr_channels;
		public:
			Image();
			~Image();
		public:
			void SetFlip(bool flip);
			bool LoadFromFile(const char* path);
			void Free();
		};

	}
}