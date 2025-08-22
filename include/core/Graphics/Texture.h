#pragma once
#include "common.h"

#include "Image.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API Texture : public Image {
		public:
			mc_u8 m_ID;
		public:
			Texture() = default;
			~Texture() = default;
		public:
			bool LoadFromFile(const char* file, unsigned int mode);
			void Bind();
			void Unbind();
		};

	}
}