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
        if(glfwGetKey(m_window, GLFW_KEY_F11) == GLFW_PRESS)
            m_input.commands |= input_command::toggle_fullscreen;
        return m_input;
    }
}
