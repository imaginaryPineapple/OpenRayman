#include <platform/file.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <cunistd>
#endif

namespace openrayman
{
    void file::exists(const std::string& path)
    {
#ifdef _WIN32
        DWORD attrib = GetFileAttributes(std::wstring(fix_string(path)));
        return attrib != INVALID_FILE_ATTRIBUTES;
#else
        return access(fix_string(path), F_OK | R_OK) != -1;
#endif
    }

    const std::string fix_string(const std::string& string)
    {
        std::string new_str(string);
        for(size_t n = 0; n < new_str.length(); n++)
        {
            if(new_str[n] == '/' || new_str[n] == '\\')
                new_str.replace(n, 1, 1, path_separator);
        }
        return new_str;
    }
}
