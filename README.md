# OpenRayman


An open source reimplementation of the Rayman 2: The Great Escape engine using C++11 and modern OpenGL.


## Usage


`openrayman [options] ...`  


OpenRayman needs the base game `rayman2` to run.


`rayman2` can be created by starting OpenRayman without it or by using the command line option `--convert-data "folder"`. The `folder` variable should point to a valid Rayman 2: The Great Escape installation.


`rayman2` will be copied into the OpenRayman storage folder (in windows `%appdata%\openrayman` or linux `~/.local/share/openrayman`). This is all that needs to be done to run OpenRayman.


Do note that OpenRayman contains some extensions to the Rayman 2: The Great Escape game, such as more menu options.


If for some reason you wish to run without these modifications then you will either need to edit the `config.json` file and change the `game` variable from `rayman2_openrayman` to `rayman2`, or by specifying the game to run via the command line option `--game rayman2`.


You should always target `rayman2_openrayman` when making a mod.


## Obtaining "Rayman 2: The Great Escape"


As previously mentioned, a valid installation of Rayman 2: The Great Escape (PC version) must be used for data extraction.


Rayman 2: The Great Escape can be bought on GOG (https://www.gog.com/game/rayman_2_the_great_escape) or installed from a valid CD.


Installing the game on Linux needs to be done via `wine` but should work without further tweaking.


(TODO: installing on Android)


The only tested version is the GOG version.


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


## Licensing


All files in this repository are licensed under GPLv3, except for files that are a part of libopenrayman.


## libopenrayman


libopenrayman is a library that contains several classes for decoding and decompiling file formats used by the Rayman 2: The Great Escape engine.


libopenrayman is at large based upon szymski's Rayman2Lib (https://github.com/szymski/Rayman2Lib).


All files related to libopenrayman are located in the libopenrayman/ subdirectory.


## Special thanks to


https://github.com/szymski/Rayman2Lib


All others who have taken part of the UnLockRay project (https://www.reddit.com/r/UnLockRay/) and helped reverse engineer the Rayman 2: The Great Escape engine.


Palorifgrodbierzrt for creating Ray2Get, which the `apm_audio_track` implementation is partially based upon (I figured out the `WAVEFORMATEX` part though ;)).