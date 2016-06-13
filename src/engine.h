#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <window/window.h>
#include <platform/backend_specifics.h>
#include <GL/gl3w.h>

#ifdef LIBRETRO_CORE
#error Building as a libretro core is not yet supported!
#else
#include <window/glfw_window.h>
#include <platform/standalone_backend_specifics.h>
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
            m_window(*(new openrayman::libretro_window())),
            m_backend_specifics(*(new openrayman::libretro_backend_specifics()))
#else
            m_window(*(new openrayman::glfw_window())),
            m_backend_specifics(*(new openrayman::standalone_backend_specifics()))
#endif
            { };

        // Starts the game loop and loads the specified game.
        int run(const std::string& game);

        // Requests that the game loop stops.
        void exit()
        {
            m_exit_requested = true;
        }

        // Returns true if an exit request was cancelled.
        bool cancel_exit()
        {
            if(m_exit_requested)
            {
                m_exit_requested = false;
                return true;
            }
            return false;
        }

        // Returns a reference to the engine window.
        const openrayman::window& get_window()
        {
            return m_window;
        }

        // Returns a reference to the backend specifics that are currently in use.
        const openrayman::backend_specifics& get_backend_specifics()
        {
            return m_backend_specifics;
        }
        
private:
        openrayman::window& m_window;
        openrayman::backend_specifics& m_backend_specifics;
        bool m_exit_requested;
    };
}

#endif
