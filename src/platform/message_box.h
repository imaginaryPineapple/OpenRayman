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
private:
        static bool gtk_has_init;
    };
}

#endif
