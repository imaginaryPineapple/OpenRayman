#include <cstdlib>
#include <iostream>
#include <info.h>
#include <engine.h>
#include <platform/message_box.h>
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

int openrayman::engine::run(const std::string& game)
{
    if(!glfwInit())
    {
        openrayman::message_box::display("Error!", "The GLFW library could not be initialized.\nAre your graphics drivers up to date?", true);
        return EXIT_FAILURE;
    }
    GLFWwindow* window = glfwCreateWindow(1024, 768, ("OpenRayman " + openrayman::version + " (game \"" + game + "\")").c_str(), nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // TODO: png loader, set icon
    while(!m_exit_requested)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);
        m_exit_requested = m_exit_requested || glfwWindowShouldClose(window);
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}

void openrayman::engine::exit()
{
    m_exit_requested = true;
}
