#ifndef ENGINE_H
#define ENGINE_H

#ifdef LIBRETRO_CORE
#error Building as a libretro core is not yet supported!
#else
#include <window/sdl_window.h>
#include <platform/standalone_backend_specifics.h>
#endif

#include <window/window.h>
#include <platform/backend_specifics.h>
#include <GL/gl3w.h>
#include <config/config.h>
#include <game.h>
#include <game_controller.h>
#include <renderer/renderer.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <memory>

namespace openrayman
{
    // The engine controls game initialization, timing and renderer/input/window management.
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
            m_accumulated_time_timed(0),
            m_accumulated_time_fps(0),
            m_total_frames(0),
            m_total_timed_updates(0),
            m_accumulated_frames_fps(0),
            m_fps(0),
			m_game(nullptr),
			m_game_controller(nullptr),
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
        inline window& active_window() const
        {
            return m_window;
        }

        // Returns a reference to the backend specifics that are currently in use.
        inline backend_specifics& active_backend_specifics() const
        {
            return m_backend_specifics;
        }

        // Returns a reference to the most recent (currently active) input state.
        inline const input_state& input() const
        {
            return m_current_input;
        }

        // Returns a reference to the input state that was active during the last frame.
        inline const input_state& last_input() const
        {
            return m_last_input;
        }

        // Returns the total amount of seconds that have passed since the start of the game.
        inline double total_time() const
        {
            return m_total_time;
        }

        // Returns the amount of seconds that have passed since the last frame.
        inline double delta_time() const
        {
            return m_current_delta_time;
        }

        // Returns the total amount of frames that have passed since the start of the game.
        inline std::uint64_t total_frames() const
        {
            return m_total_frames;
        }

        // Returns the total amount of timed updates that have passed since the start of the game.
        inline std::uint64_t total_timed_updates() const
        {
            return m_total_timed_updates;
        }

        // Returns the amount of frames that were executed during the previous second.
        inline double fps() const
        {
            if(std::abs((int)(m_fps - (1 / m_current_delta_time))) > 2)
                return 1 / m_current_delta_time;
            // this is more accurate
            return (m_fps + (1 / m_current_delta_time)) / 2;
        }

        // Returns a reference to the active config.
        inline const config& active_config() const
        {
            return m_config;
        }

        // Returns a reference to the active game.
        inline const game& active_game() const
        {
            return *m_game;
        }

		// Returns a reference to the active game controller.
		inline game_controller& active_game_controller()
		{
			return *m_game_controller;
		}
private:
        window& m_window;
        backend_specifics& m_backend_specifics;
        input_state m_current_input;
        input_state m_last_input;
        double m_last_timer_value, m_current_delta_time, m_total_time, m_accumulated_time_timed, m_accumulated_time_fps;
        std::uint64_t m_total_frames, m_total_timed_updates, m_accumulated_frames_fps, m_fps;
        config m_config;
        std::unique_ptr<game> m_game;
		renderer m_renderer;
        std::unique_ptr<game_controller> m_game_controller;
        bool m_exit_requested;
    };
}

#endif
