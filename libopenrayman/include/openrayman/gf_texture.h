#ifndef OPENRAYMAN_GF_TEXTURE_H
#define OPENRAYMAN_GF_TEXTURE_H

#include <openrayman/common_stream.h>
#include <lodepng.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>

namespace openrayman
{
    struct gf_texture_info
    {
        gf_texture_info(std::size_t w, std::size_t h, std::uint8_t channels) :
            w(w), h(h), channels(channels), repeat_byte(0x1)
        {
        }

        gf_texture_info(std::size_t w, std::size_t h, std::uint8_t channels, std::uint8_t repeat_byte) :
            w(w), h(h), channels(channels), repeat_byte(repeat_byte)
        {
        }

        // The size of the GF texture.
        std::uint32_t w, h;

        // The number of channels that are present in the GF texture.
        // This is either 3 (BGR24) or 4 (BGRA32).
        std::uint8_t channels;

        // The byte used to indicate that a stream of pixels is coming in the GF texture.
        std::uint8_t repeat_byte;
    };

    // GF (graphics texture) files are used as textures.
    class gf_texture
    {
public:
        gf_texture(std::istream& stream);

        // Returns true if the stream data could be read as a valid GF texture.
        inline bool valid() const
        {
            return m_valid;
        }

        // Returns the required size for converting this texture to RGBA32.
        // This should be regarded as the maximum possible size that is written to a stream from convert_to_rgba.
        // This can be calculated manually via the formula width * height * 4.
        inline std::size_t required_size_rgba() const
        {
            return m_info.w * m_info.h * 4;
        }

        // Returns a reference to information about this GF texture.
        inline const gf_texture_info& info() const
        {
            return m_info;
        }

        // Converts this GF texture into a raw array of bytes, consisting of a RGBA32 image.
        // Returns the number of bytes written or 0 if the operation failed.
        std::size_t convert_to_rgba(std::ostream& stream);
private:
        void read_channel(std::istream& data, std::uint8_t repeat_byte, std::vector<char>& channel);

        std::istream& m_stream;
        bool m_valid;
        gf_texture_info m_info;
    };
}

#endif
