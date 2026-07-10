#include "Graphics/Texture.h"

#include <Log/Log.h>

#include <stb_image.h>
#include <gfx/glad.h>

namespace MC 
{
	namespace Graphics 
	{
		bool Texture::LoadFromFile(const char *file, u32t mode) 
		{
			const char *errmsg = {};
			if (!Image::LoadFromFile(file, errmsg)) {
				err.SetError(ErrorType::AssetLoad);
				mc_error("error loading image. internal error {}", errmsg);
				return false;
			}
				
			u32t format = 0;
			switch (nr_channels)
			{
			case 1: 
				format = GL_RED;  
				break;
			case 3: 
				format = GL_RGB;  
				break;
			case 4: 
				format = GL_RGBA; 
				break;
			default:
				mc_fatal("unsupported image channel format: {}", nr_channels);
				err.SetError(ErrorType::Noimp);
				return false;
			}

			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

			glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			Free();

			mc_info("texture loaded: path={} x={} y={} mode={:x} format={:x}", path, x, y, static_cast<unsigned>(mode), static_cast<unsigned>(format));

			return true;
		}

		void Texture::Bind(u32t id) 
		{
			glActiveTexture(GL_TEXTURE0 + id);
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}

		void Texture::Unbind() 
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}