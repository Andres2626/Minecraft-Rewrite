#include "Filesystem/NativeFileSystem.h"

#include "Utils/Util.h"

#include <filesystem>

namespace MC 
{
	namespace File 
	{
		static const char *modes[]
		{
			"rb",
			"wb",
			"rb+",
			"ab"
		};

		NativeFileSystem::NativeFileSystem(const char *root)
			: m_Root(root)
		{

		}

		NativeFileSystem::~NativeFileSystem()
		{

		}

		FileHandle NativeFileSystem::Open(const char* file, FileMode mode)
		{
			mc_assert((int)mode >= 0 && (int)mode < 4, "invalid file mode");

			const char *md = modes[(int)mode];

			std::filesystem::path path = m_Root;
			path /= file;

			FILE *fp = fopen(path.string().c_str(), md);
			if (!fp)
				return FileHandle(MC_FILE_INVALID, nullptr);

			return FileHandle(reinterpret_cast<uintptr_t>(fp), this);
		}


		size_t NativeFileSystem::Read(const FileHandle &file, void* buffer, size_t size)
		{
			FILE *fp = reinterpret_cast<FILE*>(file.handle);
			if (!fp)
				return 0;
			return fread(buffer, 1, size, fp);
		}

		size_t NativeFileSystem::Write(const FileHandle &file, const void* buffer, size_t size)
		{
			FILE *fp = reinterpret_cast<FILE*>(file.handle);
			if (!fp)
				return 0;
			return fwrite(buffer, 1, size, fp);
		}

		void NativeFileSystem::Close(const FileHandle &file)
		{
			FILE* fp = reinterpret_cast<FILE*>(file.handle);
			if (!fp)
				return;
			fclose(fp);
		}

		bool NativeFileSystem::Seek(const FileHandle &file, size_t pos)
		{
			FILE* fp = reinterpret_cast<FILE*>(file.handle);
			if (!fp)
				return false;
			return fseek(fp, (long)pos, SEEK_SET) == 0;
		}

		size_t NativeFileSystem::Size(const FileHandle &file)
		{
			FILE* fp = reinterpret_cast<FILE*>(file.handle);
			if (!fp)
				return 0;

			long current = ftell(fp);
			fseek(fp, 0, SEEK_END);
			size_t size = ftell(fp);
			fseek(fp, current, SEEK_SET);
			return size;
		}

		bool NativeFileSystem::Flush(const FileHandle &file)
		{
			FILE* fp = reinterpret_cast<FILE*>(file.handle);
			if (!fp)
				return 0;
			return fflush(fp) == 0;
		}
	}
}
