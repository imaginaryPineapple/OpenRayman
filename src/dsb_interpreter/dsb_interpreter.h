#ifndef DSB_INTERPRETER_H
#define DSB_INTERPRETER_H

#include <dsb_interpreter/dsb_instruction.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace openrayman
{
    // DSB interpreter.
    // Builds an array of instructions from a valid odsb file.
    class dsb_interpreter
    {
public:
        dsb_interpreter(const std::string& odsb_path);

        ~dsb_interpreter()
        {
            for(dsb_instruction* instruction : m_interpreted_dsb)
                delete instruction;
        }

        // If this DSB was interpreted successfully.
        inline bool success() const
        {
            return section_count(dsb_instruction_type::invalid_dsb) == 0;
        }

        // Returns the number of times that the specified section type occurs in this DSB.
        inline std::size_t section_count(dsb_instruction_type type) const
        {
            std::size_t count = 0;
            for(dsb_instruction* instruction : m_interpreted_dsb)
            {
                if(instruction->type == type)
                    count++;
            }
            return count;
        }

        // Returns the first instance of the specified instruction type, or nullptr if one doesn't exist.
        template<typename T>
        inline T* get_instruction(dsb_instruction_type type) const
        {
            for(dsb_instruction* instruction : m_interpreted_dsb)
            {
                if(instruction->type == type)
                    return static_cast<T*>(instruction);
            }
            return nullptr;
        }

        // Returns all instances of the specified instruction type.
        template<typename T>
        inline std::vector<T*> get_instructions(dsb_instruction_type type) const
        {
            std::vector<T*> values;
            for(dsb_instruction* instruction : m_interpreted_dsb)
            {
                if(instruction->type == type)
                    values.push_back(static_cast<T*>(instruction));
            }
            return values;
        }

        // Returns a reference to the raw instruction array.
        // This can be manually searched and interpreted.
        inline const std::vector<dsb_instruction*>& get_complete_instruction_array() const
        {
            return m_interpreted_dsb;
        }
private:
        void interpret_sections(std::ifstream& stream);
        void interpret_line(dsb_section_type current_section, const std::string& line, std::size_t statement_begins_at, std::size_t line_n);
        void interpret_instruction(dsb_section_type current_section, const std::string& name, const std::vector<std::string>& args, std::size_t func_begins_at, std::size_t line_n);

        // Sets the error at the top of the dsb instruction array.
        void set_error(std::size_t line, std::size_t column, const std::string& error, dsb_section_type current_section, const std::string& function);

        std::vector<dsb_instruction*> m_interpreted_dsb;
    };
}

#endif
