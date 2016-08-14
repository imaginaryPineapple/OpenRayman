#ifndef DATA_EXTRACTOR_DIALOG_PLATFORM_H
#define DATA_EXTRACTOR_DIALOG_PLATFORM_H

#ifdef _WIN32
#include <platform/data_extractor/windows.h>
#endif
#ifdef __linux__
#include <platform/data_extractor/gtk.h>
#endif

namespace openrayman
{
#ifdef _WIN32
	using data_extractor_dialog = data_extractor_dialog_windows;
#endif
#ifdef __linux__
	using data_extractor_dialog = data_extractor_dialog_gtk;
#endif
}

#endif
