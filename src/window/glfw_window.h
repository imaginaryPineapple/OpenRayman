#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <GLFW/glfw3.h>
#include <string>
#include <cstdint>
#include <window/window.h>

namespace openrayman
{
    // Window using GLFW as backend.
    class glfw_window : public window
    {
public:
        bool open(const std::string& title, int w, int h, bool initial_fullscreen) override;
        void close() override;

        glfw_window();
        ~glfw_window();

        void gl_make_current() override;
        void present() override;
        void poll_events() override;
        bool wants_close() override;

        int get_size_w() override;
        int get_size_h() override;

        int get_size_retina_w() override;
        int get_size_retina_h() override;

        void set_size(int w, int h) override;
        void set_title(const std::string& title) override;
        void set_vsync(bool vsync) override;
        void set_fullscreen(bool fullscreen) override;
        void set_icon(const std::uint8_t* rgba32_data, int w, int h) override;
private:
        GLFWwindow* m_window;
        bool m_current_fullscreen;
        int m_saved_x, m_saved_y, m_saved_w, m_saved_h;
        static int m_glfw_ref_count;
    };
}

#endif
