#include <cstdlib>
#include <iostream>
#include <info.h>
#include <engine.h>
#ifndef _WIN32
#include <gtk/gtk.h>
#endif

int main(int argc, char** argv)
{
    std::string selected_game = "";
    for(int n = 0; n < argc; n++)
    {
        std::string str(argv[n]);
        if(str == "--game")
        {
            n++;
            if(n >= argc)
            {
                std::cout << "No game was specified." << std::endl;
                return EXIT_FAILURE;
            }
            std::string game(argv[n]);
            selected_game = game;
        }
        // follow GNU format
        if(str == "--help")
        {
            std::cout << "Usage: openrayman [options] ...";
            std::cout << "Options:" << std::endl;
            std::cout << "  --version  Display version information" << std::endl;
            std::cout << "  --game     Specifies what game to run" << std::endl;
            std::cout << "             This will default to the game set in \"info.json\" if no game is specified." << std::endl;
            return EXIT_SUCCESS;
        }
        if(str == "--version")
        {
            std::cout << "OpenRayman " << openrayman::version << " Copyright (C) 2016 Hannes Mann" << std::endl;
            std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details read the source file \"LICENSE.md\"." << std::endl;
            std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
            std::cout << "under certain conditions." << std::endl;
            return EXIT_SUCCESS;
        }
    }
    std::cout << "OpenRayman " << openrayman::version << std::endl;
    std::cout << "Running on " << (openrayman::this_platform == openrayman::platform::windows ? "Windows" : "Linux") << std::endl;
    std::cout << "Using game " << (selected_game == "" ? "from \"info.json\"" : "\"" + selected_game + "\"") << std::endl;
    openrayman::engine engine;
    return engine.run(selected_game);
}

#ifdef _WIN32

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	// fuck windows
	return main(0, nullptr);
}

#endif