#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <cstdint>
#include <string>

namespace openrayman
{
    // Buttons on a N64 controller.
    enum input_button : std::uint16_t
    {
        // All four dpad axises.
        dpad_left = (1 << 1),
        dpad_right = (1 << 2),
        dpad_up = (1 << 3),
        dpad_down = (1 << 4),

        // The start button. Used for pausing.
        start = (1 << 5),

        // The front buttons. Used for jumping and shooting.
        a = (1 << 6),
        b = (1 << 7),

        // The triggers. Used for targeting and info
        l = (1 << 8),
        r = (1 << 9),
        z = (1 << 10),

        // All four C-buttons. Used for camera movement.
        cbtn_left = (1 << 11),
        cbtn_right = (1 << 12),
        cbtn_up = (1 << 13),
        cbtn_down = (1 << 14)
    };

    // Additional and optional user commands that can be sent by an input state (for toggling fullscreen, etc).
    enum input_command : std::uint8_t
    {
        // Requests that the engine toggles fullscreen mode.
        toggle_fullscreen = (1 << 1)
    };

    // State of input at a point in time.
    // Input is mapped to an N64 controller layout (the original Rayman 2 console).
    struct input_state
    {
    	// The buttons that are held down.
        std::uint16_t buttons;

        // Additional user commands from e.g. a keyboard or other user input.
        std::uint8_t commands;

        // The analog stick axises, ranges from -128 to 127.
        int8_t stick_x, stick_y;

        input_state(std::uint16_t buttons, int8_t stick_x, int8_t stick_y) :
            buttons(buttons), commands(0), stick_x(stick_x), stick_y(stick_y) { }

        input_state(std::uint16_t buttons, std::uint8_t commands, int8_t stick_x, int8_t stick_y) :
            buttons(buttons), commands(commands), stick_x(stick_x), stick_y(stick_y) { }

        // Helper function to determine if the specified button is held down.
        inline bool button(std::uint16_t button) const
        {
            return (buttons & button) == button;
        }

        // Helper function to determine if a command was specified.
        inline bool command(std::uint8_t command) const
        {
            return (commands & command) == command;
        }
    };
}

#endif
