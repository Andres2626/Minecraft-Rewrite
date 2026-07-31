#pragma once

#include "common.h"

#include "IFileSystem.h"

namespace MC 
{
	namespace File 
	{
		struct MountPoint
		{
			std::unique_ptr<IFileSystem> fs;
		};

		class MC_API VirtualFileSystem
		{
		private:
			static std::unordered_map<mc_str, MountPoint> m_MountPoints;
		public:
			static bool Mount(const char *virtpath, std::unique_ptr<IFileSystem> filesystem);
			static bool Unmount(const char *mount);
		public:
			static FileHandle Open(const char *virtpath, FileMode mode);
			static size_t Read(FileHandle file, void* buffer, size_t size);
			static size_t Write(FileHandle file, const void* buffer, size_t size);
			static void Close(FileHandle file);
			static bool Exists(const char* file);
			static bool Seek(FileHandle file, size_t pos);
			static size_t Size(FileHandle file);
			static bool Flush(FileHandle file);
		public:
			static mc_str NormalizePath(const char* mount);
		};
	}
}
