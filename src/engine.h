#ifndef ENGINE_H
#define ENGINE_H

#include <string>

namespace openrayman
{
    // Core of OpenRayman. Handles timing, input and GL contexts.
    class engine
    {
public:
        int run(const std::string& game);
    };
}

#endif
