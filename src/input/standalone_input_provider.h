#ifndef STANDALONE_INPUT_PROVIDER_H
#define STANDALONE_INPUT_PROVIDER_H

#include <input/input_state.h>
#include <input/input_provider.h>
#include <GLFW/glfw3.h>
#include <string>

namespace openrayman
{
    // Implements input when the game is run standalone.
    // Sources are GLFW and libgcadapter.
    class standalone_input_provider : public input_provider
    {
public:
        standalone_input_provider(GLFWwindow* window);
        ~standalone_input_provider();

        const std::string get_description() const override;
        const input_state& poll() override;
private:
        input_state m_input;
        GLFWwindow* m_window;
    };
}

#endif
