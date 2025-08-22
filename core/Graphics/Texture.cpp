#include "Graphics/Texture.h"

#include <stb_image.h>
#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{

		bool Texture::LoadFromFile(const char* file, unsigned int mode) 
		{
			SetFlip(true);

			if (!Image::LoadFromFile(file)) 
				return false;

			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			Free();
			return true;
		}

		void Texture::Bind() 
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}

		void Texture::Unbind() 
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}