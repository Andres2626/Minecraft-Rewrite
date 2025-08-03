#include "Texture.h"

#include "Log/Log.h"

#include <stb_image.h>
#include <glad\glad.h>

namespace MC {
	namespace Graphics {

		bool Texture::LoadFromFile(const char* file, unsigned int mode) 
		{
			this->SetFlip(true);

			if (!Image::LoadFromFile(file)) 
				return false;

			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->x, this->y, 0, GL_RGB, GL_UNSIGNED_BYTE, this->pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			this->Free();
			return true;
		}

		void Texture::Bind() 
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);
		}

		void Texture::Unbind() 
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}