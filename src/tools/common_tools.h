#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <string>

// Common tools used by main.cc and other tools

static bool console_open = false;

inline void make_sure_console_open()
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

inline int fail_and_print(const std::string& msg)
{
	make_sure_console_open(); std::cout << msg << std::endl;
	return EXIT_FAILURE;
}

#endif
