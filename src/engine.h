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
            m_window(*(new libretro_window())),
            m_backend_specifics(*(new libretro_backend_specifics())),
#else
            m_window(*(new glfw_window())),
            m_backend_specifics(*(new standalone_backend_specifics())),
#endif
            m_current_input(0, 0, 0),
            m_last_input(0, 0, 0),
            m_last_timer_value(0),
            m_current_delta_time(0),
            m_total_time(0)
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
        window& get_window() const
        {
            return m_window;
        }

        // Returns a reference to the backend specifics that are currently in use.
        backend_specifics& get_backend_specifics() const
        {
            return m_backend_specifics;
        }

        // Returns a reference to the most recent (currently active) input state.
        const input_state& get_input() const
        {
            return m_current_input;
        }

        // Returns a reference to the input state that was active during the last frame.
        const input_state& get_last_input() const
        {
            return m_last_input;
        }

        // Returns the total amount of seconds that have passed since the start of the game.
        double get_total_time() const
        {
            return m_total_time;
        }

        // Returns the amount of seconds that have passed since the last frame.
        double get_delta_time() const
        {
            return m_current_delta_time;
        }

private:
        window& m_window;
        backend_specifics& m_backend_specifics;
        input_state m_current_input;
        input_state m_last_input;
        double m_last_timer_value, m_current_delta_time, m_total_time;
        bool m_exit_requested;
    };
}

#endif
