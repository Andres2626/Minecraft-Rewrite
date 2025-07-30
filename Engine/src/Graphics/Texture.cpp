#include "Texture.h"

#include "Utils/Util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad\glad.h>

namespace MC {
	namespace Graphics {

		bool Texture::LoadFromFile(const char* file, unsigned int mode) 
		{
			unsigned char* data;
			int width, height, nrChannels;

			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

			stbi_set_flip_vertically_on_load(true);

			data = stbi_load(file, &width, &height, &nrChannels, 0);
			if (!data) {
				MC_ERROR("Error loading texture file. %s", file);
				return false;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);

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