#include <config/info.h>
#include <platform/file.h>
#include <iostream>
#include <fstream>

namespace openrayman
{
    info::info(const backend_specifics& backend_specifics) :
        m_backend_specifics(backend_specifics),
        m_loaded(false),
        game("?"),
        base_w(0),
        base_h(0),
        updates_per_second(0)
    {
        std::string info_json_file = file::fix_string(m_backend_specifics.get_data_path() + "/common/info.json");
        if(file::exists(info_json_file))
        {
            nlohmann::json info_json;
            std::ifstream info_stream(info_json_file);
            if(info_stream.is_open())
            {
                info_stream >> info_json;
                if(info_json.count("game") > 0)
                    game = info_json["game"];
                if(info_json.count("base_w") > 0)
                    base_w = info_json["base_w"];
                if(info_json.count("base_h") > 0)
                    base_h = info_json["base_h"];
                if(info_json.count("updates_per_second") > 0)
                    updates_per_second = info_json["updates_per_second"];
                m_loaded = true;
                return;
            }
        }
        std::cout << "Warning: info reload from " << info_json_file << " failed!" << std::endl;
    }
}
