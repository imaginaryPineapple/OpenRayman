#include <info.h>
#include <engine.h>
#include <openrayman/dsb_script.h>
#include <tools/common_tools.h>
#include <tools/gf_tools.h>
#include <tools/cnt_tools.h>
#include <vector>
#include <iostream>
#include <platform/file.h>
#include <platform/standalone_backend_specifics.h>

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
			if(format == "dsb")
			{
                std::ifstream file(path);
				openrayman::dsb_script script(file);
                if(!script.valid())
                    return fail_and_print("Operation failed");
				if(script.decompile(
					target))
				{
					return EXIT_SUCCESS;
				}
				else
					return fail_and_print("Operation failed");
			}
			else if(format == "png")
				return openrayman::gf_tools::convert_to_png(path, target);
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
			if(format == "cnt")
				return openrayman::cnt_tools::print_hierarchy(path);
			else if(format == "gf")
				return openrayman::gf_tools::print_info(path);
			return fail_and_print("Invalid format was specified");
		}
		if(str == "--force-reset-base-rayman2")
		{
			openrayman::standalone_backend_specifics backend_specifics;
			openrayman::file::delete_directory(backend_specifics.storage_path() + "/games/rayman2");
		}
		if(str == "--extract-cnt-to")
		{
			n++;
			if(n >= argc)
				return fail_and_print("No archive was specified");
			std::string archive(argv[n]);
			n++;
			if(n >= argc)
				return fail_and_print("No path was specified");
			std::string path(argv[n]);
			n++;
			if(n >= argc)
				return fail_and_print("No target was specified");
			std::string target(argv[n]);
			return openrayman::cnt_tools::extract(archive, path, target);
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
            std::cout << "  --convert-data \"install folder\"              Converts the install folder specified into formats that OpenRayman (and humans) can read" << std::endl;
            std::cout << "                                               This is needed to ease modding support" << std::endl;
            std::cout << "                                               These will be created in the storage path (in windows %appdata%\\openrayman or linux ~/.local/share/openrayman)" << std::endl;
			std::cout << "                                               This can also be done by starting the game without extracted data" << std::endl;
            std::cout << "                                               In that case, you will get a directory picker for the install folder" << std::endl;
			std::cout << "  --convert-to \"format\" \"path\" \"target\"        Converts the specified file into the target format" << std::endl;
			std::cout << "                                                   \"dsb\": Decompiles the DSB into the specified directory (several files will be created)" << std::endl;
			std::cout << "                                                   \"png\": Decodes a GF (graphics texture) file into a png file" << std::endl;
			std::cout << "  --inspect \"format\" \"path\"                    Inspects and prints info about the specified file or directory" << std::endl;
			std::cout << "                                                   \"cnt\": Prints file hierarchy" << std::endl;
			std::cout << "                                                   \"gf\": Prints width, height and number of channels" << std::endl;
			std::cout << "  --extract-cnt-to \"archive\" \"path\" \"target\"   Extracts a file from a CNT archive" << std::endl;
			std::cout << "  --force-reset-base-rayman2                   Forces a removal of the rayman2 base game, if it exists" << std::endl;
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
    std::cout << "Running on " << openrayman::map_platform(openrayman::this_platform) << std::endl;
    std::cout << "Using game " << (selected_game == "" ? "from config" : "\"" + selected_game + "\"") << std::endl;
    openrayman::engine engine;
    return engine.run(selected_game, selected_install_folder);
}

#ifdef _WIN32

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	return main(__argc, __argv);
}

#endif
