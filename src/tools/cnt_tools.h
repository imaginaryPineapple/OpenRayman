#ifndef CNT_TOOLS_H
#define CNT_TOOLS_H

#include <string>

namespace openrayman
{
    // Tools for extracting from and printing information about CNT files.
    // Used by main.cc
    namespace cnt_tools
    {
        int extract(const std::string& path, const std::string& name_in, const std::string& target);
        int print_hierarchy(const std::string& path);
    }
}

#endif
