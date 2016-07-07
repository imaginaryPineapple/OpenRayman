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
        standalone_backend_specifics();

        ~standalone_backend_specifics()
        {
            if(m_data_path != nullptr)
                delete m_data_path;
            if(m_storage_path != nullptr)
                delete m_storage_path;
        }

        const std::string& data_path() const override;
        const std::string& storage_path() const override;

        double time() const override;

        void yield_cpu() const override;
private:
        void initialize_data_path();
        void initialize_storage_path();
        std::string* m_data_path;
        std::string* m_storage_path;
    };
}

#endif
