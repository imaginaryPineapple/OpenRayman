#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl3w.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <istream>

namespace openrayman
{
	enum class texture_filter
	{
		nearest = GL_NEAREST,
		linear = GL_LINEAR
	};

	// RGBA32 texture on the GPU.
	class texture
	{
		// Creates a new texture filled with white pixels (1, 1, 1, 1).
		texture(glm::vec2 size, texture_filter filter = texture_filter::linear);

		// Creates a new texture filled with the color specified by "fill_color".
		texture(glm::vec2 size, glm::vec4 fill_color, texture_filter filter = texture_filter::linear);

		// Creates a new texture filled with the data in the vector "image".
		texture(glm::vec2 size, const std::vector<glm::vec4>& image, texture_filter filter = texture_filter::linear);

		// Creates a new texture from a stream.
		// Current supported file formats are PNG.
		texture(std::istream& stream, texture_filter filter = texture_filter::linear);

		~texture();

		// Returns true if this texture contains the expected data.
		// This can return false if a file failed to load,
		// size x/y == 0, or creation from a vector did not match the size parameter.
		// In this case the texture will contain all white pixels (1, 1, 1, 1).
		inline bool is_expected_data() const
		{
			return m_is_expected_data;
		}

		// Returns the size of this texture.
		inline glm::vec2 size() const
		{
			return m_size;
		}

		// Returns the filtering of this texture.
		inline texture_filter filter() const
		{
			return m_filter;
		}

		// Returns the color of the pixel at the sample position.
		glm::vec4 sample_color(glm::vec2 at) const;

		// Returns an array of all colors in this texture.
		inline const std::vector<glm::vec4>& sample_all() const
		{
			return m_local_cpy;
		}

		// Binds this texture as the currently active texture.
		void bind() const;

		// Returns true if this texture is the currently bound one.
		inline bool is_bound() const
		{
			return m_current_bound == m_gl_object;
		}

		// Binds a null texture as the currently active texture.
		static void bind_null();
	private:
		GLuint m_gl_object;
		thread_local static GLuint m_current_bound;

		void fill_with_white();

		bool m_is_expected_data;
		glm::vec2 m_size;
		texture_filter m_filter;
		std::vector<glm::vec4> m_local_cpy;
	};
}

#endif
