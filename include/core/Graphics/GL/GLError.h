#pragma once
#include "common.h"

#define GL_CHECK() \
    do { \
        GLenum err = MC::Graphics::GL::Error::GetError(); \
        if (err != GL_NO_ERROR) \
            mc_error("OpenGL error: 0x{} at {}:{}", err, __FILE__, __LINE__); \
    } while (0)

namespace MC 
{
	namespace Graphics 
	{
		namespace GL 
		{
			class MC_API Error 
			{
			public:
				static u32t GetError();
				static const char *GetErrorStr(u32t num);
			};
		}
	}
}