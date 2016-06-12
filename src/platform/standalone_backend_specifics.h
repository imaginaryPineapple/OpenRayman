#ifndef STANDALONE_BACKEND_SPECIFICS_H
#define STANDALONE_BACKEND_SPECIFICS_H

#include <platform/backend_specifics.h>
#include <string>

namespace openrayman
{
    // Implements backend specifics when the game is run standalone.
    class standalone_backend_specifics : public backend_specifics
    {
public:
        standalone_backend_specifics() : m_data_path(nullptr), m_storage_path(nullptr) { }

        ~standalone_backend_specifics()
        {
            if(m_data_path != nullptr)
                delete m_data_path;
            if(m_storage_path != nullptr)
                delete m_storage_path;
        }

        const std::string& get_data_path() override;
        const std::string& get_storage_path() override;

        double get_time() override;
private:
        std::string* m_data_path;
        std::string* m_storage_path;
    };
}

#endif
