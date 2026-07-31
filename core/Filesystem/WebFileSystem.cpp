#include "Filesystem/WebFileSystem.h"

#include <sys/stat.h>

#ifdef MC_PLATFORM_WEB
#include <emscripten.h>
#endif

namespace MC 
{
	namespace File 
	{

#ifdef MC_PLATFORM_WEB
        EM_ASYNC_JS(int, WebFS_Mount, (const char* path), {
            const dir = UTF8ToString(path);
            try {
                if (!FS.analyzePath(dir).exists)
                    FS.mkdirTree(dir);
                const mounted = FS.analyzePath(dir).object?.mount?.mountpoint === dir;
                if (!mounted)
                    FS.mount(IDBFS, {}, dir);
                await new Promise((resolve, reject) => {
                    FS.syncfs(true, function(err) {
                        if (err) reject(err);
                        else resolve();
                    });
                });
                return 1;
            }
            catch (e) {
                console.error("WebFS_Mount failed:", e);
                return 0;
            } 

            });

        EM_ASYNC_JS(int, WebFS_Sync, (), {
            try {
                await new Promise((resolve, reject) => {
                    FS.syncfs(false, function(err) {
                        if (err) 
                            reject(err);
                        else resolve();
                    });
                });
                return 1;
            }
             catch (e) {
                console.error("WebFS_Sync failed:", e);
                return 0;
            }
            });
    
        EM_ASYNC_JS(int, WebFS_Download,
        (const char* remote, const char* local), {

            const url  = UTF8ToString(remote);
            const path = UTF8ToString(local);
            try {
                const response = await fetch(url);
                if (!response.ok)
                    return 0;
                const buffer = await response.arrayBuffer();
                const index = path.lastIndexOf('/');
                if (index >= 0) {
                    const parent = path.substring(0, index);
                    if (parent.length > 0)
                        FS.mkdirTree(parent);
                }
                FS.writeFile(path, new Uint8Array(buffer));
                return 1;
            }
            catch (e) {
                console.error(e);
                return 0;
            }

        });

#endif /* MC_PLATFORM_WEB */  

        mc_str WebFileSystem::m_Remote;
        mc_str WebFileSystem::m_Local;

        void WebFileSystem::Init(const char* remote, const char* local)
        {
            m_Remote = remote;
            m_Local = local;
        }

        bool WebFileSystem::Mount(const char *path)
        {
#ifdef MC_PLATFORM_WEB
            return WebFS_Mount(path) != 0;
#else
            return false;
#endif /* MC_PLATFORM_WEB */  
        }

        bool WebFileSystem::Sync()
        {
#ifdef MC_PLATFORM_WEB
            return WebFS_Sync() != 0;
#else
            return false;
#endif /* MC_PLATFORM_WEB */  
        }
    
        bool WebFileSystem::Download(const char *relpath)
        {
#ifdef MC_PLATFORM_WEB
            std::string remote = m_Remote + relpath;
            std::string local = m_Local + relpath;
            return WebFS_Download(remote.c_str(), local.c_str()) != 0;
#else
            return false;
#endif /* MC_PLATFORM_WEB */  
        }

        bool WebFileSystem::CheckAndDownload(const char *relpath)
        {
        /* TODO: Implement this with VFS */
#ifdef MC_PLATFORM_WEB
            mc_str path = m_Local + relpath;
            struct stat buffer;
            if (stat(path.c_str(), &buffer) != 0)
                return Download(relpath);
            return true;
#else
            return false;
#endif /* MC_PLATFORM_WEB */  
        }
	}
}
