#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <cstdint>
#include <window/window.h>
#include <input/input_provider.h>
#include <input/standalone_input_provider.h>

namespace openrayman
{
    // Window using SDL2 as backend. This is used when the game is run standalone.
    class sdl_window : public window
    {
public:
        bool open(const std::string& title, int w, int h, bool initial_fullscreen) override;
        void close() override;

        sdl_window();
        ~sdl_window();

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
        void set_fullscreen(bool fullscreen) override;
        void set_icon(std::uint8_t* rgba32_data, int w, int h) override;
private:
        SDL_Window* m_window;
        SDL_GLContext m_gl_ctx;
        bool m_current_fullscreen, m_vsync_enabled, m_wants_close;
        standalone_input_provider m_input_provider;

        static int m_sdl_ref_count;
    };
}

#endif
