#ifndef BIND_GUARD_H
#define BIND_GUARD_H

#include <renderer/buf.h>
#include <renderer/texture.h>

namespace openrayman
{
	// Holds a bind on a GL resource until it goes out of scope.
	template<typename T>
	class bind_guard
	{
		static_assert(std::is_same<vertex_buf, T>::value ||
				std::is_same<element_buf, T>::value ||
				std::is_same<texture, T>::value,
				"Not a GL resource type");
public:
		bind_guard(const T& t)
		{
			t.bind();
		}

		~bind_guard()
		{
			T::bind_null();
		}
	};
}

#endif
