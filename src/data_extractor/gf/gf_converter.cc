#include <data_extractor/gf/gf_converter.h>
#include <lodepng.h>

namespace openrayman
{
    struct memorybuf : std::streambuf
    {
        memorybuf(char* begin, char* end)
        {
            setg(begin, begin, end);
        }
    };

    bool gf_converter::get_gf_info(std::vector<std::uint8_t>& in, std::size_t* w, std::size_t* h, std::uint8_t* num_channels, std::uint8_t* repeat_byte)
    {
        memorybuf streambuf((char*)in.data(), (char*)in.data() + in.size());
        std::istream in_stream(&streambuf);
        if(in.size() <= 0)
            return false;
        std::int32_t signature;
        in_stream.read((char*)&signature, sizeof(std::int32_t));
        if(signature != 0x378 && signature != 0x22b8)
            return false;

        std::int32_t width, height;
        in_stream.read((char*)&width, sizeof(std::int32_t));
        in_stream.read((char*)&height, sizeof(std::int32_t));
        *w = width;
        *h = height;

        in_stream.read((char*)num_channels, 1);
        in_stream.read((char*)repeat_byte, 1);
        return true;
    }

    std::vector<std::uint8_t> gf_converter::convert_to_rgba(std::vector<std::uint8_t>& in, std::size_t* w, std::size_t* h)
    {
    #define FAIL \
        { \
            w = h = 0; \
            return std::vector<std::uint8_t>(0, 0); \
        }

        memorybuf streambuf((char*)in.data(), (char*)in.data() + in.size());
        std::istream in_stream(&streambuf);
        if(in.size() <= 0)
            FAIL;
        std::int32_t signature;
        in_stream.read((char*)&signature, sizeof(std::int32_t));
        if(signature != 0x378 && signature != 0x22b8)
            FAIL;

        std::int32_t width, height;
        in_stream.read((char*)&width, sizeof(std::int32_t));
        in_stream.read((char*)&height, sizeof(std::int32_t));
        *w = width;
        *h = height;

        std::vector<std::uint8_t> data(width * height * 4, 255);

        std::uint8_t channels, repeat_byte;
        in_stream.read((char*)&channels, 1);
        in_stream.read((char*)&repeat_byte, 1);
        if(channels != 3 && channels != 4)
            FAIL;

        std::cout << "size: " << width << "x" << height << " channels: " << std::to_string(channels) << " ";
        std::vector<std::uint8_t> r_ch(width * height, 0), g_ch(width * height, 0), b_ch(width * height, 0), a_ch(width * height, 255);
        read_channel(in_stream, repeat_byte, b_ch);
        read_channel(in_stream, repeat_byte, g_ch);
        read_channel(in_stream, repeat_byte, r_ch);
        if(channels == 4)
            read_channel(in_stream, repeat_byte, a_ch);

        std::size_t x, y;
        for(y = 0; y < height; y++)
        {
            for(x = 0; x < width; x++)
            {
                data[(((y * width) + x) * 4)] = r_ch[((height - 1 - y) * width) + x];
                data[(((y * width) + x) * 4) + 1] = g_ch[((height - 1 - y) * width) + x];
                data[(((y * width) + x) * 4) + 2] = b_ch[((height - 1 - y) * width) + x];
                data[(((y * width) + x) * 4) + 3] = a_ch[((height - 1 - y) * width) + x];
            }
        }

        return data;
    }

    void gf_converter::read_channel(std::istream& data, std::uint8_t repeat_byte, std::vector<std::uint8_t>& channel)
    {
        std::size_t at = 0;
        while(at < channel.size())
        {
            std::uint8_t value;
            data.read((char*)&value, 1);
            if(value == repeat_byte)
            {
                data.read((char*)&value, 1);
                std::uint8_t count;
                data.read((char*)&count, 1);
                for(std::size_t n = 0; n < count; n++)
                {
                    // either we are wrong or some files are formatted weird?
                    // this seems to turn out ok
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

    std::vector<std::uint8_t> gf_converter::convert_to_png(std::vector<std::uint8_t>& in)
    {
        std::size_t w, h;
        std::vector<std::uint8_t> data = convert_to_rgba(in, &w, &h);
        if(data.size() == 0)
            return std::vector<std::uint8_t>(0, 0);
        std::vector<std::uint8_t> png;
        std::uint32_t error = lodepng::encode(png, data, w, h);
        if(error)
            return std::vector<std::uint8_t>(0, 0);
        return png;
    }
}
