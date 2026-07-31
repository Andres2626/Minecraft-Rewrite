#pragma once

#include "common.h"

namespace MC 
{
	namespace File 
	{
		class MC_API WebFileSystem
		{
		private:
			static mc_str m_Remote;
			static mc_str m_Local;
		public:
			static void Init(const char *remote, const char *local);
			static bool Mount(const char *path);
			static bool Sync();
            static bool Download(const char *relpath);
		public:
			static bool CheckAndDownload(const char *relpath);
		};
	}
}
