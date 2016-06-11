#include <cstdlib>
#include <iostream>
#include <sstream>
#include <info.h>
#include <engine.h>
#include <platform/message_box.h>

namespace openrayman
{
    int engine::run(const std::string& game)
    {
        if(!m_window.open("OpenRayman", 1024, 768, false))
        {
            openrayman::message_box::display("Error in window creation!", "The window could not open.\nMake sure your graphics drivers are up to date.", true);
            return EXIT_FAILURE;
        }
        m_window.gl_make_current();
        // libepoxy and window should have included GL for us at this point.
        int gl_major, gl_minor;
        glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
        glGetIntegerv(GL_MINOR_VERSION, &gl_minor);
        std::string gl_vendor = std::string((const char*)glGetString(GL_VENDOR));
        std::string gl_gpu = std::string((const char*)glGetString(GL_RENDERER));
        // Provide more complete info for the user.
        std::stringstream title;
        title << "OpenRayman " << openrayman::version
              << " (OpenGL " << gl_major << "." << gl_minor
              << " on " << gl_vendor << " " << gl_gpu << ")"
              << " (Game \"" << game << "\")";
        m_window.set_title(title.str());
        while(!m_exit_requested)
        {
            m_window.poll_events();
            m_window.present();
            m_exit_requested = m_exit_requested || m_window.wants_close();
        }
        m_window.close();
        return EXIT_SUCCESS;
    }
}
