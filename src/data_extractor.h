#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H

#include <string>
#include <platform/backend_specifics.h>

namespace openrayman
{
    class cnt_directory_node;

    // Extracts data from a valid Rayman 2: The Great Escape installation.
    class data_extractor
    {
public:
        data_extractor(const backend_specifics& backend_specifics) :
            m_backend_specifics(backend_specifics)
        {
        }

        // Starts the extraction process.
        // Returns true if the extraction succeeded.
        bool extract(const std::string& install_folder);
private:
        bool check_prerequisites(const std::string& install_folder);
        bool create_base();
        bool decompile_game_dsb(const std::string& install_folder);
        bool extract_recursive(std::string base, cnt_directory_node& parent);
        bool make_game_resources(const std::string& install_folder);

        const backend_specifics& m_backend_specifics;
    };
}

#endif
