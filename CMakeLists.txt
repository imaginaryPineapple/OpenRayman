project(OpenRayman)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Release CACHE STRING "The type of build")
set(USE_LIBRETRO OFF CACHE BOOL "If OpenRayman should be built as a libretro core (TODO)")
set(CMAKE_CXX_STANDARD 11)

add_definitions(-std=gnu++11)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# taken from glfw
if(MSVC)
	foreach(flag CMAKE_C_FLAGS
				CMAKE_C_FLAGS_DEBUG
				CMAKE_C_FLAGS_RELEASE
				CMAKE_C_FLAGS_MINSIZEREL
				CMAKE_C_FLAGS_RELWITHDEBINFO)
		if(${flag} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
		endif()
		if(${flag} MATCHES "/MDd")
			string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
		endif()
	endforeach()
endif()

set(BuildTests OFF)
add_subdirectory(lib/gl3w)
add_subdirectory(lib/json)
add_subdirectory(lib/glm)
add_subdirectory(src)
