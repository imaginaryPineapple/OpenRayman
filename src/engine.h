#ifndef ENGINE_H
#define ENGINE_H

#include <string>

namespace openrayman
{
    // Core of OpenRayman. Handles timing, input and GL contexts.
    class engine
    {
public:
        // Starts the game loop and loads the specified game.
        int run(const std::string& game);
    };
}

#endif
