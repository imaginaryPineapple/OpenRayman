#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <window/window.h>
#include <platform/backend_specifics.h>
#include <GL/gl3w.h>
#include <config/config.h>
#include <cstdint>
#include <game.h>
#include <memory>

#ifdef LIBRETRO_CORE
#error Building as a libretro core is not yet supported!
#else
#include <window/sdl_window.h>
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
            m_backend_specifics(*(new libretro_backend_specifics())),
#else
            m_backend_specifics(*(new standalone_backend_specifics())),
#endif
            m_config(m_backend_specifics),
            m_current_input(0, 0, 0),
            m_last_input(0, 0, 0),
            m_last_timer_value(0),
            m_current_delta_time(0),
            m_total_time(0),
            m_accumulated_time_fixed(0),
            m_accumulated_time_fps(0),
            m_total_frames(0),
            m_total_fixed_updates(0),
            m_accumulated_frames_fps(0),
            m_fps(0),
#ifdef LIBRETRO_CORE
            m_window(*(new libretro_window()))
#else
            m_window(*(new sdl_window(m_config)))
#endif
            { };

        // Starts the game loop and loads the specified game.
        int run(const std::string& selected_game, const std::string& selected_install_folder);

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
        inline window& get_window() const
        {
            return m_window;
        }

        // Returns a reference to the backend specifics that are currently in use.
        inline backend_specifics& get_backend_specifics() const
        {
            return m_backend_specifics;
        }

        // Returns a reference to the most recent (currently active) input state.
        inline const input_state& get_input() const
        {
            return m_current_input;
        }

        // Returns a reference to the input state that was active during the last frame.
        inline const input_state& get_last_input() const
        {
            return m_last_input;
        }

        // Returns the total amount of seconds that have passed since the start of the game.
        inline double get_total_time() const
        {
            return m_total_time;
        }

        // Returns the amount of seconds that have passed since the last frame.
        inline double get_delta_time() const
        {
            return m_current_delta_time;
        }

        // Returns the total amount of frames that have passed since the start of the game.
        inline std::uint64_t get_total_frames() const
        {
            return m_total_frames;
        }

        // Returns the total amount of fixed updates that have passed since the start of the game.
        inline std::uint64_t get_total_fixed_updates() const
        {
            return m_total_fixed_updates;
        }

        // Returns the amount of frames that were executed the previous second.
        inline std::uint64_t get_fps() const
        {
            return m_fps;
        }

        // Returns a reference to the active config.
        inline const config& get_config() const
        {
            return m_config;
        }

        // Returns a reference to the active game.
        inline const game& get_game() const
        {
            return *m_game;
        }

private:
        window& m_window;
        backend_specifics& m_backend_specifics;
        input_state m_current_input;
        input_state m_last_input;
        double m_last_timer_value, m_current_delta_time, m_total_time, m_accumulated_time_fixed, m_accumulated_time_fps;
        std::uint64_t m_total_frames, m_total_fixed_updates, m_accumulated_frames_fps, m_fps;
        config m_config;
        std::unique_ptr<game> m_game;
        bool m_exit_requested;
    };
}

#endif
