#ifndef BACKEND_SPECIFICS_H
#define BACKEND_SPECIFICS_H

#include <string>

namespace openrayman
{
    // Provides access several methods that are backend and/or platform specific.
    class backend_specifics
    {
public:
        virtual ~backend_specifics() { }

        // Get the path where data files reside. This should be assumed to be read-only.
        virtual const std::string& get_data_path() = 0;

        // Get the path where files should be stored.
        virtual const std::string& get_storage_path() = 0;

        // Returns a high-resolution timer value in seconds.
        virtual double get_time() = 0;
    };
}

#endif
