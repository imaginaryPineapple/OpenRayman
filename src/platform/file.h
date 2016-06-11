#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <string>

namespace openrayman
{
    // Simple platform abstraction for files.
    class file
    {
public:
        // If the specified file or directory exists.
        static void exists(const std::string& path);

        // Replaces all instances of / and \ in the specified string with the current path separator.
        static const std::string fix_string(const std::string& string);

        // The path separator of the current platform.
#ifdef _WIN32
        static const char path_separator = '\\';
#else
        static const char path_separator = '/';
#endif
    };
}

#endif
