#ifndef DSB_INSTRUCTION_INVALID_DSB_H
#define DSB_INSTRUCTION_INVALID_DSB_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    struct dsb_instruction_invalid_dsb : public dsb_instruction
    {
        dsb_instruction_invalid_dsb(std::size_t line, std::size_t column, const std::string& error, const std::string& trace) :
            dsb_instruction(dsb_instruction_type::invalid_dsb),
            line(line), column(column), error(error), trace(trace)
        {
        }

        // The line that the interpreter encountered the error.
        std::size_t line;

        // The column that the interpreter encountered the error.
        std::size_t column;

        // The error that the interpreter encountered.
        std::string error;

        // Trace info about the error.
        std::string trace;
    };
}

#endif
