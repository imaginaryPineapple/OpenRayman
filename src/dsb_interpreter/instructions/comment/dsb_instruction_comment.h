#ifndef DSB_INSTRUCTION_COMMENT_H
#define DSB_INSTRUCTION_COMMENT_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    struct dsb_instruction_comment : public dsb_instruction
    {
        dsb_instruction_comment(std::size_t line, const std::string& comment) :
            dsb_instruction(dsb_instruction_type::comment),
            line(line), comment(comment)
        {
        }

        // The line that the interpreter encountered the comment.
        std::size_t line;

        // The user specified comment that was found.
        std::string comment;
    };
}

#endif
