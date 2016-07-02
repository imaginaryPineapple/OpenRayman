#ifndef GF_TOOLS_H
#define GF_TOOLS_H

#include <string>

namespace openrayman
{
    // Tools for converting and printing information about GF files.
    // Used by main.cc
    namespace gf_tools
    {
        int convert_to_png(const std::string& path, const std::string& target);
        int print_info(const std::string& path);
    }
}

#endif
