#include <info.h>
#include <engine.h>
#include <platform/message_box.h>
#include <platform/file.h>
#include <lodepng.h>
#include <data_extractor.h>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <sstream>

namespace openrayman
{
    int engine::run(const std::string& selected_game, const std::string& selected_install_folder)
    {
        if(!file::exists(m_backend_specifics.data_path()))
        {
            message_box::display("[openrayman::engine::run] Error!", "The data directory was not found.", true);
            return EXIT_FAILURE;
        }
        if(!file::exists(m_backend_specifics.storage_path() + "/games"))
            file::create_directory(m_backend_specifics.storage_path() + "/games");

        if(selected_install_folder != "")
        {
            data_extractor extractor(m_backend_specifics);
            return extractor.extract(selected_install_folder) ? EXIT_SUCCESS : EXIT_FAILURE;
        }

        std::string load_game = selected_game == "" ? m_config.game : selected_game;
        m_game = std::unique_ptr<game>(new game(m_backend_specifics, load_game));

        if(!m_game->valid())
        {
            if(message_box::display_yesno("[openrayman::engine::run] Extract data?", "Should OpenRayman attempt to extract a"
                "\nRayman 2: The Great Escape installation and try again?", false))
            {
                std::string directory = message_box::display_pickdir("Select a valid Rayman 2: The Great Escape installation folder");
                if(directory == "")
                    return EXIT_FAILURE;
                data_extractor extractor(m_backend_specifics);
                if(!extractor.extract(directory))
                    return EXIT_FAILURE;
                m_game = std::unique_ptr<game>(new game(m_backend_specifics, load_game));
                if(!m_game->valid())
                    return EXIT_FAILURE;
            }
            else
                return EXIT_FAILURE;
        }

        if(!m_window.open("OpenRayman", 1024, 768, m_config.fullscreen))
        {
            message_box::display("[openrayman::engine::run] Error!", "The window could not open."
                "\nMake sure your graphics drivers are up to date.", true);
            return EXIT_FAILURE;
        }
        m_window.gl_make_current();
        m_window.set_vsync(m_config.vsync);
		if(gl3wInit())
		{
			message_box::display("[openrayman::engine::run] Error!", "GL3W could not be initialized."
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
            << map_platform(this_platform)
            << " (OpenGL " << gl_major << "." << gl_minor << ")"
            << " (Game \"" << load_game << "\")";
        m_window.set_title(title.str());

        std::vector<std::uint8_t> icon_data;
        unsigned int width, height;
        if(!lodepng::decode(icon_data, width, height, file::fix_string(m_backend_specifics.data_path() + "/common/icon.png")))
            m_window.set_icon(icon_data.data(), width, height);

		m_game_controller = std::unique_ptr<game_controller>(new game_controller(*this, *m_game, m_renderer));

        m_last_timer_value = m_backend_specifics.time();
        input_provider& provider = m_window.create_input_provider();
        while(!m_exit_requested)
        {
            double current_timer_value = m_backend_specifics.time();
            m_current_delta_time = current_timer_value - m_last_timer_value;
            m_accumulated_time_timed += m_current_delta_time;
            m_accumulated_time_fps += m_current_delta_time;
            m_total_time += m_current_delta_time;
            m_last_timer_value = current_timer_value;

            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(0, 0, m_window.size_retina_w(), m_window.size_retina_h());

            m_window.poll_events();
            m_last_input = m_current_input;

            const input_state& st = provider.poll();
            m_current_input = input_state(st.buttons, st.commands, st.stick_x, st.stick_y);
            if(m_current_input.command(input_command::toggle_fullscreen) && !m_last_input.command(input_command::toggle_fullscreen))
                m_config.fullscreen = !m_config.fullscreen;

            std::cout << "[openrayman::engine::run] FPS: " << fps() << std::endl;
            m_total_frames++;
            m_accumulated_frames_fps++;
            if(m_accumulated_time_fps >= 1)
            {
                m_fps = m_accumulated_frames_fps;
                m_accumulated_time_fps = m_accumulated_frames_fps = 0;
            }
            while(m_accumulated_time_timed >= 1 / 60.0)
            {
                m_total_timed_updates++;
                m_accumulated_time_timed -= 1 / 60.0;
            }

            m_window.present();

            // this reduces vsync latency on NVIDIA (maybe AMD/Intel? idk)
            if(m_config.vsync)
                glFinish();

            if(m_config.max_fps > 0)
            {
                while(m_backend_specifics.time() - m_last_timer_value < 1 / m_config.max_fps)
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
