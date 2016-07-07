file(GLOB_RECURSE SOURCES *.cc)

include_directories(AFTER "${CMAKE_SOURCE_DIR}/src")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/libopenrayman/include")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/lib/lodepng")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/lib/gl3w/include")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/lib/json/src")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/lib/glm")

add_executable(openrayman WIN32 ${SOURCES} "${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng.cpp" "${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng_util.cpp")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
target_link_libraries(openrayman libopenrayman_static)
target_link_libraries(openrayman gl3w)
target_link_libraries(openrayman nlohmann_json)

if(WIN32)
  target_link_libraries(openrayman shlwapi)
  target_link_libraries(openrayman opengl32)
  target_link_libraries(openrayman "${CMAKE_SOURCE_DIR}/win32/SDL2/libSDL2.dll.a")

  file(COPY "${CMAKE_SOURCE_DIR}/win32/SDL2/SDL2.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin")
  include_directories(AFTER "${CMAKE_SOURCE_DIR}/win32/SDL2")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
  set_target_properties(openrayman PROPERTIES LINK_FLAGS "\"${CMAKE_SOURCE_DIR}/win32/app.res\"")
endif()

if(UNIX)
  target_link_libraries(openrayman SDL2)
  find_package(PkgConfig REQUIRED)

  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  include_directories(${GTK3_INCLUDE_DIRS})
  link_directories(${GTK3_LIBRARY_DIRS})
  add_definitions(${GTK3_CFLAGS_OTHER})

  target_link_libraries(openrayman ${GTK3_LIBRARIES})
endif()

if(USE_LIBRETRO)
  add_definitions(-D_LIBRETRO_CORE)
endif()

file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_SOURCE_DIR}/bin")
