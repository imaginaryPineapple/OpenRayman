#include <tools/gf_tools.h>
#include <data_extractor/gf/gf_converter.h>
#include <tools/common_tools.h>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>

namespace openrayman
{
    namespace gf_tools
    {
        int convert_to_png(const std::string& path, const std::string& target)
        {
            openrayman::gf_converter converter;

            std::ifstream gf_stream(path, std::ifstream::in | std::ifstream::binary);
            if(!gf_stream.is_open())
                return fail_and_print("Operation failed");

            gf_stream.seekg(0, std::ifstream::end);
            std::size_t size = gf_stream.tellg();
            gf_stream.seekg(0);

            std::uint8_t gf_data[size];
            gf_stream.read((char*)gf_data, size);

            std::vector<std::uint8_t> vector(gf_data, gf_data + size);
            std::vector<std::uint8_t> png_data = converter.convert_to_png(vector);
            if(png_data.size() == 0)
                return fail_and_print("Operation failed");
            std::cout << std::endl;
            std::ofstream png_stream(target, std::ofstream::out | std::ofstream::binary);
            if(!png_stream.is_open())
                return fail_and_print("Operation failed");

            return png_stream.write((char*)png_data.data(), png_data.size()) ? EXIT_SUCCESS : EXIT_FAILURE;
        }

        int print_info(const std::string& path)
        {
            openrayman::gf_converter converter;

            std::ifstream gf_stream(path, std::ifstream::in | std::ifstream::binary);
            if(!gf_stream.is_open())
                return fail_and_print("Operation failed");

            gf_stream.seekg(0, std::ifstream::end);
            std::size_t size = gf_stream.tellg();
            gf_stream.seekg(0);

            std::uint8_t gf_data[size];
            gf_stream.read((char*)gf_data, size);

            std::size_t w, h;
            std::uint8_t num_channels, repeat_byte;
            std::vector<std::uint8_t> vector(gf_data, gf_data + size);
            if(!converter.get_gf_info(vector, &w, &h, &num_channels, &repeat_byte))
                return fail_and_print("Operation failed");

            std::cout << "Width: " << w << std::endl;
            std::cout << "Height: " << h << std::endl;
            std::cout << "Channels: " << (num_channels == 4 ? "Blue, Green, Red, Alpha" : "Blue, Green, Red") << std::endl;
            std::cout << "Repeat byte (for compression): 0x" << std::hex << std::setfill('0') << std::setw(2) << (std::int32_t)repeat_byte << std::endl;
            return EXIT_SUCCESS;
        }
    }
}
