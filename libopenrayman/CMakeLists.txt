file(GLOB_RECURSE SOURCES src/*.cc)

include_directories(AFTER "${CMAKE_SOURCE_DIR}/libopenrayman/include")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/lib/lodepng")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/lib/json/src")

add_library(libopenrayman_shared SHARED ${SOURCES} "${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng.cpp" "${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng_util.cpp")
add_library(libopenrayman_static STATIC ${SOURCES} "${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng.cpp" "${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng_util.cpp")

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/libopenrayman/bin")
target_link_libraries(libopenrayman_static nlohmann_json)
target_link_libraries(libopenrayman_shared nlohmann_json)
set_target_properties(libopenrayman_static PROPERTIES OUTPUT_NAME "libopenrayman")
set_target_properties(libopenrayman_shared PROPERTIES OUTPUT_NAME "libopenrayman")
set_target_properties(libopenrayman_static PROPERTIES PREFIX "")
set_target_properties(libopenrayman_shared PROPERTIES PREFIX "")

if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
endif()
