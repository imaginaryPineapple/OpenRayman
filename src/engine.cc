#include <cstdlib>
#include <iostream>
#include <engine.h>
#include <platform/message_box.h>

int openrayman::engine::run(const std::string& game)
{
    openrayman::message_box::display("Test title", "This is a test of the message_box class.\nMultilines are supported.", false);
    return EXIT_SUCCESS;
}
