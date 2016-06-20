#include <cstdlib>
#include <iostream>
#include <info.h>
#include <engine.h>
#include <data_extractor/dsb/dsb_decompiler.h>

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
	std::string selected_install_folder = "";
    for(int n = 0; n < argc; n++)
    {
        std::string str(argv[n]);
        if(str == "--game")
        {
            n++;
            if(n >= argc)
            {
				make_sure_console_open(); std::cout << "No game was specified." << std::endl;
                return EXIT_FAILURE;
            }
            std::string game(argv[n]);
            selected_game = game;
        }
        if(str == "--convert-data")
        {
			n++;
            if(n >= argc)
            {
				make_sure_console_open(); std::cout << "No install folder was specified." << std::endl;
                return EXIT_FAILURE;
            }
            std::string install_folder(argv[n]);
            selected_install_folder = install_folder;
        }
		if(str == "--decompile")
		{
			n++;
			if(n >= argc)
			{
				make_sure_console_open(); std::cout << "No type was specified." << std::endl;
				return EXIT_FAILURE;
			}
			std::string type(argv[n]);
			n++;
			if(n >= argc)
			{
				make_sure_console_open(); std::cout << "No path was specified." << std::endl;
				return EXIT_FAILURE;
			}
			std::string path(argv[n]);
			n++;
			if(n >= argc)
			{
				make_sure_console_open(); std::cout << "No target was specified." << std::endl;
				return EXIT_FAILURE;
			}
			std::string target(argv[n]);
			if(type == "odsb" || type == "rdsb")
			{
				openrayman::dsb_decompiler decompiler;
				if(decompiler.decompile_dsb(
					path,
					target,
					type == "odsb" ?
						openrayman::dsb_format::openrayman :
						openrayman::dsb_format::rayman2_decoded))
				{
					return EXIT_SUCCESS;
				}
				else
				{
					std::cout << "Operation failed" << std::endl;
					return EXIT_FAILURE;
				}
			}
			else
			{
				make_sure_console_open(); std::cout << "Invalid type specified." << std::endl;
				return EXIT_FAILURE;
			}
		}
        // Follow GNU format
        if(str == "--help")
        {
			make_sure_console_open();
            std::cout << "Usage: openrayman [options] ..." << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --version                                    Display version information" << std::endl;
            std::cout << "  --game \"game name\"                           Specifies what game OpenRayman should load" << std::endl;
            std::cout << "                                               This will default to the game set in the config file if no game is specified" << std::endl;
            std::cout << "  --convert-data \"install folder\"              Converts the install folder specified into a format that OpenRayman can read" << std::endl;
            std::cout << "                                               This is needed to ease modding support" << std::endl;
			std::cout << "                                               This can also be done by starting the game without extracted data" << std::endl;
			std::cout << "                                               In that case, you will get a directory picker" << std::endl;
			std::cout << "  --decompile \"type\" \"path\" \"target\"           Decompiles the specified file into target." << std::endl;
			std::cout << "                                               Type can be any of:" << std::endl;
			std::cout << "                                                   \"odsb\": Creates an OpenRayman dsb file" << std::endl;
			std::cout << "                                                   \"rdsb\": Decodes a dsb file" << std::endl;
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
    return engine.run(selected_game, selected_install_folder);
}

#ifdef _WIN32

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    // fuck windows
	return main(__argc, __argv);
}

#endif
