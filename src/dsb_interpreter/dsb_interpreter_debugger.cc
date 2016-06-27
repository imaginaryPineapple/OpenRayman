#include <dsb_interpreter/dsb_interpreter_debugger.h>
#include <dsb_interpreter/dsb_instruction.h>
#include <dsb_interpreter/instructions/all.h>
#include <iostream>

namespace openrayman
{
    void dsb_interpreter_debugger::print_summary() const
    {
        if(m_interpreter.success())
        {
            std::cout << std::endl << "DSB interpreted -> "
                << m_interpreter.get_complete_instruction_array().size() << " instructions emitted" << std::endl << std::endl;
            for(dsb_instruction* instruction : m_interpreter.get_complete_instruction_array())
            {
                std::cout << "    dsb_instruction_type::" << instruction_to_string(instruction->type) << std::endl;
                switch(instruction->type)
                {
                    case dsb_instruction_type::comment:
                    {
                        dsb_instruction_comment* cast = static_cast<dsb_instruction_comment*>(instruction);
                        std::cout << "        comment -> \"" << cast->comment << "\"" << std::endl;
                        break;
                    }
                    case dsb_instruction_type::begin_section:
                    {
                        dsb_instruction_begin_section* cast = static_cast<dsb_instruction_begin_section*>(instruction);
                        std::cout << "        end -> dsb_section_type::" << section_to_string(cast->end) << std::endl;
                        std::cout << "        begin -> dsb_section_type::" << section_to_string(cast->begin) << std::endl;
                        break;
                    }
                    case dsb_instruction_type::set_data_dir:
                    {
                        dsb_instruction_set_data_dir* cast = static_cast<dsb_instruction_set_data_dir*>(instruction);
                        std::cout << "        directory -> dsb_data_directory::" << data_directory_to_string(cast->directory) << std::endl;
                        std::cout << "        path -> \"" << cast->path << "\"" << std::endl;
                        break;
                    }
                    case dsb_instruction_type::set_texture_file:
                    {
                        dsb_instruction_set_texture_file* cast = static_cast<dsb_instruction_set_texture_file*>(instruction);
                        std::cout << "        id -> dsb_texture_file_id::" << (cast->id == dsb_texture_file_id::vignettes ? "vignettes" : "textures") << std::endl;
                        std::cout << "        archive -> \"" << cast->archive << "\"" << std::endl;
                        break;
                    }
                    case dsb_instruction_type::vignette_load_img:
                    {
                        dsb_instruction_vignette_load_img* cast = static_cast<dsb_instruction_vignette_load_img*>(instruction);
                        std::cout << "        image -> \"" << cast->image << "\"" << std::endl;
                        break;
                    }
                    case dsb_instruction_type::vignette_set_color:
                    {
                        dsb_instruction_vignette_set_color* cast = static_cast<dsb_instruction_vignette_set_color*>(instruction);
                        std::cout << "        id -> dsb_vignette_id::" << vignette_id_to_string(cast->id) << std::endl;
                        std::cout << "        pair1 -> [" << cast->r << ", " << cast->g << ", " << cast->b << ", " << cast->a << "]" << std::endl;
                        std::cout << "        pair2 -> [" << cast->pair2_r << ", " << cast->pair2_g << ", " << cast->pair2_b << ", " << cast->pair2_a << "]" << std::endl;
                        std::cout << "        pair3 -> [" << cast->pair3_r << ", " << cast->pair3_g << ", " << cast->pair3_b << ", " << cast->pair3_a << "]" << std::endl;
                        std::cout << "        pair4 -> [" << cast->pair4_r << ", " << cast->pair4_g << ", " << cast->pair4_b << ", " << cast->pair4_a << "]" << std::endl;
                        break;
                    }
                    case dsb_instruction_type::vignette_create_bar:
                    {
                        dsb_instruction_vignette_create_bar* cast = static_cast<dsb_instruction_vignette_create_bar*>(instruction);
                        std::cout << "        pos -> [" << cast->x << ", " << cast->y << ", " << cast->w << ", " << cast->h << "]" << std::endl;
                        break;
                    }
                    case dsb_instruction_type::level_add:
                    {
                        dsb_instruction_level_add* cast = static_cast<dsb_instruction_level_add*>(instruction);
                        std::cout << "        level -> \"" << cast->level << "\"" << std::endl;
                        break;
                    }
                }
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "Operation failed" << std::endl;
            dsb_instruction_invalid_dsb* error_reason = m_interpreter.get_instruction<dsb_instruction_invalid_dsb>
                (dsb_instruction_type::invalid_dsb);
            std::cout << "At " << error_reason->line << " : " << error_reason->column << std::endl;
            std::cout << error_reason->error << std::endl;
            std::cout << error_reason->trace << std::endl;
        }
    }
}
