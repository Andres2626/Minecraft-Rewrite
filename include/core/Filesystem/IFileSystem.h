#pragma once

#include "common.h"

#define MC_FILE_INVALID 0

namespace MC
{
	namespace File
	{
		class IFileSystem;

		struct MC_API FileHandle
		{
			uintptr_t handle;
			IFileSystem *imp;

			FileHandle(uintptr_t h, IFileSystem *i)
				: handle(h), imp(i)
			{ }
		};
        
		enum class FileMode
		{
			Read, 
			Write,
			ReadWrite,
			Append
		};

		class MC_API IFileSystem
		{
		public:
			virtual ~IFileSystem() = default;
		public:
			virtual FileHandle Open(const char *file, FileMode mode) = 0;
			virtual size_t Read(FileHandle file, void *buffer, size_t size) = 0;
			virtual size_t Write(FileHandle file, const void* buffer, size_t size) = 0;
			virtual void Close(FileHandle file) = 0;
			virtual bool Seek(FileHandle file, size_t pos) = 0;
			virtual size_t Size(FileHandle file) = 0;
			virtual bool Flush(FileHandle file) = 0;
		};
	}
}