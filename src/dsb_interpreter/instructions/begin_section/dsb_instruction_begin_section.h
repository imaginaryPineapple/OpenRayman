#ifndef DSB_INSTRUCTION_BEGIN_SECTION_H
#define DSB_INSTRUCTION_BEGIN_SECTION_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    struct dsb_instruction_begin_section : public dsb_instruction
    {
        dsb_instruction_begin_section(dsb_section_type end, dsb_section_type begin) :
            dsb_instruction(dsb_instruction_type::begin_section),
            end(end), begin(begin)
        {
        }

        // The previous section that is ending.
        dsb_section_type end;

        // The section that is following this instruction.
        dsb_section_type begin;
    };
}

#endif
