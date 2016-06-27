#include <dsb_interpreter/dsb_interpreter.h>
#include <dsb_interpreter/dsb_interpreter_string_utils.h>
#include <dsb_interpreter/instructions/all.h>
#include <platform/file.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <sstream>
#include <iomanip>

namespace openrayman
{
    dsb_interpreter::dsb_interpreter(const std::string& odsb_path)
    {
        std::ifstream stream(file::fix_string(odsb_path), std::ifstream::in);
        if(!stream.is_open())
        {
            set_error(0, 0, "The file \"" + odsb_path + "\" does not exist.", dsb_section_type::none, "dsb_interpreter::dsb_interpreter (constructor)");
            return;
        }
        interpret_sections(stream);
    }

    void dsb_interpreter::set_error(std::size_t line, std::size_t column, const std::string& error, dsb_section_type current_section, const std::string& function)
    {
        std::stringstream trace;
        trace << "Occured when section was: " << std::hex << "0x" << std::setfill('0') << std::setw(2) << (int)current_section << " (" << section_to_string(current_section) << ")" << "\n";
        trace << "Originated from function: " << function;
        m_interpreted_dsb.clear();
        m_interpreted_dsb.push_back(new dsb_instruction_invalid_dsb(line, column, error, trace.str()));
    }

    void dsb_interpreter::interpret_sections(std::ifstream& stream)
    {
        dsb_section_type current_section = dsb_section_type::none;
        std::string line;
        std::size_t line_n = 0;
        while(std::getline(stream, line))
        {
            if(m_interpreted_dsb.size() > 0 && m_interpreted_dsb[0]->type == dsb_instruction_type::invalid_dsb)
                return;
            std::size_t n = 0;
            n += skip_whitespace(line, n);
            // if the string wasn't all whitespace
            if(line.length() > 0 && n != line.length() - 1)
            {
                bool comment = false;
                if(line[n] == '#')
                {
                    n++;
                    n += skip_whitespace(line, n);
                    comment = true;
                }
                if(comment)
                {
                    std::string comment_string = read_until_end(line, n);
                    if(comment_string.length() > 0)
                        m_interpreted_dsb.push_back(new dsb_instruction_comment(line_n, comment_string));
                }
                else
                {
                    if(read_ahead_match(line, n, "section"))
                    {
                        n += sizeof("section");
                        n += skip_whitespace(line, n);
                        std::string section = read_until_end(line, n);
                        dsb_section_type new_section = string_to_section(section);

                        if(new_section == dsb_section_type::none)
                        {
                            set_error(line_n, n, "Invalid syntax (expected section type, got \"" + section + "\").", current_section, "dsb_interpreter::interpret_sections");
                            return;
                        }

                        m_interpreted_dsb.push_back(new dsb_instruction_begin_section(current_section, new_section));
                        current_section = new_section;
                    }
                    else if(current_section == dsb_section_type::none)
                    {
                        set_error(line_n, n, "Invalid syntax (expected section, got \"" + read_until_whitespace(line, n) + "\").", current_section, "dsb_interpreter::interpret_sections");
                        return;
                    }
                    else
                        interpret_line(current_section, line, n, line_n);
                }
            }
            line_n++;
        }
        // end of instruction array
        if(m_interpreted_dsb.size() == 0)
            set_error(0, 0, "No valid instructions could be interpreted from the specified file.", current_section, "dsb_interpreter::interpret_sections");
        else
            m_interpreted_dsb.push_back(new dsb_instruction_begin_section(current_section, dsb_section_type::none));
    }

    void dsb_interpreter::interpret_line(dsb_section_type current_section, const std::string& line, std::size_t statement_begins_at, std::size_t line_n)
    {
        std::size_t n = statement_begins_at;
        std::string instruction_name = read_until_char(line, n, '(');
        n += instruction_name.length() + 1;
        std::size_t instr_begins_at = n - 1;
        std::vector<std::string> instruction_args;
        n += skip_whitespace(line, n);
        std::string arg = "";
        while(n < line.length())
        {
            if(line[n] == '"')
            {
                arg = read_quote_escaped_string(line, n);
                n += arg.length() + 1;
            }
            else if(line[n] != ',' && line[n] != ')')
                arg += line[n];
            if(line[n] == ',')
            {
                instruction_args.push_back(arg);
                arg = "";
                n++;
                n += skip_whitespace(line, n);
            }
            else if(line[n] == ')')
                break;
            else
                n++;
        }
        if(arg.length() > 0)
            instruction_args.push_back(arg);
        interpret_instruction(current_section, instruction_name, instruction_args, instr_begins_at, line_n);
    }

