#ifndef STANDALONE_INPUT_PROVIDER_H
#define STANDALONE_INPUT_PROVIDER_H

#include <input/input_state.h>
#include <input/input_provider.h>
#include <config/config.h>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

namespace openrayman
{
    // Implements input when the game is run standalone.
    // Sources are GLFW and libgcadapter.
    class standalone_input_provider : public input_provider
    {
        friend class sdl_window;
public:
        standalone_input_provider(config& config) :
            m_input(0, 0, 0),
            m_config(config)
        {
        }

        const std::string description() const override;
        const input_state& poll() override;
private:
        void poll_keyboard();
        void poll_game_controller();
        input_state m_input;
        config& m_config;
        std::unordered_map<int, SDL_GameController*> m_game_controllers;
    };
}

#endif
