#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <cstdint>

namespace openrayman
{
    // Window, input and GL context abstraction.
    class window
    {
public:
        // Opens or reopens the window, and creates a GL context.
        virtual bool open(const std::string& title, int w, int h, bool initial_fullscreen) = 0;
        // Closes the window if it's not already closed.
        virtual void close() = 0;

        virtual ~window() { };

        virtual void gl_make_current() = 0;
        virtual void present() = 0;
        virtual void poll_events() = 0;
        virtual bool wants_close() = 0;

        // These are not retina aware, should be used for most purposes.
        virtual int get_size_w() = 0;
        virtual int get_size_h() = 0;

        // These are retina aware, should be used for glViewport, etc.
        virtual int get_size_retina_w() = 0;
        virtual int get_size_retina_h() = 0;

        virtual void set_size(int w, int h) = 0;
        virtual void set_title(const std::string& title) = 0;
        virtual void set_vsync(bool vsync) = 0;
        virtual void set_fullscreen(bool fullscreen) = 0;
        virtual void set_icon(const std::uint8_t* rgba32_data, int w, int h) = 0;
    };
}

#endif
