file(GLOB_RECURSE SOURCES *.cc)

include_directories(AFTER "${CMAKE_SOURCE_DIR}/src")

add_executable(openrayman ${SOURCES})
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
target_link_libraries(openrayman glfw)

if(UNIX)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(openrayman ${GTK3_LIBRARIES})
target_link_libraries(openrayman epoxy)
endif()

file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_SOURCE_DIR}/bin")
