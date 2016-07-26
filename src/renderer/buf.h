#ifndef BUF_H
#define BUF_H

#include <GL/gl3w.h>
#include <glm/vec4.hpp>
#include <cstdint>
#include <vector>

namespace openrayman
{
    // The suggested frequency at which a buffer will be modified, to enable for driver optimizations.
    enum class buf_mod_freq
    {
        // The buffer will have data uploaded in its constructor and then preferably never again.
        never,
        // The buffer will be modified from time to time, but drawn many more times than that.
        some,
        // The buffer will be modified as many times at it is drawn (usually once every frame).
        every_frame
    };

    // Buffer on the GPU that holds vertices.
    class vertex_buf
    {
public:
        vertex_buf(std::vector<glm::vec4>& initial_data, buf_mod_freq frequency = buf_mod_freq::some);
        ~vertex_buf();

        // Data stored in RAM. This can be modified and then committed via commit().
        inline std::vector<glm::vec4>& local_data()
        {
            return m_local_data;
        }

        // Commits applied changes to the GPU.
        void commit();

		// Binds this element buffer as the currently active buffer.
		void bind() const;

		// Returns true if this buffer is the currently bound one.
		inline bool is_bound() const
		{
			return m_current_bound == m_gl_object;
		}

		// Binds a null buffer as the currently active buffer.
		static void bind_null();
private:
		GLuint m_gl_object;
		thread_local static GLuint m_current_bound;

		std::vector<glm::vec4> m_local_data;
		buf_mod_freq m_frequency;
    };

    // Buffer on the GPU that holds elements.
    class element_buf
    {
public:
        element_buf(std::vector<std::uint32_t>& initial_data, buf_mod_freq frequency = buf_mod_freq::some);
        ~element_buf();

        // Data stored in RAM. This can be modified and then committed via commit().
        inline std::vector<std::uint32_t>& local_data()
        {
            return m_local_data;
        }

        // Commits applied changes to the GPU.
        void commit();

		// Binds this element buffer as the currently active buffer.
		void bind() const;

		// Returns true if this buffer is the currently bound one.
		inline bool is_bound() const
		{
			return m_current_bound == m_gl_object;
		}

		// Binds a null buffer as the currently active buffer.
		static void bind_null();
private:
        GLuint m_gl_object;
		thread_local static GLuint m_current_bound;

		std::vector<std::uint32_t> m_local_data;
		buf_mod_freq m_frequency;
	};
}

#endif
