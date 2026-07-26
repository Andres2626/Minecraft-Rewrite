#include "Filesystem/FileSystem.h"

namespace MC 
{
	namespace File 
	{
		u64t FileSystem::FileSize(const mc_str& path)
		{
			std::ifstream file(path, std::ios::binary);

			if (!file)
				return 0;

			file.seekg(0, std::ios::end);
			return file.tellg();
		}

		bool FileSystem::Exists(const mc_str &path)
		{
			std::ifstream file(path);
			return file.good();
		}

		void *FileSystem::ReadBinary(const mc_str &path, size_t &size)
		{
			std::ifstream file(path, std::ios::binary);

			if (!file)
				return nullptr;

			/* get size */
			file.seekg(0, std::ios::end);
			size = file.tellg();

			file.seekg(0);

			void* buffer = malloc(size);
			if (!buffer)
				return nullptr;

			if (!file.read((char*)buffer, size)) {
				free(buffer);
				return nullptr;
			}

			return buffer;
		}

		mc_str FileSystem::ReadText(const mc_str &path)
		{
			std::ifstream file(path);

			if (!file)
				return {};

			return std::string(std::istreambuf_iterator<char>(file),
							   std::istreambuf_iterator<char>());
		}

		bool FileSystem::WriteBinary(const mc_str &path, const void *data, size_t size)
		{
			std::ofstream file(path, std::ios::binary);

			if (!file)
				return false;

			if (!file.write((const char*)data, size))
				return false;

			return true;
		}

		bool FileSystem::WriteText(const mc_str &path, const mc_str &text)
		{
			std::ofstream file(path);

			if (!file)
				return false;

			file << text;

			return true;
		}
	}
}
