#pragma once

#include "common.h"

#include "IFileSystem.h"

namespace MC 
{
	namespace File 
	{
		class MC_API VirtualFileSystem
		{
		public:
			static bool Mount(const char *virtpath, IFileSystem *filesystem);
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
		};
	}
}
