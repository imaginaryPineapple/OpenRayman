#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <game.h>
#include <renderer/renderer.h>
#include <input/input_state.h>
#include <cstdint>

namespace openrayman
{
	class engine;

    // The game controller controls the world, collision and entities within it.
    // It is also responsible for loading resources and showing "vignettes" (images, loading screens).
    class game_controller
    {
public:
        game_controller(engine& active_engine, game& active_game, renderer& active_renderer);
		~game_controller();

		void tick(double delta, std::uint64_t update, input_state& input, input_state& last_input);
		void timed_tick(std::uint64_t update, input_state& input, input_state& last_input);
		void render();

        inline engine& active_engine()
        {
            return m_engine;
        }

        inline game& active_game()
        {
            return m_game;
        }

        inline renderer& active_renderer()
        {
            return m_renderer;
        }
private:
        engine& m_engine;
        game& m_game;
        renderer& m_renderer;
    };
}

#endif
