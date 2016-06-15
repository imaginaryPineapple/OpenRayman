#include <input/standalone_input_provider.h>
#include <cstdint>

namespace openrayman
{
    standalone_input_provider::standalone_input_provider(SDL_Window* window) :
        m_input(0, 0, 0),
        m_window(window)
    {
    }

    standalone_input_provider::~standalone_input_provider()
    {
    }

    const std::string standalone_input_provider::get_description() const
    {
        return "SDL2 input provider (standalone)";
    }

    const input_state& standalone_input_provider::poll()
    {
        m_input.stick_x = m_input.stick_y = m_input.buttons = m_input.commands = 0;
        poll_keyboard();
        return m_input;
    }

    void standalone_input_provider::poll_keyboard()
    {
        const std::uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr);

        // Keyboard
        // This is not 100% accurate to the original PC version because
        // the N64 version is not the same as the PC version
        // TODO: rebindable keyboard controls
        // and controller...

        // Control stick
        // This simulates a slower walk done by the control stick
        // Used in some areas to avoid pirates
        // TODO: Is half (64) accurate?!?!
        int walk_strength = keyboard_state[SDL_SCANCODE_LSHIFT] ? 64 : 127;

        if(keyboard_state[SDL_SCANCODE_LEFT])
            m_input.stick_x -= walk_strength;
        if(keyboard_state[SDL_SCANCODE_RIGHT])
            m_input.stick_x += walk_strength;
        if(keyboard_state[SDL_SCANCODE_UP])
            m_input.stick_y -= walk_strength;
        if(keyboard_state[SDL_SCANCODE_DOWN])
            m_input.stick_y += walk_strength;

        // Front buttons
        if(keyboard_state[SDL_SCANCODE_SPACE])
            m_input.buttons |= input_button::a;
        if(keyboard_state[SDL_SCANCODE_A])
            m_input.buttons |= input_button::b;

        // Triggers
        if(keyboard_state[SDL_SCANCODE_LCTRL])
            m_input.buttons |= input_button::z;
        if(keyboard_state[SDL_SCANCODE_F1])
            m_input.buttons |= input_button::l;
        if(keyboard_state[SDL_SCANCODE_J])
            m_input.buttons |= input_button::r;

        // Start button
        if(keyboard_state[SDL_SCANCODE_ESCAPE])
            m_input.buttons |= input_button::start;

        // C buttons
        if(keyboard_state[SDL_SCANCODE_Q])
            m_input.commands |= input_button::cbtn_left;
        if(keyboard_state[SDL_SCANCODE_W])
            m_input.commands |= input_button::cbtn_right;
        if(keyboard_state[SDL_SCANCODE_O])
            m_input.commands |= input_button::cbtn_up;
        if(keyboard_state[SDL_SCANCODE_END])
            m_input.commands |= input_button::cbtn_down;

        // Commands
        if(keyboard_state[SDL_SCANCODE_F11])
            m_input.commands |= input_command::toggle_fullscreen;
    }
}
