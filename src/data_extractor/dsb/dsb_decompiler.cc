#include <data_extractor/dsb/dsb_decompiler.h>
#include <data_extractor/data_decoder.h>
#include <info.h>
#include <cstdint>
#include <unordered_map>
#include <iomanip>
#include <json.hpp>

namespace openrayman
{
    bool dsb_decompiler::decompile_dsb(const std::string& source, const std::string& target, dsb_format fmt)
    {
        std::cout << "[openrayman::dsb_decompiler] Reading dsb " << source << " to " << target << std::endl;
        std::ifstream source_stream(source, std::ifstream::in | std::ifstream::binary);
        if(source_stream.is_open())
        {
            source_stream.seekg(0, source_stream.end);
            std::size_t length = source_stream.tellg();
            // Skip header
            source_stream.seekg(4);
            // This will NEVER happen with valid dsbs
            if(length <= 4)
                return false;

            // Reading the whole dsb should be ok, they aren't that big
            char buffer[length - 4];
            source_stream.read(buffer, length - 4);
            data_decoder decoder;
            // This is for documentation
            // data_decoder starts at 0
            // The header is not encoded
            decoder.set_virtual_position(0);
            decoder.decode_array(buffer, length - 4);

            if(fmt == dsb_format::rayman2_decoded)
            {
                std::ofstream target_stream(target, std::ofstream::out | std::ofstream::binary);
                if(target_stream.is_open())
                    target_stream.write(buffer, length - 4);
                return target_stream.is_open();
            }
            else
                decompile_sections(buffer, length - 4, target);
            return true;
        }
        return false;
    }

    struct memorybuf : std::streambuf
    {
        memorybuf(char* begin, char* end)
        {
            setg(begin, begin, end);
        }
    };

