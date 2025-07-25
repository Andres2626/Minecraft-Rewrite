#pragma once

#include "..\common.h"

namespace MC {
	namespace Graphics {

		class Texture {
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