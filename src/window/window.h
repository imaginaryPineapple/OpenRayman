#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <cstdint>

namespace openrayman
{
    // Window, input and GL context backend.
    class window
    {
public:
        // Opens or reopens the window, and creates a GL context.
        virtual bool open(const std::string& title, int w, int h, bool initial_fullscreen) = 0;

        // Closes the window if it's not already closed.
        virtual void close() = 0;

        virtual ~window() { };

        // Makes the GL context associated with the window the current one.
        virtual void gl_make_current() = 0;

        // Swaps GL buffers and presents them on the screen.
        virtual void present() = 0;

        // Polls backend specific events such as input and user commands.
        virtual void poll_events() = 0;

        // Returns true if the user has manually requested a shutdown via e.g. the close button.
        virtual bool wants_close() = 0;

        // Returns the size of the client area of the window.
        // This will be different than the specified size if the window is in fullscreen mode.
        // These are not retina aware and should be used for most purposes.
        virtual int get_size_w() = 0;
        virtual int get_size_h() = 0;

        // These are retina aware and should be used for when the actual framebuffer size is needed, as for glViewport, etc.
        virtual int get_size_retina_w() = 0;
        virtual int get_size_retina_h() = 0;

        // Set the size of the window client area. Not honored when in fullscreen.
        virtual void set_size(int w, int h) = 0;

        // Set the title of the window.
        virtual void set_title(const std::string& title) = 0;

        // Enable or disable vertical synchronization of the GL context.
        virtual void set_vsync(bool vsync) = 0;

        // Toggles fullscreen mode of the window.
        virtual void set_fullscreen(bool fullscreen) = 0;

        // Set the user visible icon of the window. This will default to the OS picked icon if nullptr is specified.
        virtual void set_icon(const std::uint8_t* rgba32_data, int w, int h) = 0;
    };
}

#endif
