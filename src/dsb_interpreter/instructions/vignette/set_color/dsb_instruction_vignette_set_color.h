#ifndef DSB_INSTRUCTION_VIGNETTE_SET_COLOR_H
#define DSB_INSTRUCTION_VIGNETTE_SET_COLOR_H

#include <dsb_interpreter/dsb_instruction.h>
#include <cstdint>
#include <glm/glm.hpp>

namespace openrayman
{
    enum class dsb_vignette_id
    {
        none,
        // TODO: document these
        outline,
        inside,
        bar
    };

    inline std::string vignette_id_to_string(dsb_vignette_id id)
    {
    #define VALID_ID_TO(name) \
        if(id == dsb_vignette_id::name) \
            return #name;

        VALID_ID_TO(outline);
        VALID_ID_TO(inside);
        VALID_ID_TO(bar);

        return "none";
    }

    inline dsb_vignette_id string_to_vignette_id(const std::string& id)
    {
    #define VALID_ID_FROM(name) \
        if(id == #name) \
            return dsb_vignette_id::name;

        VALID_ID_FROM(outline);
        VALID_ID_FROM(inside);
        VALID_ID_FROM(bar);

        return dsb_vignette_id::none;
    }

    struct dsb_instruction_vignette_set_color : public dsb_instruction
    {
        dsb_instruction_vignette_set_color(dsb_vignette_id id,
            glm::vec4 color, glm::vec4 color2, glm::vec4 color3, glm::vec4 color4) :
            dsb_instruction(dsb_instruction_type::vignette_set_color),
            color(color), color2(color2), color3(color3), color4(color4)
        {
        }

        // The id of the color to set.
        dsb_vignette_id id;

        // Color gradient, all are only used when id is "bar".
        glm::vec4 color, color2, color3, color4;
    };
}

#endif
