#include <window/glfw_window.h>
#include <info.h>

namespace openrayman
{
    int glfw_window::m_glfw_ref_count = 0;

    glfw_window::glfw_window() :
        m_window(nullptr),
        m_current_fullscreen(false),
        m_saved_w(0),
        m_saved_h(0),
        m_input_provider(nullptr)
    {
        if(m_glfw_ref_count <= 0)
        {
            if(!glfwInit())
                m_glfw_ref_count--;
        }
        m_glfw_ref_count++;
    }

    glfw_window::~glfw_window()
    {
        if(m_window != nullptr)
            close();
        if(m_glfw_ref_count == 1)
            glfwTerminate();
        m_glfw_ref_count--;
    }

    bool glfw_window::open(const std::string& title, int w, int h, bool initial_fullscreen)
    {
        if(m_glfw_ref_count <= 0)
            return false;

        if(m_window != nullptr)
            glfwDestroyWindow(m_window);

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, initial_fullscreen ? GLFW_TRUE : GLFW_FALSE);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if(monitor == nullptr)
            return false;
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if(mode == nullptr)
            return false;
        int width = initial_fullscreen ? mode->width : w;
        int height = initial_fullscreen ? mode->height : h;
        m_window = glfwCreateWindow(width, height, title.c_str(), initial_fullscreen ? monitor : nullptr, nullptr);
        if(m_window == nullptr)
            return false;

        m_input_provider = standalone_input_provider(m_window);

		int left, top, right, bottom;
		glfwGetWindowFrameSize(m_window, &left, &top, &right, &bottom);
		int total_w = w + left + right;
		int total_h = h + top + bottom;
		if(!initial_fullscreen)
		{
			glfwSetWindowPos(m_window, mode->width / 2 - total_w / 2, mode->height / 2 - total_h / 2);
			glfwShowWindow(m_window);
		}
		m_saved_w = w;
		m_saved_h = h;

        glfwSwapInterval(0);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        m_current_fullscreen = initial_fullscreen;
        return true;
    }

    void glfw_window::close()
    {
        if(m_window != nullptr)
            glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    void glfw_window::gl_make_current()
    {
        glfwMakeContextCurrent(m_window);
    }

    void glfw_window::present()
    {
        glfwSwapBuffers(m_window);
    }

    input_provider& glfw_window::get_input_provider()
    {
        return m_input_provider;
    }

    void glfw_window::poll_events()
    {
        glfwPollEvents();
    }

    bool glfw_window::wants_close() const
    {
        return glfwWindowShouldClose(m_window);
    }

    int glfw_window::get_size_w() const
    {
        int w, tmp;
        glfwGetWindowSize(m_window, &w, &tmp);
        return w;
    }

    int glfw_window::get_size_h() const
    {
        int h, tmp;
        glfwGetWindowSize(m_window, &h, &tmp);
        return h;
    }

    int glfw_window::get_size_retina_w() const
    {
        int w, tmp;
        glfwGetFramebufferSize(m_window, &w, &tmp);
        return w;
    }

    int glfw_window::get_size_retina_h() const
    {
        int h, tmp;
        glfwGetFramebufferSize(m_window, &h, &tmp);
        return h;
    }

    void glfw_window::set_size(int w, int h)
    {
        if(!m_current_fullscreen)
            glfwSetWindowSize(m_window, w, h);
    }

    void glfw_window::set_title(const std::string& title)
    {
        glfwSetWindowTitle(m_window, title.c_str());
    }

    void glfw_window::set_vsync(bool vsync)
    {
        glfwSwapInterval(vsync ? 1 : 0);
    }

    bool glfw_window::get_fullscreen() const
    {
        return m_current_fullscreen;
    }

    void glfw_window::set_fullscreen(bool fullscreen)
    {
        if(fullscreen == m_current_fullscreen)
            return;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if(monitor == nullptr)
            return;
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if(mode == nullptr)
            return;
        int width = fullscreen ? mode->width : m_saved_w;
        int height = fullscreen ? mode->height : m_saved_h;
        glfwSetWindowMonitor(m_window, fullscreen ? monitor : nullptr, 0, 0, width, height, GLFW_DONT_CARE);

        if(!fullscreen)
        {
            int left, top, right, bottom;
            glfwGetWindowFrameSize(m_window, &left, &top, &right, &bottom);
            glfwSetWindowPos(m_window, mode->width / 2 - (width + left + right) / 2, mode->height / 2 - (height + top + bottom) / 2);
        }

        m_current_fullscreen = fullscreen;
    }

    void glfw_window::set_icon(const std::uint8_t* rgba32_data, int w, int h)
    {
        GLFWimage image;
        image.pixels = const_cast<std::uint8_t*>(rgba32_data);
        image.width = w;
        image.height = h;
        glfwSetWindowIcon(m_window, rgba32_data == nullptr ? 0 : 1, &image);
    }
}
