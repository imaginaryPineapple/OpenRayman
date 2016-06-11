# OpenRayman


An open source reimplementation of the Rayman 2: The Great Escape engine using C++11 and modern OpenGL.


## Usage


`openrayman [options] ...`  


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


Open `msvc\OpenRayman\OpenRayman.sln` and build the desired target.


## Building on Mac OS X


Mac OS X is not supported at the moment (might work with CMake?).


## Thanks to


* Ubisoft and Michael Ancel
* The glfw library
* http://raymanpc.com/
