# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/hannesmann/UbuntuHDD/Dev/OpenRayman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build

# Include any dependencies generated for this target.
include src/CMakeFiles/OpenRayman.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/OpenRayman.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/OpenRayman.dir/flags.make

src/CMakeFiles/OpenRayman.dir/main.cc.o: src/CMakeFiles/OpenRayman.dir/flags.make
src/CMakeFiles/OpenRayman.dir/main.cc.o: ../src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/OpenRayman.dir/main.cc.o"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenRayman.dir/main.cc.o -c /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/main.cc

src/CMakeFiles/OpenRayman.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenRayman.dir/main.cc.i"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/main.cc > CMakeFiles/OpenRayman.dir/main.cc.i

src/CMakeFiles/OpenRayman.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenRayman.dir/main.cc.s"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/main.cc -o CMakeFiles/OpenRayman.dir/main.cc.s

src/CMakeFiles/OpenRayman.dir/main.cc.o.requires:

.PHONY : src/CMakeFiles/OpenRayman.dir/main.cc.o.requires

src/CMakeFiles/OpenRayman.dir/main.cc.o.provides: src/CMakeFiles/OpenRayman.dir/main.cc.o.requires
	$(MAKE) -f src/CMakeFiles/OpenRayman.dir/build.make src/CMakeFiles/OpenRayman.dir/main.cc.o.provides.build
.PHONY : src/CMakeFiles/OpenRayman.dir/main.cc.o.provides

src/CMakeFiles/OpenRayman.dir/main.cc.o.provides.build: src/CMakeFiles/OpenRayman.dir/main.cc.o


src/CMakeFiles/OpenRayman.dir/engine.cc.o: src/CMakeFiles/OpenRayman.dir/flags.make
src/CMakeFiles/OpenRayman.dir/engine.cc.o: ../src/engine.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/OpenRayman.dir/engine.cc.o"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenRayman.dir/engine.cc.o -c /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/engine.cc

src/CMakeFiles/OpenRayman.dir/engine.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenRayman.dir/engine.cc.i"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/engine.cc > CMakeFiles/OpenRayman.dir/engine.cc.i

src/CMakeFiles/OpenRayman.dir/engine.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenRayman.dir/engine.cc.s"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/engine.cc -o CMakeFiles/OpenRayman.dir/engine.cc.s

src/CMakeFiles/OpenRayman.dir/engine.cc.o.requires:

.PHONY : src/CMakeFiles/OpenRayman.dir/engine.cc.o.requires

src/CMakeFiles/OpenRayman.dir/engine.cc.o.provides: src/CMakeFiles/OpenRayman.dir/engine.cc.o.requires
	$(MAKE) -f src/CMakeFiles/OpenRayman.dir/build.make src/CMakeFiles/OpenRayman.dir/engine.cc.o.provides.build
.PHONY : src/CMakeFiles/OpenRayman.dir/engine.cc.o.provides

src/CMakeFiles/OpenRayman.dir/engine.cc.o.provides.build: src/CMakeFiles/OpenRayman.dir/engine.cc.o


src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o: src/CMakeFiles/OpenRayman.dir/flags.make
src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o: ../src/platform/message_box.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenRayman.dir/platform/message_box.cc.o -c /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/platform/message_box.cc

src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenRayman.dir/platform/message_box.cc.i"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/platform/message_box.cc > CMakeFiles/OpenRayman.dir/platform/message_box.cc.i

src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenRayman.dir/platform/message_box.cc.s"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src/platform/message_box.cc -o CMakeFiles/OpenRayman.dir/platform/message_box.cc.s

src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.requires:

.PHONY : src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.requires

src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.provides: src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.requires
	$(MAKE) -f src/CMakeFiles/OpenRayman.dir/build.make src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.provides.build
.PHONY : src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.provides

src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.provides.build: src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o


# Object files for target OpenRayman
OpenRayman_OBJECTS = \
"CMakeFiles/OpenRayman.dir/main.cc.o" \
"CMakeFiles/OpenRayman.dir/engine.cc.o" \
"CMakeFiles/OpenRayman.dir/platform/message_box.cc.o"

# External object files for target OpenRayman
OpenRayman_EXTERNAL_OBJECTS =

../bin/OpenRayman: src/CMakeFiles/OpenRayman.dir/main.cc.o
../bin/OpenRayman: src/CMakeFiles/OpenRayman.dir/engine.cc.o
../bin/OpenRayman: src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o
../bin/OpenRayman: src/CMakeFiles/OpenRayman.dir/build.make
../bin/OpenRayman: lib/glfw/src/libglfw3.a
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/librt.so
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/libm.so
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/libX11.so
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/libXrandr.so
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/libXinerama.so
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
../bin/OpenRayman: /usr/lib/x86_64-linux-gnu/libXcursor.so
../bin/OpenRayman: src/CMakeFiles/OpenRayman.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../bin/OpenRayman"
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OpenRayman.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/OpenRayman.dir/build: ../bin/OpenRayman

.PHONY : src/CMakeFiles/OpenRayman.dir/build

src/CMakeFiles/OpenRayman.dir/requires: src/CMakeFiles/OpenRayman.dir/main.cc.o.requires
src/CMakeFiles/OpenRayman.dir/requires: src/CMakeFiles/OpenRayman.dir/engine.cc.o.requires
src/CMakeFiles/OpenRayman.dir/requires: src/CMakeFiles/OpenRayman.dir/platform/message_box.cc.o.requires

.PHONY : src/CMakeFiles/OpenRayman.dir/requires

src/CMakeFiles/OpenRayman.dir/clean:
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src && $(CMAKE_COMMAND) -P CMakeFiles/OpenRayman.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/OpenRayman.dir/clean

src/CMakeFiles/OpenRayman.dir/depend:
	cd /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/hannesmann/UbuntuHDD/Dev/OpenRayman /media/hannesmann/UbuntuHDD/Dev/OpenRayman/src /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src /media/hannesmann/UbuntuHDD/Dev/OpenRayman/build/src/CMakeFiles/OpenRayman.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/OpenRayman.dir/depend
