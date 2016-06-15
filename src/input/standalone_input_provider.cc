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
        poll_game_controller();
        return m_input;
    }

    void standalone_input_provider::poll_keyboard()
    {
        const std::uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr);

        // Keyboard
        // This is not 100% accurate to the original PC version because
        // the N64 version is not the same as the PC version

        // Control stick
        // This simulates a slower walk done by the control stick
        // Used in some areas to avoid pirates
        // TODO: Is half (64) accurate?!?!
        int walk_strength = keyboard_state[SDL_SCANCODE_LSHIFT] ? 64 : 127;

        int new_stick_x = m_input.stick_x;
        int new_stick_y = m_input.stick_y;

        if(keyboard_state[SDL_SCANCODE_LEFT])
            new_stick_x -= walk_strength;
        if(keyboard_state[SDL_SCANCODE_RIGHT])
            new_stick_x += walk_strength;
        if(keyboard_state[SDL_SCANCODE_UP])
            new_stick_y -= walk_strength;
        if(keyboard_state[SDL_SCANCODE_DOWN])
            new_stick_y += walk_strength;

        new_stick_x = std::min(127, std::max(-127, new_stick_x));
        new_stick_y = std::min(127, std::max(-127, new_stick_y));

        m_input.stick_x = new_stick_x;
        m_input.stick_y = new_stick_y;

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

    void standalone_input_provider::poll_game_controller()
    {
        // Game controller
        // Again, not 100% accurate to the original PC version because
        // the N64 version is not the same as the PC version and the original game
        // was not meant to be played with Xbox controllers and frankly, the original
        // layout is shit and doesn't work most of the time (dinput)

        for(const std::pair<int, SDL_GameController*>& pair : m_game_controllers)
        {
            SDL_GameController* controller = pair.second;

            // Control stick
            std::int16_t stick_x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) / 258;
            std::int16_t stick_y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 258;

            int new_stick_x = m_input.stick_x;
            int new_stick_y = m_input.stick_y;

            new_stick_x += stick_x;
            new_stick_y += stick_y;

            new_stick_x = std::min(127, std::max(-127, new_stick_x));
            new_stick_y = std::min(127, std::max(-127, new_stick_y));

            m_input.stick_x = new_stick_x;
            m_input.stick_y = new_stick_y;

            // Front buttons
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
                m_input.buttons |= input_button::a;
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
                m_input.buttons |= input_button::b;

            // Triggers
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 20000 ||
                SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
                m_input.buttons |= input_button::z;
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
                m_input.buttons |= input_button::l;
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
                m_input.buttons |= input_button::r;

            // Start button
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START))
                m_input.buttons |= input_button::start;

            // C buttons
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
                m_input.commands |= input_button::cbtn_left;
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                m_input.commands |= input_button::cbtn_right;
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
                m_input.commands |= input_button::cbtn_up;
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
                m_input.commands |= input_button::cbtn_down;
        }
    }
}
