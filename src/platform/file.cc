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
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace openrayman
{
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

    void file::create_directory(const std::string& path)
    {
#ifdef _WIN32
        CreateDirectory(fix_string(path).c_str(), nullptr);
#else
        mkdir(fix_string(path).c_str(), 0775);
#endif
    }
}
