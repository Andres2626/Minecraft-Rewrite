#pragma once
#include "common.h"

#include "Image.h"

#include "Utils/Error.h"

namespace MC 
{
	namespace Graphics 
	{

		class MC_API Texture : public Image 
		{
		private:
			ErrorHandler err;
		public:		
			u32t m_ID;
		public:
			Texture() = default;
			~Texture() = default;
		public:
			bool LoadFromFile(const char *file, u32t mode);
			void Bind(u32t id);
			void Unbind();
		public:
			Error &GetError() { return err.GetError(); }
		};

	}
}