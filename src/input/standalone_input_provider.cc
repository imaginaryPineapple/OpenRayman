#include <input/standalone_input_provider.h>

namespace openrayman
{
    // TODO: libgcadapter, map keys to input state

    standalone_input_provider::standalone_input_provider(GLFWwindow* window) :
        m_input(0, 0, 0), m_window(window)
    {
    }

    standalone_input_provider::~standalone_input_provider()
    {
    }

    const std::string standalone_input_provider::get_description() const
    {
        return "Standalone input provider (GLFW, libgcadapter)";
    }

    const input_state& standalone_input_provider::poll()
    {
        m_input.stick_x = m_input.stick_y = m_input.buttons = m_input.commands = 0;
        poll_keyboard();
        return m_input;
    }

    void standalone_input_provider::poll_keyboard()
    {
        // Keyboard
        // This is not 100% accurate to the original PC version because
        // the N64 version is not the same as the PC version
        // TODO: rebindable keyboard controls
        // and controller...

        // Control stick
        // This simulates a slower walk done by the control stick
        // Used in some areas to avoid pirates
        // TODO: Is half (64) accurate?!?!
        int walk_strength = glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 64 : 127;

        if(glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
            m_input.stick_x -= walk_strength;
        if(glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            m_input.stick_x += walk_strength;
        if(glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
            m_input.stick_y -= walk_strength;
        if(glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
            m_input.stick_y += walk_strength;

        // Front buttons
        if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
            m_input.buttons |= input_button::a;
        if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            m_input.buttons |= input_button::b;

        // Triggers
        if(glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            m_input.buttons |= input_button::z;
        if(glfwGetKey(m_window, GLFW_KEY_F1) == GLFW_PRESS)
            m_input.buttons |= input_button::l;
        if(glfwGetKey(m_window, GLFW_KEY_J) == GLFW_PRESS)
            m_input.buttons |= input_button::r;

        // Start button
        if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            m_input.buttons |= input_button::start;

        // C buttons
        if(glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            m_input.commands |= input_button::cbtn_left;
        if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            m_input.commands |= input_button::cbtn_right;
        if(glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS)
            m_input.commands |= input_button::cbtn_up;
        if(glfwGetKey(m_window, GLFW_KEY_END) == GLFW_PRESS)
            m_input.commands |= input_button::cbtn_down;

        // Commands
        if(glfwGetKey(m_window, GLFW_KEY_F11) == GLFW_PRESS)
            m_input.commands |= input_command::toggle_fullscreen;
    }
}
