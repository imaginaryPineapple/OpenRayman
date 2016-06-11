#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <string>

namespace openrayman
{
    // Simple platform abstraction for a message box.
    class message_box
    {
public:
        // Displays a message box and blocks.
        static void display(const std::string& title, const std::string& msg, bool error);
    };
}

#endif
