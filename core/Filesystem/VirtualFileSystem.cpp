#include "Filesystem/VirtualFileSystem.h"

namespace MC
{
    namespace File
    {
        std::unordered_map<mc_str, MountPoint> VirtualFileSystem::m_MountPoints;
        bool VirtualFileSystem::Mount(const char* mount, std::unique_ptr<IFileSystem> filesystem)
        {
            std::string vp = mount;
            /* Normalize path separator */
            std::replace(vp.begin(), vp.end(), '\\', '/');
            /* ensure '/' at the end */
            if (vp.empty() || vp.back() != '/')
                vp += '/';

            auto [it, inserted] = m_MountPoints.try_emplace(vp, MountPoint{ std::move(filesystem) });
            return inserted;
        }

        bool VirtualFileSystem::Unmount(const char* mount)
        {
            return m_MountPoints.erase(mount) != 0;
        }

        FileHandle VirtualFileSystem::Open(const char* virpath, FileMode mode)
        {
            mc_str vp = virpath;
            /* Normalize path separator */
            std::replace(vp.begin(), vp.end(), '\\', '/');
            FileHandle ret(MC_FILE_INVALID, nullptr);
            IFileSystem* fs = nullptr;
            char target = '/';
            size_t pos = 0, mountEnd = 0;
            mc_str substr;
            while ((pos = vp.find(target, pos)) != std::string::npos) {
                mc_str prefix = vp.substr(0, pos + 1);
                auto it = m_MountPoints.find(prefix);
                if (it != m_MountPoints.end()) {
                    fs = it->second.fs.get();
                    mountEnd = pos + 1;
                }
                pos++;
            }
            if (!fs)
                return ret;
            mc_str relpath = vp.substr(mountEnd);
            ret = fs->Open(relpath.c_str(), mode);
            return ret;
        }

        size_t VirtualFileSystem::Read(FileHandle file, void* buffer, size_t size)
        {
            IFileSystem* fs = file.imp;
            if (!fs)
                return 0;
            return fs->Read(file, buffer, size);
        }

        size_t VirtualFileSystem::Write(FileHandle file, const void* buffer, size_t size)
        {
            IFileSystem* fs = file.imp;
            if (!fs)
                return 0;
            return fs->Write(file, buffer, size);
        }

        void VirtualFileSystem::Close(FileHandle file)
        {
            IFileSystem* fs = file.imp;
            if (!fs)
                return;
            fs->Close(file);
        }

        bool VirtualFileSystem::Exists(const char* file)
        {
            FileHandle fp = Open(file, FileMode::Read);
            if (fp.handle == MC_FILE_INVALID)
                return false;
            Close(fp);
            return true;
        }

        bool VirtualFileSystem::Seek(FileHandle file, size_t pos)
        {
            IFileSystem* fs = file.imp;
            if (!fs)
                return false;
            return fs->Seek(file, pos);
        }

        size_t VirtualFileSystem::Size(FileHandle file)
        {
            IFileSystem* fs = file.imp;
            if (!fs)
                return 0;
            return fs->Size(file);
        }

        bool VirtualFileSystem::Flush(FileHandle file)
        {
            IFileSystem* fs = file.imp;
            if (!fs)
                return false;
            return fs->Flush(file);
        }
    }
}