#include <data_extractor.h>
#include <openrayman/dsb_script.h>
#include <openrayman/cnt_archive.h>
#include <openrayman/gf_texture.h>
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
            std::cout << "[openrayman::data_extractor::extract] All tasks succeeded" << std::endl;
            return true;
        }
        else
        {
            message_box::display("[openrayman::data_extractor::extract] Error!", "An unknown error occured.", true);
            file::delete_directory(m_backend_specifics.data_path() + "/games/rayman2");
            return false;
        }
    }

    bool data_extractor::check_prerequisites(const std::string& install_folder)
    {
        std::cout << "[openrayman::data_extractor::check_prerequisites] Checking prerequisites" << std::endl;
        if(file::exists(install_folder))
        {
            std::cout << "[openrayman::data_extractor::check_prerequisites] " << install_folder << " exists " << std::endl;
            if(file::exists(install_folder + "/Data"))
            {
                std::cout << "[openrayman::data_extractor::check_prerequisites] Data/ exists " << std::endl;
                if(file::exists(install_folder + "/Data/Game.dsb"))
                {
                    std::cout << "[openrayman::data_extractor::check_prerequisites] Game.dsb exists, all good " << std::endl;
                    return true;
                }
                else
                {
                    message_box::display("[openrayman::data_extractor::check_prerequisites] Error!", "The installation folder is not valid."
                        "\nReason: The \"Game.dsb\" file doesn't exist."
                        "\nThis file is the database for all dependencies"
                        "\nand is required for extracting the relevant data.", true);
                    return false;
                }
            }
            else
            {
                message_box::display("[openrayman::data_extractor::check_prerequisites] Error!", "The installation folder is not valid."
                    "\nReason: The \"Data\" folder doesn't exist.", true);
                return false;
            }
        }
        message_box::display("[openrayman::data_extractor::check_prerequisites] Error!", "The installation folder specified doesn't exist.", true);
        return false;
    }

    bool data_extractor::create_base()
    {
        std::cout << "[openrayman::data_extractor::create_base] Creating \"rayman2\" at " << file::fix_string(m_backend_specifics.data_path() + "/games/rayman2") << std::endl;
        file::create_directory(m_backend_specifics.data_path() + "/games/rayman2");

        nlohmann::json manifest;
        manifest["info"] = nlohmann::json::object();
        manifest["info"]["name"] = "Rayman 2: The Great Escape";
        manifest["info"]["description"] = "Developed by Ubisoft Montpellier, released in 1999";
        manifest["dependencies"] = nlohmann::json::array();

        std::cout << "[openrayman::data_extractor::create_base] Creating default manifest" << std::endl;
        std::cout << "                             info/name = " << manifest["info"]["name"] << std::endl;
        std::cout << "                             info/description = " << manifest["info"]["description"] << std::endl;
        std::cout << "                             dependencies = []" << std::endl;
        std::ofstream manifest_stream(file::fix_string(m_backend_specifics.data_path() + "/games/rayman2/manifest.json"));
        if(manifest_stream.is_open())
            manifest_stream << std::setw(4) << manifest;
        else
            message_box::display("[openrayman::data_extractor::create_base] Error!", "The manifest for \"rayman2\" could not be created.", true);
        return manifest_stream.is_open();
    }

    bool data_extractor::decompile_game_dsb(const std::string& install_folder)
    {
        std::cout << "[openrayman::data_extractor::decompile_game_dsb] Decompiling Game.dsb" << std::endl;
        std::ifstream script_stream(install_folder + "/Data/Game.dsb");
        dsb_script script(script_stream);
        if(!script.valid())
            return false;
        return script.decompile(m_backend_specifics.data_path() + "/games/rayman2");
    }

    bool data_extractor::extract_recursive(std::string base, cnt_directory_node& parent)
    {
        for(cnt_file& file : parent.local_files())
        {
            std::cout << "Extracting " << file.name << " which is " << (file.size / 1024.0) << " KB... ";
            std::ofstream stream(file::fix_string(base + "/" + file.name.substr(0, file.name.length() - 3) + ".png"), std::ofstream::binary);
            if(!stream.is_open())
                return false;

            std::vector<char> array;
            array.reserve(file.size);
            std::size_t at = 0;
            while(at < file.size)
            {
                std::vector<char> read = file.read(at, 1024 * 4096);
                for(char c : read)
                    array.push_back(c);
                at += read.size();
                if(read.size() == 0)
                    break;
            }
            std::cout << "OK" << std::endl;
            std::cout << "Converting " << file.name << " to png via gf_texture::convert_to_png... ";
            common::in_mem_istream texture_stream(array);
            gf_texture texture(texture_stream);
            if(!texture.convert_to_png(stream))
                return false;
            std::cout << "OK" << std::endl;
        }
        for(cnt_directory_node& child : parent.local_children())
        {
            file::create_directory(base + "/" + child.name);
            if(!extract_recursive(base + "/" + child.name, child))
                return false;
        }
        return true;
    }

    bool data_extractor::make_game_resources(const std::string& install_folder)
    {
        message_box::display("[openrayman::data_extractor::make_game_resources] Info", "OpenRayman will now extract and convert all texture files used within the game."
            "\nThis can take a couple of minutes depending on your disk and CPU speed.", false);
        file::create_directory(m_backend_specifics.data_path() + "/games/rayman2/textures");
        file::create_directory(m_backend_specifics.data_path() + "/games/rayman2/textures/vignettes");
        std::ifstream textures_stream(install_folder + "/Data/Textures.cnt", std::ifstream::binary);
        std::ifstream vignettes_stream(install_folder + "/Data/Vignette.cnt", std::ifstream::binary);
        cnt_archive textures(textures_stream);
        cnt_archive vignettes(vignettes_stream  );
        if(!textures.valid())
        {
            message_box::display("[openrayman::data_extractor::make_game_resources] Error!", "The texture archive could not be read.", true);
            return false;
        }
        if(!vignettes.valid())
        {
            message_box::display("[openrayman::data_extractor::make_game_resources] Error!", "The texture archive containing vignettes could not be read.", true);
            return false;
        }
        if(!extract_recursive(m_backend_specifics.data_path() + "/games/rayman2/textures", textures.archive_node()))
        {
            message_box::display("[openrayman::data_extracto::make_game_resourcesr] Error!", "A texture file could not be extracted or read.", true);
            return false;
        }
        if(!extract_recursive(m_backend_specifics.data_path() + "/games/rayman2/textures/vignettes", vignettes.archive_node()))
        {
            message_box::display("[openrayman::data_extractor::make_game_resources] Error!", "A texture file could not be extracted or read.", true);
            return false;
        }
        return true;
    }
}
