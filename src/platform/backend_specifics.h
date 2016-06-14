#ifndef BACKEND_SPECIFICS_H
#define BACKEND_SPECIFICS_H

#include <string>
#include <cstdint>

namespace openrayman
{
    // Provides access to several methods that are backend and/or platform specific.
    class backend_specifics
    {
public:
        virtual ~backend_specifics() { }

        // Returns the path where data files reside. This is be assumed to be read-only.
        virtual const std::string& get_data_path() const = 0;

        // Returns the path where files should be stored.
        virtual const std::string& get_storage_path() const = 0;

        // Returns a high-resolution timer value in seconds.
        virtual double get_time() const = 0;

        // Yields CPU using the most efficient method possible.
        virtual void yield_cpu() const = 0;
    };
}

#endif
