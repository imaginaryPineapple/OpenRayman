#include <tools/gf_tools.h>
#include <tools/common_tools.h>
#include <openrayman/common_stream.h>
#include <openrayman/gf_texture.h>
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
            std::ifstream gf_stream(path, std::ifstream::binary);
            std::ofstream png_stream(target, std::ofstream::binary);
            if(!gf_stream.is_open() || !png_stream.is_open())
                return fail_and_print("Operation failed");

            gf_texture texture(gf_stream);
            if(!texture.valid())
                return fail_and_print("Operation failed");

            common::in_mem_ostream rgba_data(texture.required_size_rgba());
            if(!texture.convert_to_rgba(rgba_data))
                return fail_and_print("Operation failed");

            unsigned int error = lodepng::encode(
                    target,
                    (unsigned char*)rgba_data.mem(),
                    texture.info().w,
                    texture.info().h);
            if(error)
                return fail_and_print("Operation failed");

            return EXIT_SUCCESS;
        }

        int print_info(const std::string& path)
        {
            std::ifstream gf_stream(path, std::ifstream::binary);
            if(!gf_stream.is_open())
                return fail_and_print("Operation failed");

            gf_texture texture(gf_stream);
            if(!texture.valid())
                return fail_and_print("Operation failed");

            std::cout << "Width: " << texture.info().w << std::endl;
            std::cout << "Height: " << texture.info().h << std::endl;
            std::cout << "Channels: " << (texture.info().channels == 4 ? "Blue, Green, Red, Alpha" : "Blue, Green, Red") << std::endl;
            std::cout << "Repeat byte (for compression): 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)texture.info().repeat_byte << std::endl;
            return EXIT_SUCCESS;
        }
    }
}
