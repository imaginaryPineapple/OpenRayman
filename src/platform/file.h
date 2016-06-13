#ifndef FILE_H
#define FILE_H

#include <string>

namespace openrayman
{
    // Multi platform abstraction for filesystem access.
    class file
    {
public:
        // Returns true if the specified file or directory exists.
        static bool exists(const std::string& path);

        // Replaces all instances of / and \ in the specified string with the current path separator.
        static const std::string fix_string(const std::string& string);

        // Creates a new directory with the specified path.
        static void create_directory(const std::string& path);

        // The path separator of the current platform.
#ifdef _WIN32
        static const char path_separator = '\\';
#else
        static const char path_separator = '/';
#endif
    };
}

#endif
