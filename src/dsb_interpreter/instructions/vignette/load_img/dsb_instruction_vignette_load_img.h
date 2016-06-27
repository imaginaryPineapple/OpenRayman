#ifndef DSB_INSTRUCTION_VIGNETTE_LOAD_IMG_H
#define DSB_INSTRUCTION_VIGNETTE_LOAD_IMG_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    struct dsb_instruction_vignette_load_img : public dsb_instruction
    {
        dsb_instruction_vignette_load_img(const std::string& image) :
            dsb_instruction(dsb_instruction_type::vignette_load_img),
            image(image)
        {
        }

        std::string image;
    };
}

#endif
