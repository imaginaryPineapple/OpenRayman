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
        std::cout << "The GLFW library could not be initialized." << std::endl << "Are your graphics drivers up to date?" << std::endl;
        openrayman::message_box::display("Error!", "The GLFW library could not be initialized.\nAre your graphics drivers up to date?", true);
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE , GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(1024, 768, ("OpenRayman " + openrayman::version + " (game \"" + game + "\") OpenGL").c_str(), nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        std::cout << "The GLFW window could not be created." << std::endl << "Are your graphics drivers up to date?" << std::endl;
        openrayman::message_box::display("Error!", "The GLFW window could not be created.\nAre your graphics drivers up to date?", true);
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    // TODO: png loader, set icon
    while(!m_exit_requested)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);
        glFinish();
        m_exit_requested = m_exit_requested || glfwWindowShouldClose(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void openrayman::engine::exit()
{
    m_exit_requested = true;
}
