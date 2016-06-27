#ifndef DSB_INSTRUCTION_VIGNETTE_SET_COLOR_H
#define DSB_INSTRUCTION_VIGNETTE_SET_COLOR_H

#include <dsb_interpreter/dsb_instruction.h>
#include <cstdint>

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
        // TODO: refactor this..........
        // ugh
        dsb_instruction_vignette_set_color(dsb_vignette_id id,
            std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) :
            dsb_instruction(dsb_instruction_type::vignette_set_color),
            r(r), g(g), b(b), a(a)
        {
        }

        // The id of the color to set.
        dsb_vignette_id id;

        // The first pair of colors.
        std::uint8_t r, g, b, a;

        // The second pair of colors.
        // Only used when id is bar, to specify gradient.
        std::uint8_t pair2_r, pair2_g, pair2_b, pair2_a;

        // The third pair of colors.
        // Only used when id is bar, to specify gradient.
        std::uint8_t pair3_r, pair3_g, pair3_b, pair3_a;

        // The fourth pair of colors.
        // Only used when id is bar, to specify gradient.
        std::uint8_t pair4_r, pair4_g, pair4_b, pair4_a;
    };
}

#endif
