#pragma once

#include "common.h"

#include "IFileSystem.h"

namespace MC 
{
	namespace File 
	{
		class MC_API NativeFileSystem : public IFileSystem
		{
		private:
			const char *m_Root;
		public:
			NativeFileSystem(const char *root);
			~NativeFileSystem();
		public:
			FileHandle Open(const char* file, FileMode mode) override;
			size_t Read(const FileHandle &file, void* buffer, size_t size) override;
			size_t Write(const FileHandle &file, const void* buffer, size_t size) override;
			void Close(const FileHandle &file) override;
			bool Seek(const FileHandle &file, size_t pos) override;
			size_t Size(const FileHandle &file) override;
			bool Flush(const FileHandle &file) override;
		};
	}
}