    void dsb_decompiler::decompile_sections(char* source, std::size_t source_length, const std::string& target)
    {
        memorybuf streambuf(source, source + source_length);
        std::istream in(&streambuf);
        std::int32_t id;
        in.read((char*)&id, sizeof(std::int32_t));
        while(!in.eof() && id != 0xFFFF)
        {

            #define DECOMPILE_SECTION(id, name, function) \
                case id: \
                { \
                    std::cout << "[openrayman::dsb_decompiler] Decompiling " \
                        << std::hex << "0x" << std::setfill('0') << std::setw(2) << id\
                        << " (decimal " << std::dec << id << ") \"" \
                        << name << "\"" << std::endl; \
                    function(in, target); \
                    break; \
                }

            switch(id)
            {
                // different sections of the dsb file
                DECOMPILE_SECTION(0x00, "alloc", decompile_alloc);
                DECOMPILE_SECTION(0x1E, "levels", decompile_lvl_list);
                DECOMPILE_SECTION(0x28, "data_directories", decompile_data_directories);
                DECOMPILE_SECTION(0x20, "unknown_blob_0x20", decompile_unknown_blob_0x20);
                DECOMPILE_SECTION(0x46, "vignette", decompile_vignette);
                DECOMPILE_SECTION(0x40, "texture_files", decompile_texture_files);
                DECOMPILE_SECTION(0x6E, "unknown_blob_0x6e", decompile_unknown_blob_0x6e);
                DECOMPILE_SECTION(0x64, "game_options", decompile_game_options);
                DECOMPILE_SECTION(0x5A, "sound_banks", decompile_sound_banks);
                DECOMPILE_SECTION(0x5C, "load_sound_banks", decompile_load_sound_banks);
                default:
                {
                    std::cout << "[openrayman::dsb_decompiler] Encountered unknown id 0x"
                        << std::hex << id
                        << " (decimal " << std::dec << id << ")"
                        << std::endl;
                    break;
                }
            }
            in.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_alloc(std::istream& source, const std::string& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            if(id == 0x10)
            {
                std::int32_t a, b;
                source.read((char*)&a, sizeof(std::int32_t));
                source.read((char*)&b, sizeof(std::int32_t));
            }
            else
            {
                std::int32_t a;
                source.read((char*)&a, sizeof(std::int32_t));
                if(a == 0xFFFF)
                    return;
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_lvl_list(std::istream& source, const std::string& target)
    {
        std::ofstream levels_file(target + "/levels.json");
        nlohmann::json levels_json;
        levels_json["levels"] = nlohmann::json::array();
        levels_json["initial"] = "";
        std::int32_t num_levels;
        source.read((char*)&num_levels, sizeof(std::int32_t));
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            if(id == 0x1F)
            {
                std::string str = read_string_with_length_u16(source);
                // levels have four spaced appended for some reason, trim them
                str.erase(str.find_last_not_of(" \t") + 1);
                // this seems to be the case for all games, makes sense too
                if(levels_json["initial"] == "")
                    levels_json["initial"] = str;
                levels_json["levels"].push_back(str);
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
        if(levels_file.is_open())
            levels_file << std::setw(4) << levels_json;
    }

    void dsb_decompiler::decompile_data_directories(std::istream& source, const std::string& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            std::string dir = "";

#define TRANSLATE_DATA_ID(id, str) \
                case (id): \
                    dir = (str); \
                    break;

            switch(id)
            {
                TRANSLATE_DATA_ID(41, "dll");
                TRANSLATE_DATA_ID(42, "root");
                TRANSLATE_DATA_ID(43, "world");
                TRANSLATE_DATA_ID(44, "levels");
                TRANSLATE_DATA_ID(45, "sound");
                TRANSLATE_DATA_ID(46, "saves");
                TRANSLATE_DATA_ID(48, "vignette");
                TRANSLATE_DATA_ID(49, "options");
            }
            std::uint16_t str_length;
            source.read((char*)&str_length, sizeof(std::uint16_t));
            char str[str_length];
            source.read(str, str_length);
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_unknown_blob_0x20(std::istream& source, const std::string& target)
    {
        std::cout << "[openrayman::dsb_decompiler] Warning! encountered 0x20" << std::endl;
        std::uint32_t size;
        source.read((char*)&size, sizeof(std::uint32_t));
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        std::unordered_map<std::int32_t, int> encountered_ids;
        while(id != 0xFFFF)
        {
            if(encountered_ids.count(id) == 0)
                encountered_ids[id] = 0;
            encountered_ids[id]++;
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_vignette(std::istream& source, const std::string& target)
    {
        std::ofstream vignette_file(target + "/vignette.json");
        nlohmann::json vignette_json;
        vignette_json["bar"] = nlohmann::json::object();
        vignette_json["image"] = "";
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            switch(id)
            {
                case 71:
                case 72:
                {
                    std::string str = read_string_with_length_u16(source);
                    if(str.find_first_of("Random<") == 0)
                    {
                        // this is supposed to select a random vignette
                        // but we just use the first one
                        str = str.substr(7, std::string::npos);
                        str = str.substr(0, str.find_first_of(','));
                    }
                    if(str.find_last_of(".bmp") != std::string::npos)
                        str = str.substr(0, str.find_last_of(".bmp") - 3);
                    vignette_json["image"] = str;
                    break;
                }
                case 75:
                {
                    // display
                    break;
                }
                case 76:
                case 77:
                {
                    std::int32_t tmp;
                    // skip these three
                    // TODO
                    for(int n = 0; n < 3; n++)
                        source.read((char*)&tmp, sizeof(std::int32_t));
                    std::uint8_t r, g, b, a;
                    source.read((char*)&r, 1); source.read((char*)&g, 1); source.read((char*)&b, 1); source.read((char*)&a, 1);
                    if(id == 76)
                        vignette_json["bar"]["outside"] = { r, g, b, a };
                    else
                        vignette_json["bar"]["inside"] = { r, g, b, a };
                    break;
                }
                case 78:
                {
                    std::uint8_t colors[(sizeof(std::uint8_t) * 4) * 4];
                    source.read((char*)colors, (sizeof(std::uint8_t) * 4) * 4);
                    vignette_json["bar"]["main"] = nlohmann::json::object();
                    // TODO: are these coordinates right?!?
                    vignette_json["bar"]["main"]["(x, -)"] = { colors[0], colors[1], colors[2], colors[3] };
                    vignette_json["bar"]["main"]["(y, -)"] = { colors[4], colors[5], colors[6], colors[7] };
                    vignette_json["bar"]["main"]["(x, +)"] = { colors[8], colors[9], colors[10], colors[11] };
                    vignette_json["bar"]["main"]["(y, +)"] = { colors[12], colors[13], colors[14], colors[15] };
                    break;
                }
                case 79:
                {
                    std::int32_t vec[4];
                    source.read((char*)vec, sizeof(std::int32_t) * 4);
                    vignette_json["bar"]["pos"] = { vec[0], vec[1], vec[2], vec[3] };
                    break;
                }
                case 80:
                {
                    // add_bar
                    break;
                }
                case 81:
                {
                    std::int32_t max;
                    source.read((char*)&max, sizeof(std::int32_t));
                    vignette_json["bar"]["max"] = max;
                    break;
                }
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
        if(vignette_file.is_open())
            vignette_file << std::setw(4) << vignette_json;
    }

    void dsb_decompiler::decompile_texture_files(std::istream& source, const std::string& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            read_string_with_length_u16(source);
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_unknown_blob_0x6e(std::istream& source, const std::string& target)
    {
        std::cout << "[openrayman::dsb_decompiler] Warning! encountered 0x6e" << std::endl;
        std::uint8_t tmp = 0x00;
        while(tmp != 0xFF)
            source.read((char*)&tmp, 1);
        for(int n = 0; n < 3; n++)
            source.read((char*)&tmp, 1);
    }

    void dsb_decompiler::decompile_game_options(std::istream& source, const std::string& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            switch(id)
            {
                case 0x65:
                case 0x66:
                {
                    read_string_with_length_u16(source);
                    break;
                }
                case 0x67:
                {
                    std::int16_t tmp;
                    source.read((char*)&tmp, sizeof(std::int16_t));
                    break;
                }
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_sound_banks(std::istream& source, const std::string& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            if(id == 92)
                decompile_load_sound_banks(source, target);
            else
            {
                std::int32_t unknown;
                source.read((char*)&unknown, sizeof(std::int32_t));
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    void dsb_decompiler::decompile_load_sound_banks(std::istream& source, const std::string& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            if(id == 94)
            {
                std::int32_t bank;
                source.read((char*)&bank, sizeof(std::int32_t));
            }
            else
            {
                std::int32_t size;
                source.read((char*)&size, sizeof(std::int32_t));
                char bytes[size * 4];
                source.read((char*)&bytes, size * 4);
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    const std::string dsb_decompiler::read_string_null_terminated(std::istream& source)
    {
        std::stringstream target;
        char c = 0x00;
        source.read(&c, 1);
        while(c != '\0')
        {
            target << c;
            source.read(&c, 1);
        }
        return target.str();
    }

    const std::string dsb_decompiler::read_string_with_length_u16(std::istream& source)
    {
        std::uint16_t length;
        source.read((char*)&length, sizeof(std::uint16_t));
        char str[length];
        source.read(str, length);
        return std::string(str);
    }
}
