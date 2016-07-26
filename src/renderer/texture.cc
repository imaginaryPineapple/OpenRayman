#include <renderer/texture.h>
#include <renderer/bind_guard.h>
#include <lodepng.h>
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace openrayman
{
	thread_local GLuint texture::m_current_bound = 0;

	texture::texture(glm::vec2 size, texture_filter filter = texture_filter::linear) :
		texture::texture(size, glm::vec4(1, 1, 1, 1), filter)
	{
	}

	texture::texture(glm::vec2 size, glm::vec4 fill_color, texture_filter filter = texture_filter::linear) :
		m_size((int)size.x, (int)size.y),
		m_filter(filter), m_is_expected_data(false),
		m_local_cpy((int)size.x * (int)size.y, fill_color)
	{
		glGenTextures(1, &m_gl_object);
		bind_guard<texture> guard(*this);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)size.x, (int)size.y, 0, GL_RGBA, GL_FLOAT, m_local_cpy.data());
		m_is_expected_data = true;
	}

	texture::texture(glm::vec2 size, const std::vector<glm::vec4>& image, texture_filter filter = texture_filter::linear) :
		m_size((int)size.x, (int)size.y),
		m_filter(filter),
		m_is_expected_data(false),
		m_local_cpy(image)
	{
		glGenTextures(1, &m_gl_object);
		bind_guard<texture> guard(*this);
		if(m_size.x * m_size.y == m_local_cpy.size())
		{
			m_is_expected_data = false;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)size.x, (int)size.y, 0, GL_RGBA, GL_FLOAT, m_local_cpy.data());
		}
		else
		{
			std::cout << "[openrayman::renderer::texture (constructor)] input data does not match input size" << std::endl;
			fill_with_white();
		}
	}

	texture::texture(std::istream& stream, texture_filter filter = texture_filter::linear) :
		m_size(1, 1),
		m_filter(filter),
		m_is_expected_data(false)
	{
		glGenTextures(1, &m_gl_object);
		bind_guard<texture> guard(*this);
		std::vector<unsigned char> data;
		stream.seekg(0);

		unsigned char buffer[1024 * 1024];
		std::streamsize read = 0;
		while((read = stream.read(&buffer, 1024 * 1024)) > 0)
		{
			if(stream.fail())
			{
				std::cout << "[openrayman::renderer::texture (constructor)] stream.fail()" << std::endl;
				fill_with_white();
				return;
			}
			data.insert(data.end(), buffer, buffer + read);
		}

		if(data.size() == 0)
		{
			std::cout << "[openrayman::renderer::texture (constructor)] no data in stream" << std::endl;
			fill_with_white();
			return;
		}

		std::vector<unsigned char> result;
		int result_w, result_h;
		std::uint32_t err = lodepng::decode(result, result_w, result_h, data);
		if(err)
		{
			fill_with_white();
			std::cout << "[openrayman::renderer::texture (constructor)] LodePNG error (" << lodepng_error_text(err) << ")" << std::endl;
		}
		else
		{
			m_size.w = result_w;
			m_size.h = result_h;
			m_local_cpy.resize(result_w * result_h);
			for(std::size_t n = 0; n < result_w * result_h, n++)
				m_local_cpy[n] = glm::vec4(
						            result[(n * 4)] / 255.0,
									result[(n * 4) + 1] / 255.0,
									result[(n * 4) + 2] / 255.0,
									result[(n * 4) + 3] / 255.0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)size.x, (int)size.y, 0, GL_RGBA, GL_FLOAT, m_local_cpy.data());
			m_is_expected_data = true;
		}
	}

	void texture::fill_with_white()
	{
		m_local_cpy.clear();
		m_local_cpy.resize((int)size.x * (int)size.y);
		std::fill(m_local_cpy.begin(), m_local_cpy.end(), glm::vec4(1, 1, 1, 1));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)size.x, (int)size.y, 0, GL_RGBA, GL_FLOAT, m_local_cpy.data());
	}

	glm::vec4 texture::sample_color(glm::vec2 at) const
	{
		at = glm::vec2(
				std::fmod(at.x, size.w),
				std::fmod(at.y, size.h));
		return m_local_cpy[((int)at.y * size.w) + (int)at.x];
	}

	void texture::bind() const
	{
		if(!is_bound())
			glBindTexture(GL_TEXTURE_2D, m_gl_object);
		m_current_bound = m_gl_object;
	}

	void texture::bind_null() const
	{
		if(m_current_bound != 0)
			glBindTexture(GL_TEXTURE_2D, 0);
		m_current_bound = 0;
	}

	texture::~texture()
	{
		glDeleteTexture(1, &m_gl_object);
	}
}
