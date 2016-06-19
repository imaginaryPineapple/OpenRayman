#include <data_extractor/data_extractor.h>
#include <data_extractor/dsb/dsb_decompiler.h>
#include <platform/message_box.h>
#include <platform/file.h>
#include <json.hpp>
#include <iostream>
#include <fstream>

namespace openrayman
{
    bool data_extractor::extract(const std::string& install_folder)
    {
        // TODO: is this how c++ works even?!?
        return
            check_prerequisites(install_folder) &&
            create_base() &&
            decompile_game_dsb(install_folder);
    }

    bool data_extractor::check_prerequisites(const std::string& install_folder)
    {
        std::cout << "[openrayman::data_extractor] Checking prerequisites" << std::endl;
        if(file::exists(install_folder))
        {
            std::cout << "[openrayman::data_extractor] " << install_folder << " exists " << std::endl;
            if(file::exists(install_folder + "/Data"))
            {
                std::cout << "[openrayman::data_extractor] Data/ exists " << std::endl;
                if(file::exists(install_folder + "/Data/Game.dsb"))
                {
                    std::cout << "[openrayman::data_extractor] Game.dsb exists, all good " << std::endl;
                    return true;
                }
                else
                {
                    message_box::display("[openrayman::data_extractor] Error!", "The installation folder is not valid."
                        "\nReason: The \"Game.dsb\" file doesn't exist."
                        "\nThis file is the database for all dependencies"
                        "\nand is required for extracting the relevant data.", true);
                    return false;
                }
            }
            else
            {
                message_box::display("[openrayman::data_extractor] Error!", "The installation folder is not valid."
                    "\nReason: The \"Data\" folder doesn't exist.", true);
                return false;
            }
        }
        message_box::display("[openrayman::data_extractor] Error!", "The installation folder specified doesn't exist.", true);
        return false;
    }

    bool data_extractor::create_base()
    {
        std::cout << "[openrayman::data_extractor] Creating \"rayman2\" at " << file::fix_string(m_backend_specifics.get_data_path() + "/games/rayman2") << std::endl;
        file::create_directory(m_backend_specifics.get_data_path() + "/games/rayman2");

        nlohmann::json manifest;
        manifest["info"] = nlohmann::json::object();
        manifest["info"]["name"] = "Rayman 2: The Great Escape";
        manifest["info"]["description"] = "Developed by Ubisoft Montpellier, released in 1999";
        manifest["dependencies"] = nlohmann::json::array();

        std::cout << "[openrayman::data_extractor] Creating default manifest" << std::endl;
        std::cout << "                             info/name = " << manifest["info"]["name"] << std::endl;
        std::cout << "                             info/description = " << manifest["info"]["description"] << std::endl;
        std::cout << "                             dependencies = []" << std::endl;
        std::ofstream manifest_stream(file::fix_string(m_backend_specifics.get_data_path() + "/games/rayman2/manifest.json"));
        if(manifest_stream.is_open())
            manifest_stream << std::setw(4) << manifest;
        else
            message_box::display("[openrayman::data_extractor] Error!", "The manifest for \"rayman2\" could not be created.", true);
        return manifest_stream.is_open();
    }

    bool data_extractor::decompile_game_dsb(const std::string& install_folder)
    {
        std::cout << "[openrayman::data_extractor] Decompiling Game.dsb" << std::endl;
        dsb_decompiler decompiler;
        return decompiler.decompile_dsb(install_folder + "/Data/Game.dsb", m_backend_specifics.get_data_path() + "/games/rayman2/game.db");
    }
}
