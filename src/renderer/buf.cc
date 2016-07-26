#include <renderer/buf.h>
#include <renderer/bind_guard.h>

namespace openrayman
{
	thread_local GLuint vertex_buf::m_current_bound = 0;
	thread_local GLuint element_buf::m_current_bound = 0;

	vertex_buf::vertex_buf(std::vector<glm::vec4>& initial_data, buf_mod_freq frequency) :
        m_local_data(initial_data), m_frequency(frequency)
    {
        glGenBuffers(1, &m_gl_object);
        commit();
    }

    void vertex_buf::commit()
    {
        bind_guard<vertex_buf> guard(*this);
        GLenum usage;
        switch(m_frequency)
        {
            case buf_mod_freq::never:
                usage = GL_STATIC_DRAW;
                break;

            case buf_mod_freq::some:
                usage = GL_DYNAMIC_DRAW;
                break;

            case buf_mod_freq::every_frame:
                usage = GL_STREAM_DRAW;
                break;
        }
        glBufferData(GL_ARRAY_BUFFER, m_local_data.size() * 4, m_local_data.data(), usage);
    }

    vertex_buf::~vertex_buf()
    {
        glDeleteBuffers(1, &m_gl_object);
    }

	void vertex_buf::bind() const
	{
		if(!is_bound())
			glBindBuffer(GL_ARRAY_BUFFER, m_gl_object);
		m_current_bound = m_gl_object;
	}

	void vertex_buf::bind_null()
	{
		if(m_current_bound != 0)
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_current_bound = 0;
	}

    element_buf::element_buf(std::vector<std::uint32_t>& initial_data, buf_mod_freq frequency) :
        m_local_data(initial_data), m_frequency(frequency)
    {
        glGenBuffers(1, &m_gl_object);
        commit();
    }

    void element_buf::commit()
    {
		bind_guard<element_buf> guard(*this);
        GLenum usage;
        switch(m_frequency)
        {
            case buf_mod_freq::never:
                usage = GL_STATIC_DRAW;
                break;

            case buf_mod_freq::some:
                usage = GL_DYNAMIC_DRAW;
                break;

            case buf_mod_freq::every_frame:
                usage = GL_STREAM_DRAW;
                break;
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_local_data.size(), m_local_data.data(), usage);
    }

	void element_buf::bind() const
	{
		if(!is_bound())
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_object);
		m_current_bound = m_gl_object;
	}

	void element_buf::bind_null()
	{
		if(m_current_bound != 0)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_current_bound = 0;
	}

    element_buf::~element_buf()
    {
        glDeleteBuffers(1, &m_gl_object);
    }
}
