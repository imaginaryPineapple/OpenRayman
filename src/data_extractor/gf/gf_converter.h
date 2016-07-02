#ifndef GF_CONVERTER_H
#define GF_CONVERTER_H

#include <vector>
#include <iostream>
#include <cstdint>

namespace openrayman
{
    // GF (graphics texture) files are used as textures.
    // This is all heavily adapted from Rayman2Lib.
    class gf_converter
    {
public:
        // Returns true if information about the GF file could be extracted.
        bool get_gf_info(std::vector<std::uint8_t>& in, std::size_t* w, std::size_t* h, std::uint8_t* num_channels, std::uint8_t* repeat_byte);

        // Converts a GF file in memory into a raw array of bytes, consisting of a RGBA32 image.
        // This function will return an array of size 0 if it fails.
        // Alternatively, check w or h if they have a value of 0.
        std::vector<std::uint8_t> convert_to_rgba(std::vector<std::uint8_t>& in, std::size_t* w, std::size_t* h);

        // Converts a GF file in memory into a raw array of bytes, consisting of a PNG image.
        std::vector<std::uint8_t> convert_to_png(std::vector<std::uint8_t>& in);
private:
        void read_channel(std::istream& data, std::uint8_t repeat_byte, std::vector<std::uint8_t>& channel);
    };
}

#endif
