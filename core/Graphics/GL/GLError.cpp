#include "Graphics/GL/GLError.h"

#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{
		namespace GL
		{
			u32t Error::GetError()
			{
				return glGetError();
			}

			mc_str Error::GetErrorStr(u32t num)
			{
				mc_str ret;
				switch (num) {
				case GL_INVALID_ENUM:                  
					ret = "Invalid enum"; 
					break;
				case GL_INVALID_VALUE:                 
					ret = "Invalid value"; 
					break;
				case GL_INVALID_OPERATION:             
					ret = "Invalid operation"; 
					break;
				case GL_STACK_OVERFLOW:                
					ret = "Stack overflow"; 
					break;
				case GL_STACK_UNDERFLOW:               
					ret = "Stack underflow"; 
					break;
				case GL_OUT_OF_MEMORY:                 
					ret = "Out of memory"; 
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: 
					ret = "Invalid framebuffer operation"; 
					break;
				}

				return ret;
			}
		}
    }
}


