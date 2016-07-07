#include <openrayman/gf_texture.h>
#include <iostream>
#include <iomanip>

namespace openrayman
{
    gf_texture::gf_texture(std::istream& stream) :
        m_stream(stream), m_valid(false), m_info(0, 0, 0)
    {
        m_stream.seekg(0);
        if(m_stream.fail())
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::gf_texture (constructor)] m_stream.fail()" << std::endl;
#endif
            return;
        }

        std::int32_t signature;
        m_stream.read((char*)&signature, sizeof(std::int32_t));
        if(signature != 0x00000378 && signature != 0x000022b8)
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::gf_texture (constructor)] Invalid signature (not 0x00000378 or 0x000022b8), "
				      << std::hex << std::setw(8) << "0x" << signature << std::endl;
#endif
            return;
		}

        std::int32_t rw, rh;
        m_stream.read((char*)&rw, sizeof(std::int32_t));
        m_stream.read((char*)&rh, sizeof(std::int32_t));
        if(rw < 1 || rh < 1)
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::gf_texture (constructor)] rw < 1 || rh < 1" << std::endl;
#endif
            return;
        }
        m_info.w = rw;
        m_info.h = rh;

        std::uint8_t rc, rb;
        m_stream.read((char*)&rc, 1);
        m_stream.read((char*)&rb, 1);
        if(rc != 3 && rc != 4)
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::gf_texture (constructor)] Invalid number of channels (" << rc << ", expected 3 or 4)" << std::endl;
#endif
            return;
        }
        m_info.channels = rc;
        m_info.repeat_byte = rb;

        m_valid = true;
    }

    std::size_t gf_texture::convert_to_rgba(std::ostream& stream)
    {
        // pixel data starts at position 14
        m_stream.seekg(14);
        if(m_stream.fail())
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::gf_texture::convert_to_rgba] m_stream.fail() (after m_stream.seekg(14))" << std::endl;
#endif
            return 0;
        }

        std::vector<char> r_ch(m_info.w * m_info.h, 0), g_ch(m_info.w * m_info.h, 0), b_ch(m_info.w * m_info.h, 0), a_ch(m_info.w * m_info.h, 255);
        read_channel(m_stream, m_info.repeat_byte, b_ch);
        read_channel(m_stream, m_info.repeat_byte, g_ch);
        read_channel(m_stream, m_info.repeat_byte, r_ch);
        if(m_info.channels == 4)
            read_channel(m_stream, m_info.repeat_byte, a_ch);

        std::size_t written = 0;
        std::size_t x, y;
        for(y = 0; y < m_info.h; y++)
        {
            for(x = 0; x < m_info.w; x++)
            {
                stream << r_ch[((m_info.h - 1 - y) * m_info.w) + x];
                stream << g_ch[((m_info.h - 1 - y) * m_info.w) + x];
                stream << b_ch[((m_info.h - 1 - y) * m_info.w) + x];
                stream << a_ch[((m_info.h - 1 - y) * m_info.w) + x];
                if(stream.fail())
                {
#ifdef LIBOPENRAYMAN_COUT
					std::cout << "[openrayman::gf_texture::gf_texture::convert_to_rgba] stream.fail() (wrote " << written << ")" << std::endl;
#endif
                    return written;
                }
                written += 4;
            }
        }
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::convert_to_rgba] written == " << written << std::endl;
#endif
        return written;
    }

    void gf_texture::read_channel(std::istream& stream, std::uint8_t repeat_byte, std::vector<char>& channel)
    {
        std::size_t at = 0;
        while(at < channel.size())
        {
            if(stream.fail())
            {
#ifdef LIBOPENRAYMAN_COUT
					std::cout << "[openrayman::gf_texture::gf_texture::read_channel] stream.fail() (at " << at << ")" << std::endl;
#endif
                return;
            }
            std::uint8_t value;
            stream.read((char*)&value, 1);
            if(value == repeat_byte)
            {
                stream.read((char*)&value, 1);
                std::uint8_t count;
                stream.read((char*)&count, 1);
                for(std::size_t n = 0; n < count; n++)
                {
                    // either we are wrong or some files are formatted weird?
                    // -this seems to turn out ok-
                    // TODO: this does not seem to turn out ok, there are red stripes on some textures
                    if(at + n < channel.size())
                        channel[at + n] = value;
                }
                at += count;
            }
            else
            {
                channel[at] = value;
                at++;
            }
        }
    }

    std::size_t gf_texture::convert_to_png(std::ostream& stream)
    {
        common::in_mem_ostream rgba_stream(required_size_rgba());
        if(convert_to_rgba(rgba_stream) == 0)
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::convert_to_png] convert_to_rgba(rgba_stream) failed (== 0)" << std::endl;
#endif
            return 0;
        }
        std::vector<unsigned char> png;
        std::vector<unsigned char> rgba_vector(rgba_stream.mem(), rgba_stream.mem_end());
        std::uint32_t err = lodepng::encode(png, rgba_vector, m_info.w, m_info.h);
        if(err)
        {
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::gf_texture::gf_texture::convert_to_png] LodePNG error (" << lodepng_error_text(err) << ")" << std::endl;
#endif
            return 0;
        }
        std::size_t written = 0;
        while(written < png.size())
        {
            if(stream.fail())
            {
#ifdef LIBOPENRAYMAN_COUT
				std::cout << "[openrayman::gf_texture::gf_texture::convert_to_png] stream.fail() (wrote " << written << ")" << std::endl;
#endif
                return written;
            }
            stream << png[written];
            written++;
        }
        return written;
    }
}
