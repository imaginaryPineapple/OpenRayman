#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <GLFW/glfw3.h>
#include <string>
#include <cstdint>
#include <window/window.h>
#include <input/input_provider.h>
#include <input/standalone_input_provider.h>

namespace openrayman
{
    // Window using GLFW as backend. This is used when the game is run standalone.
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
        input_provider& get_input_provider() override;
        bool wants_close() const override;

        int get_size_w() const override;
        int get_size_h() const override;
        int get_size_retina_w() const override;
        int get_size_retina_h() const override;

        void set_size(int w, int h) override;
        void set_title(const std::string& title) override;
        void set_vsync(bool vsync) override;
        bool get_fullscreen() const override;
        void set_fullscreen(bool fullscreen) override;
        void set_icon(const std::uint8_t* rgba32_data, int w, int h) override;
private:
        GLFWwindow* m_window;
        bool m_current_fullscreen;
        int m_saved_w, m_saved_h;
        standalone_input_provider m_input_provider;

        static int m_glfw_ref_count;
    };
}

#endif
