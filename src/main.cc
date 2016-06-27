#include <cstdlib>
#include <iostream>
#include <info.h>
#include <engine.h>
#include <data_extractor/dsb/dsb_decompiler.h>
#include <dsb_interpreter/dsb_interpreter.h>
#include <dsb_interpreter/dsb_interpreter_debugger.h>

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

int fail_and_print(const std::string& msg)
{
	make_sure_console_open(); std::cout << msg << std::endl;
	return EXIT_FAILURE;
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
                return fail_and_print("No game was specified");;
            std::string game(argv[n]);
            selected_game = game;
        }
        if(str == "--convert-data")
        {
			n++;
            if(n >= argc)
				return fail_and_print("No install folder was specified");
            std::string install_folder(argv[n]);
            selected_install_folder = install_folder;
        }
		if(str == "--convert-to")
		{
			n++;
			if(n >= argc)
				return fail_and_print("No format was specified");
			std::string format(argv[n]);
			n++;
			if(n >= argc)
				return fail_and_print("No path was specified");
			std::string path(argv[n]);
			n++;
			if(n >= argc)
				return fail_and_print("No target was specified");
			std::string target(argv[n]);
			if(format == "odsb" || format == "rdsb")
			{
				openrayman::dsb_decompiler decompiler;
				if(decompiler.decompile_dsb(
					path,
					target,
					format == "odsb" ?
						openrayman::dsb_format::openrayman :
						openrayman::dsb_format::rayman2_decoded))
				{
					return EXIT_SUCCESS;
				}
				else
					return fail_and_print("Operation failed");
			}
			else
				return fail_and_print("Invalid format specified");
		}
		if(str == "--inspect")
		{
			n++;
			if(n >= argc)
				return fail_and_print("No format was specified");
			std::string format(argv[n]);
			n++;
			if(n >= argc)
				return fail_and_print("No path was specified");
			std::string path(argv[n]);
			if(format == "odsb")
			{
				openrayman::dsb_interpreter interpreter(path);
				openrayman::dsb_interpreter_debugger debugger(interpreter);
				debugger.print_summary();
				return interpreter.success() ? EXIT_SUCCESS : EXIT_FAILURE;
			}
			else
				return fail_and_print("Invalid format was specified");
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
			std::cout << "  --convert-to \"format\" \"path\" \"target\"         Converts the specified file into the target format" << std::endl;
			std::cout << "                                               Format can be any of:" << std::endl;
			std::cout << "                                                   \"odsb\": Creates an OpenRayman DSB file" << std::endl;
			std::cout << "                                                   \"rdsb\": Decodes a DSB file" << std::endl;
			std::cout << "  --inspect \"format\" \"path\"                    Inspects and prints info about the specified file" << std::endl;
			std::cout << "                                               Format can be any of:" << std::endl;
			std::cout << "                                                   \"odsb\": Interprets the DSB and outputs all instructions" << std::endl;
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
