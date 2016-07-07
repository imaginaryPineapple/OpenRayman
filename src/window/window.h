#ifndef WINDOW_H
#define WINDOW_H

#include <input/input_provider.h>
#include <string>
#include <cstdint>

namespace openrayman
{
    // Window, input and GL context backend.
    class window
    {
public:
        virtual ~window() { };

        // Opens or reopens the window, and creates a GL context.
        virtual bool open(const std::string& title, int w, int h, bool initial_fullscreen) = 0;

        // Closes the window if it's not already closed.
        virtual void close() = 0;

        // Makes the GL context associated with the window the current one.
        virtual void gl_make_current() = 0;

        // Swaps GL buffers and presents them on the screen.
        virtual void present() = 0;

        // Polls backend specific events such as input and user commands.
        virtual void poll_events() = 0;

        // Returns a reference to an input provider driven by this window.
        virtual input_provider& create_input_provider() = 0;

        // Returns true if the user has manually requested a shutdown via e.g. the close button.
        virtual bool wants_close() const = 0;

        // Returns the size of the client area of the window.
        // This will be different than the specified size if the window is in fullscreen mode.
        // These are not retina aware and should be used for most purposes.
        virtual int size_w() const = 0;
        virtual int size_h() const = 0;

        // These are retina aware and should be used for when the actual framebuffer size is needed, as for glViewport, etc.
        virtual int size_retina_w() const = 0;
        virtual int size_retina_h() const = 0;

        // Set the size of the window client area. Not honored when in fullscreen.
        virtual void set_size(int w, int h) = 0;

        // Set the title of the window.
        virtual void set_title(const std::string& title) = 0;

        // Enable or disable vertical synchronization of the GL context.
        // This should not incur any performance penalty.
        virtual void set_vsync(bool vsync) = 0;

        // Toggles fullscreen mode of the window.
        // This should not incur any performance penalty.
        virtual void set_fullscreen(bool fullscreen) = 0;

        // Set the user visible icon of the window.
        virtual void set_icon(std::uint8_t* rgba32_data, int w, int h) = 0;
    };
}

#endif
