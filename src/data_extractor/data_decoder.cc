#include <data_extractor/data_decoder.h>

namespace openrayman
{
    void data_decoder::set_virtual_position(std::uint64_t position)
    {
        if(position > m_virtual_position)
        {
            for(std::uint64_t n = 0; n < position - m_virtual_position; n++)
                m_magic = 16807 * (m_magic ^ 0x75BD924) - 0x7FFFFFFF * ((m_magic ^ 0x75BD924u) / 0x1F31D);
        }
        else if(position < m_virtual_position)
        {
            m_magic = 1790299257;
            for(std::uint64_t n = 0; n < position; n++)
                m_magic = 16807 * (m_magic ^ 0x75BD924) - 0x7FFFFFFF * ((m_magic ^ 0x75BD924u) / 0x1F31D);
        }
        m_virtual_position = position;
    }

    void data_decoder::advance_virtual_position(std::int64_t n)
    {
        if((std::int64_t)m_virtual_position + n < 0)
            set_virtual_position(0);
        else
            set_virtual_position(m_virtual_position + n);
    }

    char data_decoder::decode_char(char ch)
    {
        char result = ch ^ (m_magic >> 8) & 255;
        advance_virtual_position(1);
        return result;
    }

    void data_decoder::decode_array(char* array, std::size_t length)
    {
        for(std::size_t n = 0; n < length; n++)
            array[n] = decode_char(array[n]);
    }
}
