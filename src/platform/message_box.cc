#include <platform/message_box.h>
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <gtkmm.h>
#include <platform/gtk.h>
#endif
#include <iostream>

namespace openrayman
{
    void message_box::display(const std::string& title, const std::string& msg, bool error)
    {
        // Print to stdout first, then show a GUI message box.
        std::cout <<
            std::endl << "#################" << std::endl << std::endl <<
            title << std::endl << std::endl << msg << std::endl <<
            std::endl << "#################" << std::endl << std::endl;
#ifdef _WIN32
        MessageBox(nullptr, msg.c_str(), title.c_str(), MB_OK | (error ? MB_ICONERROR : MB_ICONINFORMATION));
#endif
#ifdef __linux__
		if(can_use_gtk())
		{
			Gtk::MessageDialog dialog(msg,
				false,
				error ? Gtk::MessageType::MESSAGE_ERROR : Gtk::MessageType::MESSAGE_INFO,
				Gtk::ButtonsType::BUTTONS_OK,
				true);
			dialog.set_title(title);
			dialog.run();
		}
#endif
    }

    bool message_box::display_yesno(const std::string& title, const std::string& msg, bool error)
    {
        // Print to stdout first, then show a GUI message box.
        std::cout <<
            std::endl << "#################" << std::endl << std::endl <<
            title << std::endl << std::endl << msg << std::endl << "(Y/N)" << std::endl <<
            std::endl << "#################" << std::endl << std::endl;
#ifdef _WIN32
        return MessageBox(nullptr, msg.c_str(), title.c_str(), MB_YESNO | (error ? MB_ICONERROR : MB_ICONINFORMATION)) == IDYES;
#endif
#ifdef __linux__
		if(can_use_gtk())
		{
			Gtk::MessageDialog dialog(msg,
				false,
				error ? Gtk::MessageType::MESSAGE_ERROR : Gtk::MessageType::MESSAGE_INFO,
				Gtk::ButtonsType::BUTTONS_YES_NO,
				true);
			dialog.set_title(title);
			return dialog.run() == Gtk::ResponseType::RESPONSE_YES;
		}
#endif
        char in;
        std::cin >> in;
        return in == 'Y' || in == 'y';
    }

    const std::string message_box::display_pickdir(const std::string& title)
    {
#ifdef _WIN32
        BROWSEINFO binfo = {0};
        binfo.lpszTitle = title.c_str();
        binfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
        binfo.lpfn = nullptr;
        LPITEMIDLIST pidl = SHBrowseForFolder(&binfo);
        if(pidl != nullptr)
        {
            char str[MAX_PATH];
            SHGetPathFromIDList(pidl, str);
            return std::string(str);
        }
#endif
#ifdef __linux__
		if(can_use_gtk())
		{
			Gtk::FileChooserDialog dialog(title, Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SELECT_FOLDER);
			dialog.add_button("Cancel", Gtk::ResponseType::RESPONSE_CANCEL);
			dialog.add_button("Open", Gtk::ResponseType::RESPONSE_ACCEPT);
			if(dialog.run() == GTK_RESPONSE_ACCEPT)
				return dialog.get_filename();
		}
#endif
        return "";
    }
}
