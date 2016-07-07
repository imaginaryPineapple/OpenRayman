#ifndef OPENRAYMAN_DSB_SCRIPT_H
#define OPENRAYMAN_DSB_SCRIPT_H

#include <openrayman/common_stream.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

namespace openrayman
{
    // DSB files are compiled scripts, with different sections.
    class dsb_script
    {
public:
        dsb_script(std::istream& stream);

        // Returns true if the stream data was accessible.
        inline bool valid() const
        {
            return m_valid;
        }

        // Decodes and decompiles this script into several files in the target directory.
        // This function assumes that the target directory is accessible.
        // Returns true if the function succeeded.
        bool decompile(const std::string& target);
private:
        void decompile_alloc(std::istream& source, const std::string& target);
        void decompile_lvl_list(std::istream& source, const std::string& target);
        void decompile_data_directories(std::istream& source, const std::string& target);
        void decompile_unknown_blob_0x20(std::istream& source, const std::string& target);
        void decompile_vignette(std::istream& source, const std::string& target);
        void decompile_texture_files(std::istream& source, const std::string& target);
        void decompile_unknown_blob_0x6e(std::istream& source, const std::string& target);
        void decompile_game_options(std::istream& source, const std::string& target);
        void decompile_sound_banks(std::istream& source, const std::string& target);
        void decompile_load_sound_banks(std::istream& source, const std::string& target);

        // Reads a null-terminated string from source.
        const std::string read_string_null_terminated(std::istream& source);

        // Reads the length of the string from source.
        const std::string read_string_with_length_u16(std::istream& source);

        bool m_valid;
        std::unique_ptr<common::encoded_buf> m_buf;
        std::istream m_stream;
    };
}

#endif
