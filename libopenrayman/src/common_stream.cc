#include <openrayman/common_stream.h>

namespace openrayman
{
    namespace common
    {
        in_mem_buf::int_type in_mem_buf::underflow()
        {
            if(m_mem_current == m_mem_end)
        		return traits_type::eof();
    		return traits_type::to_int_type(*m_mem_current);
        }

        in_mem_buf::int_type in_mem_buf::overflow(int_type ch)
        {
    		if(ch != traits_type::eof())
    			*m_mem_current = (char)ch;
    		return ch;
        }

        std::streamsize in_mem_buf::xsputn(const char* seq, std::streamsize n)
        {
    		for(std::streamsize written = 0; written < n; written++)
    		{
    			if(m_mem_current == m_mem_end)
    			{
    				*m_mem_current = seq[written];
    				return written;
    			}
    			*m_mem_current++ = seq[written];
    		}
    		return n;
        }

        in_mem_buf::int_type in_mem_buf::uflow()
        {
            if(m_mem_current == m_mem_end)
        		return traits_type::eof();
        	return traits_type::to_int_type(*m_mem_current++);
        }

        in_mem_buf::int_type in_mem_buf::pbackfail(int_type ch)
        {
    		if(m_mem_current == m_mem_begin || (ch != traits_type::eof() && ch != *(m_mem_current - 1)))
    			return traits_type::eof();
    		return traits_type::to_int_type(*--m_mem_current);
        }

        std::streamsize in_mem_buf::showmanyc()
        {
    		return m_mem_end - m_mem_current;
        }

        std::streampos in_mem_buf::seekoff(
            std::streamoff off,
            std::ios_base::seekdir way,
            std::ios_base::openmode which)
        {
            if(way == std::ios_base::beg)
                m_mem_current = m_mem_begin + off;
            if(way == std::ios_base::cur)
                m_mem_current = m_mem_current + off;
            if(way == std::ios_base::end)
                m_mem_current = m_mem_end - off;
            if(m_mem_current < m_mem_begin)
        		m_mem_current = m_mem_begin;
        	if(m_mem_current > m_mem_end)
        		m_mem_current = m_mem_end;
            return m_mem_current - m_mem_begin;
        }

        std::streampos in_mem_buf::seekpos(std::streampos sp, std::ios_base::openmode which)
        {
   		    m_mem_current = m_mem_begin + sp;
            if(m_mem_current < m_mem_begin)
        		m_mem_current = m_mem_begin;
        	if(m_mem_current > m_mem_end)
        		m_mem_current = m_mem_end;
            return m_mem_current - m_mem_begin;
        }

        encoded_buf::encoded_buf(std::istream& in, std::size_t initial_virtual_position) :
            m_in(in), m_virtual_position(0), m_current_magic(1790299257)
        {
            set_virtual_position(initial_virtual_position);
        }

        encoded_buf::encoded_buf(std::istream& in) :
            encoded_buf(in, in.tellg())
        {
        }

        void encoded_buf::set_virtual_position(std::size_t position)
        {
            if(position > m_virtual_position)
            {
                for(std::uint64_t n = 0; n < position - m_virtual_position; n++)
                    m_current_magic = 16807 * (m_current_magic ^ 0x75BD924) - 0x7FFFFFFF * ((m_current_magic ^ 0x75BD924u) / 0x1F31D);
            }
            else if(position < m_virtual_position)
            {
                m_current_magic = 1790299257;
                for(std::uint64_t n = 0; n < position; n++)
                    m_current_magic = 16807 * (m_current_magic ^ 0x75BD924) - 0x7FFFFFFF * ((m_current_magic ^ 0x75BD924u) / 0x1F31D);
            }
            m_virtual_position = position;
        }

        void encoded_buf::advance_virtual_position(std::size_t n, bool forward)
        {
            if(forward)
                set_virtual_position(m_virtual_position + n);
            else
                set_virtual_position(m_virtual_position - n);
        }

        char encoded_buf::decode_char(char c) const
        {
            return c ^ (m_current_magic >> 8) & 255;
        }

        encoded_buf::int_type encoded_buf::underflow()
        {
            auto value = m_in.peek();
            if(value == traits_type::eof())
                return traits_type::eof();
            return traits_type::to_int_type(decode_char((char)value));
        }

        encoded_buf::int_type encoded_buf::uflow()
        {
            auto value = m_in.get();
            if(value == traits_type::eof())
                return traits_type::eof();
            auto c = decode_char((char)value);
            advance_virtual_position(1, true);
            return traits_type::to_int_type(c);
        }

        encoded_buf::int_type encoded_buf::pbackfail(int_type ch)
        {
            std::streampos pos = m_in.tellg();
            if(pos == 0)
                return traits_type::eof();
            m_in.seekg((std::size_t)pos - 1);
            if(ch != traits_type::eof() && ch != m_in.peek())
                return traits_type::eof();
            return traits_type::to_int_type(m_in.peek());
        }

        std::streamsize encoded_buf::showmanyc()
        {
            return 0;
        }

        std::streampos encoded_buf::seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which)
        {
            m_in.seekg(off, way);
            return m_in.tellg();
        }

        std::streampos encoded_buf::seekpos(std::streampos sp, std::ios_base::openmode which)
        {
            m_in.seekg(sp);
            return m_in.tellg();
        }
    }
}
