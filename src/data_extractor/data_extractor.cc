#include <data_extractor/data_extractor.h>
#include <data_extractor/dsb/dsb_decompiler.h>
#include <data_extractor/cnt/cnt_archive.h>
#include <data_extractor/gf/gf_converter.h>
#include <platform/message_box.h>
#include <platform/file.h>
#include <json.hpp>
#include <iostream>
#include <fstream>

namespace openrayman
{
    bool data_extractor::extract(const std::string& install_folder)
    {
        if(check_prerequisites(install_folder) &&
            create_base() &&
            decompile_game_dsb(install_folder) &&
            make_game_resources(install_folder))
        {
            std::cout << "[openrayman::data_extractor] All tasks succeeded" << std::endl;
            return true;
        }
        else
        {
            file::delete_directory(m_backend_specifics.get_data_path() + "/games/rayman2");
            return false;
        }
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
        return decompiler.decompile_dsb(install_folder + "/Data/Game.dsb", m_backend_specifics.get_data_path() + "/games/rayman2", dsb_format::openrayman);
    }

    bool data_extractor::extract_recursive(std::string base, cnt_directory_node& parent)
    {
        gf_converter converter;
        for(cnt_file& file : parent.get_local_files())
        {
            std::cout << "Extracting " << file.name << " which is " << (file.size / 1024.0) << " KB... ";
            std::ofstream stream(file::fix_string(base + "/" + file.name.substr(0, file.name.length() - 3) + ".png"), std::ofstream::out | std::ofstream::binary);
            if(!stream.is_open())
                return false;

            std::vector<std::uint8_t> array;
            array.reserve(file.size);
            std::size_t at = 0;
            while(at < file.size)
            {
                std::vector<std::uint8_t> read = file.read(at, 1024 * 4096);
                for(char c : read)
                    array.push_back(c);
                at += read.size();
                if(read.size() == 0)
                    break;
            }
            std::cout << "OK" << std::endl;

            std::cout << "Converting " << file.name << " to png via gf_converter::convert_to_png... ";
            std::vector<std::uint8_t> data = converter.convert_to_png(array);
            stream.write((char*)data.data(), data.size());
            std::cout << "OK" << std::endl;
        }
        for(cnt_directory_node& child : parent.get_local_children())
        {
            file::create_directory(base + "/" + child.name);
            if(!extract_recursive(base + "/" + child.name, child))
                return false;
        }
        return true;
    }

    bool data_extractor::make_game_resources(const std::string& install_folder)
    {
        message_box::display("[openrayman::data_extractor] Info", "OpenRayman will now extract and convert all texture files used within the game."
            "\nThis can take a couple of minutes depending on your disk and CPU speed.", false);
        file::create_directory(m_backend_specifics.get_data_path() + "/games/rayman2/textures");
        file::create_directory(m_backend_specifics.get_data_path() + "/games/rayman2/textures/vignettes");
        cnt_archive textures(install_folder + "/Data/Textures.cnt");
        cnt_archive vignettes(install_folder + "/Data/Vignette.cnt");
        if(!textures.valid())
        {
            message_box::display("[openrayman::data_extractor] Error!", "The texture archive could not be read.", true);
            return false;
        }
        if(!vignettes.valid())
        {
            message_box::display("[openrayman::data_extractor] Error!", "The texture archive containing vignettes could not be read.", true);
            return false;
        }
        if(!extract_recursive(m_backend_specifics.get_data_path() + "/games/rayman2/textures", textures.get_archive_node()))
        {
            message_box::display("[openrayman::data_extractor] Error!", "A texture file could not be extracted or read.", true);
            return false;
        }
        if(!extract_recursive(m_backend_specifics.get_data_path() + "/games/rayman2/textures/vignettes", vignettes.get_archive_node()))
        {
            message_box::display("[openrayman::data_extractor] Error!", "A texture file could not be extracted or read.", true);
            return false;
        }
        return true;
    }
}
