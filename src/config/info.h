#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include <string>
#include <platform/backend_specifics.h>
#include <json.hpp>

namespace openrayman
{
    // Static info file for the engine using JSON internally.
    class info
    {
public:
        info(const backend_specifics& backend_specifics);

        // If the file was loaded successfully.
        inline bool loaded_successfully() const
        {
            return m_loaded;
        }

        // The default game to load.
        std::string game;

        // The base canvas size.
        int base_w, base_h;

        // The number of fixed updates per second.
        double updates_per_second;
private:
        const backend_specifics& m_backend_specifics;
        bool m_loaded;
    };
}

#endif
