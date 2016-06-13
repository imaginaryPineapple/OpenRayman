#include <config/config.h>
#include <platform/file.h>
#include <iostream>
#include <fstream>

namespace openrayman
{
    config::config(const info& static_info, const backend_specifics& backend_specifics) :
        m_backend_specifics(backend_specifics),
        vsync(true),
        fullscreen(false),
        max_fps(0),
        game(static_info.game)
    {
        if(!reload())
            save();
    }

    config::~config()
    {
        save();
    }

    bool config::reload()
    {
        std::string config_json_file = file::fix_string(m_backend_specifics.get_storage_path() + "/config.json");
        if(file::exists(config_json_file))
        {
            nlohmann::json config_json;
            std::ifstream config_stream(config_json_file);
            if(config_stream.is_open())
            {
                config_stream >> config_json;
                if(config_json.count("vsync") > 0)
                    vsync = config_json["vsync"];
                if(config_json.count("fullscreen") > 0)
                    fullscreen = config_json["fullscreen"];
                if(config_json.count("max_fps") > 0)
                    max_fps = config_json["max_fps"];
                if(config_json.count("game") > 0)
                    game = config_json["game"];
                return true;
            }
        }
        std::cout << "Warning: config reload from " << config_json_file << " failed!" << std::endl;
        return false;
    }

    bool config::save() const
    {
        std::string config_json_file = file::fix_string(m_backend_specifics.get_storage_path() + "/config.json");
        nlohmann::json config_json;
        config_json["vsync"] = vsync;
        config_json["fullscreen"] = fullscreen;
        config_json["max_fps"] = max_fps;
        config_json["game"] = game;
        std::ofstream config_stream(config_json_file, std::ofstream::out | std::ofstream::trunc);
        if(config_stream.is_open())
            config_stream << std::setw(4) << config_json;
        else
            std::cout << "Warning: config save failed at " << config_json_file << "!" << std::endl;
        return config_stream.is_open();
    }
}
