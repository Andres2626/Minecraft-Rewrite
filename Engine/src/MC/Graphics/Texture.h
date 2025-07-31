#pragma once
#include "..\common.h"
#include "Image.h"

namespace MC {
	namespace Graphics {

		class RD_API Texture : public Image {
		public:
			unsigned int texture;
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