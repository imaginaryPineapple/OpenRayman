#ifndef FILE_H
#define FILE_H

#include <string>

namespace openrayman
{
    // Simple platform abstraction for files.
    class file
    {
public:
        // If the specified file or directory exists.
        static bool exists(const std::string& path);

        // Replaces all instances of / and \ in the specified string with the current path separator.
        static const std::string fix_string(const std::string& string);

        // Get the path where the openrayman executable resides.
        static const std::string& get_executable_path();

        // The path separator of the current platform.
#ifdef _WIN32
        static const char path_separator = '\\';
#else
        static const char path_separator = '/';
#endif
private:
        static std::string* m_executable_path;
    };
}

#endif
