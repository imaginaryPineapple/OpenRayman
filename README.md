# OpenRayman


An open source reimplementation of the Rayman 2: The Great Escape engine using C++11 and modern OpenGL.


## Usage


`openrayman [options] ...`  


## GL3W


You need to build gl3w.h and gl3w.c before you build the project.


This can be done by executing lib/gl3w/gl3w_gen.py with Python 2.


## Building on Linux


CMake should be used to build the engine.


```
mkdir build
cd build
cmake ..
make -j8
```

The engine can be installed via `sudo make install` and can then be started by executing the command `openrayman`, and additionally, a .desktop file will be created.


## Building on Windows


Building should be done with mingw, and is almost the same as on Linux.


You need to use the command `cmake -G "MinGW Makefiles" ..` instead of `cmake ..` to create makefiles, as OpenRayman cannot be built with MSVC.


You might also need `mingw32-make -j8` instead of `make -j8`.


## Building on Mac OS X


Mac OS X is not supported at the moment (might work with CMake?).


## Thanks to


* Ubisoft and Michael Ancel
* GLFW 3
* GL3W
* lodepng
* http://raymanpc.com/
