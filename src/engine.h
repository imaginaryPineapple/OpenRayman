#ifndef ENGINE_H
#define ENGINE_H

#include <string>

namespace openrayman
{
    // Core of OpenRayman. Handles timing, input and GL contexts.
    class engine
    {
public:
        engine() :
            m_exit_requested(false) { };

        // Starts the game loop and loads the specified game.
        int run(const std::string& game);
        // Stops the game loop.
        void exit();
private:
        bool m_exit_requested;
    };
}

#endif
