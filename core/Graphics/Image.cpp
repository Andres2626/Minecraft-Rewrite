#include "Graphics/Image.h"
#include "Utils/Util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Filesystem/VirtualFileSystem.h>

namespace MC 
{
	namespace Graphics
	{

		Image::Image()
			: x(0), y(0), nr_channels(0), pixels(nullptr), path(nullptr)
		{

		}

		Image::~Image()
		{

		}

		void Image::SetFlip(bool flip)
		{
			stbi_set_flip_vertically_on_load(flip);
		}

		bool Image::LoadFromFile(const char *virtpath, const char **errmsg)
		{
			this->path = virtpath;

			File::FileHandle fp = File::VirtualFileSystem::Open(virtpath, File::FileMode::Read);
			if (fp.handle == MC_FILE_INVALID) {
				*errmsg = "error opening image file";
				return false;
			}

			pixels = stbi_load_from_file(reinterpret_cast<FILE*>(fp.handle), &x, &y, &nr_channels, 0);
			File::VirtualFileSystem::Close(fp);

			if (!pixels) {
				*errmsg = stbi_failure_reason();
				return false;
			}

			*errmsg = NULL;
			return true;
		}

		void Image::Free()
		{
			stbi_image_free(pixels);
		}
	}
}