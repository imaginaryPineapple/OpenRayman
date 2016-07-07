#ifndef OPENRAYMAN_COMMON_H
#define OPENRAYMAN_COMMON_H

#include <memory>
#include <iostream>
#include <streambuf>
#include <vector>
#include <cstdlib>

namespace openrayman
{
    namespace common
    {
        // Stream buffer that reads and writes from/in a block of memory.
        class in_mem_buf : public std::streambuf
        {
public:
            in_mem_buf(char* mem, std::size_t mem_size) :
                m_mem_begin(mem),
	  	m_mem_current(m_mem_begin),
	  	m_mem_end(mem + mem_size)
            {
            }

            in_mem_buf(std::size_t mem_size) :
                m_local_mem(mem_size, 0),
	  	m_mem_begin(m_local_mem.data()),
	  	m_mem_current(m_mem_begin),
	  	m_mem_end(m_local_mem.data() + mem_size)
            {
            }

            in_mem_buf(std::vector<char>& mem) :
                in_mem_buf(mem.data(), mem.size())
            {
            }

            // Returns a pointer to the block of memory backing this buffer.
            inline char* mem()
            {
                return m_mem_begin;
            }

	    // Returns a pointer to the current value in the stream.
	    inline char* mem_current()
	    {
	    	return m_mem_current;
	    }

	    // Returns a pointer to the end of the block of memory backing this buffer.
	    inline char* mem_end()
	    {
	        return m_mem_end;
	    }

            // Returns the size of the block of memory backing this buffer.
            inline std::size_t mem_size()
            {
                return m_mem_end - m_mem_begin;
            }
private:
            int_type underflow() override;
	    int_type overflow(int_type ch) override;
	    std::streamsize xsputn(const char* seq, std::streamsize n) override;
            int_type uflow() override;
            int_type pbackfail(int_type ch) override;
            std::streamsize showmanyc() override;
            std::streampos seekoff(
                std::streamoff off,
                std::ios_base::seekdir way,
                std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;
            std::streampos seekpos(std::streampos sp, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;

            std::vector<char> m_local_mem;
            char* m_mem_begin, *m_mem_current, *m_mem_end;
        };

        // Input stream wrapping a in_mem_buf.
        class in_mem_istream : public std::istream
        {
public:
            in_mem_istream(char* mem, std::size_t mem_size) :
                std::istream(&m_buf), m_buf(mem, mem_size)
            {
            }

            in_mem_istream(std::size_t mem_size) :
                std::istream(&m_buf), m_buf(mem_size)
            {
            }

            in_mem_istream(std::vector<char>& mem) :
                in_mem_istream(mem.data(), mem.size())
            {
            }

            // Returns a pointer to the block of memory backing this stream.
            inline char* mem()
            {
                return m_buf.mem();
            }

	    // Returns a pointer to the current value in the stream.
	    inline char* mem_current()
	    {
	    	return m_buf.mem_current();
	    }

	    // Returns a pointer to the end of the block of memory backing this buffer.
	    inline char* mem_end()
	    {
	        return m_buf.mem_end();
	    }

            // Returns the size of the block of memory backing this stream.
            inline std::size_t mem_size()
            {
                return m_buf.mem_size();
            }

            std::streambuf* rdbuf(std::streambuf* rb)
            {
                // we always use an in_mem_buf
                return std::istream::rdbuf();
            }
private:
            in_mem_buf m_buf;
        };

        // Output stream wrapping a in_mem_buf.
        class in_mem_ostream : public std::ostream
        {
public:
            in_mem_ostream(char* mem, std::size_t mem_size) :
                std::ostream(&m_buf), m_buf(mem, mem_size)
            {
            }

            in_mem_ostream(std::size_t mem_size) :
                std::ostream(&m_buf), m_buf(mem_size)
            {
            }

            in_mem_ostream(std::vector<char>& mem) :
                in_mem_ostream(mem.data(), mem.size())
            {
            }

            // Returns a pointer to the block of memory backing this stream.
            inline char* mem()
            {
                return m_buf.mem();
            }

	    // Returns a pointer to the current value in the stream.
	    inline char* mem_current()
	    {
	    	return m_buf.mem_current();
	    }

	    // Returns a pointer to the end of the block of memory backing this buffer.
	    inline char* mem_end()
	    {
	        return m_buf.mem_end();
	    }

            // Returns the size of the block of memory backing this stream.
            inline std::size_t mem_size()
            {
                return m_buf.mem_size();
            }

            std::streambuf* rdbuf(std::streambuf* rb)
            {
                // we always use an in_mem_buf
                return std::ostream::rdbuf();
            }
private:
            in_mem_buf m_buf;
        };

        // Stream buffer that reads encoded data from an existing std::istream.
        class encoded_buf : public std::streambuf
        {
public:
            encoded_buf(std::istream& in);
            encoded_buf(std::istream& in, std::size_t initial_virtual_position);

            // Sets the virtual position used for decoding.
            void set_virtual_position(std::size_t position);

            // Advances the virtual position by n steps.
            void advance_virtual_position(std::size_t n, bool forward);

            // Decodes a character using the magic value at the current virtual position.
            char decode_char(char c) const;
private:
            int_type underflow() override;
            int_type uflow() override;
            int_type pbackfail(int_type ch) override;
            std::streamsize showmanyc() override;
            std::streampos seekoff(
                std::streamoff off,
                std::ios_base::seekdir way,
                std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;
            std::streampos seekpos(std::streampos sp, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;

            std::istream& m_in;
            std::size_t m_virtual_position;
            std::uint32_t m_current_magic;
        };
    }
}

#endif
