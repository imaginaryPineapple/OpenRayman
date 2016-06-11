#include <cstdlib>
#include <iostream>
#include <info.h>
#include <engine.h>
#include <platform/message_box.h>

namespace openrayman
{
    int engine::run(const std::string& game)
    {
        return EXIT_SUCCESS;
    }

    void engine::exit()
    {
        m_exit_requested = true;
    }
}
