#ifndef DSB_INTERPRETER_DEBUGGER_H
#define DSB_INTERPRETER_DEBUGGER_H

#include <dsb_interpreter/dsb_interpreter.h>

namespace openrayman
{
    // Used for printing some information about an interpreted DSB.
    class dsb_interpreter_debugger
    {
public:
        dsb_interpreter_debugger(const dsb_interpreter& interpreter) :
            m_interpreter(interpreter)
        {
        }

        void print_summary() const;
private:
        const dsb_interpreter& m_interpreter;
    };
}

#endif
