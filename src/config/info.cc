#include <config/info.h>
#include <platform/file.h>
#include <iostream>
#include <fstream>

namespace openrayman
{
    info::info(const backend_specifics& backend_specifics) :
        m_backend_specifics(backend_specifics),
        m_loaded(false),
        default_game("?")
    {
        std::string info_json_file = file::fix_string(m_backend_specifics.get_data_path() + "/common/info.json");
        if(file::exists(info_json_file))
        {
            nlohmann::json info_json;
            std::ifstream info_stream(info_json_file);
            if(info_stream.is_open())
            {
                info_stream >> info_json;
                if(info_json.count("default_game") > 0)
                    default_game = info_json["default_game"];
                else
                    return;
                m_loaded = true;
                return;
            }
        }
        std::cout << "Warning: info reload from " << info_json_file << " failed!" << std::endl;
    }
}
