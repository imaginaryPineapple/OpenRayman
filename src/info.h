#ifndef INFO_H
#define INFO_H

#include <string>
#include <map>

namespace openrayman
{
    // The current version of the engine.
    const std::string version = "1.0";

    // The current version of the engine, formatted as an integer.
    const int version_code = 10;

    // Platform definitions.
    enum class platform { win32, linux_desktop, libretro, android };

	inline std::string map_platform(platform p)
	{
		switch(p)
		{
			case platform::win32:
				return "Windows (Win32)";

			case platform::linux_desktop:
				return "Linux";

			case platform::libretro:
				return "libretro";

			case platform::android:
				return "Android";
		}
	}

    // The platform that engine is running on (that it was compiled for).
    // Assume we're on linux unless _WIN32 is specified, as those are the only two platforms we support right now.
#ifdef _WIN32
    const platform this_platform = platform::win32;
#else
    const platform this_platform = platform::linux_desktop;
#endif
}

#endif
