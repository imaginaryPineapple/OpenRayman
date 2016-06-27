#ifndef DSB_INSTRUCTION_LEVEL_ADD_H
#define DSB_INSTRUCTION_LEVEL_ADD_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    struct dsb_instruction_level_add : public dsb_instruction
    {
        dsb_instruction_level_add(const std::string& level) :
            dsb_instruction(dsb_instruction_type::level_add),
            level(level)
        {
        }

        // The level name, relative to the levels folder.
        std::string level;
    };
}

#endif
