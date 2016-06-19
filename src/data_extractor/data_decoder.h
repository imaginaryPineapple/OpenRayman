#ifndef DATA_DECODER_H
#define DATA_DECODER_H

#include <cstdint>

namespace openrayman
{
    // Decodes various file formats used by the Rayman 2: The Great Escape engine.
    class data_decoder
    {
public:
        data_decoder() :
            m_virtual_position(0),
            m_magic(1790299257)
        {
        }

        // Sets the virtual position.
        // This or advance_virtual_position needs to be used when a file is seeked.
        void set_virtual_position(std::uint64_t position);

        // Advances the virtual position by n steps.
        // This or set_virtual_position needs to be used when a file is seeked.
        // Negative values are allowed for n.
        void advance_virtual_position(std::int64_t n);

        // Decodes the specified character and advances virtual position by one.
        char decode_char(char ch);

        // Decodes the specified array and advances virtual position by length.
        void decode_array(char* array, std::size_t length);
private:
        std::uint64_t m_virtual_position;
        std::uint32_t m_magic;
    };
}

#endif
