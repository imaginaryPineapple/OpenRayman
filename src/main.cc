#include <cstdlib>
#include <iostream>
#include <info.h>
#include <engine.h>

bool console_open = false;

void make_sure_console_open()
{
#ifdef _WIN32
	if(!console_open)
	{
		if(!AttachConsole(-1))
			AllocConsole();
		freopen("CONOUT$", "w", stdout);
		console_open = true;
	}
#endif
}

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
				make_sure_console_open();
                std::cout << "No game was specified." << std::endl;
                return EXIT_FAILURE;
            }
            std::string game(argv[n]);
            selected_game = game;
        }
        if(str == "--convert-data")
        {
			make_sure_console_open();
            std::cout << "TODO" << std::endl;
            return EXIT_FAILURE;
        }
        // Follow GNU format
        if(str == "--help")
        {
			make_sure_console_open();
            std::cout << "Usage: openrayman [options] ..." << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --version                    Display version information" << std::endl;
            std::cout << "  --game \"game name\"           Specifies what game OpenRayman should load" << std::endl;
            std::cout << "                               This will default to the game set in the config file if no game is specified" << std::endl;
            std::cout << "  --convert-data \"folder\"      Converts the data folder specified into a format that OpenRayman can read" << std::endl;
            std::cout << "                               This is needed to ease modding support" << std::endl;
            return EXIT_SUCCESS;
        }
        if(str == "--version")
        {
			make_sure_console_open();
            std::cout << "OpenRayman " << openrayman::version << " Copyright (C) 2016 Hannes Mann" << std::endl;
            std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details read the source file \"LICENSE.md\"." << std::endl;
            std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
            std::cout << "under certain conditions." << std::endl;
            return EXIT_SUCCESS;
        }
    }
    std::cout << "OpenRayman " << openrayman::version << std::endl;
    std::cout << "Running on " << (openrayman::this_platform == openrayman::platform::windows ? "Windows" : "Linux") << std::endl;
    std::cout << "Using game " << (selected_game == "" ? "from config" : "\"" + selected_game + "\"") << std::endl;
    openrayman::engine engine;
    return engine.run(selected_game);
}

#ifdef _WIN32

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    // fuck windows
	return main(__argc, __argv);
}

#endif
