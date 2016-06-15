#include <window/sdl_window.h>
#include <info.h>
#include <iostream>

namespace openrayman
{
    int sdl_window::m_sdl_ref_count = 0;

    sdl_window::sdl_window(config& config) :
        m_window(nullptr),
        m_current_fullscreen(false),
        m_vsync_enabled(false),
        m_wants_close(false),
        m_input_provider(config)
    {
        if(m_sdl_ref_count <= 0)
        {
            if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
                m_sdl_ref_count--;
        }
        m_sdl_ref_count++;
    }

    sdl_window::~sdl_window()
    {
        if(m_window != nullptr)
            close();
        if(m_sdl_ref_count == 1)
            SDL_Quit();
        m_sdl_ref_count--;
    }

    bool sdl_window::open(const std::string& title, int w, int h, bool initial_fullscreen)
    {
        if(m_sdl_ref_count <= 0)
            return false;

        if(m_window != nullptr)
            close();

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        m_window = SDL_CreateWindow(title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            w,
            h,
            SDL_WINDOW_OPENGL
            | SDL_WINDOW_ALLOW_HIGHDPI
            | SDL_WINDOW_RESIZABLE
            | (initial_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0x00));
        if(m_window == nullptr)
            return false;

        m_gl_ctx = SDL_GL_CreateContext(m_window);
        if(m_gl_ctx == nullptr)
        {
            close();
            return false;
        }

        SDL_GL_SetSwapInterval(0);
        SDL_ShowCursor(SDL_DISABLE);

        m_current_fullscreen = initial_fullscreen;
        return true;
    }

    void sdl_window::close()
    {
        if(m_gl_ctx != nullptr)
            SDL_GL_DeleteContext(m_gl_ctx);
        if(m_window != nullptr)
            SDL_DestroyWindow(m_window);
        m_gl_ctx = nullptr;
        m_window = nullptr;
    }

    void sdl_window::gl_make_current()
    {
        SDL_GL_MakeCurrent(m_window, m_gl_ctx);
    }

    void sdl_window::present()
    {
        SDL_GL_SwapWindow(m_window);
    }

    input_provider& sdl_window::get_input_provider()
    {
        return m_input_provider;
    }

    void sdl_window::poll_events()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    m_wants_close = true;
                    break;

                case SDL_CONTROLLERDEVICEADDED:
                {
                    SDL_GameController* ctrl = SDL_GameControllerOpen(event.cdevice.which);
                    if(ctrl)
                    {
                        SDL_Joystick* joy = SDL_GameControllerGetJoystick(ctrl);
                        int id = SDL_JoystickInstanceID(joy);
                        m_input_provider.m_game_controllers[id] = ctrl;
                    }
                    break;
                }

                case SDL_CONTROLLERDEVICEREMOVED:
                    if(m_input_provider.m_game_controllers.count(event.cdevice.which) > 0)
                    {
                        SDL_GameControllerClose(m_input_provider.m_game_controllers[event.cdevice.which]);
                        m_input_provider.m_game_controllers.erase(event.cdevice.which);
                    }
                    break;
            }
        }
    }

    bool sdl_window::wants_close() const
    {
        return m_wants_close;
    }

    int sdl_window::get_size_w() const
    {
        int w, tmp;
        SDL_GetWindowSize(m_window, &w, &tmp);
        return w;
    }

    int sdl_window::get_size_h() const
    {
        int h, tmp;
        SDL_GetWindowSize(m_window, &tmp, &h);
        return h;
    }

    int sdl_window::get_size_retina_w() const
    {
        int w, tmp;
        SDL_GL_GetDrawableSize(m_window, &w, &tmp);
        return w;
    }

    int sdl_window::get_size_retina_h() const
    {
        int h, tmp;
        SDL_GL_GetDrawableSize(m_window, &tmp, &h);
        return h;
    }

    void sdl_window::set_size(int w, int h)
    {
        if(!m_current_fullscreen)
            SDL_SetWindowSize(m_window, w, h);
    }

    void sdl_window::set_title(const std::string& title)
    {
        SDL_SetWindowTitle(m_window, title.c_str());
    }

    void sdl_window::set_vsync(bool vsync)
    {
        if(m_vsync_enabled != vsync)
            SDL_GL_SetSwapInterval(vsync ? 1 : 0);
        m_vsync_enabled = vsync;
    }

    void sdl_window::set_fullscreen(bool fullscreen)
    {
        if(fullscreen == m_current_fullscreen)
            return;
        SDL_SetWindowFullscreen(m_window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0x00);
        m_current_fullscreen = fullscreen;
    }

    void sdl_window::set_icon(std::uint8_t* rgba32_data, int w, int h)
    {
        // SDL interprets in native endian
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(rgba32_data, w, h, 32, w * 4, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
        SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(rgba32_data, w, h, 32, w * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
        SDL_SetWindowIcon(m_window, surface);
        SDL_FreeSurface(surface);
    }
}
