#ifndef INFO_H
#define INFO_H

#include <string>

namespace openrayman
{
    // The current version of the engine.
    const std::string version = "1.0";

    // The current version of the engine, formatted as an integer.
    const int version_code = 10;

    // Platform definitions.
#ifdef linux
#undef linux
#endif
    enum class platform { windows, linux };

    // The platform that engine is running on (that it was compiled for).
    // Assume we're on linux unless _WIN32 is specified, as those are the only two platforms we support.
    // TODO: platform for libretro?!?
#ifdef _WIN32
    const platform this_platform = platform::windows;
#else
    const platform this_platform = platform::linux;
#endif
}

#endif
