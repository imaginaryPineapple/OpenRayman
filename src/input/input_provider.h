#ifndef INPUT_PROVIDER_H
#define INPUT_PROVIDER_H

#include <input/input_state.h>
#include <string>

namespace openrayman
{
    // Pollable input backend.
    // May provide input from multiple actual sources.
    class input_provider
    {
public:
        virtual ~input_provider() { };

        // Returns a basic description (e.g. name) of this input provider.
        virtual const std::string get_description() const = 0;

        // Returns and polls input at the current point in time.
        virtual const input_state& poll() = 0;
    };
}

#endif
