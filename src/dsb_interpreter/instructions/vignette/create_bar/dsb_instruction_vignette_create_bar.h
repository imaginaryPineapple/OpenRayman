#ifndef DSB_INSTRUCTION_VIGNETTE_CREATE_BAR_H
#define DSB_INSTRUCTION_VIGNETTE_CREATE_BAR_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    struct dsb_instruction_vignette_create_bar : public dsb_instruction
    {
        dsb_instruction_vignette_create_bar(int x, int y, int w, int h) :
            dsb_instruction(dsb_instruction_type::vignette_create_bar),
            x(x), y(y), w(w), h(h)
        {
        }

        int x, y, w, h;
    };
}

#endif
