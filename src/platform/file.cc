#include <platform/file.h>
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <locale>
#include <codecvt>
#else
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#endif

namespace openrayman
{
    std::string* file::m_executable_path = nullptr;

    bool file::exists(const std::string& path)
    {
#ifdef _WIN32
        DWORD attrib = GetFileAttributes(fix_string(path).c_str());
        return attrib != INVALID_FILE_ATTRIBUTES;
#else
        return access(fix_string(path).c_str(), F_OK | R_OK) != -1;
#endif
    }

    const std::string file::fix_string(const std::string& string)
    {
        std::string new_str(string);
        for(size_t n = 0; n < new_str.length(); n++)
        {
            if(new_str[n] == '/' || new_str[n] == '\\')
                new_str.replace(n, 1, 1, path_separator);
        }
        return new_str;
    }

    const std::string& file::get_executable_path()
    {
        if(m_executable_path != nullptr)
            return *m_executable_path;
        std::string path;
#if _WIN32
        char str[MAX_PATH];
        int result = GetModuleFileName(nullptr, str, MAX_PATH);
        if(result == 0)
        {
            m_executable_path = new std::string("");
            return *m_executable_path;
        }
		PathRemoveFileSpec(str);
        path = std::string(str);
#else
        char destination[PATH_MAX];
        size_t length;
        if((length = readlink("/proc/self/exe", destination, PATH_MAX)) == -1)
        {
            m_executable_path = new std::string("");
            return *m_executable_path;
        }
        else
        {
            destination[length] = '\0';
            path = std::string(dirname(destination));
        }
#endif
        m_executable_path = new std::string(path);
        return *m_executable_path;
    }
}
