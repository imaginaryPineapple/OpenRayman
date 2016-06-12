#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <cstdint>
#include <string>

namespace openrayman
{
    // Buttons on a N64 controller.
    enum input_button : std::uint16_t
    {
        dpad_left = (1 << 0),
        dpad_right = (1 << 1),
        dpad_up = (1 << 2),
        dpad_down = (1 << 3),
        start = (1 << 4),
        a = (1 << 5),
        b = (1 << 6),
        l = (1 << 7),
        r = (1 << 8),
        z = (1 << 9),
        cbtn_left = (1 << 10),
        cbtn_right = (1 << 11),
        cbtn_up = (1 << 12),
        cbtn_down = (1 << 13)
    };

    // State of input at a point in time.
    // Input is mapped to an N64 controller layout (the original Rayman 2 console).
    struct input_state
    {
        input_button buttons;
        int8_t stick_x, stick_y;

        input_state(input_button buttons, int8_t stick_x, int8_t stick_y) :
            buttons(buttons), stick_x(stick_x), stick_y(stick_y) { }

        bool button(input_button button)
        {
            return (buttons & button) == button;
        }
    };
}

#endif
