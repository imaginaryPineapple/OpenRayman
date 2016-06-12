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
    const std::string& standalone_backend_specifics::get_data_path()
    {
        if(m_data_path != nullptr)
            return *m_data_path;
        std::string path;
#if _WIN32
        char str[MAX_PATH];
        int result = GetModuleFileName(nullptr, str, MAX_PATH);
        if(result == 0)
        {
            m_data_path = new std::string(".\\data");
            return *m_data_path;
        }
        PathRemoveFileSpec(str);
        path = std::string(str);
#else
        char destination[PATH_MAX];
        size_t length;
        if((length = readlink("/proc/self/exe", destination, PATH_MAX)) == -1)
        {
            m_data_path = new std::string("./data");
            return *m_data_path;
        }
        else
        {
            destination[length] = '\0';
            path = std::string(dirname(destination));
        }
#endif
        m_data_path = new std::string(openrayman::file::fix_string(path + "/data"));
        return *m_data_path;
    }

    const std::string& standalone_backend_specifics::get_storage_path()
    {
        if(m_storage_path != nullptr)
            return *m_storage_path;
        std::string path;
#if _WIN32
        char destination[PATH_MAX];
        if(SHGetFolderPath(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, SHGFP_TYPE_CURRENT, &destination) != S_OK)
        {
            m_storage_path = new std::string(".\\storage");
            return *m_storage_path;
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
                return *m_storage_path;
            }
            else
            {
                m_storage_path = new std::string("./storage");
                return *m_storage_path;
            }
        }
        path = std::string(xdg_data_home);
#endif
        m_storage_path = new std::string(openrayman::file::fix_string(path + "/openrayman"));
        return *m_storage_path;
    }

    double standalone_backend_specifics::get_time()
    {
        // we use GLFW when we are run standalone.
        return glfwGetTime();
    }
}