    void dsb_interpreter::interpret_instruction(dsb_section_type current_section, const std::string& name, const std::vector<std::string>& args, std::size_t instr_begins_at, std::size_t line_n)
    {
        dsb_instruction* instr = nullptr;
        std::vector<float> args_float(args.size(), 0);
        std::stringstream stream;
        for(const std::string& arg : args)
        {
            stream << arg;
            float val = 0;
            stream >> val;
            args_float.push_back(val);
        }

    #define MAP_INSTRUCTION(section, from, to, min_num_args, ...) \
        if(current_section == section && name == from) \
        { \
            if(args.size() < min_num_args) \
            { \
                set_error(line_n, instr_begins_at, "Instruction " + name + " requires at least " + std::to_string(min_num_args) + " arguments.", current_section, "dsb_interpreter::interpret_instruction"); \
                return; \
            } \
            instr = new to(__VA_ARGS__); \
        }

    #define IGNORE_INSTRUCTION(section, instr_name) \
        if(current_section == section && name == instr_name) \
        { \
            std::cout << "[openrayman::dsb_interpreter] Warning! Ignored instruction with name " << instr_name << std::endl; \
            return; \
        }

        // dir
        MAP_INSTRUCTION(dsb_section_type::data_directories,
            "dir",
            dsb_instruction_set_data_dir,
            2, string_to_data_directory(args[0]), args[1]);

        // add
        MAP_INSTRUCTION(dsb_section_type::texture_files,
            "add",
            dsb_instruction_set_texture_file,
            2, args[0] == "vignettes" ? dsb_texture_file_id::vignettes : dsb_texture_file_id::textures, args[1]);

        // load_img
        MAP_INSTRUCTION(dsb_section_type::vignette,
            "load_img",
            dsb_instruction_vignette_load_img,
            1, args[0]);

        // color
        MAP_INSTRUCTION(dsb_section_type::vignette,
            "color",
            dsb_instruction_vignette_set_color,
            5, string_to_vignette_id(args[0]),
            glm::vec4(args_float[1] / 255, args_float[2] / 255, args_float[3] / 255, args_float[4] / 255),
            args.size() > 5 ? glm::vec4(args_float[5] / 255, args_float[6] / 255, args_float[7] / 255, args_float[8] / 255) : glm::vec4(0, 0, 0, 0),
            args.size() > 5 ? glm::vec4(args_float[9] / 255, args_float[10] / 255, args_float[11] / 255, args_float[12] / 255) : glm::vec4(0, 0, 0, 0),
            args.size() > 5 ? glm::vec4(args_float[13] / 255, args_float[14] / 255, args_float[15] / 255, args_float[16] / 255) : glm::vec4(0, 0, 0, 0));

        // create_bar
        MAP_INSTRUCTION(dsb_section_type::vignette,
            "create_bar",
            dsb_instruction_vignette_create_bar,
            4, args_float[0], args_float[1], args_float[2], args_float[3]);

        // level_add
        MAP_INSTRUCTION(dsb_section_type::levels,
            "add",
            dsb_instruction_level_add,
            1, args[0]);

        // TODO: some of these at least
        IGNORE_INSTRUCTION(dsb_section_type::alloc, "alloc");
        IGNORE_INSTRUCTION(dsb_section_type::alloc, "skip");
        IGNORE_INSTRUCTION(dsb_section_type::vignette, "bar_max");
        IGNORE_INSTRUCTION(dsb_section_type::vignette, "add_bar");
        IGNORE_INSTRUCTION(dsb_section_type::vignette, "display");
        IGNORE_INSTRUCTION(dsb_section_type::unknown_blob_0x20, "size");
        IGNORE_INSTRUCTION(dsb_section_type::unknown_blob_0x20, "id_encounter");
        IGNORE_INSTRUCTION(dsb_section_type::game_options, "default_file");
        IGNORE_INSTRUCTION(dsb_section_type::game_options, "current_file");
        IGNORE_INSTRUCTION(dsb_section_type::sound_banks, "unknown");
        IGNORE_INSTRUCTION(dsb_section_type::load_sound_banks, "add_bank");

        if(instr == nullptr)
            set_error(line_n, instr_begins_at, "Invalid instruction (got \"" + name + "\" in section " + section_to_string(current_section) + ").", current_section, "dsb_interpreter::interpret_instruction");
        else
            m_interpreted_dsb.push_back(instr);
    }
}
