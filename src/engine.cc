#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <info.h>
#include <engine.h>
#include <platform/message_box.h>
#include <platform/file.h>
#include <lodepng.h>

namespace openrayman
{
    int engine::run(const std::string& game)
    {
        if(!openrayman::file::exists(m_backend_specifics.get_data_path()))
        {
            openrayman::message_box::display("Error!", "The data directory was not found.", true);
            return EXIT_FAILURE;
        }
        if(!openrayman::file::exists(m_backend_specifics.get_data_path() + "/common"))
        {
            openrayman::message_box::display("Error!", "The data/common/ directory was not found.", true);
            return EXIT_FAILURE;
        }
        if(!openrayman::file::exists(m_backend_specifics.get_storage_path()))
            openrayman::file::create_directory(m_backend_specifics.get_storage_path() + "/");
        if(!m_window.open("OpenRayman", 1024, 768, false))
        {
            openrayman::message_box::display("Error in window creation!", "The window could not open.\nMake sure your graphics drivers are up to date.", true);
            return EXIT_FAILURE;
        }

        m_window.gl_make_current();
		if(gl3wInit())
		{
			openrayman::message_box::display("Error in GL3W!", "GL3W could not be initialized.\nMake sure your graphics drivers are up to date.", true);
			return EXIT_FAILURE;
        }

        // GL3W and window should have included GL for us at this point.
        int gl_major, gl_minor;
        glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
        glGetIntegerv(GL_MINOR_VERSION, &gl_minor);

        // Provide more complete info for the user.
        std::stringstream title;
        title << "OpenRayman " << openrayman::version << " "
        	  << (openrayman::this_platform == openrayman::platform::windows ? "Win32" : "Linux")
              << " (OpenGL " << gl_major << "." << gl_minor << ")"
              << " (Game \"" << game << "\")";
        m_window.set_title(title.str());

        std::vector<std::uint8_t> icon_data;
        unsigned int width, height;
        if(!lodepng::decode(icon_data, width, height, openrayman::file::fix_string(m_backend_specifics.get_data_path() + "/common/icon.png")))
            m_window.set_icon(icon_data.data(), width, height);

        while(!m_exit_requested)
        {
            glViewport(0, 0, m_window.get_size_retina_w(), m_window.get_size_retina_h());
            m_window.poll_events();
            m_window.present();
            m_exit_requested = m_exit_requested || m_window.wants_close();
        }

        m_window.close();
        return EXIT_SUCCESS;
    }
}
