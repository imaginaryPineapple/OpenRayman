#ifndef MATH_H
#define MATH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace openrayman
{
	// Math helpers.
	namespace math
	{
		// Interpolates between values "from" - "to".
		inline float interp(float from, float to, float value)
		{
			return from + (to - from) * value;
		}

		// Interpolates between values "from" - "to".
		inline glm::vec2 interp(glm::vec2 from, glm::vec2 to, float value)
		{
			return glm::vec2(
					interp(from.x, to.x, value),
					interp(from.y, to.y, value));
		}

		// Interpolates between values "from" - "to".
		inline glm::vec3 interp(glm::vec3 from, glm::vec3 to, float value)
		{
			return glm::vec3(
					interp(from.x, to.x, value),
					interp(from.y, to.y, value),
					interp(from.z, to.z, value));
		}

		// Interpolates between values "from" - "to".
		inline glm::vec4 interp(glm::vec4 from, glm::vec4 to, float value)
		{
			return glm::vec4(
					interp(from.r, to.r, value),
					interp(from.g, to.g, value),
					interp(from.b, to.b, value),
					interp(from.a, to.a, value));
		}
	}
}

#endif