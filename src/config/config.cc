#include <config/config.h>
#include <platform/file.h>
#include <json.hpp>
#ifndef USE_LIBRETRO
#include <SDL2/SDL.h>
#endif
#include <iostream>
#include <fstream>

namespace openrayman
{
    config::config(const backend_specifics& backend_specifics) :
        m_backend_specifics(backend_specifics),
        vsync(true),
        fullscreen(false),
        max_fps(0),
        game("rayman2_openrayman")
    {
#ifndef USE_LIBRETRO
        keyboard_map["stick(strength)"] = SDL_GetScancodeName(SDL_SCANCODE_LSHIFT);

        keyboard_map["stick(x, -)"] = SDL_GetScancodeName(SDL_SCANCODE_LEFT);
        keyboard_map["stick(x, +)"] = SDL_GetScancodeName(SDL_SCANCODE_RIGHT);
        keyboard_map["stick(y, -)"] = SDL_GetScancodeName(SDL_SCANCODE_UP);
        keyboard_map["stick(y, +)"] = SDL_GetScancodeName(SDL_SCANCODE_DOWN);

        keyboard_map["start"] = SDL_GetScancodeName(SDL_SCANCODE_ESCAPE);

        keyboard_map["a"] = SDL_GetScancodeName(SDL_SCANCODE_A);
        keyboard_map["b"] = SDL_GetScancodeName(SDL_SCANCODE_SPACE);

        keyboard_map["l"] = SDL_GetScancodeName(SDL_SCANCODE_LCTRL);
        keyboard_map["r"] = SDL_GetScancodeName(SDL_SCANCODE_F1);
        keyboard_map["z"] = SDL_GetScancodeName(SDL_SCANCODE_J);

        keyboard_map["cbtn(x, -)"] = SDL_GetScancodeName(SDL_SCANCODE_Q);
        keyboard_map["cbtn(x, +)"] = SDL_GetScancodeName(SDL_SCANCODE_W);
        keyboard_map["cbtn(y, -)"] = SDL_GetScancodeName(SDL_SCANCODE_O);
        keyboard_map["cbtn(y, +)"] = SDL_GetScancodeName(SDL_SCANCODE_END);
#endif
        if(!reload())
            save();
    }

    config::~config()
    {
        save();
    }

    bool config::reload()
    {
        std::string config_json_file = file::fix_string(m_backend_specifics.storage_path() + "/config.json");
        if(file::exists(config_json_file))
        {
            nlohmann::json config_json;
            std::ifstream config_stream(config_json_file);
            if(config_stream.is_open())
            {
                config_stream >> config_json;
                if(config_json.count("game") > 0)
                    game = config_json["game"];
                if(config_json.count("renderer") > 0)
                {
                    vsync = config_json["renderer"]["vsync"];
                    fullscreen = config_json["renderer"]["fullscreen"];
                    max_fps = config_json["renderer"]["max_fps"];
                }
                if(config_json.count("input") > 0 && config_json["input"].count("keyboard") > 0)
                {
                    keyboard_map["stick(strength)"] = config_json["input"]["keyboard"]["stick(strength)"];

                    keyboard_map["stick(x, -)"] = config_json["input"]["keyboard"]["stick(x, -)"];
                    keyboard_map["stick(x, +)"] = config_json["input"]["keyboard"]["stick(x, +)"];
                    keyboard_map["stick(y, -)"] = config_json["input"]["keyboard"]["stick(y, -)"];
                    keyboard_map["stick(y, +)"] = config_json["input"]["keyboard"]["stick(y, +)"];

                    keyboard_map["start"] = config_json["input"]["keyboard"]["start"];

                    keyboard_map["a"] = config_json["input"]["keyboard"]["a"];
                    keyboard_map["b"] = config_json["input"]["keyboard"]["b"];

                    keyboard_map["l"] = config_json["input"]["keyboard"]["l"];
                    keyboard_map["r"] = config_json["input"]["keyboard"]["r"];
                    keyboard_map["z"] = config_json["input"]["keyboard"]["z"];

                    keyboard_map["cbtn(x, -)"] = config_json["input"]["keyboard"]["cbtn(x, -)"];
                    keyboard_map["cbtn(x, +)"] = config_json["input"]["keyboard"]["cbtn(x, +)"];
                    keyboard_map["cbtn(y, -)"] = config_json["input"]["keyboard"]["cbtn(y, -)"];
                    keyboard_map["cbtn(y, +)"] = config_json["input"]["keyboard"]["cbtn(y, +)"];
                }
                return true;
            }
        }
        std::cout << "Warning: config reload from " << config_json_file << " failed!" << std::endl;
        return false;
    }

    bool config::save()
    {
        std::string config_json_file = file::fix_string(m_backend_specifics.storage_path() + "/config.json");
        nlohmann::json config_json;
        config_json["game"] = game;

        config_json["renderer"] = nlohmann::json::object();

        config_json["renderer"]["vsync"] = vsync;
        config_json["renderer"]["fullscreen"] = fullscreen;
        config_json["renderer"]["max_fps"] = max_fps;

        config_json["input"] = nlohmann::json::object();
        config_json["input"]["keyboard"] = nlohmann::json::object();

        config_json["input"]["keyboard"]["stick(strength)"] = keyboard_map["stick(strength)"];

        config_json["input"]["keyboard"]["stick(x, -)"] = keyboard_map["stick(x, -)"];
        config_json["input"]["keyboard"]["stick(x, +)"] = keyboard_map["stick(x, +)"];
        config_json["input"]["keyboard"]["stick(y, -)"] = keyboard_map["stick(y, -)"];
        config_json["input"]["keyboard"]["stick(y, +)"] = keyboard_map["stick(y, +)"];

        config_json["input"]["keyboard"]["start"] = keyboard_map["start"];

        config_json["input"]["keyboard"]["a"] = keyboard_map["a"];
        config_json["input"]["keyboard"]["b"] = keyboard_map["b"];

        config_json["input"]["keyboard"]["l"] = keyboard_map["l"];
        config_json["input"]["keyboard"]["r"] = keyboard_map["r"];
        config_json["input"]["keyboard"]["z"] = keyboard_map["z"];

        config_json["input"]["keyboard"]["cbtn(x, -)"] = keyboard_map["cbtn(x, -)"];
        config_json["input"]["keyboard"]["cbtn(x, +)"] = keyboard_map["cbtn(x, +)"];
        config_json["input"]["keyboard"]["cbtn(y, -)"] = keyboard_map["cbtn(y, -)"];
        config_json["input"]["keyboard"]["cbtn(y, +)"] = keyboard_map["cbtn(y, +)"];

        std::ofstream config_stream(config_json_file, std::ofstream::trunc);
        if(config_stream.is_open())
            config_stream << std::setw(4) << config_json;
        else
            std::cout << "Warning: config save failed at " << config_json_file << "!" << std::endl;
        return config_stream.is_open();
    }
}
