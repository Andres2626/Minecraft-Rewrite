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
			size_t Read(FileHandle file, void* buffer, size_t size) override;
			size_t Write(FileHandle file, const void* buffer, size_t size) override;
			void Close(FileHandle file) override;
			bool Seek(FileHandle file, size_t pos) override;
			size_t Size(FileHandle file) override;
			bool Flush(FileHandle file) override;
		};
	}
}
