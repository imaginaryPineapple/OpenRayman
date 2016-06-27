#include <dsb_interpreter/dsb_interpreter.h>
#include <dsb_interpreter/dsb_interpreter_string_utils.h>
#include <dsb_interpreter/instructions/all.h>
#include <platform/file.h>
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
        std::string function_name = read_until_char(line, n, '(');
        n += function_name.length() + 1;
        std::vector<std::string> function_args;
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
                function_args.push_back(arg);
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
            function_args.push_back(arg);
        std::cout << "[openrayman::dsb_interpreter] Function: \"" << function_name << "\", " << function_args.size() << std::endl;
        for(const std::string& str : function_args)
            std::cout << "                              Argument: \"" << str << "\"" << std::endl;
    }
}
