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
    int engine::run(const std::string& selected_game)
    {
        if(!file::exists(m_backend_specifics.get_data_path()))
        {
            message_box::display("[openrayman::engine] Error!", "The data directory was not found.", true);
            return EXIT_FAILURE;
        }
        if(!file::exists(m_backend_specifics.get_storage_path() + "/games"))
            file::create_directory(m_backend_specifics.get_storage_path() + "/games");

        std::string load_game = selected_game == "" ? m_config.game : selected_game;
        m_game = std::unique_ptr<game>(new game(m_backend_specifics, load_game));

        if(!m_game->valid())
            return EXIT_FAILURE;

        if(!m_window.open("OpenRayman", 1024, 768, m_config.fullscreen))
        {
            message_box::display("[openrayman::engine] Error!", "The window could not open."
                "\nMake sure your graphics drivers are up to date.", true);
            return EXIT_FAILURE;
        }
        m_window.gl_make_current();
        m_window.set_vsync(m_config.vsync);
		if(gl3wInit())
		{
			message_box::display("[openrayman::engine] Error!", "GL3W could not be initialized."
                "\nMake sure your graphics drivers are up to date.", true);
			return EXIT_FAILURE;
        }

        // GL3W and window should have included GL for us at this point.
        int gl_major, gl_minor;
        glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
        glGetIntegerv(GL_MINOR_VERSION, &gl_minor);

        // Provide more complete info for the user.
        std::stringstream title;
        title << "OpenRayman " << openrayman::version << " "
        	  << (this_platform == platform::windows ? "Win32" : "Linux")
              << " (OpenGL " << gl_major << "." << gl_minor << ")"
              << " (Game \"" << load_game << "\")";
        m_window.set_title(title.str());

        std::vector<std::uint8_t> icon_data;
        unsigned int width, height;
        if(!lodepng::decode(icon_data, width, height, file::fix_string(m_backend_specifics.get_data_path() + "/common/icon.png")))
            m_window.set_icon(icon_data.data(), width, height);

        m_last_timer_value = m_backend_specifics.get_time();
        while(!m_exit_requested)
        {
            double current_timer_value = m_backend_specifics.get_time();
            m_current_delta_time = current_timer_value - m_last_timer_value;
            m_accumulated_time_fixed += m_current_delta_time;
            m_accumulated_time_fps += m_current_delta_time;
            m_total_time += m_current_delta_time;
            m_last_timer_value = current_timer_value;

            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(0, 0, m_window.get_size_retina_w(), m_window.get_size_retina_h());

            m_window.poll_events();
            m_last_input = m_current_input;
            const input_state& st = m_window.get_input_provider().poll();
            m_current_input = input_state(st.buttons, st.commands, st.stick_x, st.stick_y);
            if(m_current_input.command(input_command::toggle_fullscreen) && !m_last_input.command(input_command::toggle_fullscreen))
                m_config.fullscreen = !m_config.fullscreen;

            std::cout << "Update: " << m_current_delta_time * 1000 << "ms, " << m_total_frames << std::endl;
            m_total_frames++;
            m_accumulated_frames_fps++;
            if(m_accumulated_time_fps >= 1)
            {
                m_fps = m_accumulated_frames_fps;
                std::cout << "FPS: " << m_fps << std::endl;
                m_accumulated_time_fps = m_accumulated_frames_fps = 0;
            }
            while(m_accumulated_time_fixed >= 1 / 60.0)
            {
                std::cout << "Fixed update: " << m_total_fixed_updates << std::endl;
                m_total_fixed_updates++;
                m_accumulated_time_fixed -= 1 / 60.0;
            }

            m_window.present();

            if(m_config.max_fps > 0)
            {
                while(m_backend_specifics.get_time() - m_last_timer_value < 1 / m_config.max_fps)
                    m_backend_specifics.yield_cpu();
            }

            m_window.set_vsync(m_config.vsync);
            m_window.set_fullscreen(m_config.fullscreen);

            m_exit_requested = m_exit_requested || m_window.wants_close();
        }

        m_window.close();
        return EXIT_SUCCESS;
    }
}
