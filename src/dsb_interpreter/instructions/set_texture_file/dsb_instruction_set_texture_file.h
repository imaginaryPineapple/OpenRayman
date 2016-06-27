#ifndef DSB_INSTRUCTION_SET_TEXTURE_FILE_H
#define DSB_INSTRUCTION_SET_TEXTURE_FILE_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    enum class dsb_texture_file_id
    {
        // Stores vignettes.
        vignettes,

        // Stores all other (world, etc) textures.
        textures
    };

    struct dsb_instruction_set_texture_file : public dsb_instruction
    {
        dsb_instruction_set_texture_file(dsb_texture_file_id id, const std::string& archive) :
            dsb_instruction(dsb_instruction_type::set_texture_file),
            id(id), archive(archive)
        {
        }

        // The texture file to set.
        dsb_texture_file_id id;

        // The archive which it should be set to.
        std::string archive;
    };
}

#endif
