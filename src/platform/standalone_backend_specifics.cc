#include <platform/standalone_backend_specifics.h>
#include <platform/file.h>
#include <GLFW/glfw3.h>
#ifdef _WIN32
#include <shlobj.h>
#include <windows.h>
#include <shlwapi.h>
#include <locale>
#include <codecvt>
#else
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace openrayman
{
    standalone_backend_specifics::standalone_backend_specifics()
    {
        initialize_data_path();
        initialize_storage_path();
        if(!file::exists(get_storage_path()))
            file::create_directory(get_storage_path());
    }

    void standalone_backend_specifics::initialize_data_path()
    {
        std::string path;
#if _WIN32
        char str[MAX_PATH];
        int result = GetModuleFileName(nullptr, str, MAX_PATH);
        if(result == 0)
        {
            m_data_path = new std::string(".\\data");
            return;
        }
        PathRemoveFileSpec(str);
        path = std::string(str);
#else
        char destination[PATH_MAX];
        size_t length;
        if((length = readlink("/proc/self/exe", destination, PATH_MAX)) == -1)
        {
            m_data_path = new std::string("./data");
            return;
        }
        else
        {
            destination[length] = '\0';
            path = std::string(dirname(destination));
        }
#endif
        m_data_path = new std::string(file::fix_string(path + "/data"));
    }

    void standalone_backend_specifics::initialize_storage_path()
    {
        std::string path;
#if _WIN32
        char destination[PATH_MAX];
        if(SHGetFolderPath(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, SHGFP_TYPE_CURRENT, destination) != S_OK)
        {
            m_storage_path = new std::string(".\\storage");
            return;
        }
        path = std::string(destination);
#else
        char* home;
        char* xdg_data_home;
        if(!(xdg_data_home = std::getenv("XDG_DATA_HOME")))
        {
            if(home = std::getenv("HOME"))
            {
                m_storage_path = new std::string(std::string(home) + "/.local/share/openrayman");
                return;
            }
            else
            {
                m_storage_path = new std::string("./storage");
                return;
            }
        }
        path = std::string(xdg_data_home);
#endif
        m_storage_path = new std::string(file::fix_string(path + "/openrayman"));
    }

    const std::string& standalone_backend_specifics::get_data_path() const
    {
        return *m_data_path;
    }

    const std::string& standalone_backend_specifics::get_storage_path() const
    {
        return *m_storage_path;
    }

    double standalone_backend_specifics::get_time() const
    {
        // we use GLFW when we are run standalone.
        return glfwGetTime();
    }

    void standalone_backend_specifics::yield_cpu() const
    {
#ifdef _WIN32
        // might consume 100% cpu? who cares lmao
        Sleep(0);
#else
        // usleep has the same effect as sched_yield, but does not consume 100% CPU.
        usleep(0);
#endif
    }
}
