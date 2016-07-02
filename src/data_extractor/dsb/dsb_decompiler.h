#ifndef DSB_DECOMPILER_H
#define DSB_DECOMPILER_H

#include <data_extractor/data_decoder.h>
#include <string>
#include <iostream>
#include <fstream>

namespace openrayman
{
    enum class dsb_format
    {
        // The default format.
        // This is a more human readable/editable format than the binary format used by Rayman 2.
        // This can be interpreted by the dsb engine.
        // Creates several files such as vignette.json, levels.json
        openrayman,

        // The format used by the Rayman 2: The Great Escape engine.
        // This is a binary format.
        // This can not be interpreted by OpenRayman.
        // The only operation that is performed by the dsb decompiler when this format is specified
        // is to decode the dsb.
        // Uses the extension .rdsb
        rayman2_decoded
    };

    // Decodes and decompiles DSB files.
    // DSB files are compiled scripts, with different sections.
    // This is all heavily adapted from Rayman2Lib.
    class dsb_decompiler
    {
public:
        // Decodes and optionally decompiles the specified DSB file into several files in the target directory.
        // This function assumes that the source file exists and that the target directory is accessible.
        // Returns true if the function succeeded.
        bool decompile_dsb(const std::string& source, const std::string& target, dsb_format fmt);
private:
        void decompile_sections(char* source, std::size_t source_length, const std::string& target);
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

        data_decoder m_decoder;
    };
}

#endif
