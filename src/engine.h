#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <epoxy/gl.h>
#include <window/window.h>

#ifdef LIBRETRO_CORE
#error Building as a libretro core is not yet supported!
#else
#include <window/glfw_window.h>
#endif

namespace openrayman
{
    // Core of OpenRayman. Handles timing, input and GL contexts.
    class engine
    {
public:
        engine() :
            m_exit_requested(false),
#ifdef LIBRETRO_CORE
            m_window(*(new openrayman::libretro_window()))
#else
            m_window(*(new openrayman::glfw_window()))
#endif
            { };

        // Starts the game loop and loads the specified game.
        int run(const std::string& game);
        // Stops the game loop.
        void exit();
private:
        openrayman::window& m_window;
        bool m_exit_requested;
    };
}

#endif
