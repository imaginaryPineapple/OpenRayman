#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <string>

namespace openrayman
{
    // Simple multi platform abstraction for a message box.
    class message_box
    {
public:
        // Displays a blocking message box.
        static void display(const std::string& title, const std::string& msg, bool error);

        // Displays a blocking message box and returns true if the user selected "Yes".
        static bool display_yesno(const std::string& title, const std::string& msg, bool error);

        // Displays a blocking directory picker and returns the name of the selected directory or "".
        static const std::string display_pickdir(const std::string& title);
    };
}

#endif
