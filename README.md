# OpenRayman


An open source reimplementation of the Rayman 2: The Great Escape engine using C++11 and modern OpenGL.


## Usage


`openrayman [options] ...`  


OpenRayman needs the base game `rayman2` to run.


`rayman2` can be created by using the command line option `--extract-data "folder"`. The `folder` variable should point to a valid Rayman 2: The Great Escape installation.


`rayman2` will be copied into the OpenRayman data folder (in `data/games`). This is all that needs to be done to run OpenRayman.


Do note that OpenRayman contains some extensions to the Rayman 2: The Great Escape game, such as more menu options.


If you for some reason wish to run without these modifications then you will either need to edit the `config.json` file and change the `game` variable from `rayman2_openrayman` to `rayman2`, or by specifying the game to run via the command line option `--game rayman2`.


You should always target `rayman2_openrayman` when making a mod.


## GL3W


You need to build gl3w.h and gl3w.c before you build the project.


This can be done by executing lib/gl3w/gl3w_gen.py with Python 2.


## Building on Linux


CMake should be used to generate makefiles, and `make` should be used to build the engine.


```
mkdir build
cd build
cmake ..
make -j8
```

The engine can be installed via `sudo make install` and can then be started by executing the command `openrayman`.


Additionally, a .desktop file will be created.


## Building on Windows


Building should be done with MinGW, and is almost the same as on Linux.


You need to use the command `cmake -G "MinGW Makefiles" ..` instead of `cmake ..` to create makefiles, as OpenRayman cannot be built with MSVC.


You might also need `mingw32-make -j8` instead of `make -j8`.


## Building on Mac OS X


Mac OS X is not supported at the moment (might work with CMake?).
