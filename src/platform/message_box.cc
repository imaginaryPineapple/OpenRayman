#include <platform/message_box.h>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <gtk/gtk.h>
#endif

namespace openrayman
{
    bool message_box::gtk_has_init = false;
    bool message_box::com_has_init = false;

    void message_box::display(const std::string& title, const std::string& msg, bool error)
    {
        // Print to stdout first, then show a GUI message box.
        std::cout <<
            std::endl << "#################" << std::endl << std::endl <<
            title << std::endl << std::endl << msg << std::endl <<
            std::endl << "#################" << std::endl << std::endl;
#ifdef _WIN32
        MessageBox(nullptr, msg.c_str(), title.c_str(), MB_OK | (error ? MB_ICONERROR : MB_ICONINFORMATION));
#else
        if(!gtk_has_init)
            gtk_has_init = gtk_init_check(0, nullptr);
        if(gtk_has_init)
        {
            GtkDialogFlags flags = (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_USE_HEADER_BAR);
            GtkWidget* dialog = gtk_message_dialog_new(nullptr, flags, error ? GTK_MESSAGE_ERROR : GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg.c_str());
            gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_hide(dialog);
            gtk_widget_destroy(dialog);
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
#else
        if(!gtk_has_init)
            gtk_has_init = gtk_init_check(0, nullptr);
        if(gtk_has_init)
        {
            GtkDialogFlags flags = (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_USE_HEADER_BAR);
            GtkWidget* dialog = gtk_message_dialog_new(nullptr, flags, error ? GTK_MESSAGE_ERROR : GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, msg.c_str());
            gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
            gint result = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_hide(dialog);
            gtk_widget_destroy(dialog);
            return result == GTK_RESPONSE_YES;
        }
#endif
        char in;
        std::cin >> in;
        return in == 'Y' || in == 'y';
    }

    const std::string message_box::display_pickdir(const std::string& title)
    {
#ifdef _WIN32
        if(!com_has_init)
        {
            CoInitialize(nullptr);
            com_has_init = true;
        }
        if(com_has_init)
        {
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
        }
#else
        if(!gtk_has_init)
            gtk_has_init = gtk_init_check(0, nullptr);
        if(gtk_has_init)
        {
            GtkWidget* dialog = gtk_file_chooser_dialog_new(title.c_str(), nullptr, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                "Cancel", GTK_RESPONSE_CANCEL,
                "Open", GTK_RESPONSE_ACCEPT,
                nullptr);
            gint result = gtk_dialog_run(GTK_DIALOG(dialog));
            if(result == GTK_RESPONSE_ACCEPT)
            {
                char* directory_cstr = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                std::string directory = std::string(directory_cstr);
                g_free(directory_cstr);
                gtk_widget_hide(dialog);
                gtk_widget_destroy(dialog);
                return directory;
            }
            gtk_widget_hide(dialog);
            gtk_widget_destroy(dialog);
        }
#endif
        return "";
    }
}
