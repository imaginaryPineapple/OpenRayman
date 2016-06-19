#include <data_extractor/dsb/dsb_decompiler.h>
#include <data_extractor/data_decoder.h>
#include <cstdint>

namespace openrayman
{
    bool dsb_decompiler::decompile_dsb(const std::string& source, const std::string& target)
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

            // TODO: decompile DSB

            std::ofstream target_stream(target, std::ofstream::out | std::ofstream::binary);
            if(target_stream.is_open())
            {
                decompile_ids(buffer, length - 4, target_stream);
                return true;
            }
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

    // different "sections"?!?
    void dsb_decompiler::decompile_ids(char* source, std::size_t source_length, std::ofstream& target)
    {
        memorybuf streambuf(source, source + source_length);
        std::istream in(&streambuf);
        std::int32_t id;
        in.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {

#define DECOMPILE_ID(id, name, function) \
                case (id): \
                    target << "section " << (name) << "\n"; \
                    target << "{\n"; \
                    (function)(in, target); \
                    target << "}\n\n"; \
                    break;

            switch(id)
            {
                DECOMPILE_ID(0x00, "alloc", decompile_alloc);
                DECOMPILE_ID(0x1E, "levels", decompile_lvl_list);
                DECOMPILE_ID(0x28, "data_directories", decompile_data_directories);
                case 0x20:
                    break;
                case 0x46:
                    break;
                case 0x40:
                    break;
                case 0x5A:
                    break;
                case 0x64:
                    break;
                case 0x5C:
                    break;
                case 0x6E:
                    break;
                case 0x78:
                    break;
            }
            in.read((char*)&id, sizeof(std::int32_t));
        }
    }

    // this allocates variables?!?!
    void dsb_decompiler::decompile_alloc(std::istream& source, std::ofstream& target)
    {
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            if(id == 0x10)
            {
                // skip(?, ?);
                target << "    skip(";
                std::int32_t a, b;
                source.read((char*)&a, sizeof(std::int32_t));
                source.read((char*)&b, sizeof(std::int32_t));
                target << std::to_string(a) << ", " << std::to_string(b) << ");\n";
            }
            else
            {
                // alloc(slot, value);
                target << "    alloc(" << std::to_string(id) << ", ";
                std::int32_t a;
                source.read((char*)&a, sizeof(std::int32_t));
                if(a == 0xFFFF)
                    return;
                target << std::to_string(a) << ");\n";
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    // Adds levels
    void dsb_decompiler::decompile_lvl_list(std::istream& source, std::ofstream& target)
    {
        std::int32_t temp;
        source.read((char*)&temp, sizeof(std::int32_t));
        std::int32_t id;
        source.read((char*)&id, sizeof(std::int32_t));
        while(id != 0xFFFF)
        {
            if(id == 0x1F)
            {
                // add(name);
                target << "    add(\"";
                char c;
                source.read(&c, 1);
                while(c != '\0')
                {
                    target << c;
                    source.read(&c, 1);
                }
                target << "    \");";
            }
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }

    // Data directories
    void dsb_decompiler::decompile_data_directories(std::istream& source, std::ofstream& target)
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
            // dir(name, path);
            if(dir != "")
                target << "    dir(" << dir << ", \"" << str << "\");\n";
            source.read((char*)&id, sizeof(std::int32_t));
        }
    }
}
