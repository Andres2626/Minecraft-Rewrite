#include "Graphics/Image.h"
#include "Utils/Util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace MC 
{
	namespace Graphics
	{

		Image::Image()
			: x(0), y(0), nr_channels(0), pixels(0), path(0)
		{

		}

		Image::~Image()
		{

		}

		void Image::SetFlip(bool flip)
		{
			stbi_set_flip_vertically_on_load(flip);
		}

		bool Image::LoadFromFile(const char* path, const char* errmsg)
		{
			this->path = path;
			pixels = stbi_load(path, &x, &y, &nr_channels, 0);
			if (!pixels) {
				errmsg = stbi_failure_reason();
				return false;
			}

			errmsg = NULL;
			return true;
		}

		void Image::Free()
		{
			stbi_image_free(pixels);
		}
	}
}